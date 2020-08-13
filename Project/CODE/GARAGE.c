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

}


void ruku()
{

  static uint8 banmaxian[128*3]={0};


  if(mt9v03x_csi_finish_flag)
  {
    mt9v03x_csi_finish_flag=0;
    //getHist(*mt9v03x_csi_image,hist);
    
  //th=GetOSTUThreshold(hist);




    //例如访问第10行 50列的点，mt9v03x_csi_image[10][50]就可以了
    for(int i=0;i<128*3;i++)
    {
      banmaxian[i]=mt9v03x_csi_image[i/128+55][i%128];
    }
    jumpnum=0;
    for(int i=1;i<128*3;i++)
    {

      if((banmaxian[i-1]<90&&banmaxian[i]>180)||(banmaxian[i-1]>180&&banmaxian[i]<90))
        jumpnum++;
    }


  

  
  if(jumpnum>16&&rukuFlag==0)
  {rukuFlag=1;gyro_x_i=0;}
  /*
      oled_dis_bmp(64,128,*mt9v03x_csi_image,th);
      oled_printf_int32(30,4,rukuFlag,3);
      oled_printf_int32(30,5,jumpnum,3);
      oled_printf_int32(30,6,jumpnum_b,3);
      oled_printf_int32(30,7,th,3);*/
  }
  

  if(rukuFlag==1)
    
  {
    
    pwm_duty(PWM4_MODULE2_CHA_C30,80*FANGXIANG+SERVO_MID);
    if(abs(angle)>86)
    { 
              rukuFlag=3;
              StopCar();
    }
  }
  
  
}
void chuku()
{
  if(chukuFlag==1)
  {
    uint8 servo_bias=100-FANGXIANG*angle*0.9;
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
  systick_delay_ms(200);
  pwm_duty(PWM1_MODULE0_CHB_D13,0);//左反
  pwm_duty(PWM1_MODULE0_CHA_D12,0);//左正
  pwm_duty(PWM1_MODULE1_CHB_D15,0);//右正
  pwm_duty(PWM1_MODULE1_CHA_D14,0);//右反
}