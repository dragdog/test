
/************************************************************************
* �ļ�����DSURelatedDeal.c
* �ļ�������CBTCϵͳ��DSUЭ����ش���ĺ���
��Ҫ���ڴ洢�ж��ܹ������Լ����彨��ϸ����Ϣ�ĺ���
* ����ʱ�䣺2009.01.17  
* �ļ�������
1.����Ӧ���ṩ�����ݿ��е���Ϣ
2.�жϸ����������豸��·��ϵ�Ƿ�Ϸ���Ҳ���ܷ�����
3.���ݸ����ı����ͶԷ��Ķ������õ������ľ�����Ϣ��
4.ZC������ѯ��������Ҫ����һ���豸�����Һ���ͬ���͵���ͨ�Ź�ϵ�������豸Ϊ���𷽵���·��
5.�ṩType+ID�����豸���ıȽϺ�����
*          
************************************************************************/
#include "CommonMemory.h"
#include "dfsDataRead.h"
#include "dsuRelatedDeal.h"			/*���ļ���.h�ļ�*/
#include "dquDataTypeDefine.h"
#include "string.h"

#define DSU_OBJ_INFO_LENGTH 14	   /*DSUͨ�Ŷ�����Ϣ���ȣ���λ���ֽڣ�*/
#define DSU_DIFF_RELATION_LENGTH 15	   /*DSU��ͬ�豸����ͨ�Ź�ϵ��¼���ȣ���λ���ֽڣ�*/
#define DSU_SAME_RELATION_LENGTH 17	   /*DSU��ͬ�豸����ͨ�Ź�ϵ��¼���ȣ���λ���ֽڣ�*/

/*�ṹ���ڲ����������ܺ�*/
#define DSU_IP_INFO_STRU_SIZE 22

#define DSU_OBJ_TYPE_SUBINDEX 0	   /*DSUͨ�Ŷ�������������*/
#define DSU_OBJ_ID_SUBINDEX 1	   /*DSUͨ�Ŷ���ID������*/
#define DSU_OBJ_BLUEIP_SUBINDEX 2	   /*DSUͨ�Ŷ�������IP������*/
#define DSU_OBJ_BLUEPORT_SUBINDEX 6	   /*DSUͨ�Ŷ��������˿�������*/
#define DSU_OBJ_REDIP_SUBINDEX 8	   /*DSUͨ�Ŷ������IP������*/
#define DSU_OBJ_REDPORT_SUBINDEX 12	   /*DSUͨ�Ŷ�������˿�������*/


#define DSU_DIFF_SPON_TYPE_SUBINDEX 0	   /*DSU��ͬ�����豸ͨ�Ź�ϵ��������������*/
#define DSU_DIFF_SPON_COMTYPE_SUBINDEX 1          /*DSU��ͬ�����豸ͨ�Ź�ϵ����ͨ������������*/
#define DSU_DIFF_SPON_CYCLE_SUBINDEX  2                                /*DSU��ͬ�����豸ͨ�Ź�ϵ��������������*/
#define DSU_DIFF_SPON_MSG_MAXDELAY_SUBINDEX 4              /*DSU��ͬ�����豸ͨ�Ź�ϵ�����������ʱ*/
#define DSU_DIFF_SPON_ACK_MAXDELAY_SUBINDEX 6              /*DSU��ͬ�����豸ͨ�Ź�ϵ���𷽽���ACK�����ʱ*/
#define DSU_DIFF_FOLL_TYPE_SUBINDEX 8	   /*DSU��ͬ�����豸ͨ�Ź�ϵ����淽����������*/
#define DSU_DIFF_FOLL_COMTYPE_SUBINDEX 9         /*DSU��ͬ�����豸ͨ�Ź�ϵ����淽ͨ������������*/
#define DSU_DIFF_FOLL_CYCLE_SUBINDEX  10                                /*DSU��ͬ�����豸ͨ�Ź�ϵ����淽����������*/
#define DSU_DIFF_FOLL_MSG_MAXDELAY_SUBINDEX 12              /*DSU��ͬ�����豸ͨ�Ź�ϵ����淽�������ʱ*/
#define DSU_DIFF_SENDMSG_MINDELAY_SUBINDEX 14              /*DSU��ͨ�Ź�ϵ�гɹ�����һ������������С������*/

#define DSU_SAME_SPON_TYPE_SUBINDEX 0	   /*DSU��ͬ�����豸ͨ�Ź�ϵ��������������*/
#define DSU_SAME_SPON_ID_SUBINDEX 1	   /*DSU��ͬ�����豸ͨ�Ź�ϵ����ID������*/
#define DSU_SAME_SPON_COMTYPE_SUBINDEX 2          /*DSU��ͬ�����豸ͨ�Ź�ϵ����ͨ������������*/
#define DSU_SAME_SPON_CYCLE_SUBINDEX  3                                /*DSU��ͬ�����豸ͨ�Ź�ϵ��������������*/
#define DSU_SAME_SPON_MSG_MAXDELAY_SUBINDEX 5              /*DSU��ͬ�����豸ͨ�Ź�ϵ�����������ʱ*/
#define DSU_SAME_SPON_ACK_MAXDELAY_SUBINDEX 7              /*DSU��ͬ�����豸ͨ�Ź�ϵ���𷽽���ACK�����ʱ*/
#define DSU_SAME_FOLL_TYPE_SUBINDEX 9	   /*DSU��ͬ�����豸ͨ�Ź�ϵ����淽����������*/
#define DSU_SAME_FOLL_ID_SUBINDEX 10	               /*DSU��ͬ�����豸ͨ�Ź�ϵ����淽ID������*/
#define DSU_SAME_FOLL_COMTYPE_SUBINDEX 11         /*DSU��ͬ�����豸ͨ�Ź�ϵ����淽ͨ������������*/
#define DSU_SAME_FOLL_CYCLE_SUBINDEX  12                    /*DSU��ͬ�����豸ͨ�Ź�ϵ����淽����������*/
#define DSU_SAME_FOLL_MSG_MAXDELAY_SUBINDEX 14              /*DSU��ͬ�����豸ͨ�Ź�ϵ����淽�������ʱ*/
#define DSU_SAME_SENDMSG_MINDELAY_SUBINDEX 16              /*DSU��ͨ�Ź�ϵ�гɹ�����һ������������С������*/


#define DSU_DATA_PROTCL_DIFF_RELATION     0x04   /*DSU��ͬ�豸����ͨ�Ź�ϵtableId*/
#define DSU_DATA_PROTCL_SAME_RELATION     0x05   /*DSU��ͬ�豸����ͨ�Ź�ϵtableId*/
/***************************************************************************************************************
�������������豸TYPE���豸IDת����long�͵��豸��
����˵����
[in]UINT8 devType �豸����
[in]UINT8 devID  �豸ID
����ֵ��unsigned INT32 �͵��豸��
�������������豸TYPE���豸IDת����long�͵��豸��
***************************************************************************************************************/
static UINT32 dsuTypeIdToName(UINT8 devType,UINT8 devID)
{
	UINT32 Templong;
	Templong = 0;
	Templong = devType;
	Templong = (Templong*0x100) + devID;
	Templong = Templong*0x10000  ;
	return Templong;
}

/*	
��������:�ж��豸�ܷ��������Է��豸��DSUͨ�Ŷ����У���Ҫȥ�������ҶԷ��豸����ͬ�豸��ϵ���ͬ�豸��ϵ���� add by lcj)
����˵��:
[in]struc_Unify_Info * LocalInfo�������󱾵���Ϣͳһ�ṹ��
[in]UINT32 DestDevName���Է��豸Name
[out]UINT8* role    ������ɫ
����ֵ:�ܹ�����1,���򷵻�0
*/
UINT8 dsuIsLnkLegal(struc_Unify_Info * LocalInfo, UINT32 DestDevName,UINT8* role)
{
	register UINT32 lTemp;   /*��ʱlong�ͱ���*/
	register UINT8 * data;  /*���ݿ��ַ��ʱ����*/

	UINT32 i; 
	UINT8 ret = 0;

	/*UINT16 nObjNumber;*/   /*ͨ�Ŷ������*/
	UINT16 nDiffRelation;   /*��ͬͨ������ͨ�Ź�ϵ����*/
	UINT16 nSameRelation;   /*��ͬͨ������ͨ�Ź�ϵ����*/
	UINT32 lSameRelationIndex;   /*��ͬͨ������ͨ�Ź�ϵ����ʼ����*/
	UINT32 lDiffRelationIndex;   /*��ͬͨ������ͨ�Ź�ϵ����ʼ����*/
	UINT32 lDevName;   /*ͨ�Ŷ�������ʱ����*/
	UINT32 lDevName2;   /*ͨ�Ŷ�������ʱ����*/
	UINT32 lLocalDevName;   /*����������ʱ����*/
	UINT32 lDestDevType;   /*�Է������豸������ʱ����*/

	if(dsuDataCheck((UINT8 *) LocalInfo->DataBase) == 1)
	{
		data = (UINT8 *) LocalInfo->DataBase;

		lTemp=0;
#ifdef SFP_CKECK_DSU_IP 
		/*�õ�DSU��ͨ�Ŷ������*/
		nObjNumber = ShortFromChar(&data[lTemp]);
		lTemp+=2;

		/*�õ�DSU�в�ͬ����ͨ�Ź�ϵ��������*/
		lTemp +=nObjNumber* DSU_OBJ_INFO_LENGTH;
#endif
		/*�õ�DSU�в�ͬ����ͨ�Ź�ϵ����*/
		nDiffRelation = ShortFromChar(&data[lTemp]);
		lTemp+=2;
		lDiffRelationIndex = lTemp;

		/*�õ�DSU����ͬ����ͨ�Ź�ϵ��������*/
		lTemp =  lTemp+nDiffRelation * DSU_DIFF_RELATION_LENGTH;

		/*�õ�DSU����ͬ����ͨ�Ź�ϵ����*/
		nSameRelation = ShortFromChar(&data[lTemp]);
		lTemp+=2;
		lSameRelationIndex = lTemp;
#ifdef SFP_CKECK_DSU_IP 
		/*�жϱ����豸�Ϸ���*/
		lLocalDevName = dsuTypeIdToName(LocalInfo->LocalType,LocalInfo->LocalID);
		lTemp=dsuIsDevExist(LocalInfo,lLocalDevName);
		if (lTemp == 0)
		{
			/*�����豸���Ϸ�*/
			ret = 0;
			return ret;
	    }


		lTemp=dsuIsDevExist(LocalInfo,DestDevName);/* ����ͨ�Ŷ�����ʼ��ַ */

		if(lTemp!=0)
	
		{
		#endif
			/*Ŀ���豸�Ϸ�*/

			/*����ͬ����ͨ�Ŷ����ϵ��*/
			if(LocalInfo->LocalType  == ((DestDevName>>24) & 0xFF))
			{
				lTemp = lSameRelationIndex;
				lLocalDevName = ((UINT32) LocalInfo->LocalType << 24) + ((UINT32) LocalInfo->LocalID<<16);
				for(i = 0; i < nSameRelation; i++)
				{
					/*�÷����豸��*/
					lDevName =  ((UINT32) data[lTemp]<<24) + ((UINT32) data[lTemp + 1] << 16);
					/*�ø��淽�豸��*/
					lDevName2 =  ((UINT32) data[lTemp + DSU_SAME_FOLL_TYPE_SUBINDEX] << 24) + 
						((UINT32) data[lTemp + DSU_SAME_FOLL_ID_SUBINDEX] << 16);
					/*����Ϊ���𷽣��Է�Ϊ���淽*/
					if((lLocalDevName  == lDevName )&& (DestDevName == lDevName2))
					{
						(*role) = DSU_ROLE_SPON;
						ret = 1;
						return ret;
					}
					/*����Ϊ���淽���Է�Ϊ����*/
					else if((lLocalDevName  == lDevName2) && (DestDevName == lDevName))
					{
						(*role) = DSU_ROLE_FLW;
						ret = 1;
						return ret;
					}
					else
					{
						lTemp = lTemp + DSU_SAME_RELATION_LENGTH;
					}

				}
			}
			else  /*�鲻ͬ����ͨ�Ŷ����ϵ��*/ 
			{
				lTemp = lDiffRelationIndex;
				lLocalDevName =  LocalInfo->LocalType;
				lDestDevType = (DestDevName>>24) & 0xff;
				for(i = 0; i < nDiffRelation; i++)
				{
					/*�÷����豸�������������豸������*/
					lDevName =  data[lTemp];
					/*�ø��淽�豸�������������豸������*/
					lDevName2 =  data[lTemp +   DSU_DIFF_FOLL_TYPE_SUBINDEX];

					if((lLocalDevName  == lDevName) && (lDestDevType == lDevName2))
					{
						(*role) = DSU_ROLE_SPON;
						ret = 1;
						return ret;
					}
					else if((lLocalDevName  == lDevName2) && (lDestDevType == lDevName))
					{
						(*role) = DSU_ROLE_FLW;
						ret = 1;
						return ret;
					}
					else
					{
						lTemp = lTemp + DSU_DIFF_RELATION_LENGTH;
					}
				}
			}
			ret = 0;
			return ret;
#ifdef SFP_CKECK_DSU_IP 
		}
		else
		{
			ret = 0;
			return ret;
		}
#endif
	}
	else
	{
		ret = 0;
		return ret;
	}
}


/*
* ��������:��ȡlnk��Ϣ
* ����˵��:
*  [in]UINT32 DestDevName         �豸����+id
*  [in]struc_Unify_Info * LocalInfo �����󱾵���Ϣͳһ�ṹ��
* ʹ�õ���ȫ�ֱ�����
* ����ֵ:������ַ���1�����򷵻�0
*/
UINT8 dsuGetLnkInfo(struc_Unify_Info* LocalInfo, UINT32 DestDevName, struc_LnkBasicInfo* lnk_basic_info )
{
	register UINT32 lTemp;   /*��ʱlong�ͱ���*/
	register UINT8 * data;  /*���ݿ��ַ��ʱ����*/

	UINT16 i;
	/*UINT16 nObjNumber;*/   /*ͨ�Ŷ������*/
	UINT16 nDiffRelation;   /*��ͬͨ������ͨ�Ź�ϵ����*/
	UINT16 nSameRelation;   /*��ͬͨ������ͨ�Ź�ϵ����*/
	UINT32 lSameRelationIndex;   /*��ͬͨ������ͨ�Ź�ϵ����ʼ����*/
	UINT32 lDiffRelationIndex;   /*��ͬͨ������ͨ�Ź�ϵ����ʼ����*/
	UINT32 lDestDevType;   /*�Է������豸������ʱ����*/
	UINT32 lLocalDevName;   /*����������ʱ����*/
	UINT32 lDevName;
	UINT32 lDevName2;
	UINT8 ret;
#ifdef SFP_CKECK_DSU_IP 
	/*�жϱ����豸�Ϸ���*/
	lLocalDevName = dsuTypeIdToName(LocalInfo->LocalType,LocalInfo->LocalID);

	lTemp=dsuIsDevExist(LocalInfo,lLocalDevName);
	if (lTemp == 0)
	{
		/*�����豸���Ϸ�*/
		ret = 0;
		return ret;
	}
#endif
	lLocalDevName = dsuTypeIdToName(LocalInfo->LocalType,LocalInfo->LocalID);
#ifdef SFP_CKECK_DSU_IP 
	/*�ж��豸�Ƿ���DSU���ݱ��д��ڣ�������ڣ��򷵻�����豸��Ϣ�ĵ�һ���ֽ���DSU�����е�λ������*/
	if(dsuIsDevExist(LocalInfo , DestDevName) != 0)
	{
#endif
		data = (UINT8 *) LocalInfo->DataBase;
		lTemp=0;
#ifdef SFP_CKECK_DSU_IP 
		/*�õ�DSU��ͨ�Ŷ������*/
		nObjNumber = ShortFromChar(&data[lTemp]);
		lTemp+=2;

		/*�õ�DSU�в�ͬ����ͨ�Ź�ϵ��������*/
		lTemp = lTemp + nObjNumber* DSU_OBJ_INFO_LENGTH;
#endif
		/*�õ�DSU�в�ͬ����ͨ�Ź�ϵ����*/
		nDiffRelation = ShortFromChar(&data[lTemp]);
		lTemp+=2;
		lDiffRelationIndex = lTemp;

		/*�õ�DSU����ͬ����ͨ�Ź�ϵ��������*/
		lTemp = lTemp + nDiffRelation * DSU_DIFF_RELATION_LENGTH;

		/*�õ�DSU����ͬ����ͨ�Ź�ϵ����*/
		nSameRelation = ShortFromChar(&data[lTemp]);
		lTemp+=2;
		lSameRelationIndex = lTemp;

		if(LocalInfo->LocalType  == ((DestDevName>>24) & 0xFF))/* ��ͬ�豸���� */
		{
			lTemp = lSameRelationIndex;
			lLocalDevName = ((UINT32) LocalInfo->LocalType << 24) + ((UINT32) LocalInfo->LocalID<<16);
			for(i = 0; i < nSameRelation; i++)
			{
				/*�÷����豸��*/
				lDevName =  ((UINT32) data[lTemp]<<24) + ((UINT32) data[lTemp + 1] << 16);
				/*�ø��淽�豸��*/
				lDevName2 =  ((UINT32) data[lTemp + DSU_SAME_FOLL_TYPE_SUBINDEX]<<24) + 
					((UINT32) data[lTemp + DSU_SAME_FOLL_ID_SUBINDEX] << 16);

				if((lLocalDevName  == lDevName )&& (DestDevName == lDevName2))
				{
					lnk_basic_info->Role = DSU_ROLE_SPON;	/*������ɫ*/
					/*��ͨ�Ź�ϵ����������Ϸ�������*/
					lnk_basic_info->NCyclesPerPack = data[lTemp + DSU_SAME_SENDMSG_MINDELAY_SUBINDEX];
					/* ���𷽷�������ͨ�����ͣ�Ҳ�Ǹ��淽�������ݵ�ͨ�����ͣ�*/
					lnk_basic_info->LocalCommType = data[lTemp + DSU_SAME_SPON_COMTYPE_SUBINDEX];
					/*�������ķ�������*/
					lnk_basic_info->LocalCycle = ShortFromChar(&data[lTemp + DSU_SAME_SPON_CYCLE_SUBINDEX]);
					/*�������������ʱ*/
					lnk_basic_info->LocalMaxDelay = ShortFromChar(&data[lTemp + DSU_SAME_SPON_MSG_MAXDELAY_SUBINDEX]);
					/*������Ҫ֪��Ack�����ʱ*/
					lnk_basic_info->AckMaxDelay = ShortFromChar(&data[lTemp + DSU_SAME_SPON_ACK_MAXDELAY_SUBINDEX]);
					/*�Է����ķ�������*/
					lnk_basic_info->DestCycle = ShortFromChar(&data[lTemp + DSU_SAME_FOLL_CYCLE_SUBINDEX]);
					/*�Է����������ʱ*/
					lnk_basic_info->DestMaxDelay = ShortFromChar(&data[lTemp + DSU_SAME_FOLL_MSG_MAXDELAY_SUBINDEX]);
					/* ���淽��������ͨ�����ͣ�Ҳ�Ƿ��𷽽������ݵ�ͨ�����ͣ�*/
					lnk_basic_info->DestCommType = data[lTemp + DSU_SAME_FOLL_COMTYPE_SUBINDEX];
#ifdef SFP_CKECK_DSU_IP 
					/*��д����������Ϣ*/
					dsuFillLnkCommConfig(LocalInfo, DestDevName,lnk_basic_info);
#endif
					ret = 1;
					return ret;

				}
				else if((lLocalDevName  == lDevName2) && (DestDevName == lDevName))
				{
					lnk_basic_info->Role = DSU_ROLE_FLW;	/*������ɫ*/
					/*��ͨ�Ź�ϵ����������Ϸ�������*/
					lnk_basic_info->NCyclesPerPack = data[lTemp + DSU_SAME_SENDMSG_MINDELAY_SUBINDEX];
					/* ���𷽷�������ͨ�����ͣ�Ҳ�Ǹ��淽�������ݵ�ͨ�����ͣ�*/
					lnk_basic_info->LocalCommType = data[lTemp + DSU_SAME_FOLL_COMTYPE_SUBINDEX];
					/*�������ķ�������*/
					lnk_basic_info->LocalCycle = ShortFromChar(&data[lTemp + DSU_SAME_FOLL_CYCLE_SUBINDEX]);
					/*�������������ʱ*/
					lnk_basic_info->LocalMaxDelay = ShortFromChar(&data[lTemp + DSU_SAME_FOLL_MSG_MAXDELAY_SUBINDEX]);
					/*�Է����ķ�������*/
					lnk_basic_info->DestCycle = ShortFromChar(&data[lTemp + DSU_SAME_SPON_CYCLE_SUBINDEX]);
					/*�Է����������ʱ*/
					lnk_basic_info->DestMaxDelay = ShortFromChar(&data[lTemp + DSU_SAME_SPON_MSG_MAXDELAY_SUBINDEX]);
					/* ���淽��������ͨ�����ͣ�Ҳ�Ƿ��𷽽������ݵ�ͨ�����ͣ�*/
					lnk_basic_info->DestCommType = data[lTemp + DSU_SAME_SPON_COMTYPE_SUBINDEX];
					
					#ifdef SFP_CKECK_DSU_IP 
					/*��д����������Ϣ*/
					dsuFillLnkCommConfig(LocalInfo, DestDevName,lnk_basic_info);
                    #endif
					ret = 1;
					return ret;
				}
				else
				{
					lTemp = lTemp + DSU_SAME_RELATION_LENGTH;
				} 
			}
		}
		else
		{
			lTemp = lDiffRelationIndex;
			lLocalDevName =  LocalInfo->LocalType;
			lDestDevType = ((DestDevName>>24) & 0xff);
			for(i = 0; i < nDiffRelation; i++)
			{
				/*�÷����豸������*/
				lDevName =  data[lTemp];
				/*�ø��淽�豸������*/
				lDevName2 =  data[lTemp +   DSU_DIFF_FOLL_TYPE_SUBINDEX];
				if((lLocalDevName  == lDevName) && (lDestDevType == lDevName2))
				{
					lnk_basic_info->Role = DSU_ROLE_SPON;	/*������ɫ*/
					lnk_basic_info->NCyclesPerPack = data[lTemp + DSU_DIFF_SENDMSG_MINDELAY_SUBINDEX];/*��ͨ�Ź�ϵ����������Ϸ�������*/
					lnk_basic_info->LocalCommType = data[lTemp + DSU_DIFF_SPON_COMTYPE_SUBINDEX];/* ���𷽷�������ͨ�����ͣ�Ҳ�Ǹ��淽�������ݵ�ͨ�����ͣ�*/
					lnk_basic_info->LocalCycle = ShortFromChar(&data[lTemp + DSU_DIFF_SPON_CYCLE_SUBINDEX]);    /*�������ķ�������*/
					lnk_basic_info->LocalMaxDelay = ShortFromChar(&data[lTemp + DSU_DIFF_SPON_MSG_MAXDELAY_SUBINDEX]);/*�������������ʱ*/
					lnk_basic_info->AckMaxDelay = ShortFromChar(&data[lTemp + DSU_DIFF_SPON_ACK_MAXDELAY_SUBINDEX]);/*������Ҫ֪��Ack�����ʱ*/
					lnk_basic_info->DestCycle = ShortFromChar(&data[lTemp + DSU_DIFF_FOLL_CYCLE_SUBINDEX]);     /*�Է����ķ�������*/
					lnk_basic_info->DestMaxDelay = ShortFromChar(&data[lTemp + DSU_DIFF_FOLL_MSG_MAXDELAY_SUBINDEX]);	/*�Է����������ʱ*/
					lnk_basic_info->DestCommType = data[lTemp + DSU_DIFF_FOLL_COMTYPE_SUBINDEX];/* ���淽��������ͨ�����ͣ�Ҳ�Ƿ��𷽽������ݵ�ͨ�����ͣ�*/
					#ifdef SFP_CKECK_DSU_IP 
					/*��д����������Ϣ*/
					dsuFillLnkCommConfig(LocalInfo, DestDevName,lnk_basic_info) ;
					#endif
					ret = 1;
					return ret;
				}
				else if((lLocalDevName  == lDevName2) && (lDestDevType == lDevName))
				{
					lnk_basic_info->Role = DSU_ROLE_FLW;	/*������ɫ*/
					lnk_basic_info->NCyclesPerPack = data[lTemp + DSU_DIFF_SENDMSG_MINDELAY_SUBINDEX];/*��ͨ�Ź�ϵ����������Ϸ�������*/
					lnk_basic_info->LocalCommType = data[lTemp + DSU_DIFF_FOLL_COMTYPE_SUBINDEX];/* ���𷽷�������ͨ�����ͣ�Ҳ�Ǹ��淽�������ݵ�ͨ�����ͣ�*/
					lnk_basic_info->LocalCycle = ShortFromChar(&data[lTemp + DSU_DIFF_FOLL_CYCLE_SUBINDEX]);    /*�������ķ�������*/
					lnk_basic_info->LocalMaxDelay = ShortFromChar(&data[lTemp + DSU_DIFF_FOLL_MSG_MAXDELAY_SUBINDEX]);/*�������������ʱ*/
					lnk_basic_info->DestCycle = ShortFromChar(&data[lTemp + DSU_DIFF_SPON_CYCLE_SUBINDEX]);     /*�Է����ķ�������*/
					lnk_basic_info->DestMaxDelay = ShortFromChar(&data[lTemp + DSU_DIFF_SPON_MSG_MAXDELAY_SUBINDEX]);	/*�Է����������ʱ*/
					lnk_basic_info->DestCommType = data[lTemp + DSU_DIFF_SPON_COMTYPE_SUBINDEX];/* ���淽��������ͨ�����ͣ�Ҳ�Ƿ��𷽽������ݵ�ͨ�����ͣ�*/
					#ifdef SFP_CKECK_DSU_IP 
					/*��д����������Ϣ*/
					dsuFillLnkCommConfig(LocalInfo, DestDevName,lnk_basic_info);
					#endif
					ret = 1;
					return ret;
				}
				else
				{
					lTemp = lTemp + DSU_DIFF_RELATION_LENGTH;
				}
			}
		}
		ret = 0;
		return ret;   /*����죬���еı�û�鵽���ʱ���*/
		#ifdef SFP_CKECK_DSU_IP 
	}
	else
	{
		ret = 0;
		return ret;   /*���ݿ������д�*/
	}
    #endif
}
#ifdef SFP_CKECK_DSU_IP 
/*
* ��������:��DSU��ĳһ����¼��������Ϣ��䵽��·������Ϣ�ṹ��Ԫ����
* ����˵��:
*   [in]UINT32	DevName	�豸name
*   [out]struc_LnkBasicInfo* lnk_Comm_info            ������·����Ҫ�Ļ�����Ϣ
*   [in]struc_Unify_Info * LocalInfo �����󱾵���Ϣͳһ�ṹ��
* ����ֵ:�ɹ�����1���򷵻�0
*/
UINT8 dsuFillLnkCommConfig(struc_Unify_Info * LocalInfo, UINT32 DevName,struc_LnkBasicInfo* lnk_Comm_info)
{
	/*****************2012.4.1����΢����Ŀ�޸�***********************************/
	register UINT32 lTemp;   /*��ʱlong�ͱ���*/
	register UINT8 * data;  /*���ݿ��ַ��ʱ����*/

	UINT8 ret;
	UINT32 i;
	UINT16 nObjNumber;   /*ͨ�Ŷ������*/
	UINT32 lDevName;   /*ͨ�Ŷ�������ʱ����*/

	/*Ĭ�����ҵ�ip��ϢΪ0��*/
	lnk_Comm_info->dstNum=0;

	if(dsuDataCheck((UINT8 *) LocalInfo->DataBase) == 1)
	{
		data = (UINT8 *) LocalInfo->DataBase;
		lTemp=0;

		/*�õ�DSU��ͨ�Ŷ������*/
		nObjNumber = ShortFromChar(&data[lTemp]);
		lTemp+=2;

		for(i = 0; i < nObjNumber; i++)
		{
			lDevName =  ((UINT32) data[lTemp]<<24) + ((UINT32) data[lTemp + 1] << 16);
			if(lDevName == DevName)
			{
				/*�����豸name���ڴ������豸name*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].RedIp[0] = data[lTemp + DSU_OBJ_REDIP_SUBINDEX];/*����IP��ַ*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].RedIp[1] = data[lTemp + DSU_OBJ_REDIP_SUBINDEX + 1];/*����IP��ַ*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].RedIp[2] = data[lTemp + DSU_OBJ_REDIP_SUBINDEX + 2];/*����IP��ַ*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].RedIp[3] = data[lTemp + DSU_OBJ_REDIP_SUBINDEX + 3];/*����IP��ַ*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].PortRed = ShortFromChar(&data[lTemp + DSU_OBJ_REDPORT_SUBINDEX]);/*�����˿�*/


				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].BlueIp[0] = data[lTemp + DSU_OBJ_BLUEIP_SUBINDEX];/*����IP��ַ*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].BlueIp[1] = data[lTemp + DSU_OBJ_BLUEIP_SUBINDEX + 1];/*����IP��ַ*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].BlueIp[2] = data[lTemp + DSU_OBJ_BLUEIP_SUBINDEX + 2];/*����IP��ַ*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].BlueIp[3] = data[lTemp + DSU_OBJ_BLUEIP_SUBINDEX + 3];/*����IP��ַ*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].PortBlue = ShortFromChar(&data[lTemp + DSU_OBJ_BLUEPORT_SUBINDEX]);/*�����˿�*/

				lnk_Comm_info->dstNum++;
				
			}
			else
			{
				/*�����豸name�����ڴ������豸name*/


			}

			lTemp = lTemp + DSU_OBJ_INFO_LENGTH;
		}


	}
	else
	{
		/*���ݲ��Ϸ���ֱ�ӷ���ʧ��*/

	}

	/*�ж��Ƿ��ҵ�ip��ַ����*/
	if (lnk_Comm_info->dstNum==0)
	{
		ret = 0;

	}
	else
	{
		ret = 1;
	}

	return ret;

}
#endif
/*
*DSU����У�飺����DSU���ݵ���ȷ��
* ��ڣ��洢DSU������
* ���ڣ������ȷ����1���󷵻�0
*/
UINT8 dsuDataCheck(const UINT8* data)
{
	UINT8 ret=1;
	UINT8 tempData;

	/*��һ�����ݣ����Թرվ��档*/
	tempData=data[0];
	return ret;
}
#ifdef SFP_CKECK_DSU_IP 
/*
* ��������:�ж��豸�Ƿ������ݿ��еĶ����ж��豸�Ƿ���DSU��ͨ�Ŷ����У�
* ����˵��:
*   [in]UINT32 DevName         �豸����+id
*   [in]struc_Unify_Info * LocalInfo �����󱾵���Ϣͳһ�ṹ��
* ʹ�õ���ȫ�ֱ�����
* ����ֵ:������ַ�������DSU���ݿ��е��ڴ�λ��ƫ�����������򷵻�0	
*/
UINT32 dsuIsDevExist(struc_Unify_Info * LocalInfo , UINT32 DevName)
{
	register UINT32 lTemp;   /*��ʱlong�ͱ���*/
	register UINT8 * data;  /*���ݿ��ַ��ʱ����*/

	UINT32 ret;
	UINT32 i;
	UINT16 nObjNumber;   /*ͨ�Ŷ������*/
	UINT32 lDevName;   /*ͨ�Ŷ�������ʱ����*/

	if(dsuDataCheck((UINT8 *) LocalInfo->DataBase) == 1)
	{
		data = (UINT8 *) LocalInfo->DataBase;
		lTemp=0;

		/*�õ�DSU��ͨ�Ŷ������*/
		nObjNumber = ShortFromChar(&data[lTemp]);
		lTemp+=2;

		for(i = 0; i < nObjNumber; i++)
		{
			lDevName =  ((UINT32) data[lTemp]<<24) + ((UINT32) data[lTemp + 1] << 16);
			if(lDevName != DevName)
			{
				lTemp = lTemp + DSU_OBJ_INFO_LENGTH;
			}
			else
			{
				return lTemp;
			}
		}
		ret = 0;
		return ret;

	}
	else
	{
		ret = 0;
		return ret;
	}
}
#endif
/*
* ��������:��ȡDSU��ͨ�Ŷ������
* ����˵��:
*   [in] struc_Unify_Info * LocalInfo ��ǰ���еĶ���ͳһ�ṹ������
* ����ֵ:  >0DSU��ͨ�Ŷ������,
           0��������
*/
UINT16 dsuGetObjNum(struc_Unify_Info * LocalInfo)
{
#ifdef SFP_CKECK_DSU_IP 
	UINT8 *data;  /*���ݿ��ַ��ʱ����*/
	UINT16 nObjNumber;     /*DSU��ͨ�Ŷ������*/

	data = (UINT8 *) LocalInfo->DataBase;
	/*�õ�DSU��ͨ�Ŷ������*/

	if(dsuDataCheck(data) == 1)
	{
		nObjNumber = ShortFromChar(&data[0]);
	}
	else
	{
		nObjNumber=0;
	}
#else
	UINT16 nObjNumber;     /*DSU��ͨ�Ŷ������*/
    nObjNumber = LocalInfo->MaxNumLink;/* ��Ҫ����һ�����ֵ */
#endif
	return nObjNumber;
	
}

/*
* ��������:���ݴ���������ݵ��ļ����Ի������ݵĵ�ַ����ȡЭ����������ݵ��ڴ��С������ٵ��ڴ��ַͨ��pDataBase���ء�
* ����˵��:
UINT8* fileName  �ļ������ַ
UINT8** pDataBase  ���ݿ��ַ
����ֵ:�����ȷ����1���򷵻�0	
*/
UINT8 dsuSfpInit(UINT8* fileName,UINT8** pDataBase)
{
	UINT8 chReturnValue =0 ; /*����ֵ*/
	UINT8 TempRet;
	UINT8* pData=NULL; /*���ص�����*/
	UINT32 dataOff=0;  /*����ƫ����*/
	UINT32 dataOffIp=0;  /*ip������ƫ����*/
	UINT32 dataLineNum=0;  /*ip�����ݸ���*/

#ifdef SFP_CKECK_DSU_IP 
	UINT8* pData1=NULL;  /*DSUͨ�Ŷ�������*/
	UINT32 data1Len=0;  /*DSUͨ�Ŷ������ݳ���*/
#endif
	UINT8* pData2=NULL;  /*DSU��ͬ�豸����ͨ�Ź�ϵ����*/ 
	UINT32 data2Len=0;  /*DSU��ͬ�豸����ͨ�Ź�ϵ���ݳ��� */
	UINT8* pData3=NULL;  /*DSU��ͬ�豸����ͨ�Ź�ϵ����*/
	UINT32 data3Len=0;  /*DSU��ͬ�豸����ͨ�Ź�ϵ���ݳ���*/

	/*UINT32 i;*/
#ifdef SFP_CKECK_DSU_IP 
	/*��ȡ����*/
	TempRet=dquGetConfigData((CHAR *)fileName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData1 , &data1Len);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*����ִ��*/
	}
#endif
	/*��ȡ����*/
	TempRet=dquGetConfigData((CHAR *)fileName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_DIFF_RELATION, &pData2 , &data2Len);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*����ִ��*/
	}

	/*��ȡ����*/
	TempRet=dquGetConfigData((CHAR *)fileName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_SAME_RELATION, &pData3 , &data3Len);
	if (TempRet!=1)
	{
		/*���ݶ�ȡʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*����ִ��*/
	}


	/*��̬�������ݿռ䣬�����100�ǿ������㹻������*/
#ifdef SFP_CKECK_DSU_IP 
	pData=(UINT8*)malloc(data1Len+data2Len+data3Len+100);
#else
    pData=(UINT8*)malloc(data2Len+data3Len+100);
#endif
	if (pData==NULL)
	{
		/*����ռ�ʧ��*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
#ifdef SFP_CKECK_DSU_IP 
		/*�������ִ��*/
		CommonMemSet(pData,(data1Len+data2Len+data3Len+100),0,(data1Len+data2Len+data3Len+100));
#else
        CommonMemSet(pData,(data2Len+data3Len+100),0,(data2Len+data3Len+100));
#endif
	}

	/*������������*/
	dataOff=0;
#ifdef SFP_CKECK_DSU_IP 
	/*ip�����ݿ���*/

	/*����ip���ݵ�����*/
	dataLineNum=data1Len/DSU_IP_INFO_STRU_SIZE;

	if (dataLineNum>65535)
	{
		/*�ж�����Խ��*/
		chReturnValue=0;
		return chReturnValue;
	}

	ShortToChar((UINT16)dataLineNum,pData+dataOff);
	dataOff+=2;

	/*��ʼ��ipƫ��������*/
	dataOffIp=0;
	/*ѭ����������*/
	for (i=0;i<dataLineNum;i++)
	{
		/*��������*/
		memcpy(pData+dataOff,pData1+dataOffIp,2);/* ����+ID*/
		dataOff+=2;

		memcpy(pData+dataOff,pData1+dataOffIp+2,4);/* ����IP*/
		dataOff+=4;

		memcpy(pData+dataOff,pData1+dataOffIp+10,2);/* SFP�����˿�*/
		dataOff+=2;

		memcpy(pData+dataOff,pData1+dataOffIp+6,4);/* ����IP*/
		dataOff+=4;

		memcpy(pData+dataOff,pData1+dataOffIp+12,4);/* SFP�����˿�*/
		dataOff+=2;

		/*ƫ������������*/
		dataOffIp+=DSU_IP_INFO_STRU_SIZE;
	}

#endif

	/*�������ݵ�����*/
	dataLineNum=data2Len/DSU_DIFF_RELATION_LENGTH;

	ShortToChar((UINT16)dataLineNum,pData+dataOff);
	dataOff+=2;

	/*��������*/
	memcpy(pData+dataOff,pData2,data2Len);
	dataOff+=data2Len;

	/*�������ݵ�����*/
	dataLineNum=data3Len/DSU_SAME_RELATION_LENGTH;

	ShortToChar((UINT16)dataLineNum,pData+dataOff);
	dataOff+=2;

	/*��������*/
	memcpy(pData+dataOff,pData3,data3Len);
	dataOff+=data3Len;

	*pDataBase=pData;

	chReturnValue=1;
	return chReturnValue;
}

