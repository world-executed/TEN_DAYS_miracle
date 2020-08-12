#ifndef _SCANCALCULATE_H_
#define _SCANCALCULATE_H_

#include "headfile.h"

#define eleLen 40

extern int status[4600] ;
extern int angle_int[4600];
extern int posL[4600] ;
extern int posR[4600] ;
extern int servoShould[4600];
extern float servotem[4600];

extern int nowPos;
extern int recordMode;
extern int finishFlag;
extern int eleArray[eleLen];
extern int nowEleNum;
#endif