#include "headfile.h"

uint8 hillFlag=0;
int8 FANGXIANG=1;//1为顺时针，-1为逆时针
uint8 chukuFlag=0;
uint8 rukuFlag=0;
uint8 th=110;
uint8 jumpnum=0;
uint8 jumpnum_b=0;
int32 gyro_x_i_ruku=0;
void hillProcess()
{
  if(gyro[1]>1000&&hillFlag==0&&chukuFlag==2)
    hillFlag=1;//上坡
  if(gyro[1]<-1000&&hillFlag==1)
    hillFlag=2;//坡顶
  if(gyro[1]>1000&&hillFlag==2)
    hillFlag=3;//完成坡道
  
}


void ruku()
{
  
  static uint8 banmaxian[128*3]={0};
  static uint8 banmaxian_b[128*3]={0};
  
  
  if(mt9v03x_csi_finish_flag)
  {
    mt9v03x_csi_finish_flag=0;

    //例如访问第10行 50列的点，mt9v03x_csi_image[10][50]就可以了
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
    
    

    if(jumpnum>20&&hillFlag==3&&rukuFlag==0)//已经经过坡道
      rukuFlag=1;//预识别
    if(jumpnum_b>16&&hillFlag==3&&rukuFlag<=1)
      rukuFlag=2;

    
  }
  
  
  if(rukuFlag==2)
  {
    
    gyro_x_i_ruku+=gyro[0];
    int angle_ruku=(int)(gyro_x_i_ruku*360.0/700000.0);
    pwm_duty(PWM4_MODULE2_CHA_C30,80*FANGXIANG+SERVO_MID);
    if(abs(angle_ruku)>80)
    { 
      rukuFlag=3;
      gyro_x_i_ruku=0;
      StopCar();
    }
  }
  
  
}
void chuku()
{
  if(dip[0])//1号拨码开关开着，为顺时针
    FANGXIANG=1;
  else
    FANGXIANG=-1;
  
  
  
  if(chukuFlag==1)
  {
    int8 servo_bias=100-(int)(FANGXIANG*angle*0.9);
    servo_bias=range(servo_bias,-70,70);
    
    if(angle<80)
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
  
  pwm_duty(PWM1_MODULE0_CHB_D13,0);//左反
  pwm_duty(PWM1_MODULE0_CHA_D12,0);//左正
  pwm_duty(PWM1_MODULE1_CHB_D15,0);//右正
  pwm_duty(PWM1_MODULE1_CHA_D14,0);//右反
  systick_delay_ms(5);
  pwm_duty(PWM1_MODULE0_CHB_D13,4000);//左反
  pwm_duty(PWM1_MODULE0_CHA_D12,0);//左正
  pwm_duty(PWM1_MODULE1_CHB_D15,0);//右正
  pwm_duty(PWM1_MODULE1_CHA_D14,4000);//右反
  systick_delay_ms(250);
  pwm_duty(PWM1_MODULE0_CHB_D13,0);//左反
  pwm_duty(PWM1_MODULE0_CHA_D12,0);//左正
  pwm_duty(PWM1_MODULE1_CHB_D15,0);//右正
  pwm_duty(PWM1_MODULE1_CHA_D14,0);//右反
}