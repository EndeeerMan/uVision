#include <REGX52.H>
#include "LCD1602.h"

void main(){
    LCD_Init();
    LCD_ShowString(1,1,"HELLO WORLD!");
    LCD_ShowNum(2,1,65535,5);
    while(1);
}