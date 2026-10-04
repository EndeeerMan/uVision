#include <STC89C5xRC.H>

sbit DS1302_CE   = P3^5;
sbit DS1302_SCLK = P3^6;
sbit DS1302_IO   = P3^4;

void DS1302_Init(void)
{
    DS1302_CE = 0;
    DS1302_SCLK = 0;
    DS1302_IO = 0;
}

unsigned char DS1302_ReadByte(unsigned char cmd)
{
    unsigned char i;
    unsigned char dat = 0x00;

    DS1302_CE = 1;

    for(i=0;i<8;++i)
    {
        DS1302_IO = cmd & 0x01;
        cmd >>= 1;

        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
    }

    for(i=0;i<8;++i)
    {
        if(DS1302_IO)
        {
            dat |= (0x01 << i);
        }

        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
    }

    DS1302_Init();

    return dat;
}

void DS1302_WriteByte(unsigned char cmd, unsigned char dat)
{
    unsigned char i;

    DS1302_CE = 1;

    for(i=0;i<8;++i)
    {
        DS1302_IO = cmd & 0x01;
        cmd >>= 1;

        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
    }

    for(i=0;i<8;++i)
    {
        DS1302_IO = dat & 0x01;
        dat >>= 1;

        DS1302_SCLK = 1;
        DS1302_SCLK = 0;
    }

    DS1302_Init();
}

void DS1302_Disable_WriteProtection(void)
{
    DS1302_WriteByte(0x8E, 0x00);
}

void DS1302_Enable_WriteProtection(void)
{
    DS1302_WriteByte(0x8E, 0x80);
}