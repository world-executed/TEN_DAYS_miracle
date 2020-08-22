#ifndef _SPEEDCIRCLE_H_
#define _SPEEDCIRCLE_H_

#define max(a, b)			(a>b? a:b)
#define min(a, b)			(a<b? a:b)
#define range(x, a, b)		(min(max(x, a), b))
extern int16 SetLeftSpeed;
extern int16 SetRightSpeed;
extern int16 PWML;
extern int16 PWMR;
extern int16 LeftSpeed;
extern int16 RightSpeed;
extern int setLeftSpeed_L;
extern int setRightSpeed_L;
extern int leftSpeedInt;
extern int rightSpeedInt;
extern int FTMint_fin;
extern uint8 FTMfin_mark;
extern int prev;
typedef struct PID{

    int16 err;
    int16 lasterr;
    int16 preverr;
    float p,i,d;
    
}PID;
extern PID pid_l,pid_r;
int16 PID_control(PID *pid,int16 goalspeed,int16 actualspeed);
void SpeedControl();
void setSpeed();
extern int ringNum;

#endif