#ifndef __EV1527_H_
#define __EV1527_H_

#include "STC15F2K60S2.H"

sbit P3_RF_RXD = P3 ^ 2; //–≈∫≈ ‰»ÎΩ≈

#define TUNNING_STEP 15   /*step for tunning the receiving code length*/

extern void exint0_init(void);
extern void RF_ini_receive(void);
extern void RF_decode_main(void);
extern void receive_rf_decoder(void);
extern void RF_decode_main_sjz_test(void);

extern unsigned char old2_RF_RECE_REG[3];

extern unsigned char return_again_receive_rf_decoder_finished(void);
extern void clear_again_receive_rf_decoder_finished(void);

extern unsigned char return_again_and_again_decoder_table(void);
extern void clear_again_and_again_decoder_table(void);


extern void SendIRdata(unsigned char id_data_3,unsigned char id_data_2,unsigned char id_data_1);
#endif