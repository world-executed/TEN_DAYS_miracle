#include "headfile.h"

uint16 readyinring_st=0;
uint16 readyinring_nd=0;
uint8 ringflag_st=0;
uint8 ringflag_nd=0;
uint8 inring_st=0;
uint8 inring_nd=0;
uint8 ringstate=0;

uint8 ring_servo=55;  //�뾶100�����40���뾶50�����60��
int RMPWM=0;
int count=0;
int angle_ring;
void RingProcess()
{

  static float gyro_x_i_ring;
  if(ringstate==0&&ADC[FANGXIANG==1?1:2]>2700&&ADC[4]+ADC[5]>6000&&(hillFlag==0||hillFlag==4)/*ringstate==0&&ADC[0]>2000&&ADC[1]>2600&&(hillFlag==0||hillFlag==4)*/)//youhuan  //1000,2400
    ringflag_st=1;
  else
    ringflag_st=0;
  
  
  
  if(ringflag_st&&readyinring_st==0)
  {
    readyinring_st=1;//�뻷���
    inring_st=1;
    ringstate=1;
  }

  
  if(inring_st==1)
  {
    gyro_x_i_ring+=gyro[0];
    angle_ring=gyro_x_i_ring*360.0/GYRO360;
  }
  
  if(readyinring_st&&abs(angle_ring)>30)
  {
    readyinring_st=0;//�������
  }  
  /*
  if(abs(angle_ring)>180&&abs(angle_ring)<270)
  {
    RMPWM+=MPWM;
    count++;
  }
  if(abs(angle_ring)>270&&abs(angle_ring)<330)
  {
    //MPWM=RMPWM/count;
    //MPWM=(int)range(MPWM,-85,85);
  }*/
  
  
  if(abs(angle_ring)>355)
  {
    inring_st=0;  
    gyro_x_i_ring=angle_ring=RMPWM=count=0;
  }
  
  


}