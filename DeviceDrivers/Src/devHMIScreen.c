/*
 * devHMIScreen.c
 *
 *  Created on: 2019��3��22��
 *      Author: admin
 */


#include "devHMIScreen.h"


extern UART_HandleTypeDef huart5;

void HMI_Init()		//����Ҫ��ʾ��ȫ������
{
	uint8_t CMD[20]="qr0.bco=0xFFFF";
	uint8_t CMD_END[3]={0xFF,0xFF,0xFF};
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,14,0xFF);
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

	strcpy(CMD,"qr0.pco=0xFFFF");
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,14,0xFF);
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

	strcpy(CMD,"t4.bco=0x7fa4");
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

	strcpy(CMD,"t4.pco=0x7fa4");
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

	HideQRCode();
	HidePay();
	HideRemain();

}
void SwitchQRcode(uint32_t disp)
{
	uint8_t CMD[20]={0};
	uint8_t CMD_END[3]={0xFF,0xFF,0xFF};

	if(disp == 0)
	{
		strcpy(CMD,"qr0.bco=0x7FA4");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,14,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

		strcpy(CMD,"qr0.pco=0x7FA4");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,14,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
	}else
	{
		strcpy(CMD,"qr0.bco=0xFFFF");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,14,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

		strcpy(CMD,"qr0.pco=0x0000");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,14,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
	}
}

//ex: SendQRcode("https://www.baidu.com/",sizeof("https://www.baidu.com/"));
uint8_t QRLINK[100];
void SendQRcode(uint8_t *str,uint32_t udLen)
{
//	uint8_t CMD[50]={0};
	uint8_t CMD_END[3]={0xFF,0xFF,0xFF};

	strcpy(QRLINK,"qr0.txt=\"");
	strcpy((QRLINK+9),(const char *)str);	// + len_pay_txt
	strcpy(QRLINK+9+udLen-1,"\"");

	HAL_UART_Transmit(&huart5,(uint8_t *)QRLINK,udLen+9,0xFF);
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
	DisplayQRCode();

}
void SwitchPay(uint32_t disp)
{
	uint8_t CMD[20]={0};
	uint8_t CMD_END[3]={0xFF,0xFF,0xFF};

	if(disp == 0)
	{
		strcpy(CMD,"t4.bco=0x7fa4");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

		strcpy(CMD,"t4.pco=0x7fa4");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
	}else
	{
		strcpy(CMD,"t4.bco=0x7fa4");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

		strcpy(CMD,"t4.pco=0x0000");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
	}
}
void SwitchRemain(uint32_t disp)
{
	uint8_t CMD[20]={0};
	uint8_t CMD_END[3]={0xFF,0xFF,0xFF};

	if(disp == 0)
	{
		strcpy(CMD,"t5.bco=0x7fa4");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

		strcpy(CMD,"t5.pco=0x7fa4");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
	}else
	{
		strcpy(CMD,"t5.bco=0x7fa4");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

		strcpy(CMD,"t5.pco=0x0000");
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
		HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
	}
}
void IntToChar(uint8_t *strint,uint8_t *strlength,uint32_t Integer,uint32_t Decimal)	//С��ֻ��һλ,�����������ֵ��9999.9Ԫ
{
	uint32_t floatnum = 0,intnum = 0;
	if(Decimal != 0){
		if((Decimal>=0)&&(Decimal<=9))
			floatnum = Decimal;
		else
			floatnum = 0;
	}

	if(Integer != 0){
		if((Integer>=0)&&(Integer<10000))
			intnum = Integer;
		else
			intnum = 0;
	}

//	printf("loc 1\r\n");

	if((floatnum+intnum)>0)	// >0Ԫ
	{
		if(intnum>0)				//����1Ԫ
		{
			if(intnum>999)								//1000-9999
			{
				*strlength = 9+1;
				sprintf(strint,"%d.%d Ԫ\"",intnum,floatnum);
			}else if((intnum>99)&&(intnum<1000))		//100-999
			{
				*strlength = 8+1;
				sprintf(strint,"%d.%d Ԫ\"",intnum,floatnum);
			}else if((intnum>9)&&(intnum<100))			//10-99
			{
				*strlength = 7+1;
				sprintf(strint,"%d.%d Ԫ\"",intnum,floatnum);
			}else if(intnum<10)							//1-9
			{
				*strlength = 6+1;
				sprintf(strint,"%d.%d Ԫ\"",intnum,floatnum);
			}
//			for(int i=0;i<9;i++)
//				printf("%c",strint[i]);
//			printf("\r\n");
		}else if (intnum == 0)		//0-1Ԫ
		{
			*strlength = 6+1;
			sprintf(strint,"0.%d Ԫ\"",floatnum);
		}
	}else			// 0.0Ԫ
	{
		*strlength = 6+1;
		strcpy(strint,"0.0 Ԫ\"");
	}

//	printf("before return\r\n");

}
void SendPay(uint32_t Integer,uint32_t Decimal)
{
	uint8_t CMD[20]={0};
	uint8_t CMD_END[3]={0xFF,0xFF,0xFF};
	uint8_t pay_txt[10],len_pay_txt;
	IntToChar(pay_txt,&len_pay_txt,Integer,Decimal);
//	printf("len:%d\r\n",(len_pay_txt));
	strcpy(CMD,"t4.txt=\"");					// 8
	strcpy((CMD+8),(const char *)pay_txt);	// + len_pay_txt
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,(8+len_pay_txt),0xFF);
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

	DisplayPay();
//	for(int i=0;i<(8+len_pay_txt);i++)
//			printf("%c",CMD[i]);
//	printf("\r\n");
}
void SendRemain(uint32_t Integer,uint32_t Decimal)
{
	uint8_t CMD[20]={0};
	uint8_t CMD_END[3]={0xFF,0xFF,0xFF};
	uint8_t pay_txt[10],len_pay_txt;
	IntToChar(pay_txt,&len_pay_txt,Integer,Decimal);
//	printf("len:%d\r\n",(len_pay_txt));
	strcpy(CMD,"t5.txt=\"");					// 8
	strcpy((CMD+8),(const char *)pay_txt);	// + len_pay_txt
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,(8+len_pay_txt),0xFF);
	HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);

	DisplayRemain();
}



void SwitchResult(uint32_t isSuccess,uint32_t errtype,uint32_t disp,uint32_t txtcolor)
{
	uint8_t CMD[20]={0};
	uint8_t CMD_END[3]={0xFF,0xFF,0xFF};
	if(isSuccess == 0)
	{
		if(disp == 1)
		{
			if(txtcolor == 0)
			{
				strcpy(CMD,"t6.pco=0x0000");
				HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
				HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
			}else
			{
				strcpy(CMD,"t6.pco=0xFFFF");
				HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
				HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
			}
		}
		else
		{
			strcpy(CMD,"t6.pco=0x7FA4");
			HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
			HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
		}
	}
	else
	{
		if(disp == 1)
		{
			switch(errtype)
			{
				case ERR_PAYFAIL:
					strcpy(CMD,"t7.txt=\"");
					strcpy((CMD+8),(const char *)"֧��ʧ��\"");
//					printf("%s\r\n",CMD);
					HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,(8+9),0xFF);
					HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
					break;
				case ERR_BALANCECHARGE:
					strcpy(CMD,"t7.txt=\"");
					strcpy((CMD+8),(const char *)"����\"");
//					printf("%s\r\n",CMD);
					HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,(8+9),0xFF);
					HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
					break;
				case ERR_NOSUCHID:
					strcpy(CMD,"t7.txt=\"");
					strcpy((CMD+8),(const char *)"��Ƭδע��\"");
//					printf("%s\r\n",CMD);
					HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,(8+11),0xFF);
					HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
					break;
				default:
					strcpy(CMD,"t7.txt=\"");
					strcpy((CMD+8),(const char *)"δ֪����\"");
//					printf("%s\r\n",CMD);
					HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,(8+9),0xFF);
					HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
					break;
			}

			if(txtcolor == 0)		//��ɫ��ʾ
			{
				strcpy(CMD,"t7.pco=0x0000");
				HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
				HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
			}else				//white display
			{
				strcpy(CMD,"t7.pco=0xFFFF");
				HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
				HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
			}
		}
		else
		{
			strcpy(CMD,"t7.pco=0x7FA4");
			HAL_UART_Transmit(&huart5,(uint8_t *)&CMD,13,0xFF);
			HAL_UART_Transmit(&huart5,(uint8_t *)&CMD_END,sizeof(CMD_END),0xFF);
		}
	}
}
