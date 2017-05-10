#include "KeyPad.h"
#include "systick.h"
#include "GTFont.h"
#include "eep.h"
#include "Global.h"
#include "string.h"



extern void WrSetSel(unsigned char x, unsigned char y, char *s);
extern void LCD_BL(unsigned int bl_Level);
extern void DisConfSel(unsigned char x, unsigned char y, unsigned int tColor);
extern void Draw_Sel(unsigned int tLeft, unsigned int tTop, unsigned int tWidth, unsigned int tHeight, unsigned int tColor);
extern void ShowStr30(unsigned int x, unsigned int y, char *p, unsigned char slen, unsigned int ForeColor, unsigned int BakColor);
extern void Draw_Lamp(unsigned int x, unsigned int y, unsigned char tColor);
extern unsigned long calYFL(void);
extern void Dis_1_Pend(unsigned char IdPend);
extern unsigned char dGetPendNum(unsigned char id);
extern void lcd_text24(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
extern void ConfChr2DatPlz(void);
extern void reNewSetDat(void);
extern void DisPendSel(unsigned char y, unsigned int tColor);
extern void DisSetSel(unsigned char x, unsigned char y, unsigned int tColor);
extern void ClrSetSel(unsigned char x, unsigned char y);
extern unsigned int FyChr2Dat(void);
extern void CalSavMaxVol(void);
extern void funcSavFyDat(void);
extern void Dis_Fy(void);

extern unsigned char EepWrBuf[200];
extern unsigned char EepRdBuf[200];
extern unsigned char disSlaveId;//子站显示ID号
//extern void LcdDisplayStringZH(char * zh, uint16_t x, uint16_t y, uint16_t tForeColor, uint16_t tBackColor,bool tTransparency, GTFont_type tType );
extern void lcd_text16(unsigned short x, unsigned short y, unsigned short fc, unsigned short bc, char *s);
extern void Dis_Num16(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc, uint16_t Dat);
extern void Draw_Frame(unsigned char  x, unsigned char y, unsigned int color);
extern void LcdDrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t tColor, bool tIsfill);
extern unsigned char sysSavDat[10];
extern char disAddrBuf[3];
extern unsigned char AddrIfix, AddrIfix_pre;
extern void LcdDrawCircle(uint16_t X, uint16_t Y, uint16_t R, uint16_t tColor, bool tIsfill);
extern void TFT_Fill(unsigned int xsta, unsigned int ysta, unsigned int xend, unsigned int yend, unsigned int color);
void Key_Pwd(void);
void Key_MON(void);
void Key_Man(void);
void Key_Conf(void);
void getWarnOnOff(void);
void Key_Pend(void);
void Key_Lst(void);

void KeyHardwareConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //sysTickStartSoftTimer(KEY_TICK, 20, true);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_8);

}

uint8_t KeyRead(void)
{
    uint8_t tKeyValue = 0;
    tKeyValue = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);
    tKeyValue <<= 1;
    tKeyValue |= GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6);
    tKeyValue <<= 1;
    tKeyValue |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0);
    tKeyValue <<= 1;
    tKeyValue |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
    tKeyValue <<= 1;
    tKeyValue |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
    tKeyValue <<= 1;
    tKeyValue |= GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
    return tKeyValue;
}
void Key_Log(void)
{
    unsigned char ti;
    if((flgAMT == 1) && (ubKey_Command == KEY_RTN))
    {
        flgLogEdit = 0;
        ubKey_Command = 0;
        flgAMT = 0;
        curPos[0] = 0;
        curPos[1] = 0;
        TFT_Fill(735, 100, 760, 150, WHITE);
        frmNum = FRM_MAIN;
        if(sys_Conn == 0)
            UsrAvid = 1;
        //GetUsrID(tUsrChr);
        flgCur = 0;

    }
    if(textPos == 0) //用户
    {
        if(ubKey_Command == KEY_SET) //设定键按下
        {
            ubKey_Command = 0;
            flgLogEdit = 1;
            Draw_Sel(146, 96, 208, 53, BLUE);
            for(ti = 0; ti < 7; ti++)
            {
                tmplogUsr[0][ti] = ' ';
                tUsr[ti] = tmplogUsr[0][ti];
            }
            curPos[0] = 0;
            tUsr[4] = 0;
            TFT_Fill(162, 106, 162 + 2 * 32, 106 + 32, WHITE);
            ShowStr30(162, 106, tUsr, 4, BLACK, WHITE);
        }
        if(ubKey_Command == KEY_ENTER) //确认
        {
            ubKey_Command = 0;
            flgLogEdit = 0;
            Draw_Sel(146, 96, 208, 53, RED);
            //ti=LogIn();
            for(ti = 0; ti < 4; ti++)
            {
                logUsr[0][ti] = tmplogUsr[0][ti];
                tUsr[ti] = logUsr[0][ti];
            }
            tUsr[4] = 0;
            TFT_Fill(162, 106, 162 + 2 * 32, 106 + 32, WHITE);
            ShowStr30(162, 106, tUsr, 4, BLACK, WHITE);
        }
//		if(KeyBuf==KEY_DEL)	//DEL键按下
        if(ubKey_Command == KEY_DEL)	//DEL键按下
        {
            if(curPos[0] == 4) //四位工员输完
            {
                tmplogUsr[0][3] = ' ';
                curPos[0] = 3;
            }
            else if((curPos[0] < 4) && (curPos[0] > 0))
            {
                tmplogUsr[0][curPos[0] - 1] = ' ';
                curPos[0]--;
            }
            for(ti = 0; ti < 7; ti++)
            {
                tUsr[ti] = tmplogUsr[0][ti];
            }
            tUsr[4] = 0;
            TFT_Fill(162, 106, 162 + 2 * 32, 106 + 32, WHITE);
            ShowStr30(162, 106, tUsr, 4, BLACK, WHITE);
        }
        //Num Key
//		if((flgLogEdit==1)&&(KeyBuf<0x4A)&&(KeyBuf>0x3F))
        if((flgLogEdit == 1) && (ubKey_Command < 0x4A) && (ubKey_Command > 0x3F))
        {
            if(curPos[0] <= 3)
            {
                tmplogUsr[0][curPos[0]] = (ubKey_Command & 0x0F) + 0x30;
                curPos[0]++;
            }
            for(ti = 0; ti < 4; ti++)
            {
                tUsr[ti] = tmplogUsr[0][ti];
            }
            tUsr[4] = 0;
            TFT_Fill(162, 106, 162 + 2 * 32, 106 + 32, WHITE);
            ShowStr30(162, 106, tUsr, 4, BLACK, WHITE);
        }
//		if(KeyBuf==KEY_R_ARROW)
        if(ubKey_Command == KEY_R_ARROW)
        {
            //如果没有按ENTER就切换，
            TFT_Fill(735, 100, 760, 150, WHITE); //清除密码错误指示灯
            flgLogEdit = 0;
            for(ti = 0; ti < 6; ti++)
            {
                tmplogUsr[1][ti] = logUsr[1][ti];
                if((logUsr[1][ti] >= '0') && (logUsr[1][ti] <= '9'))
                    tUsr[ti] = '-';
                else
                    tUsr[ti] = ' ';
            }
            TFT_Fill(512, 106, 512 + 3 * 32, 106 + 32, WHITE);
            ShowStr30(512, 106, tUsr, 6, BLACK, WHITE);
            //for(ti=0;ti<4;ti++)
            //{
            //	tUsr[ti]=logUsr[0][ti];
            //}
            //ShowStr30(162,102,tUsr,4,BLACK,WHITE);
            Draw_Sel(146, 96, 208, 53, WHITE);
            Draw_Sel(496, 96, 208, 53, RED);
            textPos = 1;
        }
    }
    else 						//密码
    {
//		if(KeyBuf==KEY_SET)//设定键按下
        if(ubKey_Command == KEY_SET) //设定键按下
        {
            flgLogEdit = 1;
            Draw_Sel(496, 96, 208, 53, BLUE);
            for(ti = 0; ti < 7; ti++)
            {
                tmplogUsr[1][ti] = ' ';
                tUsr[ti] = tmplogUsr[1][ti];
            }
            tUsr[1] = 0;
            TFT_Fill(512, 106, 512 + 3 * 32, 106 + 32, WHITE);
            ShowStr30(512, 106, tUsr, 6, BLACK, WHITE);
            curPos[1] = 0;
        }
//		if(KeyBuf==KEY_ENTER)//确认
        if(ubKey_Command == KEY_ENTER) //确认
        {
            flgLogEdit = 0;
            ubKey_Command = 0;
            Draw_Sel(496, 96, 208, 53, RED);
            ti = LogIn();
        }
//		if(KeyBuf==KEY_DEL)	//DEL键按下
        if(ubKey_Command == KEY_DEL)	//DEL键按下
        {
            if(curPos[1] == 6) //六位密码输完
            {
                tmplogUsr[1][5] = ' ';
                curPos[1] = 5;
            }
            else if((curPos[1] < 6) && (curPos[1] > 0))
            {
                tmplogUsr[1][curPos[1] - 1] = ' ';
                curPos[1]--;
            }
            for(ti = 0; ti < 7; ti++)
            {
                //tUsr[ti]=tmplogUsr[1][ti];
                if((tmplogUsr[1][ti] >= '0') && (tmplogUsr[1][ti] <= '9'))
                    tUsr[ti] = '-';
                else
                    tUsr[ti] = ' ';
            }
            tUsr[6] = 0;
            TFT_Fill(512, 106, 512 + 3 * 32, 106 + 32, WHITE);
            ShowStr30(512, 106, tUsr, 6, BLACK, WHITE);
        }
        //Num Key

//		if((flgLogEdit==1)&&((KeyBuf&0x0F)<0x0A)&&((KeyBuf&0xF0)==0x40))
        if((flgLogEdit == 1) && ((ubKey_Command & 0x0F) < 0x0A) && ((ubKey_Command & 0xF0) == 0x40))
        {
            if(curPos[1] <= 5)
            {
                tmplogUsr[1][curPos[1]] = (ubKey_Command & 0x0F) + 0x30;
                curPos[1]++;
            }
            for(ti = 0; ti < 6; ti++)
            {
                //tUsr[ti]=tmplogUsr[1][ti];
                if((tmplogUsr[1][ti] >= '0') && (tmplogUsr[1][ti] <= '9'))
                    tUsr[ti] = '-';
                else
                    tUsr[ti] = ' ';
            }
            tUsr[6] = 0;
            TFT_Fill(512, 106, 512 + 3 * 32, 106 + 32, WHITE);
            ShowStr30(512, 106, tUsr, 6, BLACK, WHITE);
        }
//		if(KeyBuf==KEY_L_ARROW)
        if(ubKey_Command == KEY_L_ARROW)
        {
            TFT_Fill(735, 100, 760, 150, WHITE); //清除密码错误指示灯
            //如果没有按ENTER就切换，
            for(ti = 0; ti < 6; ti++)
            {
                //tUsr[ti]=logUsr[1][ti];
                if((logUsr[1][ti] >= '0') && (logUsr[1][ti] <= '9'))
                    tUsr[ti] = '-';
                else
                    tUsr[ti] = ' ';
            }
            TFT_Fill(512, 106, 512 + 3 * 32, 106 + 32, WHITE);
            ShowStr30(512, 106, tUsr, 6, BLACK, WHITE);
            for(ti = 0; ti < 4; ti++)
            {
                tUsr[ti] = logUsr[0][ti];
                tmplogUsr[0][ti] = logUsr[0][ti];
            }
            tUsr[4] = 0;
            TFT_Fill(162, 106, 162 + 2 * 32, 106 + 32, WHITE);
            ShowStr30(162, 106, tUsr, 4, BLACK, WHITE);
            Draw_Sel(146, 96, 208, 53, RED);
            Draw_Sel(496, 96, 208, 53, WHITE);
            textPos = 0;
        }
    }
}

/*
return:
1:用户名输入错误
2:密码输入错误
3:用户名不存在
4:密码不正确
55:超级密码
60+i:正确登陆
*/
unsigned char LogIn(void)
{
    unsigned char ti;
    unsigned char tj;
    unsigned char flgTypeOK;//输入字符合格标志
    unsigned char flgUsrOK;	//用户名存在
    unsigned char flgPwdOK;	//密码存在
    char tUsrChr[5];
    char tPwdChr[7];
    unsigned char tUsrId;
    //unsigned char tPwdId;
    ReadUsrPwd();
    //先判断用户名输入是否正确
    flgTypeOK = 1;
    for(ti = 0; ti < 4; ti++)
    {
        tUsrChr[ti] = tmplogUsr[0][ti];
        if((tUsrChr[ti] < 0x30) || (tUsr[ti] > 0x39)) //输入非法字符
        {
            flgTypeOK = 0;
            Draw_Lamp(740, 124, 1);
            return 1;
        }
    }
    if(flgTypeOK == 1)	//字符有效，存入
    {
        for(ti = 0; ti < 4; ti++)
        {
            logUsr[0][ti] = tmplogUsr[0][ti];
            tUsrChr[ti]   = tmplogUsr[0][ti];
        }
    }
    //再判断密码输入是否正确
    flgTypeOK = 1;
    for(ti = 0; ti < 6; ti++)
    {
        tPwdChr[ti] = tmplogUsr[1][ti];
        if((tPwdChr[ti] < 0x30) || (tPwdChr[ti] > 0x39)) //输入非法字符
        {
            flgTypeOK = 0;
            Draw_Lamp(740, 124, 1);
            return 2;
        }
    }
    if(flgTypeOK == 1)		 	//字符有效，存入
    {
        for(ti = 0; ti < 6; ti++)
        {
            logUsr[1][ti] = tmplogUsr[1][ti];
            tPwdChr[ti]   = tmplogUsr[1][ti];
        }
    }

    tUsrChr[4] = '\0';
    tPwdChr[6] = '\0';
    if(strcmp(tUsrChr, "2536") == 0)	//发油,后门
    {
        if(strcmp(tPwdChr, "292758") == 0)	//
        {
            //clear usr and pwd
            curPos[0] = 0;
            curPos[1] = 0;
            TFT_Fill(735, 100, 760, 150, WHITE);
            //frmNum=FRM_MAIN;//frmNum=FRM_MON;
            frmNum = FRM_MON; //frmNum=FRM_MAIN;
            if(sys_Conn == 0)
                UsrAvid = 1;
            GetUsrID(tUsrChr);
            flgCur = 0;
            u8PendNum[0] = dGetPendNum(0);
            u8PendNum[1] = dGetPendNum(1); //u8PendNum=GetPendNum();
            if(((u8PendNum[0] > 0) || (u8PendNum[1] > 0)) && (sys_Conn == 1)) //if(0)//TRUCK
            {
                Dis_1_Pend(1);//Dis_Id_Pend(1);
                flgPendLst = 1;
            }
            return 99;
        }
    }
    if(strcmp(tUsrChr, "0314") == 0)	//设置,后门
    {
        if(strcmp(tPwdChr, "070536") == 0)	//
        {
            //clear usr and pwd
            curPos[0] = 0;
            curPos[1] = 0;
            TFT_Fill(735, 100, 760, 150, WHITE);
            frmNum = FRM_SETTING;
            GetUsrID(tUsrChr);
            flgCur = 0;
            //flg_EnSend=0;
            //Dis_Scr(frmNum);
            //flg_EnSend=1;
            return 99;
        }
    }
    if(strcmp(tUsrChr, "3647") == 0)	//手动操作
    {
        if(strcmp(tPwdChr, "403869") == 0)
        {
            //clear usr and pwd
            //TFT_Fill(735,100,760,150,WHITE);
            curPos[0] = 0;
            curPos[1] = 0;
            //frmNum=FRM_MAIN;//frmNum=FRM_MON;
            frmNum = FRM_MON; //frmNum=FRM_MAIN;
            //frmNum=FRM_SD;
            GetUsrID(tUsrChr);
            flgCur = 0;
            u8PendNum[0] = dGetPendNum(0);
            u8PendNum[1] = dGetPendNum(1); //u8PendNum=GetPendNum();
            if(((u8PendNum[0] > 0) || (u8PendNum[1] > 0)) && (sys_Conn == 1)) //if(0)//TRUCK
            {
                Dis_1_Pend(1);//Dis_Id_Pend(1);
                flgPendLst = 1;
            }
            return 99;
        }
    }

    if(strcmp(tUsrChr, "4758") == 0)	//测试界面
    {
        if(strcmp(tPwdChr, "513980") == 0)	//超级密码
        {
            //clear usr and pwd
            TFT_Fill(735, 100, 760, 150, WHITE);
            curPos[0] = 0;
            curPos[1] = 0;
            frmNum = FRM_MAN;
            GetUsrID(tUsrChr);
            flgCur = 0;
//		flg_EnSend=0;
// 		Dis_Scr(frmNum);
//		flg_EnSend=1;
            return 99;
        }
    }
    if(strcmp(tUsrChr, "1425") == 0)	//组态设置
    {
        if(strcmp(tPwdChr, "181647") == 0)	//超级密码
        {
            //clear usr and pwd
            TFT_Fill(735, 100, 760, 150, WHITE);
            curPos[0] = 0;
            curPos[1] = 0;
            GetUsrID(tUsrChr);
            frmNum = FRM_CONFIG;
            flgCur = 0;
//		flg_EnSend=0;
// 		Dis_Scr(frmNum);
//		flg_EnSend=1;
            return 99;
        }
    }

    if(strcmp(tUsrChr, "8687") == 0)	//密码设置
    {
        if(strcmp(tPwdChr, "625091") == 0)	//超级密码
        {
            //clear usr and pwd
            TFT_Fill(735, 100, 760, 150, WHITE);
            curPos[0] = 0;
            curPos[1] = 0;
            frmNum = FRM_PWD;
            GetUsrID(tUsrChr);
            flgCur = 0;
            return 99;
        }
    }
//核对用户名
    flgUsrOK = 1;	 //usr is exist;
    tUsrId = 10;
    for(ti = 0; ti < 8; ti++)
    {
        flgUsrOK = 1;
        for(tj = 0; tj < 4; tj++)
        {
            if(flgUsrOK)
            {
                if(tUsrChr[tj] != SavUsr[ti][tj]) flgUsrOK = 0;
            }
        }
        if(flgUsrOK)
        {
            tUsrId = ti;
            ti = 8;
        }
    }
    flgPwdOK = 1;	 //pwd is exist;
//tPwdId=10;
    for(tj = 0; tj < 6; tj++)
    {
        if(flgPwdOK)
        {
            if(tPwdChr[tj] != SavPwd[tUsrId][tj]) flgPwdOK = 0;
        }
    }

    if(tUsrId > 7)
    {
        Draw_Lamp(740, 124, 1);
        return 3;		 //用户名不存在
    }
    if(flgPwdOK == 0)
    {
        Draw_Lamp(740, 124, 1);
        return 4;		//密码不正确
    }

//clear usr and pwd
    for(ti = 0; ti < 6; ti++)
    {
        logUsr[0][ti] = ' ';
        logUsr[1][ti] = ' ';
        tmplogUsr[0][ti] = ' ';
        tmplogUsr[1][ti] = ' ';
    }
    curPos[0] = 0;
    curPos[1] = 0;
    TFT_Fill(735, 100, 760, 150, WHITE);
    GetUsrID(tUsrChr);
    switch(tUsrId)
    {
    case 0:
    case 1:
    case 2:
        if(flgAMT == 0)
        {
            //frmNum=FRM_MAIN;//frmNum=FRM_MON;
            frmNum = FRM_MON; //frmNum=FRM_MAIN;
            flgCur = 0;
            u8PendNum[0] = dGetPendNum(0);
            u8PendNum[1] = dGetPendNum(1); //u8PendNum=GetPendNum();
            if(((u8PendNum[0] > 0) || (u8PendNum[1] > 0)) && (sys_Conn == 1)) //if(0)//TRUCK
            {
                Dis_1_Pend(1);//Dis_Id_Pend(1);
                flgPendLst = 1;
            }
        }
        else
            Draw_Lamp(740, 124, 1); //用户名不存在
        break;
    case 3:
        frmNum = FRM_AMS;
        break;
    case 4://组态参数
        if(flgAMT == 0)
        {
            frmNum = FRM_SETTING;
            flgCur = 0;
        }
        else
            Draw_Lamp(740, 124, 1); //用户名不存在
        break;
    case 5://仪表参数
        if(flgAMT == 0)
        {
            frmNum = FRM_CONFIG;
            flgCur = 0;
        }
        else
            Draw_Lamp(740, 124, 1); //用户名不存在
        break;
    case 6://测试界面
        if(flgAMT == 0)
        {
            frmNum = FRM_MAN;
            flgCur = 0;
        }
        else
            Draw_Lamp(740, 124, 1); //用户名不存在

        break;
    case 7://手动操作
        frmNum = FRM_SD;
        flgAMT = 0;
        flgCur = 0;
        break;
    }
    return (60 + tUsrId);
}
unsigned char ChkUsrCard(void)
{
    unsigned char i, j;
    char tUsrChr[5];
    unsigned char flgUsrOK;
    flgUsrOK = 1;	 //usr is exist;
    CardUsr[5] = 0;
//if((frmNum==FRM_LOGIN)||(frmNum==FRM_MON))
    if(frmNum == FRM_MON)
    {
        for(i = 0; i < 4; i++)
        {
            tUsrChr[i] = CardUsr[i];
            //	LogUsrID[i]=tUsrChr[i];
        }
        tUsrChr[4] = 0;
        if(strcmp(tUsrChr, "7531") == 0) //路号1-7531	 		"1111"
        {   dualChNum = 1; //mbdata[25]=1;
            canRtData[24] = 1;
            frmNum = FRM_MAIN; //FRM_PENDSEL;
            for(i = 0; i < 10; i++)
            {
                idCarTypeId[i] = 0;
                idCarId[i] = 0;
                idCarYFL[i] = 0;
            }
            return 1;
        }
        if(strcmp(tUsrChr, "8642") == 0) //路号2-8642	 		"1212"
        {   dualChNum = 2; //mbdata[25]=2;
            canRtData[24] = 2;
            frmNum = FRM_MAIN; //FRM_PENDSEL;
            for(i = 0; i < 10; i++)
            {
                idCarTypeId[i] = ' ';
                idCarId[i] = ' ';
                idCarYFL[i] = ' ';
            }
            return 2;
        }
    }
    else
    {

        for(i = 0; i < 8; i++)
        {
            flgUsrOK = 1;
            for(j = 0; j < 4; j++)
            {
                if(flgUsrOK)
                {
                    if(CardUsr[j] != SavUsr[i][j]) flgUsrOK = 0;
                }
            }
            if(flgUsrOK)
            {
                for(j = 0; j < 4; j++)
                    LogUsrID[j] = CardUsr[j];
                if(frmNum == FRM_MAIN)
                {
                    UsrAvid = 1;
                    cntRfid = 1;
                }
                else
                {
                    cntRfid = 1;
                    UsrAvid = 0;
                }
                if(frmNum == FRM_LOGIN)
                {
                    switch(i)
                    {
                    case 0:
                        frmNum = FRM_MON;
                        break;
                    case 1:
                        frmNum = FRM_MON;
                        break;
                    case 2:
                        frmNum = FRM_MON;
                        break;
                    case 3:
                        frmNum = FRM_MON;
                        break;
                    case 4:
                        frmNum = FRM_SETTING;
                        break;
                    case 5:
                        frmNum = FRM_CONFIG;
                        break;
                    case 6:
                        frmNum = FRM_MAN;
                        break;
                    case 7:
                        frmNum = FRM_SD;
                        break;
                    }
                }
                return 0x00;
            }
        }
        return 0xFF;
    }
}
void Key_Set_Handler(void)
{
    unsigned char ti = 0;
    unsigned char tj;
    char tFyDis[10];
    unsigned int  tIntDat;
    //unsigned char u8SavFyDat[32];

    if((sys_STA == 0) && (ubKey_Command == KEY_RTN))
    {
        frmNum = FRM_LOGIN;
    }

    if(flgFyEdit == 0)
    {
        //Right Key
        if(ubKey_Command == KEY_R_ARROW) 	//if(KeyBuf==KEY_R_ARROW)
        {
            if(FyIndex < 2)
            {
                DisSetSel(FyIndex, FyPos, WHITE);
                FyIndex++;
                if(FyIndex >= 1)FyPos = 0;
                DisSetSel(FyIndex, FyPos, RED);
            }
        }
        //Left Key
        if(ubKey_Command == KEY_L_ARROW) //if(KeyBuf==KEY_L_ARROW)
        {
            if(FyIndex > 0)
            {
                DisSetSel(FyIndex, FyPos, WHITE);
                FyIndex--;
                if(FyIndex < 2)FyPos = 0;
                DisSetSel(FyIndex, FyPos, RED);
            }
        }
        if(ubKey_Command == KEY_U_ARROW) //if(KeyBuf==KEY_U_ARROW)
        {
            if(FyPos > 0)
            {
                DisSetSel(FyIndex, FyPos, WHITE);
                //Draw_Sel(200-4,58-4+SetPos*35,56+8,22+8,WHITE);
                FyPos--;
                DisSetSel(FyIndex, FyPos, RED);
                //Draw_Sel(200-4,58-4+SetPos*35,56+8,22+8,RED);
            }
        }
        if(ubKey_Command == KEY_D_ARROW) //if(KeyBuf==KEY_D_ARROW)
        {
            if(FyIndex == 0)
            {
                if(swVer == 0)
                    ti = 7; //AO阀8+8个发油参数
                else
                    ti = 8; //AB阀9+8个发油参数
            }
            if(FyIndex == 1) ti = 8;
            if(FyIndex == 2) ti = 3; //3==“AMS”，
            if(FyPos < ti)
            {
                DisSetSel(FyIndex, FyPos, WHITE);
                FyPos++;
                DisSetSel(FyIndex, FyPos, RED);
            }
        }
        //设置界面，非编辑状态按下SET
        if(ubKey_Command == KEY_SET)	//if(KeyBuf==KEY_SET)
        {
            if(FyIndex < 2)
            {
                flgFyEdit = 1;
                if((FyIndex == 1) && (FyPos == 8))
                {
                    for(ti = 0; ti < 7; ti++)
                    {
                        tmpSavMaxVol[ti] = ' ';				 //AO:8 AB:9
                        tFyDis[ti] = tmpSavMaxVol[ti];	 //AO:8 AB:9
                    }
                    tFyDis[7] = '\0';
                    ClrSetSel(FyIndex, FyPos);
                    FyCur = 0;
                    DisSetSel(FyIndex, FyPos, BLUE);
                }
                else
                {
                    for(ti = 0; ti < 3; ti++)
                    {
                        tmpSavFy[FyIndex * 9 + FyPos][ti] = ' ';				 //AO:8 AB:9
                        tFyDis[ti] = tmpSavFy[FyIndex * 9 + FyPos][ti];	 //AO:8 AB:9
                    }
                    tFyDis[3] = '\0';
                    ClrSetSel(FyIndex, FyPos);
                    FyCur = 0;
                    DisSetSel(FyIndex, FyPos, BLUE);
                }
            }
        }
        if(ubKey_Command == KEY_ENTER) //if(KeyBuf==KEY_ENTER)
        {
            ubKey_Command = 0;
            if(FyIndex == 2) //选择命令-->0:保存,1:缺省,2:返回
            {
                if(FyPos == 0) //保存
                {
                    for(ti = 0; ti < 17; ti++) //AO 16,AB 17
                    {
                        for(tj = 0; tj < 4; tj++)
                            u8SavFyBuf[tj] = tmpSavFy[ti][tj];
                        tIntDat = FyChr2Dat();
                        //tSavSetDat=u16SavSetDat;
                        if((tIntDat > SavFyDatMax[ti]) || (tIntDat < SavFyDatMin[ti])) //数据OK!可以保存
                        {
                            SavFyDat[ti] = SavFyDatMin[ti];
                        }
                        else
                        {
                            SavFyDat[ti] = tIntDat;
                        }
                    }
                    //ClrSetSel(1,FyPos);
                    CalSavMaxVol();
                    lcd_text16(454, 62 + 8 * 35, 0, WHITE, tmpSavMaxVol);

                    //EEP_Write(SavFyDat,130,32);
                    //	if(SavFyDat[0]>SavFyDat[1])		//Only AB
                    //		SavFyDat[0]=SavFyDat[1];
                    funcSavFyDat();
                    //SetWarnDat|=0x80;
                    //sys_SavOvKg=SavFyDat[8];			//AO:7 AB:8
                    reNewSetDat();
                    Dis_Fy();
                    bl_LCD = SavFyDat[16];					//AO:15 AB:16
                    LCD_BL(100);//LCD_BL(bl_LCD);
                    //FillFyTxd();
                }
                else if(FyPos == 1)
                {
                    SavFyDat[0] = 1;
                    SavFyDat[1] = 10;
                    SavFyDat[2] = 1;
                    SavFyDat[3] = 10;
                    SavFyDat[4] = 1;
                    SavFyDat[5] = 0;
                    SavFyDat[6] = 100;
                    SavFyDat[7] = 30;
                    SavFyDat[9] = 10;
                    funcSavFyDat();
                    Dis_Fy();
                }
                else if(FyPos == 2)
                {
                    flgCur = 0;
                    for(ti = 0; ti < 6; ti++)
                    {
                        logUsr[0][ti] = ' ';
                        logUsr[1][ti] = ' ';
                    }
                    textPos = 0;
                    curPos[0] = 0;
                    curPos[1] = 0;
                    frmNum = FRM_LOGIN;
                }
                else if(FyPos == 3)
                {
                    frmNum = FRM_AMS;
                    flgAMS = 1;
                }
            }
        }
    }
    else if(flgFyEdit == 1)
    {
        if(FyIndex < 2)
        {
            if((ubKey_Command < 0x4A) && (ubKey_Command > 0x3F)) //if((KeyBuf<0x4A)&&(KeyBuf>0x3F))
            {
                if((FyIndex == 1) && (FyPos == 8))
                {
                    if(FyCur <= 6)
                    {
                        tmpSavMaxVol[FyCur] = (ubKey_Command & 0x0F) + 0x30;
                        FyCur++;
                    }
                    for(ti = 0; ti < 7; ti++)
                    {
                        tFyDis[ti] = tmpSavMaxVol[ti];										//AO:8 AB:9
                    }
                    tFyDis[7] = '\0';

                }
                else
                {
                    if(FyCur <= 2)
                    {
                        tmpSavFy[(FyIndex * 9) + FyPos][FyCur] = (ubKey_Command & 0x0F) + 0x30;	//AO:8 AB:9
                        FyCur++;
                    }
                    for(ti = 0; ti < 3; ti++)
                    {
                        tFyDis[ti] = tmpSavFy[(FyIndex * 9) + FyPos][ti];										//AO:8 AB:9
                    }
                    tFyDis[3] = '\0';
                }
                //TFT_Fill(220,62+SetPos*35,220+24,62+SetPos*35+16,WHITE);
                ClrSetSel(FyIndex, FyPos);
                WrSetSel(FyIndex, FyPos, tFyDis);
                //lcd_text16(220,62+SetPos*35,0,0,tSetDis);
            }
        }
        //编辑状态下回车
        if(ubKey_Command == KEY_ENTER) //if(KeyBuf==KEY_ENTER)
        {
            ubKey_Command = 0;
            flgFyEdit = 0;
            DisSetSel(FyIndex, FyPos, RED);
        }
    }
}
void Dis_WarnLst(void)
{
    unsigned char ti, flgWarnDatOK;
    unsigned char i;
    unsigned char tCode;//,tFunc;//,tLen;
    char tDateTime[20];
//char tBuf[2];
//	SPI2_Flash_Read(FlashWarnBuf,1*4096,160);
    if(dualChNum == 1)
        EepRd(0x6000, 120);
    else
        EepRd(0x6000 + 120, 120);

    for(i = 0; i < 120; i++)
        FlashWarnBuf[i] = EepRdBuf[i];

    lst_WarnCnt = FlashWarnBuf[0];
    {
        if(lst_WarnCnt > 12)
            lst_WarnCnt = 12;
        tDateTime[0] = '2';
        tDateTime[1] = '0';
        tDateTime[4] = '-';
        tDateTime[7] = '-';
        tDateTime[10] = ' ';
        tDateTime[13] = ':';
        tDateTime[16] = ':';
        for(ti = 1; ti <= 12; ti++)
        {
            flgWarnDatOK = 1;
            if((FlashWarnBuf[ti * 10 + 1] == 0) && (FlashWarnBuf[ti * 10 + 2] == 0) && (FlashWarnBuf[ti * 10 + 3] == 0))
                flgWarnDatOK = 0;
            if(FlashWarnBuf[ti * 10 + 1] > 99) flgWarnDatOK = 0;
            if(FlashWarnBuf[ti * 10 + 2] > 12) flgWarnDatOK = 0;
            if(FlashWarnBuf[ti * 10 + 3] > 31) flgWarnDatOK = 0;
            if(FlashWarnBuf[ti * 10 + 4] > 24) flgWarnDatOK = 0;
            if(FlashWarnBuf[ti * 10 + 5] > 59) flgWarnDatOK = 0;
            if(FlashWarnBuf[ti * 10 + 6] > 59) flgWarnDatOK = 0;
            if(flgWarnDatOK)
            {
                tDateTime[2] = '0' + FlashWarnBuf[ti * 10 + 1] / 10;
                tDateTime[3] = '0' + FlashWarnBuf[ti * 10 + 1] % 10;
                tDateTime[5] = '0' + FlashWarnBuf[ti * 10 + 2] / 10;
                tDateTime[6] = '0' + FlashWarnBuf[ti * 10 + 2] % 10;
                tDateTime[8] = '0' + FlashWarnBuf[ti * 10 + 3] / 10;
                tDateTime[9] = '0' + FlashWarnBuf[ti * 10 + 3] % 10;
                tDateTime[11] = '0' + FlashWarnBuf[ti * 10 + 4] / 10;
                tDateTime[12] = '0' + FlashWarnBuf[ti * 10 + 4] % 10;
                tDateTime[14] = '0' + FlashWarnBuf[ti * 10 + 5] / 10;
                tDateTime[15] = '0' + FlashWarnBuf[ti * 10 + 5] % 10;
                tDateTime[17] = '0' + FlashWarnBuf[ti * 10 + 6] / 10;
                tDateTime[18] = '0' + FlashWarnBuf[ti * 10 + 6] % 10;
                tDateTime[19] = '\0';
                lcd_text16(90, 50 + 7 + (ti - 1) * 30, 0, 0, tDateTime);
                tCode = FlashWarnBuf[ti * 10 + 7] & 0x0F;
                //tFunc=FlashWarnBuf[ti*10+7]>>4;
                switch(tCode)
                {
                case 1://flgAlert[0]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "静电报警");
                    //tLen=4;
                    break;
                case 2://flgAlert[1]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "溢流报警");
                    //tLen=4;
                    break;
                case 3://flgAlert[2]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "流量计通讯故障");
                    //tLen=7;
                    break;
                case 4://flgAlert[3]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "无流量报警");
                    //tLen=5;
                    break;
                case 5://flgAlert[4]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "泄漏报警");
                    //tLen=4;
                    break;
                case 6://flgAlert[5]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "密度报警");
                    //tLen=4;
                    break;
                case 7://flgAlert[6]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "用户暂停");
                    //tLen=4;
                    break;
                case 8://flgAlert[7]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "压力过低");
                    //tLen=4;
                    break;
                case 9://flgAlert[8]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "流量过低");
                    //tLen=4;
                    break;
                case 10://flgAlert[9]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "AMS报警");
                    //tLen=4;
                    break;
                case 11://flgAlert[9]
                    lcd_text16(290, 50 + 7 + (ti - 1) * 30, 0, 0, "脉冲量报警");
                    //tLen=4;
                    break;
                }
            }
//tBuf[0]=tFunc;
//if((tBuf[0]!=1)&&(tBuf[0]!=2))
//{		 tBuf[0]=' ';		}
//else
//{   tBuf[0]='0'+tBuf[0];}
//tBuf[1]=0;
//lcd_text16(290+tLen*16,50+7+(ti-1)*30,0,0,tBuf);

        }
    }
}

//按列显示已发记录
void Dis_Lst_Cnt(char *tBuf, unsigned char tNum, unsigned char tCol)
{
    char dBuf[80];
    char tDisChr[30];
    unsigned char i, j;
    unsigned int tk;
    float flDat, flDat1;
    unsigned long tlngDat;//,tlngDat1;//,tlngDat2;
//	unsigned char tPg;
    unsigned int  tx;//横坐标，tx=ti%3;
    for(i = 0; i < 80; i++)
        dBuf[i] = tBuf[i];

    tx = tCol % 3;
    tx = tx * 230;
    if(tCol == 0)
    {
        for(j = 0; j < 3; j++)
        {
            tk = j * 230;
            TFT_Fill(105 + tk, 82 + (0 * 30), 300 + tk, 76 + (1 * 30), WHITE);
            TFT_Fill(105 + tk, 82 + (1 * 30), 300 + tk, 76 + (2 * 30), WHITE);
            TFT_Fill(105 + tk, 82 + (2 * 30), 180 + tk, 76 + (3 * 30), WHITE);
            TFT_Fill(tk + 100 + 115 + 30, 82 + (2 * 30), 315 + tk, 76 + (3 * 30), WHITE);
            TFT_Fill(105 + tk, 82 + (3 * 30), 180 + tk, 76 + (4 * 30), WHITE);
            TFT_Fill(tk + 100 + 115 + 30, 82 + (3 * 30), 315 + tk, 76 + (4 * 30), WHITE);
            TFT_Fill(105 + tk, 82 + (4 * 30), 300 + tk, 76 + (5 * 30), WHITE);
            TFT_Fill(105 + tk, 82 + (5 * 30), 220 + tk, 76 + (6 * 30), WHITE);
            TFT_Fill(tk + 100 + 115, 82 + (5 * 30), 315 + tk, 76 + (6 * 30), WHITE);
            TFT_Fill(105 + tk, 82 + (6 * 30), 220 + tk, 76 + (7 * 30), WHITE);
            TFT_Fill(tk + 100 + 115, 82 + (6 * 30), 315 + tk, 76 + (7 * 30), WHITE);
            TFT_Fill(105 + tk, 82 + (7 * 30), 220 + tk, 76 + (8 * 30), WHITE);
            TFT_Fill(tk + 100 + 115, 82 + (7 * 30), 315 + tk, 76 + (8 * 30), WHITE);
            TFT_Fill(105 + tk, 82 + (8 * 30), 300 + tk, 76 + (9 * 30), WHITE);
            TFT_Fill(105 + tk, 82 + (9 * 30), 300 + tk, 76 + (10 * 30), WHITE);
            //for(i=0;i<9;i++)
            //{	TFT_Fill(105+tk,82+(i*30),300+tk,75+((i+1)*30),WHITE);}//85--97//80--102
            Draw_Cap( tk + 100 + 115 - 35, 80 + 2 * 30, 64, 30, "工号", 2);
            //Draw_Cap( tk+100+115-35, 80+ 3*30,64,30,"铅封号",3);
            Draw_Line(tk + 100 + 115, 80 + 5 * 30, tk + 100 + 115, 80 + 8 * 30, 1, BLACK);
            Draw_Line(tk + 100 + 115 - 30, 80 + 2 * 30, tk + 100 + 115 - 30, 80 + 4 * 30, 1, BLACK);
            Draw_Line(tk + 100 + 115 + 30, 80 + 2 * 30, tk + 100 + 115 + 30, 80 + 3 * 30, 1, BLACK);
        }
    }
    else
    {
        TFT_Fill(105 + tx, 82 + (0 * 30), 300 + tx, 76 + (1 * 30), WHITE);
        TFT_Fill(105 + tx, 82 + (1 * 30), 300 + tx, 76 + (2 * 30), WHITE);
        TFT_Fill(105 + tx, 82 + (2 * 30), 180 + tx, 76 + (3 * 30), WHITE);
        TFT_Fill(tx + 100 + 115 + 30, 82 + (2 * 30), 315 + tx, 76 + (3 * 30), WHITE);
        TFT_Fill(105 + tx, 82 + (3 * 30), 180 + tx, 76 + (4 * 30), WHITE);
        TFT_Fill(tx + 100 + 115 + 30, 82 + (3 * 30), 315 + tx, 76 + (4 * 30), WHITE);
        TFT_Fill(105 + tx, 82 + (4 * 30), 300 + tx, 76 + (5 * 30), WHITE);
        TFT_Fill(105 + tx, 82 + (5 * 30), 220 + tx, 76 + (6 * 30), WHITE);
        TFT_Fill(tx + 100 + 115, 82 + (5 * 30), 315 + tx, 76 + (6 * 30), WHITE);
        TFT_Fill(105 + tx, 82 + (6 * 30), 220 + tx, 76 + (7 * 30), WHITE);
        TFT_Fill(tx + 100 + 115, 82 + (6 * 30), 315 + tx, 76 + (7 * 30), WHITE);
        TFT_Fill(105 + tx, 82 + (7 * 30), 220 + tx, 76 + (8 * 30), WHITE);
        TFT_Fill(tx + 100 + 115, 82 + (7 * 30), 315 + tx, 76 + (8 * 30), WHITE);
        TFT_Fill(105 + tx, 82 + (8 * 30), 300 + tx, 76 + (9 * 30), WHITE);
        TFT_Fill(105 + tx, 82 + (9 * 30), 300 + tx, 76 + (10 * 30), WHITE);
        Draw_Line(330 + 115, 80 + 5 * 30, 330 + 115, 80 + 8 * 30, 1, BLACK);
        Draw_Line(560 + 115, 80 + 5 * 30, 560 + 115, 80 + 8 * 30, 1, BLACK);
        Draw_Line(tx + 100 + 115, 80 + 5 * 30, tx + 100 + 115, 80 + 8 * 30, 1, BLACK);
        Draw_Line(tx + 100 + 115 - 30, 80 + 2 * 30, tx + 100 + 115 - 30, 80 + 4 * 30, 1, BLACK);
        Draw_Line(tx + 100 + 115 + 30, 80 + 2 * 30, tx + 100 + 115 + 30, 80 + 3 * 30, 1, BLACK);
    }
    //数据有效
    //记录号
    tDisChr[0] = ((tNum) / 100) + 0x30;
    tDisChr[1] = (((tNum) % 100) / 10) + 0x30;
    tDisChr[2] = ((tNum) % 10) + 0x30;
    tDisChr[3] = 0;
    for(i = 0; i < 2; i++)
    {
        if(tDisChr[0] == '0')
        {
            for(j = 0; j < 3; j++)
                tDisChr[j] = tDisChr[j + 1];
        }
    }
    lcd_text16(105 + tx, 80 + 6, 0, 0, tDisChr); //序号
    Draw_Button_Cap(140 + tx, 80 + 6, 60, 18, "更新", 2);
    //员工号
    tDisChr[0] = (dBuf[15] >> 4) + 0x30;
    tDisChr[1] = (dBuf[15] & 0x0F) + 0x30;
    tDisChr[2] = (dBuf[16] >> 4) + 0x30;
    tDisChr[3] = (dBuf[16] & 0x0F) + 0x30;
    tDisChr[4] = 0;
    lcd_text16(tx + 250, 80 + 2 * 30 + 6, 0, 0, tDisChr); //员工号
    //路号
    if(dBuf[63] == 1)		{
        tDisChr[0] = '1';
        tDisChr[1] = '3';
    }
    if(dBuf[63] == 2)		{
        tDisChr[0] = '1';
        tDisChr[1] = '4';
    }
    tDisChr[2] = 0;
    lcd_text16(tx + 207, 80 + 1 * 30 + 6, 0, 0, tDisChr); //路号
    //铅封号
    tDisChr[0] = (dBuf[72] >> 4) + 0x30;
    tDisChr[1] = (dBuf[72] & 0x0F) + 0x30;
    tDisChr[2] = (dBuf[73] >> 4) + 0x30;
    tDisChr[3] = (dBuf[73] & 0x0F) + 0x30;
    tDisChr[4] = (dBuf[74] >> 4) + 0x30;
    tDisChr[5] = (dBuf[74] & 0x0F) + 0x30;
    tDisChr[6] = (dBuf[75] >> 4) + 0x30;
    tDisChr[7] = (dBuf[75] & 0x0F) + 0x30;
    tDisChr[8] = 0;
    for(i = 0; i < 8; i++)
    {
        if((tDisChr[i] > 0x39) || (tDisChr[i] < 0x30))
        {
            tDisChr[i] = ' ';
        }
    }
    //lcd_text16(tx+250,80+3*30+6,0,0,tDisChr);//铅封号
    //提油密码
    tDisChr[0] = (dBuf[3] >> 4) + 0x30;
    tDisChr[1] = (dBuf[3] & 0x0F) + 0x30;
    tDisChr[2] = (dBuf[4] >> 4) + 0x30;
    tDisChr[3] = (dBuf[4] & 0x0F) + 0x30;
    tDisChr[4] = (dBuf[5] >> 4) + 0x30;
    tDisChr[5] = (dBuf[5] & 0x0F) + 0x30;
    tDisChr[6] = (dBuf[6] >> 4) + 0x30;
    tDisChr[7] = (dBuf[6] & 0x0F) + 0x30;
    tDisChr[8] = 0;
    lcd_text16(105 + tx, 80 + 2 * 30 + 6, 0, 0, tDisChr); //提油密码
    //车号
    for(i = 0; i < 8; i++)
    {
        if((dBuf[64 + i] >= '0') && (dBuf[64 + i] <= '9'))
        {
            tDisChr[i] = (dBuf[64 + i]);
        }
        else
        {
            tDisChr[i] = ' ';
        }
    }
    tDisChr[8] = 0;
    lcd_text16(105 + tx, 80 + 3 * 30 + 6, 0, 0, tDisChr); //车号
    //应发量
    tDisChr[0] = (dBuf[7] >> 4) + 0x30;
    tDisChr[1] = (dBuf[7] & 0x0F) + 0x30;
    tDisChr[2] = (dBuf[8] >> 4) + 0x30;
    tDisChr[3] = (dBuf[8] & 0x0F) + 0x30;
    tDisChr[4] = (dBuf[9] >> 4) + 0x30;
    tDisChr[5] = (dBuf[9] & 0x0F) + 0x30;
    tDisChr[6] = (dBuf[10] >> 4) + 0x30;
    tDisChr[7] = (dBuf[10] & 0x0F) + 0x30;
    tDisChr[8] = 0;
    for(i = 0; i < 7; i++)
    {
        if(tDisChr[i] == '0')
            tDisChr[i] = ' ';
        else
            break;
    }
    tDisChr[ 8] = 'L';
    tDisChr[ 9] = 0;
    lcd_text16(155 + tx, 80 + 4 * 30 + 6, 0, 0, tDisChr); //应发量
    //实发质量BCD
    tDisChr[0] = (dBuf[11] >> 4) + 0x30;
    tDisChr[1] = (dBuf[11] & 0x0F) + 0x30;
    tDisChr[2] = (dBuf[12] >> 4) + 0x30;
    tDisChr[3] = (dBuf[12] & 0x0F) + 0x30;
    tDisChr[4] = (dBuf[13] >> 4) + 0x30;
    tDisChr[5] = (dBuf[13] & 0x0F) + 0x30;
    tDisChr[6] = (dBuf[14] >> 4) + 0x30;
    tDisChr[7] = '.';
    tDisChr[8] = (dBuf[14] & 0x0F) + 0x30;
    tDisChr[9] = 0;
    for(i = 0; i < 6; i++)
    {
        if(tDisChr[i] == '0')
            tDisChr[i] = ' ';
        else
            break;
    }
    tDisChr[ 9] = 'K';
    tDisChr[10] = 'g';
    tDisChr[11] = 0;
    lcd_text16(121 + tx, 80 + 5 * 30 + 6, 0, 0, tDisChr); //实发量
    //实发体积

    //表前数——质量 	浮点数形式
    flDat = ctof(dBuf[20], dBuf[19], dBuf[18], dBuf[17]);
    tlngDat = flDat * 10;
    //tlngDat1=tlngDat;
    tDisChr[0] = tlngDat / 1000000000;
    tlngDat = tlngDat % 1000000000;
    tDisChr[1] = tlngDat / 100000000;
    tlngDat = tlngDat % 100000000;
    tDisChr[2] = tlngDat / 10000000;
    tlngDat = tlngDat % 10000000;
    tDisChr[3] = tlngDat / 1000000;
    tlngDat = tlngDat % 1000000;
    tDisChr[4] = tlngDat / 100000;
    tlngDat = tlngDat % 100000;
    tDisChr[5] = tlngDat / 10000;
    tlngDat = tlngDat % 10000;
    tDisChr[6] = tlngDat / 1000;
    tlngDat = tlngDat % 1000;
    tDisChr[7] = tlngDat / 100;
    tlngDat = tlngDat % 100;
    tDisChr[8] = tlngDat / 10;
    tlngDat = tlngDat % 10;
    tDisChr[9] = '.';
    tDisChr[10] = tlngDat;
    tDisChr[11] = 0;
    for(i = 0; i < 9; i++)
        tDisChr[i] += '0';
    tDisChr[10] += '0';
    for(i = 0; i < 8; i++)
    {
        if(tDisChr[i] == '0')
            tDisChr[i] = ' ';
        else
            break;
    }
    tDisChr[11] = 'K';
    tDisChr[12] = 'g';
    tDisChr[13] = 0;
    lcd_text16(105 + tx, 80 + 6 * 30 + 6, 0, 0, tDisChr); //表前数——质量

    //表前数——体积 	浮点数形式
    flDat1 = ctof(dBuf[58], dBuf[57], dBuf[56], dBuf[55]);
    tlngDat = flDat1 * 10;
    tDisChr[0] = tlngDat / 1000000000;
    tlngDat = tlngDat % 1000000000;
    tDisChr[1] = tlngDat / 100000000;
    tlngDat = tlngDat % 100000000;
    tDisChr[2] = tlngDat / 10000000;
    tlngDat = tlngDat % 10000000;
    tDisChr[3] = tlngDat / 1000000;
    tlngDat = tlngDat % 1000000;
    tDisChr[4] = tlngDat / 100000;
    tlngDat = tlngDat % 100000;
    tDisChr[5] = tlngDat / 10000;
    tlngDat = tlngDat % 10000;
    tDisChr[6] = tlngDat / 1000;
    tlngDat = tlngDat % 1000;
    tDisChr[7] = tlngDat / 100;
    tlngDat = tlngDat % 100;
    tDisChr[8] = tlngDat / 10;
    tlngDat = tlngDat % 10;
    tDisChr[9] = '.';
    tDisChr[10] = tlngDat;
    tDisChr[11] = 0;
    for(i = 0; i < 9; i++)
        tDisChr[i] += '0';
    tDisChr[10] += '0';
    for(i = 0; i < 8; i++)
    {
        if(tDisChr[i] == '0')
            tDisChr[i] = ' ';
        else
            break;
    }
    tDisChr[11] = 'L';
    tDisChr[12] = 0;
    tDisChr[13] = 0;
    lcd_text16(105 + tx + 115, 80 + 6 * 30 + 6, 0, 0, tDisChr); //表前数——体积
    //表后数——质量1	浮点数形式
    flDat = ctof(dBuf[24], dBuf[23], dBuf[22], dBuf[21]);
    tlngDat = flDat * 10;
    tDisChr[0] = tlngDat / 1000000000;
    tlngDat = tlngDat % 1000000000;
    tDisChr[1] = tlngDat / 100000000;
    tlngDat = tlngDat % 100000000;
    tDisChr[2] = tlngDat / 10000000;
    tlngDat = tlngDat % 10000000;
    tDisChr[3] = tlngDat / 1000000;
    tlngDat = tlngDat % 1000000;
    tDisChr[4] = tlngDat / 100000;
    tlngDat = tlngDat % 100000;
    tDisChr[5] = tlngDat / 10000;
    tlngDat = tlngDat % 10000;
    tDisChr[6] = tlngDat / 1000;
    tlngDat = tlngDat % 1000;
    tDisChr[7] = tlngDat / 100;
    tlngDat = tlngDat % 100;
    tDisChr[8] = tlngDat / 10;
    tlngDat = tlngDat % 10;
    tDisChr[9] = '.';
    tDisChr[10] = tlngDat;
    tDisChr[11] = 0;
    for(i = 0; i < 9; i++)
        tDisChr[i] += '0';
    tDisChr[10] += '0';
    for(i = 0; i < 8; i++)
    {
        if(tDisChr[i] == '0')
            tDisChr[i] = ' ';
        else
            break;
    }
    tDisChr[11] = 'K';
    tDisChr[12] = 'g';
    tDisChr[13] = 0;
    lcd_text16(105 + tx, 80 + 7 * 30 + 6, 0, 0, tDisChr); //表后数1
    //表后数——质量1	浮点数形式
    flDat = ctof(dBuf[62], dBuf[61], dBuf[60], dBuf[59]);
    tlngDat = flDat * 10;
    tDisChr[0] = tlngDat / 1000000000;
    tlngDat = tlngDat % 1000000000;
    tDisChr[1] = tlngDat / 100000000;
    tlngDat = tlngDat % 100000000;
    tDisChr[2] = tlngDat / 10000000;
    tlngDat = tlngDat % 10000000;
    tDisChr[3] = tlngDat / 1000000;
    tlngDat = tlngDat % 1000000;
    tDisChr[4] = tlngDat / 100000;
    tlngDat = tlngDat % 100000;
    tDisChr[5] = tlngDat / 10000;
    tlngDat = tlngDat % 10000;
    tDisChr[6] = tlngDat / 1000;
    tlngDat = tlngDat % 1000;
    tDisChr[7] = tlngDat / 100;
    tlngDat = tlngDat % 100;
    tDisChr[8] = tlngDat / 10;
    tlngDat = tlngDat % 10;
    tDisChr[9] = '.';
    tDisChr[10] = tlngDat;
    tDisChr[11] = 0;
    for(i = 0; i < 9; i++)
        tDisChr[i] += '0';
    tDisChr[10] += '0';
    for(i = 0; i < 8; i++)
    {
        if(tDisChr[i] == '0')
            tDisChr[i] = ' ';
        else
            break;
    }
    tDisChr[11] = 'L';
    tDisChr[12] = 0;
    tDisChr[13] = 0;
    lcd_text16(105 + 115 + tx, 80 + 7 * 30 + 6, 0, 0, tDisChr); //表后数——体积

    tlngDat = (flDat - flDat1) * 10;
    tDisChr[0] = tlngDat / 1000000000;
    tlngDat = tlngDat % 1000000000;
    tDisChr[1] = tlngDat / 100000000;
    tlngDat = tlngDat % 100000000;
    tDisChr[2] = tlngDat / 10000000;
    tlngDat = tlngDat % 10000000;
    tDisChr[3] = tlngDat / 1000000;
    tlngDat = tlngDat % 1000000;
    tDisChr[4] = tlngDat / 100000;
    tlngDat = tlngDat % 100000;
    tDisChr[5] = tlngDat / 10000;
    tlngDat = tlngDat % 10000;
    tDisChr[6] = tlngDat / 1000;
    tlngDat = tlngDat % 1000;
    tDisChr[7] = tlngDat / 100;
    tlngDat = tlngDat % 100;
    tDisChr[8] = tlngDat / 10;
    tlngDat = tlngDat % 10;
    tDisChr[9] = '.';
    tDisChr[10] = tlngDat;
    tDisChr[11] = 0;
    for(i = 0; i < 9; i++)
        tDisChr[i] += '0';
    tDisChr[10] += '0';
    for(i = 0; i < 8; i++)
    {
        if(tDisChr[i] == '0')
            tDisChr[i] = ' ';
        else
            break;
    }
    tDisChr[11] = 'L';
    tDisChr[12] = 0;
    tDisChr[13] = 0;
    lcd_text16(105 + 115 + tx, 80 + 5 * 30 + 6, 0, 0, tDisChr); //实发量——体积

    //开始时间
    tDisChr[0] = '2';
    tDisChr[1] = '0';
    tDisChr[2] = (dBuf[25] >> 4) + 0x30;
    tDisChr[3] = (dBuf[25] & 0x0F) + 0x30;
    tDisChr[4] = '-';
    tDisChr[5] = (dBuf[26] >> 4) + 0x30;
    tDisChr[6] = (dBuf[26] & 0x0F) + 0x30;
    tDisChr[7] = '-';
    tDisChr[8] = (dBuf[27] >> 4) + 0x30;
    tDisChr[9] = (dBuf[27] & 0x0F) + 0x30;
    tDisChr[10] = ' ';
    tDisChr[11] = (dBuf[28] >> 4) + 0x30;
    tDisChr[12] = (dBuf[28] & 0x0F) + 0x30;
    tDisChr[13] = ':';
    tDisChr[14] = (dBuf[29] >> 4) + 0x30;
    tDisChr[15] = (dBuf[29] & 0x0F) + 0x30;
    tDisChr[16] = ':';
    tDisChr[17] = (dBuf[30] >> 4) + 0x30;
    tDisChr[18] = (dBuf[30] & 0x0F) + 0x30;
    tDisChr[19] = 0;
    lcd_text16(105 + tx, 80 + 8 * 30 + 6, 0, 0, tDisChr); //开始时间
    //结束时间
    tDisChr[0] = '2';
    tDisChr[1] = '0';
    tDisChr[2] = (dBuf[31] >> 4) + 0x30;
    tDisChr[3] = (dBuf[31] & 0x0F) + 0x30;
    tDisChr[4] = '-';
    tDisChr[5] = (dBuf[32] >> 4) + 0x30;
    tDisChr[6] = (dBuf[32] & 0x0F) + 0x30;
    tDisChr[7] = '-';
    tDisChr[8] = (dBuf[33] >> 4) + 0x30;
    tDisChr[9] = (dBuf[33] & 0x0F) + 0x30;
    tDisChr[10] = ' ';
    tDisChr[11] = (dBuf[34] >> 4) + 0x30;
    tDisChr[12] = (dBuf[34] & 0x0F) + 0x30;
    tDisChr[13] = ':';
    tDisChr[14] = (dBuf[35] >> 4) + 0x30;
    tDisChr[15] = (dBuf[35] & 0x0F) + 0x30;
    tDisChr[16] = ':';
    tDisChr[17] = (dBuf[36] >> 4) + 0x30;
    tDisChr[18] = (dBuf[36] & 0x0F) + 0x30;
    tDisChr[19] = 0;
    lcd_text16(105 + tx, 80 + 9 * 30 + 6, 0, 0, tDisChr); //结束时间
}

//新显示记录程序
void Dis_New_Lst(unsigned char tFyPgNum)
{
    unsigned char thCnt;
    char thi;		//临时指针
//	unsigned char thLos;	//当前位置号
//	unsigned int  thRtn;	//返回值
    unsigned char tPgNum;	//由页号查询得出起点,tPgNum=3*tFyPgNum
    char thBuf[80];				//单记录缓存
    unsigned char ti;
    //unsigned char
//	unsigned char tCrc;		//校验

//	thRtn=Fnd_Lst_Index();//找出最后一条记录
//	thLos=thRtn>>8;
    tPgNum = 3 * tFyPgNum;
    thCnt = 0;
//	if(thLos>=tPgNum)
//	for(thi=(thLos-tPgNum);;thi--)
    for(thi = LastDisLos;; thi--)
    {
        EepRd(0x340 + thi * 80, 80);
        for(ti = 0; ti < 80; ti++)
            thBuf[ti] = EepRdBuf[ti];

        if(((thBuf[0] & 0xF0) == 0xA0) && ((thBuf[1] & 0xF0) == 0x50) && (thBuf[79] == 0x5F)) //&&(thBuf[1]<200))
        {
            for(ti = 0; ti < 80; ti++)
            {
                ManLstBuf[thCnt][ti] = thBuf[ti];
            }
            cntLstPage = thCnt;
            Dis_Lst_Cnt(thBuf, tPgNum + thCnt + 1, thCnt);
            thCnt++;
            LstDisCnt++;
            if(LastDisLos > 0)
                LastDisLos--;
            else
                LastDisLos = 199;
        }
        if(LstDisCnt == LstAllCnt)
            break;
//		if(thi==0)
//			break;
        if(thCnt >= 3)
            break;
    }
}
//第三次修改
void Draw_Key(unsigned char x, unsigned char y, unsigned int color)
{
    Draw_Rect(200 + x * 115, 40 + y * 110, 100, 100, color);
}
extern unsigned char frmNum;
unsigned char calc_X, calc_Y;
/*
**计算器按键处理
*/
void Key_CALC(void)
{
    if(ubKey_Command == KEY_L_ARROW) //左移，旧x变黑，新x变红
    {
        if(calc_X > 0)
        {
            Draw_Key(calc_X, calc_Y, BLACK);
            calc_X--;
            Draw_Key(calc_X, calc_Y, RED);
        }
    }
    if(ubKey_Command == KEY_R_ARROW) //右移，旧x变黑，新x变红
    {
        {   if(calc_X < 3)
            {
                Draw_Key(calc_X, calc_Y, BLACK);
                calc_X++;
                Draw_Key(calc_X, calc_Y, RED);
            }
        }

    }
    if(ubKey_Command == KEY_U_ARROW) //上移，旧x变黑，新x变红
    {
        if(calc_Y > 0)
        {
            Draw_Key(calc_X, calc_Y, BLACK);
            calc_Y--;
            Draw_Key(calc_X, calc_Y, RED);
        }
    }
    if(ubKey_Command == KEY_D_ARROW) //下移，旧x变黑，新x变红
    {
        if(calc_Y < 3)
        {
            Draw_Key(calc_X, calc_Y, BLACK);
            calc_Y++;
            Draw_Key(calc_X, calc_Y, RED);
        }
    }

}
//第四次修改2017年2、17（计算器按键确定部分）
static int nDir = 1;
int s1[10] = {0};
int s2[10] = {0};
int Olen  = 0;
int Tlen  = 0;
int nFlag = 0;
int xy[4][4] = {'*', '-', '+', '=', '1', '4', '7', '/', '2', '5', '8', '0', '3', '6', '9', 'c'};
void S1_S2()
{
    if(!nFlag)
    {
        s1[Olen++] = xy[calc_X][calc_Y];
    }
    else
    {
        s2[Tlen++] = xy[calc_X][calc_Y];
    }
}


void WT(int x, int y, int z)
{
    z = z * 40;
    switch(x)
    {
    case 0:
        if(y == 1)
        {
            lcd_text16(10 * (nDir - 1), z, BLACK, WHITE, "1");
            S1_S2();
            nDir++;
        }
        else if(y == 2)
        {
            lcd_text16(10 * (nDir - 1), z, BLACK, WHITE, "2");
            S1_S2();
            nDir++;
        }
        else if(y == 3)
        {
            lcd_text16(10 * (nDir - 1), z, BLACK, WHITE, "3");
            S1_S2();
            nDir++;
        }
        break;
    case 1:
        if(y == 1)
        {
            lcd_text16(10 * (nDir - 1), z, BLACK, WHITE, "4");
            S1_S2();
            nDir++;
        }
        else if(y == 2)
        {
            lcd_text16(10 * (nDir - 1), z, BLACK, WHITE, "5");
            S1_S2();
            nDir++;
        }
        else if(y == 3)
        {
            lcd_text16(10 * (nDir - 1), z, BLACK, WHITE, "6");
            S1_S2();
            nDir++;
        }
        break;
    case 2:
        if(y == 1)
        {
            lcd_text16(10 * (nDir - 1), z, BLACK, WHITE, "7");
            S1_S2();
            nDir++;
        }
        else if(y == 2)
        {
            lcd_text16(10 * (nDir - 1), z, BLACK, WHITE, "8");
            S1_S2();
            nDir++;
        }
        else if(y == 3)
        {
            lcd_text16(10 * (nDir - 1), z, BLACK, WHITE, "9");
            S1_S2();
            nDir++;
        }
        break;
    case 3:
        if(y == 2)
        {
            lcd_text16(10 * (nDir - 1), z, BLACK, WHITE, "0");
            S1_S2();
            nDir++;
        }
        break;
    }

}


void FuncOk()
{
    if(calc_Y == 0)
    {
        if(calc_X == 0)
        {
            lcd_text16(0, 40, BLACK, WHITE, "*");
            nFlag = 1;
        }
        else if(calc_X == 1)
        {
            lcd_text16(0, 40, BLACK, WHITE, "-");
            nFlag = 2;
        }
        else if(calc_X == 2)
        {
            lcd_text16(0, 40, BLACK, WHITE, "+");
            nFlag = 3;
        }
        else if(calc_X == 3)
        {
            lcd_text16(0, 40, BLACK, WHITE, "/");
            nFlag = 4;
        }
    }
    else if(calc_Y != 0 && nFlag == 0)
    {
        if((calc_X >= 0) && (calc_X <= 3) && (calc_Y >= 1) && (calc_Y <= 3))
        {
            WT(calc_X, calc_Y, 1);
        }
    }
    else if(calc_Y != 0 && nFlag != 0)
        if((calc_X >= 0) && (calc_X <= 3) && (calc_Y >= 1) && (calc_Y <= 3))
        {
            WT(calc_X, calc_Y, 2);
        }
    if(calc_X == 3 && calc_Y == 1)
    {   lcd_text16(0, 120, BLACK, WHITE, "=");
        int i = 0;
        long int n = 0;
        long int m = 0;
        int s[10];
        for(i = 0; i < Olen; i++)
        {
            n = n + s1[i] * (10 ^ (i - 1));
        }
        for(i = 0; i < Tlen; i++)
        {
            m = m + s2[i] * (10 ^ (i - 1));
        }
        if(nFlag == 1)
        {
            n = m + n;
        }
        if(nFlag == 2)
        {
            n = n - m;
        }
        if(nFlag == 3)
        {
            n = n * m;
        }
        if(nFlag == 4)
        {
            n = n / m;
        }
        while(n / 10 > 0)
        {
            s[i++] = n % 10;
            n = n / 10;
        }
        s[i] = n;
        // lcd_text16(140,160,BLACK,WHITE,"=");

    }
}


void KeyHandler(void)
{
    KeyCode_new = KeyRead(); //获取键值，0X3F(0b111111)表示无键按下

    if(KeyCode_new != KeyCode) //KeyCode是什么？
    {
        if((KeyCode_new == 0x3F)) //&&(KeyPress==1)
        {
            KeyPress = 0;
            ubKey_Command = 0;
        }
        KeyCode = KeyCode_new;
        KeyCnt = 0;
    }
    else
    {
        if(KeyCode_new != 0x3F) //去抖
        {
            KeyCnt++;
            if((KeyCnt > 1) && (KeyRelease == 1))
            {
                switch(KeyCode)
                {
                case  1:
                    ubKey_Command = 0x41;
                    break;//Num1
                case  2:
                    ubKey_Command = 0x42;
                    break;//Num2
                case  3:
                    ubKey_Command = 0x43;
                    break;//Num3
                case  6:
                    ubKey_Command = 0x44;
                    break;//Num4
                case  7:
                    ubKey_Command = 0x45;
                    break;//Num5
                case  8:
                    ubKey_Command = 0x46;
                    break;//Num6
                case 11:
                    ubKey_Command = 0x47;
                    break;//Num7
                case 12:
                    ubKey_Command = 0x48;
                    break;//Num8
                case 13:
                    ubKey_Command = 0x49;
                    break;//Num9
                case 16:
                    ubKey_Command = 0x4A;
                    break;//Num.
                case 17:
                    ubKey_Command = 0x40;
                    break;//Num0
                case 18:
                    ubKey_Command = 0x4B;
                    break;//Num000
                case  4:
                    ubKey_Command = 0x21;
                    break;//SET
                case  5:
                    ubKey_Command = 0x25;
                    break;//ESC
                case 19:
                    ubKey_Command = 0x24;
                    break;//KeyDel
                case 20:
                    ubKey_Command = 0x22;
                    break;//KeyOK
                case 14:
                    ubKey_Command = 0x11;
                    break;//Key_Up
                case 15:
                    ubKey_Command = 0x12;
                    break;//Key_Dn
                case  9:
                    ubKey_Command = 0x13;
                    break;//Key_Left
                case 10:
                    ubKey_Command = 0x14;
                    break;//Key_Right
                case 21:
                    ubKey_Command = 0x31;
                    break;
                case 22:
                    ubKey_Command = 0x32;
                    break;
                case 23:
                    ubKey_Command = 0x33;
                    break;
                case 24:
                    ubKey_Command = 0x34;
                    break;
                default:
                    ubKey_Command = 0;
                    break;
                }
                KeyRelease = 0;
                Key_CALC();//光标处理
                FuncOk();
                KeyPress = 1;
            }
        }
        else
        {
            KeyCnt_free++;
            if(KeyCnt_free > 1)
                KeyRelease = 1;
            KeyPress = 0;
            ubKey_Command = 0;
        }
    }
}



void KeyHandler_(void)
{
    if(flgRmCon > 0)
    {
        Key_Main();
    }
    KeyCode_new = KeyRead();

    if(KeyCode_new != KeyCode)
    {
        if((KeyCode_new == 0x3F)) //&&(KeyPress==1)
        {
            KeyPress = 0;
            ubKey_Command = 0;
        }
        KeyCode = KeyCode_new;
        KeyCnt = 0;
    }
    else
    {
        if(KeyCode_new != 0x3F)
        {
            KeyCnt++;
            if((KeyCnt > 1) && (KeyRelease == 1))
            {
                switch(KeyCode)
                {
                case  1:
                    ubKey_Command = 0x41;
                    break;//Num1
                case  2:
                    ubKey_Command = 0x42;
                    break;//Num2
                case  3:
                    ubKey_Command = 0x43;
                    break;//Num3
                case  6:
                    ubKey_Command = 0x44;
                    break;//Num4
                case  7:
                    ubKey_Command = 0x45;
                    break;//Num5
                case  8:
                    ubKey_Command = 0x46;
                    break;//Num6
                case 11:
                    ubKey_Command = 0x47;
                    break;//Num7
                case 12:
                    ubKey_Command = 0x48;
                    break;//Num8
                case 13:
                    ubKey_Command = 0x49;
                    break;//Num9
                case 16:
                    ubKey_Command = 0x4A;
                    break;//Num.
                case 17:
                    ubKey_Command = 0x40;
                    break;//Num0
                case 18:
                    ubKey_Command = 0x4B;
                    break;//Num000
                case  4:
                    ubKey_Command = 0x21;
                    break;//SET
                case  5:
                    ubKey_Command = 0x25;
                    break;//ESC
                case 19:
                    ubKey_Command = 0x24;
                    break;//KeyDel
                case 20:
                    ubKey_Command = 0x22;
                    break;//KeyOK
                case 14:
                    ubKey_Command = 0x11;
                    break;//Key_Up
                case 15:
                    ubKey_Command = 0x12;
                    break;//Key_Dn
                case  9:
                    ubKey_Command = 0x13;
                    break;//Key_Left
                case 10:
                    ubKey_Command = 0x14;
                    break;//Key_Right
                case 21:
                    ubKey_Command = 0x31;
                    break;
                case 22:
                    ubKey_Command = 0x32;
                    break;
                case 23:
                    ubKey_Command = 0x33;
                    break;
                case 24:
                    ubKey_Command = 0x34;
                    break;
                default:
                    ubKey_Command = 0;
                    break;
                }
                KeyRelease = 0;
                switch(frmNum)
                {
                case FRM_MON:
                    Key_MON();
                    break;
                case FRM_CONFIG:
                    Key_Conf();
                    break;
                case FRM_MAN:
                    Key_Man();
                    break;
                case FRM_LOGIN:
                    Key_Log();
                    break;
                case FRM_PWD:
                    Key_Pwd();
                    break;
                case FRM_MAIN:
                    Key_Main();
                    break;
                case FRM_PENDLST:
                    Key_Pend();
                    break;
//				case FRM_PENDSEL:	Key_PendSel(); 	break;
                case FRM_DONELST:
                    Key_Lst();
                    break;
//				case FRM_WARN:		Key_WarnLst();	break;
                case FRM_SETTING:
                    Key_Set_Handler();
                    break;
//				case FRM_SD: 			Key_SD();				break;
                case FRM_AMS:
                    Key_AMS();
                    break;
                }
                KeyPress = 1;
                //if(KeyCode_new!=0x3F)
                //	Dis_Num16(280,450,0,GOLD,KeyCode);
            }
        }
        else
        {
            KeyCnt_free++;
            if(KeyCnt_free > 1)
                KeyRelease = 1;
            KeyPress = 0;
            ubKey_Command = 0;
        }
    }
}
unsigned char flgInitSet;
unsigned char flgSetGet[15];//组态信息成功获取标志
unsigned char xpos, ypos;

unsigned char GetFvConn(unsigned char id)
{
    return 1;
}
void DrSetSel(unsigned char x, unsigned char y, unsigned char id)
{
    unsigned int  xL[7] = {39, 144, 244, 344, 490, 579, 694};
    unsigned int  xT[4] = {180, 220, 260, 300};
    unsigned char xW[7] = {62, 34, 50, 58, 50, 74, 58};
    if(id == 0)
    {
        if(x != 4)
            LcdDrawRectangle(xL[x] - 5, xT[y] - 5, xL[x] + xW[x] + 10, xT[y] + 20, WHITE, 0);
        else
            LcdDrawRectangle(xL[x] - 5, xT[y] - 2, xL[x] + xW[x] + 6, xT[y] + 31, WHITE, 0);
    }
    if(id == 1)
    {   if(x != 4)
            LcdDrawRectangle(xL[x] - 5, xT[y] - 5, xL[x] + xW[x] + 10, xT[y] + 20, RED, 0);
        else
            LcdDrawRectangle(xL[x] - 5, xT[y] - 2, xL[x] + xW[x] + 6, xT[y] + 31, RED, 0);
    }
    if(id == 2)
    {   if(x != 4)
            LcdDrawRectangle(xL[x] - 5, xT[y] - 5, xL[x] + xW[x] + 10, xT[y] + 20, BLUE, 0);
        else
            LcdDrawRectangle(xL[x] - 5, xT[y] - 2, xL[x] + xW[x] + 6, xT[y] + 31, BLUE, 0);
    }
}

extern unsigned int  ftaDIdat, ftaDIdat_pre;
extern unsigned char ftaDOdat, ftaDOdat_pre;
unsigned char curAddr;//地址编码输入时的光标
void Key_Set(void)
{
    unsigned char yMax[7] = {3, 3, 2, 2, 0, 3, 2};
    unsigned int  yTop[4] = {186, 226, 266, 306};
    unsigned char i;
    unsigned char ypos_pre;
    if(flgEdit == 0)
    {
        if(KeyCommand == KEY_Esc)
        {
            frmNum = FRM_MAIN;
            KeyCommand = 0;
            ftaDIdat_pre = 0xFF;
            ftaDOdat_pre = 0xFF;
        }
        if(KeyCommand == KEY_L_ARROW) //左键
        {   KeyCommand = 0;
            if(xpos > 0)
            {
                DrSetSel(xpos, ypos, 0);
                xpos--;
                if(yMax[xpos] < ypos)
                    ypos = yMax[xpos];
                DrSetSel(xpos, ypos, 1);
            }
        }
        if(KeyCommand == KEY_R_ARROW)
        {   KeyCommand = 0;
            if(xpos < 6)
            {
                DrSetSel(xpos, ypos, 0);
                xpos++;
                if(yMax[xpos] < ypos)
                    ypos = yMax[xpos];
                DrSetSel(xpos, ypos, 1);
            }

        }
        if(KeyCommand == KEY_U_ARROW)
        {   KeyCommand = 0;
            if(ypos > 0)
            {
                DrSetSel(xpos, ypos, 0);
                ypos--;
                DrSetSel(xpos, ypos, 1);
            }

        }
        if(KeyCommand == KEY_D_ARROW)
        {   KeyCommand = 0;
            if(ypos < yMax[xpos])
            {
                DrSetSel(xpos, ypos, 0);
                ypos++;
                DrSetSel(xpos, ypos, 1);
            }
        }
        if(KeyCommand == KEY_SET)
        {
            KeyCommand = 0;
            if(xpos == 4)
            {
                disAddrBuf[0] = ' ';
                disAddrBuf[1] = ' ';
                disAddrBuf[2] = 0;
                //lcd_text16(510,189,BLACK,WHITE,disAddrBuf);
                TFT_Fill(492, 185, 538, 205, WHITE);
            }
            DrSetSel(xpos, ypos, 2);
            flgEdit = 1;

        }
    }
    else
    {   //flgEdit=1 编辑模式
        if(xpos == 4)
        {
            if(((KeyCommand > 0) && (KeyCommand < 10)) || (KeyCommand == 11))
            {
                if(curAddr < 2)
                {
                    if(KeyCommand == 11)
                        disAddrBuf[curAddr] = '0';
                    else
                        disAddrBuf[curAddr] = '0' + KeyCommand;
                    curAddr++;
                }
                disAddrBuf[2] = 0;
                lcd_text16(510, 189, BLUE, WHITE, disAddrBuf);

            }
        }
        if(KeyCommand == KEY_ENTER)
        {
            ypos_pre = sysSavDat[xpos] - 1;
            switch(xpos)
            {
            case 0:
                LcdDrawCircle(45, 186, 3, WHITE, 1);
                LcdDrawCircle(45, 226, 3, WHITE, 1);
                LcdDrawCircle(45, 266, 3, WHITE, 1);
                LcdDrawCircle(45, 306, 3, WHITE, 1);
                sysSavDat[xpos] = ypos + 1;
                LcdDrawCircle(45, yTop[ypos], 3, 0, 1);
                break;
            case 1:
                LcdDrawCircle(150, 186, 3, WHITE, 1);
                LcdDrawCircle(150, 226, 3, WHITE, 1);
                LcdDrawCircle(150, 266, 3, WHITE, 1);
                LcdDrawCircle(150, 306, 3, WHITE, 1);
                sysSavDat[xpos] = ypos + 1;
                LcdDrawCircle(150, yTop[ypos], 3, 0, 1);
                break;
            case 2:
                LcdDrawCircle(250, 186, 3, WHITE, 1);
                LcdDrawCircle(250, 226, 3, WHITE, 1);
                LcdDrawCircle(250, 266, 3, WHITE, 1);
                LcdDrawCircle(250, 306, 3, WHITE, 1);
                sysSavDat[xpos] = ypos + 1;
                LcdDrawCircle(250, yTop[ypos], 3, 0, 1);
                break;
            case 3:
                LcdDrawCircle(350, 186, 3, WHITE, 1);
                LcdDrawCircle(350, 226, 3, WHITE, 1);
                LcdDrawCircle(350, 266, 3, WHITE, 1);
                LcdDrawCircle(350, 306, 3, WHITE, 1);
                sysSavDat[xpos] = ypos + 1;
                LcdDrawCircle(350, yTop[ypos], 3, 0, 1);
                break;
            case 5:
                LcdDrawCircle(585, 186, 3, WHITE, 1);
                LcdDrawCircle(585, 226, 3, WHITE, 1);
                LcdDrawCircle(585, 266, 3, WHITE, 1);
                LcdDrawCircle(585, 306, 3, WHITE, 1);
                sysSavDat[xpos] = ypos + 1;
                LcdDrawCircle(585, yTop[ypos], 3, 0, 1);
                break;
            case 6:
                LcdDrawCircle(700, 186, 3, WHITE, 1);
                LcdDrawCircle(700, 226, 3, WHITE, 1);
                LcdDrawCircle(700, 266, 3, WHITE, 1);
                LcdDrawCircle(700, 306, 3, WHITE, 1);
                sysSavDat[xpos] = ypos + 1;
                LcdDrawCircle(700, yTop[ypos], 3, 0, 1);
                break;
            case 4:
                AddrIfix_pre = AddrIfix;
                if((disAddrBuf[1] == ' ') && (disAddrBuf[0] > 0x2F) && (disAddrBuf[0] < 0x3A))
                {
                    AddrIfix = (disAddrBuf[0] - '0');
                }
                if((disAddrBuf[0] != ' ') && (disAddrBuf[1] != ' '))
                {
                    if((disAddrBuf[0] > 0x2F) && (disAddrBuf[0] < 0x3A))
                        AddrIfix = (disAddrBuf[0] - '0') * 10;
                    if((disAddrBuf[1] > 0x2F) && (disAddrBuf[1] < 0x3A))
                        AddrIfix += (disAddrBuf[1] - '0');
                }
                if((AddrIfix > 0) && (AddrIfix < 99))
                {
                    lcd_text16(510, 189, BLACK, WHITE, disAddrBuf);
                }
                else
                {
                    AddrIfix = AddrIfix_pre;
                }
                sysSavDat[4] = AddrIfix;
                disAddrBuf[0] = '0' + (AddrIfix / 10);
                disAddrBuf[1] = '0' + (AddrIfix % 10);
                disAddrBuf[2] = 0;
                if(disAddrBuf[0] == '0')
                    disAddrBuf[0] = ' ';
                TFT_Fill(492, 185, 538, 205, WHITE);
                lcd_text16(518, 189, BLACK, WHITE, disAddrBuf);
                break;
            }
            for(i = 0; i < 7; i++)
            {
                EepWrBuf[i] = sysSavDat[i];
            }
            EepWr(10, 7);
            flgEdit = 0;
            DrSetSel(xpos, ypos, 1);

        }
    }
}
void Key_MON(void)
{
    unsigned char i;
    switch(ubKey_Command)
    {
    case KEY_RTN:
        frmNum = FRM_LOGIN;
        break;
    case 0x41://NUM1
        if(sys_Conn == 0) //调试用，正式打包时去除
        {
            dualChNum = 1; //mbdata[25]=1;
            canRtData[24] = 1;
            frmNum = FRM_MAIN; //FRM_PENDSEL;
            for(i = 0; i < 10; i++)
            {
                idCarTypeId[i] = 0;
                idCarId[i] = 0;
                idCarYFL[i] = 0;
            }
        }
//else  //测试联网模式下功能
//{
//  for(i=0;i<10;i++)
//  {idCarTypeId[i]=0;idCarId[i]=0;idCarYFL[i]=0;}
//  dualChNum=1;mbdata[25]=1;
// 	frmNum=FRM_PENDSEL;
//}
        break;
    case 0x42://NUM2
        if(sys_Conn == 0) //调试用，正式打包时去除
        {
            for(i = 0; i < 10; i++)
            {
                idCarTypeId[i] = 0;
                idCarId[i] = 0;
                idCarYFL[i] = 0;
            }
            dualChNum = 2; //mbdata[25]=2;
            canRtData[24] = 2;
            frmNum = FRM_MAIN; //FRM_PENDSEL;
        }
//else  //测试联网模式下功能
//{
//  for(i=0;i<10;i++)
//  {idCarTypeId[i]=0;idCarId[i]=0;idCarYFL[i]=0;}
//  dualChNum=2;mbdata[25]=2;
// 	frmNum=FRM_PENDSEL;
//}
        break;

    }
}
void Key_Lst(void)
{
    unsigned char ti;
//unsigned char LstCur;		//已发记录界面光标
//unsigned char LstCnt;		//有效记录数
//unsigned char LstPgNum;	//页数
    if((sys_STA == 0) && (ubKey_Command == KEY_RTN))
    {
        frmNum = FRM_MAIN;
    }
    if(ubKey_Command == KEY_D_ARROW)
    {
        if(LstIndex == 0)
        {
            LstIndex = 1;
            Draw_Rect_Color(140 + LstCur * 230 - 4, 86 - 4, 60 + 8, 18 + 8, 2, WHITE);
            LstCur = 0;
            Draw_Sel(33 - 5, 390 - 5, 80 + 10, 30 + 10, RED);
        }
    }
    if(ubKey_Command == KEY_U_ARROW)
    {
        if(LstIndex == 1)
        {
            LstIndex = 0;

            if(LstCur == 0)
            {
                Draw_Sel(33 - 5, 390 - 5, 80 + 10, 30 + 10, WHITE);
            }
            if(LstCur == 1)
            {
                Draw_Sel(133 - 5, 390 - 5, 80 + 10, 30 + 10, WHITE);
            }
            if(LstCur == 2)
            {
                Draw_Sel(710 - 5, 390 - 5, 80 + 10, 30 + 10, WHITE);
            }
            LstCur = 0;
            Draw_Rect_Color(140 + LstCur * 230 - 4, 86 - 4, 60 + 8, 18 + 8, 2, RED);
        }
    }
    if(ubKey_Command == KEY_R_ARROW)
    {
//		if(LstCur<2)
//			LstCur++;
//		if(LstCur==1)
//		{
//			Draw_Sel(33-5,365-5,80+10,30+10,WHITE);
//			Draw_Sel(133-5,365-5,80+10,30+10,RED);
//		}
//		if(LstCur==2)
//		{
//			Draw_Sel(133-5,365-5,80+10,30+10,WHITE);
//			Draw_Sel(710-5,365-5,80+10,30+10,RED);
//		}
        if(LstIndex == 1)
        {
            if(LstCur < 2)
            {
                LstCur++;
                if(LstCur == 1)
                {
                    Draw_Sel(33 - 5, 390 - 5, 80 + 10, 30 + 10, WHITE);
                    Draw_Sel(133 - 5, 390 - 5, 80 + 10, 30 + 10, RED);
                }
                if(LstCur == 2)
                {
                    Draw_Sel(133 - 5, 390 - 5, 80 + 10, 30 + 10, WHITE);
                    Draw_Sel(710 - 5, 390 - 5, 80 + 10, 30 + 10, RED);
                }
            }
        }
        if(LstIndex == 0)
        {
            if(LstCur < cntLstPage)
            {
                Draw_Rect_Color(140 + LstCur * 230 - 4, 86 - 4, 60 + 8, 18 + 8, 2, WHITE);
                LstCur++;
                Draw_Rect_Color(140 + LstCur * 230 - 4, 86 - 4, 60 + 8, 18 + 8, 2, RED);
            }
        }
    }
    if(ubKey_Command == KEY_L_ARROW)
    {
//		if(LstCur>0)
//			LstCur--;
//		if(LstCur==0)
//		{
//			Draw_Sel(133-5,365-5,80+10,30+10,WHITE);
//			Draw_Sel(33-5,365-5,80+10,30+10,RED);
//		}
//		if(LstCur==1)
//		{
//			Draw_Sel(710-5,365-5,80+10,30+10,WHITE);
//			Draw_Sel(133-5,365-5,80+10,30+10,RED);
//		}
        if(LstIndex == 1)
        {
            if(LstCur > 0)
            {
                LstCur--;
                if(LstCur == 0)
                {
                    Draw_Sel(133 - 5, 390 - 5, 80 + 10, 30 + 10, WHITE);
                    Draw_Sel(33 - 5, 390 - 5, 80 + 10, 30 + 10, RED);
                }
                if(LstCur == 1)
                {
                    Draw_Sel(710 - 5, 390 - 5, 80 + 10, 30 + 10, WHITE);
                    Draw_Sel(133 - 5, 390 - 5, 80 + 10, 30 + 10, RED);
                }
            }
        }
        if(LstIndex == 0)
        {
            if(LstCur > 0)
            {
                Draw_Rect_Color(140 + LstCur * 230 - 4, 86 - 4, 60 + 8, 18 + 8, 2, WHITE);
                LstCur--;
                Draw_Rect_Color(140 + LstCur * 230 - 4, 86 - 4, 60 + 8, 18 + 8, 2, RED);
            }
        }
    }
    if(ubKey_Command == KEY_ENTER)
    {
        ubKey_Command = 0;
        if(LstIndex == 0)
        {
            Fill_Man_Lst(ManLstBuf[LstCur]);
        }
        else
        {
            if(LstCur == 0) //上一页
            {
                if(LstDisPgNum > 0)
                {   LstDisPgNum--;
                    LstDisCnt = LstDisPgNum * 3;
                    LastDisLos += 6;
                    if(LastDisLos >= 200)
                    {
                        LastDisLos -= 200;
                    }
                    Dis_New_Lst(LstDisPgNum);
                }
            }
            if(LstCur == 1) //下一页
            {
                if((LstDisPgNum < 66) && (LstDisCnt < LstAllCnt))
                {
                    LstDisPgNum++;
                    LstDisCnt = LstDisPgNum * 3;
                    Dis_New_Lst(LstDisPgNum);
                }
            }
            if(LstCur == 2)
            {
                frmMainEdit = 0;
                frmMainCur = 0;
                flgCur = 0;
                for(ti = 0; ti < 9; ti++)
                {   frmMainOrdId[ti] = ' ';
                    frmMainYFval[ti] = ' ';
                }
                LstDisPgNum = 0;
                frmNum = FRM_MAIN;
                u8PendNum[dualChNum - 1] = dGetPendNum(dualChNum - 1); //u8PendNum[1]=dGetPendNum(1);//u8PendNum=GetPendNum();
                if((u8PendNum[dualChNum - 1] > 0) && (sys_Conn == 1)) //if(0)//
                {
                    Dis_1_Pend(1);//Dis_Id_Pend(1);
                    flgPendLst = 1;
                }
            }
        }
    }
}

void Key_Conf(void)
{
    unsigned char ti = 0, i;
    unsigned char tj;
    char tConfDis[10];
    unsigned char tDate;
//	char tmpDate[11];
    if((sys_STA == 0) && (ubKey_Command == KEY_RTN))
    {
        frmNum = FRM_LOGIN;
    }
    if(flgConfEdit == 0)
    {
        if(ubKey_Command == KEY_R_ARROW)
        {
            if(ConfIndex < 3)
            {
                DisConfSel(ConfIndex, ConfPos, WHITE);
                ConfIndex++;
                if(ConfIndex >= 1)ConfPos = 0;
                DisConfSel(ConfIndex, ConfPos, RED);
            }
        }
        if(ubKey_Command == KEY_L_ARROW)
        {
            if(ConfIndex > 0)
            {
                DisConfSel(ConfIndex, ConfPos, WHITE);
                ConfIndex--;
                if(ConfIndex < 3)ConfPos = 0;
                DisConfSel(ConfIndex, ConfPos, RED);
            }
        }
        if(ubKey_Command == KEY_U_ARROW)
        {
            if(ConfPos > 0)
            {
                DisConfSel(ConfIndex, ConfPos, WHITE);
                ConfPos--;
                DisConfSel(ConfIndex, ConfPos, RED);
            }
        }
        if(ubKey_Command == KEY_D_ARROW)
        {
            if(ConfIndex == 0) ti = 9;
            if(ConfIndex == 1) ti = 9; //加入脉冲与
            if(ConfIndex == 2) ti = 10;
            if(ConfIndex == 3) ti = 2;
            if(ConfPos < ti)
            {
                DisConfSel(ConfIndex, ConfPos, WHITE);
                ConfPos++;
                DisConfSel(ConfIndex, ConfPos, RED);
            }
        }
        //设置界面，非编辑状态按下SET
//	if(KeyBuf==KEY_SET)
        if(ubKey_Command == KEY_SET)
        {
            flgConfEdit = 1;
            if(ConfIndex == 0) //第一列10个
            {
                if(ConfPos < 10) //4+5
                {
                    for(ti = 0; ti < 3; ti++)
                    {
                        tmpSavConf[ConfPos][ti] = ' ';
                        tConfDis[ti] = tmpSavConf[ConfPos][ti];
                    }
                    tConfDis[3] = '\0';
                }
            }
            if(ConfIndex == 1) //第二列新增至10个
            {
                if(ConfPos == 9) //新增管径
                {
                    for(ti = 0; ti < 4; ti++)
                    {
                        tmppipeSavConf[ti] = ' ';
                        tConfDis[ti] = tmppipeSavConf[ti];
                    }
                    tConfDis[3] = '\0';
                }
                if(ConfPos == 7) //密度5位			 x.xxxx
                {
                    for(ti = 0; ti < 5; ti++)
                    {
                        tmpplzSavConf[ConfPos - 7][ti] = ' ';
                        tConfDis[ti] = tmpplzSavConf[ConfPos][ti];
                    }
                    tConfDis[5] = '\0';
                }
                if(ConfPos == 8) //系数4位,xx.xx
                {
                    for(ti = 0; ti < 5; ti++)
                    {
                        tmpplzSavConf[ConfPos - 7][ti] = ' ';
                        tConfDis[ti] = tmpplzSavConf[ConfPos][ti];
                    }
                    tConfDis[5] = '\0';
                }
                if((ConfPos == 5) || (ConfPos == 6))
                {
                    for(ti = 0; ti < 6; ti++)
                    {
                        tmpSavConfDate[ConfPos - 5][ti] = ' ';
                        tConfDis[ti] = tmpSavConfDate[ConfPos - 5][ti];
                    }
                    tConfDis[6] = '\0';
                }
                if(ConfPos < 5)
                {
                    for(ti = 0; ti < 2; ti++)
                    {
                        tmpdnSavConf[ConfPos][ti] = ' ';
                        tConfDis[ti] = tmpdnSavConf[ConfPos][ti];
                    }
                    tConfDis[2] = '\0';
                }
            }
            ClrConfSel(ConfIndex, ConfPos);
            ConfCur = 0;
            DisConfSel(ConfIndex, ConfPos, BLUE);
        }
        if(ubKey_Command == KEY_ENTER)	//if(KeyBuf==KEY_ENTER)
        {
            ubKey_Command = 0;
            if(ConfIndex == 3) //选择命令-->0:保存,1:缺省,2:返回
            {
                if(ConfPos == 0) //保存
                {
                    for(ti = 0; ti < 10; ti++)
                    {
                        for(tj = 0; tj < 4; tj++)
                            u8SavConfBuf[tj] = tmpSavConf[ti][tj];
                        ConfChr2Dat();
                        //tSavSetDat=u16SavSetDat;
                        if((u16SavConfDat <= SavConfDatMax[ti]) && (u16SavConfDat >= SavConfDatMin[ti]))
                        {   //数据OK!可以保存
                            SavConfDat[ti] = u16SavConfDat;
                            //flgSavEn[ti]=1;
                        }
                    }

                    //判断日期时间格式是否正确
                    for(ti = 0; ti < 2; ti++)
                    {
                        flgSavEn[4 + ti] = 1;
                        for(tj = 0; tj < 6; tj++)
                        {
                            if(tmpSavConfDate[ti][tj] == ' ')
                            {
                                flgSavEn[4 + ti] = 0;
                                tj = 6;
                            }
                        }
                    }
                    //判断日期时间输入是否正确
                    if(flgSavEn[4] == 1)
                    {
                        flgWrRtc = 1;
                        tDate = (tmpSavConfDate[0][0] & 0x0F) * 10 + (tmpSavConfDate[0][1] & 0x0F);
                        if((tDate <= SavConfDatMax[10]) && (tDate >= SavConfDatMin[10])) //年有效
                        {
                            SavConfDat[10] = tDate;
                            savY = tDate;
                        }
                        else
                        {
                            SavConfDat[10] = sysY;
                            flgWrRtc = 0;
                        }

                        tDate = (tmpSavConfDate[0][2] & 0x0F) * 10 + (tmpSavConfDate[0][3] & 0x0F);
                        if((tDate <= SavConfDatMax[11]) && (tDate >= SavConfDatMin[11])) //月有效
                        {
                            SavConfDat[11] = tDate;
                            savM = tDate;
                        }
                        else
                        {
                            SavConfDat[11] = sysM;
                            flgWrRtc = 0;
                        }

                        tDate = (tmpSavConfDate[0][4] & 0x0F) * 10 + (tmpSavConfDate[0][5] & 0x0F);
                        if((tDate <= SavConfDatMax[12]) && (tDate >= SavConfDatMin[12])) //日有效
                        {
                            SavConfDat[12] = tDate;
                            savD = tDate;
                        }
                        else
                        {
                            SavConfDat[12] = sysD;
                            flgWrRtc = 0;
                        }
                        //Dis_Date();

                        tDate = (tmpSavConfDate[1][0] & 0x0F) * 10 + (tmpSavConfDate[1][1] & 0x0F);
                        if((tDate <= SavConfDatMax[13]) && (tDate >= SavConfDatMin[13])) //时有效
                        {
                            SavConfDat[13] = tDate;
                            savHH = tDate;
                        }
                        else
                        {
                            SavConfDat[13] = sysHH;
                            flgWrRtc = 0;
                        }

                        tDate = (tmpSavConfDate[1][2] & 0x0F) * 10 + (tmpSavConfDate[1][3] & 0x0F);
                        if((tDate <= SavConfDatMax[14]) && (tDate >= SavConfDatMin[14])) //分有效
                        {
                            SavConfDat[14] = tDate;
                            savMM = tDate;
                        }
                        else
                        {
                            SavConfDat[14] = sysMM;
                            flgWrRtc = 0;
                        }

                        tDate = (tmpSavConfDate[1][4] & 0x0F) * 10 + (tmpSavConfDate[1][5] & 0x0F);
                        if((tDate <= SavConfDatMax[15]) && (tDate >= SavConfDatMin[15])) //秒有效
                        {
                            SavConfDat[15] = tDate;
                            savSS = tDate;
                        }
                        else
                        {
                            SavConfDat[15] = sysSS;
                            flgWrRtc = 0;
                        }

                        if(flgWrRtc == 1)
                        {
                            RTCSetDate(savY, savM, savD);
                            RTCSetTime(savHH, savMM, savSS);
                        }

                    }
                    for(ti = 0; ti < 11; ti++)
                        WarnOnOff[ti] = tmpConfWarn[ti];

                    tmpSavConfWarn = tmpConfWarn[0] << 5;
                    tmpSavConfWarn |= tmpConfWarn[1] << 4;
                    tmpSavConfWarn |= tmpConfWarn[2] << 3;
                    tmpSavConfWarn |= tmpConfWarn[3] << 2;
                    tmpSavConfWarn |= tmpConfWarn[4] << 1;
                    tmpSavConfWarn |= tmpConfWarn[5];

                    SavConfDat[16] = tmpSavConfWarn;
                    set_Config = SavConfDat[16];			//组态参数
                    getWarnOnOff();
                    TFT_Fill(160, 20 - 15, 200, 44 - 15, GOLD);
                    if(WarnOnOff[10] == 1) //if((set_Config&0x40)==0x40)
                    {
                        lcd_text24(160, 5, BLUE, GOLD, "远程");
                        //ShowHz(160,5,28);ShowHz(180,5,29);
                        sys_Conn = 1;
                        EepRd(210, 1);
                        EepWrBuf[0] = EepRdBuf[0] | 0x40;
                        //SavConfDat[16]=EepWrBuf[0];
                        EepWr(210, 1);
                    }
                    else
                    {
                        lcd_text24(160, 5, BLUE, GOLD, "本地");
                        //ShowHz(160,5,26);ShowHz(180,5,27);
                        sys_Conn = 0;
                        EepRd(210, 1);
                        EepWrBuf[0] = EepRdBuf[0] & 0xBF;
                        //SavConfDat[16]=EepWrBuf[0];
                        EepWr(210, 1);
                    }
                    if(WarnOnOff[10] == 1)
                        SavConfDat[16] = SavConfDat[16] | 0x40;
                    else
                        SavConfDat[16] = SavConfDat[16] & 0xBF;
                    for(i = 0; i < 17; i++)
                        EepWrBuf[i] = SavConfDat[i];

                    EepWr(194, 17);
                    //for(i=7;i<11;i++)
                    tmpSavConfWarn = tmpConfWarn[6];
                    tmpSavConfWarn = tmpSavConfWarn << 1;
                    tmpSavConfWarn |= tmpConfWarn[7];
                    tmpSavConfWarn = tmpSavConfWarn << 1;
                    tmpSavConfWarn |= tmpConfWarn[8];
                    tmpSavConfWarn = tmpSavConfWarn << 1;
                    tmpSavConfWarn |= tmpConfWarn[9];
                    tmpSavConfWarn = tmpSavConfWarn << 4;
                    EepWrBuf[0] = tmpSavConfWarn;
                    EepWr(230, 1);

                    //保存关阀数据，分速率1,2。分段点1,2。
                    for(i = 0; i < 5; i++)
                    {   for(tj = 0; tj < 3; tj++)
                            u8dnSavConfBuf[tj] = tmpdnSavConf[i][tj];
                        if(u8dnSavConfBuf[0] == ' ')
                        {
                            u8dnSavConfDat = u8dnSavConfBuf[1] - '0';
                        }
                        else
                        {
                            u8dnSavConfDat = (u8dnSavConfBuf[0] - '0') * 10 + (u8dnSavConfBuf[1] - '0');
                        }
                        if((u8dnSavConfDat <= dnSavConfDatMax[i]) && (u8dnSavConfDat >= dnSavConfDatMin[i]))
                        {
                            dnSavConfDat[i] = u8dnSavConfDat;
                        }
                    }
                    if(dnSavConfDat[3] < dnSavConfDat[1])
                    {
                        dnSavConfDat[3] = dnSavConfDat[1];
                    }
                    for(i = 0; i < 5; i++)
                    {
                        EepWrBuf[i] = dnSavConfDat[i];
                    }
                    EepWr(224, 5);
                    //保存管径
                    if(u8pipeSavConfBuf[0] != ' ')
                    {
                        for(tj = 0; tj < 3; tj++)
                            u8pipeSavConfBuf[tj] = tmppipeSavConf[tj];
                        if(u8pipeSavConfBuf[2] == ' ')
                        {
                            if(u8pipeSavConfBuf[1] == ' ')
                                u16pipeSavConfDat = u8pipeSavConfBuf[0] - '0';
                            else
                                u16pipeSavConfDat = (u8pipeSavConfBuf[0] - '0') * 10 + (u8pipeSavConfBuf[1] - '0');
                        }
                        else
                        {
                            u16pipeSavConfDat = (u8pipeSavConfBuf[0] - '0') * 100;
                            u16pipeSavConfDat += (u8pipeSavConfBuf[1] - '0') * 10;
                            u16pipeSavConfDat += (u8pipeSavConfBuf[2] - '0');
                        }
                        EepWrBuf[0] = u16pipeSavConfDat >> 8;
                        EepWrBuf[1] = u16pipeSavConfDat & 0xFF;
                        EepWr(234, 2);
                    }

                    for(tj = 0; tj < 5; tj++)
                        u8plzSavConfBuf[tj] = tmpplzSavConf[0][tj];
                    ConfChr2DatMd();
                    //tSavSetDat=u16SavSetDat;
                    if((u16plzSavConfDat <= plzSavConfDatMax[0]) && (u16plzSavConfDat >= plzSavConfDatMin[0]))
                    {   //数据OK!可以保存
                        plzSavConfDat[0] = u16plzSavConfDat;
                    }

                    for(tj = 0; tj < 5; tj++)
                        u8plzSavConfBuf[tj] = tmpplzSavConf[1][tj];
                    ConfChr2DatPlz();
                    //tSavSetDat=u16SavSetDat;
                    if((u16plzSavConfDat <= plzSavConfDatMax[1]) && (u16plzSavConfDat >= plzSavConfDatMin[1]))
                    {   //数据OK!可以保存
                        plzSavConfDat[1] = u16plzSavConfDat;
                    }
                    EepWrBuf[0] = plzSavConfDat[0] >> 8;
                    EepWrBuf[1] = plzSavConfDat[0] & 0x00FF;
                    EepWrBuf[2] = plzSavConfDat[1] >> 8;
                    EepWrBuf[3] = plzSavConfDat[1] & 0x00FF;
                    EepWr(220, 4);
                    //原先密度改成脉冲偏差
                    //if(WarnOnOff[9]==1)
                    //{dMD=(float)plzSavConfDat[0]/10000;}
                    rtPLerr = plzSavConfDat[0];
                    rtPLZk = (float)plzSavConfDat[1] / 1000;
                    SavMdWd();//SavMD();
                    if(rtPLZk < 0.01)
                    {
                        rtPLZk = 1.0;
                    }

//			EEP_Write(SavConfDat,194,11);//funcSavConfDat();
                    reNewSetDat();
                    Dis_Conf();
                    //FillConfTxd();
                    tConfDis[0] = 0x30 + (set_AddCa / 10);
                    tConfDis[1] = 0x30 + (set_AddCa % 10);
                    tConfDis[2] = '\0';
                    ShowStr30( 30 + 60, 0, tConfDis, 3, BLACK, GOLD);
//			flgCur=0;
//			for(ti=0;ti<6;ti++)
//			{	logUsr[0][ti]=' ';
//				logUsr[1][ti]=' ';
//			}
//			textPos=0;
//			curPos[0]=0;
//			curPos[1]=0;
//	 		frmNum=FRM_LOGIN;
                }
                else if(ConfPos == 1)
                {
                    SavConfDat[0] = 1;
                    SavConfDat[1] = 5;
                    SavConfDat[2] = 100;
                    SavConfDat[3] = 0;
                    SavConfDat[4] = 20;
                    SavConfDat[5] = 20;
                    SavConfDat[6] = 0;
                    SavConfDat[7] = 20;
                    SavConfDat[8] = 0;
                    SavConfDat[10] = sysY;
                    SavConfDat[11] = sysM;
                    SavConfDat[12] = sysD;
                    SavConfDat[13] = sysHH;
                    SavConfDat[14] = sysMM;
                    SavConfDat[15] = sysSS;
                    SavConfDat[16] = 0;
                    for(i = 0; i < 17; i++)
                        EepWrBuf[i] = SavConfDat[i];
                    EepWr(194, 17);

                    TFT_Fill(160, 20, 200, 44, GOLD);
                    if((set_Config & 0x40) == 0x40)
                    {
                        lcd_text24(160, 5, BLUE, GOLD, "远程");
                        //ShowHz(160,20,28);ShowHz(180,20,29);
                        sys_Conn = 1;
                        EepRd(209, 1);
                        EepWrBuf[0] = EepRdBuf[0] | 0x40;
                        EepWr(209, 1);
                    }
                    else
                    {
                        lcd_text24(160, 5, BLUE, GOLD, "本地");
                        //ShowHz(160,20,26);ShowHz(180,20,27);
                        sys_Conn = 0;
                        EepRd(209, 1);
                        EepWrBuf[0] = EepRdBuf[0] & 0xBF;
                        EepWr(209, 1);
                    }
                    reNewSetDat();

//			EEP_Write(SavConfDat,194,11);//funcSavConfDat();
                    Dis_Conf();
                    flgCur = 0;
                    for(ti = 0; ti < 6; ti++)
                    {   logUsr[0][ti] = ' ';
                        logUsr[1][ti] = ' ';
                    }
                    textPos = 0;
                    curPos[0] = 0;
                    curPos[1] = 0;
                    frmNum = FRM_LOGIN;
                    //FillFyTxd();
                }
                else if(ConfPos == 2)
                {
                    flgCur = 0;
                    for(ti = 0; ti < 6; ti++)
                    {   logUsr[0][ti] = ' ';
                        logUsr[1][ti] = ' ';
                    }
                    textPos = 0;
                    curPos[0] = 0;
                    curPos[1] = 0;
                    frmNum = FRM_LOGIN;
//				flg_EnSend=0;
//		 		Dis_Scr(frmNum);
//				flg_EnSend=1;
                }
            }
        }
    }
    else if(flgConfEdit == 1)
    {
//第一列数据处理
        if(ConfIndex == 0)
        {
            if((ubKey_Command < 0x4A) && (ubKey_Command > 0x3F))	//if((KeyBuf<0x4A)&&(KeyBuf>0x3F))
            {
                if(ConfPos < 10) //0~8为3位数
                {
                    if(ConfCur <= 2)
                    {
                        tmpSavConf[ConfPos][ConfCur] = (ubKey_Command & 0x0F) + 0x30;
                        ConfCur++;
                    }
                    for(ti = 0; ti < 3; ti++)
                    {
                        tConfDis[ti] = tmpSavConf[ConfPos][ti];
                    }
                    tConfDis[3] = '\0';
                }
                //TFT_Fill(220,62+ConfPos*35,220+24,62+ConfPos*35+16,WHITE);
                ClrConfSel(ConfIndex, ConfPos);
                WrConfSel(ConfIndex, ConfPos, tConfDis);
                //lcd_text16(220,62+ConfPos*35,0,0,tSetDis);
            }
        }
//第二列数据处理
        if(ConfIndex == 1)
        {
            if((ubKey_Command < 0x4A) && (ubKey_Command > 0x3F))	//if((KeyBuf<0x4A)&&(KeyBuf>0x3F))
            {
                if(ConfPos < 5) //0~4为2位数
                {
                    if(ConfCur <= 1)
                    {
                        tmpdnSavConf[ConfPos][ConfCur] = (ubKey_Command & 0x0F) + 0x30;
                        ConfCur++;
                    }
                    for(ti = 0; ti < 2; ti++)
                    {
                        tConfDis[ti] = tmpdnSavConf[ConfPos][ti];
                    }
                    tConfDis[2] = '\0';
                }

                if((ConfPos == 5) || (ConfPos == 6)) //5~6为6位数
                {
                    if(ConfCur <= 5)
                    {
                        tmpSavConfDate[ConfPos - 5][ConfCur] = (ubKey_Command & 0x0F) + 0x30;
                        ConfCur++;
                    }
                    for(ti = 0; ti < 6; ti++)
                    {
                        tConfDis[ti] = tmpSavConfDate[ConfPos - 5][ti];
                    }
                    tConfDis[6] = '\0';
                }
                if(ConfPos == 7)
                {
                    if(ConfCur <= 4)
                    {
                        tmpplzSavConf[ConfPos - 7][ConfCur] = (ubKey_Command & 0x0F) + 0x30;
                        ConfCur++;
                    }
                    for(ti = 0; ti < 5; ti++)
                    {
                        tConfDis[ti] = tmpplzSavConf[ConfPos - 7][ti];
                    }
                    tConfDis[5] = '\0';
                }
                if(ConfPos == 8)
                {
                    if(ConfCur <= 4)
                    {
                        tmpplzSavConf[ConfPos - 7][ConfCur] = (ubKey_Command & 0x0F) + 0x30;
                        ConfCur++;
                    }
                    for(ti = 0; ti < 5; ti++)
                    {
                        tConfDis[ti] = tmpplzSavConf[ConfPos - 7][ti];
                    }
                    tConfDis[5] = '\0';
                }
                if(ConfPos == 9)
                {
                    if(ConfCur <= 3)
                    {
                        tmppipeSavConf[ConfCur] = (ubKey_Command & 0x0F) + 0x30;
                        ConfCur++;
                    }
                    for(ti = 0; ti < 4; ti++)
                    {
                        tConfDis[ti] = tmppipeSavConf[ti];
                    }
                    tConfDis[3] = '\0';
                }

                //TFT_Fill(220,62+ConfPos*35,220+24,62+ConfPos*35+16,WHITE);
                ClrConfSel(ConfIndex, ConfPos);
                WrConfSel(ConfIndex, ConfPos, tConfDis);
                //lcd_text16(220,62+ConfPos*35,0,0,tSetDis);
            }
        }
        //编辑状态下回车
        if(ubKey_Command == KEY_ENTER) //if(KeyBuf==KEY_ENTER)
        {
            ubKey_Command = 0;
            flgConfEdit = 0;
            DisConfSel(ConfIndex, ConfPos, RED);
            if(ConfIndex == 2)	//组态参数下
            {
                if(tmpConfWarn[ConfPos] == 0)
                {
                    tmpConfWarn[ConfPos] = 1;
                    ClrConfSel(ConfIndex, ConfPos);
                    if(ConfPos != 10)
                        WrConfSel(ConfIndex, ConfPos, "Y");
                    else
                    {
                        WrConfSel(ConfIndex, ConfPos, "联网");
                    }
                }
                else
                {
                    tmpConfWarn[ConfPos] = 0;
                    ClrConfSel(ConfIndex, ConfPos);
                    if(ConfPos != 10)
                        WrConfSel(ConfIndex, ConfPos, "N");
                    else
                    {
                        WrConfSel(ConfIndex, ConfPos, "本地");
                    }
                }
            }
        }
    }
}

void Key_Pend(void)
{
    unsigned char i, j;
    unsigned char tPendNum;
    u8PendNum[0] = dGetPendNum(0);
    u8PendNum[1] = dGetPendNum(1); //u8PendNum=GetPendNum();
    tPendNum = u8PendNum[dualChNum - 1];
    if((ubKey_Command == KEY_RTN) || ((PendPos == 0) && (ubKey_Command == KEY_ENTER)))
    {
        ubKey_Command = 0;
        frmNum = FRM_MAIN;
        flgCur = 0;
        re_SFL_float(dSFLv);
        u8PendNum[0] = dGetPendNum(0);
        u8PendNum[1] = dGetPendNum(1); //u8PendNum=GetPendNum();
        //tPendNum=u8PendNum[dualChNum-1];
        if((u8PendNum[dualChNum - 1] > 0) && (sys_Conn == 1)) //if(0)//TRUCK
        {
            Dis_1_Pend(1);//Dis_Id_Pend(1);
        }
    }
    if(flgPendEdit == 0)
    {
        if(ubKey_Command == KEY_D_ARROW)
        {
            DisPendSel(PendPos, WHITE);
            if(PendPos < tPendNum)
                PendPos++;
            else
                PendPos = 0;
            DisPendSel(PendPos, RED);
        }
        if(ubKey_Command == KEY_U_ARROW)
        {
            DisPendSel(PendPos, WHITE);
            if(PendPos > 0)
                PendPos--;
            else
                PendPos = tPendNum;
            DisPendSel(PendPos, RED);
        }
        if(ubKey_Command == KEY_SET)
        {
            if(PendPos > 0)
            {
                flgPendEdit = 1;
                DisPendSel(PendPos, BLUE);
            }
        }
    }
    else if(flgPendEdit == 1)
    {
        if((ubKey_Command == KEY_ENTER) && (PendPos > 0))
        {
            ubKey_Command = 0;
            flgPendEdit = 0;
            for(i = 1; i < 25; i++)
            {
                PendBuf[PendPos - 1][i] = 0xFF;
            }
            for(i = (PendPos - 1); i < tPendNum; i++)
            {
                for(j = 1; j < 25; j++)
                    PendBuf[i][j] = PendBuf[i + 1][j];
            }
            PendBuf[tPendNum - 1][0] = 0xFF;
            for(i = tPendNum; i < 10; i++)
            {
                for(j = 0; j < 25; j++)
                    PendBuf[i][j] = 0xFF;
            }
            for(i = 0; i < 10; i++)
            {
                for(j = 0; j < 25; j++)
                {
                    EepWrBuf[j] = PendBuf[i][j];
                }
                if(dualChNum == 1) EepWr(PendAdd[i], 25);
                else EepWr(250 + PendAdd[i], 25);
            }
            for(i = 0; i < 8; i++)
            {
                chrCarId[dualChNum - 1][i] = ' ';
                chrOilId[dualChNum - 1][i] = ' ';
                chrYFL[dualChNum - 1][i] = ' ';
            }
            u8PendNum[0] = dGetPendNum(0);
            u8PendNum[1] = dGetPendNum(1); ////u8PendNum=GetPendNum();
            //tPendNum=u8PendNum[0];
            Dis_Pend(dualChNum - 1);
            DisPendSel(PendPos, WHITE);
            PendPos = PendPos - 1;
            DisPendSel(PendPos, RED);
        }
    }
}


void Key_Man(void)
{
//unsigned char ti;
    char disBuf[8];
    unsigned char ti;
//unsigned char tdat;
//unsigned char tempDat;
//        float tma;
//unsigned char tKeyC;
//unsigned int  tIntMa;
    char tManDis[10];
//unsigned int  tIntDat;

    if(flgManEdit == 0)
    {
        if(ubKey_Command == KEY_ENTER)
        {
            if(ManIndex == 0)
            {
                if((rioDat[2] & (1 << ManPos)) == (1 << ManPos))
                {
                    rio_DO = rioDat[2] & (~(1 << ManPos));
                }
                else
                {
                    rio_DO = rioDat[2] | ((1 << ManPos));
                }
                //DO手动测试
                //if((rtDoDat&(1<<ManPos))==(1<<ManPos))
//		if((rio_ub_DoDat&(1<<ManPos))==(1<<ManPos))
//		  FunChr=0xAA;
//		else
//			FunChr=0x55;
//	 FunDO =(1<<ManPos);
            }
        }
        if(ubKey_Command == KEY_RTN)
        {
            frmNum = FRM_LOGIN;
        }
        if(ubKey_Command == KEY_L_ARROW)
        {
            if(ManIndex > 0)
            {
                DisManSel(ManIndex, ManPos, WHITE);
                ManPos = 0;
                ManIndex--;
                DisManSel(ManIndex, ManPos, RED);
            }
        }
        if(ubKey_Command == KEY_R_ARROW)
        {
            if(ManIndex < 3)
            {
                DisManSel(ManIndex, ManPos, WHITE);
                ManPos = 0;
                ManIndex++;
                DisManSel(ManIndex, ManPos, RED);
            }
        }
        if(ubKey_Command == KEY_D_ARROW)
        {
            switch(ManIndex)
            {
            case 0:
                ti = 7;
                break;
            case 1:
                ti = 1;
                break;
            case 2:
                ti = 3;
                break;
            case 3:
                ti = 1;
                break;
            default:
                ti = 0;
                break;
            }
            if(ManPos < ti)
            {
                DisManSel(ManIndex, ManPos, WHITE);
                ManPos++;
                DisManSel(ManIndex, ManPos, RED);
            }
            if(ManIndex == 1)
            {
                if(ManAoDat < 80)			 ManAoDat = 80;
                if(AoDat < 80)				 AoDat = 80;
                if(ManAoDat > 4095)	 ManAoDat = 4095;
                if(AoDat > 4095)    		 AoDat = 4095;
                ManAoDat -= 5; //30;
                AoDat -= 5; //30;
                mAoDat = ManAoDat;
                rio_AoL1 = mAoDat & 0xFF;
                rio_AoH1 = mAoDat >> 8;
                rio_AoL2 = mAoDat & 0xFF;
                rio_AoH2 = mAoDat >> 8;
                //DAC_SetChannel1Data(DAC_Align_12b_R,AoDat);
                //TFT_Fill(276,250,400,266,WHITE);
                TFT_Fill(276, 250, 400, 266, WHITE);
                lcd_text16(560, 150, 0, WHITE, "Key_Up        ");
                disBuf[0] = mAoDat / 1000;
                disBuf[1] = (mAoDat - disBuf[0] * 1000) / 100;
                disBuf[2] = (mAoDat - disBuf[0] * 1000 - disBuf[1] * 100) / 10;
                disBuf[3] = (mAoDat - disBuf[0] * 1000 - disBuf[1] * 100 - disBuf[2] * 10);
                disBuf[4] = 0;
                disBuf[0] += '0';
                disBuf[1] += '0';
                disBuf[2] += '0';
                disBuf[3] += '0';
                //tma=0.5209*AoDat;
                //TFT_Fill(110,220,300,236,WHITE);
                lcd_text16(330, 75 + 0 * 40, 0, WHITE, disBuf);
            }
        }
        if(ubKey_Command == KEY_U_ARROW)
        {
            if(ManPos > 0)
            {
                DisManSel(ManIndex, ManPos, WHITE);
                ManPos--;
                DisManSel(ManIndex, ManPos, RED);
            }
            if(ManIndex == 1)
            {
                if(ManAoDat > 4060)	 ManAoDat = 4060;
                if(AoDat > 4060)    		 AoDat = 4060;
                if(ManAoDat < 80)			 ManAoDat = 80;
                if(AoDat < 80)				 AoDat = 80;
                ManAoDat += 5; //30;
                AoDat += 5; //30;
                mAoDat = ManAoDat;
                rio_AoL1 = mAoDat & 0xFF;
                rio_AoH1 = mAoDat >> 8;
                rio_AoL2 = mAoDat & 0xFF;
                rio_AoH2 = mAoDat >> 8;
                //DAC_SetChannel1Data(DAC_Align_12b_R,AoDat);
                //TFT_Fill(276,250,400,266,WHITE);
                TFT_Fill(276, 250, 400, 266, WHITE);
                lcd_text16(560, 150, 0, WHITE, "Key_Down      ");
                disBuf[0] = mAoDat / 1000;
                disBuf[1] = (mAoDat - disBuf[0] * 1000) / 100;
                disBuf[2] = (mAoDat - disBuf[0] * 1000 - disBuf[1] * 100) / 10;
                disBuf[3] = (mAoDat - disBuf[0] * 1000 - disBuf[1] * 100 - disBuf[2] * 10);
                disBuf[4] = 0;
                disBuf[0] += '0';
                disBuf[1] += '0';
                disBuf[2] += '0';
                disBuf[3] += '0';
                //tma=0.5209*AoDat;
                //TFT_Fill(110,220,300,236,WHITE);
                lcd_text16(330, 75 + 0 * 40, 0, WHITE, disBuf);
            }
        }

//if(tKeyC==KEY_ENTER)
//{
//	tKeyC=0;
//	KeyBuf=0;
//	if(ManIndex==0)
//	{
//		flgCur=0;
//		for(ti=0;ti<6;ti++)
//		{
//			logUsr[0][ti]=' ';
//			logUsr[1][ti]=' ';
//		}
//		textPos=0;
//		curPos[0]=0;
//		curPos[1]=0;
//		flgManCon=0;
// 		frmNum=FRM_LOGIN;
//	}
//}
        if(ubKey_Command == KEY_SET)
        {
            if(ManIndex == 2)
            {
                flgManEdit = 1;
                for(ti = 0; ti < 3; ti++)
                {
                    tmpSavMan[ManPos][ti] = ' ';
                    tManDis[ti] = tmpSavMan[ManPos][ti];
                }
                tManDis[3] = '\0';
                ClrManSel(ManIndex, ManPos);
                ManCur = 0;
                DisManSel(ManIndex, ManPos, BLUE);
            }
        }
        flgManCon = 1;
        //tKeyC=ubKey_Command;
        if(ubKey_Command == 0x41) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 1     ");
        }
        if(ubKey_Command == 0x42) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 2     ");
        }
        if(ubKey_Command == 0x43) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 3     ");
        }
        if(ubKey_Command == 0x4A) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num .     ");
        }
//		if(KeyBuf==0x11){TFT_Fill(276,250,400,266,WHITE);lcd_text16(276,250,0,0,"Key_Up");if(sys_mbAdd<15)sys_mbAdd++;}
//		if(KeyBuf==0x12){TFT_Fill(276,250,400,266,WHITE);lcd_text16(276,250,0,0,"Key_Dn");if(sys_mbAdd>1)sys_mbAdd--;}
//		if(KeyBuf==0x13){TFT_Fill(276,250,400,266,WHITE);lcd_text16(276,250,0,0,"Key_Left");}
//		if(KeyBuf==0x14){TFT_Fill(276,250,400,266,WHITE);lcd_text16(276,250,0,0,"Key_Right");}

        if(ubKey_Command == 0x44) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 4     ");
        }
        if(ubKey_Command == 0x45) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 5     ");
        }
        if(ubKey_Command == 0x46) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 6     ");
        }
        if(ubKey_Command == 0x40) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 0     ");
        }
        if(ubKey_Command == 0x21) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_SET       ");
        }
        if(ubKey_Command == 0x22) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Enter     ");
        }
        if(ubKey_Command == 0x23) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_END       ");
        }
        if(ubKey_Command == 0x24) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_DEL       ");
        }
        if(ubKey_Command == 0x25) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Rtn       ");
        }

        if(ubKey_Command == 0x47) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 7     ");
        }
        if(ubKey_Command == 0x48) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 8     ");
        }
        if(ubKey_Command == 0x49) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 9     ");
        }
        if(ubKey_Command == 0x4B) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_Num 000   ");
        }
        if(ubKey_Command == 0x31) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_START 1   ");
        }
        if(ubKey_Command == 0x32) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_STOP 1    ");
        }
        if(ubKey_Command == 0x33) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_START 2   ");
        }
        if(ubKey_Command == 0x34) {
            ubKey_Command = 0;
            TFT_Fill(276, 250, 400, 266, WHITE);
            lcd_text16(560, 150, 0, WHITE, "Key_STOP 2    ");
        }
        if(ubKey_Command)
            ubKey_Command = 0;
    }
    else if(flgManEdit == 1)
    {
        if(ManIndex == 2)
        {
            if((ubKey_Command < 0x4A) && (ubKey_Command > 0x3F)) //if((KeyBuf<0x4A)&&(KeyBuf>0x3F))
            {
                if(ManCur <= 2)
                {
                    tmpSavMan[ManPos][ManCur] = (ubKey_Command & 0x0F) + 0x30;
                    ManCur++;
                }
                for(ti = 0; ti < 3; ti++)
                {
                    tManDis[ti] = tmpSavMan[ManPos][ti];
                }
                tManDis[3] = '\0';
                //TFT_Fill(220,62+SetPos*35,220+24,62+SetPos*35+16,WHITE);
                ClrManSel(ManIndex, ManPos);
                WrManSel(ManIndex, ManPos, tManDis);
                //lcd_text16(220,62+SetPos*35,0,0,tSetDis);
            }
        }
        //编辑状态下回车
        if(ubKey_Command == KEY_ENTER) //if(KeyBuf==KEY_ENTER)
        {
            flgManEdit = 0;
            DisManSel(ManIndex, ManPos, RED);
        }
    }
}
void Key_AMS(void)
{
    if((sys_STA == 0) && (ubKey_Command == KEY_RTN))
    {
        //frmNum=FRM_MAIN;//frmNum=FRM_MON;
        frmNum = FRM_MON; //frmNum=FRM_MAIN;
    }

}
void Key_Main(void)
{
    unsigned char ti, i;
//主操作界面
    if((sys_STA == 0) && (ubKey_Command == KEY_RTN))
    {
        //frmNum=FRM_MAIN;//frmNum=FRM_MON;
        frmNum = FRM_MON; //frmNum=FRM_MAIN;
    }
    if(frmMainEdit == 0)
    {
        if(ubKey_Command == KEY_R_ARROW) //右键
        {
            if(frmMainCur < 2)
                frmMainCur++;
            if((sys_Conn == 1) && (frmMainCur == 1))
            {
                frmMainCur = 2;
            }
        }
        else if(ubKey_Command == KEY_L_ARROW) //左键
        {
            if(frmMainCur > 1) //原来是1，后来取消提货密码后变成2
                frmMainCur--;
            if((sys_Conn == 1) && (frmMainCur == 1))
            {
                frmMainCur = 2;
            }
        }
        else if(ubKey_Command == KEY_U_ARROW) //左键
        {
            if(frmMainCur > 2)
                frmMainCur--;
        }

        else if(ubKey_Command == KEY_D_ARROW) //左键
        {
            if((frmMainCur < 6) && (frmMainCur >= 2))
                frmMainCur++;
        }

        if((ubKey_Command == KEY_SET) && (sys_STA == 0) && (flgFyEn == 0))	 //先选好
        {
            if((frmMainCur == 1) && (sys_Conn == 0)) //石家庄取消了提货密码
            {
                frmMainEdit = 1;
                frmMainYFcur = 0;
                dYFLv = 0;
                for(ti = 0; ti < 9; ti++)
                {
                    frmMainYFval[ti] = ' ';
                    frmMainYFcur = 0;
                    ShowStr30(400, 56, frmMainYFval, 8, BLACK, WHITE);
                }
            }
        }
    }
    else
    {
//	if(KeyBuf==KEY_ENTER)
        if(ubKey_Command == KEY_ENTER)
        {
            frmMainEdit = 0;
            if(frmMainCur == 1)
            {   //手动输入应发量时，发油密码全为0
                for(ti = 0; ti < 8; ti++)
                {
                    frmMainOrdId[ti] = 0x30;
                    nowPendLst[dualChNum - 1][ti] = 0x30;
                }
                frmMainOrdId[8] = 0;
                nowPendLst[dualChNum - 1][8] = 0;
                for(ti = 0; ti < 4; ti++)
                {
                    nowOrdId[ti] = 0;
                }
                dYFLv = calYFL();
                dYFL[dualChNum - 1] = dYFLv;
                if(dYFLv > 0.0)
                {
                    for(ti = 0; ti < 4; ti++)
                    {
                        lstOilId[dualChNum - 1][ti] = (nowPendLst[dualChNum - 1][ti * 2] << 4) + (nowPendLst[dualChNum - 1][ti * 2 + 1] & 0x0F);
                        lstYFL[dualChNum - 1][ti] = nowYFval[ti];
                    }
                }
                ShowStr30(400, 56, frmMainYFval, 8, BLACK, WHITE);
                dSFLv = 0.0;
                canSFLv[dualChNum - 1] = dSFLv;
                lng_KgNeed = dYFLv;
                dLJSv_first = dLJSv;
                dLJSm_first = dLJSm;
            }
        }
        if(frmMainCur == 1)	 //应发量输入
        {
            if((ubKey_Command == 0x4B) && (frmMainYFcur <= 4))
            {
                frmMainYFval[frmMainYFcur] = 0x30;
                frmMainYFval[frmMainYFcur + 1] = 0x30;
                frmMainYFval[frmMainYFcur + 2] = 0x30;
                frmMainYFcur += 3;
                ShowStr30(400, 56, frmMainYFval, 8, BLACK, WHITE);
            }
            if(((ubKey_Command & 0x0F) < 0x0A) && ((ubKey_Command & 0xF0) == 0x40))
            {
                if(frmMainYFcur <= 7)
                {
                    frmMainYFval[frmMainYFcur] = (ubKey_Command & 0x0F) + 0x30;
                    frmMainYFcur++;
                    ShowStr30(400, 56, frmMainYFval, 8, BLACK, WHITE);
                }
            }
            if(ubKey_Command == KEY_DEL)
            {
                if(frmMainYFcur > 0)
                {
                    frmMainYFval[frmMainYFcur - 1] = ' ';
                    frmMainYFcur--;
                    ShowStr30(400, 56, frmMainYFval, 8, BLACK, WHITE);
                }
                else
                {
                    frmMainYFval[0] = ' ';
                    frmMainYFcur = 0;
                    ShowStr30(400, 56, frmMainYFval, 8, BLACK, WHITE);
                }
            }
        }
    }
    if(ubKey_Command == KEY_ENTER)
    {
        if(sys_STA == 0)
        {
            if(frmMainCur == 2) //待发记录
            {
                frmNum = FRM_PENDLST;
                ubKey_Command = 0;
                flgCur = 0;
            }
            if(frmMainCur == 3) //已发记录
            {
                frmNum = FRM_DONELST;
                ubKey_Command = 0;
                LstAllCnt = GetLstNum();
                LstDisCnt = 0;
                flgCur = 0;
            }
            if(frmMainCur == 4) //报警记录
            {
                frmNum = FRM_WARN;
                ubKey_Command = 0;
                flgCur = 0;
            }
            if(frmMainCur == 5) // A/M
            {
                //frmNum=FRM_AMS;
                //flgAMS=1;staMdPv=0;staMdSet=0;cntSend=1;
                //flgSetGet=0;
                frmNum = FRM_LOGIN;
                ubKey_Command = 0;
                flgAMT = 1;
                flgCur = 0;
                //FillMdSetMsg(1);
            }
//	if(frmMainCur==9)
//	{
//		sys_frmEdit=0;ubKey_Command=0;
//		flgCur=0;
//		for(ti=0;ti<6;ti++)
//		{	logUsr[0][ti]=' ';
//			logUsr[1][ti]=' ';
//		}
//		textPos=0;
//		curPos[0]=0;
//		curPos[1]=0;
// 		frmNum=FRM_LOGIN;
//	}
        }
        if(frmMainCur == 6) //副阀
        {
            ubKey_Command = 0;
            flgCur = 0;
            for(ti = 0; ti < 6; ti++)
            {   logUsr[0][ti] = ' ';
                logUsr[1][ti] = ' ';
            }
            textPos = 0;
            curPos[0] = 0;
            curPos[1] = 0;
            frmNum = FRM_LOGIN;
            //if((((rtDoDat&0x30)==0x20)&&(dualChNum==1))||(((rtDoDat&0xC0)==0x80)&&(dualChNum==2)))
            //{
            //	valv_pump=1;
            //	Draw_Lamp(750,360,1);
            //}
            //if((((rtDoDat&0x30)==0x10)&&(dualChNum==1))||(((rtDoDat&0xC0)==0x40)&&(dualChNum==2)))
            //{
            //	valv_pump=0;
            //	Draw_Lamp(750,360,0);
            //}
        }
    }
// 	if(frmNum==FRM_MAIN)

    switch(frmMainCur)
    {
    case 1://应发量
        if(sys_STA == 0)
        {
            if(frmMainEdit == 0)
                Draw_Sel(400 - 2, 58 - 4, 8 * 16 + 5, 32, RED);
            else
                Draw_Sel(400 - 2, 58 - 4, 8 * 16 + 5, 32, BLUE);
            //Draw_Sel(18,85,8*16+5,30,WHITE);
            //Draw_Sel(353,85,8*16+5,30,WHITE);
            //Draw_Sel(526,85,8*16+10,30,WHITE);
        }
        Draw_Btn("dfjl");
        Draw_Btn("yfjl");
        Draw_Btn("bjjl");
        Draw_Btn("AMS");
        Draw_Btn("czfs");
        //Draw_Sel(405-4,378-4,60+8,24+8,0xFFFF);
        break;
    case 2://待发记录
        Draw_Sel(400 - 2, 58 - 4, 8 * 16 + 5, 32, WHITE);
        Sel_Btn("dfjl");
        Draw_Btn("yfjl");
        Draw_Btn("bjjl");
        Draw_Btn("AMS");
        Draw_Btn("czfs");
        //Draw_Sel(405-4,378-4,60+8,24+8,0xFFFF);
        break;
    case 3://已发记录
        Draw_Sel(400 - 2, 58 - 4, 8 * 16 + 5, 32, WHITE);
        Draw_Btn("dfjl");
        Sel_Btn("yfjl");
        Draw_Btn("bjjl");
        Draw_Btn("AMS");
        Draw_Btn("czfs");
        //Draw_Sel(405-4,378-4,60+8,24+8,0xFFFF);

        break;
    case 4://报警记录
        Draw_Sel(400 - 2, 58 - 4, 8 * 16 + 5, 32, WHITE);
        Draw_Btn("dfjl");
        Draw_Btn("yfjl");
        Sel_Btn("bjjl");
        Draw_Btn("AMS");
        Draw_Btn("czfs");
        //Draw_Sel(405-4,378-4,60+8,24+8,0xFFFF);
        break;
    case 5://手自动
        Draw_Sel(400 - 2, 58 - 4, 8 * 16 + 5, 32, WHITE);
        Draw_Btn("dfjl");
        Draw_Btn("yfjl");
        Draw_Btn("bjjl");
        Sel_Btn("AMS");
        Draw_Btn("czfs");
        //Draw_Sel(405-4,378-4,60+8,24+8,0xFFFF);
        break;
    case 6://副阀
        Draw_Sel(400 - 2, 58 - 4, 8 * 16 + 5, 32, WHITE);
        Draw_Btn("dfjl");
        Draw_Btn("yfjl");
        Draw_Btn("bjjl");
        Draw_Btn("AMS");
        Sel_Btn("czfs");
        //Draw_Sel(405-4,378-4,60+8,24+8,0xFFFF);
        break;
//case 10://副阀
//Draw_Sel(400-2,52-2,8*16+5,30,WHITE);
//Draw_Btn("dfjl");
//Draw_Btn("yfjl");
//Draw_Btn("bjjl");
//Draw_Btn("AMS");
//Draw_Btn("czfs");
//Draw_Sel(405-4,378-4,60+8,24+8,RED);
//break;
    }
    /*空闲状态，不在登陆界面和手动界面，按下开始键发油*/
    if((frmNum != FRM_LOGIN) && (frmNum != FRM_SD))
    {
//	if((sys_STA==0)&&(frmNum==FRM_MAIN)&&(frmMainEdit==0))
//	{
//		if((ubKey_Command==KEY_DEL)&&(cntRfid==2)&&(UsrAvid==0))//删除提单
//		{
//				cntRfid=1;UsrAvid=0; ubKey_Command=0;
// 				for(i=0;i<8;i++)
//				{ frmMainYFval[i]=' ';frmMainOrdId[i]=' ';}
//				ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE); //小数点以后舍去
//				//ShowStr30(20,82,frmMainOrdId,8,BLACK,WHITE);
//				for(i=0;i<4;i++)
//				{
//				 nowOrdId[i]=0;lstYFL[dualChNum-1][i]=0;
//				}
//				dYFLv=0.0;//rtYFL=0;rtYFL1=0;rtYFL2=0;
//				Dis_CarId(0);//Dis_Pad(0);//Dis_Pad_Mini(0);
//		}
//	}
        if((sys_STA > 0) && (sys_STA < 6)) //发油过程中处理暂停
        {
            if(((dualChNum == 1) && (ubKey_Command == KEY_START1)) || (flgRmCon == 2) || ((dualChNum == 2) && (ubKey_Command == KEY_START2)))
            {
                if((sys_STA > 0) && (sys_STA < 6)) //正在发油
                {
                    flgUsrSp = 1;
                    ubKey_Command = 0;
                    flgRmCon = 0;
                    AoDat = 0;
                    //DAC_Out(AoDat);
                    sys_STA = 7;
                    flgSysWarn = 2;
                    flgAlert[6] = 1;
                    flgWrWarnLst = 1;
                    flgAlertDone = 1;
                    flgAlertRst = 0;
                    flgRmCon = 0;
                }

            }//发油过程中用户暂停
        }

        if((sys_STA == 8) || (sys_STA == 6)) //发油结束过程中
        {
            if(((dualChNum == 1) && (ubKey_Command == KEY_END1)) || ((dualChNum == 2) && (ubKey_Command == KEY_END2))) //结束发油
            {
                //sys_FyNeed=3;KeyCon=3;
                if((sys_STA != 0)) //发油结束
                {
                    ubKey_Command = 0;
                    valv_pump = 0; //close vice valve
                    //ClBump;ClFValv;
                    AoDat = 0;
                    //DAC_Out(AoDat);
                    //sys_STA=0;
                    //ClWarnBeep;
                    flgEnd = 1;
                    flgAlertDone = 0;
                    flgAlertRst = 0;
                    //for(i=0;i<8;i++)
                    //{ frmMainYFval[i]=' ';}
                    //ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE); //小数点以后舍去
                }
            }
        }
        if(sys_STA == 7) //发油暂停过程中
        {
            //if(flgAlert[6]==0)
            //{	 flgAlert[6]=1;		}
            if(flgRmCon == 3)
            {
                ubKey_Command = 0;
                valv_pump = 0; //close vice valve
                AoDat = 0;
                flgEnd = 1;
                sys_STA = 8;
                flgRmCon = 0;

            }
            if((flgAlertRst == 1) && (((dualChNum == 1) && (ubKey_Command == KEY_END1)) || ((dualChNum == 2) && (ubKey_Command == KEY_END2))))
            {
                ubKey_Command = 0;
                valv_pump = 0; //close vice valve
                AoDat = 0;
                flgEnd = 1;
                sys_STA = 8;
            }
            if((flgEnd == 0) && ((flgFyAllow == 1) || (flgRmCon == 1))) //复位后发油
            {
                if(((dualChNum == 1) && (ubKey_Command == KEY_START1)) || (flgRmCon == 1) || ((dualChNum == 2) && (ubKey_Command == KEY_START2)))
                {
                    flgUsrSp = 0;
                    flgAlertRst = 0;
                    ubKey_Command = 0;
                    flgRmCon = 0;
                    flgAlert[2] = 0;
                    flgAlert[3] = 0;
                    flgAlert[4] = 0;
                    flgAlert[5] = 0;
                    flgAlert[6] = 0;
                    flgFyEn = 1;
                    //if(WarnOnOff[6]==1)
                    //{
                    //	if(flPress>=Press_Up_Lmt)
                    //	{
                    //		flgFyEn=1;
                    //	}
                    //}
                    //else
                    //{
                    //	flgFyEn=1;
                    //}
                    if(WarnOnOff[8] == 1)	//flgAMS
                    {
                        flgAMS = 1;
                        if(dualChNum == 1) {
                            TFT_Fill(223, 423 + 20, 222 + 165, 448 + 20, GOLD);
                            lcd_text24(225, 422 + 20, BLUE, GOLD, "提取ＡＭＳ");
                        }
                        if(dualChNum == 2) {
                            TFT_Fill(623, 423 + 20, 622 + 165, 448 + 20, GOLD);
                            lcd_text24(625, 422 + 20, BLUE, GOLD, "提取ＡＭＳ");
                        }
                    }
                }
            }
            if((ubKey_Command == KEY_ENTER) || (flgRmCon == 1))
            {   //保存报警记录
                flgAlertRst = 1;
                ubKey_Command = 0;
                flgRmCon = 0;
                if(flgSysWarn == 1)
                    flgSysWarn = 2;
                if(err_FlowNo == 1)
                {
                    cntFVflow0 = 0;
                }
                if(err_FV == 1)
                {
                    cntFV2connTmr = 0;
                    cntFVconnTmr = 0;
                }
                if(err_Leak == 1)
                    cntClValvTmr = 0;
                //sys_Warn=0;
                if(sys_STA == 7)
                {
                    ubKey_Command = 0; //ClWarnBeep;
                }
            }
        }
        if((frmNum != FRM_MAN) && (frmNum != FRM_LOGIN))
        {
            if((sys_STA == 0) && (((dualChNum == 1) && (ubKey_Command == KEY_START1)) || (flgRmCon == 1) || ((dualChNum == 2) && (ubKey_Command == KEY_START2))))
            {
                //启动条件1,联网，应发量大于0
                //启动条件2,本地，应发量大于0
                //判断提油密码是否存入缓存
                //flgLstSent=0;
                //if(((UsrAvid==1)||(sys_Conn==0))&&(sys_STA==0)&&((flPress>=Press_Up_Lmt))&&(uwCmpAMS==0))
                ubKey_Command = 0;
                flgUsrSp = 0;
                flgAlertRst = 0;
                flgRmCon = 0;
                if(flgFyAllow == 1)
                {
                    for(i = 0; i < 9; i++)
                    {
                        if((frmMainOrdId[i] > 0x2F) && (frmMainOrdId[i] < 0x3A)) //不为空
                            nowPendLst[dualChNum - 1][i] = frmMainOrdId[i];
                        else
                            nowPendLst[dualChNum - 1][i] = 0;
                    }
                    for(i = 0; i < 4; i++)
                    {
                        nowOrdId[i] = (nowPendLst[dualChNum - 1][i * 2] << 4) + (nowPendLst[dualChNum - 1][i * 2 + 1] & 0x0F);
                        //nowYFval[ti]=(tBuf1[ti*2]<<4)+(tBuf1[ti*2+1]&0x0F);
                    }
                    dYFLv = calYFL();
                    dYFL[dualChNum - 1] = dYFLv;
                    if(dYFLv > 0)
                    {
                        //flgCalOvKg=0;//过冲量调整标志位
                        lstYFL[dualChNum - 1][0] = ((unsigned char)(frmMainYFval[0] << 4)) + (frmMainYFval[1] & 0x0F);
                        lstYFL[dualChNum - 1][1] = ((unsigned char)(frmMainYFval[2] << 4)) + (frmMainYFval[3] & 0x0F);
                        lstYFL[dualChNum - 1][2] = ((unsigned char)(frmMainYFval[4] << 4)) + (frmMainYFval[5] & 0x0F);
                        lstYFL[dualChNum - 1][3] = ((unsigned char)(frmMainYFval[6] << 4)) + (frmMainYFval[7] & 0x0F);
                        //存入开始时间
                        FyDateSt[0] = sysY;
                        FyDateSt[1] = sysM;
                        FyDateSt[2] = sysD;
                        FyDateSt[3] = sysHH;
                        FyDateSt[4] = sysMM;
                        FyDateSt[5] = sysSS;
                        /////ShowStr30(400,56,frmMainYFval,8,BLACK,WHITE);
                        dSFLv = 0.0; //rtSFL1=0.0;rtSFL2=0.0;
                        canSFLv[dualChNum - 1] = dSFLv;
                        lng_KgNeed = dYFLv;
                        //dLJS_first1=rtLJS1;rtLJS_first2=rtLJS2;
                        dLJSv_first = dLJSv;
                        dLJSm_first = dLJSm; //为了保存发油数据，以便断电
                        //表前数
                        flgSFLdis = 1;
                        re_SFL_float(dSFLv);
                        SavNewWorkDat();
                        //rdLJS=0.0;dSSLv_pls=0.0;dSSLv_pls_first=0.0;
                        flgFyEn = 1;
                        flgKeySt = 1;
                        if(WarnOnOff[8] == 1)
                        {
                            flgAMS = 1;
                            if(dualChNum == 1) {
                                TFT_Fill(223, 423 + 20, 222 + 165, 448 + 20, GOLD);
                                lcd_text24(225, 422 + 20, BLUE, GOLD, "提取ＡＭＳ");
                            }
                            if(dualChNum == 2) {
                                TFT_Fill(623, 423 + 20, 622 + 165, 448 + 20, GOLD);
                                lcd_text24(625, 422 + 20, BLUE, GOLD, "提取ＡＭＳ");
                            }
                        }
                    }
                }
            }
        }
    }

    ubKey_Command = 0;

}

void SavUsrPwd(void)
{
    char tUsr[34];
    char tPwd[50];
    unsigned char supi;
    unsigned char supj;
    unsigned char ti;

    for(supi = 0; supi < 8; supi++)
        for(supj = 0; supj < 4; supj++)
            tUsr[supi * 4 + supj] = SavUsr[supi][supj];
    for(supi = 0; supi < 8; supi++)
        for(supj = 0; supj < 6; supj++)
            tPwd[supi * 6 + supj] = SavPwd[supi][supj];

    for(ti = 0; ti < 32; ti++)
        EepWrBuf[ti] = tUsr[ti];
    EepWr(2, 32); //用户名
    for(ti = 0; ti < 48; ti++)
        EepWrBuf[ti] = tPwd[ti];
    EepWr(66, 48); //密码
}

void Key_Pwd(void)
{
    unsigned char ti;
    char tDisChr[10];
    unsigned char tj;
    if((sys_STA == 0) && (ubKey_Command == KEY_RTN))
    {
        frmNum = FRM_LOGIN;
    }

    if(flgPwdEdit == 0)
    {
//	if(KeyBuf==KEY_R_ARROW)
        if(ubKey_Command == KEY_R_ARROW)
        {
            if(IndexId < 3)
            {
                DisPwdSel(UsrPos, PwdPos, WHITE);
                if(IndexId == 0)
                    PwdPos = UsrPos;
                IndexId++;
                DisPwdSel(UsrPos, PwdPos, RED);
            }
        }
//	if(KeyBuf==KEY_L_ARROW)
        if(ubKey_Command == KEY_L_ARROW)
        {
            if(IndexId > 0)
            {
                DisPwdSel(UsrPos, PwdPos, WHITE);
                if(IndexId == 1)
                    UsrPos = PwdPos;
                IndexId--;
                DisPwdSel(UsrPos, PwdPos, RED);
            }
        }

//	if(KeyBuf==KEY_U_ARROW)
        if(ubKey_Command == KEY_U_ARROW)
        {
            if(IndexId == 0)
            {
                if(UsrPos > 0)
                {
                    DisPwdSel(UsrPos, PwdPos, WHITE);
                    UsrPos--;
                    DisPwdSel(UsrPos, PwdPos, RED);
                }
            }
            if(IndexId == 1)
            {
                if(PwdPos > 0)
                {
                    DisPwdSel(UsrPos, PwdPos, WHITE);
                    PwdPos--;
                    DisPwdSel(UsrPos, PwdPos, RED);
                }
            }
        }

//	if(KeyBuf==KEY_D_ARROW)
        if(ubKey_Command == KEY_D_ARROW)
        {
            if(IndexId == 0)
            {
                if(UsrPos < 7)
                {
                    DisPwdSel(UsrPos, PwdPos, WHITE);
                    UsrPos++;
                    DisPwdSel(UsrPos, PwdPos, RED);
                }
            }
            if(IndexId == 1)
            {
                if(PwdPos < 7)
                {
                    DisPwdSel(UsrPos, PwdPos, WHITE);
                    PwdPos++;
                    DisPwdSel(UsrPos, PwdPos, RED);
                }
            }
        }
//		if(KeyBuf==KEY_SET)
        if(ubKey_Command == KEY_SET)
        {
            if(IndexId == 0)
            {
                flgPwdEdit = 1;
                DisPwdSel(UsrPos, PwdPos, BLUE);
                PwdCur = 0;
                for(ti = 0; ti < 4; ti++)
                {
                    tmpSavUsr[UsrPos][ti] = ' ';
                    tDisChr[ti] = tmpSavUsr[UsrPos][ti];
                }
                //ShowStr30(152,76+UsrPos*40,tDisChr,4,BLACK,WHITE);
                lcd_text24(152, 85 + PwdPos * 40, BLACK, WHITE, tDisChr); //GREEN
            }
            if(IndexId == 1)
            {
                flgPwdEdit = 1;
                DisPwdSel(UsrPos, PwdPos, BLUE);
                PwdCur = 0;
                for(ti = 0; ti < 6; ti++)
                {
                    tmpSavPwd[PwdPos][ti] = ' ';
                    tDisChr[ti] = tmpSavPwd[PwdPos][ti];
                }
                //ShowStr30(302,76+PwdPos*40,tDisChr,6,BLACK,WHITE);
                lcd_text24(302, 85 + PwdPos * 40, BLACK, WHITE, tDisChr); //GREEN
            }

        }
        //非编辑状态下回车
//	if(KeyBuf==KEY_ENTER)
        if(ubKey_Command == KEY_ENTER)
        {
            ubKey_Command = 0;
            if(IndexId == 2)
            {
                for(ti = 0; ti < 8; ti++)
                {
                    for(tj = 0; tj < 4; tj++)
                        SavUsr[ti][tj] = tmpSavUsr[ti][tj];
                    SavUsr[ti][4] = '\0';
                }
                for(ti = 0; ti < 8; ti++)
                {
                    for(tj = 0; tj < 6; tj++)
                        SavPwd[ti][tj] = tmpSavPwd[ti][tj];
                    SavPwd[ti][6] = '\0';
                }

                SavUsrPwd();
                ReadUsrPwd();
            }
            if(IndexId == 3)
            {
                flgCur = 0;
                for(ti = 0; ti < 6; ti++)
                {   logUsr[0][ti] = ' ';
                    logUsr[1][ti] = ' ';
                }
                textPos = 0;
                curPos[0] = 0;
                curPos[1] = 0;
                frmNum = FRM_LOGIN;
//			flg_EnSend=0;
//	 		Dis_Scr(frmNum);
//			flg_EnSend=1;
            }

        }

    }//if(flgPwdEdit==0)
    else
    {
        //编辑状态下的回车
// if(KeyBuf==KEY_ENTER)
        if(ubKey_Command == KEY_ENTER)
        {
            ubKey_Command = 0;
            flgPwdEdit = 0;
            DisPwdSel(UsrPos, PwdPos, RED);
            PwdCur = 0;
        }
// if(KeyBuf==KEY_DEL)
        if(ubKey_Command == KEY_DEL)
        {
            if(IndexId == 0)
            {
                if((PwdCur < 5) && (PwdCur > 0))
                {
                    tmpSavUsr[UsrPos][PwdCur - 1] = ' ';
                    PwdCur--;
                }
                for(ti = 0; ti < 4; ti++)
                {
                    tDisChr[ti] = tmpSavUsr[UsrPos][ti];
                }
                //ShowStr30(152,80+UsrPos*40,tDisChr,4,BLACK,WHITE);
                lcd_text24(152, 85 + UsrPos * 40, BLACK, WHITE, tDisChr); //GREEN
            }
            if(IndexId == 1)
            {
                if((PwdCur < 7) && (PwdCur > 0))
                {
                    tmpSavPwd[PwdPos][PwdCur - 1] = ' ';
                    PwdCur--;
                }
                for(ti = 0; ti < 6; ti++)
                {
                    tDisChr[ti] = tmpSavPwd[PwdPos][ti];
                }
                //ShowStr30(302,80+PwdPos*40,tDisChr,6,BLACK,WHITE);
                lcd_text24(302, 85 + PwdPos * 40, BLACK, WHITE, tDisChr); //GREEN
            }

        }
//数字输入
// if((KeyBuf<0x4A)&&(KeyBuf>0x3F))
        if((ubKey_Command < 0x4A) && (ubKey_Command > 0x3F))
        {
            if(IndexId == 0)
            {
                if(PwdCur < 4)
                {
//			tmpSavUsr[UsrPos][PwdCur]=(KeyBuf&0x0F)+0x30;
                    tmpSavUsr[UsrPos][PwdCur] = (ubKey_Command & 0x0F) + 0x30;
                    PwdCur++;
                    for(ti = 0; ti < 4; ti++)
                    {
                        tDisChr[ti] = tmpSavUsr[UsrPos][ti];
                    }
                    //ShowStr30(152,80+UsrPos*40,tDisChr,4,BLACK,WHITE);
                    lcd_text24(152, 85 + UsrPos * 40, BLACK, WHITE, tDisChr); //GREEN
                }
            }
            if(IndexId == 1)
            {
                if(PwdCur < 6)
                {
//			tmpSavPwd[PwdPos][PwdCur]=(KeyBuf&0x0F)+0x30;
                    tmpSavPwd[PwdPos][PwdCur] = (ubKey_Command & 0x0F) + 0x30;
                    PwdCur++;
                    for(ti = 0; ti < 6; ti++)
                    {
                        tDisChr[ti] = tmpSavPwd[PwdPos][ti];
                    }
                    //ShowStr30(302,80+PwdPos*40,tDisChr,6,BLACK,WHITE);
                    lcd_text24(302, 85 + PwdPos * 40, BLACK, WHITE, tDisChr); //GREEN
                }
            }
        }
    }
}

void GetUsrID(char *tUsrID)
{
    unsigned char ti;
    for(ti = 0; ti < 4; ti++)
        LogUsrID[ti] = tUsrID[ti];
}
//读用户名密码，如果读数不为数字，用缺省值代替
void ReadUsrPwd(void)
{
    unsigned char supi;
    unsigned char supj;
    EepRd(2, 32);
    for(supi = 0; supi < 8; supi++)
        for(supj = 0; supj < 4; supj++)
            SavUsr[supi][supj] = EepRdBuf[supi * 4 + supj];
    EepRd(66, 48);
    for(supi = 0; supi < 8; supi++)
        for(supj = 0; supj < 6; supj++)
            SavPwd[supi][supj] = EepRdBuf[(supi * 6) + supj];
}

void Dis_CarId(unsigned char id)
{
    char tBuf[10];
    if(frmNum == FRM_MAIN)
    {
        TFT_Fill(240 + 93, 354, 395 + 93, 383, WHITE);
        if(id == 1)
            Draw_EN24(245 + 93, 355, 0, 0, chrCarId[dualChNum - 1]);
    }

}

/*
写一条新记录，高8位为位置号，低8位为记录号
*/
void Wr_New_Lst(unsigned int tIndex)
{
//	char tBuf[64];
//	unsigned char tCrc;
    unsigned int  i;
    unsigned int  tWrLos;//最后一次写的位置号
    unsigned int t_uwLos;//位置号,0x340+t_u8Los*64;
//	unsigned int t_uwIndex;//索引号,1~210;在0~200条记录中循环使用
    //step1: check the wr bank is all empty
    t_uwLos = (unsigned char)(tIndex >> 8);
    t_uwLos++;
    if(t_uwLos >= 200)
        t_uwLos = 0;
    tWrLos = 0x340 + t_uwLos * 80; //(t_uwLos<<7);//(位置号=0x340+t_uwLos*64)
    for(i = 0; i < 80; i++)
        EepWrBuf[i] = LstFyBuf[i];
    EepWr(tWrLos, 80);
    //return 0;
}

unsigned long cal_YFL(char tBuf[9])//只计算应发量
{
    unsigned long tyfl;
    unsigned char ti, tj;

    for(ti = 7; ti > 0; ti--)
    {
        if(tBuf[7] == ' ')
        {
            for(tj = 7; tj > (7 - ti); tj--)
            {
                tBuf[tj] = tBuf[tj - 1];
            }
            tBuf[7 - ti] = ' ';
        }
    }

    for(ti = 0; ti < 8; ti++)
    {
        if((tBuf[ti] < '0') || (tBuf[ti] > '9'))
            tBuf[ti] = 0;
        else
            tBuf[ti] = tBuf[ti] - '0';
    }
    tyfl = tBuf[0] * 10000000;
    tyfl += tBuf[1] * 1000000;
    tyfl += tBuf[2] * 100000;
    tyfl += tBuf[3] * 10000;
    tyfl += tBuf[4] * 1000;
    tyfl += tBuf[5] * 100;
    tyfl += tBuf[6] * 10;
    tyfl += tBuf[7];
    return tyfl;
}

unsigned long calYFL(void)
{
    unsigned long tyfl;
    unsigned char ti, tj;
    unsigned char tBuf[8];

    for(ti = 7; ti > 0; ti--)
    {
        if(frmMainYFval[7] == ' ')
        {
            for(tj = 7; tj > (7 - ti); tj--)
            {
                frmMainYFval[tj] = frmMainYFval[tj - 1];
            }
            frmMainYFval[7 - ti] = ' ';
        }
    }

    for(ti = 0; ti < 8; ti++)
    {
        if((frmMainYFval[ti] < '0') || (frmMainYFval[ti] > '9'))
            tBuf[ti] = 0;
        else
            tBuf[ti] = frmMainYFval[ti] - '0';
    }
    tyfl = tBuf[0] * 10000000;
    tyfl += tBuf[1] * 1000000;
    tyfl += tBuf[2] * 100000;
    tyfl += tBuf[3] * 10000;
    tyfl += tBuf[4] * 1000;
    tyfl += tBuf[5] * 100;
    tyfl += tBuf[6] * 10;
    tyfl += tBuf[7];
    if(tyfl > 0)
    {
        ftoc(tyfl);
        //mbdata[1]=(c2f[3]<<8)+c2f[2];
        //mbdata[0]=(c2f[1]<<8)+c2f[0];
        //mbBuf[0]=mbdata[0];
        //mbBuf[1]=mbdata[1];
        //mbdata[2]=0;mbdata[3]=0;mbdata[4]=0;mbdata[5]=0;
        //mbBuf[2]=mbdata[2];mbBuf[3]=mbdata[3];
        //mbBuf[4]=mbdata[4];mbBuf[5]=mbdata[5];
        YFL[dualChNum - 1] = tyfl;
        for(ti = 0; ti < 8; ti++)
        {
            chrOilId[dualChNum - 1][ti] = frmMainOrdId[ti];
            chrYFL[dualChNum - 1][ti] = frmMainYFval[ti];
        }
    }
    return tyfl;
}

void getWarnOnOff(void)
{
    unsigned char tCfg;
    EepRd(210, 1);
    tCfg = EepRdBuf[0];
    WarnOnOff[0] = (tCfg & 0x20) >> 5; //静电
    WarnOnOff[1] = (tCfg & 0x10) >> 4; //溢流
    WarnOnOff[2] = (tCfg & 0x08) >> 3; //流量计
    WarnOnOff[3] = (tCfg & 0x04) >> 2; //无流量
    WarnOnOff[4] = (tCfg & 0x02) >> 1; //泄漏
    WarnOnOff[5] = (tCfg & 0x01); //密度	[汽化]
    EepRd(230, 1);
    tCfg = EepRdBuf[0];
    WarnOnOff[6] = (tCfg & 0x80) >> 7; //低流量
    WarnOnOff[7] = (tCfg & 0x40) >> 6; //低压力
    WarnOnOff[8] = (tCfg & 0x20) >> 5; //AMS
    WarnOnOff[9] = (tCfg & 0x10) >> 4; //自调整
}


