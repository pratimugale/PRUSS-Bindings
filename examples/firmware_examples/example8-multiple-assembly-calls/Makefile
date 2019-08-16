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

userspace:
	@g++ userspace.cpp ../../../cpp-bindings/pruss.cpp -o multi-asm.o
	@echo '------------------------------------------------------------------------- '
	@echo 'Running the UserSpace Program'
	@echo '------------------------------------------------------------------------- '
	@./multi-asm.o
	@echo '------------------------------------------------------------------------- '
	@echo 'Execution Complete'
	@echo '------------------------------------------------------------------------- '

clean:
	@cd ./PRU0/ && make clean
