#include "stc15.h"
#include "ev1527.h"
#include "timer.h"
#include "FUNCTION.h"
#include "out.h"
#include "memu.h"

/********************解码方式专用变量*******************************/
unsigned char Save_RF_trans1 = 0;
unsigned char Save_RF_trans0 = 0;
unsigned char RF_trans0 = 0;
unsigned char RF_trans1 = 0;
unsigned char RF_BIT_COUNTER = 0;
unsigned char Timer0_interrupt_count = 0; //进入中断后400MS  无结果 恢复初始化操作
unsigned char P_RF_INT = 0x00;
unsigned char RF_RECE_REG[3] = { 0 };

unsigned int measure_sync_count1 = 70; //波长保存变量
unsigned int measure_sync_count2 = 120;//同上
unsigned int RF_trans1_test_count = 0; //码长测试变量
unsigned int RF_trans0_test_count = 0; //码长测试变量
unsigned int RF_trans_count = 0;	   //码长测试变量
unsigned int measure_sync_count1_saved = 70;
unsigned int measure_sync_count2_saved = 120;
unsigned int tunning_finish_count = 0;

unsigned char receive_rf_decoder_finished = 0;      //一次解码完成标志位
unsigned char again_receive_rf_decoder_finished = 0;//二次解码完成标志位
unsigned char rx_table = 0;							//二次解码switch步骤变量
unsigned char again_and_again_decoder_table = 0;     //二次编码成功标志位

unsigned char register_manager = 0;

void RF_decode_main(void);
void clear_again_receive_rf_decoder_finished(void);

unsigned char old1_RF_RECE_REG[3] = { 0 };//用于校验
unsigned char old2_RF_RECE_REG[3] = { 0 };//用于读取

//unsigned char old2_RF_RECE_REG[3][10] = { 0 };//用于存储

void exint0_init(void) //外部中断0 初始化
{
	EA = 1;
	EX0 = 1;
	IT0 = 1;
}

void exint0() interrupt 0 //外部中断0中断服务函数
{
	EX0 = 0;
		
	if (TR0 == 0)
	{
		//Init_Timer1();//sjz add for initialization timer0 before enable it
		TR0 = 1;
		//display(H,L);
	}
}

void RF_ini_receive(void) //初始化所有解码所用到的变量
{
	Save_RF_trans1 = 0; Save_RF_trans0 = 0;
	RF_trans0 = RF_trans1 = 0;
	RF_BIT_COUNTER = 0;
	P_RF_INT = 0x00;
	Timer0_interrupt_count = 0;//sjz
}

void RF_decode_main(void)
{
	if (P3_RF_RXD == 1)
	{
		++RF_trans1;
	}
	else
	{
		++RF_trans0;
	}
	Timer0_interrupt_count++;
	if (Timer0_interrupt_count>4000)/*4000*100us=400ms,*/
	{
		TR0 = 0;/*shut down timer0 interrupt sjz*/
		EX0 = 1;
		RF_ini_receive();
		return;

	}
	switch (P_RF_INT)
	{
	case 0:  //等待长低电平同步
		if (RF_trans1 > 0)
		{
			//包含的高脉冲太多
			RF_ini_receive();
			EX0 = 1;
			TR0 = 0;
			return;
		}
		if (RF_trans0 >70)//measure_sync_count1)
		{
			//长低同步完成
			P_RF_INT++;
			RF_trans0 = RF_trans1 = 0;
		}
		break;
	case 1: //等待真正的同步头
		if ((RF_trans0 + RF_trans1) >140)//measure_sync_count2)/*sjz change from 150 to 120,to shorten the detection when  the header of synchronization coming*/
		{
			//10mS没有同步高脉冲
			RF_ini_receive();
			EX0 = 1;
			TR0 = 0;
			return;
		}
		if (RF_trans1 > 1)
		{
			//高电平同步到了(最少检测到 2 次)
			if (EX0 == 1)
			{
				EX0 = 0;/*double check,since we have already detect the synchronization,if EXTINT0 is enable,need to disable  sjz*/
			}
			P_RF_INT++;
			RF_trans0 = 0;

			// RF_L_wait_H = 1;
		}
		break;
	case 2:
		if (RF_trans0 > 1)
		{
			Save_RF_trans1 = RF_trans1;
			RF_trans1 = 0;
			P_RF_INT++;
		}
		if (RF_trans1>25)//((measure_sync_count2 + measure_sync_count1) >> 3)) /*if the high level is bigger than 25*100us,Then should be setted as noise instead of useful signal sjz*/
		{
			RF_ini_receive();//KEY_HOLD = 0;
			EX0 = 1;
			TR0 = 0;
			return;
		}
		break;
	case 3:if (RF_trans1 > 1)
	{
			   Save_RF_trans0 = RF_trans0;
			   RF_trans0 = 0;
			   P_RF_INT = 2;
			   RF_RECE_REG[RF_BIT_COUNTER / 8] <<= 1;
			   if (Save_RF_trans1 >Save_RF_trans0)
			   {
				   RF_RECE_REG[(RF_BIT_COUNTER) / 8] |= 0x01;
			   }
			   ++RF_BIT_COUNTER;
			   if (RF_BIT_COUNTER >23)
			   {
				   //P_RF_INT = 4;
				   TR0 = 0;
				   RF_ini_receive();
				   receive_rf_decoder_finished = 1;
				   EX0 = 1;
				   break;
			   }
	}
		   if (RF_trans0> 25)//((measure_sync_count2 + measure_sync_count1) >> 3))
		   {
			   RF_ini_receive();
			   EX0 = 1;
			   TR0 = 0;
			   return;
		   }
		   break;

	default: //异常处理
	{
				 RF_ini_receive();
				 EX0 = 1;
				 TR0 = 0;
	}break;
	}
}

void receive_rf_decoder(void)
{
	if (receive_rf_decoder_finished == 1)
	{
		receive_rf_decoder_finished = 0;
		EX0 = 0;
		switch (rx_table)
		{
		case 0:	//第一次取得码
			//SendString("第一次取得码");
			old1_RF_RECE_REG[0] = RF_RECE_REG[0];
			old1_RF_RECE_REG[1] = RF_RECE_REG[1];
			old1_RF_RECE_REG[2] = RF_RECE_REG[2];
			rx_table = 1;
			break;
		case 1:
			if (old1_RF_RECE_REG[0] == RF_RECE_REG[0] && old1_RF_RECE_REG[1] == RF_RECE_REG[1] && old1_RF_RECE_REG[2] == RF_RECE_REG[2])
			{
				if (old2_RF_RECE_REG[0] == RF_RECE_REG[0] && old2_RF_RECE_REG[1] == RF_RECE_REG[1] && old2_RF_RECE_REG[2] == RF_RECE_REG[2])
				{
					if (again_and_again_decoder_table == 0) //如果该位不等于0  代表着上一次解码等于这次
					{
						old2_RF_RECE_REG[0] = RF_RECE_REG[0];
						old2_RF_RECE_REG[1] = RF_RECE_REG[1];
						old2_RF_RECE_REG[2] = RF_RECE_REG[2];
						again_receive_rf_decoder_finished = 1;
					}

				}
				else
				{
					old2_RF_RECE_REG[0] = RF_RECE_REG[0];
					old2_RF_RECE_REG[1] = RF_RECE_REG[1];
					old2_RF_RECE_REG[2] = RF_RECE_REG[2];
					again_receive_rf_decoder_finished = 1;
				}
				again_and_again_decoder_table = 1;
				clear_return_standby_time();

				RF_RECE_REG[0] = 0;
				RF_RECE_REG[1] = 0;
				RF_RECE_REG[2] = 0;
				old1_RF_RECE_REG[0] = 0;
				old1_RF_RECE_REG[1] = 0;
				old1_RF_RECE_REG[2] = 0;
				rx_table = 0;
			}
			else
			{
				old1_RF_RECE_REG[0] = RF_RECE_REG[0];
				old1_RF_RECE_REG[1] = RF_RECE_REG[1];
				old1_RF_RECE_REG[2] = RF_RECE_REG[2];
			}
			break;
		}
		EX0 = 1;
	}
}



unsigned char return_again_receive_rf_decoder_finished(void)
{
	unsigned char temp;
	temp = again_receive_rf_decoder_finished;
	return temp;
}

void clear_again_receive_rf_decoder_finished(void)
{
	again_receive_rf_decoder_finished = 0;
}

unsigned char return_again_and_again_decoder_table(void)
{
	unsigned char temp;
	temp = again_and_again_decoder_table;
	return temp;
}

void clear_again_and_again_decoder_table(void)
{
	again_and_again_decoder_table = 0;
}