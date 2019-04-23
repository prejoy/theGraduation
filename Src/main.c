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
  GuiPageCostInit();		//GUI  Init
  RCC522_Init();		//RFID Init
  HMI_Init();			//HMI Init
  UserTblInit(0);		//Cost Init

  uint8_t res;
  uint32_t total,free;
  exfuns_init();	//为FATFS申请内存
  res=f_mount(fs[2],"2:",1);	//0:SD卡  1：W25QXX Flash 2：Nand Flash
  if(res==0X0D)
  {
      printf("NAND Disk Formatting...\r\n");
      res=f_mkfs("2:",1,4096);
      if(res==0)
      {
	      f_setlabel((const TCHAR *)"2:NANDDISK");
	      printf("NAND Disk Format Finish\r\n");
      }else  printf("NAND Disk Format Error \r\n");
  }
  while(exf_getfree("2:",&total,&free))
  {
      printf("Nand Flash Fatfs Error!\r\n");
      delay_ms(200);
  }
  printf("Flash Total Size:  %d MB\r\n",total>>10);
  printf("Flash Free  Size:  %d MB\r\n",free>>10);
  printf("FATFS OK!\r\n");

//  GuiPageSettingInit();
//  GuiPageSettingInput();
//  uint8_t udRtn;
//  while(1)
//    {
//      if(FT5206_Scan(0)==1)
//	{
//	udRtn = SettingPageKeyConversion();
//	printf("udRtn:%d\r\n",udRtn);
//	}
//      HAL_Delay(500);
//    }

//  GuiPageCheckInit();
//  uint8_t udRtn;
//  while(1)
//    {
//            if(FT5206_Scan(0)==1)
//      	{
//      	udRtn = CheckPageKeyConversion();
//      	printf("udRtn:%d\r\n",udRtn);
//      	}
//            HAL_Delay(500);
//    }

//  DelAllUser(0);
//	PayRecord_t Auser1={0x73b36589,0,0,10000},
//		  Auser2={0x474eb089,0,0,12000};
//
//	AddUser(&Auser1);
//	AddUser(&Auser2);
//	CheckUser(&Auser2);
////	printf("0x474eb089 Remain:%u\r\n",Auser2.Remain);
//	CostUser(&Auser2,200|0x80000000);
//    uint32_t Spi_Table_Buffer[1024]={0};
//    uint32_t Spi_User_Buffer[1024]={0};
//    W25QXX_Read((u8*)Spi_Table_Buffer,0,4096);
//    W25QXX_Read((u8*)Spi_User_Buffer,0x1000,4096);


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
//  const char teststring[]="aabb file abcd\r\n";
//  char readbuf[20]={0};
//  uint32_t numberToWrite=16,index=16,pread;
////  f_Retn = f_mkdir("2:/test");
////	  printf("f_mkdir ret:%d\r\n",f_Retn);
//  f_Retn = f_open(&textfile,"2:/test/test.txt",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
//  	  printf("f_open ret:%d\r\n",f_Retn);
//
////  f_Retn = f_lseek(&textfile,textfile.obj.objsize);
////  	  printf("f_lseek ret:%d\r\n",f_Retn);
////  f_Retn = f_write(&textfile,(const void*)teststring,index,(UINT*)&numberToWrite);
////  	  printf("f_write ret:%d\r\n",f_Retn);
//  f_Retn = f_read(&textfile,readbuf,15,(UINT*)&pread);
//  printf("f_read ret:%d\r\n",f_Retn);
//  if(!f_Retn)
//  {
//	  printf("context:%s\r\n",readbuf);
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

//// HMI test
//  HMI_Init();
//  SendPay(12,2);
//  SendRemain(123,9);
//  SendQRcode("https://www.baidu.com/",sizeof("https://www.baidu.com/"));
//  while(1)
//    {
//      HAL_Delay(1000);
//      DisplayQRCode();
//      HAL_Delay(1000);
//      HideQRCode();
//      HAL_Delay(1000);
//      HidePay();
//      HAL_Delay(1000);
//      DisplayPay();
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
//  VoiceSetVolume(30);		//设置初始音量
//  HAL_Delay(5);
  SinglePlay(VOICE_POWERON);
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
