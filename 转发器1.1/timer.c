#include "timer.h"
#include "ev1527.h"
#include "stc15.h"

unsigned char await_number = 0;      //待机时刻计算50MS次数变量
unsigned char await_number_table = 0;//0.5秒标志变量

unsigned char main_press_time_table = 0;//main计算时间变量跟标志位
unsigned char main_press_time = 0;

unsigned char again_and_again_time = 0;// 判断重复解码所需要的计算时间变量

unsigned char logout_cycle_number = 0; //销号 循环 计数变量以及标志位
unsigned char logout_cycle_table = 0;

unsigned char return_standby_time = 0;//一段时间未操作  返回待机界面

unsigned char second_times = 0;

unsigned char fd_table = 0;

unsigned char filter_main = 0;

unsigned int  send_delay = 0;			//发送时间累加计数（7500为0.8s）

unsigned char send_start;				//发送开始标志位
unsigned char delay_time;				//停止 延时时间
unsigned char send_time = 1;			//发送时间
unsigned char flag_Pattern= 0;			/*工作模式 标志
											0：只接收 
											1：发送（0.8s）
											2：接收并计算延时*/

unsigned char times = 2;					//预定次数标志
unsigned char send_times = 0;			//已经发送次数

//unsigned char send_end;				//发送停止标志
unsigned char received_end;				//解=接收停止标志

unsigned int waite_time_s = 20;			//缓存时间
unsigned char waite_end = 0;			//缓存结束标志

unsigned char repeat_time = 120;

void Init_Timer0(void) 
{
	TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响	
	TH0 = (65536 - 100) >> 8; //重新赋值 100us
	TL0 = (65536 - 100) & 0xff;
	EA = 1;            //总中断打开
	ET0 = 1;           //定时器中断打开
	//TR1=1;           //定时器开关打开
}

void Timer0_isr(void) interrupt 1  //定时器1中断服务程序
{
	TF0 = 0;
	TH0 = (65536 - 100) >> 8;		  //重新赋值 100us
	TL0 = (65536 - 100) & 0xff;
	if (flag_Pattern == 1)
	{
		send_delay++;
		//LED_G = 1;
		if (send_delay == 7500)
		{
			send_delay = 0;
			LED_G = 0;
			send_start = 0;

		}
	}
	else
	{
		RF_decode_main();
	}
	//	TF0 = 0;
	//	TH0 = (65536 - 100) >> 8;		  //重新赋值 100us
	//	TL0 = (65536 - 100) & 0xff;
	//	switch (flag_Pattern)
	//	{
	//	case 0:
	//		RF_decode_main();
	//		break;
	//	case 1:
	//		send_delay++;
	//		//LED_G = 1;
	//		if (send_delay == 7500)
	//		{
	//			send_delay = 0;
	//			LED_G = 0;
	//			send_start = 0;
	//			
	//		}
	//		break;
	//	case 2:
	//		RF_decode_main();
	//		break;
	//	case 3:
	//		RF_decode_main();
	//		break;
	//	default:
	//		break;
	//	}


		/*if(send_start == 1)
		{	
			send_delay++;
			if (send_delay==7500)
			{
				send_start = 0;
				send_delay = 0;
				LED_G = !LED_G;
			}
		}
		else
		{
			delay_time++;
			if (delay_time<send_time)
			{
				RF_decode_main();
			}
		}	*/
}

void clear_return_standby_time(void)
{
	return_standby_time = 0;
}