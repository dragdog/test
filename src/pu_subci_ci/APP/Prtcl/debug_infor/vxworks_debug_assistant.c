/***************************************************************************************
*		版本号：V1.0
*
*		版权所有 (C) 2007 北京交通大学 CBTC项目 2乘2取2安全计算机课题组
*
*		作者：陈树泉
*
*		文件名：vxWorks_debug_Assistant.c
*
*       描述：vxWorks辅助调试工具中，vxWorks部分相关文件
*
*       参考文档：
*
*       创建时间：2007-5-9
*
*		修改历史：
*			2007-5-10 	陈树泉	增加互斥信号量semM_debug_infor保护用于发送消息的全局数组。
*			2007-5-10 	陈树泉	把输出功能改为一个函数，写消息队列部分都移至辅助调试输出函数中。
*			2007-5-10 	陈树泉	输出的数据长度根据实际长度不同不同，而不是都输出定长数据，以减少存储时对存储
*				空间的浪费。
*			2007-5-12 	陈树泉	增加了数组输出函数debug_out_array。建议不要一次输出数据长度超过50*MAX_LEN_DEBUG。
*           2007-5-12 	陈树泉  修改与上位机通信是的格式。每次通信时第一个字节作为传送数据存储方式标识符。
*       		0x55表示方式一，0xAA表示方式二。存储方式一：第一个字节为存储方式标识符，后边紧接着存两个4字节整型
*       		变量，紧接着以十六进制存放一个4字节变量，从第十五个字节开始以字符串形式存储。存储方式二：所有数据都以十
*               六进制形式存储。
*			2007-5-14 	陈树泉	对于输出非数组信息时，修正数据长度。
*			2007-5-16 	陈树泉	改正了数组输出函数中一个bug，原来会丢掉最后一个字节。
*			2007-5-17 	陈树泉	修改用于保护全局资源的信号量为二进制信号量semB_debug_infor。
*			2007-5-19 	陈树泉	去掉调试信息输出函数和数组输出函数中对全局变量的保护，因为总是会出现这个信号量错误而导致的
*				记录失败。
*           2007-5-25   陈树泉  将相关全局资源的保护改为条件编译处理。原来使用#if 0条件编译去掉。
*           2007-6-19   陈树泉  将表示任务堆栈大小的宏改为DEBUG_TASK_STACK_SIZE。
*           2007-6-19   陈树泉  修改debug中返回错误时的处理，不用return。
*           2007-6-25   陈树泉  将发送缓冲区debug_mess_snd改为局部变量，因为没有必要作为全局的。这样函数本身就是可重入的了，
*               而无需再增加信号量保证其不可重入。
*           2007-6-26   张岩    增加了第三方printf显示函数debug_infor_out_printf()。
*           2007-7-13   刘朔    增加ZC应用输出数据函数ZC_debug_out_array()。
*           2007-7-20   杨旭文  增加任意数目个参数的输出函数debug_infor_printf()及所用到的相关函数。
*           2007-7-25   杨旭文  增加了用于求幂的函数power，因为发现数学库《math.h》中的求幂函数pow存在漏洞，求出来
*               的数据有时候对有时候不对。
*           2007-7-30   陈树泉  去掉了关于用于全局资源保护的信号量及相关条件编译。因为没有全局的资源需要保护。发送缓冲区都改成了局部的了。
*           2007-7-30   陈树泉  增加了Format_hex_to_char函数中对0这个数据的处理，原来不处理。
*           2007-8-2    陈树泉  调整消息队列的设置，由原来1000个消息，每个消息最多1000字节改为最多2000消息，每个消息最大500个
*               字节，这样主要是为了提高记录能力，因为大部分记录其实不超过500字节，增大消息孰能减小记录不全的可能性。
*           2007-8-24   陈树泉  修改数组输出函数的参数设置，增加记录方式的参数，这样就可以将多种情况的数组输出函数整合。也便于扩展功能。
*           2007-8-24   陈树泉  将16进制记录的记录格式宏定义由原来的MODE2改为HEX_MODE，值不变。
*           2007-9-10   陈树泉  增加任务优先级宏定义。
            2008-7-22   王悉    将原来的发送任务取消，将sendto函数放到了每一个输出的函数中，取消了消息队列和与任务有关的宏定义
            2008-7-22   王悉    添加了发送端的序列号和crc
            2010-2-22   王悉    debug函数和其它没有使用的信号量删除。
            2010-6-30   王悉    删除了debug_infor_out和debug_infor_out_printf函数
            2010-7-5    王悉    在debug_infor_printf和debug_out_array函数中加入了crc和序列号
			2010-8-26   董帅    在debug_out_array函数中修改while(len  >= MAX_LEN_DEBUG-8)为while(len  >= MAX_LEN_DEBUG-9)，
								9包含前边4个字节的序列号和1个字节的mode标示，后边4个字节的CRC
			2010-8-26   董帅    修改debug_out_array函数中memmove(&debug_mess_snd[5], debug_array + indicate_sended, MAX_LEN_DEBUG-5)为
								memmove(&debug_mess_snd[5], debug_array + indicate_sended, MAX_LEN_DEBUG-9)
								减去9是因为9包含前边4个字节的序列号和1个字节的mode标示，后边4个字节的CRC
			2010-8-26   董帅    修改debug_out_array函数中CRC打包CRC_Check_Sum = Crc32long ( ( unsigned char *)&debug_mess_snd,  ( MAX_LEN_DEBUG-5 ) )为
								CRC_Check_Sum = Crc32long ( ( unsigned char *)&debug_mess_snd,  ( MAX_LEN_DEBUG-4 ) )
								减去4是因为数组后边留出4个字节来存CRC
			2010-8-26   董帅    将debug_out_array函数中传过来的数据，当数据量大的时候分为多帧发送，修改为就用一帧来发送，然后将					
								vxWorks_debug_Assistant.h文件中MAX_LEN_DEBUG从原来的1000修改为10000。
    存在的问题：
        1、偶尔有记录不完全的情况。可以通过设置发送消息队列时时间参数以及设置网络通信重发机制避免。
**************************************************************************************/
#include <vxWorks.h>
#include <stdio.h>
//#include <ioLib.h>
//#include <taskLib.h>
//#include <msgQLib.h>
//#include <semLib.h>
//#include <sockLib.h>
//#include <netinet/in.h>
#include <stdarg.h>
#include <string.h>


#include "vxWorks_debug_Assistant.h"
#include "Convert.h"


/*辅助调试输出相关全局变量*/
int socket_debug_green = 0;								/*辅助调试输出信息的socket*/
int socket_debug_purple = 0;								/*辅助调试输出信息的socket*/
int socket_debug = 0;								/*辅助调试输出信息的socket*/
unsigned long SN_send1 = 0;


/*=================================================================================================
*	函数名：debug_initial
*	功能：辅助调试输出信息相关资源的初始化
*	参数：void
*	返回值：OK or ERROR
*==================================================================================================*/
STATUS debug_initial( void )
{
#if 0
	struct sockaddr_in  selfGreenAddr;         		/*设置本地socket*/
	struct sockaddr_in  selfPurpleAddr;         		/*设置本地socket*/
	struct sockaddr_in  selfAddr;         			/*设置本地socket*/

   if((socket_debug = socket(AF_INET,SOCK_DGRAM,0)) == ERROR) 				/*建立socket*/
   	{
      	perror("socket created purple fail!");
      	printf("ERROR:socket():socket_debug!\n");
      	return(ERROR);
   	}
   	
	if((socket_debug_green = socket(AF_INET,SOCK_DGRAM,0)) == ERROR)		/*建立socket*/
   	{
      	perror("socket created green fail!");
      	printf("ERROR:socket():socket_debug_green!\n");
      	return(ERROR);
   	}
   	
   	if((socket_debug_purple = socket(AF_INET,SOCK_DGRAM,0)) == ERROR)		/*建立socket*/
   	{
      	perror("socket created purple fail!");
      	printf("ERROR:socket():socket_debug_purple!\n");
      	return(ERROR);
   	}
#endif
   	return(OK);

}/*end STATUS debug_initial( void )*/


/*=================================================================================================
*	函数名：debug_out_array
*	功能：输出所需的辅助输出信息。该函数与debug_infor_out函数的区别在于该函数用于数组方式输出。
*	参数：	char  mode                      记录格式标识符
*           char* debug_array				需要输出的数组首地址
*			short len						需要输出的数组长度，最大不宜超过MAX_LEN_DEBUG*50
*	返回值：OK or ERROR
*==================================================================================================*/
#if 0
STATUS debug_out_array(char mode, char* debug_array, short len)
{
#if 0
	unsigned char debug_mess_snd_green[MAX_LEN_DEBUG] = {0};	/*辅助调试输出信息的发送缓冲区*/
	unsigned char debug_mess_snd_purple[MAX_LEN_DEBUG] = {0};	/*辅助调试输出信息的发送缓冲区*/

  struct sockaddr_in  aimAddrGreen;         		/*目的socket设置*/
  struct sockaddr_in  aimAddrPurple;         		/*目的socket设置*/
	char   *aimIP_green = AIM_IP_GREEN;     		        /*对方IP*/	
	char   *aimIP_purple = AIM_IP_PURPLE;     		        /*对方IP*/
	
	unsigned char dataLen = 17;                   /* 一帧数据不包含CRC总共的帧头长度。*/
  unsigned long SequenceNo_array;               /* 序列号用于鉴别消息 发出的次序4 BYTE */

	unsigned long CRC_Check_Sum = 0;   /*用来保存计算出来的CRC测试结果*/
	
	if ( len >= MAX_OUT_DEBUG_INFO_SIZE ) 
	{
		len = MAX_OUT_DEBUG_INFO_SIZE;
	}
	else
	{
	}
	
	SequenceNo_array = ++SN_send1; /*内网帧序号*/  /*这个地方序列号需要注意区别使用，不同对象使用不同的*/
	
	bzero ((char *) &aimAddrGreen, sizeof (struct sockaddr_in));
  aimAddrGreen.sin_len = (u_char) sizeof (struct sockaddr_in);
  aimAddrGreen.sin_family = AF_INET;
  aimAddrGreen.sin_port = htons (DEBUG_PORT);
  
  if (((aimAddrGreen.sin_addr.s_addr = inet_addr (aimIP_green)) == ERROR) && 	/*查看IP是否对应*/
     ((aimAddrGreen.sin_addr.s_addr = hostGetByName (aimIP_green)) == ERROR))
  {
   	/*perror ("unknown aimIP\n");
   	printf("unknown aimIP, errno is %x\n", errno);*/
   	close (socket_debug_green);
      return(ERROR);
  }/*end 查看IP是否对应*/	
  
  bzero ((char *) &aimAddrPurple, sizeof (struct sockaddr_in));
  aimAddrPurple.sin_len = (u_char) sizeof (struct sockaddr_in);
  aimAddrPurple.sin_family = AF_INET;
  aimAddrPurple.sin_port = htons (DEBUG_PORT);
  
  if (((aimAddrPurple.sin_addr.s_addr = inet_addr (aimIP_purple)) == ERROR) && 	/*查看IP是否对应*/
     ((aimAddrPurple.sin_addr.s_addr = hostGetByName (aimIP_purple)) == ERROR))
  {
   	/*perror ("unknown aimIP\n");
   	printf("unknown aimIP, errno is %x\n", errno);*/
   	close (socket_debug_purple);
      return(ERROR);
  }/*end 查看IP是否对应*/	
  
	/*由于将缓冲区修改的很大，所以一帧能完全输出*/
	memset(debug_mess_snd_green, 0, MAX_LEN_DEBUG);						/*清零调试消息发送缓冲区*/
	memset(debug_mess_snd_purple, 0, MAX_LEN_DEBUG);						/*清零调试消息发送缓冲区*/

	debug_mess_snd_green[0] = 0xa0;                                 /* 帧头*/
	debug_mess_snd_green[1] = 0x50;                                 /* 协议类型*/
	debug_mess_snd_green[2] = 0x01;                                 /* 协议版本号*/
	debug_mess_snd_green[3] = 0xa1;                                 /* 帧类型*/
	debug_mess_snd_green[4] = mode;                                 /* 子类型*/
	
	LongToChar(SequenceNo_array,&debug_mess_snd_green[5]);          /*所应答的内网序列号 4字节 保留字节*/
	
	debug_mess_snd_green[9] = 0x00; 
	debug_mess_snd_green[10] = 0x3c;                                /* 源机器ID*/                                
	debug_mess_snd_green[11] = 0x00;                                 
	debug_mess_snd_green[12] = 0x01;                                /* 目标机器数*/                                  
	debug_mess_snd_green[13] = 0x00;                                 
	debug_mess_snd_green[14] = 0x03;                                /* 目标机器ID*/ 
	
	ShortToChar(len, &debug_mess_snd_green[15]);                    /* 数据域长度。*/                                  
	                         
	memmove(&debug_mess_snd_green[17], debug_array, len);           /* 数据域*/
	
	 /*CRC打包*/
  CRC_Check_Sum = Crc32long ( ( unsigned char *)&debug_mess_snd_green,  len+dataLen );/*计算CRC*/
  
  LongToChar(CRC_Check_Sum,&debug_mess_snd_green[len + dataLen]);
  
  memcpy(debug_mess_snd_purple,debug_mess_snd_green,len+dataLen+4);
  
  #ifdef GREEN_NET_MAIN	
  if(ERROR == sendto(socket_debug_green, debug_mess_snd_green,  len+dataLen+4, 0, (struct sockaddr *)&aimAddrGreen,sizeof (struct sockaddr_in)))
  {   	    
      return(ERROR);	  
  
  }/*end if(ERROR == sendto())*/   	
  else
  {
      /*清零接收缓冲区*/
      memset(debug_mess_snd_green, 0,  len+dataLen+4);/*数据长度不是整个缓冲区而是实际发送的数据长度，这样做是为了提高效率，避免对未使用缓冲区重复清零*/
     
  }/*发送成功*/
  #endif
  
  #ifdef PURPLE_NET_MAIN	
  if(ERROR == sendto(socket_debug_purple, debug_mess_snd_purple,  len+dataLen+4, 0, (struct sockaddr *)&aimAddrPurple,sizeof (struct sockaddr_in)))
  {   	    
      return(ERROR);
  }/*end if(ERROR == sendto())*/   	
  else
  {
      /*清零接收缓冲区*/
      memset(debug_mess_snd_purple, 0,  len+dataLen+4);/*数据长度不是整个缓冲区而是实际发送的数据长度，这样做是为了提高效率，避免对未使用缓冲区重复清零*/
     
  }/*发送成功*/
  #endif
#endif   	
	return(OK);
}/*end STATUS debug_out_array(char* debug_array, short len)*/
#endif

STATUS CI_debug_out_array(char* debug_array, short len)
{
#if 0	
	short indicate_sended = 0;				/*已发送数据指针，用于在一帧不能完全输出数组时，指示已经发送到什么地方了*/
	unsigned char debug_mess_snd[MAX_LEN_DEBUG] = {0};	/*辅助调试输出信息的发送缓冲区*/

    
	while(len >= MAX_LEN_DEBUG)				/*一帧不能发送完所有数据*/
	{
		memset(debug_mess_snd, 0, MAX_LEN_DEBUG);									/*清零调试消息发送缓冲区*/
		memmove(&debug_mess_snd[0], debug_array + indicate_sended, MAX_LEN_DEBUG);	/*需要输出的数据，*/
																					
		debug_out_array(ZC_INFOR,debug_mess_snd,MAX_LEN_DEBUG);

		indicate_sended = indicate_sended + MAX_LEN_DEBUG;		/*更新已发送位置指针*/
		len = len - MAX_LEN_DEBUG;								/*更新未发送数据长度*/
		
	}/*end while(len <= MAX_LEN_DEBUG)*/

	/*剩余数据一帧能完全输出*/
	memset(debug_mess_snd, 0, MAX_LEN_DEBUG);						/*清零调试消息发送缓冲区*/
	memmove(&debug_mess_snd[0], debug_array + indicate_sended, len);	/*需要输出的数据*/
	
	debug_out_array(ZC_INFOR,debug_mess_snd,len);
#endif  	
	return(OK);
}/*end STATUS debug_out_array(char* debug_array, short len)*/

/*=================================================================================================
*	函数名：ZC_debug_out_array
*	功能：ZC输出所需的辅助输出信息。该函数与debug_out_array函数的区别在于该函数用于输出ZC所需数组，
*       数组中具体结构由ZC应用定义。接收到以后也是由ZC应用作的函数进行数据的解析，按特定格式进行记录。
*	参数：	char* debug_array				需要输出的数组首地址
*			short len						需要输出的数组长度，最大不宜超过MAX_LEN_DEBUG*50
*	返回值：OK or ERROR
*==================================================================================================*/
STATUS ZC_debug_out_array(char* debug_array, short len)
{
#if 0
	short indicate_sended = 0;				/*已发送数据指针，用于在一帧不能完全输出数组时，指示已经发送到什么地方了*/
	unsigned char debug_mess_snd[MAX_LEN_DEBUG] = {0};	/*辅助调试输出信息的发送缓冲区*/

    struct sockaddr_in  aimAddr;         		/*目的socket设置*/
	char   *aimIP = AIM_IP;     		        /*对方IP*/		

	bzero ((char *) &aimAddr, sizeof (struct sockaddr_in));
   	aimAddr.sin_len = (u_char) sizeof (struct sockaddr_in);
   	aimAddr.sin_family = AF_INET;
   	aimAddr.sin_port = htons (DEBUG_PORT);

   	if (((aimAddr.sin_addr.s_addr = inet_addr (aimIP)) == ERROR) && 	/*查看IP是否对应*/
       ((aimAddr.sin_addr.s_addr = hostGetByName (aimIP)) == ERROR))
   	{
   	 	perror ("unknown aimIP\n");
   	 	printf("Warning:unknown aimIP\n");
   	 	close (socket_debug);
        return(ERROR);
   	}/*end 查看IP是否对应*/
    
	while(len >= MAX_LEN_DEBUG)				/*一帧不能发送完所有数据*/
	{
		memset(debug_mess_snd, 0, MAX_LEN_DEBUG);									/*清零调试消息发送缓冲区*/
	    debug_mess_snd[0] = ZC_INFOR;              /*填写存储方式标识位*/
		memmove(&debug_mess_snd[1], debug_array + indicate_sended, MAX_LEN_DEBUG-1);	/*需要输出的数据，*/
																					/*需要减1，因为已经占用了第一个字节了。*/
		
		if(ERROR == sendto(socket_debug, debug_mess_snd, MAX_LEN_DEBUG, 0, (struct sockaddr *)&aimAddr,sizeof (struct sockaddr_in)))
        {
   	        /*perror("error of send debug information\n");   */
   	        return(ERROR);	       
   	
   	    }/*end if(ERROR == sendto())*/   	
        else
        {
   	        /*清零接收缓冲区*/
   	        memset(debug_mess_snd, 0,  MAX_LEN_DEBUG);/*数据长度不是整个缓冲区而是实际发送的数据长度，这样做是为了提高效率，避免对未使用缓冲区重复清零*/
   	        
   	    }/*发送成功*/
		indicate_sended = indicate_sended + MAX_LEN_DEBUG-1;		/*更新已发送位置指针*/
		len = len - MAX_LEN_DEBUG+1;								/*更新未发送数据长度*/
		
	}/*end while(len <= MAX_LEN_DEBUG)*/

	/*剩余数据一帧能完全输出*/
	memset(debug_mess_snd, 0, MAX_LEN_DEBUG);						/*清零调试消息发送缓冲区*/
	debug_mess_snd[0] = ZC_INFOR;              /*填写存储方式标识位*/
	memmove(&debug_mess_snd[1], debug_array + indicate_sended, len);	/*需要输出的数据*/
	
    if(ERROR == sendto(socket_debug, debug_mess_snd, len+1, 0, (struct sockaddr *)&aimAddr,sizeof (struct sockaddr_in)))
    {
   	    /*perror("error of send debug information\n"); */
   	    return(ERROR);  	    
   	
   	}/*end if(ERROR == sendto())*/   	
    else
    {
   	    /*清零接收缓冲区*/
   	    memset(debug_mess_snd, 0,  len+1);/*数据长度不是整个缓冲区而是实际发送的数据长度，这样做是为了提高效率，避免对未使用缓冲区重复清零*/
   	    
   	}/*发送成功*/
#endif   	
	return(OK);
}/*end STATUS debug_out_array(char* debug_array, short len)*/

/*=================================================================================================
*	函数名：debug_infor_printf
*	功能：用于实现C语言中的printf函数的功能，目前根据现场调试的需求实现了%d,%c,%x以及%s的功能，若有
          新的需求可以根据实际情况进行程序的编写
*	参数：	char* debug_string          需要输出的字符串首地址
            ...                         不定数量的变量
*	返回值：OK or ERROR

    注意：所传参数不要超过十个。目前只处理不超过十个参数。
*==================================================================================================*/
#if 0
STATUS debug_infor_printf(char* debug_string, ...)
{
	 
	short len_out = 0;			/*需要输出的数据长度*/
	short chCycle = 0;

    /*使用va_list初始化*/
	va_list arg_ptr;

	int dwTempNumber = 0;
	char chTempChar = 0;
	char  * chTempArray;
	char debug_mess_snd[MAX_LEN_DEBUG];	/*辅助调试输出信息的发送缓冲区*/
	char debug_mess_temp[MAX_LEN_DEBUG];   /*局部的用于缓存数组内容的缓冲区*/



       
	unsigned long CRC_Check_Sum = 0;   /*用来保存计算出来的CRC测试结果*/
	unsigned long SequenceNo = 0;
	

	
	va_start(arg_ptr,10);

	memset(debug_mess_temp, 0 ,MAX_LEN_DEBUG);  /*清零用于缓存数组内容的缓冲区*/
	memset(debug_mess_snd, 0, MAX_LEN_DEBUG);	/*清零调试消息发送缓冲区*/


	memmove(debug_mess_temp,debug_string,strlen(debug_string));/*将输入的字符串数组拷贝到temp数组*/

	while(chCycle < (short)strlen(debug_mess_temp))
	{
 	    if(debug_mess_temp[chCycle] == '%')
        {
            switch(debug_mess_temp[chCycle + 1])
            {
                case 'd':
                {
                    /*获取整形变量的具体数值*/
                    dwTempNumber = va_arg(arg_ptr, int);

                    /*调用进行字符串合并的函数*/
                    String_connect(debug_mess_temp,chCycle,dwTempNumber);

                    break;
                }
                case 'x':
                {
                    /*获取整形变量的具体数值*/
                    dwTempNumber = va_arg(arg_ptr, int);
                    /*调用进行字符串合并的函数*/
                    String_connect(debug_mess_temp,chCycle,dwTempNumber);

                    break;
                }
                case 'c':
                {
                    /*获取字符型变量的具体数值*/
                    chTempChar = va_arg(arg_ptr, int);
                    /*调用进行字符串合并的函数*/
                    String_connect(debug_mess_temp,chCycle,chTempChar);

                    break;
                }
                case 's':
                {
                    /*获取字符串的具体数值*/
                    chTempArray = va_arg(arg_ptr, char *);
                    /*调用进行字符串合并的函数*/
                    String_connect(debug_mess_temp,chCycle,chTempArray);
                    break;
                }
                default:
                {
                    break;
                }
            }/*#end of switch*/
        }/*#end of if*/
        chCycle++;
	}/*#end of while*/

      
	strcpy(debug_mess_snd, debug_mess_temp);
	len_out = (short)strlen(debug_mess_temp);		/*输出数据长度，因为是从第16个字节开始写字符串*/
	
	debug_out_array(STRING,debug_mess_snd,len_out);
	
	va_end(arg_ptr);

	return(OK);
}/*end STATUS debug_infor_out(int decimal_data1, int decimal_data2, int hexdata, char* debug_string)*/
#endif

/*=================================================================================================
*	函数名：String_connect
*	功能：用于处理需要进行字符串连接的函数
*	参数：	char Temp_String[MAX_LEN_DEBUG]          需要输出的字符串首地址
            short wLength                            记录获得的%*的位置
            ...                                      不定数量的变量
*	返回值：无
*==================================================================================================*/
void String_connect(char Temp_String[MAX_LEN_DEBUG],short wLength,...)
{
#if 0
    char num2 = 1;

	char chTempString[MAX_LEN_DEBUG];   /*用于存放%*之后的字符串的临时数组*/
    char chTempPreString[MAX_LEN_DEBUG];   /*用于存放%*之前的字符串的临时数组*/

	short wTempCycle = 0;      /*控制存放%*之前临时数组下标的局部变量*/
    char wCycleNum = 0;       /*控制存放%*之后临时数组下标的局部变量*/
    char chCycle;
    char tmp = 2;

    int dwInputNumber;
    char chInputChar;
    char * chInputArray;

    /*使用va_list初始化*/
	va_list arg_ptr;

	va_start(arg_ptr,1);

    /*初始化数组*/
    memset(chTempString, 0 ,MAX_LEN_DEBUG);
    memset(chTempPreString, 0 ,MAX_LEN_DEBUG);

    /*将%*之前的字符串顺序存放到chTempPreString数组中*/
    for(wTempCycle = 0;wTempCycle < wLength;wTempCycle++)
    {
    	chTempPreString[wTempCycle] = Temp_String[wTempCycle];
    }

    switch(Temp_String[wLength + 1])
    {
        case 'd':
        {
            dwInputNumber = (int)va_arg(arg_ptr, int);
            /*记录Format_decimal_to_char函数的返回值，将其作为数组的起始位置*/
            wCycleNum = Format_decimal_to_char(chTempString,dwInputNumber);
            break;
        }
        case 'x':
        {
            dwInputNumber = va_arg(arg_ptr, int);

            /*记录Format_hex_to_char函数的返回值，将其作为数组的起始位置*/
            wCycleNum = Format_hex_to_char(chTempString,dwInputNumber);

            break;
        }
        case 'c':
        {
            chInputChar = (char)va_arg(arg_ptr, int);
            /*由于char型数据不需要进行转化，直接占用1个字节的位置*/
            chTempString[wCycleNum++] = chInputChar;
            break;
        }
        case 's':
        {
            chInputArray = va_arg(arg_ptr, char *);
            /*记录Format_string_to_char函数的返回值，表示字符串的长度，将其作为数组的起始位置*/
            wCycleNum = Format_string_to_char(chTempString,chInputArray);
            break;
        }
        default:
        {
            break;
        }
    }/*#end of switch*/

    /*将%d %x %c %s后面的其他字符串暂时缓存在一个局部数组中*/
    for(chCycle = 0;chCycle <= (strlen((char *)Temp_String) - 2 - wLength + 1);chCycle++)
    {
        chTempString[wCycleNum] = Temp_String[wLength + (tmp++)];
        wCycleNum++;
    }

    /*将经过处理后的两个数组连接起来，组成新的用于输出的数组*/
    strcat(chTempPreString,chTempString);

    /*将Temp_String数组初始化成空，用于方便将组合后的数组重新覆盖原先的字符串*/
    memset(Temp_String, 0 ,MAX_LEN_DEBUG);
    /*将组合后的数组拷贝回原先的数组中，替换原先的数组*/
    memmove(Temp_String,chTempPreString,strlen(chTempPreString));
#endif
}


/*=================================================================================================
*	函数名：Format_decimal_to_char
*	功能：用于将十进制整数转化为char型的字符串用于输出
*	参数：	char Array[MAX_LEN_DEBUG]          需要输出的字符串首地址
            unsigned int Interger              存放函数传送过来的整形数据
*	返回值：chFigure                           返回计算获得的十进制整数的位数
*==================================================================================================*/

char Format_decimal_to_char(char Array[MAX_LEN_DEBUG],unsigned int Interger)
{
	char chCycle;
	char chCycle_temp;
	unsigned char chFlag = 0;
	unsigned char chFigure = 1;    /*用于存放计算获得的整数位数的变量*/

	if(0 == Interger)
	{
	    Array[0] = '0';
    }/*end if(0 == Interger)*/
	else /*非0整数*/
	{
	    for(chCycle_temp = 9;chCycle_temp >= 0;chCycle_temp--)
    	{
    		/*将该数据除以10的整数次方，用于确定该整数的十进制位数，再将其按位转化为char型数据，用于输出*/
    		if(Interger / (power(10,chCycle_temp)))
    		{
    			/*判断获得第一次非零的位数，由此来确定该整数的位数*/
    			chFigure = chCycle_temp + 1;
    			/*跳出for循环*/
    			break;
    		}/*#end of if*/
    	}/*#end of for*/

    	/*在获取到整数的位数后将该整数按照十进制的位数转换为char型数据，存放到数组中*/
    	for(chCycle = chFigure - 1;chCycle >= 0;chCycle--)
    	{
    		Array[chFlag] = Interger / (power(10,chCycle)) + 48;
    		Interger = Interger % (power(10,chCycle));
    		chFlag++;
    	}
	}/*end 非0整数*/

	/*返回十进制数的具体位数*/
	return(chFigure);
}

/*=================================================================================================
*	函数名：Format_hex_to_char
*	功能：用于将十六进制的数转化为char型的字符串输出
*	参数：	char Array[MAX_LEN_DEBUG]          需要输出的字符串首地址
            unsigned int Hex                   存放函数传送过来的需要转换为十六进制的数据
*	返回值：6                                  返回十六进制整数的位数
*==================================================================================================*/

char Format_hex_to_char(char Array[MAX_LEN_DEBUG],unsigned int Hex)
{
	char chCycle;
	unsigned char chFlag = 2;
	char chFlagForHex = 0;    /*用于标识第一位不为零的十六进制数的标志位*/

	Array[0] = '0';
	Array[1] = 'x';
	/*将该整数按照十六进制的位数转换为char型数据，存放到数组中*/
	if(0 == Hex)
	{
	    Array[2] = '0';
	    chFlag++;
	}/*end if(0 == Hex)*/
	else/*非0数据*/
	{
	    for(chCycle = 7;chCycle >= 0;chCycle--)
    	{
    		if(((Hex / (power(16,chCycle))) > 9)&&((Hex / (power(16,chCycle))) < 16))
    		{
    			Array[chFlag] = Hex / (power(16,chCycle)) + 87;
    			chFlagForHex = 1;
    			chFlag++;
    		}
    		else if((Hex / (power(16,chCycle)) > 0)&&((Hex / (power(16,chCycle))) < 10))
    		{
    			Array[chFlag] = Hex / (power(16,chCycle)) + 48;
    			chFlagForHex = 1;
    			chFlag++;
    		}
    		else if((chFlagForHex == 1)&&((Hex / (power(16,chCycle)) == 0)))
    		{
    			Array[chFlag] = Hex / (power(16,chCycle)) + 48;
    			chFlagForHex = 1;
    			chFlag++;
    		}
    		else
    		{
    			chFlagForHex = 0;
    		}
    		Hex = Hex % (power(16,chCycle));
    	}/*end for(chCycle = 7;chCycle >= 0;chCycle--)*/
	}/*end 非0数据*/

	return(chFlag); /*返回转换后的字符串长度*/
}


/*=================================================================================================
*	函数名：Format_string_to_char
*	功能：用于将字符串插入到原先的输出字符串中的函数
*	参数：	char Array[MAX_LEN_DEBUG]          需要输出的字符串首地址
            char String[MAX_LEN_DEBUG]         存放函数传送过来的字符串数据
*	返回值：wLength                            返回计算获得的字符串的长度
*==================================================================================================*/

char Format_string_to_char(char Array[MAX_LEN_DEBUG],char String[MAX_LEN_DEBUG])
{
	short wLength = 0;
	short wCycleNum;
	/*将%s传送过来的字符串的长度进行计算*/
	while(String[wLength] != '\0')
	{
		wLength++;
	}

	/*将字符串的数据填写给chTempString数组中*/
	for(wCycleNum = 0;wCycleNum < wLength;wCycleNum++)
	{
		Array[wCycleNum] = String[wCycleNum];
	}

	return(wLength);
}


/*=================================================================================================
*	函数名：power
*	功能：用于计算任意数的任意次幂的函数
*	参数：	int dwData                         底数
            char chEquation                    指数
*	返回值：dwTotal                            幂。即dwData的chEquation次方的结果
*==================================================================================================*/
long power(int dwData,char chEquation)
{
	long dwTotal = 1;                     /*用于存放任意数的任意次幂的结果的变量*/
	char chCycleNum;
	for(chCycleNum = 0;chCycleNum < chEquation;chCycleNum++)
	{
		dwTotal = dwTotal * dwData;
	}

	return(dwTotal);
}
