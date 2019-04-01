/*
 * App_Gui.h
 *
 *  Created on: 2019��3��30��
 *      Author: prejoy
 */

#ifndef APP_INC_APP_GUI_H_
#define APP_INC_APP_GUI_H_

#include "stm32f4xx.h"

//draw user page 480X504	24������
//   (0,0)		(480,0)
//  ------		���ţ�
//  |	 |		������
//  |	 |		�꼶��
//  ------		רҵ��
//			��
//		�������ѽ�
//  ɨ��֧��		ˢ��֧��
//   (0,504)		(480,504)

//  pic:��25-145��195��
#define PIC_SX		35
#define PIC_SY		60
#define PIC_WIDTH	130	//ռ�������PIC_SX+PIC_WIDTH��Ҫ���� MSG_SX
#define PIC_HEIGHT	190

#define MSG_FONT	24
#define MSG_SX		200
#define MSG_SY		(PIC_SY)
#define MSG_LINESPACE	12
#define MSGn_SX(n)	MSG_SX
#define MSGn_SY(n)	(MSG_SY+(MSG_LINESPACE+MSG_FONT)*(n-1))





//===================    softkbd  =================
//===================    ������Ӵ���������	===
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
//ɨ��֧����X(65-175),Y(380-420)
//ɨ��֧����X(310-420),Y(380-420)
//NUM:X:120*4 (0-480)
//    Y:74*4  (504-800)



void GuiPageCostInit(void);
void DisplayCostMoney(uint32_t CostMoney);

#endif /* APP_INC_APP_GUI_H_ */
