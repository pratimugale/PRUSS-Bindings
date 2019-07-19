;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* This example is used to control a stepper motor by sending out precise number of pulse trains at required frequency(RPM). 
;* The 4 byte ON_Cycles input must be in memory location 0x00010000 - 0x00010003 in little-endian byte order
;* The 4 byte Total_Cycles input must be in memory location 0x00010004 - 0x00010007 in little endian byte order
;* The 4 byte Number of Pulses input must be in memory location 0x00010008 - 0x000100011 in little-endian byte order

	.cdecls "main_pru0.c"

PRU_SRAM .set 0x00010000            ; Set the location of PRU Shared Memory

	.clink
	.global start
start:                              ; One time setup.
        LDI32   R9, PRU_SRAM        ; R9 -> Base address of PRU SRAM
        SUB     R6, R6, R6          ; Clear the contents of R6
        SUB     R7, R7, R7          ; Clear the contents of R7
        SUB     R8, R8, R8          ; Clear the contents of R7
        LBBO    &R6, R9, 0, 4       ; R6 -> Duty Cycle.(actually ON cycles); Copy (4) bytes into R6 from memory address R9+offset(0)
        LBBO    &R7, R9, 4, 4       ; R7 -> Total Cycles. Copy (4) bytes into R7 from memory address R9+offset(4)
        LBBO    &R8, R9, 8, 4       ; R8 -> Number of pulses to be generated. Copy (4) bytes into R8 from memory address R9+offset(8)
        LDI     R11, 0              ; R11 -> Total sample count (goes from 0 to 100 for 1MHz)
        LDI     R10, 0              ; R10 -> Counts the number of pulses to be generated.
        QBA     sample_start

        ADD     R8, R8, 1           ; To equalize the pulse count

count_check:                        ; Sends precise number of pulses (steps) to the stepper motor.
        ADD     R10, R10, 1           ; Increment counter
        QBEQ    stop, R10, R8        ; Stop if the total number of pulses have been generated.
        
sample_start:                       ; 
        SET     R30, R30.t0         ; GPIO P9_31 HIGH -> Each low to high signal counts as a step for the motor.
sample_high:                        ; [Loop consuming 2 PRU cycles per iteration]
        ADD     R11, R11, 0x00000001 ; Increment counter by 1 
        QBNE    sample_high, R11, R6 ; Repeat loop until ON_Cycles 
        NOP
        NOP
       ; NOP                        ; NOPs can be removed. This will yield a better frequency. The duty cycle of the pulses is not important.
       ; NOP
       ; NOP
        CLR     R30, R30.t0         ; GPIO P9_31 OFF
sample_low:                         ; [Loop consuming 2 PRU cycles per iteration]
        ADD     R11, R11, 0x00000001  ; Increment counter by 1
        QBNE    sample_low, R11, R7 ; Repeat loop until Total Cycles
        SUB     R11, R11, R11       ; Clear the counter register
        QBA     count_check         ; One PWM cycle is completed. Repeat again for back to back pulses.

stop:
;        SET     R31, R31.t30
        ;SET     R31, R31.t5         ; Strobe interrupt configured in main_pru0.c by setting bit 5 of R31 to HIGH
        JMP      R3.w2               ; R3.w2->contains return address; jumps back to the execution of c-code from which this .asm code was called
