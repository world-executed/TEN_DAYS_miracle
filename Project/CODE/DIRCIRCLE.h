#ifndef _DIRCIRCLE_H_
#define _DIRCIRCLE_H_

#define ADCLOWBAND 20   //ADC下限，低于此值认为冲出赛道且error为0
#define ERRORBAND 0  //偏差下限，低于此值认为方向无偏差
#define SERVO_MID 760

extern int16 ADCLL;
extern int16 ADCRR;
extern int16 ADCL;
extern int16 ADCR;
extern int16 ADCLY;
extern int16 ADCRY;



extern int16 MPWM;
extern float DirError;
extern int errorK;
extern double pureError;
extern int LockFlag;
extern int turnFlag;
extern int stopCarNum;
extern int stopCarFlag;
extern float relation;

extern int16 ADCL0;
extern int16 ADCR0;
extern int16 ADCLL0;
extern int16 ADCRR0;

extern float sp;
extern float lp;
extern int isTurnEnable;
extern float relation;

typedef struct {

    float err;
    float lasterr;
    float preverr;
	float sumerr;
    float p,i,d;
}DIRPID;
extern DIRPID dirpid;


void DirControl();
void Dir_control(float error);
void RDir_control(float error, float relation);

void GetError();

#endif