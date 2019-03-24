/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RFID_RST_Pin GPIO_PIN_3
#define RFID_RST_GPIO_Port GPIOE
#define RFID_CS_Pin GPIO_PIN_4
#define RFID_CS_GPIO_Port GPIOE
#define FT5206_TCS_Pin GPIO_PIN_8
#define FT5206_TCS_GPIO_Port GPIOI
#define W25Qxx_CS_Pin GPIO_PIN_6
#define W25Qxx_CS_GPIO_Port GPIOF
#define KEY_WK_Pin GPIO_PIN_0
#define KEY_WK_GPIO_Port GPIOA
#define EEPROM_SCL_Pin GPIO_PIN_4
#define EEPROM_SCL_GPIO_Port GPIOH
#define EEPROM_SDA_Pin GPIO_PIN_5
#define EEPROM_SDA_GPIO_Port GPIOH
#define LED0_Pin GPIO_PIN_1
#define LED0_GPIO_Port GPIOB
#define FT5206_TMISO_Pin GPIO_PIN_6
#define FT5206_TMISO_GPIO_Port GPIOH
#define FT5206_TPEN_Pin GPIO_PIN_7
#define FT5206_TPEN_GPIO_Port GPIOH
#define FT5206_TMOSI_Pin GPIO_PIN_3
#define FT5206_TMOSI_GPIO_Port GPIOI
#define LTDC_LIGHT_Pin GPIO_PIN_5
#define LTDC_LIGHT_GPIO_Port GPIOB

#define KEY_COL1_GPIO_Port GPIOA
#define KEY_COL2_GPIO_Port GPIOA
#define KEY_COL3_GPIO_Port GPIOA
#define KEY_COL4_GPIO_Port GPIOA
#define KEY_ROW1_GPIO_Port GPIOB
#define KEY_ROW2_GPIO_Port GPIOB
#define KEY_ROW3_GPIO_Port GPIOB
#define KEY_ROW4_GPIO_Port GPIOB
#define KEY_ROW5_GPIO_Port GPIOB
#define KEY_ROW1_Pin GPIO_PIN_10
#define KEY_ROW2_Pin GPIO_PIN_12
#define KEY_ROW3_Pin GPIO_PIN_13
#define KEY_ROW4_Pin GPIO_PIN_14
#define KEY_ROW5_Pin GPIO_PIN_15
#define KEY_COL1_Pin GPIO_PIN_3
#define KEY_COL2_Pin GPIO_PIN_4
#define KEY_COL3_Pin GPIO_PIN_5
#define KEY_COL4_Pin GPIO_PIN_6
#define KEY_COL_Pin_ALL (KEY_COL1_Pin|KEY_COL2_Pin|KEY_COL3_Pin|KEY_COL4_Pin)
#define KEY_ROW_Pin_ALL	(KEY_ROW1_Pin|KEY_ROW2_Pin|KEY_ROW3_Pin|KEY_ROW4_Pin|KEY_ROW5_Pin)
#define KEY_COL_GPIO_Port GPIOA
#define KEY_ROW_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
