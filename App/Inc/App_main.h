/*
 * App_main.h
 *
 *  Created on: 2019Äê1ÔÂ16ÈÕ
 *      Author: prejoy
 */

#ifndef APP_INC_APP_MAIN_H_
#define APP_INC_APP_MAIN_H_

#include <stdio.h>
#include "string.h"
#include "stm32f4xx.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "dma2d.h"
#include "ltdc.h"
#include "fmc.h"
#include "spi.h"

#include "sys.h"
#include "devDelay.h"
#include "devUsart.h"
#include "devKey.h"
#include "devLed.h"
#include "devSdram.h"
#include "devLtdc.h"
#include "devAT24C02.h"
#include "touch.h"
#include "devW25Qxx.h"
#include "MF_RC522.h"
#include "devBY8301.h"
#include "devHMIScreen.h"

#include "malloc.h"
#include "nand.h"
#include "ftl.h"

/*	Fatfs's  Include	*/
#include "ff.h"
#include "exfuns.h"

/*	GBK's  Include	*/
#include "text.h"
#include "fontupd.h"

/*	Picture's  Include	*/
#include "piclib.h"

/*	FreeRTOS's  Include	*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*	App's  Include	*/
#include "App_Pay.h"
#include "App_Gui.h"
#include "App_UserMsg.h"





int App_main(void);

#endif /* APP_INC_APP_MAIN_H_ */
