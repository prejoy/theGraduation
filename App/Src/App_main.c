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
#define STK_SIZE_LED1	256

//static void vTaskPrint( void * pvParameters );
//#define Prio_Print	1
//static TaskHandle_t TaskHandle_Print;
//#define STK_SIZE_Print	256

static void vTaskKeyScan( void * pvParameters );
#define Prio_KeyScan		3
static TaskHandle_t TaskHandle_KeyScan;
#define STK_SIZE_KeyScan 	512

static void vTaskVoiceBoardcast( void * pvParameters );
#define Prio_VoiceBoardcast		6
static TaskHandle_t TaskHandle_VoiceBoardcast;
#define STK_SIZE_VoiceBoardcast 	512

static void vTaskRFIDReadCard( void * pvParameters );
#define Prio_RFIDReadCard		8
static TaskHandle_t TaskHandle_RFIDReadCard;
#define STK_SIZE_RFIDReadCard 	512

static void vTaskMsgTrans( void * pvParameters );
#define Prio_MsgTrans		2
static TaskHandle_t TaskHandle_MsgTrans;
#define STK_SIZE_MsgTrans 	2048	//1024不够用

static void vTaskCostPage( void * pvParameters );
#define Prio_CostPage		8
static TaskHandle_t TaskHandle_CostPage;
#define STK_SIZE_CostPage 	4096

static void vTaskLCDMonitor( void * pvParameters );
#define Prio_LCDMonitor		5
static TaskHandle_t TaskHandle_LCDMonitor;
#define STK_SIZE_LCDMonitor 	512

static void vTaskFatfs( void * pvParameters );
#define Prio_Fatfs		4
static TaskHandle_t TaskHandle_Fatfs;
#define STK_SIZE_Fatfs 	1024

QueueHandle_t queueKeyIn,
	      queueKeyBuffer,
	      queueCardIn,
	      queueCardBuffer,
	      queueEthIn,
	      queueEthOut,
	      queueVoice,
	      queueUserMsg,
	      queueLCDShow;

//========     key and touchpad      ===========
typedef struct Key_In{
  uint8_t source;	//1 ，2 分别指代 物理按键和虚拟键盘
  uint8_t value;	//键值
}xKeyIn;

#define Length_queueKeyIn 	1
#define Size_queueKeyIn	    sizeof(xKeyIn)

#define SOURCE_IN_KEY 1
#define SOURCE_IN_TP 2


#define  Length_queueKeyBuffer	1
#define	 Size_queueKeyBuffer    12


//===============     voice      ==================
typedef struct Voice{
  uint8_t* voicelist;
  uint8_t  udlen;
}xVoice;

uint8_t VoiceArray[10];
uint8_t KeyVoiceArray[2]={VOICE_KEY,VOICE_KEY};
uint32_t AllVoiceSwitch =1;

#define Length_queueVoice 	1
#define Size_queueVoice	    sizeof(xVoice)


//===============     Rfid Card      ==================
#define Length_queueCardIn 		1
#define Size_queueCardIn	    sizeof(uint32_t)



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

typedef struct queueLcdShow{
  uint8_t  *Buffer;
  uint8_t  len;
  uint8_t  Type;
}xqueueLcdShow;

#define Length_queueLcdShow 	1
#define Size_queueLcdShow    sizeof(xqueueLcdShow)



//组态屏 9600 波特率
typedef struct CostMsg{
  uint32_t CostMoney;
  uint32_t TradeType;	//??全局变量
  uint8_t* QRCode;
  uint32_t QRLen;
}xCostMsg;


#define Length_queueCostMsg 	1
#define Size_queueCostMsg    sizeof(xCostMsg)

//===============        card     =====================
uint32_t CardID;//可以不用通知，Flash信息扫描该ID号，不为0xffffffff，则进行数据读取，否则任务切换
		//刷卡任务周期检卡，有卡改此ID，否则将ID置为0xffffffff，无效
		//若flash信息读取OK，则将 信息结构体发送到 显示刷新任务Refresh

//===============     Nand   Flash      =====================
typedef struct NandOperation{
  xUserMsg UMsg;
  uint32_t OperationType;
  	  	  	  	  	  	  	  	 //添加，改动，删除
}xNandOperation;

//===============     SPI   Flash      =====================
typedef struct W25QOperation{			//这个可能不用，用app_pal.h里的
  PayRecord_t CostMsg;
  uint32_t OperationType;
  	  	  	  	  	  	  	  	 //添加，改动，删除
}xW25QOperation;

//==============     System   control   block     =======================
#define PAGE_COST	1
#define PAGE_CHECK	2
#define PAGE_SETTING	3

#define PROCESS_IDLE		1
#define PROCESS_WAITCARD	2
#define PROCESS_CHECK		4
#define PROCESS_SETTING		8

typedef struct SYS_CTRL_BLOCK{

  uint32_t page;
  uint32_t InputMode;	//区分不同的输入模式，如在充值金额时，动态显示，及案件输入
			//page = setting时，置为1 = 软键盘输入有效
}xSYS_CTRL_BLOCK;

xSYS_CTRL_BLOCK syscb={.page=PAGE_COST,0};


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
  uint32_t CardIdIn;
    for(;;)
    {
//	if(xQueueReceive(queueKeyIn,&DeviceIn,pdMS_TO_TICKS(10)) == pdPASS)
//	  {
//	    printf("Message:%d,from:%d\r\n",DeviceIn.value,DeviceIn.source);
//	  }
//
//	if(xQueueReceive(queueCardIn,&CardIdIn,0) == pdPASS)
//	  {
//	    printf("CardIdIn:%x\r\n",CardIdIn);
//	  }
//	delay_ms(500);
	vTaskDelay( pdMS_TO_TICKS(5) );
    }

}

void vTaskKeyScan(void * pvParameters)
{
#define SOFTKEYUP	1
#define SOFTKEYDOWN	2
  static int keyvalue=0;
  xKeyIn KeyIn;
  xVoice xKeyVoice;
  uint8_t SotfKeyUpFlag = SOFTKEYUP,SotfKeyBef = 0;;
  KeyIn.source = 0;
  for(;;)
    {
      if(syscb.page == PAGE_COST)
	{
	  keyvalue=key_scan();		//OSdelay 替换
	  if((keyvalue != 0xff)&&(keyvalue != 0))
	    {
	      KeyIn.value=KeyConversion(keyvalue);
	      KeyIn.source = SOURCE_IN_KEY;
    //	  if(xQueueSend(queueKeyIn,&KeyIn,10) != pdPASS)
	      if(xQueueOverwrite(queueKeyIn,&KeyIn) != pdPASS)
		{
		  printf("Err:%s:%d\r\n",__FILE__,__LINE__);
		}
	      else
		{
		  xKeyVoice.voicelist = KeyVoiceArray;
		  xKeyVoice.udlen = 1;
		  if(xQueueOverwrite(queueVoice,&xKeyVoice) != pdPASS)	//语音
		    {
		      printf("Err:%s:%d\r\n",__FILE__,__LINE__);
		    }
		}
	    }
	  else
	    {
	      if(FT5206_Scan(0)!=0)
		{
		  SotfKeyBef = TouchPadConversion();
		  SotfKeyUpFlag = SOFTKEYDOWN;
		}
	      else
		{
		  SotfKeyUpFlag = SOFTKEYUP;
		}

	      if((SotfKeyUpFlag == SOFTKEYUP)&&(SotfKeyBef < KEY_MAX) && (SotfKeyBef > 0))
		{
		  KeyIn.value = SotfKeyBef;
		  if(KeyIn.value != 0xff)
		  {
		    KeyIn.source = SOURCE_IN_TP;
    //		printf("softKey:%u\r\n",KeyIn.value);
	//		  if(xQueueSend(queueKeyIn,&KeyIn,10) != pdPASS)
		    if(xQueueOverwrite(queueKeyIn,&KeyIn) != pdPASS)
		      {
			printf("Err:%s:%d\r\n",__FILE__,__LINE__);
		      }
		    else
		      {
			xKeyVoice.voicelist = KeyVoiceArray;
			xKeyVoice.udlen = 1;
			if(xQueueOverwrite(queueVoice,&xKeyVoice) != pdPASS)	//语音
			  {
			    printf("Err:%s:%d\r\n",__FILE__,__LINE__);
			  }
		      }

		  }
		  SotfKeyBef = 0xff;
		  SotfKeyUpFlag = 0;
		}

	      }
	  }
      else if(syscb.page == PAGE_CHECK)
	{
	  if(FT5206_Scan(0)!=0)
	  {
	    SotfKeyBef = CheckPageKeyConversion();
	    SotfKeyUpFlag = SOFTKEYDOWN;
//	    printf("SotfKeyBef:%d\n",SotfKeyBef);
	  }
	else
	  {
	    SotfKeyUpFlag = SOFTKEYUP;
	  }

	  if((SotfKeyUpFlag == SOFTKEYUP)&& (SotfKeyBef > 0))
	    {
	      KeyIn.value = SotfKeyBef;
	      if(KeyIn.value != 0xff)
	      {
		KeyIn.source = SOURCE_IN_TP;
//  		printf("softKey:%u\r\n",KeyIn.value);
		if(xQueueOverwrite(queueKeyIn,&KeyIn) != pdPASS)
		  {
		    printf("Err:%s:%d\r\n",__FILE__,__LINE__);
		  }
		else
		  {
		    xKeyVoice.voicelist = KeyVoiceArray;
		    xKeyVoice.udlen = 1;
		    if(xQueueOverwrite(queueVoice,&xKeyVoice) != pdPASS)	//语音
		      {
			printf("Err:%s:%d\r\n",__FILE__,__LINE__);
		      }
		  }

	      }
	      SotfKeyBef = 0xff;
	      SotfKeyUpFlag = 0;
	    }
	}
      else if(syscb.page == PAGE_SETTING)
	{
	  if(FT5206_Scan(0)!=0)
	 	  {
	 	    SotfKeyBef = SettingPageKeyConversion();
	 	    SotfKeyUpFlag = SOFTKEYDOWN;
	 //	    printf("SotfKeyBef:%d\n",SotfKeyBef);
	 	  }
	 	else
	 	  {
	 	    SotfKeyUpFlag = SOFTKEYUP;
	 	  }

	 	  if((SotfKeyUpFlag == SOFTKEYUP)&& (SotfKeyBef > 0))
	 	    {
	 	      KeyIn.value = SotfKeyBef;
	 	      if(KeyIn.value != 0xff)
	 	      {
	 		KeyIn.source = SOURCE_IN_TP;
	 //  		printf("softKey:%u\r\n",KeyIn.value);
	 		if(xQueueOverwrite(queueKeyIn,&KeyIn) != pdPASS)
	 		  {
	 		    printf("Err:%s:%d\r\n",__FILE__,__LINE__);
	 		  }
	 		else
	 		  {
	 		    xKeyVoice.voicelist = KeyVoiceArray;
	 		    xKeyVoice.udlen = 1;
	 		    if(xQueueOverwrite(queueVoice,&xKeyVoice) != pdPASS)	//语音
	 		      {
	 			printf("Err:%s:%d\r\n",__FILE__,__LINE__);
	 		      }
	 		  }

	 	      }
	 	      SotfKeyBef = 0xff;
	 	      SotfKeyUpFlag = 0;
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
        if(xQueueReceive(queueVoice,&VoiceList,portMAX_DELAY) == pdPASS)
	  {
            if(AllVoiceSwitch == 1)
              {
		vTaskDelay( pdMS_TO_TICKS(6) );
    //            taskENTER_CRITICAL();
		if(VoiceList.udlen == 1)
		  SinglePlay((*VoiceList.voicelist));
		else
		  {
		    ContinuePlay(VoiceList.voicelist,VoiceList.udlen);
		  }
    //            taskEXIT_CRITICAL();
              }
            else
              {
        	// 语音没有打开
              }

	  }
	else
	  {
	    printf("vTaskVoiceBoardcast Err Line:%d\r\n",__LINE__);
	  }
        taskYIELD();
      }
}


void vTaskRFIDReadCard(void * pvParameters)
{
  uint32_t CardId=0,CardIdTemp=0xFFFFFFFF;
    for(;;)
      {
//	taskENTER_CRITICAL();
	RFID_Get_ID((uint8_t *)&CardId);			//在使用时添加临界保护
//	taskEXIT_CRITICAL();
//	printf("CARDID:%x\r\n",CardId);
//	if((CardId != 0)&&(CardId != CardIdTemp))
//	  {
//	    xQueueOverwrite(queueCardIn,&CardId);
//	    CardIdTemp = CardId;
//	    CardId = 0;
//	  }else if((CardId == 0)&&(CardIdTemp != 0 ))
//	    {
//	      CardIdTemp =0;
//	      xQueueOverwrite(queueCardIn,&CardId);
//	    }
		xQueueOverwrite(queueCardIn,&CardId);
		CardId = 0;
		vTaskDelay( pdMS_TO_TICKS(500) );
      }
}



void vTaskMsgTrans(void * pvParameters)
{
	xKeyIn KeyInTemp;
	uint8_t KeyBuffer[8]={0},i;
	uint8_t posi = 0;
	uint32_t CardInTemp=0,_CardInTemp = 0xFFFFFFFF,RegCaidID;
	xqueueLcdShow xLcdShow = {KeyBuffer,0,0};
	PayRecord_t UserToAdd={0};
	uint8_t voicelsts[2]={0};
	xVoice VoiceList={voicelsts,0};
    for(;;)
      {
	if(syscb.page == PAGE_CHECK)
	  {
	    if(xQueueReceive(queueKeyIn,&KeyInTemp,0) == pdPASS)		//按键输入信息
	      {
		if(KeyInTemp.value == KEY_HOMEPAGE)
		  {
//		    syscb.page = PAGE_COST;	//到LCD任务切回
		    xLcdShow.Type = TOPAGECOST;
		    xQueueOverwrite(queueLCDShow,&xLcdShow);
		  }
		else if(KeyInTemp.value == KEY_PAGEBEF)
		  {

		  }
		else if(KeyInTemp.value == KEY_PAGENEXT)
		  {

		  }
	      }
	  }
	else if(syscb.page == PAGE_SETTING)
	  {
	    if(xQueueReceive(queueKeyIn,&KeyInTemp,0) == pdPASS)		//按键输入信息
	      {
		if(KeyInTemp.value == KEY_HOMEPAGE)
		  {
//		    syscb.page = PAGE_COST;	//到LCD任务切回
		    xLcdShow.Type = TOPAGECOST;
		    xQueueOverwrite(queueLCDShow,&xLcdShow);
		  }
		else if(KeyInTemp.value == KEY_REGCARD)
		  {
		    HideCardRegOK();
		    for(i=0;i<12;i++)
		      {
			xQueueReceive(queueCardIn,&RegCaidID,pdMS_TO_TICKS(500));

			if((RegCaidID != 0))
			  {
			    UserToAdd.ID = RegCaidID;
			    if(AddUser(&UserToAdd)==0)
			      {
//				printf("REG Card OK\r\n");
				  VoiceList.voicelist[0] = VOICE_CORRECT;
				  VoiceList.udlen = 1;
				  xQueueOverwrite(queueVoice,&VoiceList);
				ShowCardRegOK();pdMS_TO_TICKS(100);HideCardRegOK();pdMS_TO_TICKS(100);ShowCardRegOK();
				break;
			      }
			    else
//			      printf("Repeat ID\r\n");
			      VoiceList.voicelist[0] = VOICE_WARNING;
			      VoiceList.udlen = 1;
			      xQueueOverwrite(queueVoice,&VoiceList);
			      ShowCardRegFail();pdMS_TO_TICKS(100);HideCardRegFail();pdMS_TO_TICKS(100);ShowCardRegFail();
			      break;
			  }
		      }
		    if(i==12)
		      {
			 VoiceList.voicelist[0] = VOICE_WARNING;
			VoiceList.udlen = 1;
			xQueueOverwrite(queueVoice,&VoiceList);
			ShowCardRegFail();
		      }
		  }
		else if(KeyInTemp.value == KEY_VOICESWITCH)
		  {
		    AllVoiceSwitch = 1 - AllVoiceSwitch;
		    if(AllVoiceSwitch == 1)
		      ShowVoiceOn();
		    else
		      ShowVoiceOff();
		  }
		else if(KeyInTemp.value == KEY_CARDCHARGE)
		  {
		    //这个涉及到显示屏和按键

		    xKeyIn chargekey={0,0};
		    uint8_t chargeKeyBuffer[8]={0};
		    xqueueLcdShow xChargeShow;
		    uint32_t ChargeCardId;
		    uint32_t chargeAmount=0;
		    PayRecord_t Chargeuser={0,0,0,0};
		    GuiPageSettingInput();
		    syscb.InputMode = 1;
		    posi = 0;
		    memset(chargeKeyBuffer,0,sizeof(chargeKeyBuffer));
		    do
		      {
			if(xQueueReceive(queueKeyIn,&chargekey,portMAX_DELAY) == pdPASS){}
//			  printf("charge Key:%d\r\n",chargekey.value);
			if(posi >= 6){posi = 0;memset(chargeKeyBuffer,0,sizeof(chargeKeyBuffer));}
			if(chargekey.value>=KEY_1&&chargekey.value<=KEY_BACK)chargeKeyBuffer[posi++]=chargekey.value;

			 if(chargekey.value == KEY_BACK)
			  {
				if(posi>=2)	posi-=2;
				else		posi=0;
				chargeKeyBuffer[posi] = 0;
				chargeKeyBuffer[posi+1] = 0;

			  }else if(chargekey.value == KEY_ENT)
			    {
			      //处理，得到金额值
			      for (uint8_t i=0;chargeKeyBuffer[i]!=0&&i<6;i++)
				{
				  if( chargeKeyBuffer[i] != KEY_PLOT)
				    {
				      if(chargeKeyBuffer[i] != KEY_0)
				      chargeAmount =chargeAmount*10+chargeKeyBuffer[i];
				      else
				      chargeAmount =chargeAmount*10;
				    }
				  else
				    {
				      if(chargeKeyBuffer[i+1] != KEY_0)
				      chargeAmount =chargeAmount*10+chargeKeyBuffer[i+1];
				      else
				      chargeAmount =chargeAmount*10;
				      break;
				    }
				}

			      for (uint8_t j=0;j<6;j++)
				{
				  if(j>=6-1){chargeAmount*=10;break;}
				  if(chargeKeyBuffer[j]!=KEY_PLOT)continue;
				  if(j<6-1) break;
				  chargeAmount*=10;  break;
				}
			      break;
			    }
			  else if(chargekey.value == KEY_ESC)		//退出充值
			    {
			      posi = 0;
			      memset(chargeKeyBuffer,0,sizeof(chargeKeyBuffer));
			      GuiPageSettingInit();
			      goto nullnext;
			      break;
			    }
			  xChargeShow.Buffer = chargeKeyBuffer;
			  xChargeShow.Type   = CHARGEDISPLAY;		//在显示任务中显示
			  xChargeShow.len    = posi;
			  xQueueOverwrite(queueLCDShow,&xChargeShow);	//LCD
		      }while(chargekey.value != KEY_ENT || chargekey.value != KEY_ESC);

		    //第一次如果输入小数，会跳回主页面的问题？？？
		    printf("charge amount:%d\r\n",chargeAmount);
		    ChargeCardId = 0;
		    i=16;
		    do {
			xQueueReceive(queueCardIn,&ChargeCardId,pdMS_TO_TICKS(500));
		       }while((ChargeCardId == 0)&&(i--));

		    Chargeuser.ID = ChargeCardId;
		    CheckUser(&Chargeuser);
		    if(ChargeCardId != 0)
		    printf("cardID:%x\r\n",ChargeCardId);

//		    if(ChargeCardId != 0)	//已转换
//		      {
//			uint8_t *pchargeBuffer;
//			  for (pchargeBuffer=chargeKeyBuffer;*(pchargeBuffer + 1)!=0;pchargeBuffer++)
//			  {
//				  if((*pchargeBuffer) == KEY_0) *pchargeBuffer =0;
//				  if((*pchargeBuffer) != KEY_PLOT)
//				    chargeAmount = chargeAmount*10 + *pchargeBuffer;
//				  continue;
//			  }
//			  if(*(pchargeBuffer-2) != KEY_PLOT)
//			    chargeAmount*=10;
//		      }

		    if((ChargeCardId != 0)&&(ChargeUser(&Chargeuser,chargeAmount) == 0))
		      {
//			printf("charge ok\r\n");
			  VoiceList.voicelist[0] = VOICE_CORRECT;
			  VoiceList.udlen = 1;
			  xQueueOverwrite(queueVoice,&VoiceList);
			  GuiPageSettingInit();
			  ShowCardChargeOK();
		      }
		    else
		      {
//			printf("charge fail\r\n");
			  VoiceList.voicelist[0] = VOICE_WARNING;
			  VoiceList.udlen = 1;
			  xQueueOverwrite(queueVoice,&VoiceList);
			  GuiPageSettingInit();
			  ShowCardChargeFail();
		      }

//		    syscb.page = PAGE_COST;
//		    GuiPageCostInit();
		      posi = 0;
		      memset(chargeKeyBuffer,0,sizeof(chargeKeyBuffer));
		      goto nullnext;

		  }
	      }
	    else
	      {
nullnext:
		KeyInTemp.value = 0;
	      }
	  }


	if(syscb.page != PAGE_COST) goto errret;
		if(xQueueReceive(queueKeyIn,&KeyInTemp,0) == pdPASS)		//按键输入信息
		{
		    // 选择支付方式单独处理
			if(posi > 6)				//最大999.9元 最后一字节标记结束，不可用
				posi = 0;
				KeyBuffer[posi++]=KeyInTemp.value;
			if((KeyInTemp.value == KEY_ENT)||(KeyInTemp.value == KEY_ESC)||(KeyInTemp.value == KEY_CHECK)||(KeyInTemp.value == KEY_SETTING))
			{
			    // 添加 扫码支付和刷卡支付 这两个按键的情况
			      if(KeyInTemp.value == KEY_ENT)
				{
				  KeyBuffer[posi] = '\0';
				  xQueueOverwrite(queueKeyBuffer,KeyBuffer);
				  memset(KeyBuffer,0,12);
				  posi = 0;
				  goto cardpeek;
				}
			      else if(KeyInTemp.value == KEY_ESC)
				{
				  KeyBuffer[0] = KEY_ESC;
				  KeyBuffer[1] = '\0';
				  xQueueOverwrite(queueKeyBuffer,KeyBuffer);
				  posi = 0;
				  memset(KeyBuffer,0,sizeof(KeyBuffer));
				}
			      else if(KeyInTemp.value == KEY_CHECK)
				{
				  posi = 0;
				  memset(KeyBuffer,0,sizeof(KeyBuffer));
				  syscb.page = PAGE_CHECK;

				  xLcdShow.Type = TOPAGECHECK;
				  xQueueOverwrite(queueLCDShow,&xLcdShow);

				}
			      else if(KeyInTemp.value == KEY_SETTING)
				{
				  posi = 0;
				  memset(KeyBuffer,0,sizeof(KeyBuffer));
				  syscb.page = PAGE_SETTING;

				  xLcdShow.Type = TOPAGESETTING;
				  xQueueOverwrite(queueLCDShow,&xLcdShow);

				}
			}else if(KeyInTemp.value == KEY_BACK)
			{
				if(posi>=2)	posi-=2;
				else		posi=0;
				KeyBuffer[posi] = 0;
				KeyBuffer[posi+1] = 0;
//				printf("KeyBuffer: %x %x %x %x\r\n",KeyBuffer[0],KeyBuffer[1],KeyBuffer[2],KeyBuffer[3]);
//				printf("posi = %d\r\n",posi);
			}
			/*
			 * 再创建一个队列，用于显示临时的buffer到LCD上
			 * */
			if(syscb.page == PAGE_COST)
			  {
			    if((posi > 0)&&(posi <6))
			      {
				xLcdShow.len = posi;
				xLcdShow.Type = COST_MONEY_BUFFER;
				xQueueOverwrite(queueLCDShow,&xLcdShow);
			      }
			    else if((posi >=6)||(posi == 0))
			      {
				memset(KeyBuffer,0,sizeof(KeyBuffer));
				posi = 0;
				xLcdShow.len = posi;
				xLcdShow.Type = COST_BUFFER_CLEAR;
				xQueueOverwrite(queueLCDShow,&xLcdShow);
			      }
			  }

			//补充清除和隐藏 显示金额的函数
		}
cardpeek:
		if(xQueueReceive(queueCardIn,&CardInTemp,0) == pdPASS)		//Card输入信息
		{
			if((CardInTemp != 0)&&(CardInTemp != _CardInTemp))
			  {
				xQueueOverwrite(queueCardBuffer,&CardInTemp);
				_CardInTemp = CardInTemp;
//				CardInTemp = 0;
			  }else if((CardInTemp == 0))
				{
				  _CardInTemp =0;
				  xQueueOverwrite(queueCardBuffer,&CardInTemp);
				}

		}
errret:
		vTaskDelay( pdMS_TO_TICKS(4) );
      }
}

uint8_t xCostVoice[10];
void vTaskCostPage(void * pvParameters)
{
    uint8_t KeyBuffer[12],bufferlen;
    uint8_t i,*pKeyBuffer;
    uint32_t CardIDBuffer;
    uint32_t CostAmount=0;
    uint16_t ToCost = 0;
    PayRecord_t Auser={0,0,0,0};
    xVoice VoiceList={xCostVoice,0};
    xqueueLcdShow xLcdShow;
    struct FlagSet{
    	unsigned CardUp:1;
    	unsigned CardOn:1;
    	unsigned WaitingCost:1;
    }xFlag;
////////////    xFlag      Init      /////////////
    for(;;)
      {
    	if(syscb.page == PAGE_COST)
    	{
    		if(xQueueReceive(queueKeyBuffer,KeyBuffer,0) == pdPASS)		//len = strlen(KeyBuffer)-1 ,最后一字节为确定，取消键之类的
    		{
//    			for (i=0;i<12;i++)printf("keybuffer:%d\r\n",KeyBuffer[i]);
//				printf("len:%d\r\n",strlen(KeyBuffer));
    			bufferlen = strlen(KeyBuffer)-1;
    			if(bufferlen == 0)
    			  {
			      ToCost = 0;
			      CostAmount = 0;
    			  }
    		}
    		if(( bufferlen > 0)&&(bufferlen < 6))
    		{
    			for (pKeyBuffer=KeyBuffer;*(pKeyBuffer + 1)!=0;pKeyBuffer++)
    			{
    				if((*pKeyBuffer) == KEY_0) *pKeyBuffer =0;
    				if((*pKeyBuffer) != KEY_PLOT)
    					CostAmount = CostAmount*10 + *pKeyBuffer;
    				continue;
    			}
    			if(*(pKeyBuffer-2) != KEY_PLOT)
    			  CostAmount*=10;

    			if(*(pKeyBuffer) == KEY_ENT)		//该键表示消费，还有其他
    			{
    				printf("CostAmount:%5d角\r\n",CostAmount);
    				if(CostAmount > 0 )
    				{
    					if(CostAmount < 10000)
    					{
							xFlag.WaitingCost = 1;
							ToCost = CostAmount;
							CostAmount = 0;
							if(ToCost<10)	//0.5 元
							  {
							    i=0;
							    VoiceList.voicelist[i++] = VOICE_ZERO;
							    VoiceList.voicelist[i++] = VOICE_PLOT;
							    VoiceList.voicelist[i++] = ToCost;
							    VoiceList.voicelist[i++] = VOICE_YUAN;
							    VoiceList.udlen = i;
							  }
							else if(ToCost<100)	//x.x
							  {
							    i=0;
							    VoiceList.voicelist[i++] = ToCost/10;
							    VoiceList.voicelist[i++] = VOICE_PLOT;
							    VoiceList.voicelist[i++] = ToCost%10==0?VOICE_ZERO:ToCost%10;
							    VoiceList.voicelist[i++] = VOICE_YUAN;
							    VoiceList.udlen = i;
							  }
							else if(ToCost<1000)	//xx.x
							  {
							    i=0;
							    VoiceList.voicelist[i++] = ToCost/100;
							    VoiceList.voicelist[i++] = VOICE_TEN;
							    VoiceList.voicelist[i++] = ((ToCost%100/10)==0)?VOICE_ZERO:((ToCost%100)/10);
							    if(VoiceList.voicelist[i-1] == VOICE_ZERO) --i;
							    VoiceList.voicelist[i++] = VOICE_PLOT;
							    VoiceList.voicelist[i++] = ToCost%10==0?VOICE_ZERO:ToCost%10;
							    VoiceList.voicelist[i++] = VOICE_YUAN;
							    VoiceList.udlen = i;
							  }
							else if(ToCost<10000)
							  {
							    i=0;
							    VoiceList.voicelist[i++] = ToCost/1000;
							    VoiceList.voicelist[i++] = VOICE_HUNDRED;
							    VoiceList.voicelist[i++] = (ToCost%1000/100==0)?VOICE_ZERO:(ToCost%1000/100);
							    VoiceList.voicelist[i++] = VOICE_TEN;
							    if(VoiceList.voicelist[i-2] == VOICE_ZERO) i-=1;
							    VoiceList.voicelist[i++] = (ToCost%100/10==0)?VOICE_ZERO:(ToCost%100/10);
							    if(VoiceList.voicelist[i-1] == VOICE_ZERO) --i;
							    VoiceList.voicelist[i++] = VOICE_PLOT;
							    VoiceList.voicelist[i++] = ToCost%10==0?VOICE_ZERO:ToCost%10;
							    VoiceList.voicelist[i++] = VOICE_YUAN;
							    VoiceList.udlen = i;
							  }
							xQueueOverwrite(queueVoice,&VoiceList);

							SendPay(ToCost/10,ToCost%10);

    					}else
    					{
    						//一次不能超过999.9元
    						//错误处理
    						printf("超过单次交易上限\r\n");
						xFlag.WaitingCost = 0;
						ToCost = 0;
						CostAmount = 0;
						xLcdShow.Type = COST_BUFFER_CLEAR;
						xQueueOverwrite(queueLCDShow,&xLcdShow);

						VoiceList.voicelist[0] = VOICE_WARNING;
						VoiceList.udlen = 1;
						xQueueOverwrite(queueVoice,&VoiceList);

    					}

    				}
    				memset(KeyBuffer,0,12);
    			}else if(*(pKeyBuffer) == KEY_ESC)
    			{
    				CostAmount = 0;
    			}else if(*(pKeyBuffer) == KEY_CHECK)
    			{
//    				=============
    			}else if(*(pKeyBuffer) == KEY_SETTING)
    			{
//    				==============
    			}

			memset(KeyBuffer,0,12);
    		}
    		// Card check
checkCard:
    		if(xQueueReceive(queueCardBuffer,&CardIDBuffer,0) == pdPASS)	//放卡后会探测到一次，一旦离卡，立刻会变空
    		{
    		    if(CardIDBuffer != 0) {
    			Auser.ID = CardIDBuffer;
    			if( CheckUser(&Auser) == NOSUCHID)
    			  {
   			    printf("No Such ID\r\n");
    			    memset(&Auser,0,sizeof(Auser));
    			    xFlag.CardOn = 0;
    			    HideRemain();
			    ToCost = 0;
			    CostAmount = 0;
			    xLcdShow.Type = COST_BUFFER_CLEAR;
			    xQueueOverwrite(queueLCDShow,&xLcdShow);
    			    /*
    			     * 显示一些信息
    			     * */
			    DisplayCostFail_WHITE(ERR_NOSUCHID);LCDCostFailWhite(ERR_NOSUCHID);vTaskDelay( pdMS_TO_TICKS(100) );DisplayCostFail_BLACK(ERR_NOSUCHID);LCDCostFailBlack(ERR_NOSUCHID);vTaskDelay( pdMS_TO_TICKS(100) );
			    DisplayCostFail_WHITE(ERR_NOSUCHID);LCDCostFailWhite(ERR_NOSUCHID);vTaskDelay( pdMS_TO_TICKS(100) );DisplayCostFail_BLACK(ERR_NOSUCHID);LCDCostFailBlack(ERR_NOSUCHID);vTaskDelay( pdMS_TO_TICKS(100) );
			    HideCostFail();LCDCostHide();
			    HidePay();
    			  }
    			else
    			  {
//    			    printf("CardID:%x  ",Auser.ID);
//    			    printf("Remain:%x\r\n",Auser.Remain);
    			    SendRemain(Auser.Remain/10,Auser.Remain%10);	//HMI Send Reamin

    			    uint8_t LcdRemainShowBuffer[7];
    			    memset(LcdRemainShowBuffer,'\0',sizeof(LcdRemainShowBuffer));
    			    xLcdShow.Type = CARDREMAINSHOW;
    			    sprintf(LcdRemainShowBuffer,"%d.%d\0",Auser.Remain/10,Auser.Remain%10);
//    			    printf("%s\r\n",LcdRemainShowBuffer);
    			    xLcdShow.Buffer = LcdRemainShowBuffer;
    			    xQueueOverwrite(queueLCDShow,&xLcdShow);
    			    xFlag.CardOn = 1;								//放置card
    			  }
    		    }else
		    {
//		      printf("No Card On\r\n");
			  xFlag.CardOn = 0;
			  xFlag.CardUp =1;
		      memset(&Auser,0,sizeof(Auser));
		      HideRemain();
		      xLcdShow.Type = CARDREMAINHIDE;
		      xQueueOverwrite(queueLCDShow,&xLcdShow);
		    }
    		}



costlabel:	//HMI 屏幕反馈还没有添加
	  if((xFlag.WaitingCost == 1)&&(xFlag.CardUp == 1)&&(xFlag.CardOn == 1)&&(ToCost >0))
	    {
		  printf("balance to Cost: %5d\r\n",ToCost);
		  printf("Auser.ID: %x\r\n",Auser.ID);
	      //对卡片消费
	      if(CostUser(&Auser,ToCost) == 0)
			{
			  printf("cost OK\r\n");

			  RecentCostDetail[posiRecentCostDetail++] = Auser.ID;
			  RecentCostDetail[posiRecentCostDetail++] = ToCost;

			  VoiceList.voicelist[0] = VOICE_CORRECT;
			  VoiceList.udlen = 1;
			  xQueueOverwrite(queueVoice,&VoiceList);

			  xLcdShow.Type = COST_BUFFER_CLEAR;
			  xQueueOverwrite(queueLCDShow,&xLcdShow);
			  DisplayCostOK_WHITE();LCDCostOKWhite();vTaskDelay( pdMS_TO_TICKS(100) );DisplayCostOK_BLACK();LCDCostOKBlack();vTaskDelay( pdMS_TO_TICKS(100) );
			  DisplayCostOK_WHITE();LCDCostOKWhite();vTaskDelay( pdMS_TO_TICKS(100) );DisplayCostOK_BLACK();LCDCostOKBlack();vTaskDelay( pdMS_TO_TICKS(100) );
			  HideCostOK();LCDCostHide();
			  ToCost = 0;
			  CostAmount = 0;
			  xFlag.WaitingCost = 0;
			  xFlag.CardUp = 0;
			  SendRemain(Auser.Remain/10,Auser.Remain%10);
			  memset(&Auser,0,sizeof(Auser));
			  HidePay();
			}
	      else
			{
			  printf("Balance need to charge \r\n");

			  VoiceList.voicelist[0] = VOICE_WARNING;
			  VoiceList.udlen = 1;
			  xQueueOverwrite(queueVoice,&VoiceList);

			  xLcdShow.Type = COST_BUFFER_CLEAR;
			  xQueueOverwrite(queueLCDShow,&xLcdShow);
			  DisplayCostFail_WHITE(ERR_BALANCECHARGE);LCDCostFailWhite(ERR_BALANCECHARGE);vTaskDelay( pdMS_TO_TICKS(100) );DisplayCostFail_BLACK(ERR_BALANCECHARGE);LCDCostFailBlack(ERR_BALANCECHARGE);vTaskDelay( pdMS_TO_TICKS(100) );
			  DisplayCostFail_WHITE(ERR_BALANCECHARGE);LCDCostFailWhite(ERR_BALANCECHARGE);vTaskDelay( pdMS_TO_TICKS(100) );DisplayCostFail_BLACK(ERR_BALANCECHARGE);LCDCostFailBlack(ERR_BALANCECHARGE);vTaskDelay( pdMS_TO_TICKS(100) );
			  HideCostFail();LCDCostHide();
			  ToCost = 0;
			  CostAmount = 0;
			  xFlag.WaitingCost = 0;
			  xFlag.CardUp = 0;
			  HidePay();
			}

		}

		}
Taskyield:
    	vTaskDelay( pdMS_TO_TICKS(200) );
      }
}


void vTaskLCDMonitor (void * pvParameters)
{
  uint8_t i, costbuffer[10];	//最大999.9元,可以把专有变量放到对应.c的文件中使用
  xqueueLcdShow xLcdShow;
  for (;;)
    {
      if (xQueueReceive (queueLCDShow, &xLcdShow, portMAX_DELAY) == pdPASS)
	{
	  if(syscb.page == PAGE_COST)
	    {
	      switch (xLcdShow.Type)
		{
		case COST_MONEY_BUFFER:
		  memcpy (costbuffer, xLcdShow.Buffer, xLcdShow.len);
    //	      printf ("xLcdShow.Buffer:%x %x %x %x\r\n", xLcdShow.Buffer[0],xLcdShow.Buffer[1], xLcdShow.Buffer[2], xLcdShow.Buffer[3]);
    //	      printf("xLcdShow.len = %d\r\n",xLcdShow.len);
		  for (i = 0; i < xLcdShow.len; i++)
		    {
		      if ((costbuffer[i] > 0) && (costbuffer[i] < 10))
			costbuffer[i] += '0';
		      else if (costbuffer[i] == KEY_0)
			costbuffer[i] = '0';
		      else if (costbuffer[i] == KEY_PLOT)
			costbuffer[i] = '.';
		    }
		      costbuffer[i] = 0;
		      ClearCostMoney();
		  DisplayCostMoney (costbuffer);	//80毛
		  break;
		case COST_BUFFER_CLEAR:
		  memset(costbuffer,0,sizeof(costbuffer));
		  ClearCostMoney();
		  break;
		case CARDREMAINSHOW:
		  DispalyCardRemain(xLcdShow.Buffer);
		  break;
		case CARDREMAINHIDE:
		  clearCardRemain();
		  break;
		default:
		  break;
		}
	    }
	  else if(syscb.page == PAGE_CHECK)
	    {
	      switch (xLcdShow.Type)
	      {
		case TOPAGECHECK:
		  GuiPageCheckInit();
		  break;
		case TOPAGECOST:
		  GuiPageCostInit();
		  syscb.page = PAGE_COST;
		  break;
		default:
		  GuiPageCheckInit();
		  break;
	      }
	    }
	  else if(syscb.page == PAGE_SETTING)
	    {
	      switch (xLcdShow.Type)
	      {
		case TOPAGESETTING:
		  GuiPageSettingInit();
		  break;
		case TOPAGECOST:
		  GuiPageCostInit();
		  syscb.page = PAGE_COST;
		  break;
		case CHARGEDISPLAY:
		  GuiPageSettingChargeAmount(xLcdShow.Buffer);
		  break;
		default:
		  GuiPageSettingInit();
		  break;
	      }
	    }
	}
      taskYIELD();

    }
}

void vTaskFatfs (void * pvParameters)
{
  for(;;)
    {
//      taskYIELD();
      vTaskDelay( pdMS_TO_TICKS(5000) );
    }
}


//==================      app_main   func   ===========================

//1 队列功能测试
//2 带添加flash 消费记录改动，以及ID表头清除功能
int App_main()
{
  BaseType_t Retn;

  //================      QUEUE Create        ========================
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
  queueCardIn = xQueueCreate( Length_queueCardIn, Size_queueCardIn );
  if(queueCardIn == NULL)
    {
      printf("QUEUE:queueCardIn Create fail\r\n");
    }
  queueKeyBuffer =  xQueueCreate( Length_queueKeyBuffer, Size_queueKeyBuffer );
  if(queueKeyBuffer == NULL)
    {
      printf("QUEUE:queueKeyBuffer Create fail\r\n");
    }
  queueCardBuffer =  xQueueCreate( Length_queueCardIn, Size_queueCardIn );
  if(queueCardBuffer == NULL)
    {
      printf("QUEUE:queueCardBuffer Create fail\r\n");
    }
  queueLCDShow =  xQueueCreate( Length_queueLcdShow, Size_queueLcdShow );
  if(queueLCDShow == NULL)
    {
      printf("QUEUE:queueLCDShow Create fail\r\n");
    }
  queueUserMsg =  xQueueCreate( Length_queueUserMsg, Size_queueUserMsg );
  if(queueUserMsg == NULL)
    {
      printf("QUEUE:queueUserMsg Create fail\r\n");
    }

  //================      Task Create        ========================
  Retn = xTaskCreate( vTaskLed1, "Task_Led1", STK_SIZE_LED1, NULL, Prio_LED1, &TaskHandle_LED1 );
  if(Retn!=pdPASS)
  {
      printf("Task_Led1 create fail\r\n");
  }

//  Retn = xTaskCreate( vTaskPrint, "Task_Print", STK_SIZE_Print, NULL, Prio_Print, &TaskHandle_Print );
//  if(Retn!=pdPASS)
//  {
//      printf("Task_Print create fail\r\n");
//  }

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

  Retn = xTaskCreate( vTaskRFIDReadCard, "vTaskRFIDReadCard", STK_SIZE_RFIDReadCard, NULL, Prio_RFIDReadCard, &TaskHandle_RFIDReadCard );
  if(Retn!=pdPASS)
  {
      printf("vTaskRFIDReadCard create fail\r\n");
  }

  Retn = xTaskCreate( vTaskMsgTrans, "vTaskMsgTrans", STK_SIZE_MsgTrans, NULL, Prio_MsgTrans, &TaskHandle_MsgTrans );
  if(Retn!=pdPASS)
  {
      printf("vTaskMsgTrans create fail\r\n");
  }

  Retn = xTaskCreate( vTaskCostPage, "vTaskCostPage", STK_SIZE_CostPage, NULL, Prio_CostPage, &TaskHandle_CostPage );
  if(Retn!=pdPASS)
  {
      printf("vTaskCostPage create fail\r\n");
  }

  Retn = xTaskCreate( vTaskLCDMonitor, "vTaskLCDMonitor", STK_SIZE_LCDMonitor, NULL, Prio_LCDMonitor, &TaskHandle_LCDMonitor );
  if(Retn!=pdPASS)
  {
      printf("vTaskLCDMonitor create fail\r\n");
  }

  Retn = xTaskCreate( vTaskFatfs, "vTaskFatfs", STK_SIZE_Fatfs, NULL, Prio_Fatfs, &TaskHandle_Fatfs );
  if(Retn!=pdPASS)
  {
      printf("vTaskFatfs create fail\r\n");
  }

  vTaskStartScheduler();
  for(;;);
  return 0;
}
