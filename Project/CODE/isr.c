#include "headfile.h"

uint8 pit=0;
int CountNum = 0;
uint8 StopFlag = 1;

int roadMode;

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
  lp = static_p * 0.3;
  
  recordMode = 1;
  roadMode = 1;
  turnFlag = 0;
  LockFlag = 0;
  
  if(recordMode == 3)
	{
		PosCalculate();
		eleMatch();
		
		if(status[nowPos] != 1) //���
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
  //Prepare_Data();
  //ImuCalculate_Complementary();
  MPU6050();
  gyro_x_i+=gyro[0];
  angle=(int)(gyro_x_i*360.0/700000.0);
  
  gyro_y_i+=abs(gyro[1])>10?gyro[1]:0;
  
  GetError();
  //DynamicPID();
  ADCTest();
  //modeSelect();
  if (LockFlag == 0 && StopFlag == 0 && chukuFlag==2&&rukuFlag<=1) {        //û����
	  if(recordMode != 3)
    		Dir_control(DirError);
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
  if(ringstate==1)
  //ringjudge_nd();
  
  OSC[0]=0;
  OSC[1]=LeftSpeed;
  OSC[2]=SetLeftSpeed;
  OSC[3]=PWML/100.0;
  OSC[4]=mpu_acc_y;
  OSC[5]=mpu_acc_z;
  OSC[6]=leftSpeedInt/10;
  OSC[7]=rightSpeedInt/10;
  vcan_sendware(OSC,sizeof(OSC));
  
  
  if(ringflag_st)
    readyinring_st++;
  if(readyinring_st)
  {
    readyinring_st++;
    readyinring_st%=500;
  }
  
  if(ringflag_nd)
    readyinring_nd++;
  if(readyinring_nd)
  {
    readyinring_nd++;
    readyinring_nd%=500;
  }
  setSpeed();
  hillProcess();
  chuku();
  
  if(hillFlag==3)
  ruku();


  
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
