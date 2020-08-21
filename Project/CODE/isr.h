/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		isr
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.24
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/
 


#ifndef _isr_h
#define _isr_h

extern uint8 pit;
extern float OSC[8];
void PIT0_IRQHandler(void);
extern uint8 StopFlag;
extern uint8 ringflag_st;  //1Ϊ�һ� -1Ϊ��
extern uint8 ringflag_nd;
extern uint8 ringstate;
extern uint16 readyinring_st;
extern uint16 readyinring_nd;
extern uint8 inring_st;
extern uint8 inring_nd;
extern int setLeftSpeed_L;
extern int setRightSpeed_L;
extern int32 gyro_int;
extern int16 angle;
extern uint8 curve_to_straight;
extern int16 curve_judge_th;
float angle_calc(float angle_m, float gyro_m);
extern int Count;
extern int CountNum;
extern int roadMode;
extern int gyro_x_i;
extern int gyro_y_i;
extern int gyro_z_i;
extern float speedK;

#endif
