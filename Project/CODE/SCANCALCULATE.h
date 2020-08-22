#ifndef _SCANCALCULATE_H_
#define _SCANCALCULATE_H_

#include "headfile.h"
#define calNum 10000

#define eleLen 40

extern int status[calNum] ;
extern int angle_int[calNum];
extern int posL[calNum] ;
extern int posR[calNum] ;
extern int servoShould[calNum];
extern int servo_tem[calNum];
extern int servo[calNum];
extern int status_tem[calNum];
extern int speedStatus[calNum];
extern int speedStatusInt;

extern int nowPos;
extern int recordMode;
extern int finishFlag;
extern int eleArray[eleLen];
extern int nowEleNum;
extern int recording;
extern int ring8num;

int ScanCalculate();
void PosCalculate();
void eleMatch();
void waveScan_status();
void waveScan_Camara(int line);

extern int kTestThrH;	
extern int kTestThrL;	
extern int period;
extern int nowPosL;
extern int nowPosR;
#endif