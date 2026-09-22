#ifndef BOARD_LED_74595HC_H
#define BOARD_LED_74595HC_H

void MatrixLED_Init(void);
void _74HC595_WriteByte(unsigned char dat);
void MatrixLED_ColumnScan(unsigned char column, unsigned char dat);

#endif /* BOARD_LED_74595HC_H */