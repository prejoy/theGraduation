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


#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���

#endif	


#endif


