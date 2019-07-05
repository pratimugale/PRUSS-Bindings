;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* This example generates a PWM signal and is capable of producing frequencies from 1 Hz to 1 MHz
;* The 4 byte ON_Cycles input must be in memory location 0x00010000 - 0x00010003 in little-endian byte order
;* The 4 byte Total_Cycles input must be in memory location 0x00010004 - 0x00010007 in little endian byte order

	.cdecls "main_pru0.c"

PRU_SRAM .set 0x00010000            ; Set the location of PRU Shared Memory

	.clink
	.global start
start:                              ; One time setup.
        LDI32   R10, PRU_SRAM       ; R10 -> Base address of PRU SRAM
        SUB     R1, R1, R1          ; Clear the contents of R1
        SUB     R2, R2, R2          ; Clear the contents of R2
        LBBO    &R1, R10, 0, 4      ; R1 -> Duty Cycle.(actually ON cycles); Copy (4) bytes into R1 from memory address R10+offset(0)
        LBBO    &R2, R10, 4, 4      ; R2 -> Total Cycles. Copy (4) bytes into R2 from memory address R10+offset(5)
        LDI     R0, 0               ; R0 - Total sample count (goes from 0 to 100 for 1MHz)
        QBA     sample_start

sample_start:                       ; 
        SET     R30, R30.t0         ; GPIO P9_31 ON
sample_high:                        ; [Loop consuming 2 PRU cycles per iteration]
        ADD     R0, R0, 0x00000001  ; Increment counter by 1 
        QBNE    sample_high, R0, R1 ; Repeat loop until ON_Cycles 
        NOP
        NOP
        NOP
        CLR     R30, R30.t0         ; GPIO P9_31 OFF
sample_low:                         ; [Loop consuming 2 PRU cycles per iteration]
        ADD     R0, R0, 0x00000001  ; Increment counter by 1
        QBNE    sample_low, R0, R2  ; Repeat loop until Total Cycles
        SUB     R0, R0, R0          ; Clear the counter register
        QBA     sample_start        ; One PWM cycle is completed. Repeat again for back to back pulses.

	HALT
