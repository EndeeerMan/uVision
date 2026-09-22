#include <STC89C5xRC.H>
#include "Board_LED_74595HC.h"

sbit RCK = P3^5;        //RCLK
sbit SCK = P3^6;        //SRCLK
sbit SER = P3^4;        //SER

void MatrixLED_Init(void)
{
    SCK = 0;
    RCK = 0;
    P0 = 0xFF;
}

void _74595HC_WriteByte(unsigned char dat)  
{
    unsigned char i;

    for (i = 0; i < 8; i++)
    {
        SER = (dat >> i) & 0x01;
        SCK = 1;
        SCK = 0;
    }

    RCK = 1;
    RCK = 0;
}

void MatrixLED_ColumnScan(unsigned char column, unsigned char dat)
{
    _74595HC_WriteByte(dat);
    P0 = ~(0x01 << (column - 1));
}