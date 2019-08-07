/*
 * PRU Remote Processor Messaging Driver
 *
 * Copyright (C) 2015-2017 Texas Instruments Incorporated - http://www.ti.com/
 *	Jason Reeder <jreeder@ti.com>
 *	Suman Anna <s-anna@ti.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/rpmsg.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/kfifo.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/poll.h>
#include <linux/rpmsg/virtio_rpmsg.h>

#define PRU_MAX_DEVICES				(8)
// Matches the definition in virtio_rpmsg_bus.c 
#define RPMSG_BUF_SIZE				(512)
// Maximum rpmsg buffer size is 512 bytes of which 16 is consumed by the header. Therefore message length = 496 bytes.
#define MAX_FIFO_MSG				(32)
#define FIFO_MSG_SIZE				RPMSG_BUF_SIZE

/**
 * struct pruss_api_dev - Structure that contains the per-device data
 * @rpdev: rpmsg channel device that is associated with this pruss_api device
 * @dev: device
 * @cdev: character device
 * @locked: boolean used to determine whether or not the device file is in use
 * @devt: dev_t structure for the pruss_api device
 * @msg_fifo: kernel fifo used to buffer the messages between userspace and PRU
 * @msg_len: array storing the lengths of each message in the kernel fifo
 * @msg_idx_rd: kernel fifo read index
 * @msg_idx_wr: kernel fifo write index
 * @wait_list: wait queue used to implement the poll operation of the character
 *             device
 *
 * Each pruss_api device provides an interface, using an rpmsg channel (rpdev),
 * between a user space character device (cdev) and a PRU core. A kernel fifo
 * (msg_fifo) is used to buffer the messages in the kernel that are
 * being passed between the character device and the PRU.
 */
struct pruss_api_dev {
	struct rpmsg_device *rpdev;
	struct device *dev;
	struct cdev cdev;
	bool locked;
	dev_t devt;
	struct kfifo msg_fifo;
	u32 msg_len[MAX_FIFO_MSG];
	int msg_idx_rd;
	int msg_idx_wr;
	wait_queue_head_t wait_list;
};

static struct class *pruss_api_class;
static dev_t pruss_api_devt;
static DEFINE_MUTEX(pruss_api_lock);
static DEFINE_IDR(pruss_api_minors);

static int pruss_api_open(struct inode *inode, struct file *filp)
{
	struct pruss_api_dev *prudev;
	int ret = -EACCES;

	prudev = container_of(inode->i_cdev, struct pruss_api_dev, cdev);

	mutex_lock(&pruss_api_lock);
	if (!prudev->locked) {
		prudev->locked = true;
		filp->private_data = prudev;
		ret = 0;
	}
	mutex_unlock(&pruss_api_lock);

	if (ret)
		dev_err(prudev->dev, "Device already open\n");

	return ret;
}

static int pruss_api_release(struct inode *inode, struct file *filp)
{
	struct pruss_api_dev *prudev;

	prudev = container_of(inode->i_cdev, struct pruss_api_dev, cdev);
	mutex_lock(&pruss_api_lock);
	prudev->locked = false;
	mutex_unlock(&pruss_api_lock);
	return 0;
}

// pruss_api_read is called whenever we read from the character device file in the /dev/ directory. 
// i.e. when we execute `cat /dev/pruss_api_pru1`
// This function pulls messages from the kernel fifo and passes them to the user space i.e. cat.
static ssize_t pruss_api_read(struct file *filp, char __user *buf,
			      size_t count, loff_t *f_pos)
{
	int ret;
	u32 length;
	struct pruss_api_dev *prudev;

	prudev = filp->private_data;

	if (kfifo_is_empty(&prudev->msg_fifo) &&
	    (filp->f_flags & O_NONBLOCK))
		return -EAGAIN;

	ret = wait_event_interruptible(prudev->wait_list,
				       !kfifo_is_empty(&prudev->msg_fifo));
	if (ret)
		return -EINTR;

	ret = kfifo_to_user(&prudev->msg_fifo, buf,
			    prudev->msg_len[prudev->msg_idx_rd], &length);
	prudev->msg_idx_rd = (prudev->msg_idx_rd + 1) % MAX_FIFO_MSG;

	return ret ? ret : length;
}

// pruss_api_write is called whenever we write to the character device file in the /dev/ directory. 
// i.e. when we execute `echo "hello" | sudo tee /dev/pruss_api_pru1`
// This function takes the message and passes it to the PRU core over RPMsg
static ssize_t pruss_api_write(struct file *filp, const char __user *buf,
			       size_t count, loff_t *f_pos)
{
	int ret;
	struct pruss_api_dev *prudev;
	static char pruss_api_buf[RPMSG_BUF_SIZE];

	prudev = filp->private_data;

	if (count > RPMSG_BUF_SIZE - sizeof(struct rpmsg_hdr)) {
		dev_err(prudev->dev, "Data too large for RPMsg Buffer\n");
		return -EINVAL;
	}

	if (copy_from_user(pruss_api_buf, buf, count)) {
		dev_err(prudev->dev, "Error copying buffer from user space");
		return -EFAULT;
	}

	ret = rpmsg_send(prudev->rpdev->ept, (void *)pruss_api_buf, count);
	if (ret)
		dev_err(prudev->dev, "rpmsg_send failed: %d\n", ret);

	return ret ? ret : count;
}

static unsigned int pruss_api_poll(struct file *filp,
				   struct poll_table_struct *wait)
{
	int mask;
	struct pruss_api_dev *prudev;

	prudev = filp->private_data;

	poll_wait(filp, &prudev->wait_list, wait);

	mask = POLLOUT | POLLWRNORM;

	if (!kfifo_is_empty(&prudev->msg_fifo))
		mask |= POLLIN | POLLRDNORM;

	return mask;
}

static const struct file_operations pruss_api_fops = {
	.owner = THIS_MODULE,
	.open = pruss_api_open,
	.release = pruss_api_release,
	.read = pruss_api_read,
	.write = pruss_api_write,
	.poll = pruss_api_poll,
};

// This is the callback function that is called whenever the PRU core echoes a message to the ARM.
// The function takes the message and stores it in a fifo in the kernel.
static int pruss_api_cb(struct rpmsg_device *rpdev, void *data, int len,
			void *priv, u32 src)
{
	u32 length;
	struct pruss_api_dev *prudev;

	prudev = dev_get_drvdata(&rpdev->dev);

	if (kfifo_avail(&prudev->msg_fifo) < len) {
		dev_err(&rpdev->dev, "Not enough space on the FIFO\n");
		return -ENOSPC;
	}

	if ((prudev->msg_idx_wr + 1) % MAX_FIFO_MSG ==
		prudev->msg_idx_rd) {
		dev_err(&rpdev->dev, "Message length table is full\n");
		return -ENOSPC;
	}

	length = kfifo_in(&prudev->msg_fifo, data, len);
	prudev->msg_len[prudev->msg_idx_wr] = length;
	prudev->msg_idx_wr = (prudev->msg_idx_wr + 1) % MAX_FIFO_MSG;

	wake_up_interruptible(&prudev->wait_list);

	return 0;
}

// This function is called once the RPMsg channel is created. It creates the character device file and places it in the /dev/ directory.
static int pruss_api_probe(struct rpmsg_device *rpdev)
{
	int ret;
	struct pruss_api_dev *prudev;
	int minor_got;

	prudev = devm_kzalloc(&rpdev->dev, sizeof(*prudev), GFP_KERNEL);
	if (!prudev)
		return -ENOMEM;

	mutex_lock(&pruss_api_lock);
	minor_got = idr_alloc(&pruss_api_minors, prudev, 0, PRU_MAX_DEVICES,
			      GFP_KERNEL);
	mutex_unlock(&pruss_api_lock);
	if (minor_got < 0) {
		ret = minor_got;
		dev_err(&rpdev->dev, "Failed to get a minor number for the rpsmg device for pruss_api: %d\n",
			ret);
		goto fail_alloc_minor;
	}

	prudev->devt = MKDEV(MAJOR(pruss_api_devt), minor_got);

	cdev_init(&prudev->cdev, &pruss_api_fops);
	prudev->cdev.owner = THIS_MODULE;
	ret = cdev_add(&prudev->cdev, prudev->devt, 1);
	if (ret) {
		dev_err(&rpdev->dev, "Unable to add cdev for the rpmsg device for pruss_api\n");
		goto fail_add_cdev;
	}

	prudev->dev = device_create(pruss_api_class, &rpdev->dev, prudev->devt,
				    NULL, "pruss_api_pru%d", rpdev->dst);
	if (IS_ERR(prudev->dev)) {
		dev_err(&rpdev->dev, "Unable to create the rpmsg device for pruss_api\n");
		ret = PTR_ERR(prudev->dev);
		goto fail_create_device;
	}

	prudev->rpdev = rpdev;

	ret = kfifo_alloc(&prudev->msg_fifo, MAX_FIFO_MSG * FIFO_MSG_SIZE,
			  GFP_KERNEL);
	if (ret) {
		dev_err(&rpdev->dev, "Unable to allocate fifo for the rpmsg device for pruss_api\n");
		goto fail_alloc_fifo;
	}

	init_waitqueue_head(&prudev->wait_list);

	dev_set_drvdata(&rpdev->dev, prudev);

	dev_info(&rpdev->dev, "new pruss_api device: /dev/pruss_api_pru%d",
		 rpdev->dst);

	return 0;

fail_alloc_fifo:
	device_destroy(pruss_api_class, prudev->devt);
fail_create_device:
	cdev_del(&prudev->cdev);
fail_add_cdev:
	mutex_lock(&pruss_api_lock);
	idr_remove(&pruss_api_minors, minor_got);
	mutex_unlock(&pruss_api_lock);
fail_alloc_minor:
	return ret;
}

static void pruss_api_remove(struct rpmsg_device *rpdev)
{
	struct pruss_api_dev *prudev;

	prudev = dev_get_drvdata(&rpdev->dev);

	kfifo_free(&prudev->msg_fifo);
	device_destroy(pruss_api_class, prudev->devt);
	cdev_del(&prudev->cdev);
	mutex_lock(&pruss_api_lock);
	idr_remove(&pruss_api_minors, MINOR(prudev->devt));
	mutex_unlock(&pruss_api_lock);
}

// .name matches on RPMsg Channels and causes a probe
static const struct rpmsg_device_id rpmsg_driver_pru_id_table[] = {
	{ .name	= "pruss-api" },
	{ },
};
MODULE_DEVICE_TABLE(rpmsg, rpmsg_driver_pru_id_table);

static struct rpmsg_driver pruss_api_driver = {
	.drv.name	= KBUILD_MODNAME,
	.id_table	= rpmsg_driver_pru_id_table,
	.probe		= pruss_api_probe,
	.callback	= pruss_api_cb,
	.remove		= pruss_api_remove,
};

static int __init pruss_api_init(void)
{
	int ret;

	pruss_api_class = class_create(THIS_MODULE, "pruss_api");
	if (IS_ERR(pruss_api_class)) {
		pr_err("Unable to create class\n");
		ret = PTR_ERR(pruss_api_class);
		goto fail_create_class;
	}

	ret = alloc_chrdev_region(&pruss_api_devt, 0, PRU_MAX_DEVICES,
				  "pruss_api");
	if (ret) {
		pr_err("Unable to allocate chrdev region\n");
		goto fail_alloc_region;
	}

	ret = register_rpmsg_driver(&pruss_api_driver);
	if (ret) {
		pr_err("Unable to register rpmsg driver");
		goto fail_register_rpmsg_driver;
	}

	return 0;

fail_register_rpmsg_driver:
	unregister_chrdev_region(pruss_api_devt, PRU_MAX_DEVICES);
fail_alloc_region:
	class_destroy(pruss_api_class);
fail_create_class:
	return ret;
}

static void __exit pruss_api_exit(void)
{
	unregister_rpmsg_driver(&pruss_api_driver);
	idr_destroy(&pruss_api_minors);
	mutex_destroy(&pruss_api_lock);
	class_destroy(pruss_api_class);
	unregister_chrdev_region(pruss_api_devt, PRU_MAX_DEVICES);
}

module_init(pruss_api_init);
module_exit(pruss_api_exit);

MODULE_AUTHOR("Jason Reeder <jreeder@ti.com>");
MODULE_ALIAS("rpmsg:pruss-api");
MODULE_DESCRIPTION("Driver to establish RPMsg communication for PRUSS-Bindings using channel name 'pruss_api_pru(ch)'.");
MODULE_LICENSE("GPL v2");
