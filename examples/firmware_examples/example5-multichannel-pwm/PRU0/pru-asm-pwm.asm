;* Source written by Pratim Ugale <pratim.ugale@gmail.com>
;* This example demonstrates 4 channel (software generated)

	.cdecls "main_pru0.c"

PRU_SRAM .set 0x00010000            ; Set the location of PRU Shared Memory

	.clink
	.global setup
setup:                              ; One time setup.

        LDI32   R10, PRU_SRAM       ; R10 -> Base address of PRU SRAM

        SUB     R1, R1, R1          ; Clear the contents of R1
        SUB     R2, R2, R2          ; Clear the contents of R2
        SUB     R3, R3, R3          ; Clear the contents of R3
        SUB     R4, R4, R4          ; Clear the contents of R4
        SUB     R5, R5, R5          ; Clear the contents of R5
        SUB     R6, R6, R6          ; Clear the contents of R6
        SUB     R7, R7, R7          ; Clear the contents of R7
        SUB     R8, R8, R8          ; Clear the contents of R8

        ADD     R1.w0, R1.w0, 1           ; ON cycles of ch 1
        ADD     R2.w0, R2.w0, 2           ; ON cycles of ch 2
        ADD     R3.w0, R3.w0, 3           ; ON cycles of ch 3
        ADD     R4.w0, R4.w0, 4           ; ON cycles of ch 4

        ADD     R1.w2, R1.w2, 1           ; ON cycles of ch 1
        ADD     R2.w2, R2.w2, 2           ; ON cycles of ch 2
        ADD     R3.w2, R3.w2, 3           ; ON cycles of ch 3
        ADD     R4.w2, R4.w2, 4           ; ON cycles of ch 4

        ADD     R5.w0, R5.w0, 4           ; OFF cycles of ch 1
        ADD     R6.w0, R6.w0, 3           ; OFF cycles of ch 2
        ADD     R7.w0, R7.w0, 2           ; OFF cycles of ch 3
        ADD     R8.w0, R8.w0, 1           ; OFF cycles of ch 4

        ADD     R5.w2, R5.w2, 4           ; OFF cycles of ch 1
        ADD     R6.w2, R6.w2, 3           ; OFF cycles of ch 2
        ADD     R7.w2, R7.w2, 2           ; OFF cycles of ch 3
        ADD     R8.w2, R8.w2, 1           ; OFF cycles of ch 4

       ; LBBO    &R1, R10, 0, 4      ; R1 -> Duty Cycle.(actually ON cycles); Copy (4) bytes into R1 from memory address R10+offset(0)
       ; LBBO    &R2, R10, 4, 4      ; R2 -> Total Cycles. Copy (4) bytes into R2 from memory address R10+offset(5)
        LDI     R0, 0               ; R0 - Channel count
        QBA     startif

startif:                                 ; 
        QBEQ    elif_1, R1.w0, 0      ; If ON count of channel 1 is not equal to zero
        SUB     R1.w0, R1.w0, 1
        SET     R30, R30.t0           ; R30 = R30 | (1 << R0)
        QBA     if_2

elif_1: 
        QBEQ    else_1, R5.w0, 0
        SUB     R5.w0, R5.w0, 1
        CLR     R30, R30.t0 
        QBA     if_2

else_1: 
        MOV     R1.w0, R1.w2
        MOV     R5.w0, R5.w2

if_2:                                 ; 
        QBEQ    elif_2, R2.w0, 0      ; If ON count of channel 2 is not equal to zero
        SUB     R2.w0, R2.w0, 1
        SET     R30, R30.t1           ; R30 = R30 | (1 << R0)
        QBA     if_3

elif_2: 
        QBEQ    else_2, R6.w0, 0
        SUB     R6.w0, R6.w0, 1
        CLR     R30, R30.t1 
        QBA     if_3

else_2: 
        MOV     R2.w0, R2.w2
        MOV     R6.w0, R6.w2

if_3:                                 ; 
        QBEQ    elif_3, R3.w0, 0      ; If ON count of channel 2 is not equal to zero
        SUB     R3.w0, R3.w0, 1
        SET     R30, R30.t2           ; R30 = R30 | (1 << R0)
        QBA     if_4

elif_3: 
        QBEQ    else_3, R7.w0, 0
        SUB     R7.w0, R7.w0, 1
        CLR     R30, R30.t2
        QBA     if_4

else_3: 
        MOV     R3.w0, R3.w2
        MOV     R7.w0, R7.w2
        
if_4:                                 ; 
        QBEQ    elif_4, R4.w0, 0      ; If ON count of channel 2 is not equal to zero
        SUB     R4.w0, R4.w0, 1
        SET     R30, R30.t3           ; R30 = R30 | (1 << R0)
        QBA     startif

elif_4: 
        QBEQ    else_4, R8.w0, 0
        SUB     R8.w0, R8.w0, 1
        CLR     R30, R30.t3
        QBA     startif

else_4: 
        MOV     R4.w0, R4.w2
        MOV     R8.w0, R8.w2
        QBA     startif
