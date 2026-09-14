#include <REGX52.H> 
#include <INTRINS.H>

void Delay1ms(void)	//@11.0592MHz
{
	unsigned char data i, j;

	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}

void SelectPosi_and_Dis(unsigned char posi,unsigned char num){
	code unsigned char nums[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

	posi = 8-posi;
	P2_2 = posi & 1;
	posi >>= 1;
	P2_3 = posi & 1;
	posi >>= 1;
	P2_4 = posi & 1;

	P0 = nums[num];
}

void main() {
    unsigned long counter = 0;          // 升级为 32 位长整型，支持 8 位完整数字！
    unsigned char frame = 0;
    unsigned char disp_buf[8];          // 显示缓存数组：对应 1~8 位数码管
    unsigned char i;

    // 初始先分解一次数字
    for (i = 0; i < 8; i++) {
        disp_buf[i] = 0;
    }

    while (1) {
        // 使用一个循环搞定 8 位扫描，清爽利落
        for (i = 0; i < 8; i++) {
            SelectPosi_and_Dis(i + 1, disp_buf[i]);
            Delay1ms();
            P0 = 0x00;                  // 消影
        }
        
        // 满 125 帧（约 1 秒）步进一次
        if (++frame >= 125) {
            unsigned long temp;
            frame = 0;
            counter++;

            // 1 秒只做一次位分解，不再占用每一帧的扫描时间！
            temp = counter;
            for (i = 8; i > 0; i--) {
                disp_buf[i - 1] = temp % 10;
                temp /= 10;
            }
        }
    }
}