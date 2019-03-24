/*
 * App_main.c
 *
 *  Created on: 2019年1月16日
 *      Author: prejoy
 */

#include "App_main.h"
#include "stdio.h"

static void vTaskLed1( void * pvParameters );
#define Prio_LED1	2
static TaskHandle_t TaskHandle_LED1;
#define STK_SIZE_LED1	512

static void vTaskPrint( void * pvParameters );
#define Prio_Print	3
static TaskHandle_t TaskHandle_Print;
#define STK_SIZE_Print	4096


QueueHandle_t queueDeviceIn,
	      queueDeviceOut,
	      queueEthIn,
	      queueEthOut;
typedef struct Device_In{
  uint8_t source;	//1 ，2 分别指代 物理按键和虚拟键盘
  uint8_t value;	//键值
}xDeviceIn;

#define Length_queueDeviceIn 	5
#define Size_queueDeviceIn	sizeof(xDeviceIn)

#define SOURCE_IN_KEY 1
#define SOURCE_IN_TP 2

void vTaskLed1( void * pvParameters )
{
  xDeviceIn emuDevLed1In={1,5};
    for(;;)
    {
	toggleLed(0);
	xQueueSend(queueDeviceIn,&emuDevLed1In,10);
	vTaskDelay( pdMS_TO_TICKS(500) );
    }

}
void vTaskPrint( void * pvParameters )
{
  xDeviceIn DeviceIn;
    for(;;)
    {
	if(xQueueReceive(queueDeviceIn,&DeviceIn,portMAX_DELAY) != pdPASS)
	  {

	  }
	else
	  {
	    printf("Message:%d,from:%d\r\n",DeviceIn.value,DeviceIn.source);

	  }
//	delay_ms(500);
	vTaskDelay( pdMS_TO_TICKS(500) );
    }

}

void vTaskKeyScan(void * pvParameters)
{
  static int keyvalue=0;
  xDeviceIn KeyIn;
  KeyIn.source = SOURCE_IN_KEY;
  for(;;)
    {
      keyvalue=key_scan();		//OSdelay 替换
      if((keyvalue != 0xff)&&(keyvalue != 0))
	{
	  KeyIn.value=keyvalue;
	  if(xQueueSend(queueDeviceIn,&KeyIn,10) != pdPASS)
	    {
	      printf("queueDeviceIn Send error \r\n");
	    }
	}
      vTaskDelay( pdMS_TO_TICKS(20) );	//添加队列发送信息
    }
}

//1 队列功能测试
//2 带添加flash 消费记录改动，以及ID表头清除功能
int App_main()
{
  BaseType_t Retn;

  queueDeviceIn =  xQueueCreate( Length_queueDeviceIn, Size_queueDeviceIn );
  if(queueDeviceIn == NULL)
    {
      printf("QUEUE:queueDeviceIn Create fail\r\n");
    }

  Retn = xTaskCreate( vTaskLed1, "Task_Led1", STK_SIZE_LED1, NULL, Prio_LED1, &TaskHandle_Print );
  if(Retn!=pdPASS)
  {
      printf("Task_Led1 create fail\r\n");
  }

  Retn = xTaskCreate( vTaskPrint, "Task_Print", STK_SIZE_Print, NULL, Prio_Print, &TaskHandle_LED1 );
  if(Retn!=pdPASS)
  {
      printf("Task_Led1 create fail\r\n");
  }


  vTaskStartScheduler();
  for(;;);
  return 0;

/*  HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE);//配置下次中断接收的缓冲区,每次准备接收下一次了就要清一下缓冲区,再这样配置一下,这相当于是一个控制块
  while (1)
    {
      toggleLed(0);
      Delaysomenop(800*1000);
      toggleLed(0);
      Delaysomenop(800*1000);
      printf("in blink\r\n");
      printf("%s\r\n",aRxBuffer);
      if(keyScan(0) == 0)
	{
	  do{
	      setLedOn(0,0);
	      printf("in keyscan\r\n");
	  }while(keyScan(0) == 0);
	}
    }
  return 0;*/
}
