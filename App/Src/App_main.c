/*
 * App_main.c
 *
 *  Created on: 2019年1月16日
 *      Author: prejoy
 */

#include "App_main.h"
#include "stdio.h"


//========     TCBs      ===========
static void vTaskLed1( void * pvParameters );
#define Prio_LED1	1
static TaskHandle_t TaskHandle_LED1;
#define STK_SIZE_LED1	512

static void vTaskPrint( void * pvParameters );
#define Prio_Print	2
static TaskHandle_t TaskHandle_Print;
#define STK_SIZE_Print	2048

static void vTaskKeyScan( void * pvParameters );
#define Prio_KeyScan		3
static TaskHandle_t TaskHandle_KeyScan;
#define STK_SIZE_KeyScan 	512

static void vTaskVoiceBoardcast( void * pvParameters );
#define Prio_VoiceBoardcast		5
static TaskHandle_t TaskHandle_VoiceBoardcast;
#define STK_SIZE_VoiceBoardcast 	512


QueueHandle_t queueKeyIn,
	      queueCardIn,
	      queueEthIn,
	      queueEthOut,
	      queueVoice,
	      queueUserMsg;

//========     key and touchpad      ===========
typedef struct Key_In{
  uint8_t source;	//1 ，2 分别指代 物理按键和虚拟键盘
  uint8_t value;	//键值
}xKeyIn;

#define Length_queueKeyIn 	1
#define Size_queueKeyIn	    sizeof(xKeyIn)

#define SOURCE_IN_KEY 1
#define SOURCE_IN_TP 2


//===============     voice      ==================
typedef struct Voice{
  uint8_t* voicelist;
  uint8_t  udlen;
}xVoice;

uint8_t VoiceArray[10];

#define Length_queueVoice 	3
#define Size_queueVoice	    sizeof(xVoice)


//===============     LCD      ==================
typedef struct UserMsg{
  uint32_t CardID;
  uint8_t  name[10];
  uint8_t  Grade;
  uint8_t  profession;
  uint32_t Remain;	//以 0.1元 为单位
}xUserMsg;

#define Length_queueUserMsg 	1
#define Size_queueUserMsg    sizeof(xUserMsg)

//组态屏 9600 波特率？？？
typedef struct CostMsg{
  uint32_t CostMoney;
  uint32_t TradeType;	//??全局变量
}xCostMsg;


//===============        sys     =======================
struct SYS_CTRL_BLOCK{

  uint32_t page;
//  uint32_t InputMode;
}scb;
#define COSTIN		1
#define OPERATIONIN	2


//============       task funcs       ==================

void vTaskLed1( void * pvParameters )
{
  xKeyIn emuDevLed1In={1,5};
    for(;;)
    {
	toggleLed(0);
//	xQueueSend(queueKeyIn,&emuDevLed1In,10);
	vTaskDelay( pdMS_TO_TICKS(500) );
    }

}
void vTaskPrint( void * pvParameters )
{
  xKeyIn DeviceIn;
    for(;;)
    {
	if(xQueueReceive(queueKeyIn,&DeviceIn,portMAX_DELAY) != pdPASS)
	  {

	  }
	else
	  {
	    printf("Message:%d,from:%d\r\n",DeviceIn.value,DeviceIn.source);

	  }
//	delay_ms(500);
	vTaskDelay( pdMS_TO_TICKS(5) );
    }

}

void vTaskKeyScan(void * pvParameters)
{
  static int keyvalue=0;
  xKeyIn KeyIn;
  KeyIn.source = 0;
  for(;;)
    {
      keyvalue=key_scan();		//OSdelay 替换
      if((keyvalue != 0xff)&&(keyvalue != 0))
	{
	  KeyIn.value=KeyConversion(keyvalue);
	  KeyIn.source = SOURCE_IN_KEY;
	  if(xQueueSend(queueKeyIn,&KeyIn,10) != pdPASS)
	    {
	      printf("Err:%s:%d\r\n",__FILE__,__LINE__);
	    }
//	  if(xQueueSend(queueKeyIn,&KeyIn,10) != pdPASS)	//语音
//	    {
//	      printf("Err:%s:%d\r\n",__FILE__,__LINE__);
//	    }
	}
      else
	{
	  if(FT5206_Scan(0)!=0)
	    {
	      KeyIn.value = TouchPadConversion();
	      if(KeyIn.value != 0xff)
		{
		  KeyIn.source = SOURCE_IN_TP;
		  if(xQueueSend(queueKeyIn,&KeyIn,10) != pdPASS)
		    {
		      printf("Err:%s:%d\r\n",__FILE__,__LINE__);
		    }
		}
	    }
	}
      vTaskDelay( pdMS_TO_TICKS(20) );	//添加队列发送信息
    }
}


//带改动： 添加播放状态查询，打断播放之类的问题
void vTaskVoiceBoardcast(void * pvParameters)
{
  //  uint8_t voiceList[10]={21,22,23,14,15,16,17,18,19,20};
    xVoice VoiceList={VoiceArray,0};
    for(;;)
      {
        if(xQueueReceive(queueVoice,&VoiceList,portMAX_DELAY) != pdPASS)
	  {

	  }
	else
	  {
//	    printf("VoiceList len:%d\r\n",VoiceList.udlen);
	    ContinuePlay(VoiceList.voicelist,VoiceList.udlen);
	  }
        taskYIELD();
      }
}

//==================      app_main   func   ===========================

//1 队列功能测试
//2 带添加flash 消费记录改动，以及ID表头清除功能
int App_main()
{
  BaseType_t Retn;

  queueKeyIn =  xQueueCreate( Length_queueKeyIn, Size_queueKeyIn );
  if(queueKeyIn == NULL)
    {
      printf("QUEUE:queueDeviceIn Create fail\r\n");
    }
  queueVoice = xQueueCreate( Length_queueVoice, Size_queueVoice );
  if(queueVoice == NULL)
    {
      printf("QUEUE:queueVoice Create fail\r\n");
    }

  Retn = xTaskCreate( vTaskLed1, "Task_Led1", STK_SIZE_LED1, NULL, Prio_LED1, &TaskHandle_Print );
  if(Retn!=pdPASS)
  {
      printf("Task_Led1 create fail\r\n");
  }

  Retn = xTaskCreate( vTaskPrint, "Task_Print", STK_SIZE_Print, NULL, Prio_Print, &TaskHandle_LED1 );
  if(Retn!=pdPASS)
  {
      printf("Task_Print create fail\r\n");
  }

  Retn = xTaskCreate( vTaskKeyScan, "Task_KeyScan", STK_SIZE_KeyScan, NULL, Prio_KeyScan, &TaskHandle_KeyScan );
  if(Retn!=pdPASS)
  {
      printf("Task_KeyScan create fail\r\n");
  }

  Retn = xTaskCreate( vTaskVoiceBoardcast, "Task_VoiceBoardcast", STK_SIZE_VoiceBoardcast, NULL, Prio_VoiceBoardcast, &TaskHandle_VoiceBoardcast );
  if(Retn!=pdPASS)
  {
      printf("Task_VoiceBoardcast create fail\r\n");
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
