;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* Demonstrates 4-channel waveform generation
;* The samples are interveaved into the PRU SRAM.

	.cdecls "main_pru0.c"

PRU_SRAM .set 0x00010000            ; Set the location of PRU Shared Memory

	.clink
	.global start
start:                              ; One time setup.
        LDI32   R10, PRU_SRAM       ; R10 -> Base address of PRU SRAM
        ;SUB     R5, R5, R5          ; Clear the contents of R5
        ;LBBO    &R5, R10, 0, 1      ; R5 -> Total number of samples including all the waveforms.
        ;LDI     R5, 400
        LDI     R6, 100             ; Total number of cycles for each individual wave sample.
        ;LDI     R11, 0              ; Total Sample Counter for all Waveforms.
        ;QBA     sample_start
        ;SUB     R16, R16, R16

sample_start:                       ; 
        
        ;QBLT    same_sample, R5, R11
        ;LDI     R11, 0

same_sample:                          ; 
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R7.b0, R10, R11, 1   ; R7 -> DC of Wave 1
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R8.b0, R10, R11, 1   ; R8 -> DC of Wave 2
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R7.b1, R10, R11, 1   ; R8 -> DC of Wave 3
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R8.b1, R10, R11, 1   ; R8 -> DC of Wave 3
        LDI32   R12, 2500             ; 
        LDI     R9,  0                ; DC ON counter.
        LDI     R15, 0                ; DC OFF counter.
        LDI32   R13, 0                ; Pulse counter.
        QBLT    greaterthan_zero1, R7.b0, 1
        LDI     R7.b0, 1

greaterthan_zero1:
        QBGT    lesserthan_hun1, R7.b0, 99
        LDI     R7.b0, 99

lesserthan_hun1:
        QBLT    greaterthan_zero2, R8.b0, 1
        LDI     R8.b0, 1
        
greaterthan_zero2:
        QBGT    lesserthan_hun2, R8.b0, 99
        LDI     R8.b0, 99

lesserthan_hun2:
        QBLT    greaterthan_zero3, R7.b1, 1
        LDI     R7.b1, 1

greaterthan_zero3:
        QBGT    lesserthan_hun3, R7.b1, 99
        LDI     R7.b1, 99

lesserthan_hun3:
        QBLT    greaterthan_zero4, R8.b1, 1
        LDI     R8.b1, 1

greaterthan_zero4:
        QBGT    count_check, R8.b1, 99
        LDI     R8.b1, 99

count_check:
        ADD     R13, R13, 1     
        QBEQ    sample_start, R13, R12

start_pwm1: 
        SET     R30, R30.t0
        SUB     R14.b0, R6.b0, R7.b0
        
pwm1_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm1_high, R9.b0, R7.b0
        CLR     R30, R30.t0

pwm1_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm1_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm2: 
        SET     R30, R30.t2
        SUB     R14.b0, R6.b0, R8.b0
 
pwm2_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm2_high, R9.b0, R8.b0
        CLR     R30, R30.t2

pwm2_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm2_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm3: 
        SET     R30, R30.t1
        SUB     R14.b0, R6.b0, R7.b1
        
pwm3_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm3_high, R9.b0, R7.b1
        CLR     R30, R30.t1

pwm3_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm3_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm4: 
        SET     R30, R30.t3
        SUB     R14.b0, R6.b0, R8.b1
 
pwm4_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm4_high, R9.b0, R8.b1
        CLR     R30, R30.t3

pwm4_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm4_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

        QBA     count_check

done_sample:
        QBA     sample_start

stop:
        HALT
