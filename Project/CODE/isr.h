/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		isr
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
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
extern uint8 ringflag_st;  //1为右环 -1为左环
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

extern int CountNum;
extern int roadMode;
extern int gyro_x_i;
extern int gyro_y_i;
extern int gyro_z_i;

#endif
