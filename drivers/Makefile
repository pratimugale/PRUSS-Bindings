# Makefile for compiling out-of-tree
# Install Linux Headers first by "sudo apt install linux-headers-`uname -r`"
# Headers should install in /usr/src

KDIR := /lib/modules/$(shell uname -r)/build

obj-m += rpmsg_client_sample.o
obj-m += pruss_api.o

all:
	$(MAKE) -C $(KDIR) M=$$PWD
	
install:
	scp hello*.ko bone:.

print:
	@echo KERNELRELEASE= $(KERNELRELEASE)
	@echo CONFIG_EXAMPLES= $(CONFIG_EXAMPLES)
	@echo obj-m= $(obj-m)
	
clean:
	rm -rf .tmp_versions *.o 
	rm -rf .rpmsg_client_sample* rpmsg_client_sample*.mod.c
	rm -rf .rpmsg_pru* rpmsg_pru*.mod.c
	rm modules.order Module.symvers .built-in.o.cmd 
