/*
 * App_Pay.h
 *
 *  Created on: 2019��3��21��
 *      Author: prejoy
 */

#ifndef APP_INC_APP_PAY_H_
#define APP_INC_APP_PAY_H_

#include "App_main.h"


 /*
  * ��дw25q256 ʹ����4096�ֽڶ���
 * ��ͷʹ��4096Byte�����Դ��255-1����Ƭ������Ϣ
 * ÿ��ID�Ľ��׼�¼Ҳ��4096Byte�����Լ�¼���512�����׼�¼��
 * ���96�ֽڱ�����ֻ��¼500����¼
 * ���׼�¼��ͷ��ID	Addr	Null	NULL	...	check=0x1234567890abcdef	UserNum
 * 		4B 	4B	4B	4B		4096-16				4096-8b
 * ���׼�¼��	pay	NULL   Offset remain
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
  uint32_t CostRecord;		//�Կ������׼�¼����
  uint32_t CostMoney;		//�Կ������׽��
  uint32_t AllCostRecord;	//�ܽ��׼�¼
  uint32_t *CostMessage;	//��������
}xCostCB;

extern xCostCB xCCB;

uint32_t UserTblInit(uint32_t addr);	//���׼�¼��ʼ��
uint32_t AddUser(PayRecord_t *user);	//��¼�µĿ�Ƭ
uint32_t DelAllUser(uint32_t addr);	//ɾ��ȫ����Ƭ
uint32_t DelOneUser(PayRecord_t *user);
uint32_t CheckUser(PayRecord_t  *user);	//��ÿ�Ƭ��Ϣ����Pay֮ǰ��Ҫ����
uint32_t CostUser(PayRecord_t  *user,uint32_t cost);	//�ȵ���check��ÿ�Ƭ��Ϣ
uint32_t ChargeUser(PayRecord_t  *user,uint32_t cost);
uint32_t CheckCostMsg(PayRecord_t  *user,uint32_t list);//�漰 ˢ������ʾ
uint32_t DelCostMsg(PayRecord_t  *user);		//ɾ��һ���û��Ľ��׼�¼��Ϣ

#endif /* APP_INC_APP_PAY_H_ */
