#include <STC89C5xRC.H>
#include <INTRINS.H>

sbit RCK = P3^5;        //RCLK
sbit SCK = P3^6;        //SRCLK
sbit SER = P3^4;        //SER

void Delay2ms(void)	//@11.0592MHz
{
	unsigned char data i, j;

	_nop_();
	i = 4;
	j = 146;
	do
	{
		while (--j);
	} while (--i);
}

void _74595HC_WriteByte(unsigned char x)  
{
    unsigned char mod = 0x01;
    unsigned char i;
    for(i=0;i<=7;++i){
        SER = (x & (mod<<i)) ? 1 : 0;
        SCK = 1;
        SCK = 0;
    }
    RCK = 1;
    RCK = 0;
}

void Matrix_LED_Column_Scan(unsigned char column, unsigned char dat)
{
    _74595HC_WriteByte(dat);
    P0 = ~(0x01 << (column - 1));
}

void main(void)
{
    unsigned char dat[8] = {0x00,0x0C,0x2C,0x40,0x40,0x2C,0x0C,0x00};
    SCK = 0;
    RCK = 0;
    P0 = 0x00;
    while(1)
    {
        unsigned char i;
        for(i=1;i<=8;++i){
            Matrix_LED_Column_Scan(i + 1, dat[i]);
            Delay2ms();
            P0 = 0xFF;
        }
    }
}