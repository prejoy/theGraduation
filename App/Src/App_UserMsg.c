/*
 * App_UserMsg.c
 *
 *  Created on: 2019年4月27日
 *      Author: prejoy
 */

#include "App_UserMsg.h"
#include "malloc.h"
#include "ff.h"
#include "exfuns.h"
//得到path路径下,目标文件的总个数
//path:路径
//返回值:总有效文件数
u16 pic_get_tnum(u8 *path)
{
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO *tfileinfo;	//临时文件信息
	tfileinfo=(FILINFO*)mymalloc(SRAMIN,sizeof(FILINFO));//申请内存
	res=f_opendir(&tdir,(const TCHAR*)path); 	//打开目录
	if(res==FR_OK&&tfileinfo)
	{
		while(1)//查询总的有效文件数
		{
	        res=f_readdir(&tdir,tfileinfo);       		//读取目录下的一个文件
	        if(res!=FR_OK||tfileinfo->fname[0]==0)break;//错误了/到末尾了,退出
			res=f_typetell((u8*)tfileinfo->fname);
			if((res&0XF0)==0X50)//取高四位,看看是不是图片文件
			{
				rval++;//有效文件数增加1
			}
		}
	}
	myfree(SRAMIN,tfileinfo);//释放内存
	return rval;
}







//文件复制
//将psrc文件,copy到pdst.
//psrc,pdst:源文件和目标文件
//fwmode:文件写入模式
//0:不覆盖原有的文件
//1:覆盖原有的文件
u8 mf_copy(u8 *psrc,u8 *pdst,u8 fwmode)
{
 u8 res;
    u16 br=0;
 u16 bw=0;
 FIL *fsrc=0;
 FIL *fdst=0;
 u8 *fbuf=0;
  fsrc=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//申请内存
  fdst=(FIL*)mymalloc(SRAMIN,sizeof(FIL));

   if(fsrc==NULL||fdst==NULL/*||fbuf==NULL*/)res=100;//前面的值留给fatfs
 else
 {
//  if(fwmode==0)fwmode=FA_CREATE_NEW;//不覆盖
//  else fwmode=FA_CREATE_ALWAYS;   //覆盖存在的文件

   res=f_open(fsrc,(const TCHAR*)psrc,FA_READ|FA_OPEN_EXISTING); //打开只读文件
   if(res==0)res=f_open(fdst,(const TCHAR*)pdst,FA_READ|FA_WRITE|FA_CREATE_ALWAYS);  //第一个打开成功,才开始打开第二个
  if(res==0)//两个都打开成功了
  {
    printf("src file size:%llu\r\n",fsrc->obj.objsize);
    fbuf=(u8*)mymalloc(SRAMEX,fsrc->obj.objsize+100);
    if(fbuf == NULL)
      return 100;

    while(res==0)//开始复制
   {
    res=f_read(fsrc,fbuf,(fsrc->obj.objsize),(UINT*)&br); //源头读出512字节
    if(res||br==0)break;
     f_lseek(fdst,0);
    res=f_write(fdst,fbuf,(UINT)br,(UINT*)&bw); //写入目的文件
    if(res||bw<br)break;
   }
      f_close(fsrc);
      f_close(fdst);
  }
 }
 myfree(SRAMIN,fsrc);//释放内存
 myfree(SRAMIN,fdst);
 myfree(SRAMEX,fbuf);
 return res;
}

