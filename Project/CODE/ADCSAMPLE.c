#include "headfile.h"

#define ADCLOWBOUND 20

#define ADCExMAX_F 2000
#define ADCExMAX_B 3000
#define ADCInMAX  2000
int16 ADC_MAX[6]={ADCExMAX_F,ADCInMAX,ADCInMAX,ADCExMAX_F,ADCExMAX_B,ADCExMAX_B};
int16 ADC[6]={0,0,0,0,0,0};
int16 ADC_L[6]={0};
int16 ADC_u[6]={0};
int16 ADC_tmp[6][5]={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}};
void ADCTest()
{
	if(ADCL<ADCLOWBOUND || ADCR<ADCLOWBOUND)
	{
		LockFlag = 1;
	}
	else
	{
		LockFlag = 0;
		StopFlag = 0;
	}
	
	if(isTurnEnable == 0 && ADCL < 20&& ADCR > 20)
	{
		turnFlag = -1;                  //×ó×ª
		isTurnEnable = 1;
	}
	if(isTurnEnable == 0 && ADCR < 20&& ADCL > 20)
	{
		turnFlag = 1;                   //ÓÒ×ª  
		isTurnEnable = 1;
	}
	if(ADCL > 20 && ADCR > 20)
	{
		isTurnEnable = 0;
		turnFlag = 0;
	}
}

void ADCSample(uint8 i)
{
  ADC_tmp[0][i]=adc_mean_filter(ADC_1,ADC1_CH6_B17,1);//qianzuo
  ADC_tmp[1][i]=adc_mean_filter(ADC_1,ADC1_CH7_B18,1);//qianzuoxie
  ADC_tmp[2][i]=adc_mean_filter(ADC_1,ADC1_CH8_B19,1);//qianyouxie
  ADC_tmp[3][i]=adc_mean_filter(ADC_1,ADC1_CH5_B16,1);//qianyou
  ADC_tmp[4][i]=adc_mean_filter(ADC_1,ADC1_CH3_B14,1);//houzuo
  ADC_tmp[5][i]=adc_mean_filter(ADC_1,ADC1_CH4_B15,1);//houyou
  if(i==4){
    for(int k=0;k<6;k++)
    {
      ADC_L[k]=ADC[k];
      ADC[k]=0;
      for(int j=0;j<5;j++)
      {
        ADC[k]+=ADC_tmp[k][j];
      }
      ADC[k]/=5;
    }
  }
}

void ADCUnit()
{
  for(uint8 i=0;i<6;i++)
    ADC_u[i]=ADC[i]<ADC_MAX[i]?10000*ADC[i]/ADC_MAX[i]:9999;
}