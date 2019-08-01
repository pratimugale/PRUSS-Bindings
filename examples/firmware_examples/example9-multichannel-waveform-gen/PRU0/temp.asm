;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* Demonstrates 8-channel waveform generation
;* The sampled data values are interweaved into the PRU SRAM.

	.cdecls "main_pru0.c"

PRU_SRAM .set 0x00010000            ; Set the location of PRU Shared Memory

	.clink
	.global start
start:                              ; One time setup.
        LDI32   R10, PRU_SRAM       ; R10 -> Base address of PRU SRAM
        ;SUB     R5, R5, R5          ; Clear the contents of R5
        ;LBBO    &R5, R10, 0, 4      ; R5 -> Total number of samples including all the waveforms.
        ;LDI     R5, 800
        LDI     R0, 100             ; Total number of cycles for each individual wave sample.
        ;LDI32   R17, 0
        ;QBA     sample_start
        LDI     R4.w0,  4
        LDI     R4.w2,  5
        LDI     R5.w0, 6
        LDI     R5.w2, 7
        LDI     R6.w0, 8
        LDI     R6.w2, 9
        LDI     R11.w0, 10
        LDI     R11.w2, 11

sample_start:                       ; 
        
        ;QBLT    same_sample, R5, R11
        ;LDI32   R17, 0
        ;LDI     R11, 3

same_sample:                          ; 
        LBBO    &R7.b0, R10, R4.w0, 1   ; R7 -> DC of Wave 1
        ADD     R4.w0, R4.w0, 8        ; Increment offset counter by 1 
        QBNE    same_sample_1, R7.b0, 150;
        LDI     R4.w0, 4
        LBBO    &R7.b0, R10, R4.w0, 1   ; R7 -> DC of Wave 1

same_sample_1:
        LBBO    &R8.b0, R10, R4.w2, 1   ; R7 -> DC of Wave 1
        ADD     R4.w2, R4.w2, 8        ; Increment offset counter by 1 
        QBNE    same_sample_2, R8.b0, 150;
        LDI     R4.w2, 5
        LBBO    &R8.b0, R10, R4.w2, 1   ; R7 -> DC of Wave 1

same_sample_2:
        LBBO    &R7.b1, R10, R5.w0, 1   ; R7 -> DC of Wave 1
        ADD     R5.w0, R5.w0, 8        ; Increment offset counter by 1 
        QBNE    same_sample_3, R7.b1, 150;
        LDI     R5.w0, 6
        LBBO    &R7.b1, R10, R5.w0, 1   ; R7 -> DC of Wave 1

same_sample_3:
        LBBO    &R8.b1, R10, R5.w2, 1   ; R7 -> DC of Wave 1
        ADD     R5.w2, R5.w2, 8        ; Increment offset counter by 1 
        QBNE    same_sample_4, R8.b1, 150;
        LDI     R5.w2, 7
        LBBO    &R8.b1, R10, R5.w2, 1   ; R7 -> DC of Wave 1

same_sample_4:
        LBBO    &R7.b2, R10, R6.w0, 1   ; R7 -> DC of Wave 1
        ADD     R6.w0, R6.w0, 8  ; Increment offset counter by 1 
        QBNE    same_sample_5, R7.b2, 150;
        LDI     R6.w0, 8
        LBBO    &R7.b2, R10, R6.w0, 1   ; R7 -> DC of Wave 1

same_sample_5:
        LBBO    &R8.b2, R10, R6.w2, 1   ; R7 -> DC of Wave 1
        ADD     R6.w2, R6.w2, 8  ; Increment offset counter by 1 
        QBNE    same_sample_6, R8.b2, 150;
        LDI     R6.w2, 9
        LBBO    &R8.b2, R10, R6.w2, 1   ; R7 -> DC of Wave 1

same_sample_6:
        LBBO    &R7.b3, R10, R11.w0, 1   ; R7 -> DC of Wave 1
        ADD     R11.w0, R11.w0, 8  ; Increment offset counter by 1 
        QBNE    same_sample_7, R7.b3, 150;
        LDI     R11.w0, 10
        LBBO    &R7.b3, R10, R11.w0, 1   ; R7 -> DC of Wave 1

same_sample_7:
        LBBO    &R8.b3, R10, R11.w2, 1   ; R7 -> DC of Wave 1
        ADD     R11.w2, R11.w2, 8  ; Increment offset counter by 1 
        QBNE    same_sample_8, R8.b3, 150;
        LDI     R11.w2, 11
        LBBO    &R8.b3, R10, R11.w2, 1   ; R7 -> DC of Wave 1

same_sample_8:

        LDI32   R12, 1212             ; 
        LDI     R9.w0,  0                ; DC ON counter.
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
        ;QBEQ    same_sample, R13, R12

start_pwm1: 
        SET     R30, R30.t0
        SUB     R14.b0, R0.b0, R7.b0
        
pwm1_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm1_high, R9.b0, R7.b0
        CLR     R30, R30.t0

pwm1_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm1_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9.w0, R9.w0, R9.w0

start_pwm2: 
        SET     R30, R30.t2
        SUB     R14.b0, R0.b0, R8.b0
 
pwm2_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm2_high, R9.b0, R8.b0
        CLR     R30, R30.t2

pwm2_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm2_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9.b0, R9.b0, R9.b0

start_pwm3: 
        SET     R30, R30.t1
        SUB     R14.b0, R0.b0, R7.b1
        
pwm3_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm3_high, R9.b0, R7.b1
        CLR     R30, R30.t1

pwm3_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm3_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9.b0, R9.b0, R9.b0

start_pwm4: 
        SET     R30, R30.t3
        SUB     R14.b0, R0.b0, R8.b1
 
pwm4_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm4_high, R9.b0, R8.b1
        CLR     R30, R30.t3

pwm4_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm4_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9.b0, R9.b0, R9.b0

start_pwm5: 
        SET     R30, R30.t7
        SUB     R14.b0, R0.b0, R7.b2
        
pwm5_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm5_high, R9.b0, R7.b2
        CLR     R30, R30.t7

pwm5_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm5_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9.b0, R9.b0, R9.b0

start_pwm6: 
        SET     R30, R30.t14
        SUB     R14.b0, R0.b0, R8.b2
 
pwm6_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm6_high, R9.b0, R8.b2
        CLR     R30, R30.t14

pwm6_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm6_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9.w0, R9.w0, R9.w0

start_pwm7: 
        SET     R30, R30.t5
        SUB     R14.b0, R0.b0, R7.b3
        
pwm7_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm7_high, R9.b0, R7.b3
        CLR     R30, R30.t5

pwm7_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm7_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9.w0, R9.w0, R9.w0

start_pwm8: 
        SET     R30, R30.t15
        SUB     R14.b0, R0.b0, R8.b3
 
pwm8_high:
        ADD     R9, R9, 0x00000001
        QBNE    pwm8_high, R9.b0, R8.b3
        CLR     R30, R30.t15

pwm8_low:
        ADD     R15, R15, 0x00000001
        QBNE    pwm8_low, R15.b0, R14.b0
        SUB     R15, R15, R15
        SUB     R9.w0, R9.w0, R9.w0

        QBA     count_check

done_sample:
        QBA     sample_start

stop:
        HALT
