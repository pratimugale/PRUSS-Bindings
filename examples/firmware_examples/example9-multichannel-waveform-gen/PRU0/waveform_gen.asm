;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* Demonstrates 8-channel waveform generation
;* The sampled data values are interweaved into the PRU SRAM.

	.cdecls "main_pru0.c"

PRU_SRAM .set 0x00010000            ; Set the location of PRU Shared Memory

	.clink
	.global start
start:                              ; One time setup.
        LDI32   R10, PRU_SRAM       ; R10 -> Base address of PRU SRAM
        SUB     R5, R5, R5          ; Clear the contents of R5
        LBBO    &R5, R10, 0, 4      ; R5 -> Total number of samples including all the waveforms.
        ;LDI     R5, 800
        LDI     R6, 100             ; Total number of cycles for each individual wave sample.
        LDI32   R11, 3              ; Total Sample Counter for all Waveforms.
        LDI32   R17, 0
        QBA     sample_start
        SUB     R16, R16, R16

sample_start:                       ; 
        
        QBLT    same_sample, R5, R17
        LDI32   R17, 0
        LDI     R11, 3

same_sample:                          ; 
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R7.b0, R10, R11, 1   ; R7 -> DC of Wave 1
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R8.b0, R10, R11, 1   ; R8 -> DC of Wave 2
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R7.b1, R10, R11, 1   ; R8 -> DC of Wave 3
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R8.b1, R10, R11, 1   ; R8 -> DC of Wave 3
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R7.b2, R10, R11, 1   ; R7 -> DC of Wave 1
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R8.b2, R10, R11, 1   ; R8 -> DC of Wave 2
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R7.b3, R10, R11, 1   ; R8 -> DC of Wave 3
        ADD     R11, R11, 0x00000001  ; Increment offset counter by 1 
        LBBO    &R8.b3, R10, R11, 1   ; R8 -> DC of Wave 3
        ADD     R17, R17, 8
        LDI32   R12, 1250             ; 
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
        QBGT    lesserthan_hun4, R8.b1, 99
        LDI     R8.b1, 99

lesserthan_hun4:
        QBLT    greaterthan_zero5, R7.b2, 1
        LDI     R7.b2, 1

greaterthan_zero5:
        QBGT    lesserthan_hun5, R7.b2, 99
        LDI     R7.b2, 99

lesserthan_hun5:
        QBLT    greaterthan_zero6, R8.b2, 1
        LDI     R8.b2, 1

greaterthan_zero6:
        QBGT    lesserthan_hun6, R8.b2, 99
        LDI     R8.b2, 99

lesserthan_hun6:
        QBLT    greaterthan_zero7, R7.b3, 1
        LDI     R7.b3, 1

greaterthan_zero7:
        QBGT    lesserthan_hun7, R7.b3, 99
        LDI     R7.b3, 99

lesserthan_hun7:
        QBLT    greaterthan_zero8, R8.b3, 1
        LDI     R8.b3, 1

greaterthan_zero8:
        QBGT    count_check, R8.b3, 99
        LDI     R8.b3, 99

count_check:
        ADD     R13, R13, 1     
        QBEQ    sample_start, R13, R12

start_pwm1: 
        SET     R30, R30.t5
        SUB     R14.b0, R6.b0, R7.b0
        
pwm1_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm1_high, R9.b0, R7.b0
        CLR     R30, R30.t5

pwm1_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm1_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm2: 
        SET     R30, R30.t7
        SUB     R14.b0, R6.b0, R8.b0
 
pwm2_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm2_high, R9.b0, R8.b0
        CLR     R30, R30.t7

pwm2_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm2_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm3: 
        SET     R30, R30.t14
        SUB     R14.b0, R6.b0, R7.b1
        
pwm3_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm3_high, R9.b0, R7.b1
        CLR     R30, R30.t14

pwm3_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm3_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm4: 
        SET     R30, R30.t15
        SUB     R14.b0, R6.b0, R8.b1
 
pwm4_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm4_high, R9.b0, R8.b1
        CLR     R30, R30.t15

pwm4_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm4_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm5: 
        SET     R30, R30.t0
        SUB     R14.b0, R6.b0, R7.b2
        
pwm5_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm5_high, R9.b0, R7.b2
        CLR     R30, R30.t0

pwm5_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm5_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm6: 
        SET     R30, R30.t2
        SUB     R14.b0, R6.b0, R8.b2
 
pwm6_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm6_high, R9.b0, R8.b2
        CLR     R30, R30.t2

pwm6_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm6_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm7: 
        SET     R30, R30.t1
        SUB     R14.b0, R6.b0, R7.b3
        
pwm7_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm7_high, R9.b0, R7.b3
        CLR     R30, R30.t1

pwm7_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm7_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

start_pwm8: 
        SET     R30, R30.t3
        SUB     R14.b0, R6.b0, R8.b3
 
pwm8_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm8_high, R9.b0, R8.b3
        CLR     R30, R30.t3

pwm8_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm8_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9, R9, R9

        QBA     count_check

done_sample:
        QBA     sample_start

stop:
        HALT
