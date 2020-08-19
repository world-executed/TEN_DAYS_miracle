#include "headfile.h"

int16 SetLeftSpeed  = 0;
int16 SetRightSpeed = 0;
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
	RightSpeed = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2 );          //��ȡFTM �������� ��������(������ʾ������)
	LeftSpeed = -qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0 );
	qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );
	qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2 );
	
		if (MPWM < -35)	//��ת
	{
		setLeftSpeed_L = (2 * SetLeftSpeed) / (1.97530982 - 0.00425803 * abs(MPWM));
		setRightSpeed_L = (2 * SetLeftSpeed) / (1.97530982 - 0.00425803 * abs(MPWM)) * (0.97530982 - 0.00425803 * abs(MPWM));
	} else if(MPWM > 35){	//��ת
		setRightSpeed_L = (2 * SetLeftSpeed) / (1.97530982 - 0.00425803 * abs(MPWM));
		setLeftSpeed_L = (2 * SetLeftSpeed) / (1.97530982 - 0.00425803 * abs(MPWM)) * (0.97530982 - 0.00425803 * abs(MPWM));
	}
    else
    {
      setRightSpeed_L=SetRightSpeed;
      setLeftSpeed_L=SetLeftSpeed;
    }

    if(abs(angle_ring)>300&&abs(angle_ring)<430)
    {
      setRightSpeed_L=setRightSpeed_L*0.8;
      setLeftSpeed_L=setLeftSpeed_L*0.8;
    }
      
	PWML+=PID_control(&pid_l,setLeftSpeed_L,LeftSpeed);
	PWMR+=PID_control(&pid_r,setRightSpeed_L,RightSpeed);
	
	leftSpeedInt += LeftSpeed;
	rightSpeedInt += RightSpeed;
	
	PWML=range(PWML,-5000,5000);
	PWMR=range(PWMR,-5000,5000);
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
  PWML=range(PWML,-5000,5000);
  PWMR=range(PWMR,-5000,5000);
  
    
  if(chukuFlag==0||rukuFlag==3)//�ڿ���
    PWML=PWMR=0;
  else if(chukuFlag==1)
    PWML=PWMR=3000;// �����ٶ�
  else if(rukuFlag==1||rukuFlag==2)
    PWML=PWMR=2000;//����ٶ�
  else//������ʻ
  {
    if(StopFlag==1)
    {PWML=PWMR=0;/*StopCar();*/}
  }
  
  
  if(PWML>0)
  {
    pwm_duty(PWM1_MODULE0_CHB_D13,0);//��
    pwm_duty(PWM1_MODULE0_CHA_D12,PWML);//����
  }
  else
  {
    pwm_duty(PWM1_MODULE0_CHB_D13,-PWML);//��
    pwm_duty(PWM1_MODULE0_CHA_D12,0);//����
  }
  
  if(PWMR>0)
  {
    pwm_duty(PWM1_MODULE1_CHB_D15,PWMR);//����
    pwm_duty(PWM1_MODULE1_CHA_D14,0);//�ҷ�
  }
  else
  {
    pwm_duty(PWM1_MODULE1_CHB_D15,0);//����
    pwm_duty(PWM1_MODULE1_CHA_D14,-PWMR);//�ҷ�
  }

}
