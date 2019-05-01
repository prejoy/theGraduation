/*
 * App_UserMsg.h
 *
 *  Created on: 2019Äê4ÔÂ27ÈÕ
 *      Author: prejoy
 */

#ifndef APP_INC_APP_USERMSG_H_
#define APP_INC_APP_USERMSG_H_

#include "stdint.h"

typedef enum{
  ReadFile	=	1,
  WriteFile,
  ReadPicture,
  WritePicture,
  HideFatfsMsg,
  HideFatfsPic
}FatfsOperationType;



typedef enum{
  GradeOne	=	1,
  GradeTwo,
  GradeThree,
  GradeFour,
  GradeFive,
  GradeSix,

}GradeType;

typedef enum{
  EE	=	1,
  SF	,
  FM	,
  YJ	,
  WL	,
  EN
}ProfessionType;


uint16_t pic_get_tnum(uint8_t *path);
uint8_t mf_copy(uint8_t *psrc,uint8_t *pdst,uint8_t fwmode);



#endif /* APP_INC_APP_USERMSG_H_ */
