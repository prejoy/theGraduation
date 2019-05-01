/*
 * App_Gui.h
 *
 *  Created on: 2019年3月30日
 *      Author: prejoy
 */

#ifndef APP_INC_APP_GUI_H_
#define APP_INC_APP_GUI_H_

#include "stm32f4xx.h"


#define COST_MONEY_BUFFER		1
#define COST_BUFFER_CLEAR		2
#define TOPAGECHECK			3
#define TOPAGESETTING			4
#define TOPAGECOST			5
#define CHARGEDISPLAY			6
#define CARDREMAINSHOW			7
#define CARDREMAINHIDE			8
#define SOME_USER_MSG			9
#define DISPLAY_PICTURE			10
#define HIDE_USER_MSG			11
#define HIDE_PICTURE			12
#define NULL_USER_MSG			13

#define ERR_PAYFAIL		1
#define ERR_BALANCECHARGE	2
#define ERR_NOSUCHID		3

//draw user page 480X504	24号字体
//   (0,0)		(480,0)
//  ------		卡号：
//  |	 |		姓名：
//  |	 |		年级：
//  ------		专业：
//			余额：
//		本次消费金额：
//  扫码支付		刷卡支付
//   (0,504)		(480,504)

//  pic:（25-145，195）
#define PIC_SX		20
#define PIC_SY		30
#define PIC_WIDTH	140	//占用最大宽度PIC_SX+PIC_WIDTH不要超过 MSG_SX
#define PIC_HEIGHT	200

#define MSG_FONT	24
#define MSG_SX		200
#define MSG_SY		(PIC_SY)
#define MSG_LINESPACE	12
#define MSGn_SX(n)	MSG_SX
#define MSGn_SY(n)	(MSG_SY+(MSG_LINESPACE+MSG_FONT)*(n-1))





//===================    softkbd  =================
//===================    继续添加触摸屏功能	===
#define SOFT_KBD_LINECOLOR	0x001F	//blue
#define SOFT_KBD_FILLCOLOR	0xFFFF	//whiite
#define LINE_WIDTH		4
#define XPOSI(n)		(60+((n-1)%4)*120)
#define YPOSI(n)		(540+(n-1)/4*74)
#define FONTSIZE		32
//draw keyboard 480X296
//   (0,800-296)		(480,800-296)
//       296/4
//   (0,800-0)	480/4	(480,800-0)






//===================    touchpad  posi  =================
//扫码支付：X(65-175),Y(380-420)
//扫码支付：X(310-420),Y(380-420)
//NUM:X:120*4 (0-480)
//    Y:74*4  (504-800)



void GuiPageCostInit(void);
void GuiPageSettingInit(void);
void GuiPageCheckInit(void);
void GuiPageSettingInput(void);
void GuiPageSettingChargeAmount(uint8_t *ChargeMoney);
void DisplayCostMoney(uint8_t *CostMoney);
void DispalyCardRemain(uint8_t *RemainMoney);
void ClearCostMoney(void);
void clearCardRemain(void);
void DisplayUserMsg(uint8_t* buffer);
void HideUserMsg(void);
void HidePicture(void);
#define LCDCostOKWhite()	DisplayLCDCosRes(0,0,1,0)
#define LCDCostOKBlack()	DisplayLCDCosRes(0,0,1,1)
#define LCDCostFailWhite(n)	DisplayLCDCosRes(1,((n)),1,0)
#define LCDCostFailBlack(n)	DisplayLCDCosRes(1,((n)),1,1)
#define LCDCostHide()		DisplayLCDCosRes(0,0,0,1)
//page setting
#define ShowCardRegOK()		DisplaySettingPageRes(0,0)
#define HideCardRegOK()		DisplaySettingPageRes(0,1)
#define ShowCardRegFail()	DisplaySettingPageRes(1,0)
#define HideCardRegFail()	DisplaySettingPageRes(1,1)
#define ShowCardChargeOK()	DisplaySettingPageRes(2,0)
#define HideCardChargeOK()	DisplaySettingPageRes(2,1)
#define ShowCardChargeFail()	DisplaySettingPageRes(3,0)
#define HideCardChargeFail()	DisplaySettingPageRes(3,1)
#define ShowVoiceOn()		DisplaySettingPageRes(4,0)
#define HideVoiceOn()		DisplaySettingPageRes(4,1)
#define ShowVoiceOff()		DisplaySettingPageRes(5,0)
#define HideVoiceOff()		DisplaySettingPageRes(5,1)

void DisplayLCDCosRes(uint32_t isSuccess,uint32_t errtype,uint32_t disp,uint32_t txtcolor);
void DisplaySettingPageRes(uint32_t No,uint32_t disp);

#endif /* APP_INC_APP_GUI_H_ */
