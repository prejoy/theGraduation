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
struct netconn *tcp_clientconn=NULL;			//TCP CLIENT�������ӽṹ��
u8 tcp_client_recvbuf[TCP_CLIENT_RX_BUFSIZE];	//TCP�ͻ��˽������ݻ�����
volatile uint8_t tcp_client_flag = 0;		                        //TCP�ͻ������ݷ��ͱ�־λ
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
//tcp�ͻ���������
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
	tcp_rx=mymalloc(1,(80*1024));		//1MB��tcpӦ�ý���buff
	filecontext = mymalloc(1,(80*1024));
	memset(tcp_rx,0,(80*1024));
	memset(filecontext,0,(80*1024));



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
			netconn_write(tcp_clientconn ,"���ѻ��ն������ӣ�TCP�ͻ���\n",strlen("���ѻ��ն������ӣ�TCP�ͻ���\n"),NETCONN_COPY); //����tcp_server_sentbuf�е�����
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
					sprintf(costbuff,"���ţ�%x ���� -%u.%u \n",hEthMsg.CardId,hEthMsg.costamount/10,hEthMsg.costamount%10);
					break;
				      case ChargeMsgOut:
					sprintf(costbuff,"���ţ�%x ���� +%u.%u \n",hEthMsg.CardId,hEthMsg.costamount/10,hEthMsg.costamount%10);
					break;
				      case MsgFeedBack:
					sprintf(costbuff,"%s \n",hEthMsg.text);
					break;
				      case SysTicks:
					sprintf(costbuff,"ͨ��������%u�� \n",hEthMsg.costamount);
					break;
				      default:
					break;
				    }
				    err = netconn_write(tcp_clientconn ,costbuff,strlen((char*)costbuff),NETCONN_COPY); //����tcp_server_sentbuf�е�����
				    if(err != ERR_OK)
				    {
					    printf("����ʧ��\r\n");
				    }

				    memset(costbuff,0,sizeof(costbuff));
				  }
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

					/*
					 * Լ��������һ���ļ���Ҫ�ȷ��͸��ļ������ͺʹ�С���ֽڣ�����ΪԤ�ļ��������ٷ���ʵ��Ҫ���ļ�
					 * Ԥ�ļ��� 1:[ftype]_2:[size]_3:[card id]
					 * �ļ�
					 * */
					tcp_rx[tcprxbuflen]='\0';
					//recv data handle start
//					printf("%s",tcp_rx);

					if(filetype>0 && filesize>0)
					{
					    memcpy(filecontext+recvnumber,tcp_rx,tcprxbuflen);
					    recvnumber +=tcprxbuflen;
					    if(recvnumber >= filesize-1)	//�ٽ�ֵ�ٵ�ʽһ�µõ� ��ȷֵ
					      {
						//send mesg
						hEthIn.type = filetype;
						hEthIn.text = filecontext;
						hEthIn.len = filesize;
						sprintf(hEthIn.filename,"%s.%s",hEthIn.filename,fileexpname[hEthIn.type]);	//��̬�Ķ���ϸ��
						if(xQueueSend(queueEthIn,&hEthIn,portMAX_DELAY) != pdPASS)	//��������fatfs����ȥ��¼
						  {
						    printf("EthIn send error\r\n");
						  }

//						printf("filesize:%u\r\n",hEthIn.len);
//						printf("filename:%s\r\n",hEthIn.filename);

						memset(&hEthIn,0,sizeof(hEthIn));

						filetype = 0;
						filesize = 0;
						recvnumber =0;
					      }
					}

					pdat = tcp_rx;
					i=0;
					while(i<3)	//�ж�ǰ3���ֽ��Ƿ�����Ч��Ϣ��ʼ
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

//					printf("filetype:%d\r\n",filetype);
//					printf("filesize:%u\r\n",filesize);
//					printf("filename:%s\r\n",hEthIn.filename);
					//recv data handle end

					tcprxbuflen = 0;
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

