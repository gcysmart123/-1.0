#ifndef __FUNCTION_H_
#define __FUNCTION_H_

#define LED_R P17
#define LED_G P30

extern unsigned char H, L;
extern unsigned char	memu_count, f_num,
f_num_3_h, f_num_3_l,
f_num_4_h, f_num_4_l,
f_num_5_h, f_num_5_l;
				//f_num_3, f_num_4, f_num_5;
extern void function ();
extern void display (unsigned char LED_H,unsigned char LED_L);
extern void key();	
#endif