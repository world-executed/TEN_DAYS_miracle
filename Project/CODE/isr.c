#include "headfile.h"

uint8 pit=0;
int CountNum = 0;
uint8 StopFlag = 1;

int roadMode;

int gyro_x_i = 0;
int gyro_y_i = 0;
int gyro_z_i = 0;

int recordNum = 0;
int ringContinue = 0;
float OSC[8]={0.0};
int Count=0;
float Yaw,Pitch,Roll;

void CSI_IRQHandler(void)
{
	CSI_DriverIRQHandler();     //����SDK�Դ����жϺ��� ���������������������õĻص�����
	__DSB();                    //����ͬ������
}
/*
void PIT_1MS()
{
	Count++;
	//ADCSample(pit);
}*/

void PIT_5MS()
{
	  ADC[0]=adc_mean_filter(ADC_1,ADC1_CH6_B17,1);//qianzuo
	  ADC[1]=adc_mean_filter(ADC_1,ADC1_CH7_B18,1);//qianzuoxie
	  ADC[2]=adc_mean_filter(ADC_1,ADC1_CH8_B19,1);//qianyouxie
	  ADC[3]=adc_mean_filter(ADC_1,ADC1_CH5_B16,1);//qianyou
	  ADC[4]=adc_mean_filter(ADC_1,ADC1_CH3_B14,1);//houzuo
	  ADC[5]=adc_mean_filter(ADC_1,ADC1_CH4_B15,1);//houyou
	
	
	SpeedControl();
	
	sp = static_p;
	lp = static_p * speedK;
	
	if(inring_st || rukuFlag == 2)
		gpio_set(C17,1);
	else
		gpio_set(C17,0);
	
	roadMode = 1;
	//recordMode = 2;
	
	if(recordMode == 3 && rukuFlag != 3)
	{
		PosCalculate();
		/*
		if(status[nowPos] != 1) //���
		{
		dirpid.p = sp;
		if(dip[1]) gpio_set(D16,1);
	}
		else
		{
		if(dip[0]) dirpid.p = lp; else dirpid.p = sp;
		if(dip[1]) gpio_set(D16,0);    //ֱ��
		
	}*/
		
		if(speedStatus[nowPos] == 0) //进弯出弯状态
		{
			gpio_set(D16,1);
			dirpid.p = 1.2 * sp;
			//relation = 1.0;
		}
		
		else if(speedStatus[nowPos] == 1)	//直道
		{
			if(dip[1]) dirpid.p = lp; else  dirpid.p = sp;
			gpio_set(D16,0);    //ֱ��
			//relation = 1.0;
			dirpid.p = 0.8 * sp;
		}
		
		else if(speedStatus[nowPos] == 8)	//弯道
		{
			dirpid.p = sp;
			gpio_set(D16,0);
			//relation = 1.0;
			dirpid.p = 1.5 * sp;
		}
		
	}
	
	if(inring_st)
		dirpid.p=2*sp;
	dip[0]=gpio_get(C28); //1you
	dip[1]=gpio_get(C27); //0zuo
	dip[2]=gpio_get(C26);
	dip[3]=gpio_get(C25);
	
	//Imu_Update();
	//ImuCalculate_Complementary();
	//Prepare_Data();
	MPU6050();
	RingProcess();
	GetError();
	//DynamicPID();
	ADCTest();
	//modeSelect();
	
	if (LockFlag == 0 && StopFlag == 0 && SCflag == 0 && chukuFlag == 2 && rukuFlag==0) {        //û������������ʻ
		if(recordMode == 1)
		{
			dirpid.p = sp;
			if(abs(angle_ring)>(ring_over*0.85)&&abs(angle_ring)<ring_over)//�����ٶ�
			{
				dirpid.p = 10 * sp;
			}
				RDir_control(DirError, 1.0);
			//dirpid.p = static_p;
		}
		else if(recordMode == 3)
		{
			//dirpid.p = sp;
			//relatedCal();
			if(abs(angle_ring)>(ring_over*0.85)&&abs(angle_ring)<ring_over)//�����ٶ�
			{
				dirpid.p = 10 * sp;
			}
			RDir_control(DirError, relation);
		}
		CountNum = 0;
	}
	else
	{
		if(chukuFlag==2)
		{
			if(turnFlag == 1)
			{
				pwm_duty(PWM4_MODULE2_CHA_C30, 85+SERVO_MID);      //��ת
			}
			if(turnFlag == -1)
			{
				pwm_duty(PWM4_MODULE2_CHA_C30, -85+SERVO_MID);      //��ת
			}
		}
		CountNum++;
		if (CountNum > 201)
			CountNum = 1;
	}
	
	if (CountNum > 200)
		StopFlag = 1;
	
	
	if(recording)
	{
		posL[recordNum]		 = leftSpeedInt / 10;
		posR[recordNum]		 = rightSpeedInt / 10;
		servo[recordNum]	 = MPWM;
		angle_int[recordNum] = gyro_x_i / 100;
		
		if(ringflag_st == 1)
		{
			ringContinue = 1;
		}
		
		if(ringContinue)
			status[recordNum] = 100;
		
		if(inring_st == 0 && ringContinue == 1)
		{
			ringContinue = 0;
		}
		recordNum += 1;
		
		if(rukuFlag == 3)
		{
			recording = 0;
			page = 4;
			oled_fill(0x00);
		}
	}
	
	
	//OSC[0]=angle_y;
	/*OSC[1]=ADCR;
	OSC[2]=gyro_x_i / 100;
	OSC[3]=MPWM * 10;
	OSC[4]=DirError * 10;
	OSC[5]=ADCLL;
	OSC[6]=leftSpeedInt/10;
	OSC[7]=rightSpeedInt/10;*/
	//vcan_sendware(OSC,sizeof(OSC));
	
	
	hillProcess();
	chuku();
	ruku();
	setSpeed();

	OLED_switch();
	//statusShowNumber();
	if(hillFlag==3||(leftSpeedInt+rightSpeedInt)/2>FTMint_fin-5000&&FTMfin_mark&&rukuFlag!=3)
		gpio_set(D16,1);
	else
		gpio_set(D16,0);
}


void PIT_IRQHandler(void)
{
	
	if(PIT_FLAG_GET(PIT_CH0))
	{
		PIT_5MS();
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

