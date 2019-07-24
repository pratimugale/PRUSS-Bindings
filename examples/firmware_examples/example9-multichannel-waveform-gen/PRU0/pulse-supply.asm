;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* Demonstrates 2-channel waveform generation
;* The samples are interveaved into the PRU SRAM.

	.cdecls "main_pru0.c"

PRU_SRAM .set 0x00010000            ; Set the location of PRU Shared Memory

	.clink
	.global start
start:                              ; One time setup.
        LDI32   R10, PRU_SRAM       ; R10 -> Base address of PRU SRAM
        SUB     R5, R5, R5          ; Clear the contents of R2
        LBBO    &R5, R10, 0, 1      ; R5 -> Total number of samples including all the waveforms.
        LDI     R6, 100             ; Total number of cycles for each individual wave sample.
        LDI     R11, 0               ; Sample Counter.
        QBA     sample_start

sample_start:                       ; 
        QBLT    same_sample, R5.b0, R11.b0
        LDI     R11, 1

same_sample:                        ; [Loop consuming 2 PRU cycles per iteration]
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R7.b0, R10, R11.b0, 1; R7 -> DC of Wave 1
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R8.b0, R10, R11.b0, 1; R8 -> DC of Wave 2
        LDI32   R12, 10000
        LDI     R9, 0                ; DC ON counter.
        LDI     R15, 0                ; DC OFF counter.
        LDI32   R13, 0                ; Pulse counter.

count_check:
        ADD     R13, R13, 1     
        QBEQ    done_sample, R13, R12

start_pwm1: 
        SET     R30, R30.t0
        SUB     R14, R6, R7 
        
pwm1_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm1_high, R9.b0, R7.b0
        NOP
        NOP
        CLR     R30, R30.t0

pwm1_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm1_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm2: 
        SET     R30, R30.t2
        SUB     R14, R6, R8
 
pwm2_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm2_high, R9.b0, R8.b0
        NOP
        NOP
        CLR     R30, R30.t2

pwm2_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm2_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9
        QBA     count_check

done_sample:
        QBA     sample_start

stop:
        HALT
