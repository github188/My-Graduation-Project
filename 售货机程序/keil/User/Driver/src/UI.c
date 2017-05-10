void TFT_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{                    
  unsigned long n;

	n=(unsigned long)(yend-ysta+1)*(xend-xsta+1);

	LCD_WR_REG(0x002A);	
	LCD_WR_Data(xsta>>8);	    
	LCD_WR_Data(xsta&0x00ff);
	LCD_WR_Data(xend>>8);	    
	LCD_WR_Data(xend&0x00ff);
  LCD_WR_REG(0x002b);	
	LCD_WR_Data(ysta>>8);	    
	LCD_WR_Data(ysta&0x00ff);
	LCD_WR_Data(yend>>8);	    
	LCD_WR_Data(yend&0x00ff);
	LCD_WR_REG(0x002c);
	while(n--)LCD_WR_Data(color);//???????? 
} 
void Dis_Lcd(void)
{
LcdDrawLine(10,10,780,470,0xF800);
LcdDrawRectangle(0,35,799,37,0x0000,1);	
	
}

void Dis_Scr(unsigned char pgNum)
{
	unsigned int i=0;
	unsigned char dx;
	unsigned char tx;
	unsigned char ti;
	char tDisBuf[10];
	__disable_irq();
	LCD_Init();
	Frm_Init();
	dx=10;
	flgManCon=0;
	switch(pgNum)
	{
		case FRM_YZA_MAIN:
			LcdDrawRectangle(0,35,799,37,0x0000,1);	
			lcd_text24( 50,70,0,0,"µ±Ç°Ñ¹Á¦"); ShowStr30(180, 70,"  0.0",5,0,0xFFFF);Draw_Button(170,67,100,30);  ShowStr30(275, 70,"MPa",3,0,0xFFFF);//lcd_text24(275,70,0,0,"MPa");	
			lcd_text24( 50,116,0,0,"????");ShowStr30(180,116," 10.0",5,0,0xFFFF);Draw_Button(170,113,100,30);ShowStr30(275,116,"s",3,0,0xFFFF);//lcd_text24(275,116,0,0,"S");
			TFT_Fill(0,175,799,177,0x0000);			TFT_Fill(400,35,402,175,0x0000);				TFT_Fill(400,75,799,77,0x0000);	//????
			lcd_text24(410,43,0,0,"????"); ShowStr30(518,43,nowFileName,12,0,0xFFFF);
			lcd_text24(410,83,0,0,"????"); ShowStr30(518,83,"100.0 MPa",10,0,0xFFFF);//lcd_text24(518,83,0,0,"100.0MPa");
			lcd_text24(410,113,0,0,"????");ShowStr30(518,113,"1.5 s",10,0,0xFFFF);//lcd_text24(518,113,0,0,"1.5S");
			lcd_text24(410,143,0,0,"????"); ShowStr30(518,143,"12345",5,0,0xFFFF);//lcd_text24(518,143,0,0,"12345");
			Draw_Line(30, 190,26, 210,1,BLACK);
			Draw_Line(30, 190,34, 210,1,BLACK);
			Draw_Line(30, 190,30, 420,1,BLACK);//??
			Draw_Line(785,400,765,396,1,BLACK);
			Draw_Line(785,400,765,404,1,BLACK);
			Draw_Line(785,400, 10,400,1,BLACK);//??
			//lcd_text16( 87+122,200,0,0,"??:???????????,???????.");	
			Dis_Sav_Pam();

		break;
		case FRM_YZA_SET:
			TFT_Fill(0,35,799,37,0x0000);	
			lcd_text24( 60, 83,0,0,"????");Draw_TextBox(408,75,284,40);//
			lcd_text24( 60,163,0,0,"????");Draw_TextBox(408,155,212,40);//
			lcd_text24( 60,243,0,0,"????");Draw_TextBox(408,235,212,40);//
			lcd_text24( 60,323,0,0,"????");Draw_TextBox(408,315,212,40);//
			ShowStr30(180,163,"[20-350]",8,0,0xFFFF);	
			ShowStr30(180,243,"[1-15]",6,0,0xFFFF);	
			ShowStr30(180,323,"[   ]",6,0,0xFFFF);	
			lcd_text24(180+16,323,0,0,"??");	
			ShowStr30(634,163,"MPa",3,0,0xFFFF);	
			ShowStr30(634,243,"s",1,0,0xFFFF);	
			lcd_text24(634,323,0,0,"?");	
			//ShowStr30(410,155,"1234.5",6,0,0xffff);
			KEY_YZA_SET();
			Dis_YZA_SET();
		break;
		case FRM_YZA_LST:
			for(i=0;i<16;i++)
				Draw_Line(10, 50+24*i,790, 50+24*i,1,BLACK);
			Draw_Line( 10, 50, 10, 410,1,BLACK);
			lcd_text16( 18,54,0,0,"??");	
			Draw_Line( 80, 50, 80, 410,1,BLACK);
			lcd_text16( 88,54,0,0,"???");	
			Draw_Line(216, 50,216, 410,1,BLACK);
			lcd_text16(224,54,0,0,"????");	
			Draw_Line(296, 50,296, 410,1,BLACK);
			lcd_text16(304,54,0,0,"????");	
			Draw_Line(376, 50,376, 410,1,BLACK);
			lcd_text16(384,54,0,0,"????");	
			Draw_Line(456, 50,456, 410,1,BLACK);
			lcd_text16(464,54,0,0,"????");	
			Draw_Line(536, 50,536, 410,1,BLACK);
			lcd_text16(544,54,0,0,"????");	
			Draw_Line(616, 50,616, 410,1,BLACK);
			Draw_Line(696, 50,696, 410,1,BLACK);
			lcd_text16(624,54,0,0,"????");	
			Draw_Line(790, 50,790, 410,1,BLACK);
			lcd_text16(704,54,0,0,"????");	
			//lcd_text16(384,54,0,0,"????");	
			//lcd_text16(384,54,0,0,"????");	

		break;
		case FRM_YZA_PWR:
		break;
		case FRM_YZA_BTN:
		break;
		case FRM_YZA_SYS:
		//??????,????,????,???? ???? ???? ????
			lcd_text16(18,54,0,0,"??????");	Draw_TextBox(408,75,284,40);
			lcd_text16(50,54+1*30,0,0,"????");	
			lcd_text16(50,54+2*30,0,0,"????");	
			lcd_text16(50,54+3*30,0,0,"????");	
			lcd_text16(50,54+4*30,0,0,"????");	
			lcd_text16(50,54+5*30,0,0,"????");	
			lcd_text16(50,54+6*30,0,0,"????");	
			lcd_text16(50,54+7*30,0,0,"????");	
			lcd_text16(50,54+8*30,0,0,"????");	

		break;
		case FRM_YZA_QX1:
		break;
		case FRM_YZA_QX2:
		break;
		case FRM_YZA_JOB:
		break;
		case FRM_YZA_TEST:
		break;
		case FRM_LOGIN:			break;
		case FRM_PWD:				break;
		case FRM_MON:				break;
		case FRM_MAIN: 			break;
		case FRM_SD: 				break;
		case FRM_PENDLST:		break;
		case FRM_DONELST:		break;
		case FRM_WARN:	 		break;
		case FRM_CONFIG:
			TFT_Fill(0,50,799,411,0xffff);	
			DisConfSel(0,0,RED);
			Draw_Cap(40,55+ 0*32,64,30,"????",4);
			Draw_Cap(40,55+ 1*32,64,30,"????",4);
			Draw_Cap(40,55+ 2*32,64,30,"????",4);
			Draw_Cap(40,55+ 3*32,64,30,"???",4);
			Draw_Cap(40,55+ 4*32,64,30,"????",4);
			Draw_Cap(40,55+ 5*32,64,30,"????",4);
			Draw_Cap(40,55+ 6*32,64,30,"????",4);
			Draw_Cap(40,55+ 7*32,64,30,"????",4);
			Draw_Cap(40,55+ 8*32,64,30,"????",4);
			Draw_Cap(40,55+ 9*32,64,30,"????",4);
			//Draw_Cap(40,55+10*32,64,30,"????",4);

			lcd_text16(110,62+ 0*32,0,0,"( 1 - 15 )");
			lcd_text16(110,62+ 1*32,0,0,"( 5 - 59 )");
			lcd_text16(110,62+ 2*32,0,0,"(90 - 110)");
			lcd_text16(110,62+ 3*32,0,0,"( 0 - 99 )");
			lcd_text16(110,62+ 4*32,0,0,"( 0 - 20 )");
			lcd_text16(110,62+ 5*32,0,0,"( 0 - 20 )");
			lcd_text16(110,62+ 6*32,0,0,"( 0 - 20 )");
			lcd_text16(110,62+ 7*32,0,0,"( 0 - 20 )");
			lcd_text16(110,62+ 8*32,0,0,"( 0 - 20 )");
			lcd_text16(110,62+ 9*32,0,0,"( 0 - 999)");
			//lcd_text16(110,62+10*32,0,0,"( HHMMSS )");

			Draw_TextBox(200,58+ 0*32,60,22);
			Draw_TextBox(200,58+ 1*32,60,22);
			Draw_TextBox(200,58+ 2*32,60,22);
			Draw_TextBox(200,58+ 3*32,60,22);
			Draw_TextBox(200,58+ 4*32,60,22);
			Draw_TextBox(200,58+ 5*32,60,22);
			Draw_TextBox(200,58+ 6*32,60,22);
			Draw_TextBox(200,58+ 7*32,60,22);
			Draw_TextBox(200,58+ 8*32,60,22);
			Draw_TextBox(200,58+ 9*32,60,22);
			//Draw_TextBox(200,58+10*32,60,22);
			lcd_text16(265,62+ 2*32,BLUE,0xFFFF,"x0.01");
			lcd_text16(265,62+ 3*32,BLUE,0xFFFF,"Kg");
			lcd_text16(265,62+ 4*32,BLUE,0xFFFF,"x0.1Mpa");
			lcd_text16(265,62+ 5*32,BLUE,0xFFFF,"x0.1Mpa");
			lcd_text16(265,62+ 6*32,BLUE,0xFFFF,"x0.1Mpa");
			lcd_text16(265,62+ 7*32,BLUE,0xFFFF,"x0.1g/cm{");
			lcd_text16(265,62+ 8*32,BLUE,0xFFFF,"x0.1g/cm{");
			lcd_text16(265,62+ 9*32,BLUE,0xFFFF,"x0.1Kg/s");

			Draw_Line(340, 50,340,370 ,1,BLACK);

			Draw_Cap(352,55+ 0*32,64,30,"???1",4);
			Draw_Cap(352,55+ 1*32,64,30,"???1",4);
			Draw_Cap(352,55+ 2*32,64,30,"???2",4);
			Draw_Cap(352,55+ 3*32,64,30,"???2",4);
			Draw_Cap(352,55+ 4*32,64,30,"????",4);
			Draw_Cap(352,55+ 5*32,64,30,"??",4);
			Draw_Cap(352,55+ 6*32,64,30,"??",4);
			lcd_text16(416,62+ 0*32,0,0,"(01-99)");
			lcd_text16(416,62+ 1*32,0,0,"(10-99)");
			lcd_text16(416,62+ 2*32,0,0,"(01-99)");
			lcd_text16(416,62+ 3*32,0,0,"(10-99)");
			lcd_text16(416,62+ 4*32,0,0,"(0 -99)");
			lcd_text16(400,62+ 5*32,0,0,"(YYMMDD)");
			lcd_text16(400,62+ 6*32,0,0,"(HHMMSS)");
			Draw_TextBox(480,58+ 0*32,62,22);
			Draw_TextBox(480,58+ 1*32,62,22);
			Draw_TextBox(480,58+ 2*32,62,22);
			Draw_TextBox(480,58+ 3*32,62,22);
			Draw_TextBox(480,58+ 4*32,62,22);
			Draw_TextBox(480,58+ 5*32,62,22);
			Draw_TextBox(480,58+ 6*32,62,22);

			Draw_Cap(352,55+ 7*32,64,30,"?    ?",4);
			Draw_Cap(352,55+ 8*32,64,30,"????",4);
			lcd_text16(416,62+ 7*32,0,0,"(xxxxx)");
			lcd_text16(416,62+ 8*32,0,0,"(xxxx )");
			Draw_TextBox(480,58+ 7*32,62,22);
			Draw_TextBox(480,58+ 8*32,62,22);

			Draw_Cap(570, 55+0*32,64,30,"?  ?",5);//WarnOnOff[0]:flgAlert[0]
			Draw_Cap(570, 55+1*32,64,30,"?  ?",5);//WarnOnOff[1]:flgAlert[1]
			Draw_Cap(570, 55+2*32,64,30,"???",5);//WarnOnOff[2]:flgAlert[2]
			Draw_Cap(570, 55+3*32,64,30,"???",5);//WarnOnOff[3]:flgAlert[3]
			Draw_Cap(570, 55+4*32,64,30,"?  ?",5);//WarnOnOff[4]:flgAlert[4]
			Draw_Cap(570, 55+5*32,64,30,"?  ?",5);//WarnOnOff[5]:flgAlert[5]
			Draw_Cap(570, 55+6*32,64,30,"???",5);//WarnOnOff[6]:flgAlert[7]
			Draw_Cap(570, 55+7*32,64,30,"???",5);//WarnOnOff[7]:flgAlert[8]
			Draw_Cap(570, 55+8*32,64,30," A M S ",5);//WarnOnOff[8]:flgAlert[9]
			Draw_Cap(570, 55+9*32,64,30,"?  ?",5);//WarnOnOff[9]
			Draw_Cap(570,46+10*32,64,30,"?  ?",5);
			Draw_Cap(570,64+10*32,64,30,"?  ?",5);
			Draw_Line(562,69+10*32,610,69+10*32,1,BLACK);


			Draw_Button_Cap(620,58+0*32,42,22," ",1);
			Draw_Button_Cap(620,58+1*32,42,22," ",1);
			Draw_Button_Cap(620,58+2*32,42,22," ",1);
			Draw_Button_Cap(620,58+3*32,42,22," ",1);
			Draw_Button_Cap(620,58+4*32,42,22," ",1);
			Draw_Button_Cap(620,58+5*32,42,22," ",1);
			Draw_Button_Cap(620,58+6*32,42,22," ",1);
			Draw_Button_Cap(620,58+7*32,42,22," ",1);
			Draw_Button_Cap(620,58+8*32,42,22," ",1);
			Draw_Button_Cap(620,58+9*32,42,22," ",1);
			Draw_Button_Cap(620,58+10*32,42,22," ",1);

			lcd_text16(620+17,62+0*32,0,0,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+1*32,0,0,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+2*32,0,0,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+3*32,0,0,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+4*32,0,0,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+5*32,0,0,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+6*32,0,0,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+7*32,0,0,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+8*32,0,0,"Y");	 //17=(42-8)/2
			lcd_text16(620+17,62+9*32,0,0,"Y");	 //17=(42-8)/2
			lcd_text16(620+ 5,62+10*32,0,0,"??");	 //17=(42-8)/2

			Draw_Button_Cap(700,58+0*35, 80,22,"??",2);
			Draw_Button_Cap(700,58+1*35, 80,22,"??",2);
			Draw_Button_Cap(700,58+2*35, 80,22,"??",2);
			Dis_Conf();
			ConfIndex=0;
			ConfPos=0;
			DisConfSel(ConfIndex,ConfPos,RED);

		break;
		//??
		case FRM_AMS:
			TFT_Fill(33,50,799,411,0xffff);	
			Draw_Line( 0, 50,799, 50,3,BLACK);
			Draw_Line( 0,412,799,412,3,BLACK);
  		TFT_Fill(108,156,204,180,0);
			TFT_Fill(108,156+50,204,180+50,0);
			TFT_Fill(108,156+100,204,180+100,0);
			TFT_Fill(108,156+150,204,180+150,0);
			TFT_Fill(324,156,420,180,0);
			TFT_Fill(324,156+50,420,180+50,0);
			TFT_Fill(324,156+100,420,180+100,0);
			TFT_Fill(324,156+150,420,180+150,0);
			TFT_Fill(572,156,718,180,0);
			TFT_Fill(572,156+50,718,180+50,0);
			TFT_Fill(572,156+100,718,180+100,0);
			//TFT_Fill(572,156+150,668,180+150,0);

			Draw_Line(25, 360,799,360,2,BLACK);
			Draw_Button_Cap(710,60,60,30,"??",2);
			Draw_Button_Cap(710-80,60,60,30,"??",2);
			Draw_Button_Cap(710-160,60,60,30,"??",2);
			Draw_Sel(710-5,60-5,60+10,30+10,RED);
			AmsIndex=2;
		 	Dis_Detail();
		break;
		//case FRM_SET_PLC
		case FRM_SETTING:
			Frm_Fy(1);//Frm_Fy(swVer);
			Dis_Fy();
			DisSetSel(0,0,RED);
			FyIndex=0;FyPos=0;

		break;
		case FRM_MAN:
			TFT_Fill(26,49,799,401,0xffff);	
			lcd_text16(45,55+0*30,0,0,"Di1");
			lcd_text16(45,55+1*30,0,0,"Di2");
			lcd_text16(45,55+2*30,0,0,"Di3");
			lcd_text16(45,55+3*30,0,0,"Di4");
			lcd_text16(45,55+4*30,0,0,"Di5");
			lcd_text16(45,55+5*30,0,0,"Di6");
			lcd_text16(45,55+6*30,0,0,"Di7");
			lcd_text16(45,55+7*30,0,0,"Di8");
			lcd_text16(45,55+8*30,0,0,"Di9");
			lcd_text16(45,55+9*30,0,0,"Di10");
			lcd_text16(45,55+10*30,0,0,"Di11");
			lcd_text16(45,55+11*30,0,0,"Di12");
			for(i=0;i<8;i++)
			  Draw_Lamp(74,55+i*30,0);
			lcd_text16(125,75+0*40,0,0,"Do1");
			lcd_text16(125,75+1*40,0,0,"Do2");
			lcd_text16(125,75+2*40,0,0,"Do3");
			lcd_text16(125,75+3*40,0,0,"Do4");
			lcd_text16(125,75+4*40,0,0,"Do5");
			lcd_text16(125,75+5*40,0,0,"Do6");
			lcd_text16(125,75+6*40,0,0,"Do7");
			lcd_text16(125,75+7*40,0,0,"Do8");
			for(i=0;i<8;i++)
			{ 
			 Draw_Lamp(154,75+i*40,0);
			 Draw_Button(122,72+i*40,50,22);
			 }
			lcd_text16(205,75+0*40,0,0,"Ai1:");
			lcd_text16(205,75+1*40,0,0,"Ai2:");
			lcd_text16(205,75+2*40,0,0,"Ai3:");
			lcd_text16(205,75+3*40,0,0,"Ai4:");

			lcd_text16(240,75+0*40,0,0,"0000");
			lcd_text16(240,75+1*40,0,0,"0000");
			lcd_text16(240,75+2*40,0,0,"0000");
			lcd_text16(240,75+3*40,0,0,"0000");

			lcd_text16(300,75+0*40,0,0,"AO1:");lcd_text16(330,75+0*40,0,0,"0000");
			lcd_text16(300,75+1*40,0,0,"AO2:");lcd_text16(330,75+1*40,0,0,"0000");
			lcd_text16(300,75+2*40,0,0,"Fi1:");lcd_text16(330,75+2*40,0,0,"0000");
			lcd_text16(300,75+3*40,0,0,"Fi2:");lcd_text16(330,75+3*40,0,0,"0000");
			

			lcd_text16(430-50,75+0*40,0,0,"???max");
			lcd_text16(430-50,75+1*40,0,0,"???min");
			lcd_text16(430-50,75+2*40,0,0,"???max");
			lcd_text16(430-50,75+3*40,0,0,"???min");

			Draw_TextBox(460,72+0*40,50,22);
			Draw_TextBox(460,72+1*40,50,22);
			Draw_TextBox(460,72+2*40,50,22);
			Draw_TextBox(460,72+3*40,50,22);
			Draw_Button_Cap(710,72,60,22,"??",2);
			Draw_Button_Cap(710,72+30,60,22,"??",2);
			Draw_Rect(205,235,500,150,2);
			Draw_Rect(545,130,140, 80,2);
			DisManSel(0,0,RED);
			flgManCon=1;
			ManIndex=0;ManCur=0;ManPos=0;
		break;
	}
	__enable_irq();
}

