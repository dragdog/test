
/************************************************************************
*
* �ļ���   ��  
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2011.12.29
* ����     ��  �з����������
* �������� ��  Э��������ѯ����  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef _DSU_PROTCL_FUN_H_
#define _DSU_PROTCL_FUN_H_

#include "CommonTypes.h"

#define DSU_PROTCL_NTP   ((UINT8)0)

#define DSU_PROTCL_SFP   ((UINT8)1)
#define DSU_PROTCL_RP    ((UINT8)2)
#define DSU_PROTCL_RSR   ((UINT8)3)
#define DSU_PROTCL_RSSP   ((UINT8)4)
/*#define DSU_PROTCL_FSFB   ((UINT8)5)*/
#define DSU_PROTCL_RSSP2   ((UINT8)9)

#define DSU_LEN_IP_ADDR 4

#define DSU_MAX_DEST_NUM        10       /*���Ŀ��ϵ����*/

#define NTP_PORT                      123               /*NTPУʱʹ�ö˿�*/

#ifdef __cplusplus
extern "C" {
#endif
	/*�豸������ͨ��Э���ϵ��*/
	typedef struct  
	{
		UINT8 EmitterType;        /*�����豸����*/
		UINT8 ReceptorType;       /*�����豸����*/

		UINT8 CommuType;          /*��ʹ�õ�ͨ��Э�飬1ΪSfp��2ΪRp��3ΪRsr��4ΪRSSP,5ΪRSSP2*/
	}DSU_PROTCL_COMM_PROTCL_STRU;

	typedef struct
	{
		UINT16 DevName;        /*�豸��ʶ��*/	

		UINT8 IpAddrBlue[DSU_LEN_IP_ADDR];      /*����ip��ַ*/
		UINT8 IpAddrRed[DSU_LEN_IP_ADDR];       /*����ip��ַ*/

		UINT16 SfpBluePort;    /*Sfp���������˿ں�*/
		UINT16 SfpRedPort;     /*Sfp���غ����˿ں�*/

		UINT16 RpBluePort;     /*Rp���������˿ں�*/
		UINT16 RpRedPort;     /*Rp���غ����˿ں�*/

		UINT16 RsrBluePort;     /*Rsr���������˿ں�*/
		UINT16 RsrRedPort;     /*Rsr���غ����˿ں�*/
#if 0
		/* add RSSP2 */
		UINT16 Rssp2BluePort;    /*Rssp2���������˿ں�*/
		UINT16 Rssp2RedPort;     /*Rssp2���غ����˿ں�*/
#endif

	}DSU_PROTCL_IP_INFO_STRU;

	typedef struct
	{
		/*Ip��ַ�Ͷ˿ں���Ϣ���ݳ��Ⱥ�����*/
		UINT16 ipDevLen;    
		DSU_PROTCL_IP_INFO_STRU *pProtclIpInfoStru;/* ��ʼ����ʱ�����ռ� */

		/*�豸������ͨ��Э���ϵ��*/
		UINT16 CommProtclStruLen;
		DSU_PROTCL_COMM_PROTCL_STRU *pCommProtclStru;/* ��ʼ����ʱ�����ռ� */


	}DSU_PROTCL_BOOK_IP_INFO_STRU;

	typedef struct
	{
		UINT8 sfpProtclNum;

		UINT8 rsrProtclNum;

		UINT8 rsspProtclNum;

		UINT8 redunProtclNum;

		/* RSSP2���ӿ�ʼ */
		UINT8 rssp2ProtclNum;
		/* RSSP2���ӽ��� */
	}DSU_PROTCL_TYPE_STRU;
   
	typedef struct
	{
		UINT8 RedIp[DSU_LEN_IP_ADDR]; /*����IP��ַ*/
		UINT16 PortRed;   /*�����˿ں�*/
		UINT8 BlueIp[DSU_LEN_IP_ADDR]; /*����IP��ַ*/
		UINT16 PortBlue;  /*�����˿ں�*/
	} PROTCL_IP_PORT_STRU;

	typedef struct {    
		/*IP����*/
		UINT8 dstNum;
		PROTCL_IP_PORT_STRU DstIpInfo[DSU_MAX_DEST_NUM];
	} PROTCL_IP_INFO_STRU; 

	/*
	* ����������IP���ñ��ʼ�����������ڼ��������ȷ�ԣ��������ݽṹ����
	* ���������UINT8* FSName�� �ļ�������ָ���ַ
	* �� �� ֵ��1���ɹ�
	*           0��ʧ�ܣ������д���
	*/
	UINT8 dsuCommonInfoInit(UINT8* FSName);
	/*
	* �����������ú������ڸ��ݱ����Լ��Է����豸���ͻ�ȡ��ʹ�á�
	* ���������UINT16 LocalName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
	*           UINT16 DestName,  ���ܷ��豸��ʶ��Type��ǰ��ID�ں�
	* ���������UINT8* protclTye Э������
	* �� �� ֵ��1���ɹ�
	*           0��ʧ��
	*/
	UINT8 dsuProtclGetProtclType(UINT16 LocalName, UINT16 DestName,UINT8* protclTye);
     
	/*
	* �����������ú������ڸ��ݱ����豸���ͻ�ȡ���ʹ��Э�����Ϣ��
	* ���������UINT16 LocalDevName, ���ͷ��豸��ʶ��Type��ǰ��ID�ں�
	* ���������DSU_PROTCCL_TYPE_STRU* dsuProtclTypeInfo Э������
	* �� �� ֵ��1:�������� 0�����󷵻�
	*/
	UINT8 dsuGetProtclTypeInfo(UINT16 locDevName,DSU_PROTCL_TYPE_STRU* dsuProtclTypeInfo);

	/*
	* ������������ȡ�豸����
	* ���������UINT16 DevName �豸��
	* ���������
	* �� �� ֵ���豸����
	*/
	UINT8 dsuProtclGetDevType(UINT16 DevName);
	/*
* �������������ݺ�����IP��ַ�Ͷ˿ںŻ�ȡ�豸����
* ���������UINT16 port �˿ں�
*           UINT8* ipAdd  IP��ַ
* ���������devName �豸����  UINT16*
* �� �� ֵ��1:�������� 0�����󷵻�
*/

	UINT8 GetRedNetDevNameAcordIpAndPort(UINT8* ipAdd,UINT16 port,UINT16* devName);

	/*
* ��������������������IP��ַ�Ͷ˿ںŻ�ȡ�豸����
* ���������UINT16 port �˿ں�
*           UINT8* ipAdd  IP��ַ
* ���������devName �豸����  UINT16*
* �� �� ֵ��1:�������� 0�����󷵻�
*/
	UINT8 GetBlueNetDevNameAcordIpAndPort(UINT8* ipAdd,UINT16 port,UINT16* devName);

/*
* ���������������豸���ƻ�ȡ��������IP��ַ�Ͷ˿ں�
* ���������UINT16 devName �豸����
* ����������豸IP����Ϣ*
* �� �� ֵ��1:�������� 0�����󷵻�
*/
	UINT8 GetIpInfoAcordDevName(UINT8 protclType,UINT16 destDevName,PROTCL_IP_INFO_STRU* pIpInfo);

/*
* ��������������Э�����ͻ�ȡ��Ӧ�Ķ˿ں�
* ���������UINT8 protclType Э������
			UINT8 redOrBlue ����������(1��ʾ������2��ʾ����)
			DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru IP��ַ�Ͷ˿ں���Ϣ
* ���������
* �� �� ֵ���˿ں�
*/
	UINT16 GetPortAcordProtclType(UINT8 protclType, UINT8 redOrBlue, DSU_PROTCL_IP_INFO_STRU* pTempIpInfoStru);

#ifdef __cplusplus
}
#endif

#endif
