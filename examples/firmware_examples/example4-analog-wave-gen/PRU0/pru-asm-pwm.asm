;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* This example generates uses PWM to generate analog signal defined by the user(by writing data into the PRU SRAM)

	.cdecls "main_pru0.c"

PRU_SRAM .set 0x00010000            ; Set the location of PRU Shared Memory

	.clink
	.global start
start:                              ; One time setup.
        LDI32   R10, PRU_SRAM       ; R10 -> Base address of PRU SRAM
        SUB     R6, R6, R6          ; Clear the contents of R6
        SUB     R7, R7, R7          ; Clear the contents of R7
        LBBO    &R6.b0, R10, 0, 1      ; R6 -> Time delay; Copy (1) bytes into R6 from memory address R10+offset(0)
        LBBO    &R7.b0, R10, 1, 1      ; R7 -> Number of samples of the wave; Copy (1) bytes into R7 from memory address R10+offset(1)
        LDI     R11, 0              ; R11-> To keep a count of the samples being done.
      ; LBBO    &R8, R10, 2, 1      ; R8 -> Number of samples of the wave; Copy (1) bytes into R8 from memory address R10+offset(2)
        LDI     R12, 1              ; R12-> Keeps track of the memory location offsets
        
        ADD     R7.b0, R7.b0, 2
    
        QBA     sample_start

sample_start:                       ; 
        QBLT    same_sample, R11.b0, R7.b0          ;
        ;QBLT    same_sample, R12, R7          ;
        LDI     R12, 1              ; Reset for next sample 

same_sample:                        ; [Loop consuming 2 PRU cycles per iteration]
        ADD     R12, R12, 1         ; Increment offset counter by 1 
        LBBO    &R13.b0, R10, R12.b0, 1   ; Repeat loop until ON_Cycles 
        QBLT    greaterthan_zero, R13.b0, 1  ; Repeat loop until ON_Cycles 
        LDI     R13.b0, 1

greaterthan_zero:                         ; [Loop consuming 2 PRU cycles per iteration]
        QBGT    corrected_values, R13.b0, 100  ; Repeat loop until ON_Cycles 
        LDI     R13.b0, 99

corrected_values:
        LDI32     R14, 200            ; Total  cycles for 1MHz pwm frequency
        LDI32     R15, 5000          ; Number of pulses
        LDI32     R16, 0              ; Pulse Counter
        LDI32     R17, 0

count_check:
        ADD     R16, R16, 1
        QBEQ    done_sample, R16, R15        ; Stop if the total number of pulses have been generated.

start_pwm: 
        SET     R30, R30.t0

pwm_high:
        ADD     R17, R17, 0x00000001
        QBNE    pwm_high, R17.b0, R13.b0
        NOP
        NOP
        CLR     R30, R30.t0
pwm_low:
        ADD     R17, R17, 0x00000001
        QBNE    pwm_low, R17, R14
        SUB     R17, R17, R17
        QBA count_check

done_sample
        ADD     R11.b0, R11.b0, 1 
        ;QBBS    stop, R31.t0
        QBA     sample_start

stop: 
	HALT
