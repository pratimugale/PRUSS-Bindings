all: clean install

install: 
	@config-pin P9-31 pruout
	@cd ./PRU0/ && make
	@cd ./PRU0/ && sudo make install
	@cd ./PRU1/ && make
	@sudo python userspace_pwm.py

clean:
	@cd ./PRU0/ && make clean
	@cd ./PRU1/ && make clean
