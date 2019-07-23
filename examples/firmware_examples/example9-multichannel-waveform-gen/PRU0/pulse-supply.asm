;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* This example generates uses PWM to generate analog waveform defined by the user(by writing data into the PRU DRAM0)
;* 1 PULSE  => 1 HIGH voltage value of PWM
;* 1 SAMPLE => Wave

	.cdecls "main_pru0.c"

PRU_DRAM0 .set 0x00000000                       ; Set the location of PRU0 Data Memory 

	.clink
	.global start
start:                                          ; One time setup.
        LDI32   R10, PRU_DRAM0                  ; R10 -> Base address of PRU DRAM0
        SUB     R7, R7, R7                      ; Clear the contents of R7
        SUB     R20, R20, R20                      ; Clear the contents of R7
        LBBO    &R7.b0, R10, 0, 1               ; R7 -> Number of samples of the wave; Copy (1) bytes into R7 from memory address R10+offset(1)
        LBBO    &R23.b0, R10, 1, 1               ; R7 -> Number of samples of the wave; Copy (1) bytes into R7 from memory address R10+offset(1)
        LDI     R11, 1                          ; R11-> To keep a count of the samples being done.
        LDI     R12, 1                          ; R12-> Keeps track of the memory location offsets
        LDI     R25, 1                          ; R11-> To keep a count of the samples being done.
        LDI     R26, 101                          ; R12-> Keeps track of the memory location offsets
        
        LDI     R27, 0
        QBA     sample_start

sample_start:                                   ; Start working on a "sample" of the waveform.
        QBLT    same_sample, R7.b0, R11.b0      ; Check if all samples have been worked on. If yes, then go back to 1st sample 
        LDI     R11, 1                          ; Reset for next sample 
        LDI     R12, 1                          ; Reset for next sample 

same_sample:                                    ; 
        ADD     R12, R12, 1                     ; Increment offset counter by 1 
        LBBO    &R13.b0, R10, R12.b0, 1         ; Load current sample value (duty cycle) into R13
        QBLT    greaterthan_zero, R13.b0, 1     ; In case the sample value is less than out lower limit of 1. 
        LDI     R13.b0, 1                       ; Set it to 1 to avoid any discrepancy

greaterthan_zero:                               ; Ensured that all samples are greater than zero
        QBGT    corrected_values, R13.b0, 100   ; In case the sample value is greater than our upper limit of 100.
        LDI     R13.b0, 99                      ; Set it to 99 to avoid any discrepancy

corrected_values:
        LDI32   R14, 100                        ; Total  cycles for 1MHz PWM frequency. Change it according to the PWM frequency required.
        LDI32   R15, 10000                      ; Number of pulses *10000 pulses = 1 second time period of the input wave for 100 samples.*
        LDI32   R16, 0                          ; R16 => Pulse Counter
        LDI32   R17, 0                          ; R17 => Counts upto OnCycles for the current duty cycle. 

count_check:
        ADD     R16, R16, 1                     ; Increment the pulse count by 1.
        QBEQ    done_sample, R16, R15           ; Stop if the total number of pulses have been generated.
        QBEQ    sample_start2, R27, 0

                                                ; Once we know the duty cycle, start the PWM on it for the mentioned amount of cycles. 
        
pwm_high:                                       ; [Loop consuming 2 cycles 100 times. Therefore PWM frequency = 200MHz/200 = 1MHz.]
        SET     R30, R30.t0                     ; Set GPIO P9_31 to high.
        ADD     R17, R17, 0x00000001            ; Increment counter for OnCycles.
        QBNE    pwm_high, R17, R13              ; Check if OnCycles have been completed. ie DC is over; if yes, then turn P9_31 to LOW.
        ;QBA     pwm_high2

pwm_high_ret:
        NOP
        NOP
pwm_low:                                        ; [Loop consuming 2 cycles 100 times. Therefore PWM frequency = 200MHz/200 = 1MHz.]
        CLR     R30, R30.t0                     ; GPIO P9_31 LOW
        ADD     R17, R17, 0x00000001            ; Increment remaining counter till TotalCycles.
        QBNE    pwm_low, R17, R14               ; Check if TotalCycles have been completed. i.e. PWM cycle is over.
        SUB     R17, R17, R17                   ; Clear the counter, if yes.
        ;QBA     pwm_low2
        QBA     pwm_high2

pwm_low_ret:
        QBA     count_check                         ; Check if required number of pulses have been generated. THIS CAN CHANGE THE TIME PERIOD OF THE RESULTANT WAVEFORM.

done_sample                                     ; Work on the current sample has been done.
        ADD     R11.b0, R11.b0, 1               ; Increment to move on to the next sample.
        QBA     sample_start                    ; Start working on the next sample.


sample_start2:                                   ; Start working on a "sample" of the waveform.
        LDI     R27, 1
        QBLT    same_sample2, R23.b0, R25.b0      ; Check if all samples have been worked on. If yes, then go back to 1st sample 
        LDI     R25, 1                          ; Reset for next sample 
        LDI     R26, 1                          ; Reset for next sample 

same_sample2:                                    ; 
        ADD     R26, R26, 1                     ; Increment offset counter by 1 
        LBBO    &R21.b0, R10, R26.b0, 1         ; Load current sample value (duty cycle) into R13
        QBLT    greaterthan_zero2, R21.b0, 1     ; In case the sample value is less than out lower limit of 1. 
        LDI     R21.b0, 1                       ; Set it to 1 to avoid any discrepancy

greaterthan_zero2:                               ; Ensured that all samples are greater than zero
        QBGT    corrected_values2, R21.b0, 100   ; In case the sample value is greater than our upper limit of 100.
        LDI     R21.b0, 99                      ; Set it to 99 to avoid any discrepancy

corrected_values2:
        LDI32   R22, 100                        ; Total  cycles for 1MHz PWM frequency. Change it according to the PWM frequency required.
        LDI32   R28, 10000                      ; Number of pulses *10000 pulses = 1 second time period of the input wave for 100 samples.*
        LDI32   R29, 0                          ; R16 => Pulse Counter
        LDI32   R20, 0                          ; R17 => Counts upto OnCycles for the current duty cycle. 

count_check2:
        ADD     R29, R29, 1                     ; Increment the pulse count by 1.
        QBEQ    done_sample2, R29, R28           ; Stop if the total number of pulses have been generated.
        QBA     pwm_low_ret

pwm_high2:
        SET     R30, R30.t2
        ADD     R20, R20, 0x0000001
        QBNE    pwm_high2, R20, R21
        ;QBA     pwm_high_ret

pwm_low2:
        CLR     R30, R30.t2
        ADD     R20, R20, 0x0000001
        QBNE    pwm_low2, R20, R22
        SUB     R20, R20, R20                   ; Clear the counter, if yes.
        
done_sample2:                                     ; Work on the current sample has been done.
        ADD     R25.b0, R25.b0, 1               ; Increment to move on to the next sample.
        QBA     sample_start2                    ; Start working on the next sample.

stop: 
	HALT                                    ; If required.
