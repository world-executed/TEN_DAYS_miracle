#include "headfile.h"



int errorK = 100 * 30;
float dynamic_p = 0.001;
float dynamic_d = 0.000001;



int16 ADCL0=0;
int16 ADCR0=0;
int16 ADCLL0=0;
int16 ADCRR0=0;

float rela = 0.0;
float relation = 0.5;
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
int ring_servo_bias = 20;

double pureError = 0.0;
int LockFlag = 0;        //1Ϊ����

DIRPID dirpid;

void RDir_control(float error, float relation)
{
	dirpid.err = -error;
	
	float tMPWM = dirpid.p * dirpid.err;
	tMPWM+=dirpid.d*(dirpid.err-dirpid.lasterr);
	
	dirpid.lasterr=dirpid.err;
	
	MPWM = (1.0 - relation) * (servo_tem[nowPos] / 10) + relation * tMPWM;	
	
	if(readyinring_st)
	{
		ring_servo=SetLeftSpeed+ring_servo_bias;
		MPWM=-ring_servo*FANGXIANG+range((ADC[4]-ADC[5])/200,-15,15);
	}
	MPWM=(int)range(MPWM,-85,85);
	pwm_duty(PWM4_MODULE2_CHA_C30, MPWM+SERVO_MID);
}


//��ضȼ���----��ȫһ��Ϊ0
//1�������abs(ӦDirError - DirError)��ӦDirError֮��
//2��ֱ����abs��DirError�����趨���Error֮��
/*
void relatedCal()
{
	if(status[nowPos] != 1)	//���
	{
		rela = fabs((servotem[nowPos] + dirpid.p * DirError) / servotem[nowPos]);		//��ȫһ��Ϊ0	
		relation = range(rela, 0.0, 1.0);
	}
	else if (status[nowPos] == 1)
	{
		rela = fabs(DirError / 40.0);
		relation = range(rela, 0.0, 1.0);
	}
}
*/
void DynamicPID()
{
	dirpid.p = DirError * dynamic_p + static_p;
	dirpid.d = DirError * dynamic_d + static_d;
}

void GetError()
{
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
      //pureError=-(sqrt(ADC[4])-sqrt(ADC[5]))/(ADC[4]+ADC[5]+ADC[0]);
	else
		pureError = 0;

	DirError=errorK * pureError;
    //DirError=5000*pureError;
}