;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* This example is used to control a stepper motor by sending out precise number of pulse trains at required frequency(RPM). 
;* The 4 byte ON_Cycles input must be in memory location 0x00010000 - 0x00010003 in little-endian byte order
;* The 4 byte Total_Cycles input must be in memory location 0x00010004 - 0x00010007 in little endian byte order
;* The 4 byte Number of Pulses input must be in memory location 0x00010008 - 0x000100011 in little-endian byte order

	.cdecls "pru0.c"

PRU_SRAM .set 0x00010000            ; Set the location of PRU Shared Memory

	.clink
	.global start1
start1:                                  ; One time setup.
        SET      R30, R30.t1

stop:
        ;SET     R31, R31.t5            ; Strobe interrupt configured in main_pru0.c by setting bit 5 of R31 to HIGH
        JMP      R3.w2                  ; R3.w2 -> stores the return address. The main_pru0.c program will continue its normal execution after start().
