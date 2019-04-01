/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
#include "spi.h"

/* Private includes ----------------------------------------------------------*/
#include "App_main.h"

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_I2C2_Init();
  MX_SPI5_Init();
  MX_SPI4_Init();
  MX_FMC_Init();
  MX_LTDC_Init();
  MX_DMA2D_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */
  printf("in application \r\n");
  SDRAM_Init();
  my_mem_init(SRAM);
  my_mem_init(SDRAM);
  W25QXX_Init();
  font_init();
  LTDC_Init();
  tp_dev.init();
//  HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE);//配置下次中断接收的缓冲区,每次准备接收下一次了就要清一下缓冲区,再这样配置一下,这相当于是一个控制块
  GuiPageCostInit();

  DisplayCostMoney(80);
//  while(1)
//    {
//      if(FT5206_Scan(0)!=0)
//      HAL_Delay(100);
//    }

//SPI4 test
//=============================================================================
//  		连线：MISO -  MISO   ！！！！！！！！！！！！！！！！！
// 			MOSI - MOSI  ！！！！！！！！！！！！！！！！！
//			 接线是反的，注意了！（）（）（）（）
//=============================================================================
//  uint8_t txDat=0xAC,rxDat=0xFF;
//  while(1)
//    {
//      txDat++;
//      CLR_SPI_CS();
//      HAL_SPI_TransmitReceive(&hspi4,&txDat,&rxDat,1,0x10);
//      SET_SPI_CS();
//      printf("rxDat:%d\r\n",rxDat);
//      HAL_Delay(1000);
//    }

//// w25qxx+Pay test
////  DelAllUser(0);
//  UserTblInit(0);
//
//  PayRecord_t Auser1={0x00ACCAFF,0,0,500},
//	      Auser2={0x12125678,0,0,300};
//
//  AddUser(&Auser1);
//  AddUser(&Auser2);
////  DelAllUser(0);
//  CheckUser(&Auser2);
//  CostUser(&Auser2,20);
//
//  uint8_t Spi_Table_Buffer[4096]={0};
//  uint8_t Spi_User_Buffer[4096]={0};
//  W25QXX_Read(Spi_Table_Buffer,0,4096);
//  W25QXX_Read(Spi_User_Buffer,Auser2.Addr,4096);

#if 0
// ltdc test
//draw user page 480X504	24号字体
//   (0,0)		(480,0)
//  ------		卡号：
//  |	 |		姓名：
//  |	 |		年级：
//  ------		专业：
//			余额：
//		本次消费金额：
//  扫码支付		刷卡支付
//   (0,504)		(480,504)

//  pic:（25-145，195）
#define PIC_SX		35
#define PIC_SY		60
#define PIC_WIDTH	130	//占用最大宽度PIC_SX+PIC_WIDTH不要超过 MSG_SX
#define PIC_HEIGHT	190

#define MSG_FONT	24
#define MSG_SX		200
#define MSG_SY		(PIC_SY)
#define MSG_LINESPACE	12
#define MSGn_SX(n)	MSG_SX
#define MSGn_SY(n)	(MSG_SY+(MSG_LINESPACE+MSG_FONT)*(n-1))
   char lcd_id[12];
   sprintf((char*)lcd_id,"LCD ID:7084");
   printf("%s\r\n",lcd_id);
   LTDC_Clear(BG_COLOR);

   LTDC_Fill(PIC_SX,PIC_SY,PIC_SX+PIC_WIDTH,PIC_SY+PIC_HEIGHT,WHITE);
   POINT_COLOR=BLACK;BACK_COLOR=BG_COLOR;
   Show_Str(MSGn_SX(1),MSGn_SY(1),480-MSG_SX,MSG_FONT,"卡号：",MSG_FONT,0);
   Show_Str(MSGn_SX(2),MSGn_SY(2),480-MSG_SX,MSG_FONT,"姓名：",MSG_FONT,0);
   Show_Str(MSGn_SX(3),MSGn_SY(3),480-MSG_SX,MSG_FONT,"年级：",MSG_FONT,0);
   Show_Str(MSGn_SX(4),MSGn_SY(4),480-MSG_SX,MSG_FONT,"专业：",MSG_FONT,0);
   Show_Str(MSGn_SX(5),MSGn_SY(5),480-MSG_SX,MSG_FONT,"余额：          元",MSG_FONT,0);
   Show_Str(30,280,480-30,32,"请输入消费金额：      元",32,0);

   POINT_COLOR=BLACK;BACK_COLOR=GRAY;
//   LTDC_Fill(120-40,380-25,120+40,380+25,GRAY);
//   LTDC_Fill(240-40,380-25,120+40,380+25,GRAY);
   Show_Str(120-24*2,(PIC_SY+PIC_HEIGHT+504)/2,24*4,24,"扫码支付",24,0);
   Show_Str(360-24*2,(PIC_SY+PIC_HEIGHT+504)/2,24*4,24,"刷卡支付",24,0);
//   LTDC_ShowString(10,110,240,16,16,"ATOM@ALIENTEK");
//   LTDC_ShowString(10,150,240,12,12,"2019/2/1");
//   Show_Str(30,230,300,16,"汉字一二三，无聊六六期阿萨德",16,0);

//===================    softkbd  =================
//===================    继续添加触摸屏功能	===
#define SOFT_KBD_LINECOLOR	0x001F	//blue
#define SOFT_KBD_FILLCOLOR	0xFFFF	//whiite
#define LINE_WIDTH		4
#define XPOSI(n)		(60+((n-1)%4)*120)
#define YPOSI(n)		(540+(n-1)/4*74)
#define FONTSIZE		32
//draw keyboard 480X296
//   (0,800-296)		(480,800-296)
//       296/4
//   (0,800-0)	480/4	(480,800-0)
    LTDC_Fill(0,800-1-296,480-1,800-1,SOFT_KBD_FILLCOLOR);				//fill必须从上到下，左到右
    LTDC_DrawLine(0,800-1-296,480-1,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);		//horizon
    LTDC_DrawLine(0,800-1-296+74*1,480-1,800-1-296+74*1,LINE_WIDTH,SOFT_KBD_LINECOLOR);
    LTDC_DrawLine(0,800-1-296+74*2,480-1,800-1-296+74*2,LINE_WIDTH,SOFT_KBD_LINECOLOR);
    LTDC_DrawLine(0,800-1-296+74*3,480-1,800-1-296+74*3,LINE_WIDTH,SOFT_KBD_LINECOLOR);
    LTDC_DrawLine(0+120,800-1,0+120,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);		//vercital
    LTDC_DrawLine(0+120*2,800-1,0+120*2,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);
    LTDC_DrawLine(0+120*3,800-1,0+120*3,800-1-296,LINE_WIDTH,SOFT_KBD_LINECOLOR);

    POINT_COLOR=BLACK;BACK_COLOR=WHITE;
    Show_Str((XPOSI(1)-FONTSIZE/4),(YPOSI(1)-FONTSIZE/2),FONTSIZE,FONTSIZE,"1",FONTSIZE,0);
    Show_Str((XPOSI(2)-FONTSIZE/4),(YPOSI(2)-FONTSIZE/2),FONTSIZE,FONTSIZE,"2",FONTSIZE,0);
    Show_Str((XPOSI(3)-FONTSIZE/4),(YPOSI(3)-FONTSIZE/2),FONTSIZE,FONTSIZE,"3",FONTSIZE,0);
    Show_Str((XPOSI(5)-FONTSIZE/4),(YPOSI(5)-FONTSIZE/2),FONTSIZE,FONTSIZE,"4",FONTSIZE,0);
    Show_Str((XPOSI(6)-FONTSIZE/4),(YPOSI(6)-FONTSIZE/2),FONTSIZE,FONTSIZE,"5",FONTSIZE,0);
    Show_Str((XPOSI(7)-FONTSIZE/4),(YPOSI(7)-FONTSIZE/2),FONTSIZE,FONTSIZE,"6",FONTSIZE,0);
    Show_Str((XPOSI(9)-FONTSIZE/4),(YPOSI(9)-FONTSIZE/2),FONTSIZE,FONTSIZE,"7",FONTSIZE,0);
    Show_Str((XPOSI(10)-FONTSIZE/4),(YPOSI(10)-FONTSIZE/2),FONTSIZE,FONTSIZE,"8",FONTSIZE,0);
    Show_Str((XPOSI(11)-FONTSIZE/4),(YPOSI(11)-FONTSIZE/2),FONTSIZE,FONTSIZE,"9",FONTSIZE,0);
    Show_Str((XPOSI(13)-FONTSIZE/4),(YPOSI(13)-FONTSIZE/2),FONTSIZE,FONTSIZE,"0",FONTSIZE,0);
    Show_Str((XPOSI(14)-FONTSIZE/4),(YPOSI(14)-FONTSIZE/2),FONTSIZE,FONTSIZE,".",FONTSIZE,0);
    POINT_COLOR=BROWN;
    Show_Str((XPOSI(4)-FONTSIZE),(YPOSI(4)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"查询",FONTSIZE,0);
    Show_Str((XPOSI(8)-FONTSIZE),(YPOSI(8)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"设置",FONTSIZE,0);
    Show_Str((XPOSI(12)-FONTSIZE),(YPOSI(12)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"取消",FONTSIZE,0);
    Show_Str((XPOSI(15)-FONTSIZE/2),(YPOSI(15)-FONTSIZE/2),FONTSIZE,FONTSIZE,"←",FONTSIZE,0);
    Show_Str((XPOSI(16)-FONTSIZE),(YPOSI(16)-FONTSIZE/2),FONTSIZE*2,FONTSIZE,"确定",FONTSIZE,0);

  uint32_t x=0,LCDswitch=0;
   while(1)
  {
       //画直线 == 画矩形 == LTDC_Fill 矩形单色填充
//       LTDC_DrawLine
       if(LCDswitch == 1)
	 LTDC_Switch(0);
       if(LCDswitch == 2)
	 LTDC_Switch(1);
       x++;
       if(x==12)x=0;
       toggleLed(0);
       HAL_Delay(1000);
   }

#endif
//fatfs test part
//  uint8_t res;
//  uint32_t total,free;
//  exfuns_init();	//为FATFS申请内存
//  res=f_mount(fs[2],"2:",1);	//0:SD卡  1：W25QXX Flash 2：Nand Flash
//  if(res==0X0D)
//  {
//	printf("NAND Disk Formatting...\r\n");
//	res=f_mkfs("2:",1,4096);
//	if(res==0)
//	{
//		f_setlabel((const TCHAR *)"2:NANDDISK");
//		printf("NAND Disk Format Finish\r\n");
//	}else  printf("NAND Disk Format Error \r\n");
//  }
//  while(exf_getfree("2:",&total,&free))
//  {
//	printf("Nand Flash Fatfs Error!\r\n");
//	delay_ms(200);
//  }
//  printf("FATFS OK!\r\n");
//  printf("Flash Total Size:  %d MB\r\n",total>>10);
//  printf("Flash Free  Size:  %d MB\r\n",free>>10);
//
//  FIL  textfile;
//  uint32_t f_Retn=0;
//  const char teststring[]="test file abcd\r\n";
//  char readbuf[20]={0};
//  uint32_t numberToWrite=16,index=16,pread;
//  f_Retn = f_open(&textfile,"2:/test/test.txt",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
//  	  printf("f_open ret:%d\r\n",f_Retn);
////  f_Retn = f_lseek(&textfile,textfile.obj.objsize);
////  	  printf("f_lseek ret:%d\r\n",f_Retn);
////  f_Retn = f_write(&textfile,(const void*)teststring,index,(UINT*)&numberToWrite);
////  	  printf("f_write ret:%d\r\n",f_Retn);
//  f_Retn = f_read(&textfile,readbuf,15,(UINT*)&pread);
//  printf("f_read ret:%d\r\n",f_Retn);
//  if(!f_Retn)
//  {
//	  printf("context:%c%c%c%c\r\n",readbuf[0],readbuf[1],readbuf[2],readbuf[3]);
//  }
//  f_Retn = f_close(&textfile);
//  printf("f_close ret:%d\r\n",f_Retn);



  //existing dirs:  2:/test
  //existing files: 2:/test/test.txt
/*mkdirs:
  	      f_Retn = f_mkdir("2:/test");
  	  	  printf("f_mkdir ret:%d\r\n",f_Retn);
  readfiles:
  	  	  f_Retn = f_open(&textfile,"2:/test/test.txt",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
  	  	  printf("f_open ret:%d\r\n",f_Retn);
		  f_Retn = f_read(&textfile,readbuf,15,(UINT*)&pread);
		  printf("f_read ret:%d\r\n",f_Retn);
		  f_Retn = f_close(&textfile);
  	  	  printf("f_close ret:%d\r\n",f_Retn);
  writefiles:
		  f_Retn = f_open(&textfile,"2:/test/test.txt",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
  	  	  printf("f_open ret:%d\r\n",f_Retn);
  	  	  f_Retn = f_lseek(&textfile,textfile.obj.objsize);
  		  printf("f_lseek ret:%d\r\n",f_Retn);
		  f_Retn = f_write(&textfile,(const void*)teststring,index,(UINT*)&numberToWrite);
  		  printf("f_write ret:%d\r\n",f_Retn);
		  f_Retn = f_close(&textfile);
		  printf("f_close ret:%d\r\n",f_Retn);

 * */


//  //key_scan
//  int keyvalue=0;
//  while(1)
//    {
//      keyvalue = key_scan();
//      if(keyvalue!=0xff)
//	printf("keyvalue:%d\r\n",keyvalue);
//      OSDelay_ms(35);
//    }


//汉字显示，需要初始化W25Qxx,
//  POINT_COLOR=RED;
//  char chinese[]={0xce,0xd2,0xca,0xc7,0xc5,0xcb,0xbc,0xd1,0xce,0xb0,0};//(2) 直接自己制作几个需要的汉字放入flash？？
//  Show_Str(30,30,200,16,"汉字一二三，无聊六六期阿萨德",16,0);


//uart4,5 test
//  uint8_t transdata[10]={'a','b','c','d','\r','\n',0};
//  while(1)
//    {
//      HAL_UART_Transmit(&huart4,transdata,6,0xffff);
//      HAL_Delay(1000);
//      HAL_UART_Transmit(&huart5,transdata,6,0xffff);
//      HAL_Delay(1000);
//    }


//BY8301 test
//  uint8_t voiceList[10]={21,22,23,14,15,16,17,18,19,20};
//  while (1)
//    {
//      ContinuePlay(voiceList,3);
//      HAL_Delay(8000);
////      SinglePlay(2);
//    }

//Next:
/*  完成刷卡机的默认界面，作为背景，刷入NAND flash
 *  完成按键配合语音输入
 * */

//RTOS任务：
/*  （1） 刷卡，读ID
 *  （2）按键输入（20Key）
 *  （3）LTDC显示用户信息
 *  （）  屏幕按键 用于输入金额？
 *  （4）语音播放金额
 *  （）  写入记录账户金额变化 信息？
 *  （5） 联网上报信息
 *  （） 功能键查看用户详细信息？
 * */
  App_main();
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 80;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
