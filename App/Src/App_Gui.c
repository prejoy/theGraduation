/*
 * App_Gui.c
 *
 *  Created on: 2019年3月30日
 *      Author: prejoy
 */

#include "stdio.h"
#include "App_Gui.h"
#include "devLtdc.h"
#include "text.h"


void GuiPageCostInit(void)
{
  //=============             user page        ==========================
   char lcd_id[12];
   sprintf((char*)lcd_id,"LCD ID:7084");
   printf("%s\r\n",lcd_id);
   LTDC_Clear(BG_COLOR);

   LTDC_Fill(PIC_SX,PIC_SY,PIC_SX+PIC_WIDTH,PIC_SY+PIC_HEIGHT,WHITE);
   POINT_COLOR=BLACK;BACK_COLOR=BG_COLOR;
   Show_Str(MSGn_SX(1),MSGn_SY(1),480-MSG_SX,MSG_FONT,"卡号：",MSG_FONT,0);
   Show_Str(MSGn_SX(2),MSGn_SY(2),480-MSG_SX,MSG_FONT,"姓名：",MSG_FONT,0);
   Show_Str(MSGn_SX(3),MSGn_SY(3),480-MSG_SX,MSG_FONT,"年级：",MSG_FONT,0);
   Show_Str(MSGn_SX(4),MSGn_SY(4),480-MSG_SX,MSG_FONT,"专业：",MSG_FONT,0);
   Show_Str(MSGn_SX(5),MSGn_SY(5),480-MSG_SX,MSG_FONT,"余额：          元",MSG_FONT,0);
   Show_Str(30,280,480-30,32,"请输入消费金额：      元",32,0);

   POINT_COLOR=BLACK;BACK_COLOR=GRAY;
   Show_Str(120-24*2,(PIC_SY+PIC_HEIGHT+504)/2,24*4,24,"扫码支付",24,0);
   Show_Str(360-24*2,(PIC_SY+PIC_HEIGHT+504)/2,24*4,24,"刷卡支付",24,0);

   //===================    softkbd  =================
    LTDC_Fill(0,800-1-296,480-1,800-1,SOFT_KBD_FILLCOLOR);				//fill必须从上到下，左到右
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
    Show_Str((XPOSI(4)-FONTSIZE),(YPOSI(4)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"查询",FONTSIZE,0);
    Show_Str((XPOSI(8)-FONTSIZE),(YPOSI(8)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"设置",FONTSIZE,0);
    Show_Str((XPOSI(12)-FONTSIZE),(YPOSI(12)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"取消",FONTSIZE,0);
    Show_Str((XPOSI(15)-FONTSIZE/2),(YPOSI(15)-FONTSIZE/2),FONTSIZE,FONTSIZE,"←",FONTSIZE,0);
    Show_Str((XPOSI(16)-FONTSIZE),(YPOSI(16)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"确定",FONTSIZE,0);

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


//超过4个字注意居中参数变化
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
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "支付成功", 32, 0);
	    }
	  else
	    {
	      POINT_COLOR=WHITE;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "支付成功", 32, 0);
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
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "无效用户", 32, 0);
	      break;
	    case ERR_BALANCECHARGE:
	      POINT_COLOR=(WHITE)*txtcolor;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "余额不足", 32, 0);
	      break;
	    default:
	      POINT_COLOR=(WHITE)*txtcolor;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "支付失败", 32, 0);
	      break;
	  }
	}
      else
	{
	      POINT_COLOR=BG_COLOR;BACK_COLOR=BG_COLOR;
	      Show_Str (xCenter - 32 * 2, yCenter, 32 * 4, 32, "        ", 32, 0);
	}
    }

}

void DisplayUserMsg()
{

}

void PageRefresh()
{

}
