.PHONY: all install userspace

all: clean install userspace

install: 
	@config-pin P9-31 pruout
	@config-pin P9-30 pruout
	@config-pin P9-29 pruout
	@echo '------------------------------------------------------------------------- '
	@echo 'Compiling and Loading Firmware For PRU0'
	@cd ./PRU0/ && make
	@cd ./PRU0/ && sudo make install
	@echo '------------------------------------------------------------------------- '
	@echo '------------------------------------------------------------------------- '

userspace:
	@echo 'Compiling the UserSpace Program'
	@g++ user_test.cpp ../../../cpp-bindings/pruss.cpp -o userspace.o
	@echo '------------------------------------------------------------------------- '
	@echo 'Running the UserSpace Program'
	@./userspace.o
	@echo '------------------------------------------------------------------------- '
	@echo 'Execution Complete'
	@echo '------------------------------------------------------------------------- '

.PHONY: clean
clean:
	@cd ./PRU0/ && make clean
