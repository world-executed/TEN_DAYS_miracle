#ifndef _ALLINIT_H_
#define _ALLINIT_H_

#define GYRO360 690000;
extern uint8 dip[4];	
extern int32 gyro_offset[3];
extern int32 acc_offset[3];
extern int16 acc[3];
extern int16 gyro[3];
extern float static_p;
extern float static_d;

void PID_init();
void AllInit();

#endif