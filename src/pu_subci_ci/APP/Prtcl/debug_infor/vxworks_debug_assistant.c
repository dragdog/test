/***************************************************************************************
*		�汾�ţ�V1.0
*
*		��Ȩ���� (C) 2007 ������ͨ��ѧ CBTC��Ŀ 2��2ȡ2��ȫ�����������
*
*		���ߣ�����Ȫ
*
*		�ļ�����vxWorks_debug_Assistant.c
*
*       ������vxWorks�������Թ����У�vxWorks��������ļ�
*
*       �ο��ĵ���
*
*       ����ʱ�䣺2007-5-9
*
*		�޸���ʷ��
*			2007-5-10 	����Ȫ	���ӻ����ź���semM_debug_infor�������ڷ�����Ϣ��ȫ�����顣
*			2007-5-10 	����Ȫ	��������ܸ�Ϊһ��������д��Ϣ���в��ֶ���������������������С�
*			2007-5-10 	����Ȫ	��������ݳ��ȸ���ʵ�ʳ��Ȳ�ͬ��ͬ�������Ƕ�����������ݣ��Լ��ٴ洢ʱ�Դ洢
*				�ռ���˷ѡ�
*			2007-5-12 	����Ȫ	�����������������debug_out_array�����鲻Ҫһ��������ݳ��ȳ���50*MAX_LEN_DEBUG��
*           2007-5-12 	����Ȫ  �޸�����λ��ͨ���ǵĸ�ʽ��ÿ��ͨ��ʱ��һ���ֽ���Ϊ�������ݴ洢��ʽ��ʶ����
*       		0x55��ʾ��ʽһ��0xAA��ʾ��ʽ�����洢��ʽһ����һ���ֽ�Ϊ�洢��ʽ��ʶ������߽����Ŵ�����4�ֽ�����
*       		��������������ʮ�����ƴ��һ��4�ֽڱ������ӵ�ʮ����ֽڿ�ʼ���ַ�����ʽ�洢���洢��ʽ�����������ݶ���ʮ
*               ��������ʽ�洢��
*			2007-5-14 	����Ȫ	���������������Ϣʱ���������ݳ��ȡ�
*			2007-5-16 	����Ȫ	�������������������һ��bug��ԭ���ᶪ�����һ���ֽڡ�
*			2007-5-17 	����Ȫ	�޸����ڱ���ȫ����Դ���ź���Ϊ�������ź���semB_debug_infor��
*			2007-5-19 	����Ȫ	ȥ��������Ϣ���������������������ж�ȫ�ֱ����ı�������Ϊ���ǻ��������ź�����������µ�
*				��¼ʧ�ܡ�
*           2007-5-25   ����Ȫ  �����ȫ����Դ�ı�����Ϊ�������봦��ԭ��ʹ��#if 0��������ȥ����
*           2007-6-19   ����Ȫ  ����ʾ�����ջ��С�ĺ��ΪDEBUG_TASK_STACK_SIZE��
*           2007-6-19   ����Ȫ  �޸�debug�з��ش���ʱ�Ĵ�������return��
*           2007-6-25   ����Ȫ  �����ͻ�����debug_mess_snd��Ϊ�ֲ���������Ϊû�б�Ҫ��Ϊȫ�ֵġ���������������ǿ�������ˣ�
*               �������������ź�����֤�䲻�����롣
*           2007-6-26   ����    �����˵�����printf��ʾ����debug_infor_out_printf()��
*           2007-7-13   ��˷    ����ZCӦ��������ݺ���ZC_debug_out_array()��
*           2007-7-20   ������  ����������Ŀ���������������debug_infor_printf()�����õ�����غ�����
*           2007-7-25   ������  �������������ݵĺ���power����Ϊ������ѧ�⡶math.h���е����ݺ���pow����©���������
*               ��������ʱ�����ʱ�򲻶ԡ�
*           2007-7-30   ����Ȫ  ȥ���˹�������ȫ����Դ�������ź���������������롣��Ϊû��ȫ�ֵ���Դ��Ҫ���������ͻ��������ĳ��˾ֲ����ˡ�
*           2007-7-30   ����Ȫ  ������Format_hex_to_char�����ж�0������ݵĴ���ԭ��������
*           2007-8-2    ����Ȫ  ������Ϣ���е����ã���ԭ��1000����Ϣ��ÿ����Ϣ���1000�ֽڸ�Ϊ���2000��Ϣ��ÿ����Ϣ���500��
*               �ֽڣ�������Ҫ��Ϊ����߼�¼��������Ϊ�󲿷ּ�¼��ʵ������500�ֽڣ�������Ϣ���ܼ�С��¼��ȫ�Ŀ����ԡ�
*           2007-8-24   ����Ȫ  �޸�������������Ĳ������ã����Ӽ�¼��ʽ�Ĳ����������Ϳ��Խ������������������������ϡ�Ҳ������չ���ܡ�
*           2007-8-24   ����Ȫ  ��16���Ƽ�¼�ļ�¼��ʽ�궨����ԭ����MODE2��ΪHEX_MODE��ֵ���䡣
*           2007-9-10   ����Ȫ  �����������ȼ��궨�塣
            2008-7-22   ��Ϥ    ��ԭ���ķ�������ȡ������sendto�����ŵ���ÿһ������ĺ����У�ȡ������Ϣ���к��������йصĺ궨��
            2008-7-22   ��Ϥ    ����˷��Ͷ˵����кź�crc
            2010-2-22   ��Ϥ    debug����������û��ʹ�õ��ź���ɾ����
            2010-6-30   ��Ϥ    ɾ����debug_infor_out��debug_infor_out_printf����
            2010-7-5    ��Ϥ    ��debug_infor_printf��debug_out_array�����м�����crc�����к�
			2010-8-26   ��˧    ��debug_out_array�������޸�while(len  >= MAX_LEN_DEBUG-8)Ϊwhile(len  >= MAX_LEN_DEBUG-9)��
								9����ǰ��4���ֽڵ����кź�1���ֽڵ�mode��ʾ�����4���ֽڵ�CRC
			2010-8-26   ��˧    �޸�debug_out_array������memmove(&debug_mess_snd[5], debug_array + indicate_sended, MAX_LEN_DEBUG-5)Ϊ
								memmove(&debug_mess_snd[5], debug_array + indicate_sended, MAX_LEN_DEBUG-9)
								��ȥ9����Ϊ9����ǰ��4���ֽڵ����кź�1���ֽڵ�mode��ʾ�����4���ֽڵ�CRC
			2010-8-26   ��˧    �޸�debug_out_array������CRC���CRC_Check_Sum = Crc32long ( ( unsigned char *)&debug_mess_snd,  ( MAX_LEN_DEBUG-5 ) )Ϊ
								CRC_Check_Sum = Crc32long ( ( unsigned char *)&debug_mess_snd,  ( MAX_LEN_DEBUG-4 ) )
								��ȥ4����Ϊ����������4���ֽ�����CRC
			2010-8-26   ��˧    ��debug_out_array�����д����������ݣ������������ʱ���Ϊ��֡���ͣ��޸�Ϊ����һ֡�����ͣ�Ȼ��					
								vxWorks_debug_Assistant.h�ļ���MAX_LEN_DEBUG��ԭ����1000�޸�Ϊ10000��
    ���ڵ����⣺
        1��ż���м�¼����ȫ�����������ͨ�����÷�����Ϣ����ʱʱ������Լ���������ͨ���ط����Ʊ��⡣
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


/*��������������ȫ�ֱ���*/
int socket_debug_green = 0;								/*�������������Ϣ��socket*/
int socket_debug_purple = 0;								/*�������������Ϣ��socket*/
int socket_debug = 0;								/*�������������Ϣ��socket*/
unsigned long SN_send1 = 0;


/*=================================================================================================
*	��������debug_initial
*	���ܣ��������������Ϣ�����Դ�ĳ�ʼ��
*	������void
*	����ֵ��OK or ERROR
*==================================================================================================*/
STATUS debug_initial( void )
{
#if 0
	struct sockaddr_in  selfGreenAddr;         		/*���ñ���socket*/
	struct sockaddr_in  selfPurpleAddr;         		/*���ñ���socket*/
	struct sockaddr_in  selfAddr;         			/*���ñ���socket*/

   if((socket_debug = socket(AF_INET,SOCK_DGRAM,0)) == ERROR) 				/*����socket*/
   	{
      	perror("socket created purple fail!");
      	printf("ERROR:socket():socket_debug!\n");
      	return(ERROR);
   	}
   	
	if((socket_debug_green = socket(AF_INET,SOCK_DGRAM,0)) == ERROR)		/*����socket*/
   	{
      	perror("socket created green fail!");
      	printf("ERROR:socket():socket_debug_green!\n");
      	return(ERROR);
   	}
   	
   	if((socket_debug_purple = socket(AF_INET,SOCK_DGRAM,0)) == ERROR)		/*����socket*/
   	{
      	perror("socket created purple fail!");
      	printf("ERROR:socket():socket_debug_purple!\n");
      	return(ERROR);
   	}
#endif
   	return(OK);

}/*end STATUS debug_initial( void )*/


/*=================================================================================================
*	��������debug_out_array
*	���ܣ��������ĸ��������Ϣ���ú�����debug_infor_out�������������ڸú����������鷽ʽ�����
*	������	char  mode                      ��¼��ʽ��ʶ��
*           char* debug_array				��Ҫ����������׵�ַ
*			short len						��Ҫ��������鳤�ȣ�����˳���MAX_LEN_DEBUG*50
*	����ֵ��OK or ERROR
*==================================================================================================*/
#if 0
STATUS debug_out_array(char mode, char* debug_array, short len)
{
#if 0
	unsigned char debug_mess_snd_green[MAX_LEN_DEBUG] = {0};	/*�������������Ϣ�ķ��ͻ�����*/
	unsigned char debug_mess_snd_purple[MAX_LEN_DEBUG] = {0};	/*�������������Ϣ�ķ��ͻ�����*/

  struct sockaddr_in  aimAddrGreen;         		/*Ŀ��socket����*/
  struct sockaddr_in  aimAddrPurple;         		/*Ŀ��socket����*/
	char   *aimIP_green = AIM_IP_GREEN;     		        /*�Է�IP*/	
	char   *aimIP_purple = AIM_IP_PURPLE;     		        /*�Է�IP*/
	
	unsigned char dataLen = 17;                   /* һ֡���ݲ�����CRC�ܹ���֡ͷ���ȡ�*/
  unsigned long SequenceNo_array;               /* ���к����ڼ�����Ϣ �����Ĵ���4 BYTE */

	unsigned long CRC_Check_Sum = 0;   /*����������������CRC���Խ��*/
	
	if ( len >= MAX_OUT_DEBUG_INFO_SIZE ) 
	{
		len = MAX_OUT_DEBUG_INFO_SIZE;
	}
	else
	{
	}
	
	SequenceNo_array = ++SN_send1; /*����֡���*/  /*����ط����к���Ҫע������ʹ�ã���ͬ����ʹ�ò�ͬ��*/
	
	bzero ((char *) &aimAddrGreen, sizeof (struct sockaddr_in));
  aimAddrGreen.sin_len = (u_char) sizeof (struct sockaddr_in);
  aimAddrGreen.sin_family = AF_INET;
  aimAddrGreen.sin_port = htons (DEBUG_PORT);
  
  if (((aimAddrGreen.sin_addr.s_addr = inet_addr (aimIP_green)) == ERROR) && 	/*�鿴IP�Ƿ��Ӧ*/
     ((aimAddrGreen.sin_addr.s_addr = hostGetByName (aimIP_green)) == ERROR))
  {
   	/*perror ("unknown aimIP\n");
   	printf("unknown aimIP, errno is %x\n", errno);*/
   	close (socket_debug_green);
      return(ERROR);
  }/*end �鿴IP�Ƿ��Ӧ*/	
  
  bzero ((char *) &aimAddrPurple, sizeof (struct sockaddr_in));
  aimAddrPurple.sin_len = (u_char) sizeof (struct sockaddr_in);
  aimAddrPurple.sin_family = AF_INET;
  aimAddrPurple.sin_port = htons (DEBUG_PORT);
  
  if (((aimAddrPurple.sin_addr.s_addr = inet_addr (aimIP_purple)) == ERROR) && 	/*�鿴IP�Ƿ��Ӧ*/
     ((aimAddrPurple.sin_addr.s_addr = hostGetByName (aimIP_purple)) == ERROR))
  {
   	/*perror ("unknown aimIP\n");
   	printf("unknown aimIP, errno is %x\n", errno);*/
   	close (socket_debug_purple);
      return(ERROR);
  }/*end �鿴IP�Ƿ��Ӧ*/	
  
	/*���ڽ��������޸ĵĺܴ�����һ֡����ȫ���*/
	memset(debug_mess_snd_green, 0, MAX_LEN_DEBUG);						/*���������Ϣ���ͻ�����*/
	memset(debug_mess_snd_purple, 0, MAX_LEN_DEBUG);						/*���������Ϣ���ͻ�����*/

	debug_mess_snd_green[0] = 0xa0;                                 /* ֡ͷ*/
	debug_mess_snd_green[1] = 0x50;                                 /* Э������*/
	debug_mess_snd_green[2] = 0x01;                                 /* Э��汾��*/
	debug_mess_snd_green[3] = 0xa1;                                 /* ֡����*/
	debug_mess_snd_green[4] = mode;                                 /* ������*/
	
	LongToChar(SequenceNo_array,&debug_mess_snd_green[5]);          /*��Ӧ����������к� 4�ֽ� �����ֽ�*/
	
	debug_mess_snd_green[9] = 0x00; 
	debug_mess_snd_green[10] = 0x3c;                                /* Դ����ID*/                                
	debug_mess_snd_green[11] = 0x00;                                 
	debug_mess_snd_green[12] = 0x01;                                /* Ŀ�������*/                                  
	debug_mess_snd_green[13] = 0x00;                                 
	debug_mess_snd_green[14] = 0x03;                                /* Ŀ�����ID*/ 
	
	ShortToChar(len, &debug_mess_snd_green[15]);                    /* �����򳤶ȡ�*/                                  
	                         
	memmove(&debug_mess_snd_green[17], debug_array, len);           /* ������*/
	
	 /*CRC���*/
  CRC_Check_Sum = Crc32long ( ( unsigned char *)&debug_mess_snd_green,  len+dataLen );/*����CRC*/
  
  LongToChar(CRC_Check_Sum,&debug_mess_snd_green[len + dataLen]);
  
  memcpy(debug_mess_snd_purple,debug_mess_snd_green,len+dataLen+4);
  
  #ifdef GREEN_NET_MAIN	
  if(ERROR == sendto(socket_debug_green, debug_mess_snd_green,  len+dataLen+4, 0, (struct sockaddr *)&aimAddrGreen,sizeof (struct sockaddr_in)))
  {   	    
      return(ERROR);	  
  
  }/*end if(ERROR == sendto())*/   	
  else
  {
      /*������ջ�����*/
      memset(debug_mess_snd_green, 0,  len+dataLen+4);/*���ݳ��Ȳ�����������������ʵ�ʷ��͵����ݳ��ȣ���������Ϊ�����Ч�ʣ������δʹ�û������ظ�����*/
     
  }/*���ͳɹ�*/
  #endif
  
  #ifdef PURPLE_NET_MAIN	
  if(ERROR == sendto(socket_debug_purple, debug_mess_snd_purple,  len+dataLen+4, 0, (struct sockaddr *)&aimAddrPurple,sizeof (struct sockaddr_in)))
  {   	    
      return(ERROR);
  }/*end if(ERROR == sendto())*/   	
  else
  {
      /*������ջ�����*/
      memset(debug_mess_snd_purple, 0,  len+dataLen+4);/*���ݳ��Ȳ�����������������ʵ�ʷ��͵����ݳ��ȣ���������Ϊ�����Ч�ʣ������δʹ�û������ظ�����*/
     
  }/*���ͳɹ�*/
  #endif
#endif   	
	return(OK);
}/*end STATUS debug_out_array(char* debug_array, short len)*/
#endif

STATUS CI_debug_out_array(char* debug_array, short len)
{
#if 0	
	short indicate_sended = 0;				/*�ѷ�������ָ�룬������һ֡������ȫ�������ʱ��ָʾ�Ѿ����͵�ʲô�ط���*/
	unsigned char debug_mess_snd[MAX_LEN_DEBUG] = {0};	/*�������������Ϣ�ķ��ͻ�����*/

    
	while(len >= MAX_LEN_DEBUG)				/*һ֡���ܷ�������������*/
	{
		memset(debug_mess_snd, 0, MAX_LEN_DEBUG);									/*���������Ϣ���ͻ�����*/
		memmove(&debug_mess_snd[0], debug_array + indicate_sended, MAX_LEN_DEBUG);	/*��Ҫ��������ݣ�*/
																					
		debug_out_array(ZC_INFOR,debug_mess_snd,MAX_LEN_DEBUG);

		indicate_sended = indicate_sended + MAX_LEN_DEBUG;		/*�����ѷ���λ��ָ��*/
		len = len - MAX_LEN_DEBUG;								/*����δ�������ݳ���*/
		
	}/*end while(len <= MAX_LEN_DEBUG)*/

	/*ʣ������һ֡����ȫ���*/
	memset(debug_mess_snd, 0, MAX_LEN_DEBUG);						/*���������Ϣ���ͻ�����*/
	memmove(&debug_mess_snd[0], debug_array + indicate_sended, len);	/*��Ҫ���������*/
	
	debug_out_array(ZC_INFOR,debug_mess_snd,len);
#endif  	
	return(OK);
}/*end STATUS debug_out_array(char* debug_array, short len)*/

/*=================================================================================================
*	��������ZC_debug_out_array
*	���ܣ�ZC�������ĸ��������Ϣ���ú�����debug_out_array�������������ڸú����������ZC�������飬
*       �����о���ṹ��ZCӦ�ö��塣���յ��Ժ�Ҳ����ZCӦ�����ĺ����������ݵĽ��������ض���ʽ���м�¼��
*	������	char* debug_array				��Ҫ����������׵�ַ
*			short len						��Ҫ��������鳤�ȣ�����˳���MAX_LEN_DEBUG*50
*	����ֵ��OK or ERROR
*==================================================================================================*/
STATUS ZC_debug_out_array(char* debug_array, short len)
{
#if 0
	short indicate_sended = 0;				/*�ѷ�������ָ�룬������һ֡������ȫ�������ʱ��ָʾ�Ѿ����͵�ʲô�ط���*/
	unsigned char debug_mess_snd[MAX_LEN_DEBUG] = {0};	/*�������������Ϣ�ķ��ͻ�����*/

    struct sockaddr_in  aimAddr;         		/*Ŀ��socket����*/
	char   *aimIP = AIM_IP;     		        /*�Է�IP*/		

	bzero ((char *) &aimAddr, sizeof (struct sockaddr_in));
   	aimAddr.sin_len = (u_char) sizeof (struct sockaddr_in);
   	aimAddr.sin_family = AF_INET;
   	aimAddr.sin_port = htons (DEBUG_PORT);

   	if (((aimAddr.sin_addr.s_addr = inet_addr (aimIP)) == ERROR) && 	/*�鿴IP�Ƿ��Ӧ*/
       ((aimAddr.sin_addr.s_addr = hostGetByName (aimIP)) == ERROR))
   	{
   	 	perror ("unknown aimIP\n");
   	 	printf("Warning:unknown aimIP\n");
   	 	close (socket_debug);
        return(ERROR);
   	}/*end �鿴IP�Ƿ��Ӧ*/
    
	while(len >= MAX_LEN_DEBUG)				/*һ֡���ܷ�������������*/
	{
		memset(debug_mess_snd, 0, MAX_LEN_DEBUG);									/*���������Ϣ���ͻ�����*/
	    debug_mess_snd[0] = ZC_INFOR;              /*��д�洢��ʽ��ʶλ*/
		memmove(&debug_mess_snd[1], debug_array + indicate_sended, MAX_LEN_DEBUG-1);	/*��Ҫ��������ݣ�*/
																					/*��Ҫ��1����Ϊ�Ѿ�ռ���˵�һ���ֽ��ˡ�*/
		
		if(ERROR == sendto(socket_debug, debug_mess_snd, MAX_LEN_DEBUG, 0, (struct sockaddr *)&aimAddr,sizeof (struct sockaddr_in)))
        {
   	        /*perror("error of send debug information\n");   */
   	        return(ERROR);	       
   	
   	    }/*end if(ERROR == sendto())*/   	
        else
        {
   	        /*������ջ�����*/
   	        memset(debug_mess_snd, 0,  MAX_LEN_DEBUG);/*���ݳ��Ȳ�����������������ʵ�ʷ��͵����ݳ��ȣ���������Ϊ�����Ч�ʣ������δʹ�û������ظ�����*/
   	        
   	    }/*���ͳɹ�*/
		indicate_sended = indicate_sended + MAX_LEN_DEBUG-1;		/*�����ѷ���λ��ָ��*/
		len = len - MAX_LEN_DEBUG+1;								/*����δ�������ݳ���*/
		
	}/*end while(len <= MAX_LEN_DEBUG)*/

	/*ʣ������һ֡����ȫ���*/
	memset(debug_mess_snd, 0, MAX_LEN_DEBUG);						/*���������Ϣ���ͻ�����*/
	debug_mess_snd[0] = ZC_INFOR;              /*��д�洢��ʽ��ʶλ*/
	memmove(&debug_mess_snd[1], debug_array + indicate_sended, len);	/*��Ҫ���������*/
	
    if(ERROR == sendto(socket_debug, debug_mess_snd, len+1, 0, (struct sockaddr *)&aimAddr,sizeof (struct sockaddr_in)))
    {
   	    /*perror("error of send debug information\n"); */
   	    return(ERROR);  	    
   	
   	}/*end if(ERROR == sendto())*/   	
    else
    {
   	    /*������ջ�����*/
   	    memset(debug_mess_snd, 0,  len+1);/*���ݳ��Ȳ�����������������ʵ�ʷ��͵����ݳ��ȣ���������Ϊ�����Ч�ʣ������δʹ�û������ظ�����*/
   	    
   	}/*���ͳɹ�*/
#endif   	
	return(OK);
}/*end STATUS debug_out_array(char* debug_array, short len)*/

/*=================================================================================================
*	��������debug_infor_printf
*	���ܣ�����ʵ��C�����е�printf�����Ĺ��ܣ�Ŀǰ�����ֳ����Ե�����ʵ����%d,%c,%x�Լ�%s�Ĺ��ܣ�����
          �µ�������Ը���ʵ��������г���ı�д
*	������	char* debug_string          ��Ҫ������ַ����׵�ַ
            ...                         ���������ı���
*	����ֵ��OK or ERROR

    ע�⣺����������Ҫ����ʮ����Ŀǰֻ��������ʮ��������
*==================================================================================================*/
#if 0
STATUS debug_infor_printf(char* debug_string, ...)
{
	 
	short len_out = 0;			/*��Ҫ��������ݳ���*/
	short chCycle = 0;

    /*ʹ��va_list��ʼ��*/
	va_list arg_ptr;

	int dwTempNumber = 0;
	char chTempChar = 0;
	char  * chTempArray;
	char debug_mess_snd[MAX_LEN_DEBUG];	/*�������������Ϣ�ķ��ͻ�����*/
	char debug_mess_temp[MAX_LEN_DEBUG];   /*�ֲ������ڻ����������ݵĻ�����*/



       
	unsigned long CRC_Check_Sum = 0;   /*����������������CRC���Խ��*/
	unsigned long SequenceNo = 0;
	

	
	va_start(arg_ptr,10);

	memset(debug_mess_temp, 0 ,MAX_LEN_DEBUG);  /*�������ڻ����������ݵĻ�����*/
	memset(debug_mess_snd, 0, MAX_LEN_DEBUG);	/*���������Ϣ���ͻ�����*/


	memmove(debug_mess_temp,debug_string,strlen(debug_string));/*��������ַ������鿽����temp����*/

	while(chCycle < (short)strlen(debug_mess_temp))
	{
 	    if(debug_mess_temp[chCycle] == '%')
        {
            switch(debug_mess_temp[chCycle + 1])
            {
                case 'd':
                {
                    /*��ȡ���α����ľ�����ֵ*/
                    dwTempNumber = va_arg(arg_ptr, int);

                    /*���ý����ַ����ϲ��ĺ���*/
                    String_connect(debug_mess_temp,chCycle,dwTempNumber);

                    break;
                }
                case 'x':
                {
                    /*��ȡ���α����ľ�����ֵ*/
                    dwTempNumber = va_arg(arg_ptr, int);
                    /*���ý����ַ����ϲ��ĺ���*/
                    String_connect(debug_mess_temp,chCycle,dwTempNumber);

                    break;
                }
                case 'c':
                {
                    /*��ȡ�ַ��ͱ����ľ�����ֵ*/
                    chTempChar = va_arg(arg_ptr, int);
                    /*���ý����ַ����ϲ��ĺ���*/
                    String_connect(debug_mess_temp,chCycle,chTempChar);

                    break;
                }
                case 's':
                {
                    /*��ȡ�ַ����ľ�����ֵ*/
                    chTempArray = va_arg(arg_ptr, char *);
                    /*���ý����ַ����ϲ��ĺ���*/
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
	len_out = (short)strlen(debug_mess_temp);		/*������ݳ��ȣ���Ϊ�Ǵӵ�16���ֽڿ�ʼд�ַ���*/
	
	debug_out_array(STRING,debug_mess_snd,len_out);
	
	va_end(arg_ptr);

	return(OK);
}/*end STATUS debug_infor_out(int decimal_data1, int decimal_data2, int hexdata, char* debug_string)*/
#endif

/*=================================================================================================
*	��������String_connect
*	���ܣ����ڴ�����Ҫ�����ַ������ӵĺ���
*	������	char Temp_String[MAX_LEN_DEBUG]          ��Ҫ������ַ����׵�ַ
            short wLength                            ��¼��õ�%*��λ��
            ...                                      ���������ı���
*	����ֵ����
*==================================================================================================*/
void String_connect(char Temp_String[MAX_LEN_DEBUG],short wLength,...)
{
#if 0
    char num2 = 1;

	char chTempString[MAX_LEN_DEBUG];   /*���ڴ��%*֮����ַ�������ʱ����*/
    char chTempPreString[MAX_LEN_DEBUG];   /*���ڴ��%*֮ǰ���ַ�������ʱ����*/

	short wTempCycle = 0;      /*���ƴ��%*֮ǰ��ʱ�����±�ľֲ�����*/
    char wCycleNum = 0;       /*���ƴ��%*֮����ʱ�����±�ľֲ�����*/
    char chCycle;
    char tmp = 2;

    int dwInputNumber;
    char chInputChar;
    char * chInputArray;

    /*ʹ��va_list��ʼ��*/
	va_list arg_ptr;

	va_start(arg_ptr,1);

    /*��ʼ������*/
    memset(chTempString, 0 ,MAX_LEN_DEBUG);
    memset(chTempPreString, 0 ,MAX_LEN_DEBUG);

    /*��%*֮ǰ���ַ���˳���ŵ�chTempPreString������*/
    for(wTempCycle = 0;wTempCycle < wLength;wTempCycle++)
    {
    	chTempPreString[wTempCycle] = Temp_String[wTempCycle];
    }

    switch(Temp_String[wLength + 1])
    {
        case 'd':
        {
            dwInputNumber = (int)va_arg(arg_ptr, int);
            /*��¼Format_decimal_to_char�����ķ���ֵ��������Ϊ�������ʼλ��*/
            wCycleNum = Format_decimal_to_char(chTempString,dwInputNumber);
            break;
        }
        case 'x':
        {
            dwInputNumber = va_arg(arg_ptr, int);

            /*��¼Format_hex_to_char�����ķ���ֵ��������Ϊ�������ʼλ��*/
            wCycleNum = Format_hex_to_char(chTempString,dwInputNumber);

            break;
        }
        case 'c':
        {
            chInputChar = (char)va_arg(arg_ptr, int);
            /*����char�����ݲ���Ҫ����ת����ֱ��ռ��1���ֽڵ�λ��*/
            chTempString[wCycleNum++] = chInputChar;
            break;
        }
        case 's':
        {
            chInputArray = va_arg(arg_ptr, char *);
            /*��¼Format_string_to_char�����ķ���ֵ����ʾ�ַ����ĳ��ȣ�������Ϊ�������ʼλ��*/
            wCycleNum = Format_string_to_char(chTempString,chInputArray);
            break;
        }
        default:
        {
            break;
        }
    }/*#end of switch*/

    /*��%d %x %c %s����������ַ�����ʱ������һ���ֲ�������*/
    for(chCycle = 0;chCycle <= (strlen((char *)Temp_String) - 2 - wLength + 1);chCycle++)
    {
        chTempString[wCycleNum] = Temp_String[wLength + (tmp++)];
        wCycleNum++;
    }

    /*����������������������������������µ��������������*/
    strcat(chTempPreString,chTempString);

    /*��Temp_String�����ʼ���ɿգ����ڷ��㽫��Ϻ���������¸���ԭ�ȵ��ַ���*/
    memset(Temp_String, 0 ,MAX_LEN_DEBUG);
    /*����Ϻ�����鿽����ԭ�ȵ������У��滻ԭ�ȵ�����*/
    memmove(Temp_String,chTempPreString,strlen(chTempPreString));
#endif
}


/*=================================================================================================
*	��������Format_decimal_to_char
*	���ܣ����ڽ�ʮ��������ת��Ϊchar�͵��ַ����������
*	������	char Array[MAX_LEN_DEBUG]          ��Ҫ������ַ����׵�ַ
            unsigned int Interger              ��ź������͹�������������
*	����ֵ��chFigure                           ���ؼ����õ�ʮ����������λ��
*==================================================================================================*/

char Format_decimal_to_char(char Array[MAX_LEN_DEBUG],unsigned int Interger)
{
	char chCycle;
	char chCycle_temp;
	unsigned char chFlag = 0;
	unsigned char chFigure = 1;    /*���ڴ�ż����õ�����λ���ı���*/

	if(0 == Interger)
	{
	    Array[0] = '0';
    }/*end if(0 == Interger)*/
	else /*��0����*/
	{
	    for(chCycle_temp = 9;chCycle_temp >= 0;chCycle_temp--)
    	{
    		/*�������ݳ���10�������η�������ȷ����������ʮ����λ�����ٽ��䰴λת��Ϊchar�����ݣ��������*/
    		if(Interger / (power(10,chCycle_temp)))
    		{
    			/*�жϻ�õ�һ�η����λ�����ɴ���ȷ����������λ��*/
    			chFigure = chCycle_temp + 1;
    			/*����forѭ��*/
    			break;
    		}/*#end of if*/
    	}/*#end of for*/

    	/*�ڻ�ȡ��������λ���󽫸���������ʮ���Ƶ�λ��ת��Ϊchar�����ݣ���ŵ�������*/
    	for(chCycle = chFigure - 1;chCycle >= 0;chCycle--)
    	{
    		Array[chFlag] = Interger / (power(10,chCycle)) + 48;
    		Interger = Interger % (power(10,chCycle));
    		chFlag++;
    	}
	}/*end ��0����*/

	/*����ʮ�������ľ���λ��*/
	return(chFigure);
}

/*=================================================================================================
*	��������Format_hex_to_char
*	���ܣ����ڽ�ʮ�����Ƶ���ת��Ϊchar�͵��ַ������
*	������	char Array[MAX_LEN_DEBUG]          ��Ҫ������ַ����׵�ַ
            unsigned int Hex                   ��ź������͹�������Ҫת��Ϊʮ�����Ƶ�����
*	����ֵ��6                                  ����ʮ������������λ��
*==================================================================================================*/

char Format_hex_to_char(char Array[MAX_LEN_DEBUG],unsigned int Hex)
{
	char chCycle;
	unsigned char chFlag = 2;
	char chFlagForHex = 0;    /*���ڱ�ʶ��һλ��Ϊ���ʮ���������ı�־λ*/

	Array[0] = '0';
	Array[1] = 'x';
	/*������������ʮ�����Ƶ�λ��ת��Ϊchar�����ݣ���ŵ�������*/
	if(0 == Hex)
	{
	    Array[2] = '0';
	    chFlag++;
	}/*end if(0 == Hex)*/
	else/*��0����*/
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
	}/*end ��0����*/

	return(chFlag); /*����ת������ַ�������*/
}


/*=================================================================================================
*	��������Format_string_to_char
*	���ܣ����ڽ��ַ������뵽ԭ�ȵ�����ַ����еĺ���
*	������	char Array[MAX_LEN_DEBUG]          ��Ҫ������ַ����׵�ַ
            char String[MAX_LEN_DEBUG]         ��ź������͹������ַ�������
*	����ֵ��wLength                            ���ؼ����õ��ַ����ĳ���
*==================================================================================================*/

char Format_string_to_char(char Array[MAX_LEN_DEBUG],char String[MAX_LEN_DEBUG])
{
	short wLength = 0;
	short wCycleNum;
	/*��%s���͹������ַ����ĳ��Ƚ��м���*/
	while(String[wLength] != '\0')
	{
		wLength++;
	}

	/*���ַ�����������д��chTempString������*/
	for(wCycleNum = 0;wCycleNum < wLength;wCycleNum++)
	{
		Array[wCycleNum] = String[wCycleNum];
	}

	return(wLength);
}


/*=================================================================================================
*	��������power
*	���ܣ����ڼ�����������������ݵĺ���
*	������	int dwData                         ����
            char chEquation                    ָ��
*	����ֵ��dwTotal                            �ݡ���dwData��chEquation�η��Ľ��
*==================================================================================================*/
long power(int dwData,char chEquation)
{
	long dwTotal = 1;                     /*���ڴ����������������ݵĽ���ı���*/
	char chCycleNum;
	for(chCycleNum = 0;chCycleNum < chEquation;chCycleNum++)
	{
		dwTotal = dwTotal * dwData;
	}

	return(dwTotal);
}
