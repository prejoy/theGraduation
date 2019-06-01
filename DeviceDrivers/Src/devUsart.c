#include "usart.h"
#include "devUsart.h"


int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFFFFFF);
  return 0;
}

#if 1

#include <devUsart.h>
#include "devDelay.h"


#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲

#endif	


#endif


