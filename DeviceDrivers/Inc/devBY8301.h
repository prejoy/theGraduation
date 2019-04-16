/*
 * devBY8301.h
 *
 *  Created on: 2019Äê3ÔÂ15ÈÕ
 *      Author: prejoy
 */

#ifndef DEVICEDRIVERS_INC_DEVBY8301_H_
#define DEVICEDRIVERS_INC_DEVBY8301_H_

#include "stdint.h"
#include "stm32f4xx.h"



//1  SPK2		16  GND
//2  SPK1		15
//3			14
//4			13
//5			12
//6 VCC-5v		11
//7 TX			10
//8 RX			9
//
//V:1 6 7 8 16

/*listNo    voice
 * 1-9	    1-9
 * 10	    0
 * 11		q
 *
 * */

//private
#define VOICE_ONE	1
#define VOICE_TWO	2
#define VOICE_THREE	3
#define VOICE_FOUR	4
#define VOICE_FIVE	5
#define VOICE_SIX	6
#define VOICE_SEVEN	7
#define VOICE_EIGHT	8
#define VOICE_NINE	9
#define VOICE_ZERO	10
#define VOICE_KEY	11
#define VOICE_PLEASE	12
#define VOICE_PAY	13
#define VOICE_YUAN	14
#define VOICE_POWERON	15
#define VOICE_CORRECT	16
#define VOICE_WARNING	17
#define VOICE_SUCCESS	18
#define VOICE_FAIL	19
#define VOICE_TEN	20
#define VOICE_HUNDRED	21
#define VOICE_THOUSAND	22
#define VOICE_PLOT	23




extern UART_HandleTypeDef huart4;

uint32_t SinglePlay(uint8_t Voice_NO);
uint32_t ContinuePlay(uint8_t *list,uint8_t udLen);

uint32_t VoiceStatusCheck(void);
uint32_t VoiceSetVolume(uint32_t VolumeLevel);
uint32_t VoiceVolumeAdjust(uint32_t Direction);

#endif /* DEVICEDRIVERS_INC_DEVBY8301_H_ */
