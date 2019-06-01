/*
 * App_UserMsg.c
 *
 *  Created on: 2019��4��27��
 *      Author: prejoy
 */

#include "App_UserMsg.h"
#include "malloc.h"
#include "ff.h"
#include "exfuns.h"
//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��
//����ֵ:����Ч�ļ���
u16 pic_get_tnum(u8 *path)
{
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO *tfileinfo;	//��ʱ�ļ���Ϣ
	tfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));//�����ڴ�
	res=f_opendir(&tdir,(const TCHAR*)path); 	//��Ŀ¼
	if(res==FR_OK&&tfileinfo)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	        res=f_readdir(&tdir,tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;//������/��ĩβ��,�˳�
			res=f_typetell((u8*)tfileinfo->fname);
			if((res&0XF0)==0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�
			{
				rval++;//��Ч�ļ�������1
			}
		}
	}
	myfree(SRAMIN,tfileinfo);//�ͷ��ڴ�
	return rval;
}







//�ļ�����
//��psrc�ļ�,copy��pdst.
//psrc,pdst:Դ�ļ���Ŀ���ļ�
//fwmode:�ļ�д��ģʽ
//0:������ԭ�е��ļ�
//1:����ԭ�е��ļ�
u8 mf_copy(u8 *psrc,u8 *pdst,u8 fwmode)
{
 u8 res;
    u16 br=0;
 u16 bw=0;
 FIL *fsrc=0;
 FIL *fdst=0;
 u8 *fbuf=0;
  fsrc=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
  fdst=(FIL*)mymalloc(SRAMIN,sizeof(FIL));

   if(fsrc==NULL||fdst==NULL/*||fbuf==NULL*/)res=100;//ǰ���ֵ����fatfs
 else
 {
//  if(fwmode==0)fwmode=FA_CREATE_NEW;//������
//  else fwmode=FA_CREATE_ALWAYS;   //���Ǵ��ڵ��ļ�

   res=f_open(fsrc,(const TCHAR*)psrc,FA_READ|FA_OPEN_EXISTING); //��ֻ���ļ�
   if(res==0)res=f_open(fdst,(const TCHAR*)pdst,FA_READ|FA_WRITE|FA_CREATE_ALWAYS);  //��һ���򿪳ɹ�,�ſ�ʼ�򿪵ڶ���
  if(res==0)//�������򿪳ɹ���
  {
    printf("src file size:%llu\r\n",fsrc->obj.objsize);
    fbuf=(u8*)mymalloc(SRAMEX,fsrc->obj.objsize+100);
    if(fbuf == NULL)
      return 100;

    while(res==0)//��ʼ����
   {
    res=f_read(fsrc,fbuf,(fsrc->obj.objsize),(UINT*)&br); //Դͷ����512�ֽ�
    if(res||br==0)break;
     f_lseek(fdst,0);
    res=f_write(fdst,fbuf,(UINT)br,(UINT*)&bw); //д��Ŀ���ļ�
    if(res||bw<br)break;
   }
      f_close(fsrc);
      f_close(fdst);
  }
 }
 myfree(SRAMIN,fsrc);//�ͷ��ڴ�
 myfree(SRAMIN,fdst);
 myfree(SRAMEX,fbuf);
 return res;
}

