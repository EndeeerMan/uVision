#include <STC89C5xRC.H>
#include "LCD1602.h"

volatile unsigned char hour = 0;
volatile unsigned char min = 0;
volatile unsigned char sec = 0;
volatile unsigned char Time_FLAG = 0;
volatile unsigned char LCD_FLAG = 1;

void CLOCK0_Init(void)
{
    TMOD = 0x21;
    ET0 = 1;    //允许 Timer0 中断
    EA = 1;     //开启中断总开关
    TH0 = 0x4C;
    TL0 = 0x00;
    TR0 = 1;
}

void UART_Init(void)	//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器时钟12T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFD;			//设置定时初始值
	TH1 = 0xFD;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
    ES = 1;             //打开串口中断
}

void CLOCK0_Routine(void) interrupt 1
{
    TH0 = 0x4C;
    TL0 = 0x00;

    ++Time_FLAG;
    if(Time_FLAG >=20){
        Time_FLAG = 0;
        ++sec;
        if(sec>=60){
            sec = 0;
            ++min;
            if(min >= 60){
                min = 0;
                ++hour;
            }
        }
        LCD_FLAG = 1;
    }
}

void UART_RecvInt(void) interrupt 4
{ 
    if(RI == 1)
    {
        unsigned char buf = SBUF;
        if(buf == 'R')
        {
            hour = 0;
            min = 0;
            sec = 0;
            Time_FLAG = 0;
            LCD_FLAG = 1;
        }
        RI = 0;
    }
}

void UART_SendByte(unsigned char dat)
{
    SBUF = dat;
    while(TI == 0);
    TI = 0;
}

void main()
{
    LCD_Init();
    LCD_ShowString(1,1,"Clock:");
    LCD_ShowString(2,11,":  :");
    CLOCK0_Init();
    UART_Init();
    while(1){
        while(LCD_FLAG == 0);
        LCD_FLAG = 0;
        UART_SendByte((hour / 10)+48);
        UART_SendByte((hour % 10)+48);
        UART_SendByte(':');
        UART_SendByte((min / 10)+48);
        UART_SendByte((min % 10)+48);
        UART_SendByte(':');
        UART_SendByte((sec / 10)+48);
        UART_SendByte((sec % 10)+48);
        UART_SendByte('\n');
        LCD_ShowNum(2,15,sec,2);
        LCD_ShowNum(2,12,min,2);
        LCD_ShowNum(2,9,hour,2);
    }
}