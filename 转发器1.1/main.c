#include "stc15.h"
#include "ev1527.h"
#include "timer.h"
#include "timer_2.h"
#include "pca.h"
#include "memu.h"
#include "FUNCTION.h"
#include "delay.h"

void main ()
{
	Init_Timer0();

	Init_Timer2();
		
	exint0_init();

 	pca_timer_init();

	P3M1 = 0X04;			//设置 P3.2为高阻状态 否则无法解码 P3.5为推挽输出 否则无法发出信号
	P3M0 = 0X20;

	P1M1 = 0X00;			
	P1M0 = 0XFF;

	P5M0 = 0X30;
	P5M1 = 0X00;

	LED_G = 0;
	LED_R = 1;
	P35 = 0;

 	while(1)
	{  		
		function();
		memu();
	
	}
}