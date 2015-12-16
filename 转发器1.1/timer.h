#ifndef __TIMER_H_
#define __TIMER_H_

#include "STC15F2K60S2.H"
#define TIMER50MS 50000
#define FOSC 12000000L          //ÏµÍ³ÆµÂÊ

#define  LED_G P30
#define  LED_R P17

extern void Init_Timer0(void);



extern unsigned char return_await_number_table(void);
extern void set_await_number_table(unsigned char temp);

extern void set_main_press_time_table(unsigned char temp);
extern unsigned char return_main_press_time(void);
extern void clear_main_press_time(void);\
extern void set_main_press_time(unsigned char temp);

extern void set_logout_cycle_table(unsigned char temp);
extern unsigned char return_logout_cycle_table(void);

extern void clear_return_standby_time(void);

extern unsigned char return_fd_table(void);
extern void set_fd_table(unsigned char temp);

extern unsigned char return_filter_main(void);
extern void set_filter_main(unsigned char temp);

extern unsigned char send_start;
extern unsigned char delay_time;
extern unsigned char send_time;
extern unsigned char flag_Pattern;

extern unsigned char times;
extern unsigned char send_times;
//extern unsigned char send_end;
extern unsigned char received_end;
extern unsigned int waite_time_s;
extern unsigned char waite_end;
//extern unsigned int delay_time_200ms;

extern unsigned char again_and_again_time;
extern unsigned char repeat_time;




#endif