
/************************************************************************
* �ļ�����DSURelatedDeal.h
* �ļ�������CBTCϵͳ��DSUЭ����ش���ĺ�����ͷ�ļ�
             ��Ҫ���ڴ洢�ж��ܹ������Լ����彨��ϸ����Ϣ�ĺ���
* ����ʱ�䣺2008.07.29  
* �ļ�������
           1.��Ӧ���ṩ�����ݿ��е���Ϣ���뵽��̬���ñ�ṹ���У�
		   2.�жϸ����������豸��·��ϵ�Ƿ�Ϸ���Ҳ���ܷ�����
		   3.���ݸ����ı����ͶԷ��Ķ������õ������ľ�����Ϣ��
		   4.
		   5.�ṩType+ID�����豸���ıȽϺ�����
*            
************************************************************************/
#ifndef _DSURELATEDDEAL_H
#define _DSURELATEDDEAL_H

#include "sfpplatform.h"

#include "Convert.h"
#include "CommonTypes.h"


#define DSU_ROLE_SPON			0x00		            /*����*/
#define DSU_ROLE_FLW			0x01		            /*���淽*/	

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct
	{
		UINT8 RedIp[4]; /*����IP��ַ*/
		UINT16 PortRed;   /*�����˿ں�*/
		UINT8 BlueIp[4]; /*����IP��ַ*/
		UINT16 PortBlue;  /*�����˿ں�*/
	} IP_PORT_STRU;

	typedef struct {
		UINT8  Role;            /*������ɫ*/
		UINT8  NCyclesPerPack;  /*��ͨ�Ź�ϵ����������Ϸ�������*/
		UINT16 LocalCycle;      /*�����������ڳ���(���ķ������ڳ���)*/
		UINT16 LocalMaxDelay;   /*�������������ʱ��*/
		UINT8  LocalCommType;   /*������������ͨ�����ͣ�Ҳ�ǶԷ��������ݵ�ͨ�����ͣ�*/
		UINT16 DestCycle;       /* �Է����ķ�������*/
		UINT16 DestMaxDelay;    /*�������������ʱ��*/
		UINT16 AckMaxDelay;     /*���շ�ACK��������ӳ�*/
		UINT8  DestCommType;    /*�Է���������ͨ�����ͣ�Ҳ�Ǳ����������ݵ�ͨ�����ͣ�*/       

		#ifdef SFP_CKECK_DSU_IP 
		/*IP����*/
		UINT8 dstNum;
		IP_PORT_STRU DstIpInfo[2];
    #endif
	} struc_LnkBasicInfo; 


/*	
��������:�ж��豸�ܷ���
����˵��:
  [in]struc_Unify_Info * LocalInfo�������󱾵���Ϣͳһ�ṹ��
  [in]UINT32 DestDevName���Է��豸Name
  [out]UINT8* role    ������ɫ
����ֵ:�ܹ�����1,���򷵻�0
*/
UINT8 dsuIsLnkLegal(struc_Unify_Info * LocalInfo, UINT32 DestDevName,UINT8* role);

/*
 * ��������:�ж��豸�Ƿ������ݿ��еĶ���
 * ����˵��:
 *  [in]UINT32 DestDevName         �豸����+id
 *  [in]struc_Unify_Info * LocalInfo �����󱾵���Ϣͳһ�ṹ��
 * ʹ�õ���ȫ�ֱ�����
 * ����ֵ:������ַ���1�����򷵻�0
 */
UINT8 dsuGetLnkInfo(struc_Unify_Info* LocalInfo, UINT32 DestDevName, struc_LnkBasicInfo* lnk_basic_info );

#ifdef SFP_CKECK_DSU_IP 
/*
 * ��������:��DSU��ĳһ����¼��������Ϣ��䵽��·������Ϣ�ṹ��Ԫ����
 * ����˵��:
 *   [in]UINT32	DevName	�豸name
 *   [out]struc_LnkBasicInfo* lnk_Comm_info            ������·����Ҫ�Ļ�����Ϣ
 *   [in]struc_Unify_Info * LocalInfo �����󱾵���Ϣͳһ�ṹ��
 * ����ֵ:�ɹ�����1���򷵻�0
 */
UINT8 dsuFillLnkCommConfig(struc_Unify_Info * LocalInfo, UINT32 DevName,struc_LnkBasicInfo* lnk_Comm_info);
#endif
/*
 *	DSU����У�飺����DSU���ݵ���ȷ��
 *  ��ڣ��洢DSU������
 *  ���ڣ������ȷ����1���󷵻�0
 */
UINT8 dsuDataCheck(const UINT8* data);
#ifdef SFP_CKECK_DSU_IP 
/*
 * ��������:�ж��豸�Ƿ������ݿ��еĶ���
 * ����˵��:
 *   [in]UINT32 DevName         �豸����+id
 *   [in]struc_Unify_Info * LocalInfo �����󱾵���Ϣͳһ�ṹ��
 * ʹ�õ���ȫ�ֱ�����
 * ����ֵ:������ַ�������DSU���ݿ��е��ڴ�λ��ƫ�����������򷵻�0	
 */
UINT32 dsuIsDevExist(struc_Unify_Info * LocalInfo , UINT32 DevName);
#endif
/*
* ��������:��ȡDSU��ͨ�Ŷ������
* ����˵��:
*   [in] struc_Unify_Info * LocalInfo ��ǰ���еĶ���ͳһ�ṹ������
* ����ֵ:  >0DSU��ͨ�Ŷ������,
0��������
*/
UINT16 dsuGetObjNum(struc_Unify_Info * LocalInfo);

/*
* ��������:��ͬ����ʱ�õ����淽����Ϣ
* ����˵��:
*   [out]INT8 *FollowDevInfo:��¼���淽��Ϣ�������ͷָ�� (����Ӧ�ö���һ��1*9������)
*       �ڲ���ʽΪ
*       Byte0         Byte1          Byte2          Byte3   Byte4         Byte5        Byte6......
*       ���淽����    ���淽1Type    ���淽1ID      0       0             ���淽2Type  ���淽2ID  ...
*   [in] struc_Unify_Info * LocalInfo ��ǰ���еĶ���ͳһ�ṹ������
* ����ֵ:�����ȷ����1���򷵻�0	
*/
UINT8 dsuSameTypeLnk(struc_Unify_Info * LocalInfo, UINT8 *FollowDevInfo);

/*
* ��������:���ݴ���������ݵ��ļ����Ի������ݵĵ�ַ����ȡЭ����������ݵ��ڴ��С������ٵ��ڴ��ַͨ��pDataBase���ء�
* ����˵��:
UINT8* fileName  �ļ������ַ
UINT8** pDataBase  ���ݿ��ַ
����ֵ:�����ȷ����1���򷵻�0	
*/
UINT8 dsuSfpInit(UINT8* fileName,UINT8** pDataBase);

#ifdef __cplusplus
}
#endif

#endif


