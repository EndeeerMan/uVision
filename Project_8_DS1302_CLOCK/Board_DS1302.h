#ifndef __BOARD_DS1302_H__
#define __BOARD_DS1302_H__

/* Initialize DS1302 */
void DS1302_Init(void);
unsigned char DS1302_ReadByte(unsigned char cmd);
void DS1302_WriteByte(unsigned char cmd, unsigned char dat);
void DS1302_Disable_WriteProtection(void);
void DS1302_Enable_WriteProtection(void);

#endif
