ORG 0000H
LJMP MAIN

ORG 0100H
MAIN:
    MOV A, #0FEH

    MOV P2, A

    LOOP:
        MOV P2, A
        LCALL DELAY

        RL A
        SJMP LOOP

DELAY:
    MOV R7, #200
D1: 
    MOV R6, #250
D2: 
    DJNZ R6, D2
    DJNZ R7, D1
    RET
END
