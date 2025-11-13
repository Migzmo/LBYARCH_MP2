section .data
resultArray times 10000 dd 0
var1 dd 1000.0
var2 dd 3600.0
section .text
bits 64
Default rel

global assemaccel
assemaccel:
    ;write your code here
    ;xmm0 is initial velocity , xmm1 is final velocity, xmm2 is time
    xor r10,r10
    xor r9,r9
L1:
    
    MOVSS xmm0, [RDX + R10]
    ADD R10,4
    MOVSS xmm1, [RDX +R10]
    ADD R10,4 
    MOVSS xmm2, [RDX +R10]
    ADD R10,4
    
    CALL CALCULATE
    
    LOOP L1 
    RET
CALCULATE:
    SUBSS xmm1,xmm0;subtracvct init with final
    
    
    ;Conversion
    MULSS xmm1,[var1];multiply with 1k
    DIVSS xmm1,[var2];divide by 3600
    DIVSS xmm01xmm2
    CVTSS2SI EBX, xmm1; Put first in EBX
    LEA R8, [resultArray]
    MOV [R8+R9],EBX
    ADD R9,4
    RET