#ifndef _ADCSAMPEL_H_
#define _ADCSAMPLE_H_

extern int16 ADC[6];
extern int16 ADC_tmp[6][5];
extern int16 ADC_u[6];
extern int16 ADC_L[6];
void ADCSample(uint8 i);
void ADCUnit();

#endif