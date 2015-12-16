#include "STC15.h"
#include "delay.h"
#include "FUNCTION.h"
#include "debug.h"
#include "timer_2.h"

#define KEY_EST  	P37
#define KEY_UP   	P36
#define KEY_DOWN 	P34
#define KEY_ESC 	P33
//unsigned char u;

//void display (unsigned char LED_H,unsigned char LED_L);
unsigned char memu_dispaly_time_start = 0;			//菜单显示时间计时开始标志

void key()
{ 

	if (!KEY_EST)  //如果检测到低电平，说明按键按下
	{
		DelayMs(); //延时去抖，一般10-20ms
		if (!KEY_EST)     //再次确认按键是否按下，没有按下则退出
		{
			while (!KEY_EST);//如果确认按下按键等待按键释放，没有释放则一直等待
			{
				
				memu_count++;
				switch (memu_count)
				{
				case 1:
					H = 11;
					L = 1;
					f_num = L;
					break;
				case 2:
					if (f_num == 1)
					{
						H = f_num_3_h;
						L = f_num_3_l;
					}
					else if (f_num == 2)
					{
						H = f_num_4_h;
						L = f_num_4_l;
					}
					else
					{
						H = f_num_5_h;
						L = f_num_5_l;
					}

					break;
				case 3:
					memu_count = 0;
					H = L = 12;
					break;
				default:
					break;//默认的break
				}
#if DEBUG_FANHUI
				memu_dispaly_time_start = 1;
				memu_dispaly_time = 0;
#endif
			}
		}
		//display(H,L);
	}


	if (!KEY_UP)  //如果检测到低电平，说明按键按下
	{
		DelayMs(); //延时去抖，一般10-20ms
		if (!KEY_UP)     //再次确认按键是否按下，没有按下则退出
		{
			while (!KEY_UP);//如果确认按下按键等待按键释放，没有释放则一直等待
			{
				switch (memu_count)
				{
				case 1:
					f_num++;
					if (f_num == 4)
					{
						f_num = 1;
					}
					L = f_num;
					break;
				case 2:
					switch (f_num)
					{
					case 1:
						if (L == 9)L = 0;
						else L++;

						f_num_3_h = H;
						f_num_3_l = L;
						break;
					case 2:
						L++;
						if (L >= 10)
						{
							L = L % 10;
							H++;
							if (H >= 10)
							{
								H = 0;
								L = 0;
							}/*
							L = L % 10;
							H++;*/
						}

						f_num_4_h = H;
						f_num_4_l = L;
						break;
					case 3:
						L++;
						if (L >= 10)
						{
							L = L % 10;
							H++;
							if (H >= 10)
							{
								H = 0;
								L = 0;
							}/*
							L = L % 10;
							H++;*/
						}

						f_num_5_h = H;
						f_num_5_l = L;
						break;
					}
					break;
				default:
					break;
				}
#if DEBUG_FANHUI
				memu_dispaly_time_start = 1;
				memu_dispaly_time = 0;
#endif
			}
		}
		//display(H,L);
	}


	if (!KEY_DOWN)  //如果检测到低电平，说明按键按下
	{
		DelayMs(); //延时去抖，一般10-20ms
		if (!KEY_DOWN)     //再次确认按键是否按下，没有按下则退出
		{
			while (!KEY_DOWN);//如果确认按下按键等待按键释放，没有释放则一直等待
			{
				switch (memu_count)
				{
				case 1:
					f_num--;
					if (f_num == 0)
					{
						f_num = 3;
					}
					L = f_num;
					break;
				case 2:
					switch (f_num)
					{
					case 1:
						if (L==0)
							L = 9;
						else L--;

						f_num_3_h = H;
						f_num_3_l = L;
						break;
					case 2:
						if (L == 0)
						{
							if (H == 0)
							{
								H = 9;
								L = 9;
							}
							else
							{
								H--;
								L = 9;
							}
						}
						else
							L--;

						f_num_4_h = H;
						f_num_4_l = L;

						break;
					case 3:
						if (L == 0)
						{
							if (H == 0)
							{
								H = 9;
								L = 9;
							}
							else
							{
								H--;
								L = 9;
							}
						}
						else
							L--;

						f_num_5_h = H;
						f_num_5_l = L;
						break;

					}
					break;

				default:
					break;//默认的break
				}
#if DEBUG_FANHUI
				memu_dispaly_time_start = 1;
				memu_dispaly_time = 0;
#endif
			}
		}
		//display(H,L);
	}


	if (!KEY_ESC)  //如果检测到低电平，说明按键按下
	{
		DelayMs(); //延时去抖，一般10-20ms
		if (!KEY_ESC)     //再次确认按键是否按下，没有按下则退出
		{
			while (!KEY_ESC);//如果确认按下按键等待按键释放，没有释放则一直等待
			{
				if (memu_count == 0)
				{
					memu_count = 0;
				}
				else
					memu_count--;
				switch (memu_count)
				{
				case 0:
					H = 12;
					L = 12;
					break;
				case 1:
					H = 11;
					L = f_num;
					break;
				default:
					break;//默认的break
				}
#if DEBUG_FANHUI
				memu_dispaly_time_start = 1;
				memu_dispaly_time = 0;
#endif
			}
		}
	}
	//display(H,L);
}
