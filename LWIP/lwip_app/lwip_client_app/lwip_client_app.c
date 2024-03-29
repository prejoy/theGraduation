#include "lwip_client_app.h"
#include "lwip/opt.h"
#include "lwip_comm.h"
#include <string.h>
#include "devDelay.h"
#include "lwip/lwip_sys.h"
#include "lwip/api.h" 
#include "FreeRTOS.h"
#include "task.h"
#include "malloc.h"

extern QueueHandle_t queueEthIn,queueEthOut;
//char *tcp_client_sendbuf="Explorer STM32F407 NETCONN TCP Client send data\r\n";
char tcp_client_sendbuf[100]="abccc\n";
struct netconn *tcp_clientconn=NULL;			//TCP CLIENT网络连接结构体
u8 tcp_client_recvbuf[TCP_CLIENT_RX_BUFSIZE];	//TCP客户端接收数据缓冲区
volatile uint8_t tcp_client_flag = 0;		                        //TCP客户端数据发送标志位
char costbuff[60];
//char costbufftemp[10];

uint32_t tcptxbuflen=0,tcprxbuflen=0;
//uint8_t  tcp_tx[2048*1024]__attribute__((section(".lwiptx")));
//uint8_t  tcp_rx[1024*1024]__attribute__((section(".mymalloc")))__attribute__((aligned(32)));

#define   TCPCLIENT_TASK_PRIO      10
#define   TCPCLIENT_STK_SIZE       1024
TaskHandle_t TCPCLIENTTask_Handler;


const char fileexpname[4][4]={
    "nul","txt","jpg"
};
//tcp客户端任务函数
void tcp_client_thread(void *arg)
{
	u32 data_len = 0;
	struct pbuf *q;
	err_t err,recv_err;
	static ip_addr_t server_ipaddr,loca_ipaddr;
	static u16_t 		 server_port,loca_port;

	uint8_t *pdat=0,i=0,j=0,filetype=0;
	uint16_t filesize=0,recvnumber=0;
	EthOut hEthMsg;
	EthIn hEthIn;
	uint8_t *tcp_rx,*filecontext;
	tcp_rx=mymalloc(1,(80*1024));		//1MB的tcp应用接收buff
	filecontext = mymalloc(1,(80*1024));
	memset(tcp_rx,0,(80*1024));
	memset(filecontext,0,(80*1024));



	LWIP_UNUSED_ARG(arg);
	server_port = REMOTE_PORT;
	IP4_ADDR(&server_ipaddr, lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3]);
	
	while (1) 
	{
		tcp_clientconn=netconn_new(NETCONN_TCP);  //创建一个TCP链接
        err = netconn_connect(tcp_clientconn,&server_ipaddr,server_port);//连接服务器
		if(err != ERR_OK)
		{
			netconn_delete(tcp_clientconn); //返回值不等于ERR_OK,删除tcp_clientconn连接
		}
		else if (err == ERR_OK)    //处理新连接的数据
		{ 
			struct netbuf *recvbuf;
			tcp_clientconn->recv_timeout = 10;			//阻塞读取信息时间，影响实时性
			netconn_getaddr(tcp_clientconn,&loca_ipaddr,&loca_port,1); //获取本地IP主机IP地址和端口号
			printf("连接上服务器%d.%d.%d.%d,本机端口号为:%d\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3],loca_port);
			netconn_write(tcp_clientconn ,"消费机终端已连接：TCP客户端\n",strlen("消费机终端已连接：TCP客户端\n"),NETCONN_COPY); //发送tcp_server_sentbuf中的数据
			tcp_client_flag = 0;
			while(1)
			{
			    if(tcp_client_flag>0)
			      {
				if(xQueueReceive(queueEthOut,&hEthMsg,0) == pdPASS)
				  {
				    switch(hEthMsg.type)
				    {
				      case CostMsgOut:
					sprintf(costbuff,"卡号：%x 金额：￥ -%u.%u \n",hEthMsg.CardId,hEthMsg.costamount/10,hEthMsg.costamount%10);
					break;
				      case ChargeMsgOut:
					sprintf(costbuff,"卡号：%x 金额：￥ +%u.%u \n",hEthMsg.CardId,hEthMsg.costamount/10,hEthMsg.costamount%10);
					break;
				      case MsgFeedBack:
					sprintf(costbuff,"%s \n",hEthMsg.text);
					break;
				      case SysTicks:
					sprintf(costbuff,"通信正常：%u次 \n",hEthMsg.costamount);
					break;
				      default:
					break;
				    }
				    err = netconn_write(tcp_clientconn ,costbuff,strlen((char*)costbuff),NETCONN_COPY); //发送tcp_server_sentbuf中的数据
				    if(err != ERR_OK)
				    {
					    printf("发送失败\r\n");
				    }

				    memset(costbuff,0,sizeof(costbuff));
				  }
				tcp_client_flag--;
			      }

					
				if((recv_err = netconn_recv(tcp_clientconn,&recvbuf)) == ERR_OK)  //接收到数据
				{	
					taskENTER_CRITICAL();
					for(q=recvbuf->p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
					{
						//判断要拷贝到TCP_CLIENT_RX_BUFSIZE中的数据是否大于TCP_CLIENT_RX_BUFSIZE的剩余空间，如果大于
						//的话就只拷贝TCP_CLIENT_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
			/*			if(q->len > (TCP_CLIENT_RX_BUFSIZE-data_len)) {
						    memcpy(tcp_client_recvbuf+data_len,q->payload,(TCP_CLIENT_RX_BUFSIZE-data_len));//拷贝数据
						}
						else{
						    memcpy(tcp_client_recvbuf+data_len,q->payload,q->len);
						}

						data_len += q->len;  	
						if(data_len > TCP_CLIENT_RX_BUFSIZE) break; //超出TCP客户端接收数组,跳出	
			*/

						    memcpy(tcp_rx+tcprxbuflen,q->payload,q->len);
						    tcprxbuflen += q->len;
					}
					taskEXIT_CRITICAL();

					/*
					 * 约定：接收一个文件需要先发送该文件的类型和大小（字节），该为预文件，后面再发真实需要的文件
					 * 预文件： 1:[ftype]_2:[size]_3:[card id]
					 * 文件
					 * */
					tcp_rx[tcprxbuflen]='\0';
					//recv data handle start

					if(filetype>0 && filesize>0)
					{
					    memcpy(filecontext+recvnumber,tcp_rx,tcprxbuflen);
					    recvnumber +=tcprxbuflen;
					    if(recvnumber >= filesize-1)	//临界值再调式一下得到 精确值
					      {
						//send mesg
						hEthIn.type = filetype;
						hEthIn.text = filecontext;
						hEthIn.len = filesize;
						sprintf(hEthIn.filename,"%s.%s",hEthIn.filename,fileexpname[hEthIn.type]);	//静态的对象化细想
						if(xQueueSend(queueEthIn,&hEthIn,portMAX_DELAY) != pdPASS)	//发给其他fatfs任务去记录
						  {
						    printf("EthIn send error\r\n");
						  }

						memset(&hEthIn,0,sizeof(hEthIn));

						filetype = 0;
						filesize = 0;
						recvnumber =0;
					      }
					}

					pdat = tcp_rx;
					i=0;
					while(i<3)	//判断前3个字节是否有有效信息起始
					  {
					    if((*pdat == '1') && (*(pdat+1) == ':') && (*(pdat+6) == '2') && (*(pdat+7) == ':'))
					      {
						if(memcmp((pdat+2),"txt",3)==0)	//equal
						  {
						    filetype = txtfile;
						  }
						else if (memcmp((pdat+2),"jpg",3)==0)	//equal
						  {
						    filetype = jpgfile;
						  }
						else
						  {
						    filetype = errorfile;
						  }

						char *pnum;
						pnum = pdat+8;
						filesize = 0;
						while(*pnum>='0'&&*pnum<='9')
						  {
						    filesize = filesize*10+(*pnum)-'0';
						    pnum++;
						  }
						if(filesize > 0xF000)	//60KB
						  {
						    filesize = 0;
						    filetype = errorfile;
						  }

						//name

						for(;*pnum!='3' && *(pnum+1)!=':';pnum++){

						}
						memcpy(hEthIn.filename,pnum+2,8);

						recvnumber = 0;
//						i=4;
						break;
					      }
					    else
					      {
						i++;
						pdat++;
					      }
					  }
					//recv data handle end

					tcprxbuflen = 0;
					netbuf_delete(recvbuf);

				}else if(recv_err == ERR_CLSD)  //关闭连接
				{
					netconn_close(tcp_clientconn);
					netconn_delete(tcp_clientconn);
					printf("服务器%d.%d.%d.%d断开连接\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3]);
					break;
				}
			}
		}
	}
}

//创建TCP客户端线程
//返回值:0 TCP客户端创建成功
//其他 TCP客户端创建失败
uint8_t tcp_client_init(void)
{
	BaseType_t res;
	
	taskENTER_CRITICAL();              
	res = xTaskCreate((TaskFunction_t)tcp_client_thread,
					(const char*  )"tcp_client_task",
					(uint16_t     )TCPCLIENT_STK_SIZE,
					(void*        )NULL,
					(UBaseType_t  )TCPCLIENT_TASK_PRIO,
					(TaskHandle_t*)&TCPCLIENTTask_Handler);
	taskEXIT_CRITICAL();
	
	if(res == pdPASS)
	{
	    return 0;
	}
	return 1;
}

