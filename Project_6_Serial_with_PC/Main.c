#include <REGX52.H>
#include "LCD1602.h"

unsigned char str[32];
unsigned char str_val1=0;

void Reset_str(void){
    unsigned char i;
    for(i=0;i<32;++i){
        str[i] = ' ';
    }
}

void Delay20ms(void)	//@11.0592MHz
{
	unsigned char data i, j;

	i = 36;
	j = 217;
	do
	{
		while (--j);
	} while (--i);
}

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

void Int0_init(void){
    EA = 1;
    EX0 = 1;
    IT0 = 1;
}

void Int0_Routine(void) interrupt 0{
    unsigned char i;
    Delay20ms();
    if(P3_2 == 0){
        for(i=0;i<32;++i){
            if(i>15) LCD_ShowChar(2,i-15,str[i]);
            else LCD_ShowChar(1,i+1,str[i]);
        }
        Reset_str();
        str_val1 = 0;
    }
    while(P3_2 == 0);
    Delay20ms();
}

void main(void)
{
    unsigned char dat;

    UART_Init();
    LCD_Init();
    Int0_init();
    Reset_str();

    while(1)
    {
        dat = UART_ReceiveByte();
        str[str_val1] = dat;
        ++str_val1;
        if(str_val1>31) str_val1=0;
    }
}