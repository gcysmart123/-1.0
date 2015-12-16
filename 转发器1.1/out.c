#include "timer.h"
#include "ev1527.h"
#include "stc15.h"
#include "pca.h"
#include "out.h"

unsigned char old2_RF_RECE_REG_SAVE[3][50] = { 0 };//用于存储

void out()
{
	send_start = 1;
//	CR = 1;							  //PCA定时器开始工作
	EX0 = 0;
	TR0 = 1;		
	ET0 = 1;

		CCAP0L = value;
		CCAP0H = value >> 8;            //更新比较值 520us
		value += T100Hz;
	do
	{	
	//SendIRdata(old2_RF_RECE_REG[0], old2_RF_RECE_REG[1], old2_RF_RECE_REG[2]);
		P35 = 0;
		SendIRdata(old2_RF_RECE_REG_SAVE[0][0], old2_RF_RECE_REG_SAVE[1][0], old2_RF_RECE_REG_SAVE[2][0]);
	} while (send_start);
	
	//LED_R = 0;

	EX0 = 1;
	TR0 = 0;
	ET0 = 1;

	send_time = (20 * send_times) + 25;
	send_times++;
}		