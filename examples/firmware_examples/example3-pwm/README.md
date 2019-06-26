# PWM Example

The `pwm2.c` example generates a PWM output of a desired voltage at the pin P9_31.<br>
`config-pin P9_31 pruout`<br>
`disable_uboot_overlay_video=1` in `/boot/uEnv.txt`<br>
`export PRUN=0`<br>
`export TARGET=pwm_test`<br>
`export PRU_CGT=/usr/share/ti/cgt-pru`<br>
`make clean && make`

The example takes input from ARM into the SHARED memory. More examples to be added.

--------
## About the PRU Memory Region
--------
The PRU-ICSS comprises various distinct addressable regions that are mapped to both a local and global memory map. 
The local memory maps are maps with respect to the PRU core point of view. 
The global memory maps are maps with respect to the Host point of view, but can also be accessed by the PRUICSS.

i.e. local is with respect to PRU0, PRU1 individually; global is w.r.t the entire PRU SubSystem 

### _Local_ Instruction Memory Map
Each PRU core has a dedicated 8KB of Instruction Memory which needs to be initialized by a Host processor before the PRU executes instructions. 
This region is only accessible to masters via the interface/ OCP slave port when the PRU is not running.
`Local Instruction Memory Map`:

| Start Address | PRU0 | PRU1 |
|---------------|------|------|
| 0x0000_0000 | 8KB IRAM | 8KB IRAM | 

Note that these two memory maps are implemented inside the PRU-ICSS and are local to the components of the PRU-ICSS.
<br>
### _Local_ Data Memory Map:
Allows each PRU core to access the PRU-ICSS addressable regions and the external host’s memory map.
By default, memory addresses between 0x0000_0000 – 0x0007_FFFF will correspond to the PRU-ICSS local address. To access an address
between 0x0000_0000–0x0007_FFFF of the external Host map, the address offset of –0x0008_0000 feature is enabled through the PMAO register of the PRU-ICSS CFG register space.


***Data RAM0 is intended to be the primary data memory for PRU0, as is Data RAM1 for PRU1. However, both PRU cores can
access Data RAM0 and Data RAM1 to pass information between PRUs. Each PRU core accesses their intended Data RAM at
address 0x0000_0000 and the other Data RAM at address 0x0000_2000.***

|Start Address |PRU0 |PRU1 |
|--------------|-----|-----|
|0x0000_0000 |Data 8KB RAM 0| Data 8KB RAM 1|
|0x0000_2000 |Data 8KB RAM 1| Data 8KB RAM 0|
|0x0001_0000 |Shared Data 12KB RAM 2| Shared Data 12KB RAM 2|
|0x0002_0000 |INTC |INTC|
|0x0002_2000 |PRU0 Control |PRU0 Control|
|0x0002_2400 |Reserved |Reserved|
|0x0002_4000 |PRU1 Control |PRU1 Control|
|0x0002_4400 |Reserved |Reserved|
|0x0002_6000 |CFG |CFG|
|0x0002_8000 |UART 0 |UART 0|
|0x0002_A000 |Reserved |Reserved|
|0x0002_C000 |Reserved |Reserved|
|0x0002_E000 |IEP |IEP|
|0x0003_0000 |eCAP 0 |eCAP 0|
|0x0003_2000 |MII_RT_CFG |MII_RT_CFG|
|0x0003_2400 |MII_MDIO |MII_MDIO|
|0x0003_4000 |Reserved |Reserved|
|0x0003_8000 |Reserved |Reserved|
|0x0004_0000 |Reserved |Reserved|
|0x0008_0000 |System OCP_HP0 |System OCP_HP1|

### _Global_ Memory Map
The global view of the PRU-ICSS internal memories and control ports is shown in the next Table. The offset addresses of each region are implemented inside the PRU-ICSS but the global device memory mapping places the PRU-ICSS slave port in the address range shown in the external Host top-level memory map.
The global memory map is with respect to the Host point of view, but it can also be accessed by the PRUICSS. Note that PRU0 and PRU1 can use either the local or global addresses to access their internal memories, but using the local addresses will provide access time several cycles faster than using the global addresses. This is because when accessing via the global address the access needs to be routed through the switch fabric outside PRU-ICSS and back in through the PRU-ICSS slave port.
Each of the PRUs can access the rest of the device memory (including memory mapped peripheral and configuration registers) using the global memory space addresses.

|Offset Address |PRU-ICSS |
|---------------|---------|
|0x0000_0000 |Data 8KB RAM 0|
|0x0000_2000 |Data 8KB RAM 1|
|0x0001_0000 |Shared Data 12KB RAM 2|
|0x0002_0000 |INTC|
|0x0002_2000 |PRU0 Control|
|0x0002_2400 |PRU0 Debug|
|0x0002_4000 |PRU1 Control|
|0x0002_4400 |PRU1 Debug|
|0x0002_6000 |CFG|
|0x0002_8000 |UART 0|
|0x0002_A000 |Reserved|
|0x0002_C000 |Reserved|
|0x0002_E000 |IEP|
|0x0003_0000 |eCAP 0|
|0x0003_2000 |MII_RT_CFG|
|0x0003_2400 |MII_MDIO|
|0x0003_4000 |PRU0 8KB IRAM|
|0x0003_8000 |PRU1 8KB IRAM|
|0x0004_0000 |Reserved|
