#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7ef3190f, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xd6387855, __VMLINUX_SYMBOL_STR(idr_destroy) },
	{ 0xd9232880, __VMLINUX_SYMBOL_STR(unregister_rpmsg_driver) },
	{ 0x9491b702, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0xba11c5b4, __VMLINUX_SYMBOL_STR(__register_rpmsg_driver) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x339b7a65, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x67496df7, __VMLINUX_SYMBOL_STR(_dev_info) },
	{ 0x93de854a, __VMLINUX_SYMBOL_STR(__init_waitqueue_head) },
	{ 0xc068440e, __VMLINUX_SYMBOL_STR(__kfifo_alloc) },
	{ 0xb7f96077, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x4b1552f8, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0x217421db, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x93bf7c0e, __VMLINUX_SYMBOL_STR(idr_alloc_cmn) },
	{ 0xd80c3e93, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x98dfb43, __VMLINUX_SYMBOL_STR(finish_wait) },
	{ 0x9e52ac12, __VMLINUX_SYMBOL_STR(prepare_to_wait_event) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xfe487975, __VMLINUX_SYMBOL_STR(init_wait_entry) },
	{ 0x4578f528, __VMLINUX_SYMBOL_STR(__kfifo_to_user) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x5853171f, __VMLINUX_SYMBOL_STR(rpmsg_send) },
	{ 0x28cc25db, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0x38c9d41c, __VMLINUX_SYMBOL_STR(radix_tree_delete_item) },
	{ 0x9f7a3e33, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x91bda9a9, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0xdb760f52, __VMLINUX_SYMBOL_STR(__kfifo_free) },
	{ 0xe4ca3b4f, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0xfd16e532, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xada832f5, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x4215a929, __VMLINUX_SYMBOL_STR(__wake_up) },
	{ 0xf23fcb99, __VMLINUX_SYMBOL_STR(__kfifo_in) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=rpmsg_core";

