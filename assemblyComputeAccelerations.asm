section .data
resultArray times 10000 dd 0
var1 dd 1000.0
var2 dd 3600.0

section .text
bits 64
default rel
global assemblyComputeAccelerations

assemblyComputeAccelerations:
    XOR R10, R10            ; Set R10 to 0, it will serve as the offset for input addressing.
    XOR R9, R9              ; Set R9 to 0, it will serve as the offset for output addressing.
    LEA R8, [resultArray]   ; Load base address of resultArray into R8

L1:
    ; XMM0 is initial velocity , XMM1 is final velocity, XMM2 is time.
    MOVSS XMM0, [RDX + R10]
    ADD R10, 4

    MOVSS XMM1, [RDX + R10]
    ADD R10, 4

    MOVSS XMM2, [RDX + R10]
    ADD R10, 4

    CALL CALCULATE

    LOOP L1

    LEA RAX, [resultArray]
    RET

CALCULATE:
    SUBSS XMM1, XMM0        ; finalVelocity - initialVelocity
    MULSS XMM1, [var1]      ; Multiply with 1000
    DIVSS XMM1, [var2]      ; Divide by 3600
    DIVSS XMM1, XMM2        ; Divide by time
    CVTSS2SI R11D, XMM1     ; Convert to integer
    MOV [R8 + R9], R11D     ; Store using base + offset
    ADD R9, 4
    RET