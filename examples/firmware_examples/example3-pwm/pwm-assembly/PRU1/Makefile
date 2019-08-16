# Source modified by Pratim Ugale <pratim.ugale@gmail.com>
# Copyright (c) 2016 Zubeen Tolani <ZeekHuge - zeekhuge@gmail.com>
# Copyright (c) 2017 Texas Instruments - Jason Kridner <jdk@ti.com>
# 

# PRUN must be defined as the PRU number (0 or 1) to compile for.
PRUN=1

# TARGET must be defined as the file to be compiled without the .c extension
TARGET=main

# PRU_CGT environment variable points to the TI PRU compiler directory. (Usually /usr/share/ti/cgt-pru)
PRU_CGT:=/usr/share/ti/cgt-pru

# PRU_SUPPORT points to pru-software-support-package.
PRU_SUPPORT:=/usr/lib/ti/pru-software-support-package

# GEN_DIR points to where to put the generated files.
GEN_DIR:=/tmp/pru$(PRUN)-gen

LINKER_COMMAND_FILE=AM335x_PRU.cmd
LIBS=--library=$(PRU_SUPPORT)/lib/rpmsg_lib.lib
INCLUDE=--include_path=$(PRU_SUPPORT)/include --include_path=$(PRU_SUPPORT)/include/am335x

STACK_SIZE=0x100
HEAP_SIZE=0x100

CFLAGS=-v3 -O2 --printf_support=minimal --display_error_number --endian=little --hardware_mac=on --obj_directory=$(GEN_DIR) --pp_directory=$(GEN_DIR) --asm_directory=$(GEN_DIR) -ppd -ppa --asm_listing --c_src_interlist # --absolute_listing

LFLAGS=--reread_libs --warn_sections --stack_size=$(STACK_SIZE) --heap_size=$(HEAP_SIZE) -m $(GEN_DIR)/$(TARGET).map

# Lookup PRU by address
ifeq ($(PRUN),0)
PRU_ADDR=4a334000
endif
ifeq ($(PRUN),1)
PRU_ADDR=4a338000
endif

PRU_DIR=$(wildcard /sys/devices/platform/ocp/4a32600*.pruss-soc-bus/4a300000.pruss/$(PRU_ADDR).*/remoteproc/remoteproc*)

# PRU Start and Stop will be handled by the User Space Program itself

all: $(GEN_DIR)/$(TARGET).out

install: $(GEN_DIR)/$(TARGET).out
	@echo '-	copying firmware file $(GEN_DIR)/$(TARGET).out to $(GEN_DIR)/am335x-pru$(PRUN)-fw'
	@sudo cp $(GEN_DIR)/$(TARGET).out $(GEN_DIR)/am335x-pru$(PRUN)-fw
	@echo '-	copying file $(GEN_DIR)/am335x-pru$(PRUN)-fw to /lib/firmware/'
	@sudo cp $(GEN_DIR)/am335x-pru$(PRUN)-fw /lib/firmware/

$(GEN_DIR)/$(TARGET).out: $(GEN_DIR)/$(TARGET).obj
	@echo 'LD	$^' 
	@lnkpru -i$(PRU_CGT)/lib -i$(PRU_CGT)/include $(LFLAGS) -o $@ $^ $(LINKER_COMMAND_FILE) --library=libc.a $(LIBS) $^

$(GEN_DIR)/$(TARGET).obj: $(TARGET).c
	@mkdir -p $(GEN_DIR)
	@echo 'CC	$<'
	@clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -D=PRUN=$(PRUN) -fe $@ $<

clean:
	@echo 'CLEAN	.    PRU $(PRUN)'
	@rm -rf $(GEN_DIR)
