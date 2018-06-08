# Instructions to run Examples

## Daemon Server
* Firstly, clone the repository (to avoid changing daemon file paths, clone the repo in /home/debian)
```
git clone https://github.com/MuneebMohammed/PRUSS-Bindings
```
* Copy the prussd.service file to /lib/systemd/system/ and reload the daemon (need root permissions for this)
```
cp PRUSS-Bindings/prussd/prussd.service /lib/systemd/system/
systemctl daemon-reload
systemctl start prussd.service
systemctl status prussd.service
```
If all goes well, you should see the daemon service active and enabled.


## Compiling and Running the firmware and the example apps using the bindings
* To run the specfic example - blinky, rpmsg_echo, pru_pin_state_reader, pru1_to_pru0_to_arm, multi_threaded
```
bash run.sh blinky
./blinky.o
```
```
bash run.sh rpmsg_echo
./rpmsg_echo.o
```
```
bash run.sh pru_pin_state_reader
./pru_pin_state_reader.o
```
```
bash run.sh pru1_to_pru0_to_arm
./pru1_to_pru0_to_arm.o
```
```
bash run.sh multi_threaded
./multi_threaded.o
```
