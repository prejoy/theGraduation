#include <devLtdc.h>
#include "font.h"


LTDC_HandleTypeDef  LTDC_Handler;	    //LTDC���
DMA2D_HandleTypeDef DMA2D_Handler; 	    //DMA2D���


u16 ltdc_lcd_framebuf[1280][800] __attribute__((section(".ltdcbuf")));	//����������ֱ���ʱ,LCD�����֡���������С


u32 *ltdc_framebuf[2];					//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����
_ltdc_dev lcdltdc;						//����LCD LTDC����Ҫ����

uint32_t POINT_COLOR=0xFF000000;
uint32_t BACK_COLOR =BG_COLOR;


//��LCD����
//lcd_switch:1 ��,0���ر�
void LTDC_Switch(u8 sw)
{
	if(sw==1) __HAL_LTDC_ENABLE(&LTDC_Handler);
	else if(sw==0)__HAL_LTDC_DISABLE(&LTDC_Handler);
}

//����ָ����
//layerx:���,0,��һ��; 1,�ڶ���
//sw:1 ��;0�ر�
void LTDC_Layer_Switch(u8 layerx,u8 sw)
{
	if(sw==1) __HAL_LTDC_LAYER_ENABLE(&LTDC_Handler,layerx);
	else if(sw==0) __HAL_LTDC_LAYER_DISABLE(&LTDC_Handler,layerx);
	__HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler);
}

//ѡ���
//layerx:���;0,��һ��;1,�ڶ���;
void LTDC_Select_Layer(u8 layerx)
{
	lcdltdc.activelayer=layerx;
}

//����LCD��ʾ����
//dir:0,������1,����
void LTDC_Display_Dir(u8 dir)
{
    lcdltdc.dir=dir; 	//��ʾ����
	if(dir==0)			//����
	{
		lcdltdc.width=lcdltdc.pheight;
		lcdltdc.height=lcdltdc.pwidth;	
	}else if(dir==1)	//����
	{
		lcdltdc.width=lcdltdc.pwidth;
		lcdltdc.height=lcdltdc.pheight;
	}
}

u32 LCD_Pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}

//���㺯��
//x,y:д������
//color:��ɫֵ
void LTDC_Draw_Point(u16 x,u16 y,u32 color)
{ 

    if(lcdltdc.dir)	//����
    {
    *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
    }else 			//����
    {
    *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y))=color;
    }

}

inline void LCD_Draw_Point(u16 x,u16 y,u32 color)
{
  LTDC_Draw_Point(x,y,color);
}

//���㺯��
//x,y:��ȡ�������
//����ֵ:��ɫֵ
u32 LTDC_Read_Point(u16 x,u16 y)
{ 
	if(lcdltdc.dir)	//����
	{
		return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x));
	}else 			//����
	{
		return *(u16*)((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y)); 
	}
}

//LTDC������,DMA2D���
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
//��ʱ����ҪƵ���ĵ�����亯��������Ϊ���ٶȣ���亯�����üĴ����汾��
//���������ж�Ӧ�Ŀ⺯���汾�Ĵ��롣
void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{ 
	u32 psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	u32 timeout=0; 
	u16 offline;
	u32 addr; 
	//����ϵת��
	if(lcdltdc.dir)	//����
	{
		psx=sx;psy=sy;
		pex=ex;pey=ey;
	}else			//����
	{
		psx=sy;psy=lcdltdc.pheight-ex-1;
		pex=ey;pey=lcdltdc.pheight-sx-1;
	}
	offline=lcdltdc.pwidth-(pex-psx+1);
	addr=((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	__HAL_RCC_DMA2D_CLK_ENABLE();	//ʹ��DM2Dʱ��
	DMA2D->CR&=~(DMA2D_CR_START);	//��ֹͣDMA2D
	DMA2D->CR=DMA2D_R2M;			//�Ĵ������洢��ģʽ
	DMA2D->OPFCCR=LCD_PIXFORMAT;	//������ɫ��ʽ
	DMA2D->OOR=offline;				//������ƫ�� 

	DMA2D->OMAR=addr;				//����洢����ַ
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//�趨�����Ĵ���
	DMA2D->OCOLR=color;						//�趨�����ɫ�Ĵ��� 
	DMA2D->CR|=DMA2D_CR_START;				//����DMA2D
	while((DMA2D->ISR&(DMA2D_FLAG_TC))==0)	//�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//��ʱ�˳�
	} 
	DMA2D->IFCR|=DMA2D_FLAG_TC;		//���������ɱ�־ 		
}
//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
//void LTDC_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
//{
//	u32 psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
//	u32 timeout=0; 
//	u16 offline;
//	u32 addr;  
//    if(ex>=lcdltdc.width)ex=lcdltdc.width-1;
//	if(ey>=lcdltdc.height)ey=lcdltdc.height-1;
//	//����ϵת��
//	if(lcdltdc.dir)	//����
//	{
//		psx=sx;psy=sy;
//		pex=ex;pey=ey;
//	}else			//����
//	{
//		psx=sy;psy=lcdltdc.pheight-ex-1;
//		pex=ey;pey=lcdltdc.pheight-sx-1;
//	}
//	offline=lcdltdc.pwidth-(pex-psx+1);
//	addr=((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
//    if(LCD_PIXFORMAT==LCD_PIXEL_FORMAT_RGB565)  //�����RGB565��ʽ�Ļ���Ҫ������ɫת������16bitת��Ϊ32bit��
//    {
//        color=((color&0X0000F800)<<8)|((color&0X000007E0)<<5)|((color&0X0000001F)<<3);
//    }
//	//����DMA2D��ģʽ
//	DMA2D_Handler.Instance=DMA2D;
//	DMA2D_Handler.Init.Mode=DMA2D_R2M;			//�ڴ浽�洢��ģʽ
//	DMA2D_Handler.Init.ColorMode=LCD_PIXFORMAT;	//������ɫ��ʽ
//	DMA2D_Handler.Init.OutputOffset=offline;		//���ƫ�� 
//	HAL_DMA2D_Init(&DMA2D_Handler);              //��ʼ��DMA2D
//    HAL_DMA2D_ConfigLayer(&DMA2D_Handler,lcdltdc.activelayer); //������
//    HAL_DMA2D_Start(&DMA2D_Handler,color,(u32)addr,pex-psx+1,pey-psy+1);//��������
//    HAL_DMA2D_PollForTransfer(&DMA2D_Handler,1000);//��������
//    while((__HAL_DMA2D_GET_FLAG(&DMA2D_Handler,DMA2D_FLAG_TC)==0)&&(timeout<0X5000))//�ȴ�DMA2D���
//    {
//        timeout++;
//    }
//    __HAL_DMA2D_CLEAR_FLAG(&DMA2D_Handler,DMA2D_FLAG_TC);       //���������ɱ�־
//}

//��ָ�����������ָ����ɫ��,DMA2D���	
//�˺�����֧��u16,RGB565��ʽ����ɫ�������.
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)  
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ�����׵�ַ
void LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{
	u32 psx,psy,pex,pey;	//��LCD���Ϊ��׼������ϵ,����������仯���仯
	u32 timeout=0; 
	u16 offline;
	u32 addr; 
	//����ϵת��
	if(lcdltdc.dir)	//����
	{
		psx=sx;psy=sy;
		pex=ex;pey=ey;
	}else			//����
	{
		psx=sy;psy=lcdltdc.pheight-ex-1;
		pex=ey;pey=lcdltdc.pheight-sx-1;
	}
	offline=lcdltdc.pwidth-(pex-psx+1);
	addr=((u32)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*psy+psx));
	__HAL_RCC_DMA2D_CLK_ENABLE();	//ʹ��DM2Dʱ��
	DMA2D->CR&=~(DMA2D_CR_START);	//��ֹͣDMA2D
	DMA2D->CR=DMA2D_M2M;			//�洢�����洢��ģʽ
	DMA2D->FGPFCCR=LCD_PIXFORMAT;	//������ɫ��ʽ
	DMA2D->FGOR=0;					//ǰ������ƫ��Ϊ0
	DMA2D->OOR=offline;				//������ƫ�� 

	DMA2D->FGMAR=(u32)color;		//Դ��ַ
	DMA2D->OMAR=addr;				//����洢����ַ
	DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//�趨�����Ĵ��� 
	DMA2D->CR|=DMA2D_CR_START;					//����DMA2D
	while((DMA2D->ISR&(DMA2D_FLAG_TC))==0)		//�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)break;	//��ʱ�˳�
	} 
	DMA2D->IFCR|=DMA2D_FLAG_TC;				//���������ɱ�־  	
}  

//LCD����
//color:��ɫֵ
void LTDC_Clear(u32 color)
{
	LTDC_Fill(0,0,lcdltdc.width-1,lcdltdc.height-1,color);
}


inline void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{
  LTDC_Fill(sx,sy,ex,ey,color);
}


void LTDC_DrawLine(u16 sx,u16 sy,u16 ex,u16 ey,u16 width,u32 color)
{
  if((sy==ey))	//horizontal
    {
      sx<ex?LTDC_Fill(sx,sy-(width>>1),ex,ey+(width>>1),color):LTDC_Fill(ex,sy-(width>>1),sx,ey+(width>>1),color);
    }
  else if(sx==ex)	//vertical
    {
      sy<ey?LTDC_Fill(sx-(width>>1),sy,ex+(width>>1),ey,color):LTDC_Fill(sx-(width>>1),ey,ex+(width>>1),sy,color);
    }
  else
    {
      //.....
    }
}



//LTDCʱ��(Fdclk)���ú���
//Fvco=Fin*pllsain; 
//Fdclk=Fvco/pllsair/2*2^pllsaidivr=Fin*pllsain/pllsair/2*2^pllsaidivr;

//Fvco:VCOƵ��
//Fin:����ʱ��Ƶ��һ��Ϊ1Mhz(����ϵͳʱ��PLLM��Ƶ���ʱ��,��ʱ����ͼ)
//pllsain:SAIʱ�ӱ�Ƶϵ��N,ȡֵ��Χ:50~432.  
//pllsair:SAIʱ�ӵķ�Ƶϵ��R,ȡֵ��Χ:2~7
//pllsaidivr:LCDʱ�ӷ�Ƶϵ��,ȡֵ��Χ:RCC_PLLSAIDIVR_2/4/8/16,��Ӧ��Ƶ2~16 
//����:�ⲿ����Ϊ25M,pllm=25��ʱ��,Fin=1Mhz.
//����:Ҫ�õ�20M��LTDCʱ��,���������:pllsain=400,pllsair=5,pllsaidivr=RCC_PLLSAIDIVR_4
//Fdclk=1*400/5/4=400/20=20Mhz
//����ֵ:0,�ɹ�;1,ʧ�ܡ�
u8 LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr)
{
	RCC_PeriphCLKInitTypeDef PeriphClkIniture;
	
	//LTDC�������ʱ�ӣ���Ҫ�����Լ���ʹ�õ�LCD�����ֲ������ã�
    PeriphClkIniture.PeriphClockSelection=RCC_PERIPHCLK_LTDC;	//LTDCʱ�� 	
	PeriphClkIniture.PLLSAI.PLLSAIN=pllsain;    
	PeriphClkIniture.PLLSAI.PLLSAIR=pllsair;  
	PeriphClkIniture.PLLSAIDivR=pllsaidivr;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture)==HAL_OK)    //��������ʱ��
    {
        return 0;   //�ɹ�
    }
    else return 1;  //ʧ��    
}

//LTDC,���մ�������,������LCD�������ϵΪ��׼
//ע��:�˺���������LTDC_Layer_Parameter_Config֮��������.
//layerx:��ֵ,0/1.
//sx,sy:��ʼ����
//width,height:��Ⱥ͸߶�
void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height)
{
    HAL_LTDC_SetWindowPosition(&LTDC_Handler,sx,sy,layerx);  //���ô��ڵ�λ��
    HAL_LTDC_SetWindowSize(&LTDC_Handler,width,height,layerx);//���ô��ڴ�С    
}

//LTDC,������������.
//ע��:�˺���,������LTDC_Layer_Window_Config֮ǰ����.
//layerx:��ֵ,0/1.
//bufaddr:����ɫ֡������ʼ��ַ
//pixformat:��ɫ��ʽ.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//alpha0:Ĭ����ɫAlphaֵ,0,ȫ͸��;255,��͸��
//bfac1:���ϵ��1,4(100),�㶨��Alpha;6(101),����Alpha*�㶨Alpha
//bfac2:���ϵ��2,5(101),�㶨��Alpha;7(111),����Alpha*�㶨Alpha
//bkcolor:��Ĭ����ɫ,32λ,��24λ��Ч,RGB888��ʽ
//����ֵ:��
void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor)
{
	LTDC_LayerCfgTypeDef pLayerCfg;
	
	pLayerCfg.WindowX0=0;                       //������ʼX����
	pLayerCfg.WindowY0=0;                       //������ʼY����
	pLayerCfg.WindowX1=lcdltdc.pwidth;          //������ֹX����
	pLayerCfg.WindowY1=lcdltdc.pheight;         //������ֹY����
	pLayerCfg.PixelFormat=pixformat;		    //���ظ�ʽ
	pLayerCfg.Alpha=alpha;				        //Alphaֵ���ã�0~255,255Ϊ��ȫ��͸��
	pLayerCfg.Alpha0=alpha0;			        //Ĭ��Alphaֵ
	pLayerCfg.BlendingFactor1=(u32)bfac1<<8;    //���ò���ϵ��
	pLayerCfg.BlendingFactor2=(u32)bfac2<<8;	//���ò���ϵ��
	pLayerCfg.FBStartAdress=bufaddr;	        //���ò���ɫ֡������ʼ��ַ
	pLayerCfg.ImageWidth=lcdltdc.pwidth;        //������ɫ֡�������Ŀ��    
	pLayerCfg.ImageHeight=lcdltdc.pheight;      //������ɫ֡�������ĸ߶�
	pLayerCfg.Backcolor.Red=(u8)(bkcolor&0X00FF0000)>>16;   //������ɫ��ɫ����
	pLayerCfg.Backcolor.Green=(u8)(bkcolor&0X0000FF00)>>8;  //������ɫ��ɫ����
	pLayerCfg.Backcolor.Blue=(u8)bkcolor&0X000000FF;        //������ɫ��ɫ����
	HAL_LTDC_ConfigLayer(&LTDC_Handler,&pLayerCfg,layerx);   //������ѡ�еĲ�
}  

void LTDC_ShowChar(uint32_t x,uint32_t y,uint32_t num,uint32_t size,uint32_t mode)
{
  uint8_t temp,t1,t;
  uint16_t y0=y;
  uint8_t csize=(size/8+((size%8)?1:0))*(size/2);
  num=num-' ';
  for(t=0;t<csize;t++)
  {
	  if(size==12)temp=asc2_1206[num][t];
	  else if(size==16)temp=asc2_1608[num][t];
	  else if(size==24)temp=asc2_2412[num][t];
	  else if(size==32)temp=asc2_3216[num][t];
	  else return;
	  for(t1=0;t1<8;t1++)
	  {
		  if(temp&0x80)LTDC_Draw_Point(x,y,POINT_COLOR);
		  else if(mode==0)LTDC_Draw_Point(x,y,BACK_COLOR);
		  temp<<=1;
		  y++;
//		  if(y>=480)return;
		  if(y>=800)return;
		  if((y-y0)==size)
		  {
			  y=y0;
			  x++;
			  if(x>=800)return;
			  break;
		  }
	  }
  }
}

inline void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{
  LTDC_ShowChar(x,y,num,size,mode);
}


void LTDC_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{
  uint8_t x0=x;
    width+=x;
    height+=y;
    while((*p<='~')&&(*p>=' '))
    {
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;
        LTDC_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }
}

void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{
  uint8_t x0=x;
  width+=x;
  height+=y;
  while((*p<='~')&&(*p>=' '))
  {
      if(x>=width){x=x0;y+=size;}
      if(y>=height)break;
      LTDC_ShowChar(x,y,*p,size,0);
      x+=size/2;
      p++;
  }
}

void LTDC_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{
  u8 t,temp;
  u8 enshow=0;
  for(t=0;t<len;t++)
  {
    temp=(num/LCD_Pow(10,len-t-1))%10;
    if(enshow==0&&t<(len-1))
    {
	    if(temp==0)
	    {
		    LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
		    continue;
	    }else enshow=1;

    }
    LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
  }
}

void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{
  LTDC_ShowNum(x,y,num,len,size);
}

//LCD��ʼ������
void LTDC_Init(void)
{   
  lcdltdc.pwidth=800;			    //�����,��λ:����
  lcdltdc.pheight=480;		    //���߶�,��λ:����
  lcdltdc.hsw=1;				    //ˮƽͬ�����
  lcdltdc.vsw=1;				    //��ֱͬ�����
  lcdltdc.hbp=46;				    //ˮƽ����
  lcdltdc.vbp=23;				    //��ֱ����
  lcdltdc.hfp=210;			    //ˮƽǰ��
  lcdltdc.vfp=22;				    //��ֱǰ��
  lcdltdc.width=800;			    //�����,��λ:����
  lcdltdc.height=480;		    //���߶�,��λ:����
//  LTDC_Clk_Set(80,2,RCC_PLLSAIDIVR_2);		//��������ʱ�� 33M(�����˫��,��Ҫ����DCLK��:18.75Mhz  300/4/4,�Ż�ȽϺ�)

  lcdltdc.pixsize=2;				//ÿ������ռ2���ֽ�
  ltdc_framebuf[0]=(u32*)&ltdc_lcd_framebuf;

    //LTDC����
    LTDC_Handler.Instance=LTDC;
    LTDC_Handler.Init.HSPolarity=LTDC_HSPOLARITY_AL;         //ˮƽͬ������
    LTDC_Handler.Init.VSPolarity=LTDC_VSPOLARITY_AL;         //��ֱͬ������
    LTDC_Handler.Init.DEPolarity=LTDC_DEPOLARITY_AL;         //����ʹ�ܼ���
    LTDC_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IPC;   	//����ʱ�Ӽ���
    LTDC_Handler.Init.HorizontalSync=lcdltdc.hsw-1;          //ˮƽͬ�����
    LTDC_Handler.Init.VerticalSync=lcdltdc.vsw-1;            //��ֱͬ�����
    LTDC_Handler.Init.AccumulatedHBP=lcdltdc.hsw+lcdltdc.hbp-1; //ˮƽͬ�����ؿ��
    LTDC_Handler.Init.AccumulatedVBP=lcdltdc.vsw+lcdltdc.vbp-1; //��ֱͬ�����ظ߶�
    LTDC_Handler.Init.AccumulatedActiveW=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth-1;//��Ч���
    LTDC_Handler.Init.AccumulatedActiveH=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight-1;//��Ч�߶�
    LTDC_Handler.Init.TotalWidth=lcdltdc.hsw+lcdltdc.hbp+lcdltdc.pwidth+lcdltdc.hfp-1;   //�ܿ��
    LTDC_Handler.Init.TotalHeigh=lcdltdc.vsw+lcdltdc.vbp+lcdltdc.pheight+lcdltdc.vfp-1;  //�ܸ߶�
    LTDC_Handler.Init.Backcolor.Red=0;           //��Ļ�������ɫ����
    LTDC_Handler.Init.Backcolor.Green=0;         //��Ļ��������ɫ����
    LTDC_Handler.Init.Backcolor.Blue=0;          //��Ļ����ɫ��ɫ����
//    HAL_LTDC_Init(&LTDC_Handler);
 	
	//������
	LTDC_Layer_Parameter_Config(0,(u32)ltdc_framebuf[0],LCD_PIXFORMAT,255,0,6,7,0X000000);//���������
	LTDC_Layer_Window_Config(0,0,0,lcdltdc.pwidth,lcdltdc.pheight);	//�㴰������,��LCD�������ϵΪ��׼,��Ҫ����޸�!
	
 	LTDC_Display_Dir(0);			//Ĭ������
	LTDC_Select_Layer(0); 			//ѡ���1��
  LCD_LED=1;         		        //��������
  LTDC_Clear(0XFFFFFFFF);			//����
}

//LTDC�ײ�IO��ʼ����ʱ��ʹ��
//�˺����ᱻHAL_LTDC_Init()����
//hltdc:LTDC���
//void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
//{
//    GPIO_InitTypeDef GPIO_Initure;
//
//    __HAL_RCC_LTDC_CLK_ENABLE();                //ʹ��LTDCʱ��
//    __HAL_RCC_DMA2D_CLK_ENABLE();               //ʹ��DMA2Dʱ��
//    __HAL_RCC_GPIOB_CLK_ENABLE();               //ʹ��GPIOBʱ��
//    __HAL_RCC_GPIOF_CLK_ENABLE();               //ʹ��GPIOFʱ��
//    __HAL_RCC_GPIOG_CLK_ENABLE();               //ʹ��GPIOGʱ��
//    __HAL_RCC_GPIOH_CLK_ENABLE();               //ʹ��GPIOHʱ��
//    __HAL_RCC_GPIOI_CLK_ENABLE();               //ʹ��GPIOIʱ��
//
//    //��ʼ��PB5����������
//    GPIO_Initure.Pin=GPIO_PIN_5;                //PB5������������Ʊ���
//    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;      //�������
//    GPIO_Initure.Pull=GPIO_PULLUP;              //����
//    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //����
//    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
//
//    //��ʼ��PF10
//    GPIO_Initure.Pin=GPIO_PIN_10;
//    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //����
//    GPIO_Initure.Pull=GPIO_NOPULL;
//    GPIO_Initure.Speed=GPIO_SPEED_HIGH;         //����
//    GPIO_Initure.Alternate=GPIO_AF14_LTDC;      //����ΪLTDC
//    HAL_GPIO_Init(GPIOF,&GPIO_Initure);
//
//    //��ʼ��PG6,7,11
//    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_11;
//    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
//
//    //��ʼ��PH9,10,11,12,13,14,15
//    GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|\
//                     GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
//    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
//
//    //��ʼ��PI0,1,2,4,5,6,7,9,10
//    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|\
//                     GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10;
//    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
//}

