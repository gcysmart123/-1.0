#include "STC15.h"
#include "FUNCTION.h"
#include "timer.h"

void display (unsigned char LED_H,unsigned char LED_L);
void key();	   

unsigned char H = 12, L = 12;
//unsigned char	memu_count, f_num,
//f_num_3_h, f_num_3_l,
//f_num_4_h, f_num_4_l,
//f_num_5_h, f_num_5_l;

			//	f_num_3, f_num_4, f_num_5;
unsigned char	memu_count = 0, f_num, 
				f_num_3_h = 0, f_num_3_l = 2, 
				f_num_4_h = 0, f_num_4_l = 1, 
				f_num_5_h = 0, f_num_5_l = 5;
void function ()
{ 
	key();	
	display(H,L);
	
	times = f_num_3_h*10+f_num_3_l;		
	waite_time_s =(f_num_4_h*10+f_num_4_l)*20;
	repeat_time =(f_num_5_h*10+f_num_5_l)*20;
}					     