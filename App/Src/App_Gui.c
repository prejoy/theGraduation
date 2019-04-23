/*
 * App_Gui.c
 *
 *  Created on: 2019��3��30��
 *      Author: prejoy
 */

#include "stdio.h"
#include "App_Gui.h"
#include "devLtdc.h"
#include "text.h"
#include "App_Pay.h"

void GuiPageCostInit(void)
{
  //=============             user page        ==========================
   char lcd_id[12];
//   sprintf((char*)lcd_id,"LCD ID:7084");
//   printf("%s\r\n",lcd_id);
   LTDC_Clear(BG_COLOR);

   LTDC_Fill(PIC_SX,PIC_SY,PIC_SX+PIC_WIDTH,PIC_SY+PIC_HEIGHT,WHITE);
   POINT_COLOR=BLACK;BACK_COLOR=BG_COLOR;
   Show_Str(MSGn_SX(1),MSGn_SY(1),480-MSG_SX,MSG_FONT,"���ţ�",MSG_FONT,0);
   Show_Str(MSGn_SX(2),MSGn_SY(2),480-MSG_SX,MSG_FONT,"������",MSG_FONT,0);
   Show_Str(MSGn_SX(3),MSGn_SY(3),480-MSG_SX,MSG_FONT,"�꼶��",MSG_FONT,0);
   Show_Str(MSGn_SX(4),MSGn_SY(4),480-MSG_SX,MSG_FONT,"רҵ��",MSG_FONT,0);
   Show_Str(MSGn_SX(5),MSGn_SY(5),480-MSG_SX,MSG_FONT,"��          Ԫ",MSG_FONT,0);
   Show_Str(30,280,480-30,32,"���������ѽ�      Ԫ",32,0);

   //�������� costʱ����������������bug
//   POINT_COLOR=BLACK;BACK_COLOR=GRAY;
//   Show_Str(120-24*2,(PIC_SY+PIC_HEIGHT+504)/2,24*4,24,"ɨ��֧��",24,0);
//   Show_Str(360-24*2,(PIC_SY+PIC_HEIGHT+504)/2,24*4,24,"ˢ��֧��",24,0);

   //===================    softkbd  =================
    LTDC_Fill(0,800-1-296,480-1,800-1,SOFT_KBD_FILLCOLOR);				//fill������ϵ��£�����
    LTDC_DrawLine(0,800-1-296,480-1,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);		//horizon
    LTDC_DrawLine(0,800-1-296+74*1,480-1,800-1-296+74*1,LINE_WIDTH,SOFT_KBD_LINECOLOR);
    LTDC_DrawLine(0,800-1-296+74*2,480-1,800-1-296+74*2,LINE_WIDTH,SOFT_KBD_LINECOLOR);
    LTDC_DrawLine(0,800-1-296+74*3,480-1,800-1-296+74*3,LINE_WIDTH,SOFT_KBD_LINECOLOR);
    LTDC_DrawLine(0+120,800-1,0+120,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);		//vercital
    LTDC_DrawLine(0+120*2,800-1,0+120*2,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);
    LTDC_DrawLine(0+120*3,800-1,0+120*3,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);

    POINT_COLOR=BLACK;BACK_COLOR=WHITE;
    Show_Str((XPOSI(1)-FONTSIZE/4),(YPOSI(1)-FONTSIZE/2),FONTSIZE,FONTSIZE,"1",FONTSIZE,0);
    Show_Str((XPOSI(2)-FONTSIZE/4),(YPOSI(2)-FONTSIZE/2),FONTSIZE,FONTSIZE,"2",FONTSIZE,0);
    Show_Str((XPOSI(3)-FONTSIZE/4),(YPOSI(3)-FONTSIZE/2),FONTSIZE,FONTSIZE,"3",FONTSIZE,0);
    Show_Str((XPOSI(5)-FONTSIZE/4),(YPOSI(5)-FONTSIZE/2),FONTSIZE,FONTSIZE,"4",FONTSIZE,0);
    Show_Str((XPOSI(6)-FONTSIZE/4),(YPOSI(6)-FONTSIZE/2),FONTSIZE,FONTSIZE,"5",FONTSIZE,0);
    Show_Str((XPOSI(7)-FONTSIZE/4),(YPOSI(7)-FONTSIZE/2),FONTSIZE,FONTSIZE,"6",FONTSIZE,0);
    Show_Str((XPOSI(9)-FONTSIZE/4),(YPOSI(9)-FONTSIZE/2),FONTSIZE,FONTSIZE,"7",FONTSIZE,0);
    Show_Str((XPOSI(10)-FONTSIZE/4),(YPOSI(10)-FONTSIZE/2),FONTSIZE,FONTSIZE,"8",FONTSIZE,0);
    Show_Str((XPOSI(11)-FONTSIZE/4),(YPOSI(11)-FONTSIZE/2),FONTSIZE,FONTSIZE,"9",FONTSIZE,0);
    Show_Str((XPOSI(13)-FONTSIZE/4),(YPOSI(13)-FONTSIZE/2),FONTSIZE,FONTSIZE,"0",FONTSIZE,0);
    Show_Str((XPOSI(14)-FONTSIZE/4),(YPOSI(14)-FONTSIZE/2),FONTSIZE,FONTSIZE,".",FONTSIZE,0);
    POINT_COLOR=BROWN;
    Show_Str((XPOSI(4)-FONTSIZE),(YPOSI(4)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"��ѯ",FONTSIZE,0);
    Show_Str((XPOSI(8)-FONTSIZE),(YPOSI(8)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"����",FONTSIZE,0);
    Show_Str((XPOSI(12)-FONTSIZE),(YPOSI(12)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"ȡ��",FONTSIZE,0);
    Show_Str((XPOSI(15)-FONTSIZE/2),(YPOSI(15)-FONTSIZE/2),FONTSIZE,FONTSIZE,"��",FONTSIZE,0);
    Show_Str((XPOSI(16)-FONTSIZE),(YPOSI(16)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"ȷ��",FONTSIZE,0);
}


void DisplayCostMoney(uint8_t *CostMoney)
{
//  char str[6]={0};
//  uint32_t Integer,Decimal;
//  Integer = CostMoney/10;
//  Decimal = CostMoney%10;
  POINT_COLOR=RED;BACK_COLOR=BG_COLOR;
//  sprintf(str,"%d.%d",Integer,Decimal);
  Show_Str(280,280,480-40,32,CostMoney,32,0);
}

void ClearCostMoney()
{
    char str[6]="      ";
  POINT_COLOR=BG_COLOR;BACK_COLOR=BG_COLOR;
  Show_Str(280,280,480-50,32,str,32,0);
}

void DispalyCardRemain(uint8_t *RemainMoney)
{
  POINT_COLOR=RED;BACK_COLOR=BG_COLOR;
  Show_Str(MSGn_SX(5)+24*3,MSGn_SY(5),480-40,24,RemainMoney,24,0);
}

void clearCardRemain()
{
  char str[6]="      ";
  POINT_COLOR=BG_COLOR;BACK_COLOR=BG_COLOR;
  Show_Str(MSGn_SX(5)+24*3,MSGn_SY(5),480-40,24,str,24,0);
}

//����4����ע����в����仯
void DisplayLCDCosRes(uint32_t isSuccess,uint32_t errtype,uint32_t disp,uint32_t txtcolor)
{
#define xCenter	240
#define yCenter	440
  if (isSuccess == 0)	//success
    {
      if (disp == 1)
	{
	  if (txtcolor == 0)
	    {
	      POINT_COLOR=BLACK;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "֧���ɹ�", 32, 0);
	    }
	  else
	    {
	      POINT_COLOR=WHITE;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "֧���ɹ�", 32, 0);
	    }
	}
      else
	{
	      POINT_COLOR=BG_COLOR;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "        ", 32, 0);
	}
    }
  else
    {
      if(disp == 1)
	{
	  switch(errtype)
	  {
	    case ERR_NOSUCHID:
	      POINT_COLOR=(WHITE)*txtcolor;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "��Ч�û�", 32, 0);
	      break;
	    case ERR_BALANCECHARGE:
	      POINT_COLOR=(WHITE)*txtcolor;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "����", 32, 0);
	      break;
	    default:
	      POINT_COLOR=(WHITE)*txtcolor;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "֧��ʧ��", 32, 0);
	      break;
	  }
	}
      else
	{
	      POINT_COLOR=BG_COLOR;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "        ", 32, 0);
	}
    }
#undef xCenter
#undef yCenter
}


void GuiPageSettingInit(void)
{
#define Step	48
#define Font_Setting	32
#define xStart	30
#define yStart	60
  LTDC_Clear(BG_COLOR);
  POINT_COLOR=BLACK;BACK_COLOR=GRAY;
  Show_Str(240-32*2,10,32*4,32,"������ҳ",Font_Setting,0);
  Show_Str(xStart,yStart,32*4,32,"ע�ῨƬ",Font_Setting,0);
  Show_Str(xStart,yStart+Step,32*4,32,"��Ƭ��ֵ",Font_Setting,0);
  Show_Str(xStart,yStart+Step*2,32*4,32,"��������",Font_Setting,0);
//#undef yStart
//#undef xStart
//#undef Font_Setting
//#undef Step
}

void GuiPageSettingInput(void)
{
  POINT_COLOR=BLACK;BACK_COLOR=BG_COLOR;
  Show_Str(240-9*16,800-1-296-40,480-30,32,"��ֵ��      Ԫ",32,0);
  //===================    softkbd  =================
   LTDC_Fill(0,800-1-296,480-1,800-1,SOFT_KBD_FILLCOLOR);				//fill������ϵ��£�����
   LTDC_DrawLine(0,800-1-296,480-1,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);		//horizon
   LTDC_DrawLine(0,800-1-296+74*1,480-1,800-1-296+74*1,LINE_WIDTH,SOFT_KBD_LINECOLOR);
   LTDC_DrawLine(0,800-1-296+74*2,480-1,800-1-296+74*2,LINE_WIDTH,SOFT_KBD_LINECOLOR);
   LTDC_DrawLine(0,800-1-296+74*3,480-1,800-1-296+74*3,LINE_WIDTH,SOFT_KBD_LINECOLOR);
   LTDC_DrawLine(0+120,800-1,0+120,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);		//vercital
   LTDC_DrawLine(0+120*2,800-1,0+120*2,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);
   LTDC_DrawLine(0+120*3,800-1,0+120*3,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);

   POINT_COLOR=BLACK;BACK_COLOR=WHITE;
   Show_Str((XPOSI(1)-FONTSIZE/4),(YPOSI(1)-FONTSIZE/2),FONTSIZE,FONTSIZE,"1",FONTSIZE,0);
   Show_Str((XPOSI(2)-FONTSIZE/4),(YPOSI(2)-FONTSIZE/2),FONTSIZE,FONTSIZE,"2",FONTSIZE,0);
   Show_Str((XPOSI(3)-FONTSIZE/4),(YPOSI(3)-FONTSIZE/2),FONTSIZE,FONTSIZE,"3",FONTSIZE,0);
   Show_Str((XPOSI(5)-FONTSIZE/4),(YPOSI(5)-FONTSIZE/2),FONTSIZE,FONTSIZE,"4",FONTSIZE,0);
   Show_Str((XPOSI(6)-FONTSIZE/4),(YPOSI(6)-FONTSIZE/2),FONTSIZE,FONTSIZE,"5",FONTSIZE,0);
   Show_Str((XPOSI(7)-FONTSIZE/4),(YPOSI(7)-FONTSIZE/2),FONTSIZE,FONTSIZE,"6",FONTSIZE,0);
   Show_Str((XPOSI(9)-FONTSIZE/4),(YPOSI(9)-FONTSIZE/2),FONTSIZE,FONTSIZE,"7",FONTSIZE,0);
   Show_Str((XPOSI(10)-FONTSIZE/4),(YPOSI(10)-FONTSIZE/2),FONTSIZE,FONTSIZE,"8",FONTSIZE,0);
   Show_Str((XPOSI(11)-FONTSIZE/4),(YPOSI(11)-FONTSIZE/2),FONTSIZE,FONTSIZE,"9",FONTSIZE,0);
   Show_Str((XPOSI(13)-FONTSIZE/4),(YPOSI(13)-FONTSIZE/2),FONTSIZE,FONTSIZE,"0",FONTSIZE,0);
   Show_Str((XPOSI(14)-FONTSIZE/4),(YPOSI(14)-FONTSIZE/2),FONTSIZE,FONTSIZE,".",FONTSIZE,0);
   POINT_COLOR=BROWN;
//   Show_Str((XPOSI(4)-FONTSIZE),(YPOSI(4)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"��ѯ",FONTSIZE,0);
//   Show_Str((XPOSI(8)-FONTSIZE),(YPOSI(8)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"����",FONTSIZE,0);
   Show_Str((XPOSI(12)-FONTSIZE),(YPOSI(12)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"ȡ��",FONTSIZE,0);
   Show_Str((XPOSI(15)-FONTSIZE/2),(YPOSI(15)-FONTSIZE/2),FONTSIZE,FONTSIZE,"��",FONTSIZE,0);
   Show_Str((XPOSI(16)-FONTSIZE),(YPOSI(16)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"ȷ��",FONTSIZE,0);
}

void GuiPageSettingChargeAmount(uint8_t *ChargeMoney)
{
  char strtoshow[6]={0};
  uint8_t i=0;
  POINT_COLOR=BG_COLOR;BACK_COLOR=BG_COLOR;
  Show_Str(240-9*16+5*32,800-1-296-40,480-30,32,"      ",32,0);
  for(;ChargeMoney[i]!='\0' && i<6;i++)
    {
      if(ChargeMoney[i] == KEY_PLOT)
	{
	  strtoshow[i] = '.';
	  continue;
	}
      else if(ChargeMoney[i] == KEY_0)
	{
	  strtoshow[i] = '0';
	  continue;
	}
      else
	strtoshow[i]=ChargeMoney[i]+'0';

    }
  POINT_COLOR=RED;BACK_COLOR=BG_COLOR;
  Show_Str(240-9*16+5*32,800-1-296-40,480-30,32,(u8*)strtoshow,32,0);
}

void DisplaySettingPageRes(uint32_t No,uint32_t disp)
{
  switch (No)
  {
    case 0:	//��Ƭע��OK
      POINT_COLOR=BG_COLOR*disp;BACK_COLOR=BG_COLOR;
      Show_Str(xStart+200,yStart,32*4,32,"���",Font_Setting,0);
      break;
    case 1:	//��Ƭע��ʧ��
      POINT_COLOR=BG_COLOR*disp;BACK_COLOR=BG_COLOR;
      Show_Str(xStart+200,yStart,32*4,32,"ʧ��",Font_Setting,0);
      break;
    case 2:	//��ֵ ��� OK
      POINT_COLOR=BG_COLOR*disp;BACK_COLOR=BG_COLOR;
      Show_Str(xStart+200,yStart+Step,32*4,32,"���",Font_Setting,0);
      break;
    case 3:	//��ֵ ��� ʧ��
      POINT_COLOR=BG_COLOR*disp;BACK_COLOR=BG_COLOR;
      Show_Str(xStart+200,yStart+Step,32*4,32,"ʧ��",Font_Setting,0);
      break;
    case 4:	//������
      POINT_COLOR=BG_COLOR*disp;BACK_COLOR=BG_COLOR;
      Show_Str(xStart+200,yStart+Step*2,32*4,32,"��",Font_Setting,0);
      break;
    case 5:	//������
      POINT_COLOR=BG_COLOR*disp;BACK_COLOR=BG_COLOR;
      Show_Str(xStart+200,yStart+Step*2,32*4,32,"�ر�",Font_Setting,0);
      break;
  }
}

extern xCostCB xCCB;
char costlist[10][2][10]={'\0'};
void GuiPageCheckInit(void)
{
  uint16_t countbuff[5],numberbuff[5];
  uint16_t number_int=xCCB.CostMoney/10,number_dec=xCCB.CostMoney%10;
  uint8_t i=0,j=0;

  sprintf((char *)countbuff,(const char *)"%d ��",xCCB.CostRecord);
  sprintf((char *)numberbuff,(const char *)"�� %d.%d",number_int,number_dec);
  LTDC_Clear(BG_COLOR);
  POINT_COLOR=BLACK;BACK_COLOR=GRAY;
  Show_Str(240-32*2,10,32*4,32,"������ҳ",Font_Setting,0);
  POINT_COLOR=BLACK;BACK_COLOR=BG_COLOR;
  Show_Str(30,50,480-30,32,"���ν��״�����",32,0);
  Show_Str(30+32*9,50,480-30,32,(u8 *)countbuff,32,0);
  Show_Str(30,50+40,480-30,32,"�������׽�",32,0);
  Show_Str(30+32*8,50+40,480-30,32,(u8 *)numberbuff,32,0);

  Show_Str(30,120+40,480-30,24,"�����������飺",24,0);
  POINT_COLOR=BLACK;BACK_COLOR=GRAY;
  Show_Str(30+200,120+40,480-30,24,"��һҳ",24,0);
  Show_Str(30+290,120+40,480-30,24,"��һҳ",24,0);

  memset(costlist,'\0',sizeof(costlist));
  for( i =0;i<100;i++)
    {
      if(RecentCostDetail[i*2] != 0)
	{
	  sprintf((char *)costlist[i][0],(const char *)"%x\0",RecentCostDetail[i*2]);
	  sprintf((char *)costlist[i][1],(const char *)"%d.%d\0",RecentCostDetail[i*2+1]/10,RecentCostDetail[i*2+1]%10);
	}
      else
	break;
    }
  POINT_COLOR=WHITE;BACK_COLOR=BLACK;
  Show_Str(30,200,480-30,24,"��ƬID",24,0);
  Show_Str(360,200,480-30,24,"���",24,0);
  for(j=0;((j<i)&&(j<10));j++)
    {
      Show_Str(30,240+j*30,480-30,24,costlist[j][0],24,0);
      Show_Str(360,240+j*30,480-30,24,costlist[j][1],24,0);
    }
}

void DisplayUserMsg()
{

}

void PageRefresh()
{

}
