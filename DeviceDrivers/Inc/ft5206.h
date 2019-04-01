#ifndef __FT5206_H
#define __FT5206_H	
#include "sys.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//7寸电容触摸屏-FT5206 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
 



//与电容触摸屏连接的芯片引脚(未包含IIC引脚) 
//IO操作函数	  
#define FT_RST  PIout(8)//FT5206复位引脚
#define FT_INT  PHin(7) //FT5206断引脚	
   
//I2C读写命令	
#define FT_CMD_WR 				0X70    	//写命令
#define FT_CMD_RD 				0X71		//读命令
  
//FT5206 部分寄存器定义 
#define FT_DEVIDE_MODE 			0x00   		//FT5206模式控制寄存器
#define FT_REG_NUM_FINGER       0x02		//触摸状态寄存器

#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		//第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		//第三个触摸点数据地址
#define FT_TP4_REG 				0X15		//第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		//第五个触摸点数据地址  
 

#define	FT_ID_G_LIB_VERSION		0xA1		//版本		
#define FT_ID_G_MODE 			0xA4   		//FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP			0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器



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

















