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
#define KEY_NUM1	13
#define KEY_NUM2	14
#define KEY_NUM3	15
#define KEY_NUM4	9
#define KEY_NUM5	10
#define KEY_NUM6	11
#define KEY_NUM7	5
#define KEY_NUM8	6
#define KEY_NUM9	7
#define KEY_NUM0	2
#define KEY_F1		17
#define KEY_F2		18
#define KEY_WELL	19
#define KEY_STAR	20
#define KEY_ESC		8
#define KEY_ENT		4
#define KEY_UP		16
#define KEY_DOWN	12
#define KEY_LEFT	1
#define KEY_RIGHT	3


int keyScan(int mode);
int key_scan(void);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
#endif /* DEVICEDRIVERS_INC_DEVKEY_H_ */
