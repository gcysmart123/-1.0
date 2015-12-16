
#include "stc15.h"
#include "timer_2.h"
#include "timer.h"
#include "debug.h"
#include "memu.h"
#include "key.h"
#include "FUNCTION.h"

unsigned int waite_time = 0;
unsigned char shanshuo = 0;				//计数 红灯闪烁次数
#if DEBUG_FANHUI
unsigned char memu_dispaly_time = 0;	//计数100为5s 数码管回复“- -”
#endif

//#define T1MS (65536-FOSC/1000)      //1T模式
#define T1MS (65536-50000) //12T模式

//中断服务程序
void t2int() interrupt 12           //中断入口
{
#if DEBUG
	if (flag_Pattern == 3)
	{
		if (send_continue==0)
		{
			waite_time++;
			if (waite_time == waite_time_s)
			{
				waite_time = 0;
				waite_end = 1;

			}
		}
		else
		{
			waite_time++;
			if (waite_time == 20)
			{
				send_continue = 0;
				waite_time = 0;
				waite_end = 1;

			}
		}
#if DEBUG_SHANSHUO
		if (decode_success == 1)
		{
			LED_R = !LED_R;
			shanshuo++;
			if (shanshuo == 2)
			{
				decode_success = 0;
			}
		}
#endif
#if DEBUG_FANHUI
		if (memu_dispaly_time_start == 1)
		{
			memu_dispaly_time++;
			if (memu_dispaly_time == 100)
			{
				memu_dispaly_time_start = 0;
				memu_dispaly_time = 0;
				H = 12;
				L = 12;
				memu_count = 0;
			}
			//	return;
		}
#endif
	}
	else
	{
		waite_time = 0;
#if DEBUG_SHANSHUO
		if (decode_success == 1)
		{
			LED_R = !LED_R;
			shanshuo++;
			if (shanshuo == 2)
			{
				decode_success = 0;
			}
		}
#endif
#if DEBUG_FANHUI
		if (memu_dispaly_time_start == 1)
		{
			memu_dispaly_time++;
			if (memu_dispaly_time == 100)
			{
				memu_dispaly_time_start = 0;
				memu_dispaly_time = 0;
				H = 12;
				L = 12;
				memu_count = 0;
			}
			//	return;
		}
#endif
	}
#endif
	//  IE2 &= ~0x04;                   //若需要手动清除中断标志,可先关闭中断,此时系统会自动清除内部的中断标志
	//  IE2 |= 0x04;                    //然后再开中断即可
}

//-----------------------------------------------

/* main program */
void Init_Timer2()
{
//    AUXR |= 0x04;                   //定时器2为1T模式
  AUXR &= ~0x04;                  //定时器2为12T模式
    T2L = T1MS;                     //初始化计时值
    T2H = T1MS >> 8;
    AUXR |= 0x10;                   //定时器2开始计时
    
	IE2 |= 0x04;			 //开定时器中断
   

    
}

