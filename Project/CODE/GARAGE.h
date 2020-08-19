#ifndef _CHEKU_H_
#define _CHEKU_H_

extern uint8 hillFlag;
extern int FANGXIANG;//1为顺时针，-1为逆时针
extern uint8 chukuFlag;
extern uint8 rukuFlag;
extern uint8 th;
extern uint8 jumpnum;
extern uint8 Zebra_line;
extern uint8 black_blocks;
extern uint8 times;
extern uint8 startline;
extern float angle_x;
extern float angle_y;
void MPU6050();
void hillProcess();
void ruku();
void chuku();
void StopCar();
void check_zebra_line();

#endif