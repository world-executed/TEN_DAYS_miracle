#include "headfile.h"

uint8 ringstate=0;

uint8 inring_st=0;
uint8 inring_nd=0;

int errorK = 100 * 30;
float dynamic_p = 0.001;
float dynamic_d = 0.000001;



int16 ADCL0=0;
int16 ADCR0=0;
int16 ADCLL0=0;
int16 ADCRR0=0;

float rela = 0.0;
float relation = 1.0;
int16 ADCL=0;
int16 ADCR=0;
int16 ADCLL=0;
int16 ADCRR=0;
int16 ADCLY = 0;
int16 ADCRY = 0;

int16 MPWM=0;
float DirError=0;
int ADCLK = 0;
int ADCRK = 0;
int turnFlag = 0;
int isTurnEnable = 0;

double pureError = 0.0;
int LockFlag = 0;        //1为锁死

DIRPID dirpid;

void Dir_control(float error)
{
	dirpid.err=-error;

  float tMPWM=dirpid.p*dirpid.err;
  tMPWM+=dirpid.d*(dirpid.err-dirpid.lasterr);
  
  dirpid.lasterr=dirpid.err;
  
  MPWM=(int)range(tMPWM,-85,85);
  ringProcess();
  pwm_duty(PWM4_MODULE2_CHA_C30, MPWM+SERVO_MID);
}


void RDir_control(float error, float relation)
{
	dirpid.err = -error;
	
	float tMPWM = dirpid.p * dirpid.err;
	tMPWM+=dirpid.d*(dirpid.err-dirpid.lasterr);
	
	dirpid.lasterr=dirpid.err;
	
	MPWM = (1.0 - relation) * (servotem[nowPos]) + relation * tMPWM;
	MPWM = (int)range(MPWM,-85,85);
	pwm_duty(PWM4_MODULE2_CHA_C30, MPWM+SERVO_MID);
}

void ringProcess()
{
  if(readyinring_st&&ringstate==0)
  {
    if(dip[0]==1)
      MPWM=-70;
    else
      MPWM=70;
    if(readyinring_st==50)
    {ringstate=1;
    
    inring_st=1;}
    gpio_set(D16,1);
  }
  else
    gpio_set(D16,0);
    
    
    if(inring_st==1&&ADC[2]>2000)
      inring_st=0;
  
  if(readyinring_nd&&ringstate==1)
  {
    if(dip[1]==1)
      MPWM=-75;
    else
      MPWM=75;
    if(readyinring_nd==100)
      
    {ringstate=2;}
    gpio_set(D16,1);
    inring_nd=1;
  }  
  else 
    gpio_set(D16,0);
  
  if(ringstate==2&&ADC[1]>1300)
    inring_nd=0;
}

//相关度计算----完全一致为0
//1）弯道：abs(应DirError - DirError)与应DirError之比
//2）直道：abs（DirError）与设定最大Error之比

void relatedCal()
{
	if(status[nowPos] != 1)	//弯道
	{
		rela = fabs((servotem[nowPos] + dirpid.p * DirError) / servotem[nowPos]);		//完全一致为0	
		relation = range(rela, 0.0, 1.0);
	}
	else if (status[nowPos] == 1)
	{
		rela = fabs(DirError / 40.0);
		relation = range(rela, 0.0, 1.0);
	}
}

void DynamicPID()
{
	dirpid.p = DirError * dynamic_p + static_p;
	dirpid.d = DirError * dynamic_d + static_d;
}

void GetError()
{
	ADCL0 = ADCL;
	ADCR0 = ADCR;
	ADCLL0= ADCLL;
	ADCRR0= ADCRR;

	ADCR =ADC[5];
	ADCRR=ADC[3];
	ADCLL=ADC[0];
	ADCL =ADC[4];
	ADCLY=ADC[1];
	ADCRY=ADC[2];

	if(ADCRR < 20)
		ADCRR = 0;
	if(ADCLL < 20)
		ADCLL = 0;
	if(ADCR < 20)
		ADCR = 0;
	if(ADCL < 20)
		ADCL = 0;
	if(StopFlag == 0)
		pureError = -(double)(sqrt(ADCL)-sqrt(ADCR))/(ADCL+ADCR);
	else
		pureError = 0;

	DirError=errorK * pureError;
}