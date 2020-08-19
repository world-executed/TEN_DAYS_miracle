#include "headfile.h"

uint16 readyinring_st=0;
uint16 readyinring_nd=0;
uint8 ringflag_st=0;
uint8 ringflag_nd=0;
uint8 inring_st=0;
uint8 inring_nd=0;
uint8 ringstate=0;
float ring_ratio=1.8;
uint8 ring_servo=55;
int RMPWM=0;
int count=0;
int angle_ring;
void RingProcess()
{

  static float gyro_x_i_ring;
  if(ringstate==0&&ADC[0]>1500&&ADC[1]>2700&&ADC[4]+ADC[5]>7000&&(hillFlag==0||hillFlag==4)/*ringstate==0&&ADC[0]>2000&&ADC[1]>2600&&(hillFlag==0||hillFlag==4)*/)//youhuan  //1000,2400
    ringflag_st=1;
  else
    ringflag_st=0;
  
  
  
  if(ringflag_st&&readyinring_st==0)
  {
    readyinring_st=1;//入环打角
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
    readyinring_st=0;//结束打角
    //static_p=2.4;
//dirpid.p=sp;
  }  
  
  if(abs(angle_ring)>180&&abs(angle_ring)<270)
  {
    RMPWM+=MPWM;
    count++;
  }
  if(abs(angle_ring)>270&&abs(angle_ring)<330)
  {
    //MPWM=RMPWM/count;
    //MPWM=(int)range(MPWM,-85,85);
  }
  









  
  
  if(abs(angle_ring)>355)
  {
    inring_st=0;  
    gyro_x_i_ring=angle_ring=RMPWM=count=0;
    //dirpid.p=lp;
  }
  
  


}
/*
void ringProcess()
{
if(readyinring_st&&ringstate==0)
{
if(dip[0]==1)
MPWM=-60;
    else
MPWM=60;
if(readyinring_st==50)
{
ringstate=1;
inring_st=1;
    }

  }



if(inring_st==1&&ADC[2]>2000)
inring_st=0;

if(readyinring_nd&&ringstate==1)
{
if(dip[1]==1)
MPWM=-75;
    else
MPWM=75;
if(readyinring_nd==100) 
{
ringstate=2;
    }

inring_nd=1;
  }  


if(ringstate==2&&ADC[1]>1300)
inring_nd=0;
}


void ring_int()
{
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
}
*/
/*
void RingProcess()
{
  static int angle_ring;
  static float gyro_x_i_ring;
  if(ringstate==0&&ADC[1]+ADC[2]>1500&&ADC[1]/ADC[2]>ring_ratio&&ADC[4]+ADC[5]>5000&&(hillFlag==0||hillFlag==4))//youhuan  //1000,2400
    ringflag_st=1;
  else
    ringflag_st=0;
  
  
  
  if(ringflag_st&&readyinring_st==0)
  {
    readyinring_st=1;//入环打角
    inring_st=1;
    ringstate=1;
  }

  
  if(inring_st==1)
  {
    gyro_x_i_ring+=gyro[0];
    angle_ring=gyro_x_i_ring*360.0/GYRO360;
  }
  
  if(readyinring_st&&abs(angle_ring)>40)
  {
    readyinring_st=0;//结束打角
    //static_p=2.4;
//dirpid.p=sp;
  }  
  
  
  if(abs(angle_ring)>350)
  {
    inring_st=0;  
    gyro_x_i_ring=angle_ring=0;
    //dirpid.p=lp;
  }
  
  
  //打角
  if(readyinring_st)
  {
      MPWM=ring_servo+range((ADC[4]-ADC[5])/200,-15,15);
      MPWM=(int)range(MPWM,-85,85);
      
  }

}*/