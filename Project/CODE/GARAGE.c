#include "headfile.h"

uint8 hillFlag=0;
int FANGXIANG=1;//1Ϊ˳ʱ�룬-1Ϊ��ʱ��
uint8 chukuFlag=0;
uint8 rukuFlag=0;
uint8 th=128;
uint8 jumpnum=0;
uint8 jumpnum_b=0;
int32 gyro_x_i_ruku=0;
uint8 Zebra_line = 0;
uint8 startline=40;

float angle_x=0;
float angle_y=0;

/*
float acc_ratio = 1.6;      //���ٶȼƱ���    
float gyro_ratio = 4.08;    //�����Ǳ���   
float Complementary_filter(float angle_m, float gyro_m, uint8 i)    
{    
float temp_angle;               
float gyro_now;    
float error_angle;    

static float last_angle[2];    
static uint8 first_angle[2];    

if(!first_angle[i])//�ж��Ƿ�Ϊ��һ�����б�����    
{    
//����ǵ�һ�����У����ϴνǶ�ֵ����Ϊ����ٶ�ֵһ��    
first_angle[i] = 1;    
last_angle[i] = angle_m;    
  }    

gyro_now = gyro_m * gyro_ratio;    

//���ݲ������ļ��ٶ�ֵת��Ϊ�Ƕ�֮�����ϴεĽǶ�ֵ��ƫ��    
error_angle = (angle_m - last_angle[i])*acc_ratio;  

//����ƫ���������ǲ����õ��ĽǶ�ֵ���㵱ǰ�Ƕ�ֵ    
temp_angle = last_angle[i] + (error_angle + gyro_now)*0.005;   

//���浱ǰ�Ƕ�ֵ    
last_angle[i] = temp_angle;  

return temp_angle;    
}



void MPU6050()
{
static float angle_x_gyro;
static float angle_x_acc;
static float angle_y_gyro;
static float angle_y_acc;
get_accdata();	//��ȡ���ٶȼ�����
get_gyro();		//��ȡ����������
acc[0]=mpu_acc_x;
acc[1]=mpu_acc_y;
acc[2]=mpu_acc_z;
gyro[0]=mpu_gyro_x-gyro_offset[0];
gyro[1]=mpu_gyro_y-gyro_offset[1];
gyro[2]=mpu_gyro_z-gyro_offset[2];

gyro_x_i+=gyro[0];
angle_x_gyro=gyro_x_i*360.0/700000.0;
angle_x_acc=1023-atan2(acc[2],acc[1])*57.3;

angle_x=Complementary_filter(angle_x_acc,angle_x_gyro,0);

gyro_y_i+=gyro[1];
angle_y_gyro=gyro_y_i*360.0/700000.0;
angle_y_acc=76-atan2(acc[0],acc[2])*57.3;

angle_y=Complementary_filter(angle_y_acc,angle_y_gyro,1);
}
*/

float Complementary_filter_1_order(float angle_m, float gyro_m)
{
  static float angle_R;
  float K1 =0.1;
  angle_R = K1 * angle_m + (1-K1) * (angle_R + gyro_m * 0.005);
  return angle_R;
}


float acc_ratio = 1.0;      //���ٶȼƱ���    
float gyro_ratio = 4.08;    //�����Ǳ���  
float Complementary_filter(float angle_m, float gyro_m)    
{    
  float temp_angle;               
  float gyro_now;    
  float error_angle;    
  
  static float last_angle;    
  static uint8 first_angle;    
  
  if(!first_angle)//�ж��Ƿ�Ϊ��һ�����б�����    
  {    
    //����ǵ�һ�����У����ϴνǶ�ֵ����Ϊ����ٶ�ֵһ��    
    first_angle = 1;    
    //last_angle = angle_m; 
    last_angle=gyro_m;
  }    
  
  gyro_now = gyro_m * gyro_ratio;    
  
  //���ݲ������ļ��ٶ�ֵת��Ϊ�Ƕ�֮�����ϴεĽǶ�ֵ��ƫ��    
  error_angle = (angle_m - last_angle)*acc_ratio;  
  
  //����ƫ���������ǲ����õ��ĽǶ�ֵ���㵱ǰ�Ƕ�ֵ    
  temp_angle = last_angle + (error_angle + gyro_now)*0.005;   
  
  //���浱ǰ�Ƕ�ֵ    
  last_angle = temp_angle;  
  
  return temp_angle;    
}



void MPU6050()
{
  static float angle_x_gyro;
  static float angle_x_acc;
  static float angle_y_gyro;
  static float angle_y_acc;
  
  static float angle_y_bias=81;
  get_accdata();	//��ȡ���ٶȼ�����
  get_gyro();		//��ȡ����������
  acc[0]=mpu_acc_x;
  acc[1]=mpu_acc_y;
  acc[2]=mpu_acc_z;
  gyro[0]=mpu_gyro_x-gyro_offset[0];
  gyro[1]=mpu_gyro_y-gyro_offset[1];
  gyro[2]=mpu_gyro_z-gyro_offset[2];
  
  gyro_x_i+=gyro[0];
  angle_x_gyro=gyro_x_i*360.0/GYRO360;
  
  angle_x=angle_x_gyro;//���û����˲�
  
  
  gyro_y_i+=gyro[1];
  angle_y_gyro=gyro_y_i*360.0/GYRO360;
  angle_y_acc=angle_y_bias-atan2(acc[0],acc[2])*57.3;
  
  angle_y=Complementary_filter_1_order(angle_y_acc,angle_y_gyro);//ʹ�û����˲�
}


void hillProcess()
{
  uint16 vision_loss=0;
  for(uint8 i = 0;i<64;i++)
    for(uint8 j=0;j<128;j++)
      if(mt9v03x_csi_image[i][j]>100)
        vision_loss++;//�׵����
  
  if(vision_loss>64*128-5&&hillFlag==0&&chukuFlag==2)
    hillFlag=1;
  
  /*TODO:�µ���ʱ���*/
  static uint16 hill_prev_int;

  if(hillFlag==1&&angle_y<20)
  {
    hill_prev_int++;
  }
  if(hill_prev_int>500)//500�����ں���û������
  {
    hillFlag=0;
    hill_prev_int=0;
  }

    

  static uint16 hill_int;
  if(angle_y>20&&hillFlag==1)
    hillFlag=2;//����
  if(hillFlag==2&&hill_int==0)
    hill_int++;
  if(hill_int)
  {
    
    hill_int++;
    if(hill_int==170)
      hillFlag=3;
    if(hill_int==200)
    {
      hillFlag=4;
      hill_int=0;
      
    }
    
  }/*
  if(angle_y>20&&hillFlag==1)
  {
    hillFlag=2;//����
    hill_prev_int=0;
  }
  if(angle_y<-3&&hillFlag==2)
    hillFlag=3;
  if(angle_y>-1&&hillFlag==3)
    hillFlag=4;
  */
  
}


void ruku()
{
  
  //static uint8 banmaxian[128*3]={0};
  //static uint8 banmaxian_b[128*3]={0};
  
  
  if(mt9v03x_csi_finish_flag)
  {
    mt9v03x_csi_finish_flag=0;
    /*
    //������ʵ�10�� 50�еĵ㣬mt9v03x_csi_image[10][50]�Ϳ�����
    for(int i=0;i<128*3;i++)
    {
    banmaxian[i]=mt9v03x_csi_image[i/128+20][i%128];
    banmaxian_b[i]=mt9v03x_csi_image[i/128+50][i%128];
  }
    jumpnum=0;jumpnum_b=0;
    for(int i=1;i<128*3;i++)
    {
    
    if((banmaxian[i-1]<110&&banmaxian[i]>110)||(banmaxian[i-1]>110&&banmaxian[i]<110))
    jumpnum++;
    if((banmaxian_b[i-1]<110&&banmaxian_b[i]>110)||(banmaxian_b[i-1]>110&&banmaxian_b[i]<110))
    jumpnum_b++;
  }
    
    
    
    if(jumpnum>20&&hillFlag==3&&rukuFlag==0)//�Ѿ������µ�
    rukuFlag=1;//Ԥʶ��
    if(jumpnum_b>16&&hillFlag==3&&rukuFlag<=1)
    rukuFlag=2;
    */
    check_zebra_line();
    if(Zebra_line&&hillFlag==4&&rukuFlag<=1)
      rukuFlag=2;
    
  }
  
  
  if(rukuFlag==2)
  {
    
    gyro_x_i_ruku+=gyro[0];
    int angle_ruku=gyro_x_i_ruku*360.0/GYRO360;
    pwm_duty(PWM4_MODULE2_CHA_C30,80*FANGXIANG+SERVO_MID);
    if(abs(angle_ruku)>70)
    { 
      rukuFlag=3;
      gyro_x_i_ruku=0;
      StopCar();
    }
  }
  
  
}
void chuku()
{
  
  if(dip[3])//4�Ų��뿪�ؿ��ţ�Ϊ˳ʱ��
  FANGXIANG=1;
  else
  FANGXIANG=-1;
  
  
  
  
  if(chukuFlag==1)
  {
    int8 servo_bias=100-(int)(FANGXIANG*angle_x*0.9);
    servo_bias=range(servo_bias,-70,70);
    
    if(angle_x<70)
    {
      pwm_duty(PWM4_MODULE2_CHA_C30,servo_bias*FANGXIANG+SERVO_MID);
    }
    else
    {
      chukuFlag=2;
    }
    
    
  }
  
}

void StopCar()
{
  pwm_duty(PWM4_MODULE2_CHA_C30,SERVO_MID);
  
  pwm_duty(PWM1_MODULE0_CHB_D13,0);//��
  pwm_duty(PWM1_MODULE0_CHA_D12,0);//����
  pwm_duty(PWM1_MODULE1_CHB_D15,0);//����
  pwm_duty(PWM1_MODULE1_CHA_D14,0);//�ҷ�
  systick_delay_ms(5);
  pwm_duty(PWM1_MODULE0_CHB_D13,8000);//��
  pwm_duty(PWM1_MODULE0_CHA_D12,0);//����
  pwm_duty(PWM1_MODULE1_CHB_D15,0);//����
  pwm_duty(PWM1_MODULE1_CHA_D14,8000);//�ҷ�
  systick_delay_ms(250);
  pwm_duty(PWM1_MODULE0_CHB_D13,0);//��
  pwm_duty(PWM1_MODULE0_CHA_D12,0);//����
  pwm_duty(PWM1_MODULE1_CHB_D15,0);//����
  pwm_duty(PWM1_MODULE1_CHA_D14,0);//�ҷ�
}

uint8 black_blocks;
void check_zebra_line()
{
  black_blocks = 0;
  startline=-0.5*(SetLeftSpeed-40)+50;
  for (uint8 y = startline; y < startline+3; y++)
  {
    
    uint8 cursor = 0;    //ָ��ջ�����α�
    for (uint8 x = 0; x <= 128; x++)
    {
      if (mt9v03x_csi_image[y][x] <110)//��ɫ
      {
        cursor++;
        cursor%=40;
      }
      else //��ɫ
      {
        if (cursor >= 3 && cursor <= 20)
          black_blocks++;
        cursor = 0;
      }
    }  
  }
  
  if (black_blocks>10)
    Zebra_line = 1;
  else 
    Zebra_line = 0;
}