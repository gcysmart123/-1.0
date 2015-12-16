#include "STC15.h"
#include "delay.h"
/*由于硬件原因所以此处 数码管显示 做特殊处理 硬件修改后 代码要进行调整
unsigned char code duanma_1[13] = { 0x1d, 0x08, 0x54, 0x58, 0x49, 0x59, 0x5d, 0x08, 0x5d, 0x59, 0x55, 0x45, 0x40 };// 显示段码值0~9
unsigned char code duanma_5[13] = { 0x30, 0x10, 0x30, 0x30, 0x10, 0x20, 0x20, 0x30, 0x30, 0x30, 0x20, 0x20, 0x00 };// 显示段码值0~9

void display(unsigned char LED_H, unsigned char LED_L)
{

	P1 = P1 & 0X80;
	P5 = 0X00;
	P1 = P1 | 0X20;
	P1 = P1 | duanma_1[LED_H];
	P5 = duanma_5[LED_H];

	DelayMs(10);

	P1 = P1 & 0X80;
	P5 = 0X00;
	P1 = P1 | 0X02;
	P1 = P1 | duanma_1[LED_L];
	P5 = duanma_5[LED_L];

	DelayMs(10);
}							  */
unsigned char code duanma[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x79,0x71,0x40};

void display(unsigned char LED_H, unsigned char LED_L)
{
	P1 = P1 & 0X80;
	P5 = 0X00;
	P5 = 0X20;
	P1 = P1 | duanma[LED_H];
	DelayMs();

	P1 = P1 & 0X80;
	P5 = 0X00;
	P5 = 0X10;
	P1 = P1 | duanma[LED_L];
	DelayMs();
}