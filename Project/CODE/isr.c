#include "headfile.h"

//用recordNum代替allNum
float speedK = 0.5;

uint8 pit=0;
int CountNum = 0;
uint8 StopFlag = 1;

int roadMode;

int recordNum = 0;

int gyro_x_i = 0;
int gyro_y_i = 0;
int gyro_z_i = 0;
int16 angle = 0;

float OSC[8]={0.0};

float Yaw,Pitch,Roll;

void CSI_IRQHandler(void)
{
	CSI_DriverIRQHandler();     //����SDK�Դ����жϺ��� ���������������������õĻص�����
	__DSB();                    //����ͬ������
}

void PIT_1MS()
{
	ADCSample(pit);
}

void PIT_5MS()
{
	SpeedControl();
	
	sp = static_p;
	lp = static_p * speedK;
	
	roadMode = 1;
	
	if(recordMode == 3)
	{
		PosCalculate();
		eleMatch();
		
		if(status_tem[nowPos] != 1) //���
		{
			dirpid.p = sp;
			gpio_set(D16,1);
		}
		else
		{
			gpio_set(D16,0);    //ֱ��
			dirpid.p = lp;
		}
	}
	
	dip[0]=gpio_get(C25); //1you
	dip[1]=gpio_get(C26); //0zuo
	dip[2]=gpio_get(C27);
	dip[3]=gpio_get(C28);
	
	//Imu_Update();
	//ImuCalculate_Complementary();
	//Prepare_Data();
	MPU6050();
	GetError();
	//DynamicPID();
	ADCTest();
	//modeSelect();
	
	if (LockFlag == 0 && StopFlag == 0 && chukuFlag == 2 && rukuFlag==0) {        //û������������ʻ
		if(recordMode != 3)
		{
			Dir_control(DirError);
			dirpid.p = static_p;
		}
		else if(recordMode == 3)
		{
			//relatedCal();
			//relation = 0.5;
			RDir_control(DirError, relation);
		}
		CountNum = 0;
	}
	else
	{
		if(turnFlag == 1)
		{
			pwm_duty(PWM4_MODULE2_CHA_C30, 90+700);      //��ת
		}
		if(turnFlag == -1)
		{
			pwm_duty(PWM4_MODULE2_CHA_C30, -90+700);      //��ת
		}
		
		CountNum++;
		if (CountNum > 201)
			CountNum = 1;
	}
	
	if(roadMode == 9)
	{
		//MPWM = 0;
		//pwm_duty(PWM4_MODULE2_CHA_C30, MPWM+700);
		//DirError = 0;
	}
	
	if (CountNum > 200)
		StopFlag = 1;
	
	if(ringstate==0)
		ringjudge_st();
	if(ringstate==1);
	//ringjudge_nd();
	
	if(recording)
	{
		posL[recordNum]		 = leftSpeedInt / 10;
		posR[recordNum]		 = rightSpeedInt / 10;
		servo[recordNum]	 = MPWM;
		angle_int[recordNum] = gyro_x_i / 100;
		recordNum += 1;
		
		/*
		if(chukuFlag == 2)
		{
			AllZero();
			chukuFlag = -1;
		}
		*/
		
		if(rukuFlag == 3)
		{
			recording = 0;
			page = 4;
			oled_fill(0x00);
		}
	}
	
	/*
	OSC[0]=ADCL;
	OSC[1]=ADCR;
	OSC[2]=gyro_x_i / 100;
	OSC[3]=MPWM * 10;
	OSC[4]=dirpid.p * 10;
	OSC[5]=0;
	OSC[6]=leftSpeedInt/10;
	OSC[7]=rightSpeedInt/10;
	vcan_sendware(OSC,sizeof(OSC));
	*/
	ring_int();
	
	hillProcess();
	chuku();
	ruku();
	setSpeed();
	
	OLED_switch();
}


void PIT_IRQHandler(void)
{
	
	if(PIT_FLAG_GET(PIT_CH0))
	{
		
		PIT_1MS();
		pit++;
		if(pit==5)
		{
			PIT_5MS();
			pit=0;
		}
		
		PIT_FLAG_CLEAR(PIT_CH0);
		
	}
	__DSB();
	
}

void GPIO2_Combined_16_31_IRQHandler(void)
{
	if(GET_GPIO_FLAG(C16))
	{
		CLEAR_GPIO_FLAG(C16);//����жϱ�־λ
	}
	
	
}



void GPIO2_Combined_0_15_IRQHandler(void)
{
	if(GET_GPIO_FLAG(MT9V03X_VSYNC_PIN))
	{
		//���������־λ����־λ��mt9v03x_vsync�����ڲ������
		if(1 == flexio_camera_type)mt9v03x_vsync();
	}
	if(GET_GPIO_FLAG(SCC8660_VSYNC_PIN))
	{
		//���������־λ����־λ��scc8660_vsync�����ڲ������
		if(2 == flexio_camera_type)scc8660_vsync();
	}
}

