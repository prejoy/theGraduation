/*
 * App_Pay.h
 *
 *  Created on: 2019年3月21日
 *      Author: prejoy
 */

#ifndef APP_INC_APP_PAY_H_
#define APP_INC_APP_PAY_H_

#include "App_main.h"


 /*
  * 读写w25q256 使用了4096字节对齐
 * 表头使用4096Byte，可以存放255-1个卡片交易信息
 * 每个ID的交易记录也用4096Byte，可以记录最多512条交易记录，
 * 最后96字节保留，只记录500条记录
 * 交易记录表头：ID	Addr	Null	NULL	...	check=0x1234567890abcdef	UserNum
 * 		4B 	4B	4B	4B		4096-16				4096-8b
 * 交易记录：	pay	NULL   Offset remain
 * 		4B	4B	end-96  end-96+4
 */
//extern u8 UserTable[((MAX_UID-1)/256+1)*4096];

#define PSUCCESS	0
#define PFAIL		1
#define REPEATID	10
#define NOSUCHID	11
#define BALANCECHANGE	20
#define USERMSGERR	21



typedef struct {
  uint32_t ID;
  uint32_t Addr;
  ///////////////
  uint32_t Offset;
  uint32_t Remain;
  ///////////////
//  uint32_t paytype;
}PayRecord_t;

typedef struct {
  uint32_t UserNum;
  uint32_t InitStatus;
  uint32_t CostRecord;		//自开机交易记录条数
  uint32_t CostMoney;		//自开机交易金额
  uint32_t AllCostRecord;	//总交易记录
  uint32_t *CostMessage;	//交易详情
}xCostCB;

extern xCostCB xCCB;

uint32_t UserTblInit(uint32_t addr);	//交易记录初始化
uint32_t AddUser(PayRecord_t *user);	//记录新的卡片
uint32_t DelAllUser(uint32_t addr);	//删除全部卡片
uint32_t DelOneUser(PayRecord_t *user);
uint32_t CheckUser(PayRecord_t  *user);	//获得卡片信息，在Pay之前需要调用
uint32_t CostUser(PayRecord_t  *user,uint32_t cost);	//先调用check获得卡片信息
uint32_t ChargeUser(PayRecord_t  *user,uint32_t cost);
uint32_t CheckCostMsg(PayRecord_t  *user,uint32_t list);//涉及 刷卡，显示
uint32_t DelCostMsg(PayRecord_t  *user);		//删除一个用户的交易记录信息

#endif /* APP_INC_APP_PAY_H_ */
