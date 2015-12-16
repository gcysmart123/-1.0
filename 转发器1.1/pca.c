#include "STC15.h"
#include "timer.h"
#include "ev1527.h"
#include "pca.h"

unsigned char flag;
unsigned int count,endcount;
bit OP;

#define FOSC    12000000L
#define T100Hz  (FOSC / 12 / 1923)

#define T20Hz  (FOSC / 12 / 20)

#define CCP_S0 0x10                 //P_SW1.4
#define CCP_S1 0x20                 //P_SW1.5


WORD value;
WORD value_send_delay;

unsigned char second_filter_time = 0;
unsigned char second_filter_table = 0;

   
//CPA 终端服务程序
void PCA_isr() interrupt 7 using 1
{	
		switch (flag_Pattern)
		{
		case 0:
			CCF0 = 0;                       //清中断标志
			CCAP0L = value_send_delay;
			CCAP0H = value_send_delay >> 8;            //更新比较值
			value_send_delay += T20Hz;

			if (return_again_and_again_decoder_table() == 1)
			{
				again_and_again_time++;
				if (again_and_again_time >repeat_time)
				{
					clear_again_and_again_decoder_table();
					again_and_again_time = 0;
				}
			}
			if (second_filter_table == 1)
			{
				second_filter_time++;
				if (second_filter_time >repeat_time)
				{
					second_filter_table = 0;
					second_filter_time = 0;
				}
			}
			break;
		case 1:
			CCF0 = 0;                       //清中断标志
			CCAP0L = value;
			CCAP0H = value >> 8;            //更新比较值 520us
			value += T100Hz;
			count++;
			if (flag == 1)

			{

				OP = 1;

			}

			else

			{

				OP = 0;

			}

			P35 = OP;
			break;
		case 2:
			CCF0 = 0;                       //清中断标志
			CCAP0L = value_send_delay;				   //更新比较值 50ms
			CCAP0H = value_send_delay >> 8;            //更新比较值 50ms
			value_send_delay += T20Hz;

			delay_time++;

			if (delay_time == send_time)
			{
				P35 = 0;
				delay_time = 0;

				received_end = 1;
				CCF0 = 0;                       //清中断标志
				CCAP0L = value;
				CCAP0H = value >> 8;            //更新比较值 520us
				value += T100Hz;

			}
			else if (return_again_and_again_decoder_table() == 1)
			{
				again_and_again_time++;
				if (again_and_again_time > repeat_time)
				{
					clear_again_and_again_decoder_table();
					again_and_again_time = 0;
				}
			}
			else if (second_filter_table == 1)
			{
				second_filter_time++;
				if (second_filter_time >repeat_time)
				{
					second_filter_table = 0;
					second_filter_time = 0;
				}
			}


			break;
		case 3:
			CCF0 = 0;                       //清中断标志
			CCAP0L = value_send_delay;
			CCAP0H = value_send_delay >> 8;            //更新比较值
			value_send_delay += T20Hz;

			if (return_again_and_again_decoder_table() == 1)
			{
				again_and_again_time++;
				if (again_and_again_time >repeat_time)
				{
					clear_again_and_again_decoder_table();
					again_and_again_time = 0;
				}
			}
			if (second_filter_table == 1)
			{
				second_filter_time++;
				if (second_filter_time >120)
				{
					second_filter_table = 0;
					second_filter_time = 0;
				}
			}
			break;

		default:
			break;
		}
		
	/*
	if(CCF1==1)
	{
		
		if(flag_Pattern==3)
		{
		   CCF1 = 0;
			CCAP1L = value_waite;
			CCAP1H = value_waite >> 8;            
			value += T20Hz;
			waite_time++;

			if (waite_time == waite_time_s)
			{
				waite_time = 0;
				waite_end = 1;
				P54 = 1;
		
			}
		}
		else 
		{
			CCF1 = 0;
			CCAP1L = value_waite;
			CCAP1H = value_waite >> 8;            
			value += T20Hz;

				
		}
	} */
}
//CPA 初始化
void pca_timer_init()
{
	count = 0;

  	flag = 0;

  	OP = 0;

 	P35 = 0;

  	EA = 1;

	ACC = P_SW1;
    ACC &= ~(CCP_S0 | CCP_S1);      //CCP_S0=0 CCP_S1=0
    P_SW1 = ACC;                    //(P1.2/ECI, P1.1/CCP0, P1.0/CCP1, P3.7/CCP2)
    
    CCON = 0;                       //初始化PCA控制寄存器
                                    //PCA定时器停止
                                    //清除CF标志
                                    //清除模块中断标志
    CL = 0;                         //复位PCA寄存器
    CH = 0;
    CMOD = 0x00;                    //设置PCA时钟源
                                    //禁止PCA定时器溢出中断
	value_send_delay = T20Hz;

	value = T100Hz;
    CCAP0L = value;
    CCAP0H = value >> 8;            //初始化PCA模块0
    value += T100Hz;
    CCAPM0 = 0x49;                  //PCA模块0为16位定时器模式

	CR = 1;
}

void SendIRdata(unsigned char id_data_3,unsigned char id_data_2,unsigned char id_data_1)

{

  int i;

  char id_data;

  //发送同步头

  endcount=1;

  flag=1;
  count=0;

  do{}while(count<endcount);


  endcount=31;

  flag=0;

  count=0;

  do{}while(count<endcount);

  //发送bit23-bit16

  id_data=id_data_3;

  for(i=0;i<8;i++)

  {
  	  if(id_data&0x80)  //判断二进制数高位为1还是0

     {
		endcount=3;  //1为宽的高电平
		flag=1;
     }

    else

     {

     endcount=1;   //0为窄的高电平
	 	flag=1;
     }

    

    count=0;

    do{}while(count<endcount);
	
  endcount=4-endcount;

  flag=0;

  count=0;

  do{}while(count<endcount);

    id_data=id_data<<1;

  }

    //发送bit15-bit8

  id_data=id_data_2;

  for(i=0;i<8;i++)

  {
     if(id_data&0x80)

     {

        endcount=3;
		flag=1;
     }

     else

     {

       endcount=1;
	   flag=1;
     }

     flag=1;

     count=0;

     do{}while(count<endcount);

	  endcount=4-endcount;

  flag=0;

  count=0;

  do{}while(count<endcount);

     id_data=id_data<<1;

  }

  //发送bit8-bit0

  id_data=id_data_1;

  for(i=0;i<8;i++)

  {
     if(id_data&0x80)

     {

         endcount=3;
		 flag=1;
     }

     else

     {

       endcount=1;
	  flag=1;
     }

     flag=1;

     count=0;

     do{}while(count<endcount);

	  endcount=4-endcount;

  flag=0;

  count=0;

  do{}while(count<endcount);
	 
     id_data=id_data<<1;

  }
}

