#include "headfile.h"

float static_p = 1.30;
float static_d = 0.02;

uint8 dip[4]={0};				//拨码开关
uint8 ring_r_st=80;				//第一个环的曲率
uint8 ring_r_nd=50;				//第二个环的曲率
int32 gyro_offset[3]={0};		//角速度计偏移
int32 acc_offset[3]={0};		//加速度计偏移

float sp = 0.0;
float lp = 0.0;

//PID参数初始化
void PID_init()
{

    pid_l.err=0;
    pid_l.lasterr=0;
    pid_l.preverr=0;
    pid_l.p=8;
    pid_l.i=1.2;
    pid_l.d=0;
    

    pid_r.err=0;
    pid_r.lasterr=0;
    pid_r.preverr=0;
    pid_r.p=8;
    pid_r.i=1.2;
    pid_r.d=0;
    
    
    dirpid.p=static_p;
    dirpid.i=0.00;
    dirpid.d=static_d;
    dirpid.err=0;
    dirpid.lasterr=0;
    dirpid.preverr=0;
    
}

void AllInit()
{
  DisableGlobalIRQ();
  board_init();//务必保留，本函数用于初始化MPU 时钟 调试串口
  systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
  
  pwm_init(PWM1_MODULE0_CHB_D13, 13*1000, 0);
  pwm_init(PWM1_MODULE0_CHA_D12, 13*1000, 0);
  pwm_init(PWM1_MODULE1_CHB_D15, 13*1000, 0);
  pwm_init(PWM1_MODULE1_CHA_D14, 13*1000, 0);
  pwm_init(PWM4_MODULE2_CHA_C30, 50, SERVO_MID);//舵机
  
  
  qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
  qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
  
  PID_init();         //PID初始化
  pit_init();                                 //初始化PIT0，最小采样10ms
  pit_interrupt_ms(PIT_CH0,1);
  NVIC_SetPriority(PIT_IRQn,15);
  
  oled_init();
  key_init(KEY_MAX);
  gpio_init (C25, GPI,0,GPIO_PIN_CONFIG);   //拨码
  gpio_init (C26, GPI,0,GPIO_PIN_CONFIG);   
  gpio_init (C27, GPI,0,GPIO_PIN_CONFIG);
  gpio_init (C28, GPI,0,GPIO_PIN_CONFIG);
  
  adc_init(ADC_1,ADC1_CH4_B15,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH3_B14,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH5_B16,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH7_B18,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH6_B17,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH8_B19,ADC_12BIT);

  
  gpio_init (D16, GPO,0,GPIO_PIN_CONFIG); //蜂鸣器初始化
  oled_p6x8str(0,0,"Camera Initializing");
  
  
  mt9v03x_csi_init();//初始化摄像头 使用CSI接口
  oled_p6x8str(0,2,"IIC Initializing");
  simiic_init();		//模拟IIC初始化
  oled_p6x8str(0,4,"MPU6050 Initializing");
	mpu6050_init();		//初始化六轴加速度计陀螺仪
  systick_delay_ms(200);
  oled_fill(0x00);


    gyro_offset[0]=-20;
    gyro_offset[1]=40;
    gyro_offset[2]=0;
    acc_offset[0]=3940;
    acc_offset[1]=-70;
    acc_offset[2]=1350;

	//临时
	//elementFind();
	//eachLenth();
	//aveServo();
	
	
	for(int i = 0; i < calNum; i++)
	{
		switch(status[i])
		{
		case 2: case 6:
			status[i] = 6; break;				//事实上，2为准备左转，7正在左转，6已经左转；
		case 0: case 9:
			status[i] = 9; break;
		default:
			break;
		}
	}
		
	statusShrink();
			
	for(int i = 0; i < calNum; i++)
	{
		if(status[i] == 6)
			servo_tem[i] = 600;
		else if(status[i] == 9)
			servo_tem[i] = -600;
		else if(status[i] == 1)
			servo_tem[i] = 0;
	}	
	
	SpeedStatus();
	/*
	for(int i = 0; i < calNum; i++)
	{
		status[calNum] 		= 1;	
		posL[calNum] 		= -10000;
	}
*/
	
  EnableGlobalIRQ(0);
}

