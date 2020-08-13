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
extern float servotem[calNum];
extern int servo[calNum];

extern int nowPos;
extern int recordMode;
extern int finishFlag;
extern int eleArray[eleLen];
extern int nowEleNum;
extern int recording;
#endif