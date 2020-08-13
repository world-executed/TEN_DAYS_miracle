#include "headfile.h"

int16 SetLeftSpeed  = 60;
int16 SetRightSpeed = 60;
int setLeftSpeed_L = 0;
int setRightSpeed_L = 0;
int leftSpeedInt = 0;
int rightSpeedInt = 0;
int16 PWML=0;
int16 PWMR=0;

int16 LeftSpeed=0;
int16 RightSpeed=0;

PID pid_l,pid_r;

int16 PID_control(PID *pid,int16 goalspeed,int16 actualspeed)
{
  
  pid->err=goalspeed-actualspeed;
  int16 dpwm=pid->p*(pid->err-pid->lasterr);
  dpwm+=pid->i*pid->err;
  dpwm+=pid->d*(pid->err-2*pid->lasterr+pid->preverr);
  pid->preverr=pid->lasterr;
  pid->lasterr=pid->err;
  return dpwm;
}

void SpeedControl()
{
	RightSpeed = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2 );          //获取FTM 正交解码 的脉冲数(负数表示反方向)
	LeftSpeed = -qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0 );
	qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );
	qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2 );
	
		if (MPWM < -35)	//右转
	{
		setLeftSpeed_L = (2 * SetLeftSpeed) / (1.97530982 - 0.00425803 * abs(MPWM));
		setRightSpeed_L = (2 * SetLeftSpeed) / (1.97530982 - 0.00425803 * abs(MPWM)) * (0.97530982 - 0.00425803 * abs(MPWM));
	} else if(MPWM > 35){	//左转
		setRightSpeed_L = (2 * SetLeftSpeed) / (1.97530982 - 0.00425803 * abs(MPWM));
		setLeftSpeed_L = (2 * SetLeftSpeed) / (1.97530982 - 0.00425803 * abs(MPWM)) * (0.97530982 - 0.00425803 * abs(MPWM));
	}
	
	PWML+=PID_control(&pid_l,setLeftSpeed_L,LeftSpeed);
	PWMR+=PID_control(&pid_r,setRightSpeed_L,RightSpeed);
	
	leftSpeedInt += LeftSpeed;
	rightSpeedInt += RightSpeed;
	
	PWML=range(PWML,0,5000);
	PWMR=range(PWMR,0,5000);
}

void setSpeed() 
{
/***********************
StopFlag==1       pwm=0
StopFlag==0       pwm=PWM
chukuFlag==0      pwm=0
chukuFlag==1      pwm=3000
chukuFlag==2      pwm=PWM
rukuFlag==0       pwm=PWM
rukuFlag==1       pwm=3000
rukuFlag==2       pwm=3000
rukuFlag==3       pwm=0
***********************/
  PWML=range(PWML,0,5000);
  PWMR=range(PWMR,0,5000);
  
    
  if(chukuFlag==0||rukuFlag==3)//在库中
    PWML=PWMR=0;
  else if(chukuFlag==1)
    PWML=PWMR=5000;// 出库速度
  else if(rukuFlag==1||rukuFlag==2)
    PWML=PWMR=1500;//入库速度
  else//正常行驶
  {
    if(StopFlag==1)
    {PWML=PWMR=0;/*StopCar();*/}
  }
  
  

  pwm_duty(PWM1_MODULE0_CHB_D13,0);//左反
  pwm_duty(PWM1_MODULE0_CHA_D12,PWML);//左正
  pwm_duty(PWM1_MODULE1_CHB_D15,PWMR);//右正
  pwm_duty(PWM1_MODULE1_CHA_D14,0);//右反


}
