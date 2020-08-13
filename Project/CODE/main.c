/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file       		main
* @company	   		成都逐飞科技有限公司
* @author     		逐飞科技(QQ3184284598)
* @version    		查看doc内version文件 版本说明
* @Software 		IAR 8.3 or MDK 5.24
* @Target core		NXP RT1064DVL6A
* @Taobao   		https://seekfree.taobao.com/
* @date       		2019-04-30
********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完

//下载代码前请根据自己使用的下载器在工程里设置下载器为自己所使用的

#include "headfile.h"


int8 ringflag=0;  //1为右环 -1为左环
uint16 ring_th_ex=3500;
uint16 ring_th_in=200;
uint16 readyinring_st=0;
uint16 readyinring_nd=0;
uint8 ringflag_st=0;
uint8 ringflag_nd=0;
void signal();
int normal = 1800;
int mode9num = 0;

uint16 readyruku=0;

uint8 value=130;


int16 acc[3];
int16 gyro[3];






int main(void)
{
  DisableGlobalIRQ();
  board_init();//务必保留，本函数用于初始化MPU 时钟 调试串口
  
  //此处编写用户代码(例如：外设初始化代码等)
  
  AllInit();
  while (1)
  {
	  
    OLED_switch();
	
  }
}





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




void modeSelect()
{
	
	if(roadMode == 1)
	{
		setLeftSpeed_L = SetLeftSpeed;
		setRightSpeed_L = SetRightSpeed;
	}
	
	if(roadMode == 9)             //出弯转直道
	{
		mode9num++;
		if(mode9num > 15)
		{
			roadMode = 1;
			dirpid.p = lp;
			mode9num = 0;
			//beep = 5;
			setLeftSpeed_L = SetLeftSpeed;
			setRightSpeed_L = SetRightSpeed;
		}
		
	}
	
	if(roadMode == 1 && abs(ADCLL - ADCRR) > 800)                //直道入弯
	{
		roadMode = 6;
		//beep = 1;
		dirpid.p = sp;
		setLeftSpeed_L = 1.0 * SetLeftSpeed;
		setRightSpeed_L = 1.0 * SetRightSpeed;
	}
	
	if((roadMode == 6) /*&& (abs(ADCLL - normal)<300) && (abs(ADCRR - normal)<300)*/)       //即将出弯
	{
		if(((ADCL0 - ADCR0)/ (ADCL - ADCR))< 0)		//正负颠倒
		{
			roadMode = 9;
			//beep = 1;
		}
	}

  if(ADC[5] > 4000 && ADC[4] > 4000)
    dirpid.p = lp;
}

void AllZero()
{
	leftSpeedInt = 0;
	rightSpeedInt = 0;
	MPWM = 0;
	DirError = 0;
	turnFlag = 0;
	isTurnEnable = 0;
	pureError = 0.0;
	LockFlag = 0;
	CountNum = 0;
	StopFlag = 0;
	gyro_x_i = 0;
	gyro_y_i = 0;
	gyro_z_i = 0;

}

void CarBegin()
{
	AllZero();
	chukuFlag = 1;
    PWML=PWMR=3000;
}

void RecordBegin()
{
	AllZero();
	SetLeftSpeed = 30;
	SetLeftSpeed = 30;
	chukuFlag = 1;
    PWML=PWMR=3000;
	oled_fill(0x00);
	recording = 1;
}

		
void InmodeBegin()
{
	AllZero();
}



/***************************************************不要的************************************************/

/*
float acc_ratio = 1.6;      //加速度计比例    
float gyro_ratio = 4.08;    //陀螺仪比例    
  

//----------------------------------------------------------------    
//  @brief      一阶互补滤波    
//  @param      angle_m     加速度计数据    
//  @param      gyro_m      陀螺仪数据    
//  @return     float       数据融合后的角度    
//----------------------------------------------------------------    
float angle_calc(float angle_m, float gyro_m)    
{    
  float temp_angle;               
  float gyro_now;    
  float error_angle;    
  
  static float last_angle;    
  static uint8 first_angle;    
  
  if(!first_angle)//判断是否为第一次运行本函数    
  {    
    //如果是第一次运行，则将上次角度值设置为与加速度值一致    
    first_angle = 1;    
    last_angle = angle_m;    
  }    
  
  gyro_now = gyro_m * gyro_ratio;    
  
  //根据测量到的加速度值转换为角度之后与上次的角度值求偏差    
  error_angle = (angle_m - last_angle)*acc_ratio;  
  
  //根据偏差与陀螺仪测量得到的角度值计算当前角度值    
  temp_angle = last_angle + (error_angle + gyro_now)*0.005;   
  
  //保存当前角度值    
  last_angle = temp_angle;  
  
  return temp_angle;    
}
*/

void MPU6050()
{
    get_accdata();	//获取加速度计数据
    get_gyro();		//获取陀螺仪数据
    acc[0]=mpu_acc_x-acc_offset[0];
    acc[1]=mpu_acc_y-acc_offset[1];
    acc[2]=mpu_acc_z-acc_offset[2];
    gyro[0]=mpu_gyro_x-gyro_offset[0];
    gyro[1]=mpu_gyro_y-gyro_offset[1];
    gyro[2]=mpu_gyro_z-gyro_offset[2];
}

/*
const float fRad2Deg = 57.295779513f; //弧度换算角度乘的系数
const float dt = 0.005; //时间周期
float angle[3] = {0};
float R = 0.98f;

void ImuCalculate_Complementary(void)//计算角度
{
  MPU6050();
    static float angle_last[3]={0};
    float temp[3] = {0};
    temp[0] = sqrt(acc[1]*acc[1]+acc[2]*acc[2]);
    temp[1] = sqrt(acc[0]*acc[0]+acc[2]*acc[2]);

    for(uint8 i = 0; i < 2; i++)//pitch and roll
    {
        angle[i] = R*(angle_last[i]+gyro[i]*dt) 
                    + (1-R)*fRad2Deg*atan(acc[i]/temp[i]);
        angle_last[i] = angle[i];
    }
    
    angle[2] = angle_last[2]+gyro[2]*dt;//yaw
    angle_last[2] = angle[2];
}



void GetAngle()
{
  get_accdata();	//获取加速度计数据
  get_gyro();		//获取陀螺仪数据
  Angle=atan2(mpu_acc_z,mpu_acc_x)*57.3;
}
static float angle_bias,angle_bias_last;
void GetAngle_I()
{
  GetAngle()
  Integral_angle+=(mpu_gyro_y+19+(angle-Integral_angle)/0.23)*0.0016;
  angle_bias=(0-Integral_angle)*0.8+angle_last*0.2;
  angle_bias_last=angle_bias;
  
}


void getHist(uint8* pic,int *hist)
{
  uint8 *p;
  for (uint8 j = 0; j < MT9V03X_CSI_H; j ++)
  {
    p = (uint8 *)(pic + j * MT9V03X_CSI_W);
    for (uint8 i = 0; i < MT9V03X_CSI_W; i++)
    {
      hist[p[i]]++; //统计直方图
    }
  }
}


uint8 GetOSTUThreshold(int* HistGram)
{
  int X, Y, Amount = 0;
  int PixelBack = 0, PixelFore = 0, PixelIntegralBack = 0, PixelIntegralFore = 0, PixelIntegral = 0;
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma;              // 类间方差;
  int MinValue, MaxValue;
  int Threshold = 0;
  
  for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;
  for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ;
  if (MaxValue == MinValue) return MaxValue;          // 图像中只有一个颜色             
  if (MinValue + 1 == MaxValue) return MinValue;      // 图像中只有二个颜色
  
  for (Y = MinValue; Y <= MaxValue; Y++) Amount += HistGram[Y];        //  像素总数
  
  PixelIntegral = 0;
  for (Y = MinValue; Y <= MaxValue; Y++) PixelIntegral += HistGram[Y] * Y;
  SigmaB = -1;
  for (Y = MinValue; Y < MaxValue; Y++)
  {
    PixelBack = PixelBack + HistGram[Y];
    PixelFore = Amount - PixelBack;
    OmegaBack = (double)PixelBack / Amount;
    OmegaFore = (double)PixelFore / Amount;
    PixelIntegralBack += HistGram[Y] * Y;
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;
    MicroBack = (double)PixelIntegralBack / PixelBack;
    MicroFore = (double)PixelIntegralFore / PixelFore;
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);
    if (Sigma > SigmaB)
    {
      SigmaB = Sigma;
      Threshold = Y;
    }
  }
  return Threshold;
}

static uint8 GetIterativeBestThreshold(uint8* HistGram)
{
  int X, Iter = 0;
  int MeanValueOne, MeanValueTwo, SumOne, SumTwo, SumIntegralOne, SumIntegralTwo;
  int MinValue, MaxValue;
  int Threshold, NewThreshold;
  
  for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;
  for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ;
  
  if (MaxValue == MinValue) return MaxValue;          // 图像中只有一个颜色             
  if (MinValue + 1 == MaxValue) return MinValue;      // 图像中只有二个颜色
  
  Threshold = MinValue;
  NewThreshold = (MaxValue + MinValue) >> 1;
  while (Threshold != NewThreshold)    // 当前后两次迭代的获得阈值相同时，结束迭代    
  {
    SumOne = 0; SumIntegralOne = 0;
    SumTwo = 0; SumIntegralTwo = 0;
    Threshold = NewThreshold;
    for (X = MinValue; X <= Threshold; X++)         //根据阈值将图像分割成目标和背景两部分，求出两部分的平均灰度值      
    {
      SumIntegralOne += HistGram[X] * X;
      SumOne += HistGram[X];
    }
    MeanValueOne = SumIntegralOne / SumOne;
    for (X = Threshold + 1; X <= MaxValue; X++)
    {
      SumIntegralTwo += HistGram[X] * X;
      SumTwo += HistGram[X];
    }
    MeanValueTwo = SumIntegralTwo / SumTwo;
    NewThreshold = (MeanValueOne + MeanValueTwo) >> 1;       //求出新的阈值
    Iter++;
    if (Iter >= 1000) return -1;
  }
  return Threshold;
}

static uint8 IsDimodal(double* HistGram)       // 检测直方图是否为双峰的
    {
        // 对直方图的峰进行计数，只有峰数位2才为双峰 
        int Count = 0;
        for (int Y = 1; Y < 255; Y++)
        {
            if (HistGram[Y - 1] < HistGram[Y] && HistGram[Y + 1] < HistGram[Y])
            {
                Count++;
                if (Count > 2) return 0;
            }
        }
        if (Count == 2)
            return 1;
        else
            return 0;
    }

static uint8 GetIntermodesThreshold(uint8* HistGram)
    {
        int Y, Iter = 0, Index;
        double HistGramC[256] = {0};           // 基于精度问题，一定要用浮点数来处理，否则得不到正确的结果
        double HistGramCC[256] = {0};          // 求均值的过程会破坏前面的数据，因此需要两份数据
        for (Y = 0; Y < 256; Y++)
        {
            HistGramC[Y] = HistGram[Y];
            HistGramCC[Y] = HistGram[Y];
        }
        // 通过三点求均值来平滑直方图
        while (IsDimodal(HistGramCC) == 0)                                                  // 判断是否已经是双峰的图像了      
        {
            HistGramCC[0] = (HistGramC[0] + HistGramC[0] + HistGramC[1]) / 3;                   // 第一点
            for (Y = 1; Y < 255; Y++)
                HistGramCC[Y] = (HistGramC[Y - 1] + HistGramC[Y] + HistGramC[Y + 1]) / 3;       // 中间的点
            HistGramCC[255] = (HistGramC[254] + HistGramC[255] + HistGramC[255]) / 3;           // 最后一点
            for (uint8 i=0;i<255;i++){HistGramCC[i]=HistGramC[i];}
            Iter++;
            if (Iter >= 10000) return -1;                                                       // 似乎直方图无法平滑为双峰的，返回错误代码
        }
// 阈值为两峰值的平均值
        int Peak[2] = {0};
        for (Y = 1, Index = 0; Y < 255; Y++)
            if (HistGramCC[Y - 1] < HistGramCC[Y] && HistGramCC[Y + 1] < HistGramCC[Y]) Peak[Index++] = Y - 1;
        return ((Peak[0] + Peak[1]) / 2);
    }


#define Acc_Gain 0.0002440f			//加速度转换单位(初始化加速度计量程+-4g，由于mpu6050的数据寄存器是16位的，LSBa = 2*4 / 65535.0)
#define Gyro_Gain 0.0609756f		//角速度转换为角度(LSBg = 2000*2 / 65535)
#define Gyro_Gr 0.0010641f			//角速度转换成弧度(3.1415 / 180 * LSBg)
#define G 9.80665f					// m/s^2

short ax,ay,az;
short gx,gy,gz;


static float invSqrt(float x) 		//快速计算 1/Sqrt(x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}




#define Kp 1.50f
#define Ki 0.005f
#define halfT 0.0025f						//计算周期的一半，单位s

float Yaw,Pitch,Roll;				//我要给其他文件用所以定义了extern，不用管
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;		//四元数
float exInt = 0, eyInt = 0, ezInt = 0;		//叉积计算误差的累计积分


void Imu_Update()
{
      MPU6050();
    ax=(float)acc[0]* Acc_Gain * G;
    ay=(float)acc[1]* Acc_Gain * G;
    az=(float)acc[2]* Acc_Gain * G;
    gx=(float)gyro[0]* Gyro_Gr;
    gy=(float)gyro[1]* Gyro_Gr;
    gz=(float)gyro[2]* Gyro_Gr;
	uint8 i;
	float vx,vy,vz;							//实际重力加速度
	float ex,ey,ez;							//叉积计算的误差
	float norm;
	
 	float q0q0 = q0*q0;
 	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
 	float q1q2 = q1*q2;
 	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;
	
	if(ax*ay*az == 0)
		return;
	
	//加速度计测量的重力方向(机体坐标系)
	norm = invSqrt(ax*ax + ay+ay + az*az);
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;
	
	//四元数推出的实际重力方向(机体坐标系)
	vx = 2*(q1q3 - q0q2);												
  	vy = 2*(q0q1 + q2q3);
  	vz = q0q0 - q1q1 - q2q2 + q3q3;
	
	//叉积误差
	ex = (ay*vz - az*vy);
	ey = (az*vx - ax*vz);
	ez = (ax*vy - ay*vx);
	
	//叉积误差积分为角速度
	exInt = exInt + ex * Ki;
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;
	
	//角速度补偿
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;
	
	//更新四元数
  	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;	
	
	//单位化四元数
  	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  	q0 = q0 * norm;
  	q1 = q1 * norm;
  	q2 = q2 * norm;  
  	q3 = q3 * norm;
	
	//四元数反解欧拉角
	Yaw = atan2(2.f * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3)* 57.3f;
	Pitch = -asin(2.f * (q1q3 - q0q2))* 57.3f;
	Roll = atan2(2.f * q2q3 + 2.f * q0q1, q0q0 - q1q1 - q2q2 + q3q3)* 57.3f;
}
*/