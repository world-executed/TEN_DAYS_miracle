#include "headfile.h"

uint16 readyinring_st=0;
uint16 readyinring_nd=0;
uint8 ringflag_st=0;
uint8 ringflag_nd=0;
uint8 inring_st=0;
uint8 inring_nd=0;
uint8 ringstate=0;
void ringjudge_st()
{
  if(ADC[1]+ADC[2]>4000&&ADC[2]/ADC[1]>1.3&&ADC[4]+ADC[5]>5000)//youhuan  //1000,2400
    ringflag_st=1;
  
  else
    ringflag_st=0;
}

void ringjudge_nd()
{
  if(ADC[1]+ADC[2]>3200&&ADC[1]>2000&&ADC[0]+ADC[3]>5000)
    ringflag_nd=1;
  else
    ringflag_nd=0;
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

void ringProcess()
{
  if(readyinring_st&&ringstate==0)
  {
    if(dip[0]==1)
      MPWM=-70;
    else
      MPWM=70;
    if(readyinring_st==50)
    {ringstate=1;
    
    inring_st=1;}
    gpio_set(D16,1);
  }
  else
    gpio_set(D16,0);
    
    
    if(inring_st==1&&ADC[2]>2000)
      inring_st=0;
  
  if(readyinring_nd&&ringstate==1)
  {
    if(dip[1]==1)
      MPWM=-75;
    else
      MPWM=75;
    if(readyinring_nd==100)
      
    {ringstate=2;}
    gpio_set(D16,1);
    inring_nd=1;
  }  
  else 
    gpio_set(D16,0);
  
  if(ringstate==2&&ADC[1]>1300)
    inring_nd=0;
}
