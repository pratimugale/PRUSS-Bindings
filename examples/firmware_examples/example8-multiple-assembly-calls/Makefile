GEN := /gen
TARGET := userspace

.PHONY: all install userspace

CXX := /usr/bin/g++

all: clean install $(GEN)/$(TARGET).o

install: 
	@config-pin P9-31 pruout
	@config-pin P9-30 pruout
	@config-pin P9-29 pruout
	@mkdir gen
	@echo '------------------------------------------------------------------------- '
	@echo 'Compiling and Loading Firmware For PRU0'
	@cd ./PRU0/ && make
	@cd ./PRU0/ && sudo make install
	@echo '------------------------------------------------------------------------- '

$(GEN)/$(TARGET).o: user_test.cpp
	#@echo 'CC   $< '
	@$(CXX) -o $@ ../../../cpp-bindings/pruss.cpp
	@echo '------------------------------------------------------------------------- '
	@echo 'Running the UserSpace Program'
	@./userspace.o
	@echo '------------------------------------------------------------------------- '
	@echo 'Execution Complete'
	@echo '------------------------------------------------------------------------- '

.PHONY: clean
clean:
	@cd ./PRU0/ && make clean
