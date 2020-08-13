#ifndef _CHEKU_H_
#define _CHEKU_H_

extern uint8 hillFlag;
extern int8 FANGXIANG;//1为顺时针，-1为逆时针
extern uint8 chukuFlag;
extern uint8 rukuFlag;
extern uint8 th;
extern uint8 jumpnum;
void hillProcess();
void ruku();
void chuku();
void StopCar();

#endif