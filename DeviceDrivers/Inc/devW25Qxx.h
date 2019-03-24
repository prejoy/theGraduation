#ifndef __DEVW25QXX_H
#define __DEVW25QXX_H

#include "sys.h"
#include "stm32f4xx.h"
#include "devDelay.h"

/*
 * Flash Mem Map
 * 交易记录：0-20M
 * 程序升级：20M-25M
 * 字库相关：25MB-31.06MB
 *
 * 读写w25q256 使用了4096字节对齐
 * 表头使用4096Byte，可以存放255-1个卡片交易信息
 * 每个ID的交易记录也用4096Byte，可以记录最多512条交易记录，
 * 最后96字节保留，只记录500条记录
 * 交易记录表头：ID	Addr	Null	NULL	...	check=0x1234567890abcdef	UserNum
 * 		4B 	4B	4B	4B		4096-16				4096-8b
 * 交易记录：	pay	NULL   Offset remain
 * 		4B	4B	end-96  end-96+4
 * */

//表头和每个ID的交易记录使用4K对齐方式
//暂时只使用1MB+4KB 200用户
#define TBL_ADDR	0x00000000U
#define MAX_UID		200
#define DATA_START_ADDR	(TBL_ADDR+((MAX_UID-1)/256+1)*4096)
#define DATA_END_ADDR	(DATA_START_ADDR+MAX_UID*4096)
#define BOUNDARY_ADDR	0x01400000U	//20MB




#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

extern u16 W25QXX_TYPE;					//定义W25QXX芯片型号		   
extern SPI_HandleTypeDef hspi5;

#define	W25QXX_CS 		PFout(6)  		//W25QXX的片选信号


//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1   		0x01
#define W25X_WriteStatusReg2   		0x31
#define W25X_WriteStatusReg3    	0x11
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown		0xAB
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID		0x90
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    	0xB7
#define W25X_Exit4ByteAddr      	0xE9

u8 SPI5_ReadWriteByte(u8 TxData);
uint32_t W25QXX_Init(void);
u16  W25QXX_ReadID(void);  	    		//读取FLASH ID
u8 W25QXX_ReadSR(u8 regno);             //读取状态寄存器 
void W25QXX_4ByteAddr_Enable(void);     //使能4字节地址模式
void W25QXX_Write_SR(u8 regno,u8 sr);   //写状态寄存器
void W25QXX_Write_Enable(void);  		//写使能 
void W25QXX_Write_Disable(void);		//写保护
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void W25QXX_Erase_Chip(void);    	  	//整片擦除
void W25QXX_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void W25QXX_Wait_Busy(void);           	//等待空闲
void W25QXX_PowerDown(void);        	//进入掉电模式
void W25QXX_WAKEUP(void);				//唤醒

#endif
