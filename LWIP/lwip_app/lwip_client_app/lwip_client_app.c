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


//char *tcp_client_sendbuf="Explorer STM32F407 NETCONN TCP Client send data\r\n";
char tcp_client_sendbuf[100]="abccc\n";
struct netconn *tcp_clientconn=NULL;			//TCP CLIENT�������ӽṹ��
u8 tcp_client_recvbuf[TCP_CLIENT_RX_BUFSIZE];	//TCP�ͻ��˽������ݻ�����
volatile uint8_t tcp_client_flag = 0;		                        //TCP�ͻ������ݷ��ͱ�־λ
//char costbuff[50];
//char costbufftemp[10];

uint32_t tcptxbuflen=0,tcprxbuflen=0;
//uint8_t  tcp_tx[2048*1024]__attribute__((section(".lwiptx")));
//uint8_t  tcp_rx[1024*1024]__attribute__((section(".mymalloc")))__attribute__((aligned(32)));

#define   TCPCLIENT_TASK_PRIO      10
#define   TCPCLIENT_STK_SIZE       256
TaskHandle_t TCPCLIENTTask_Handler;


//tcp�ͻ���������
void tcp_client_thread(void *arg)
{
	u32 data_len = 0;
	struct pbuf *q;
	err_t err,recv_err;
	static ip_addr_t server_ipaddr,loca_ipaddr;
	static u16_t 		 server_port,loca_port;

	uint8_t *tcp_rx;
	tcp_rx=mymalloc(1,(800*1024));		//1MB��tcpӦ�ý���buff
	memset(tcp_rx,0,(800*1024));

	LWIP_UNUSED_ARG(arg);
	server_port = REMOTE_PORT;
	IP4_ADDR(&server_ipaddr, lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3]);
	
	while (1) 
	{
		tcp_clientconn=netconn_new(NETCONN_TCP);  //����һ��TCP����
        err = netconn_connect(tcp_clientconn,&server_ipaddr,server_port);//���ӷ�����
		if(err != ERR_OK)
		{
			netconn_delete(tcp_clientconn); //����ֵ������ERR_OK,ɾ��tcp_clientconn����
		}
		else if (err == ERR_OK)    //���������ӵ�����
		{ 
			struct netbuf *recvbuf;
			tcp_clientconn->recv_timeout = 10;			//������ȡ��Ϣʱ�䣬Ӱ��ʵʱ��
			netconn_getaddr(tcp_clientconn,&loca_ipaddr,&loca_port,1); //��ȡ����IP����IP��ַ�Ͷ˿ں�
			printf("�����Ϸ�����%d.%d.%d.%d,�����˿ں�Ϊ:%d\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3],loca_port);
			netconn_write(tcp_clientconn ,"Tcp Client connected\r\n",strlen("Tcp Client connected\r\n"),NETCONN_COPY); //����tcp_server_sentbuf�е�����
			tcp_client_flag = 0;
			while(1)
			{

			    if(tcp_client_flag>0)
			      {
				tcp_client_flag--;
			      }
//				if((tcp_client_flag & LWIP_SEND_DATA) == LWIP_SEND_DATA) //������Ҫ����
//				{
//					err = netconn_write(tcp_clientconn ,tcp_client_sendbuf,strlen((char*)tcp_client_sendbuf),NETCONN_COPY); //����tcp_server_sentbuf�е�����
//					if(err != ERR_OK)
//					{
//						printf("����ʧ��\r\n");
//					}
//					tcp_client_flag &= ~LWIP_SEND_DATA;
//				}
					
				if((recv_err = netconn_recv(tcp_clientconn,&recvbuf)) == ERR_OK)  //���յ�����
				{	
					taskENTER_CRITICAL();
			//		memset(tcp_client_recvbuf,0,TCP_CLIENT_RX_BUFSIZE);  //���ݽ��ջ���������
					for(q=recvbuf->p;q!=NULL;q=q->next)  //����������pbuf����
					{
						//�ж�Ҫ������TCP_CLIENT_RX_BUFSIZE�е������Ƿ����TCP_CLIENT_RX_BUFSIZE��ʣ��ռ䣬�������
						//�Ļ���ֻ����TCP_CLIENT_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
			/*			if(q->len > (TCP_CLIENT_RX_BUFSIZE-data_len)) {
						    memcpy(tcp_client_recvbuf+data_len,q->payload,(TCP_CLIENT_RX_BUFSIZE-data_len));//��������
						}
						else{
						    memcpy(tcp_client_recvbuf+data_len,q->payload,q->len);
						}

						data_len += q->len;  	
						if(data_len > TCP_CLIENT_RX_BUFSIZE) break; //����TCP�ͻ��˽�������,����	
			*/

						    memcpy(tcp_rx+tcprxbuflen,q->payload,q->len);
						    tcprxbuflen += q->len;
					}
					taskEXIT_CRITICAL();
//    origin
//					data_len=0;  //������ɺ�data_lenҪ���㡣
//					printf("%s\r\n",tcp_client_recvbuf);
//					netbuf_delete(recvbuf);

					tcp_rx[tcprxbuflen]='\0';
					tcprxbuflen = 0;
					printf("%s",tcp_rx);
					netbuf_delete(recvbuf);

				}else if(recv_err == ERR_CLSD)  //�ر�����
				{
					netconn_close(tcp_clientconn);
					netconn_delete(tcp_clientconn);
					printf("������%d.%d.%d.%d�Ͽ�����\r\n",lwipdev.remoteip[0],lwipdev.remoteip[1], lwipdev.remoteip[2],lwipdev.remoteip[3]);
					break;
				}
			}
		}
	}
}

//����TCP�ͻ����߳�
//����ֵ:0 TCP�ͻ��˴����ɹ�
//���� TCP�ͻ��˴���ʧ��
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

