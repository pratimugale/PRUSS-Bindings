;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* This is a PWM example 
;* Observation: When 

	.cdecls "main_pru1.c"

PRU_SRAM .set 0x00010000

	.clink
	.global start
start:  
        LDI32   R10, PRU_SRAM       ; R10-> Base address of PRU SRAM
        LBBO    &R1.b0, R10, 0, 1   ; R1 -> Duty Cycle. Copy (1) bytes into R1 from memory address R10+offset(0)
        LBBO    &R2.b0, R10, 1, 1   ; R2 -> . Copy (1) bytes into R1 from memory address R10+offset(0)
        LDI     R0, 0               ; R0 - Total sample count (goes from 0 to 100)
        QBA     sample_start

sample_start:                       ; [1 cycle]
        SET     R30, R30.t0         ; GPIO ON
sample_high:                        
        ADD     R0, R0, 1           ; Increment counter by 1 
        QBNE    sample_high, R0.b0, R1.b0
        NOP
        NOP
        NOP
        CLR     R30, R30.t0
sample_low:
        ADD     R0, R0, 1
        QBNE    sample_low, R0.b0, R2.b0
        SUB     R0, R0, R0
        QBA     sample_start

	HALT
