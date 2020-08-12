#include "headfile.h"
//起跑程序请使用CarBegin()函数
//记录程序请使用RecordBegin()函数

uint8 cursor=0;


int page = 0;
//0:主菜单 1:菜单选择 2:全部参数 3:记录模式1 4:记录模式2 5:记录模式3



void MainPage_Show()
{
	oled_p6x8str(0,cursor,"->");
	oled_p6x8str(18,0,">>GO!<<");
	oled_p6x8str(18,1,"v");            oled_printf_int32(30,1,SetLeftSpeed,4);
	oled_p6x8str(18,2,"p");            oled_printf_int32(30,2,(int)(dirpid.p*100),4);
	oled_p6x8str(18,3,"A");            oled_printf_int32(30,3,angle,4);	
	oled_p6x8str(18,4,"er");           oled_printf_int32(30,4,dirpid.err,4);             
	oled_p6x8str(18,5,"sv");           oled_printf_int32(30,5,(int)(servoShould[nowPos]),4);
	oled_p6x8str(18,6,"re");           oled_printf_int32(30,6,relation * 100,4);
	oled_p6x8str(18,7,"*BACK*");
	
	for(int i = 0;i < 8;i++)
		oled_p6x8str(62,i,"1");
	
	oled_p6x8str(72,0,"MODE");            oled_printf_int32(102,0,eleArray[nowEleNum],4);
	oled_p6x8str(72,1,"Servo");           oled_printf_int32(102,1,abs(MPWM),4);     
	oled_p6x8str(72,2,"Error");           oled_printf_int32(102,2,abs(DirError),4);             
	oled_p6x8str(72,3,"Motor");           oled_printf_int32(102,3,PWML / 10,4);
	
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
			SetLeftSpeed-=5;SetRightSpeed-=5;SetLeftSpeed=SetLeftSpeed<0?0:SetLeftSpeed;SetRightSpeed=SetRightSpeed<0?0:SetRightSpeed;oled_printf_int32(30,1,SetLeftSpeed,4);break;
		case 2:
			static_p-=0.01;static_p=static_p<0?0:static_p;oled_printf_int32(30,2,(int)(dirpid.p*100),4);  break;
		case 3:
			dirpid.d-=0.01;dirpid.d=dirpid.d<0?0:dirpid.d;oled_printf_int32(30,3,(int)(dirpid.d*100),4);  break;
		case 6:
			relation-=0.01;relation=relation<0?0:relation;oled_printf_int32(30,3,(int)(relation*100),4);  break;
		case 4:
			sp-=0.01;sp=sp<0?0:sp;oled_printf_int32(30,4,(int)(sp*100),4);break;
		case 5:
			lp-=0.01;lp=lp<0?0:lp;oled_printf_int32(30,5,(int)(lp*100),4);break;
		default:
			break;
		}
	}
	
	else if(key_check(KEY_R) ==  KEY_DOWN)
	{
		switch(cursor)
		{
		case 1:
			SetLeftSpeed+=5;SetRightSpeed+=5;oled_printf_int32(30,1,SetLeftSpeed,4);break;
		case 2:
			static_p+=0.01;oled_printf_int32(30,2,(int)(static_p*100),4);  break;
		case 3:
			dirpid.d+=0.01;oled_printf_int32(30,3,(int)(dirpid.d*100),4);  break;
		case 4:
			sp+=0.01;oled_printf_int32(30,4,(int)(sp*100),4);break;
		case 5:
			lp+=0.01;oled_printf_int32(30,5,(int)(lp*100),4);break;
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
}

void Menu()
{
	Menu_Show();
	if(key_check(KEY_D) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 2,"  ");
		cursor+=1;
		cursor%=5;
		oled_p6x8str(0,cursor + 2,"->");
		while(!key_check(KEY_D));
	}
	
	else if(key_check(KEY_U) ==  KEY_DOWN)
	{
		oled_p6x8str(0,cursor + 2,"  ");
		if(cursor==0)
			cursor=5;
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
	oled_p6x8str(42,2,"Er");           oled_printf_int32(54,2,DirError,4);
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

void Record1()
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
			RecordBegin();recordMode = 1;cursor = 0;break;
		case 7:
			page = 1;cursor = 0;recordMode = 0;oled_fill(0x00);break;
		default:
			break;
		}
		while(!key_check(KEY_A));
	}
}

void Record2_Show()
{
	oled_p6x8str(0,cursor + 6,"->");
	oled_p6x8str(0,0,"--FINISH 1 RECORD--");
	//oled_p6x8str(0,2,"Please put the car in");
	//oled_p6x8str(0,3,"the garage, and then");
	//oled_p6x8str(0,4,"select RECORD button.");
	oled_p6x8str(18,6,">>>CALCULATE<<<");         
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
			ScanCalculate();page = 5;cursor = 0;break;
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
	oled_p6x8str(0,cursor + 6,"->");
	oled_p6x8str(0,0,"--FINISH CALCULATE--");
	//oled_p6x8str(0,2,"Please put the car in");
	//oled_p6x8str(0,3,"the garage, and then");
	//oled_p6x8str(0,4,"select RECORD button.");
	oled_p6x8str(18,6,">>>INMODE<<<");         
	oled_p6x8str(18,7,"    back");         
}

void Record3()
{
	Record3_Show();
	
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
			recordMode = 3;InmodeBegin();cursor = 0;break;
		case 7:
			page = 1;cursor = 0;recordMode = 0;oled_fill(0x00);break;
		default:
			break;
		}
		while(!key_check(KEY_A));
	}
}

void Camera_Show()
{
  
  oled_dis_bmp(64,128,*mt9v03x_csi_image,th);
  oled_p6x8str(0,0,"   ");oled_printf_int32(0,0,th,3);
  oled_p6x8str(0,1,"   ");oled_printf_int32(0,1,jumpnum,3);
  oled_p6x8str(0,2,"   ");oled_printf_int32(0,2,jumpnum_b,3);
  
}

void Camera()
{
	Camera_Show();
	
	if(key_check(KEY_A) ==  KEY_DOWN)
	{
		page = 1;oled_fill(0x00);
		while(!key_check(KEY_A));
	}
    if(key_check(KEY_L) ==  KEY_DOWN)
    {
      th--;th=th<0?0:th;
    }
    if(key_check(KEY_R) ==  KEY_DOWN)
    {
      th++;th=th>255?255:th;
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
	default:
		break;
	}
}