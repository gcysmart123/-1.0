#ifndef __PCA_H_
#define __PCA_H_

#define FOSC    12000000L
#define T100Hz  (FOSC / 12 / 1923)

#define T20Hz  (FOSC / 12 / 20)

typedef unsigned char BYTE;
typedef unsigned int WORD;

extern WORD value;
extern WORD value_send_delay;
extern WORD value_waite;

extern void pca_timer_init();
extern void SendIRdata(unsigned char id_data_3, unsigned char id_data_2, unsigned char id_data_1);

#endif