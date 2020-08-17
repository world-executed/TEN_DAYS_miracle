/***********************************************************************************************************************************************************************************************************		
																															
			CarBegin()----------						RecordBegin();																											InmodeBegin();
				+			   +								+																													+
				\			   \								\																			statusShow():							\
			MainPage()	->	Menu()	->	All();					\							Temshow_calculate();																	\
			   + 			  \		->  Record1();	------------------->	Record2();	*>	ScanCalculate();		*>	Temshow_show()	->  waveScan_status()						\	
			   ----------------													+												+			statusAdjust()		->	Record3();	-----	
									->	Camera();								\											   	\					\								\
									->	ServoControl();											\												\			  		V								\
																				\											     ------------ScanCalculate()						\
																				\																									\
																				-----------------------------------------------------------------------------------------------------



**************************************************************************************************************************************************************************************************************/

#include "headfile.h"
//起跑程序请使用CarBegin()函数
//记录程序请使用RecordBegin()函数

uint8 cursor=0;


int page = 0;
//0:主菜单 1:菜单选择 2:全部参数 3:记录模式1 4:记录模式2 5:记录模式3 6:摄像头/灰度检测 7:查看记录值 8:赛道元素识别波形  9:控制舵机

int maxposL = 0;
int minposL = 10000;
int maxposR = 0;
int minposR = 10000;
int maxservo = 0;
int minservo = 10000;
int maxangle = 0;
int minangle = 10000;
int allNum = 0;

int camaraScanMode = 1;
int statusAdjustMode = 0;
int SCflag = 0;

void MainPage_Show()
{
	oled_p6x8str(0,cursor,"->");
	oled_p6x8str(18,0,">>GO!<<");
	oled_p6x8str(18,1,"v");            oled_printf_int32(30,1,SetLeftSpeed,4);
	oled_p6x8str(18,2,"p");            oled_printf_int32(30,2,(int)(dirpid.p* 100),4);
	oled_p6x8str(18,3,"d");            oled_printf_int32(30,3,dirpid.d * 100,4);	
	oled_p6x8str(18,4,"gi");           oled_printf_int32(30,4,gyro_y_i/100,4);             
	oled_p6x8str(18,5,"pK");           oled_printf_int32(30,5,speedK * 100,4);
	oled_p6x8str(18,6,"re");           oled_printf_int32(30,6,relation * 100,4);
	oled_p6x8str(18,7,"*BACK*");
	
	for(int i = 0;i < 8;i++)
		oled_p6x8str(62,i,"!");
	
	oled_p6x8str(72,0,"MODE");            oled_printf_int32(102,0,eleArray[nowEleNum],4);
	oled_p6x8str(72,1,"Servo");           oled_printf_int32(102,1,abs(MPWM),4);     
	oled_p6x8str(72,2,"Error");           oled_printf_int32(102,2,(int)abs(DirError),4);             
	oled_p6x8str(72,3,"hill");           oled_printf_int32(102,3,hillFlag,4);
	
	oled_p6x8str(72,4,"---------");
	
	oled_printf_int32(72,5,ADCLL,4);
	oled_printf_int32(102,5,ADCRR,4);
	oled_printf_int32(72,6,ADC[4],4);
	oled_printf_int32(102,6,ADC[5],4);
	oled_printf_int32(72,7,setLeftSpeed_L,4);
	oled_printf_int32(102,7,setRightSpeed_L,4);
}

void MainPage()
{
	MainPage_Show();
	
	if(key_check(KEY_D) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor,"  ");
		cursor+=1;
		cursor%=8;
		oled_p6x8str(0,cursor,"->");
		while(!key_check(KEY_D));
	}
	
	else if(key_check(KEY_U) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor,"  ");
		if(cursor==0)
			cursor=8;
		cursor-=1;
		oled_p6x8str(0,cursor,"->");
		while(!key_check(KEY_U));
	}
	
	if(key_check(KEY_L) ==  KEY_DOWN)
	{
		switch(cursor)
		{
		case 1:
			SetLeftSpeed-=1;SetRightSpeed-=1;SetLeftSpeed=SetLeftSpeed<0?0:SetLeftSpeed;SetRightSpeed=SetRightSpeed<0?0:SetRightSpeed;oled_printf_int32(30,1,SetLeftSpeed,4);break;
		case 2:
			static_p-=0.01;static_p=static_p<0?0:static_p;oled_printf_int32(30,2,(int)(static_p * 100),4);  break;
		case 3:
			dirpid.d-=0.001;oled_printf_int32(30,3,(int)(dirpid.d*100),4);  break;
		case 6:
			relation-=0.01;relation=relation<0?0:relation;oled_printf_int32(30,3,(int)(relation*100),4);  break;
		case 4:
			sp-=0.01;sp=sp<0?0:sp;oled_printf_int32(30,4,(int)(sp*100),4);break;
		case 5:
			speedK-=0.01;speedK=speedK<0?0:speedK;oled_printf_int32(30,5,(int)(speedK*100),4);break;
		default:
			break;
		}
	}
	
	else if(key_check(KEY_R) ==  KEY_DOWN)
	{
		switch(cursor)
		{
		case 1:
			SetLeftSpeed+=1;SetRightSpeed+=1;oled_printf_int32(30,1,SetLeftSpeed,4);break;
		case 2:
			static_p+=0.01;oled_printf_int32(30,2,(int)(static_p*100),4);  break;
		case 3:
			dirpid.d+=0.001;oled_printf_int32(30,3,(int)(dirpid.d*100),4);  break;
		case 4:
			sp+=0.01;oled_printf_int32(30,4,(int)(sp*100),4);break;
		case 5:
			speedK+=0.01;oled_printf_int32(30,5,(int)(speedK*100),4);break;
		case 6:
			relation+=0.01;relation=relation>1?1:relation;oled_printf_int32(30,3,(int)(relation*100),4);  break;
		default:
			break;
		}
	}
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		switch(cursor)
		{
		case 0:
			CarBegin();cursor = 0;break;
		case 7:
			page = 1;cursor = 0;oled_fill(0x00);break;
		default:
			break;
		}
		while(!key_check(KEY_A));
	}
}

void Menu_Show()
{
	oled_p6x8str(0,cursor + 2,"->");
	oled_p6x8str(0,0,"--------MENU-------");
	oled_p6x8str(18,2," >>>>GO!<<<<");         
	oled_p6x8str(18,3,"1.ALL PARAMETER");         
	oled_p6x8str(18,4,"2.RECORD MODE");         
	oled_p6x8str(18,5,"3.MAIN INTERFACE");
    oled_p6x8str(18,6,"4.CAMERA");
	oled_p6x8str(18,7,"5.SERVO CONTROL");
}

void Menu()
{
	Menu_Show();
	if(key_check(KEY_D) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 2,"  ");
		cursor+=1;
		cursor%=6;
		oled_p6x8str(0,cursor + 2,"->");
		while(!key_check(KEY_D));
	}
	
	else if(key_check(KEY_U) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 2,"  ");
		if(cursor==0)
			cursor=6;
		cursor-=1;
		oled_p6x8str(0,cursor + 2,"->");
		while(!key_check(KEY_U));
	}
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		switch(cursor + 2)
		{
		case 2:
			CarBegin();cursor = 0;break;
		case 3:
			page = 2;cursor = 0;oled_fill(0x00);break;
		case 4:
			page = 3;cursor = 0;oled_fill(0x00);break;
		case 5:
			page = 0;cursor = 0;oled_fill(0x00);break;
        case 6:
			page = 6;cursor = 0;oled_fill(0x00);break;    
		case 7:
			page = 9;cursor = 0;MPWM =0;oled_fill(0x00);break;    	
		default:
			break;
		}
		while(!key_check(KEY_A));
	}
}

void All_Show()
{
	oled_p6x8str(0,0,"----ALL PARAMEETER----"); 
	oled_p6x8str(0,1,"v");            oled_printf_int32(12,1,SetLeftSpeed,4);
	oled_p6x8str(0,2,"p");            oled_printf_int32(12,2,(int)(dirpid.p*100),4);
	oled_p6x8str(0,3,"d");            oled_printf_int32(12,3,(int)(dirpid.d*100),4);	
	oled_p6x8str(0,4,"K");           oled_printf_int32(12,4,errorK,4);             
	oled_p6x8str(0,5,"i");           oled_printf_int32(12,5,(int)(dirpid.i*100),4);
	oled_p6x8str(0,6,"sp");           oled_printf_int32(12,6,(int)(sp*100),4);
	oled_p6x8str(0,7,"lp");           oled_printf_int32(12,7,(int)(lp*100),4);
	
	oled_p6x8str(42,1,"Mo");           oled_printf_int32(54,1,MPWM,4);
	oled_p6x8str(42,2,"Er");           oled_printf_int32(54,2,(int)DirError,4);
	//oled_p6x8str(42,3,"RM");           oled_printf_int32(54,3,roadMode,4);	
	oled_p6x8str(42,4,"ta");           oled_printf_int32(54,4,mpu_gyro_x + 20,4);             
	oled_p6x8str(42,5,"ti");           oled_printf_int32(54,5,gyro_x_i / 100,4);
	oled_p6x8str(42,6,"IL");           oled_printf_int32(54,6,(int)(leftSpeedInt / 10),4);
	oled_p6x8str(42,7,"lR");           oled_printf_int32(54,7,(int)(rightSpeedInt / 10),4);
	
	oled_p6x8str(84,1,"L");            oled_printf_int32(96,1,ADCL,4);
	oled_p6x8str(84,2,"R");            oled_printf_int32(96,2,ADCR,4);
	oled_p6x8str(84,3,"LL");           oled_printf_int32(96,3,ADCLL,4);	
	oled_p6x8str(84,4,"RR");           oled_printf_int32(96,4,ADCRR,4);             
	oled_p6x8str(84,5,"L/");           oled_printf_int32(96,5,ADCLY,4);
	oled_p6x8str(84,6,"R/");           oled_printf_int32(96,6,ADCRY,4);
	oled_p6x8str(84,7,"lp");           oled_printf_int32(96,7,(int)(lp*100),4);
	
}

void All()
{
	All_Show();
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		page = 1;oled_fill(0x00);
		while(!key_check(KEY_A));
	}
}

void Record1_Show()
{
	oled_p6x8str(0,cursor + 6,"->");
	oled_p6x8str(0,0,"-----RECORD MODE-----");
	oled_p6x8str(0,2,"Please put the car in");
	oled_p6x8str(0,3,"the garage, and then");
	oled_p6x8str(0,4,"select RECORD button.");
	oled_p6x8str(18,6,"  >>>RECORD<<<");         
	oled_p6x8str(18,7,"      back");         
}

void Record1_Show_recording()
{
	oled_p6x8str(0,cursor + 6,"->");
	oled_p6x8str(0,0,"-----RECORD MODE-----");

	oled_p6x8str(0,4,"     RECORDING...");
}

void Record1()
{
	if(recording == 0)
	{
		Record1_Show();
		
		if(key_check(KEY_D) ==  KEY_DOWN)
		{
			oled_p6x8str(0,cursor + 6,"  ");
			cursor+=1;
			cursor%=2;
			oled_p6x8str(0,cursor + 6,"->");
			while(!key_check(KEY_D));
		}
		
		else if(key_check(KEY_U) ==  KEY_DOWN)
		{
			oled_p6x8str(0,cursor + 6,"  ");
			if(cursor==0)
				cursor=2;
			cursor-=1;
			oled_p6x8str(0,cursor + 6,"->");
			while(!key_check(KEY_U));
		}
		
		if(key_check(KEY_A) ==  KEY_DOWN)
		{
			switch(cursor + 6)
			{
			case 6:
				recordMode = 1;RecordBegin();cursor = 0;break;
			case 7:
				page = 1;cursor = 0;recordMode = 0;oled_fill(0x00);break;
			default:
				break;
			}
			while(!key_check(KEY_A));
		}
	}
	else if(recording == 1)
	{
		Record1_Show_recording();
	}
}

void Temshow_calculate()
{
	for(int j = 0; j < 10000; j++)
	{
		allNum = j;
		if(posL[j] == -10000)
			break;
	}
	
	for(int i = 0; i < allNum; i++)
	{
		if (posL[i] > maxposL) maxposL = posL[i];	
		if (posL[i] < minposL) minposL = posL[i];
		if (posR[i] > maxposR) maxposR = posR[i];	
		if (posR[i] < minposR) minposR = posR[i];
		if (servo[i] > maxservo) maxservo = servo[i];	
		if (servo[i] < minservo) minservo = servo[i];
		if (angle_int[i] > maxangle) maxangle = angle_int[i];	
		if (angle_int[i] < minangle) minangle = angle_int[i];
	}
	
}

void Record2_Show()
{
	oled_p6x8str(0,cursor + 6,"->");
	oled_p6x8str(0,0,"--FINISH 1 RECORD--");
	//oled_p6x8str(0,2,"Please put the car in");
	//oled_p6x8str(0,3,"the garage, and then");
	//oled_p6x8str(0,4,"select RECORD button.");
	oled_p6x8str(18,6," >>>CALCULATE<<<");         
	oled_p6x8str(18,7,"    back");      
}

void Record2()
{
	Record2_Show();
	
	if(key_check(KEY_D) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 6,"  ");
		cursor+=1;
		cursor%=2;
		oled_p6x8str(0,cursor + 6,"->");
		while(!key_check(KEY_D));
	}
	
	else if(key_check(KEY_U) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 6,"  ");
		if(cursor==0)
			cursor=2;
		cursor-=1;
		oled_p6x8str(0,cursor + 6,"->");
		while(!key_check(KEY_U));
	}
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		switch(cursor + 6)
		{
		case 6:
			/*ScanCalculate();*/Temshow_calculate();ScanCalculate();oled_fill(0x00);page = 7;cursor = 0;break;
		case 7:
			page = 1;cursor = 0;recordMode = 0;oled_fill(0x00);break;
		default:
			break;
		}
		while(!key_check(KEY_A));
	}
}

void Record3_Show()
{
	oled_p6x8str(0,cursor + 2,"->");
	oled_p6x8str(0,0,"--FINISH CALCULATE--");
	oled_p6x8str(18,2,"k");            oled_printf_int32(30,2,speedK * 100,4);
	oled_p6x8str(18,3,"v");            oled_printf_int32(30,3,SetLeftSpeed,4);
	oled_p6x8str(18,4,"p");            oled_printf_int32(30,4,(int)(static_p * 100),4);
	oled_p6x8str(18,5,"re");           oled_printf_int32(30,5,relation * 100,4);
	oled_p6x8str(18,6,">>>INMODE<<<");         
	oled_p6x8str(18,7,"    back");         
}

void Record3()
{
	Record3_Show();
	
	if(key_check(KEY_D) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 2,"  ");
		cursor+=1;
		cursor%=6;
		oled_p6x8str(0,cursor + 2,"->");
		while(!key_check(KEY_D));
	}
	
	else if(key_check(KEY_U) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 2,"  ");
		if(cursor==0)
			cursor=6;
		cursor-=1;
		oled_p6x8str(0,cursor + 2,"->");
		while(!key_check(KEY_U));
	}
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		switch(cursor + 2)
		{
		case 6:
			InmodeBegin();cursor = 0;oled_fill(0x00);break;
		case 7:
			page = 4;cursor = 0;oled_fill(0x00);break;
		default:
			break;
		}
		while(!key_check(KEY_A));
	}
	
	if(key_check(KEY_L) ==  KEY_DOWN)
	{
		switch(cursor+ 2)
		{
		case 2:
			speedK-=0.01;speedK=speedK<0?0:speedK;oled_printf_int32(30,2,speedK * 100,4);break;
		case 3:
			SetLeftSpeed-=5;SetRightSpeed-=5;SetLeftSpeed=SetLeftSpeed<0?0:SetLeftSpeed;SetRightSpeed=SetRightSpeed<0?0:SetRightSpeed;oled_printf_int32(30,3,SetLeftSpeed,4);break;
		case 4:
			static_p-=0.01;static_p=static_p<0?0:static_p;oled_printf_int32(30,4,(int)(dirpid.p*100),4);  break;
		case 5:
			relation-=0.01;relation=relation<0?0:relation;oled_printf_int32(30,5,(int)(relation*100),4);  break;
		default:
			break;
		}
	}
	
	else if(key_check(KEY_R) ==  KEY_DOWN)
	{
		switch(cursor+ 2)
		{
		case 2:
			speedK+=0.01;speedK=speedK>1?1:speedK;oled_printf_int32(30,2,speedK * 100,4);break;
		case 3:
			SetLeftSpeed+=5;SetRightSpeed+=5;oled_printf_int32(30,2,SetLeftSpeed,4);break;
		case 4:
			static_p+=0.01;oled_printf_int32(30,4,(int)(static_p*100),4);  break;
		case 5:
			relation+=0.01;relation=relation>1?1:relation;oled_printf_int32(30,5,(int)(relation*100),4);  break;
		default:
			break;
		}
	}
	
}

void Camera_Show()
{
  
  oled_dis_bmp(64,128,*mt9v03x_csi_image,th);
  oled_p6x8str(0,0,"   ");oled_printf_int32(0,0,th,3);
  oled_p6x8str(0,1,"   ");oled_printf_int32(0,1,jumpnum,3);
}

void Camera()
{
	if(camaraScanMode)
		Camera_Show();
	else
		waveScan_Camara(20);
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		page = 1;oled_fill(0x00);
		while(!key_check(KEY_A));
	}
    if(key_check(KEY_L) ==  KEY_DOWN)
    {
      if(th!=0)th--;
    }
    if(key_check(KEY_R) ==  KEY_DOWN)
    {
      if(th!=255)th++;
    }
	if(key_check(KEY_D) ==  KEY_DOWN)
    {
		while(!key_check(KEY_D));
      camaraScanMode = (camaraScanMode + 1) % 2;
	  
    }
}



void Temshow_show()
{
	oled_p6x8str(0,0,"Lmax");           oled_printf_int32(32,0,maxposL ,4);
	oled_p6x8str(0,1,"Lmin");           oled_printf_int32(32,1,minposL ,4);
	oled_p6x8str(0,2,"Rmax");           oled_printf_int32(32,2,maxposR ,4);	
	oled_p6x8str(0,3,"Rmin");           oled_printf_int32(32,3,minposR ,4);             
	oled_p6x8str(72,0,"Smax");           oled_printf_int32(96,0,maxservo,4);
	oled_p6x8str(72,1,"Smin");           oled_printf_int32(96,1,minservo,4);
	oled_p6x8str(72,2,"Amax");           oled_printf_int32(96,2,maxangle,4);
	oled_p6x8str(72,3,"Amin");           oled_printf_int32(96,3,minangle,4);
	oled_p6x8str(20,4,"Num");           oled_printf_int32(80,4,allNum ,4);
	
	oled_p6x8str(0,cursor + 6,"->");
	oled_p6x8str(18,6,">>>INMODE<<<");         
	oled_p6x8str(18,7,"    back");     
}

void Temshow()
{
	Temshow_show();
	
		if(key_check(KEY_D) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 6,"  ");
		cursor+=1;
		cursor%=2;
		oled_p6x8str(0,cursor + 6,"->");
		while(!key_check(KEY_D));
	}
	
	else if(key_check(KEY_U) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 6,"  ");
		if(cursor==0)
			cursor=2;
		cursor-=1;
		oled_p6x8str(0,cursor + 6,"->");
		while(!key_check(KEY_U));
	}
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		switch(cursor + 6)
		{
		case 6:
			oled_fill(0x00);page = 8;cursor = 0;break;
		case 7:
			page = 1;cursor = 0;recordMode = 0;oled_fill(0x00);break;
		default:
			break;
		}
		while(!key_check(KEY_A));
	}
}


void statusAdjust_show()
{
	oled_p6x8str(0,cursor + 3,"->");
	oled_p6x8str(0,0,"--STATUS ADJUST--");
	oled_p6x8str(18,3,"KH");            oled_printf_int32(30,3,kTestThrH,4);
	oled_p6x8str(18,4,"KL");            oled_printf_int32(30,4,kTestThrL,4);
	oled_p6x8str(18,5,"pe");           	oled_printf_int32(30,5,period,4);
	oled_p6x8str(18,6,">>>CALCULATE<<<");         
	oled_p6x8str(18,7,"  >>>INMODE<<<");       
}



void statusAdjust()
{
	statusAdjust_show();
	
	if(key_check(KEY_D) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 3,"  ");
		cursor+=1;
		cursor%=5;
		oled_p6x8str(0,cursor + 3,"->");
		while(!key_check(KEY_D));
	}
	
	else if(key_check(KEY_U) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 3,"  ");
		if(cursor==0)
			cursor=5;
		cursor-=1;
		oled_p6x8str(0,cursor + 3,"->");
		while(!key_check(KEY_U));
	}
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		switch(cursor + 3)
		{
		case 6:
			statusZero();ScanCalculate();statusAdjustMode = 0;oled_fill(0x00);page = 7;cursor = 0;break;
		case 7:
			page = 5;cursor = 0;oled_fill(0x00);break;
		default:
			break;
		}
		while(!key_check(KEY_A));
	}
	
	if(key_check(KEY_L) ==  KEY_DOWN)
	{
		switch(cursor + 3)
		{
		case 3:
			kTestThrH-=1;kTestThrH=kTestThrH<0?0:kTestThrH;oled_printf_int32(30,3,kTestThrH,4);break;
		case 4:
			kTestThrL-=1;kTestThrL=kTestThrL<0?0:kTestThrL;oled_printf_int32(30,4,kTestThrL,4);  break;
		case 5:
			period-=1;period=period<0?0:period;oled_printf_int32(30,5,period,4);  break;
		default:
			break;
		}
	}
	
	else if(key_check(KEY_R) ==  KEY_DOWN)
	{
		switch(cursor + 3)
		{
		case 3:
			kTestThrH+=1;kTestThrH+=1;oled_printf_int32(30,3,kTestThrH,4);break;
		case 4:
			kTestThrL+=1;oled_printf_int32(30,4,kTestThrL,4);  break;
		case 5:
			period+=1;oled_printf_int32(30,5,period,4);  break;
		default:
			break;
		}
	}
	
}


void statusShow()
{
	if(!statusAdjustMode)
		waveScan_status();
	else
		statusAdjust();
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		oled_fill(0x00);
		statusAdjustMode = 1;
	}
}

void ServoControl_show()
{
	oled_p6x8str(0,0,"--SERVO CONTROL--");
	oled_p6x8str(18,3,"SERVO");            oled_printf_int32(80,3,MPWM,4);
	oled_p6x8str(18,7,"  >>>BACK<<<");   
}

void ServoControl()
{
	SCflag = 1;
	ServoControl_show();
	
	MPWM = (int)range(MPWM,-90,90);
	pwm_duty(PWM4_MODULE2_CHA_C30, MPWM+SERVO_MID);
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		SCflag = 0;oled_fill(0x00);page = 1;
	}
	
	if(key_check(KEY_L) ==  KEY_DOWN)
	{
		MPWM -= 1;
	}
	
	else if(key_check(KEY_R) ==  KEY_DOWN)
	{
		MPWM += 1;
	}
}

void OLED_switch()
{
	switch(page)
	{
	case 0: MainPage();break;
	case 1: Menu();break;
	case 2: All();break;
	case 3: Record1();break;
	case 4: Record2();break;
	case 5: Record3();break;
    case 6: Camera();break;
	case 7: Temshow();break;
	case 8: statusShow();break;
	case 9: ServoControl();break;
	default:
		break;
	}
}