# Copyright (c) 2016 Zubeen Tolani <ZeekHuge - zeekhuge@gmail.com>
#
# Usage:
#	Main source file: pru0.c
# 	add targets to variable TARGETS
#	add other files required while linking in variable LINK_PRU1(0)_FW
#	add compile targets, as added to LINK_PRU1(0)_FW for other files.
# 

# PRU_CGT environment variable points to the TI PRU compiler directory. (Usually /usr/share/ti/cgt-pru)
PRU_CGT:=/usr/share/ti/cgt-pru


############################################################
ifndef PRU_CGT
define ERROR_BODY

************************************************************
PRU_CGT environment variable is not set. Examples given:
(Linux) export PRU_CGT=/home/jason/ti/ccs_v6_1_0/ccsv6/tools/compiler/ti-cgt-pru_2.1.0
(Windows) set PRU_CGT=C:/TI/ccs_v6_0_1/ccsv6/tools/compiler/ti-cgt-pru_2.1.0
************************************************************

endef
$(error $(ERROR_BODY))
endif

PRU_SUPPORT:=/usr/lib/ti/pru-software-support-package
############################################################

############################################################
LINKER_COMMAND_FILE=./AM335x_PRU.cmd
LIBS=--library=$(PRU_SUPPORT)/lib/rpmsg_lib.lib
INCLUDE=--include_path=$(PRU_SUPPORT)/include --include_path=$(PRU_SUPPORT)/include/am335x
STACK_SIZE=0x100
HEAP_SIZE=0x100


CFLAGS=-v3 -O2 --printf_support=minimal --display_error_number --endian=little --hardware_mac=on --obj_directory=$(GEN_DIR) --pp_directory=$(GEN_DIR) --asm_directory=$(GEN_DIR) -ppd -ppa --asm_listing --optimizer_interlist --c_src_interlist # --absolute_listing
LFLAGS=--reread_libs --warn_sections --stack_size=$(STACK_SIZE) --heap_size=$(HEAP_SIZE) -m $(GEN_DIR)/$(TARGET).map
############################################################

############################################################
GEN_DIR=gen

PRU0_FW		=$(GEN_DIR)/pru0_fw.out

# add the required firmwares to TARGETS
TARGETS		=$(PRU0_FW)

# required linking files for PRU0
LINK_PRU0_FW1= $(GEN_DIR)/assembly1.object
LINK_PRU0_FW2= $(GEN_DIR)/assembly2.object
LINK_PRU0_FW3= $(GEN_DIR)/assembly3.object
# Link to .object dependencies on line 66. Add 4th link here.

############################################################



.PHONY: all
all: $(TARGETS) 
	@echo '-	Generated firmwares are : $^'

# Linking together all the .object file made by clpru
$(PRU0_FW): $(GEN_DIR)/main_pru0.object $(LINK_PRU0_FW1) $(LINK_PRU0_FW2) $(LINK_PRU0_FW3)
	@echo 'LD	$^' 
	@$(PRU_CGT)/bin/lnkpru -i$(PRU_CGT)/lib -i$(PRU_CGT)/include $(LFLAGS) -o $@ $^  $(LINKER_COMMAND_FILE) --library=libc.a $(LIBS) $^


$(GEN_DIR)/main_pru0.object: pru0.c 
	@mkdir -p $(GEN_DIR)
	@echo 'CC	$<'
	@$(PRU_CGT)/bin/clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -fe $@ $<

$(GEN_DIR)/assembly1.object: assembly1.asm
	@mkdir -p $(GEN_DIR)
	@echo 'CC	$<'
	@$(PRU_CGT)/bin/clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -fe $@ $<

$(GEN_DIR)/assembly2.object: assembly2.asm
	@mkdir -p $(GEN_DIR)
	@echo 'CC	$<'
	@$(PRU_CGT)/bin/clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -fe $@ $<

$(GEN_DIR)/assembly3.object: assembly3.asm
	@mkdir -p $(GEN_DIR)
	@echo 'CC	$<'
	@$(PRU_CGT)/bin/clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -fe $@ $<

############################################################
# Add 4th assembly object target here:
#$(GEN_DIR)/assembly4.object: assembly4.asm
#	@mkdir -p $(GEN_DIR)
#	@echo 'CC	$<'
#	@$(PRU_CGT)/bin/clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -fe $@ $<
############################################################

.PHONY: install install-pru0 copy_pru0_fw  

install: $(patsubst $(GEN_DIR)/pru%_fw.out, install-pru%, $(TARGETS))

install-pru0: $(PRU0_FW) copy_pru0_fw 

copy_pru0_fw: $(PRU0_FW)
	@echo '-	copying firmware to /lib/firmware/am335x_pru0_fw'
	@cp $(PRU0_FW) /lib/firmware/am335x-pru0-fw

.PHONY: clean
clean:
	@echo 'CLEAN	.'
	@rm -rf $(GEN_DIR)
