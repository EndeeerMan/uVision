#include <REG52.H>
#include "LCD1602.h"

void UART_Init(void)
{
    SCON = 0x50;

    TMOD &= 0x0F;
    TMOD |= 0x20;

    PCON &= 0x7F;

    TH1 = 0xFD;
    TL1 = 0xFD;

    TR1 = 1;
}

void UART_SendByte(unsigned char dat)
{
    SBUF = dat;

    while(TI == 0);

    TI = 0;
}

unsigned char UART_ReceiveByte(void)
{
    unsigned char dat;

    while(RI == 0);

    RI = 0;
    dat = SBUF;

    return dat;
}

void main(void)
{
    unsigned char dat;
    unsigned char val1=1;
    unsigned char val2=1;

    UART_Init();
    LCD_Init();

    while(1)
    {
        dat = UART_ReceiveByte();
        LCD_ShowChar(val1,val2,dat);
        ++val2;
        if(val2>16){
            val2 = 1;
            if(val1 == 1) val1 = 2;
            else val1 = 1;
        }
        
        UART_SendByte(dat);
    }
}