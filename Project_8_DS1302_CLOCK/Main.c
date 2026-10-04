#include <STC89C5xRC.H>
#include <INTRINS.H>
#include "LCD1602.h"
#include "Board_DS1302.h"

#pragma region 各种变量
unsigned char year_bcd;     //年
unsigned char mo_bcd;       //月
unsigned char date_bcd;     //日
unsigned char day_bcd;      //星期
unsigned char hour_bcd;     //小时
unsigned char min_bcd;      //分钟
unsigned char sec_bcd;      //秒
unsigned char year;
unsigned char mo;
unsigned char date;
unsigned char day;
unsigned char hour;
unsigned char min;
unsigned char sec;

volatile unsigned char Setting_Mode = 0;
volatile unsigned char Setting_Position = 1;
volatile unsigned char Blink_Flag = 1;
unsigned char Timer_Accumulator = 0;
unsigned char Write_Time_Flag = 0;
unsigned char LCD_Flash_Flag = 0;

#pragma endregion

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

void Global_INT_Init(void)
{
    EA = 1;
}

void INT1_Init(void)
{
    EX1 = 1;
    IT1 = 1;
}

void INT0_Init(void)
{
    EX0 = 1;
    IT0 = 1;
}

void INT0_Off(void)
{
    EX0 = 0;
}

void Timer0_Init(void)
{
    EA = 1;
    ET0 = 1;
    TMOD &= 0xF0;
    TMOD |= 0x01;
    TR0 = 1;
    TH0 = 0x4C;
    TL0 = 0x00;
}

void Day_Play(unsigned char day)
{
    switch(day)
    {
        case 1:
            LCD_ShowString(2, 12, "Mon  ");
            break;
        case 2:
            LCD_ShowString(2, 12, "Tues ");
            break;
        case 3:
            LCD_ShowString(2, 12, "Wed  ");
            break;
        case 4:
            LCD_ShowString(2, 12, "Thurs");
            break;
        case 5:
            LCD_ShowString(2, 12, "Fri  ");
            break;
        case 6:
            LCD_ShowString(2, 12, "Sat  ");
            break;
        case 7:
            LCD_ShowString(2, 12, "Sun  ");
            break;
    }
}

void DS1302_ReadTime(void)
{
    year_bcd = DS1302_ReadByte(0x8D);     //年
    mo_bcd = DS1302_ReadByte(0x89);       //月
    date_bcd = DS1302_ReadByte(0x87);     //日
    day_bcd = DS1302_ReadByte(0x8B);      //星期
    hour_bcd = DS1302_ReadByte(0x85);     //小时
    min_bcd = DS1302_ReadByte(0x83);      //分钟
    sec_bcd = DS1302_ReadByte(0x81);      //秒
        
    year = (year_bcd >> 4) * 10 + (year_bcd & 0x0F);
    mo = (mo_bcd >> 4) * 10 + (mo_bcd & 0x0F);
    date = (date_bcd >> 4) * 10 + (date_bcd & 0x0F);
    day = (day_bcd & 0x07);
    hour = ((hour_bcd & 0x30) >> 4) * 10 + (hour_bcd & 0x0F);
    min = (min_bcd >> 4) * 10 + (min_bcd & 0x0F);
    sec  = ((sec_bcd & 0x70) >> 4) * 10 + (sec_bcd & 0x0F);
}

unsigned char Calculate_Day(void)
{
    unsigned int y = 2000 + year;
    unsigned char code t[12] =
    {
        0, 3, 2, 5, 0, 3,
        5, 1, 4, 6, 2, 4
    };
    unsigned char w;

    if(mo < 3)
        y--;

    w = (y + y / 4 - y / 100 + y / 400 + t[mo - 1] + date) % 7;

    if(w == 0) return 7;

    return w;
}

void DS1302_WriteTime(void)
{
    year_bcd = ((year / 10) << 4 | (year % 10)); 
    mo_bcd = ((mo / 10 << 4) | (mo % 10)); 
    date_bcd = ((date / 10 << 4) | (date % 10)); 
    hour_bcd = ((hour / 10 << 4) | (hour % 10)); 
    min_bcd = ((min / 10 << 4) | (min % 10)); 
    sec_bcd = ((sec / 10 << 4) | (sec % 10)); 
    
    day = Calculate_Day();
    day_bcd = day;
    
    DS1302_Disable_WriteProtection();
    DS1302_WriteByte(0x8C,year_bcd);
    DS1302_WriteByte(0x88,mo_bcd);
    DS1302_WriteByte(0x86,date_bcd);
    DS1302_WriteByte(0x84,hour_bcd);
    DS1302_WriteByte(0x82,min_bcd);
    DS1302_WriteByte(0x80,sec_bcd);
    DS1302_WriteByte(0x8A, day_bcd);
    DS1302_Enable_WriteProtection();
}

void DS1302_TimeRst(void)
{
    DS1302_Disable_WriteProtection();
    DS1302_WriteByte(0x8C,0x00);
    DS1302_WriteByte(0x88,0x01);
    DS1302_WriteByte(0x86,0x01);
    DS1302_WriteByte(0x84,0x00);
    DS1302_WriteByte(0x82,0x00);
    DS1302_WriteByte(0x80,0x00);

    day = Calculate_Day();
    day_bcd = day;

    DS1302_WriteByte(0x8A, day_bcd);

    DS1302_Enable_WriteProtection();
}

void Change_Setting_Mode(void) interrupt 2
{
    TH0 = 0x4C;
    TL0 = 0x00;
    Setting_Mode = !Setting_Mode;
    if(!Setting_Mode)
    {
        Write_Time_Flag = 1;
    }
}

void Timer0_500ms_Routine(void) interrupt 1
{
    TH0 = 0x4C;
    TL0 = 0x00;
    if(++Timer_Accumulator >= 10)
    {
        Blink_Flag = !Blink_Flag;
        Timer_Accumulator = 0;
    }
}

void LCD_ShowTime(void)
{
    LCD_ShowString(1,1,"20");
    LCD_ShowNum(1, 3, year, 2);
    LCD_ShowNum(1, 6, mo, 2);
    LCD_ShowNum(1, 9, date, 2);
    Day_Play(day);
    LCD_ShowNum(2, 1, hour, 2);
    LCD_ShowNum(2, 4, min, 2);
    LCD_ShowNum(2, 7, sec, 2);
}

void Setting_Position_Change(void) interrupt 0
{
    ++Setting_Position;
    if(Setting_Position > 6) Setting_Position = 1;
    LCD_Flash_Flag = 1;
}

void Setting_Add(void)
{
    switch(Setting_Position)
    {
        case 1:
            ++year;
            mo = 1;
            date = 1;
            if(year > 99) year = 0;
            break;
        
        case 2:
            ++mo;
            date = 1;
            if(mo > 12) mo = 1;
            break;

        case 3:
            if(mo == 2)
            {
                if(year % 4 == 0)
                {
                    ++date;
                    if(date > 29) date = 1;
                }
                else
                {
                    ++date;
                    if(date > 28) date = 1;
                }
            }
            else if(mo == 1 || mo == 3 || mo == 5 || mo == 7 || mo == 8 || mo == 10 || mo == 12)
            {
                ++date;
                if(date > 31) date = 1;
            }
            else
            {
                ++date;
                if(date > 30) date = 1;
            }
            break;
        
        case 4:
            ++hour;
            if(hour >= 24) hour = 0;
            break;

        case 5:
            ++min;
            if(min >= 60) min = 0;
            break;
        
        case 6:
            ++sec;
            if(sec >= 60) sec = 0;
            break;

        default:
            Setting_Position = 1;
            break;
        
    }
}

void Setting_Sub(void)
{
    switch(Setting_Position)
    {
        case 1:
            --year;
            mo = 1;
            date = 1;
            if(year > 99) year = 99;
            break;
        
        case 2:
            --mo;
            date = 1;
            if(mo == 0 || mo > 12) mo = 12;
            break;

        case 3:
            --date;
            if(date == 0 || date > 31)
            {
                if(mo == 2)
                {
                    if(year % 4 == 0)
                    {
                        date = 29;
                    }
                    else
                    {
                        date = 28;
                    }
                }
                else if(mo == 1 || mo == 3 || mo == 5 || mo == 7 || mo == 8 || mo == 10 || mo == 12)
                {
                    date = 31;
                }
                else
                {
                    date = 30;
                }
            }
            break;
        
        case 4:
            --hour;
            if(hour > 23) hour = 23;
            break;

        case 5:
            --min;
            if(min > 59) min = 59;
            break;
        
        case 6:
            --sec;
            if(sec > 59) sec = 59;
            break;

        default:
            Setting_Position = 1;
            break;
        
    }
}

void Setting_Show(void)
{
    if(LCD_Flash_Flag)
    {
        LCD_Flash_Flag = 0;
        LCD_ShowTime();
    }

    switch (Setting_Position)
    {
    case 1:
        if(Blink_Flag)
        {
            LCD_ShowString(1,1,"    ");
        }
        else
        {
            LCD_ShowString(1,1,"20");
            LCD_ShowNum(1, 3, year, 2);
        }
        break;
    
    case 2:
        if(Blink_Flag)
        {
            LCD_ShowString(1,6,"  ");
        }
        else
        {
            LCD_ShowNum(1, 6, mo, 2);
        }
        break;
    
    case 3:
        if(Blink_Flag)
        {
            LCD_ShowString(1, 9,"  ");
        }
        else
        {
            LCD_ShowNum(1, 9, date, 2);
        }
        break;

    case 4:
        if(Blink_Flag)
        {
            LCD_ShowString(2,1,"  ");
        }
        else
        {
            LCD_ShowNum(2, 1, hour, 2);
        }
        break;
    
    case 5:
        if(Blink_Flag)
        {
            LCD_ShowString(2,4,"  ");
        }
        else
        {
            LCD_ShowNum(2, 4, min, 2);
        }
        break;
    
    case 6:
        if(Blink_Flag)
        {
            LCD_ShowString(2,7,"  ");
        }
        else
        {
            LCD_ShowNum(2, 7, sec, 2);
        }
        break;

    default:
        Setting_Position = 1;
        break;
    }
}

void main()
{
    LCD_Init();
    DS1302_Init();
    
    Global_INT_Init();
    INT0_Init();
    INT1_Init();
    Timer0_Init();
    
    LCD_ShowString(1,1,"20  -  -  ");
    LCD_ShowString(2,1,"  :  :");

    while(1)
    {
        if(!Setting_Mode)
        {
            DS1302_ReadTime();
            LCD_ShowTime();
        }
        else
        {
            while(P31 && P30 && Setting_Mode) Setting_Show();
            if(!P31 || !P30) Delay20ms();
            if(!P31)
            {
                Setting_Add();

                while(!P31);
            }
            else if(!P30)
            {
                Setting_Sub();

                while(!P30);
            }
        }
        if(Write_Time_Flag)
        {
            Write_Time_Flag = 0;

            DS1302_WriteTime();     

            LCD_Init();
            LCD_ShowString(1,1,"20  -  -  ");
            LCD_ShowString(2,1,"  :  :");
        }
    }
}
