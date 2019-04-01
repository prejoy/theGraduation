#ifndef __FT5206_H
#define __FT5206_H	
#include "sys.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//7����ݴ�����-FT5206 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/12/28
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
 



//����ݴ��������ӵ�оƬ����(δ����IIC����) 
//IO��������	  
#define FT_RST  PIout(8)//FT5206��λ����
#define FT_INT  PHin(7) //FT5206������	
   
//I2C��д����	
#define FT_CMD_WR 				0X70    	//д����
#define FT_CMD_RD 				0X71		//������
  
//FT5206 ���ּĴ������� 
#define FT_DEVIDE_MODE 			0x00   		//FT5206ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02		//����״̬�Ĵ���

#define FT_TP1_REG 				0X03	  	//��һ�����������ݵ�ַ
#define FT_TP2_REG 				0X09		//�ڶ������������ݵ�ַ
#define FT_TP3_REG 				0X0F		//���������������ݵ�ַ
#define FT_TP4_REG 				0X15		//���ĸ����������ݵ�ַ
#define FT_TP5_REG 				0X1B		//��������������ݵ�ַ  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//�汾		
#define FT_ID_G_MODE 			0xA4   		//FT5206�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP			0x80   		//������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE	0x88   		//����״̬�������üĴ���



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
#define KEY_CHECK	KEY_F1
#define KEY_SETTING	KEY_F2
#define KEY_ESC		(15U)
#define KEY_ENT		(16U)
#define KEY_STAR	(17U)
#define KEY_WELL	(18U)
#define KEY_CARDPAY	KEY_WELL
#define KEY_QRPAY	KEY_STAR
//#define KEY_UP	(19U)
//#define KEY_DOWN	(20U)




u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len);
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 FT5206_Init(void);
u8 FT5206_Scan(u8 mode);
uint8_t TouchPadConversion(void);

#endif

















