#ifndef LWIP_CLIENT_APP_H
#define LWIP_CLIENT_APP_H
#include "sys.h"   

 
#define TCP_CLIENT_RX_BUFSIZE	1024	//���ջ���������
#define REMOTE_PORT				8087	//����Զ��������IP��ַ
//#define LWIP_SEND_DATA			0X80    //���������ݷ���

typedef struct{
  uint8_t type;
  uint8_t flags;
  uint16_t len;
  char *text;
  char filename[16];
}EthIn;

typedef struct{
  uint32_t CardId;
  uint32_t costamount;
  uint8_t type;
  uint8_t flags;
  uint16_t ulen;
  char *text;
}EthOut;

typedef enum{
  NullMsg =0,
  CostMsgOut,
  ChargeMsgOut,
  UserMsgIn,
  UserPictIn,
  SysTicks,
  MsgFeedBack
}eEthMsgType;

typedef enum{
  errorfile = 0,
  txtfile,
  jpgfile
}sysfiletypes;
extern u8 tcp_client_recvbuf[TCP_CLIENT_RX_BUFSIZE];	//TCP�ͻ��˽������ݻ�����
extern volatile uint8_t tcp_client_flag;		    //TCP�ͻ������ݷ��ͱ�־λ

uint8_t tcp_client_init(void);  //tcp�ͻ��˳�ʼ��(����tcp�ͻ����߳�)

#endif

