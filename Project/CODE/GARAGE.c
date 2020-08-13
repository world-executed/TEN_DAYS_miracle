#include "headfile.h"

uint8 hillFlag=0;
int8 FANGXIANG=1;//1为顺时针，-1为逆时针
uint8 chukuFlag=0;
uint8 rukuFlag=0;
uint8 th=110;
uint8 jumpnum=0;
uint8 jumpnum_b=0;

void hillProcess()
{
  if(gyro[1]>1000&&hillFlag==0)
    hillFlag=1;//上坡
  if(gyro[1]<-1000&&hillFlag==1)
    hillFlag=2;//坡顶
  if(gyro[1]>1000&&hillFlag==2)
    hillFlag=3;//完成坡道
  
  if(hillFlag==2)
    gpio_set(D16,1);
  else
    gpio_set(D16,0);
  
}


void ruku()
{
  static int hist[256]={0};
  static uint8 banmaxian[128*3]={0};
  static uint8 banmaxian_Back[128*3]={0};

  if(mt9v03x_csi_finish_flag){
    mt9v03x_csi_finish_flag=0;
    //getHist(*mt9v03x_csi_image,hist);
    
  //th=GetOSTUThreshold(hist);




    //例如访问第10行 50列的点，mt9v03x_csi_image[10][50]就可以了
    for(int i=0;i<128*3;i++)
    {
      if(mt9v03x_csi_image[i/128+5][i%128]>200)
        banmaxian[i]=1;
      else
        banmaxian[i]=0;
      
      
      if(mt9v03x_csi_image[i/128+55][i%128]>th)
        banmaxian_Back[i]=1;
      else
        banmaxian_Back[i]=0;
    }
    jumpnum=0;
    for(int i=1;i<128*3;i++)
      if(banmaxian[i-1]^banmaxian[i])
        jumpnum++;
    
    jumpnum_b=0;
    for(int i=1;i<128*3;i++)
      if(banmaxian_Back[i-1]^banmaxian_Back[i])
        jumpnum_b++;
    
  

  
  if(jumpnum>30&&rukuFlag==0)
    rukuFlag=1;
  
  if(jumpnum_b>16&&(rukuFlag==0||rukuFlag==1))
  {rukuFlag=2;gyro_x_i=0;}
  /*
      oled_dis_bmp(64,128,*mt9v03x_csi_image,th);
      oled_printf_int32(30,4,rukuFlag,3);
      oled_printf_int32(30,5,jumpnum,3);
      oled_printf_int32(30,6,jumpnum_b,3);
      oled_printf_int32(30,7,th,3);*/
  }
  

  if(rukuFlag==1)
    
  {
    pwm_duty(PWM1_MODULE0_CHA_D12,2000);//左正
    pwm_duty(PWM1_MODULE1_CHB_D15,2000);//右正
    
  }

  if(rukuFlag==2)
    
  {
    
    pwm_duty(PWM4_MODULE2_CHA_C30,80*FANGXIANG+SERVO_MID);
    if(abs(angle)>86)
    { pwm_duty(PWM4_MODULE2_CHA_C30,SERVO_MID);
        rukuFlag=3;
      pwm_duty(PWM1_MODULE0_CHB_D13,0);//左反
      pwm_duty(PWM1_MODULE0_CHA_D12,0);//左正
      pwm_duty(PWM1_MODULE1_CHB_D15,0);//右正
      pwm_duty(PWM1_MODULE1_CHA_D14,0);//右反
      systick_delay_ms(5);
      pwm_duty(PWM1_MODULE0_CHB_D13,4000);//左反
      pwm_duty(PWM1_MODULE0_CHA_D12,0);//左正
      pwm_duty(PWM1_MODULE1_CHB_D15,0);//右正
      pwm_duty(PWM1_MODULE1_CHA_D14,4000);//右反
      systick_delay_ms(200);
      pwm_duty(PWM1_MODULE0_CHB_D13,0);//左反
      pwm_duty(PWM1_MODULE0_CHA_D12,0);//左正
      pwm_duty(PWM1_MODULE1_CHB_D15,0);//右正
      pwm_duty(PWM1_MODULE1_CHA_D14,0);//右反
    }
  }
}

void chuku()
{
  if(chukuFlag)
  {
    uint8 servo_bias=100-FANGXIANG*angle*0.9;
    servo_bias=range(servo_bias,-70,70);

    if(angle<64)
    {
      pwm_duty(PWM4_MODULE2_CHA_C30,servo_bias*FANGXIANG+SERVO_MID);
    }
    if(angle<80)
    {
    pwm_duty(PWM1_MODULE0_CHA_D12,3000);//左正
    pwm_duty(PWM1_MODULE1_CHB_D15,3000);//右正   
    }
    else
      chukuFlag=2;

  }
}