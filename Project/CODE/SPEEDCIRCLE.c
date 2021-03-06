#include "headfile.h"

int16 SetLeftSpeed  = 60;
int16 SetRightSpeed = 60;
int setLeftSpeed_L = 0;
int setRightSpeed_L = 0;
int leftSpeedInt = 0;
int rightSpeedInt = 0;
int16 PWML=0;
int16 PWMR=0;
uint8 FTMfin_mark=0;
int16 LeftSpeed=0;
int16 RightSpeed=0;
int FTMint_fin=163000;
PID pid_l,pid_r;
int ringBeginNum = 0;
int ringNum = 0;
int prev = 1000;

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
	
    if(abs(angle_ring)>(ring_over*0.7)&&abs(angle_ring)<ring_over)//�����ٶ�
    {
      setRightSpeed_L=setRightSpeed_L*0.7;
      setLeftSpeed_L=setLeftSpeed_L*0.7;
    }
	
	if(hillFlag==3)
	{
	  setRightSpeed_L=setRightSpeed_L*0.8;
	  setLeftSpeed_L=setLeftSpeed_L*0.8;
	}

	 if(recordMode == 3)
	{
		if(speedStatus[nowPos] == 0) //进弯出弯状态
		{
		  setRightSpeed_L=setRightSpeed_L*0.85;
		  setLeftSpeed_L=setLeftSpeed_L*0.85;
		}
		
		else if(speedStatus[nowPos] == 1)	//直道
		{
		  setRightSpeed_L=setRightSpeed_L*1.0;
		  setLeftSpeed_L=setLeftSpeed_L*1.0;
		}
		
		else if(speedStatus[nowPos] == 8)	//弯道
		{
		  setRightSpeed_L=setRightSpeed_L*0.85;
		  setLeftSpeed_L=setLeftSpeed_L*0.85;
		}
		
		if(status[nowPos] == 8)
		{
			ringNum += 1;
			ringBeginNum = nowPos;
		}
		
		if(status[nowPos] == 8 && ringNum >= 0.75 * ring8num)
		{
			setRightSpeed_L=setRightSpeed_L*0.5;
		  	setLeftSpeed_L=setLeftSpeed_L*0.5;
		}
	}
	
	
	if((leftSpeedInt+rightSpeedInt)/2>FTMint_fin-prev&&FTMfin_mark)
		{
			setRightSpeed_L=setRightSpeed_L>45?45:setRightSpeed_L;
			setLeftSpeed_L=setLeftSpeed_L>45?45:setLeftSpeed_L;
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
    PWML=PWMR=6000;// �����ٶ�
  else if(rukuFlag==1||rukuFlag==2)
	PWML=PWMR=3500;// ����ٶ�
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
