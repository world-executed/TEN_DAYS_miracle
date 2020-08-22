#include "headfile.h"

float static_p = 1.70;
float static_d = 0.02;

uint8 dip[4]={0};				//���뿪��
uint8 ring_r_st=80;				//��һ����������
uint8 ring_r_nd=50;				//�ڶ�����������
int32 gyro_offset[3]={0};		//���ٶȼ�ƫ��
int32 acc_offset[3]={0};		//���ٶȼ�ƫ��
int32 GYRO360 = 1120000;

float sp = 0.0;
float lp = 0.0;

//PID������ʼ��
void PID_init()
{
	pid_l.err=0;
    pid_l.lasterr=0;
    pid_l.preverr=0;
    pid_l.p=100;
    pid_l.i=10;
    pid_l.d=0;
    

    pid_r.err=0;
    pid_r.lasterr=0;
    pid_r.preverr=0;
    pid_r.p=100;
    pid_r.i=10;
    pid_r.d=0;
    
    
    dirpid.p=lp;
    dirpid.i=0.00;
    dirpid.d=0.02;
    dirpid.err=0;
    dirpid.lasterr=0;
    dirpid.preverr=0;
}

void AllInit()
{
  DisableGlobalIRQ();
  board_init();//��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
  systick_delay_ms(300);	//��ʱ300ms���ȴ��������������ϵ�ɹ�
  
  pwm_init(PWM1_MODULE0_CHB_D13, 13*1000, 0);
  pwm_init(PWM1_MODULE0_CHA_D12, 13*1000, 0);
  pwm_init(PWM1_MODULE1_CHB_D15, 13*1000, 0);
  pwm_init(PWM1_MODULE1_CHA_D14, 13*1000, 0);
  pwm_init(PWM4_MODULE2_CHA_C30, 50, SERVO_MID);//���
  
  
  qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
  qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
  
  PID_init();         //PID��ʼ��
  pit_init();                                 //��ʼ��PIT0����С����10ms
  pit_interrupt_ms(PIT_CH0,5);
  NVIC_SetPriority(PIT_IRQn,15);
  
  oled_init();
  key_init(KEY_MAX);
  gpio_init (C25, GPI,0,GPIO_PIN_CONFIG);   //����
  gpio_init (C26, GPI,0,GPIO_PIN_CONFIG);   
  gpio_init (C27, GPI,0,GPIO_PIN_CONFIG);
  gpio_init (C28, GPI,0,GPIO_PIN_CONFIG);
  
  adc_init(ADC_1,ADC1_CH4_B15,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH3_B14,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH5_B16,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH7_B18,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH6_B17,ADC_12BIT);
  adc_init(ADC_1,ADC1_CH8_B19,ADC_12BIT);

  gpio_init (C17, GPO,0,GPIO_PIN_CONFIG);
  gpio_init (C18, GPO,0,GPIO_PIN_CONFIG);
  gpio_init (B9, GPO,0,GPIO_PIN_CONFIG);
  gpio_init (D16, GPO,0,GPIO_PIN_CONFIG); //��������ʼ��
  oled_p6x8str(0,0,"Camera Initializing");
  
  gpio_init (C16, GPO,0,GPIO_PIN_CONFIG);
  
	
  mt9v03x_csi_init();//��ʼ������ͷ ʹ��CSI�ӿ�
  oled_p6x8str(0,2,"IIC Initializing");
  simiic_init();		//ģ��IIC��ʼ��
  oled_p6x8str(0,4,"MPU6050 Initializing");
	mpu6050_init();		//��ʼ��������ٶȼ�������
  systick_delay_ms(200);
  oled_fill(0x00);

    gyro_offset[0]=-20;
    gyro_offset[1]=40;
    gyro_offset[2]=0;
    acc_offset[0]=3940;
    acc_offset[1]=-70;
    acc_offset[2]=1350;

	//��ʱ
	//elementFind();
	//eachLenth();
	//aveServo();
	
	for(int i = 0; i<calNum; i++)
	{
		status[i] = 1;
		status_tem[i] = 1;
	}

	/*
	for(int i = 0; i < calNum; i++)
	{
		status[calNum] 		= 1;	
		posL[calNum] 		= -10000;
	}
*/
	
  EnableGlobalIRQ(0);
}

