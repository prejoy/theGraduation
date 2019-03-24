#ifndef __DEVW25QXX_H
#define __DEVW25QXX_H

#include "sys.h"
#include "stm32f4xx.h"
#include "devDelay.h"

/*
 * Flash Mem Map
 * ���׼�¼��0-20M
 * ����������20M-25M
 * �ֿ���أ�25MB-31.06MB
 *
 * ��дw25q256 ʹ����4096�ֽڶ���
 * ��ͷʹ��4096Byte�����Դ��255-1����Ƭ������Ϣ
 * ÿ��ID�Ľ��׼�¼Ҳ��4096Byte�����Լ�¼���512�����׼�¼��
 * ���96�ֽڱ�����ֻ��¼500����¼
 * ���׼�¼��ͷ��ID	Addr	Null	NULL	...	check=0x1234567890abcdef	UserNum
 * 		4B 	4B	4B	4B		4096-16				4096-8b
 * ���׼�¼��	pay	NULL   Offset remain
 * 		4B	4B	end-96  end-96+4
 * */

//��ͷ��ÿ��ID�Ľ��׼�¼ʹ��4K���뷽ʽ
//��ʱֻʹ��1MB+4KB 200�û�
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

extern u16 W25QXX_TYPE;					//����W25QXXоƬ�ͺ�		   
extern SPI_HandleTypeDef hspi5;

#define	W25QXX_CS 		PFout(6)  		//W25QXX��Ƭѡ�ź�


//ָ���
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
u16  W25QXX_ReadID(void);  	    		//��ȡFLASH ID
u8 W25QXX_ReadSR(u8 regno);             //��ȡ״̬�Ĵ��� 
void W25QXX_4ByteAddr_Enable(void);     //ʹ��4�ֽڵ�ַģʽ
void W25QXX_Write_SR(u8 regno,u8 sr);   //д״̬�Ĵ���
void W25QXX_Write_Enable(void);  		//дʹ�� 
void W25QXX_Write_Disable(void);		//д����
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//д��flash
void W25QXX_Erase_Chip(void);    	  	//��Ƭ����
void W25QXX_Erase_Sector(u32 Dst_Addr);	//��������
void W25QXX_Wait_Busy(void);           	//�ȴ�����
void W25QXX_PowerDown(void);        	//�������ģʽ
void W25QXX_WAKEUP(void);				//����

#endif
