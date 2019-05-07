#ifndef LWIP_CLIENT_APP_H
#define LWIP_CLIENT_APP_H
#include "sys.h"   

 
#define TCP_CLIENT_RX_BUFSIZE	1024	//接收缓冲区长度
#define REMOTE_PORT				8087	//定义远端主机的IP地址
//#define LWIP_SEND_DATA			0X80    //定义有数据发送

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
extern u8 tcp_client_recvbuf[TCP_CLIENT_RX_BUFSIZE];	//TCP客户端接收数据缓冲区
extern volatile uint8_t tcp_client_flag;		    //TCP客户端数据发送标志位

uint8_t tcp_client_init(void);  //tcp客户端初始化(创建tcp客户端线程)

#endif

