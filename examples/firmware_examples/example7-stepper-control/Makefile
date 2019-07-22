all: clean install userspace

install: 
	@config-pin P9-31 pruout
	@echo '------------------------------------------------------------------------- '
	@echo 'Compiling and Loading Firmware For PRU0'
	@cd ./PRU0/ && make
	@cd ./PRU0/ && sudo make install
	@echo '------------------------------------------------------------------------- '
	@echo 'Compiling and Loading Firmware For PRU1'
	@cd ./PRU1/ && make
	@echo '------------------------------------------------------------------------- '

userspace:
	@echo 'Compiling the UserSpace Program'
	@g++ user_test.cpp driver_lib.cpp ../../../cpp-bindings/pruss.cpp -o userspace.o
	@echo '------------------------------------------------------------------------- '
	@./userspace.o
	@echo 'Started Running'

clean:
	@cd ./PRU0/ && make clean
	@cd ./PRU1/ && make clean
