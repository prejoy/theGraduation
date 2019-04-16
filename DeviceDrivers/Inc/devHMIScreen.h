/*
 * devHMIScreen.h
 *
 *  Created on: 2019年3月22日
 *      Author: admin
 */

#ifndef DEVICEDRIVERS_INC_DEVHMISCREEN_H_
#define DEVICEDRIVERS_INC_DEVHMISCREEN_H_

#include "stdio.h"
#include "string.h"
#include "stm32f4xx.h"
#include "devUsart.h"

//Usage:
/* UARTInit = 9600
 * HMI_Init();
 * 发送刷卡金额
 * HidePay();
 * SendPay();
 * DisplayPay();
 * */

void HMI_Init();		//不需要显示的全部隐藏
void SendPay(uint32_t Integer,uint32_t Decimal);
void SendRemain(uint32_t Integer,uint32_t Decimal);
void SendQRcode(uint8_t *str,uint32_t udLen);	// SendQRcode("https://www.baidu.com/",sizeof("https://www.baidu.com/"));

#define DisplayQRCode()		SwitchQRcode(1)
#define HideQRCode()		SwitchQRcode(0)
#define DisplayPay()		SwitchPay(1)
#define HidePay()			SwitchPay(0)
#define DisplayRemain()		SwitchRemain(1)
#define HideRemain()		SwitchRemain(0)
#define DisplayCostOK_WHITE()	SwitchResult(0,0,1,0xFFFF)
#define DisplayCostOK_BLACK()	SwitchResult(0,0,1,0x0000)
#define HideCostOK()		    SwitchResult(0,0,0,0x7FA4)
#define DisplayCostFail_WHITE(No)	SwitchResult(1,No,1,1)
#define DisplayCostFail_BLACK(No)	SwitchResult(1,No,1,0)
#define HideCostFail()			SwitchResult(1,0,0,0)

#define ERR_PAYFAIL		1
#define ERR_BALANCECHARGE	2
#define ERR_NOSUCHID		3

#define HMI_BGCOLOR		0x7FA4

void SwitchQRcode(uint32_t disp);
void SwitchPay(uint32_t disp);
void SwitchRemain(uint32_t disp);
void SwitchResult(uint32_t isSuccess,uint32_t errtype,uint32_t disp,uint32_t txtcolor);//支付OK or Fail ；错误类型；是否显示；显示颜色
void IntToChar(uint8_t *strint,uint8_t *strlength,uint32_t Integer,uint32_t Decimal);	//小数？? 小数仅支持1位，单次最大输入值：9999.9元

#endif /* DEVICEDRIVERS_INC_DEVHMISCREEN_H_ */
