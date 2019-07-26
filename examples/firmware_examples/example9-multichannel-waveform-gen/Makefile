all: clean install

install: 
	@config-pin P9-31 pruout
	@config-pin P9-30 pruout
	@cd ./PRU0/ && make
	@cd ./PRU0/ && sudo make install
	@cd ./PRU1/ && make
	@g++ userspace.cpp ../../../cpp-bindings/pruss.cpp -o userspace2.o
	@./userspace2.o

clean:
	@cd ./PRU0/ && make clean
	@cd ./PRU1/ && make clean
