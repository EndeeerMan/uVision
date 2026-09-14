#include <REGX52.H>
#include <INTRINS.H>
#include "LCD1602.h"

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

void Delay300ms(void)	//@11.0592MHz
{
	unsigned char data i, j, k;

	_nop_();
	i = 3;
	j = 26;
	k = 223;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

unsigned char Line_Scan(void){
    P1 = 0xF0;
    if(P1 != 0xF0){
        return P1;
    }
    return 0xF0;
}

unsigned char Scan(void){
    while(1){
        P1 = 0x0F;
        if(P1 != 0x0F){
            Delay20ms();
        }
        if(P1 != 0x0F){
            switch(P1){
                case 0x0E:
                    switch(Line_Scan()){
                        case 0x70:
                            return 10;  // +
                        case 0xB0:
                            return 11;  // -
                        case 0xD0:
                            return 12;  // *
                        case 0xE0:
                            return 13;  // /
                    }
                    break;
                case 0x0D:
                    switch(Line_Scan()){
                        case 0x70:
                            return 3;
                        case 0xB0:
                            return 6;
                        case 0xD0:
                            return 9;
                        case 0xE0:
                            return 14; // Enter
                    }
                    break;

                case 0x0B:
                    switch(Line_Scan()){
                        case 0x70:
                            return 2;
                        case 0xB0:
                            return 5;
                        case 0xD0:
                            return 8;
                        case 0xE0:
                            return 0;
                    }
                    break;

                case 0x07:
                    switch(Line_Scan()){
                        case 0x70:
                            return 1;
                        case 0xB0:
                            return 4;
                        case 0xD0:
                            return 7;
                        case 0xE0:
                            return 15;  //Del
                    }
                    break;
            }
        }
        P1 = 0x0F;
        while(P1 != 0x0F);
        if(P1 == 0x0F){
            Delay20ms();
        }
    }
}

void main(){
    LCD_Init();

    while(1){
        unsigned char i;
        unsigned int sum1 = 0;
        unsigned int sum2 = 0;

        while(1){
            i = Scan();
            Delay300ms();

            if(i <= 9){
                sum1 = sum1 * 10 + i;
            }
            if(i == 15){
                sum1 /= 10;
            }
            if(i >= 10 && i != 15){
                break;
            }
            LCD_ShowNum(1,1,sum1,5);
        }

        switch (i){
            case 10:
                LCD_ShowChar(1,6,'+');
                while(1){
                    i = Scan();
                    Delay300ms();

                    if(i <= 9){
                        sum2 = sum2 * 10 + i;
                    }
                    if(i == 15){
                        sum2 /= 10;
                    }
                    if(i == 14){
                        break;
                    }
                    if(i > 9 && i != 15){
                        continue;
                    }
                    LCD_ShowNum(1,7,sum2,5);
                }
                LCD_ShowChar(2,11,'=');
                LCD_ShowNum(2,12,sum1+sum2,5);
                while(P3_1 != 0);
                LCD_Init();
                break;

            case 11:
                LCD_ShowChar(1,6,'-');
                while(1){
                    i = Scan();
                    Delay300ms();

                    if(i <= 9){
                        sum2 = sum2 * 10 + i;
                    }
                    if(i == 15){
                        sum2 /= 10;
                    }
                    if(i == 14){
                        break;
                    }
                    if(i > 9 && i != 15){
                        continue;
                    }
                    LCD_ShowNum(1,7,sum2,5);
                }
                LCD_ShowChar(2,11,'=');
                LCD_ShowNum(2,12,sum1-sum2,5);
                while(P3_1 != 0);
                LCD_Init();
                break;

            case 12:
                LCD_ShowChar(1,6,'x');
                while(1){
                    i = Scan();
                    Delay300ms();

                    if(i <= 9){
                        sum2 = sum2 * 10 + i;
                    }
                    if(i == 15){
                        sum2 /= 10;
                    }
                    if(i == 14){
                        break;
                    }
                    if(i > 9 && i != 15){
                        continue;
                    }
                    LCD_ShowNum(1,7,sum2,5);
                }
                LCD_ShowChar(2,11,'=');
                LCD_ShowNum(2,12,sum1*sum2,5);
                while(P3_1 != 0);
                LCD_Init();
                break;

            case 13:
                LCD_ShowChar(1,6,'/');
                while(1){
                    i = Scan();
                    Delay300ms();

                    if(i <= 9){
                        sum2 = sum2 * 10 + i;
                    }
                    if(i == 15){
                        sum2 /= 10;
                    }
                    if(i == 14){
                        break;
                    }
                    if(i > 9 && i != 15){
                        continue;
                    }
                    LCD_ShowNum(1,7,sum2,5);
                }
                LCD_ShowChar(2,11,'=');
                LCD_ShowNum(2,12,sum1/sum2,5);
                while(P3_1 != 0);
                LCD_Init();
                break;
            
            default:
                break;
        }

    }
}
