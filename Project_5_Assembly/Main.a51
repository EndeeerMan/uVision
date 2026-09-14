ORG 0000H       ; 程序起始地址
LJMP MAIN       ; 跳转到主程序

ORG 0100H       ; 主程序存放地址
MAIN:
    CLR P2.0    ; 将P1.0输出低电平，点亮LED
    LCALL DELAY ; 调用延时子程序
    SETB P2.0   ; 将P1.0输出高电平，熄灭LED
    LCALL DELAY ; 调用延时子程序
    LJMP MAIN   ; 循环重复

; 简单的延时子程序
DELAY:
    MOV R7, #200
D1: 
    MOV R6, #250
D2: 
    DJNZ R6, D2 ; R6减1，不为0则循环
    DJNZ R7, D1 ; R7减1，不为0则循环
    RET         ; 返回主程序
END             ; 汇编程序结束
