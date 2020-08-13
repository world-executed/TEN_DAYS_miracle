/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file       		main
* @company	   		�ɶ���ɿƼ����޹�˾
* @author     		��ɿƼ�(QQ3184284598)
* @version    		�鿴doc��version�ļ� �汾˵��
* @Software 		IAR 8.3 or MDK 5.24
* @Target core		NXP RT1064DVL6A
* @Taobao   		https://seekfree.taobao.com/
* @date       		2019-04-30
********************************************************************************************************************/


//�����Ƽ�IO�鿴Projecct�ļ����µ�TXT�ı�



//���µĹ��̻��߹����ƶ���λ�����ִ�����²���
//��һ�� �ر��������д򿪵��ļ�
//�ڶ��� project  clean  �ȴ��·�����������

//���ش���ǰ������Լ�ʹ�õ��������ڹ���������������Ϊ�Լ���ʹ�õ�

#include "headfile.h"



void signal();
int normal = 1800;
int mode9num = 0;



uint8 value=130;


int16 acc[3];
int16 gyro[3];



int main(void)
{
  DisableGlobalIRQ();
  board_init();//��ر��������������ڳ�ʼ��MPU ʱ�� ���Դ���
  
  //�˴���д�û�����(���磺�����ʼ�������)
  
  AllInit();
  while (1)
  {
	  
    OLED_switch();
	
  }
}










void modeSelect()
{
	
	if(roadMode == 1)
	{
		setLeftSpeed_L = SetLeftSpeed;
		setRightSpeed_L = SetRightSpeed;
	}
	
	if(roadMode == 9)             //����תֱ��
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
	
	if(roadMode == 1 && abs(ADCLL - ADCRR) > 800)                //ֱ������
	{
		roadMode = 6;
		//beep = 1;
		dirpid.p = sp;
		setLeftSpeed_L = 1.0 * SetLeftSpeed;
		setRightSpeed_L = 1.0 * SetRightSpeed;
	}
	
	if((roadMode == 6) /*&& (abs(ADCLL - normal)<300) && (abs(ADCRR - normal)<300)*/)       //��������
	{
		if(((ADCL0 - ADCR0)/ (ADCL - ADCR))< 0)		//�����ߵ�
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



/***************************************************��Ҫ��************************************************/

/*
float acc_ratio = 1.6;      //���ٶȼƱ���    
float gyro_ratio = 4.08;    //�����Ǳ���    
  

//----------------------------------------------------------------    
//  @brief      һ�׻����˲�    
//  @param      angle_m     ���ٶȼ�����    
//  @param      gyro_m      ����������    
//  @return     float       �����ںϺ�ĽǶ�    
//----------------------------------------------------------------    
float angle_calc(float angle_m, float gyro_m)    
{    
  float temp_angle;               
  float gyro_now;    
  float error_angle;    
  
  static float last_angle;    
  static uint8 first_angle;    
  
  if(!first_angle)//�ж��Ƿ�Ϊ��һ�����б�����    
  {    
    //����ǵ�һ�����У����ϴνǶ�ֵ����Ϊ����ٶ�ֵһ��    
    first_angle = 1;    
    last_angle = angle_m;    
  }    
  
  gyro_now = gyro_m * gyro_ratio;    
  
  //���ݲ������ļ��ٶ�ֵת��Ϊ�Ƕ�֮�����ϴεĽǶ�ֵ��ƫ��    
  error_angle = (angle_m - last_angle)*acc_ratio;  
  
  //����ƫ���������ǲ����õ��ĽǶ�ֵ���㵱ǰ�Ƕ�ֵ    
  temp_angle = last_angle + (error_angle + gyro_now)*0.005;   
  
  //���浱ǰ�Ƕ�ֵ    
  last_angle = temp_angle;  
  
  return temp_angle;    
}
*/

void MPU6050()
{
    get_accdata();	//��ȡ���ٶȼ�����
    get_gyro();		//��ȡ����������
    acc[0]=mpu_acc_x-acc_offset[0];
    acc[1]=mpu_acc_y-acc_offset[1];
    acc[2]=mpu_acc_z-acc_offset[2];
    gyro[0]=mpu_gyro_x-gyro_offset[0];
    gyro[1]=mpu_gyro_y-gyro_offset[1];
    gyro[2]=mpu_gyro_z-gyro_offset[2];
    gyro_x_i+=gyro[0];
    angle=(int)(gyro_x_i*360.0/700000.0);
    gyro_y_i+=gyro[1];
}

/*
const float fRad2Deg = 57.295779513f; //���Ȼ���Ƕȳ˵�ϵ��
const float dt = 0.005; //ʱ������
float angle[3] = {0};
float R = 0.98f;

void ImuCalculate_Complementary(void)//����Ƕ�
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
  get_accdata();	//��ȡ���ٶȼ�����
  get_gyro();		//��ȡ����������
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
      hist[p[i]]++; //ͳ��ֱ��ͼ
    }
  }
}


uint8 GetOSTUThreshold(int* HistGram)
{
  int X, Y, Amount = 0;
  int PixelBack = 0, PixelFore = 0, PixelIntegralBack = 0, PixelIntegralFore = 0, PixelIntegral = 0;
  double OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma;              // ��䷽��;
  int MinValue, MaxValue;
  int Threshold = 0;
  
  for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;
  for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ;
  if (MaxValue == MinValue) return MaxValue;          // ͼ����ֻ��һ����ɫ             
  if (MinValue + 1 == MaxValue) return MinValue;      // ͼ����ֻ�ж�����ɫ
  
  for (Y = MinValue; Y <= MaxValue; Y++) Amount += HistGram[Y];        //  ��������
  
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
  
  if (MaxValue == MinValue) return MaxValue;          // ͼ����ֻ��һ����ɫ             
  if (MinValue + 1 == MaxValue) return MinValue;      // ͼ����ֻ�ж�����ɫ
  
  Threshold = MinValue;
  NewThreshold = (MaxValue + MinValue) >> 1;
  while (Threshold != NewThreshold)    // ��ǰ�����ε����Ļ����ֵ��ͬʱ����������    
  {
    SumOne = 0; SumIntegralOne = 0;
    SumTwo = 0; SumIntegralTwo = 0;
    Threshold = NewThreshold;
    for (X = MinValue; X <= Threshold; X++)         //������ֵ��ͼ��ָ��Ŀ��ͱ��������֣���������ֵ�ƽ���Ҷ�ֵ      
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
    NewThreshold = (MeanValueOne + MeanValueTwo) >> 1;       //����µ���ֵ
    Iter++;
    if (Iter >= 1000) return -1;
  }
  return Threshold;
}

static uint8 IsDimodal(double* HistGram)       // ���ֱ��ͼ�Ƿ�Ϊ˫���
    {
        // ��ֱ��ͼ�ķ���м�����ֻ�з���λ2��Ϊ˫�� 
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
        double HistGramC[256] = {0};           // ���ھ������⣬һ��Ҫ�ø�����������������ò�����ȷ�Ľ��
        double HistGramCC[256] = {0};          // ���ֵ�Ĺ��̻��ƻ�ǰ������ݣ������Ҫ��������
        for (Y = 0; Y < 256; Y++)
        {
            HistGramC[Y] = HistGram[Y];
            HistGramCC[Y] = HistGram[Y];
        }
        // ͨ���������ֵ��ƽ��ֱ��ͼ
        while (IsDimodal(HistGramCC) == 0)                                                  // �ж��Ƿ��Ѿ���˫���ͼ����      
        {
            HistGramCC[0] = (HistGramC[0] + HistGramC[0] + HistGramC[1]) / 3;                   // ��һ��
            for (Y = 1; Y < 255; Y++)
                HistGramCC[Y] = (HistGramC[Y - 1] + HistGramC[Y] + HistGramC[Y + 1]) / 3;       // �м�ĵ�
            HistGramCC[255] = (HistGramC[254] + HistGramC[255] + HistGramC[255]) / 3;           // ���һ��
            for (uint8 i=0;i<255;i++){HistGramCC[i]=HistGramC[i];}
            Iter++;
            if (Iter >= 10000) return -1;                                                       // �ƺ�ֱ��ͼ�޷�ƽ��Ϊ˫��ģ����ش������
        }
// ��ֵΪ����ֵ��ƽ��ֵ
        int Peak[2] = {0};
        for (Y = 1, Index = 0; Y < 255; Y++)
            if (HistGramCC[Y - 1] < HistGramCC[Y] && HistGramCC[Y + 1] < HistGramCC[Y]) Peak[Index++] = Y - 1;
        return ((Peak[0] + Peak[1]) / 2);
    }


#define Acc_Gain 0.0002440f			//���ٶ�ת����λ(��ʼ�����ٶȼ�����+-4g������mpu6050�����ݼĴ�����16λ�ģ�LSBa = 2*4 / 65535.0)
#define Gyro_Gain 0.0609756f		//���ٶ�ת��Ϊ�Ƕ�(LSBg = 2000*2 / 65535)
#define Gyro_Gr 0.0010641f			//���ٶ�ת���ɻ���(3.1415 / 180 * LSBg)
#define G 9.80665f					// m/s^2

short ax,ay,az;
short gx,gy,gz;


static float invSqrt(float x) 		//���ټ��� 1/Sqrt(x)
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
#define halfT 0.0025f						//�������ڵ�һ�룬��λs

float Yaw,Pitch,Roll;				//��Ҫ�������ļ������Զ�����extern�����ù�
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;		//��Ԫ��
float exInt = 0, eyInt = 0, ezInt = 0;		//������������ۼƻ���


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
	float vx,vy,vz;							//ʵ���������ٶ�
	float ex,ey,ez;							//�����������
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
	
	//���ٶȼƲ�������������(��������ϵ)
	norm = invSqrt(ax*ax + ay+ay + az*az);
	ax = ax * norm;
	ay = ay * norm;
	az = az * norm;
	
	//��Ԫ���Ƴ���ʵ����������(��������ϵ)
	vx = 2*(q1q3 - q0q2);												
  	vy = 2*(q0q1 + q2q3);
  	vz = q0q0 - q1q1 - q2q2 + q3q3;
	
	//������
	ex = (ay*vz - az*vy);
	ey = (az*vx - ax*vz);
	ez = (ax*vy - ay*vx);
	
	//���������Ϊ���ٶ�
	exInt = exInt + ex * Ki;
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;
	
	//���ٶȲ���
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;
	
	//������Ԫ��
  	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;	
	
	//��λ����Ԫ��
  	norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  	q0 = q0 * norm;
  	q1 = q1 * norm;
  	q2 = q2 * norm;  
  	q3 = q3 * norm;
	
	//��Ԫ������ŷ����
	Yaw = atan2(2.f * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3)* 57.3f;
	Pitch = -asin(2.f * (q1q3 - q0q2))* 57.3f;
	Roll = atan2(2.f * q2q3 + 2.f * q0q1, q0q0 - q1q1 - q2q2 + q3q3)* 57.3f;
}
*/