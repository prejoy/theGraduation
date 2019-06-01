/*
 * devKey.h
 *
 *  Created on: 2019年1月16日
 *      Author: prejoy
 */

#ifndef DEVICEDRIVERS_INC_DEVKEY_H_
#define DEVICEDRIVERS_INC_DEVKEY_H_

#include "stm32f4xx.h"
#include "devDelay.h"

/*
 * 从左4到右5
 * PA3 PA4 PA6 PA5
 * PB10，12， 13， 14， 15
 *
 * */

#define ROW1_CORRECT	(0)
#define ROW2_CORRECT	(0)
#define ROW3_CORRECT	(0)
#define ROW4_CORRECT	(0)
#define ROW5_CORRECT	(0)

#define COL1_CORRECT	(0)
#define COL2_CORRECT	(0)
#define COL3_CORRECT	(1)		//线 接反了 ，修正	PA5和PA6
#define COL4_CORRECT	(-1)		//线 接反了 ，修正

/*
 * | F1 | F2 | #  | *  |
 * | 1  | 2  | 3  | UP |
 * | 4  | 5  | 6  | DN |
 * | 7  | 8  | 9  | Es |
 * | LT | 0  | RT | En |
 * =====================
 * | 17 | 18 | 19 | 20 |
 * | 13 | 14 | 15 | 16 |
 * | 9  | 10 | 11 | 12 |
 * | 5  | 6  | 7  | 8  |
 * | 1  | 2  | 3  | 4  |
 * */


#define KEY_1		(1U)
#define KEY_2		(2U)
#define KEY_3		(3U)
#define KEY_4		(4U)
#define KEY_5		(5U)
#define KEY_6		(6U)
#define KEY_7		(7U)
#define KEY_8		(8U)
#define KEY_9		(9U)
#define KEY_0		(10U)
#define KEY_PLOT	(11U)
#define KEY_BACK	(12U)
#define KEY_F1		(13U)
#define KEY_F2		(14U)
#define KEY_ESC		(15U)
#define KEY_ENT		(16U)
#define KEY_STAR	(17U)	//支付方式 扫码
#define KEY_WELL	(18U)	//支付方式 刷卡
#define KEY_UP		(19U)
#define KEY_DOWN	(20U)
#define KEY_CHECK	KEY_F1
#define KEY_SETTING	KEY_F2
#define KEY_CARDPAY	KEY_WELL
#define KEY_QRPAY	KEY_STAR


int keyScan(int mode);
int key_scan(void);
uint8_t KeyConversion(int keyval);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
#endif /* DEVICEDRIVERS_INC_DEVKEY_H_ */
