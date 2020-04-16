/************************************************************************
*
* �ļ���   ��  dquStructInit.c
* ��Ȩ˵�� ��  �������ؿƼ����޹�˾
* �汾��   ��  1.0
* ����ʱ�� ��  2009.11.21
* ����     ��  �����
* �������� ��  ���ݲ�ѯ�����ṹ�������  
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#include <stdio.h>
#include "dquStructInit.h"
#include "dquCbtcType.h"
 

static void ROUTE_STRU_init_HLT(UINT8 * pDataAdd,DSU_ROUTE_STRU * pRouteStru);

/**************************************
�������ܣ�link�ṹ���ʼ������
����˵����[in] pData ���ݵ�ַ���������ļ���
          [out] pLinkStrc ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
static void LINK_STRU_init_DFT(UINT8 * pDataAdd,DSU_LINK_STRU* pLinkStrc)
{
	 UINT8 * pData = pDataAdd;
	/*link���*/
	pLinkStrc->wId = ShortFromChar(pData);
	pData += 2;
				
	/*link���ȣ�cm��*/
	pLinkStrc->dwLength = LongFromChar(pData);
	pData += 4;
 
	/*Link������·����������Ϣ*/   /*����---�Ϲ�ѧ*/
	pLinkStrc->wLinkLine = ShortFromChar(pData);
	pData += 2;
 
   /*link���˵�����*/ 
	pLinkStrc->wOrgnPointType = ShortFromChar(pData);
	pData += 2;

	/*link���˵���*/
	pLinkStrc->wOrgnPointId = ShortFromChar(pData);
	pData += 2;

	/*link��������ӵ���������link���*/	
	pLinkStrc->wOrgnJointMainLkId = ShortFromChar(pData);
	pData += 2;
		
	/*link��������ӵ����ڲ���link���*/	
	pLinkStrc->wOrgnJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*link�յ�˵�����*/
	pLinkStrc->wTmnlPointType = ShortFromChar(pData);
	pData += 2;
	
	/*link�յ�˵���*/
	pLinkStrc->wTmnlPointId = ShortFromChar(pData);
	pData += 2;

	/*link�յ������ӵ���������link���*/
	pLinkStrc->wTmnlJointMainLkId = ShortFromChar(pData);
	pData += 2;

	/*link�յ������ӵ����ڲ���link���*/
	pLinkStrc->wTmnlJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*����ZC������*/
	pLinkStrc->wManageZC = ShortFromChar(pData);
	pData += 2;

	/*����ATS������*/
	pLinkStrc->wManageATS = ShortFromChar(pData);
	pData += 2;

	/*�����߼�CI������*/
	pLinkStrc->wManageLogicCI = ShortFromChar(pData);
	pData += 2;

	/*��������CI������*/   /*----ȷ������Ϊ2���ֽ�---*/
	pLinkStrc->wManagePhysicalCI = ShortFromChar(pData);
	pData += 2;

	/*Link������Ϣ����*/
	pLinkStrc->wLinkResSpeed= ShortFromChar(pData);
	pData += 2;

	/*Link�¶���Ϣ����*/
	pLinkStrc->wGrade= ShortFromChar(pData);
	pData += 2;

	/*Link������վ����ID:2013��8��13��,�ɳ������ɾ��;
	pLinkStrc->wStationArea = ShortFromChar(pData);
	pData += 2;*/

	pLinkStrc->wLogicDir = pLinkStrc->wLinkLine;		
	

}

static void LINK_STRU_init_HLT(UINT8 *pDataAdd, DSU_LINK_STRU *pLinkStrc)
{
	UINT8 *pData = pDataAdd;
	/*link���*/
	pLinkStrc->wId = ShortFromChar(pData);
	pData += 2;
					
	/*link���ȣ�cm��*/
	pLinkStrc->dwLength = LongFromChar(pData);
	pData += 4;
	 
	/*Link������·����������Ϣ*/   /*����---�Ϲ�ѧ*/
	pLinkStrc->wLinkLine = ShortFromChar(pData);
	pData += 2;
	 
   /*link���˵�����*/ 
	pLinkStrc->wOrgnPointType = ShortFromChar(pData);
	pData += 2;

	/*link���˵���*/
	pLinkStrc->wOrgnPointId = ShortFromChar(pData);
	pData += 2;

	/*link��������ӵ���������link���*/	
	pLinkStrc->wOrgnJointMainLkId = ShortFromChar(pData);
	pData += 2;
			
	/*link��������ӵ����ڲ���link���*/	
	pLinkStrc->wOrgnJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*link�յ�˵�����*/
	pLinkStrc->wTmnlPointType = ShortFromChar(pData);
	pData += 2;
		
	/*link�յ�˵���*/
	pLinkStrc->wTmnlPointId = ShortFromChar(pData);
	pData += 2;

	/*link�յ������ӵ���������link���*/
	pLinkStrc->wTmnlJointMainLkId = ShortFromChar(pData);
	pData += 2;

	/*link�յ������ӵ����ڲ���link���*/
	pLinkStrc->wTmnlJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*����ZC������*/
	pLinkStrc->wManageZC = ShortFromChar(pData);
	pData += 2;

	/*����ATS������*/
	pLinkStrc->wManageStation = ShortFromChar(pData);
	pData += 2;

	/*�����߼�CI������*/
	pLinkStrc->wManageLogicCI = ShortFromChar(pData);
	pData += 2;

	/*��������CI������*/   /*----ȷ������Ϊ2���ֽ�---*/
	pLinkStrc->wManagePhysicalCI = ShortFromChar(pData);
	pData += 2;

	/*Link������Ϣ����*/
	pLinkStrc->wLinkResSpeed = ShortFromChar(pData);
	pData += 2;

	/*Link�¶���Ϣ����*/
	pLinkStrc->wGrade = ShortFromChar(pData);
	pData += 2;

	/*Link������վ����ID:2013��8��13��,�ɳ������ɾ��;
	pLinkStrc->wStationArea = ShortFromChar(pData);
	pData += 2;*/

	pLinkStrc->wLogicDir = pLinkStrc->wLinkLine;
		

	pLinkStrc->wManageATS = ShortFromChar(pData);       
	pData += 2;

	pLinkStrc->wLinkLogicDirUpAndDown = *pData;       /*������ͨ����������ϵͳ�����߼������������з���Ĺ�ϵ add by qxt 20160811*/
	pData++;

	pLinkStrc->wLogicDirChanged = *pData;            /*�߼�����仯�㣬������������⣬ add by qxt 20170816*/
	pData++;

	pLinkStrc->wTunAttribute = *pData;				/*Link������ԣ����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
	pData++;
	
}

static void LINK_STRU_init_DG(UINT8 * pDataAdd,DSU_LINK_STRU* pLinkStrc)
{
	 UINT8 * pData = pDataAdd;
	/*link���*/
	pLinkStrc->wId = ShortFromChar(pData);
	pData += 2;
				
	/*link���ȣ�cm��*/
	pLinkStrc->dwLength = LongFromChar(pData);
	pData += 4;
 
	/*Link������·����������Ϣ*/   /*����---�Ϲ�ѧ*/
	pLinkStrc->wLinkLine = ShortFromChar(pData);
	pData += 2;
 
   /*link���˵�����*/ 
	pLinkStrc->wOrgnPointType = ShortFromChar(pData);
	pData += 2;

	/*link���˵���*/
	pLinkStrc->wOrgnPointId = ShortFromChar(pData);
	pData += 2;

	/*link��������ӵ���������link���*/	
	pLinkStrc->wOrgnJointMainLkId = ShortFromChar(pData);
	pData += 2;
		
	/*link��������ӵ����ڲ���link���*/	
	pLinkStrc->wOrgnJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*link�յ�˵�����*/
	pLinkStrc->wTmnlPointType = ShortFromChar(pData);
	pData += 2;
	
	/*link�յ�˵���*/
	pLinkStrc->wTmnlPointId = ShortFromChar(pData);
	pData += 2;

	/*link�յ������ӵ���������link���*/
	pLinkStrc->wTmnlJointMainLkId = ShortFromChar(pData);
	pData += 2;

	/*link�յ������ӵ����ڲ���link���*/
	pLinkStrc->wTmnlJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*����ZC������*/
	pLinkStrc->wManageZC = ShortFromChar(pData);
	pData += 2;

	/*����ATS������*/
	pLinkStrc->wManageStation = ShortFromChar(pData);
	pData += 2;

	/*�����߼�CI������*/
	pLinkStrc->wManageLogicCI = ShortFromChar(pData);
	pData += 2;

	/*��������CI������*/   /*----ȷ������Ϊ2���ֽ�---*/
	pLinkStrc->wManagePhysicalCI = ShortFromChar(pData);
	pData += 2;

	/*Link������Ϣ����*/
	pLinkStrc->wLinkResSpeed= ShortFromChar(pData);
	pData += 2;

	/*Link�¶���Ϣ����*/
	pLinkStrc->wGrade= ShortFromChar(pData);
	pData += 2;

	/*Link������վ����ID:2013��8��13��,�ɳ������ɾ��;
	pLinkStrc->wStationArea = ShortFromChar(pData);
	pData += 2;*/

	pLinkStrc->wLogicDir = pLinkStrc->wLinkLine;
	

	pLinkStrc->wManageATS= ShortFromChar(pData);       
	pData += 2;

	pLinkStrc->wLinkLogicDirUpAndDown= *pData;       /*������ͨ����������ϵͳ�����߼������������з���Ĺ�ϵ add by qxt 20160811*/
    pData++;

}


/**************************************
�������ܣ�link�ṹ���ʼ������-���� add by sds 2019-6-13
����˵����[in] pData ���ݵ�ַ���������ļ���
          [out] pLinkStrc ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�����Ѿ�����
***************************/
static void LINK_STRU_init_CC(UINT8 * pDataAdd,DSU_LINK_STRU* pLinkStrc)
{
	UINT8 * pData = pDataAdd;
	/*link���*/
	pLinkStrc->wId = ShortFromChar(pData);
	pData += 2;

	/*link���ȣ�cm��*/
	pLinkStrc->dwLength = LongFromChar(pData);
	pData += 4;

	/*Link������·����������Ϣ*/
	pLinkStrc->wLinkLine = ShortFromChar(pData);
	pData += 2;

	/*link���˵�����*/ 
	pLinkStrc->wOrgnPointType = ShortFromChar(pData);
	pData += 2;

	/*link���˵���*/
	pLinkStrc->wOrgnPointId = ShortFromChar(pData);
	pData += 2;

	/*link��������ӵ���������link���*/	
	pLinkStrc->wOrgnJointMainLkId = ShortFromChar(pData);
	pData += 2;

	/*link��������ӵ����ڲ���link���*/	
	pLinkStrc->wOrgnJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*link�յ�˵�����*/
	pLinkStrc->wTmnlPointType = ShortFromChar(pData);
	pData += 2;

	/*link�յ�˵���*/
	pLinkStrc->wTmnlPointId = ShortFromChar(pData);
	pData += 2;

	/*link�յ������ӵ���������link���*/
	pLinkStrc->wTmnlJointMainLkId = ShortFromChar(pData);
	pData += 2;

	/*link�յ������ӵ����ڲ���link���*/
	pLinkStrc->wTmnlJointSideLkId = ShortFromChar(pData);
	pData += 2;

	/*����OC������*/
	pLinkStrc->wManageOC = ShortFromChar(pData);
	pData += 2;

	/*Link������Ϣ����*/
	pLinkStrc->wLinkResSpeed= ShortFromChar(pData);
	pData += 2;

	/*Link�¶���Ϣ����*/
	pLinkStrc->wGrade= ShortFromChar(pData);
	pData += 2;

	/*Link�������*/
	pLinkStrc->wTunAttribute= *pData;
	pData += 1;

	/*����ITS�豸ID*/
	pLinkStrc->wManageITS = ShortFromChar(pData);
	pData += 2;

	pLinkStrc->wLogicDir = pLinkStrc->wLinkLine;		

}

/*���ýӿ�*/
/*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726*/
void LINK_STRU_init(UINT8 *pDataAdd,DSU_LINK_STRU *pLinkStrc)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType) )
	{
		LINK_STRU_init_DFT(pDataAdd, pLinkStrc);
	} 
	else if (DQU_CBTC_HLT == cbtcVersionType)
	{
		LINK_STRU_init_HLT(pDataAdd, pLinkStrc);
	}
	else if(DQU_CBTC_DG == cbtcVersionType)
	{
		LINK_STRU_init_DG(pDataAdd, pLinkStrc);
	}
	else if(DQU_CBTC_CC == cbtcVersionType)
	{
		LINK_STRU_init_CC(pDataAdd, pLinkStrc);
	}
	else
	{
       /*���ɴ��֧*/
	}
   
}

/**************************************
�������ܣ��������ݽṹ���ʼ������--����
����˵����[in] pData ���ݵ�ַ���������ļ���
          [out]pPointStru ����ʼ���Ľṹ��ָ��
����ֵ����
***************************/
static void POINT_STRU_init_CC(UINT8 *pDataAdd,DSU_POINT_STRU *pPointStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i = 0;
	/*������*/
	pPointStru->wId = ShortFromChar(pData);
	pData += 2;

	/*��������*/
	for (i=0;i<16;i++)
	{
		pPointStru->szCaption[i] = *pData;
		pData++;
	}

	/*��������link���*/
	pPointStru->wMainLkId = ShortFromChar(pData);
	pData += 2;

	/*��������linkƫ����(cm)*/
	pPointStru->dwMainLkOfst = LongFromChar(pData);
	pData += 4;

	/*��������link���*/
	pPointStru->wSideLkId = ShortFromChar(pData);
	pData += 2;

	/*��������linkƫ����(cm)*/
	pPointStru->dwSideLkOfst = LongFromChar(pData);
	pData += 4;

	/*�������link���*/
	pPointStru->wConfluentLkId = ShortFromChar(pData);
	pData += 2;

	/*�������linkƫ����(cm)*/
	pPointStru->dwConfluentLkOfst = LongFromChar(pData);
	pData += 4;

	/*����λ��̬�����ٶ�(cm/s)*/
	pPointStru->wSideResSpeed = ShortFromChar(pData);
	pData += 2;

	/*����󷽷�������*/
	pPointStru->wRearSwitchProtecDis = ShortFromChar(pData);
	pData += 2;

	/*��������*/
	pPointStru->DProperty = *pData;
	pData ++;
	

}

/**************************************
�������ܣ��������ݽṹ���ʼ������
����˵����[in] pData ���ݵ�ַ���������ļ���
          [out]pPointStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ������
    UINT16  wManageZC;				   ����ZC������
	UINT16  wManagePhysicalCI;		   ��������CI������
	UINT16  wManageLogicCI;			   �����߼�CI������
�޸�˵�������ݺϿ����ݽṹV19.0.0�����޸ģ�20180208 lmy
***************************/
static void POINT_STRU_init_DFT(UINT8 *pDataAdd,DSU_POINT_STRU *pPointStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i = 0;
	/*������*/
	pPointStru->wId = ShortFromChar(pData);
	pData += 2;

	/*��������*/
	for (i=0;i<16;i++)
	{
		pPointStru->szCaption[i] = *pData;
		pData++;
	}

	/*����������*/
	pPointStru->DPointOtherId = ShortFromChar(pData);
	pData += 2;

	/*����㹫���*/
	pPointStru->dwPointKilo = LongFromChar(pData);
	pData += 4;

	/*��������link���*/
	pPointStru->wMainLkId = ShortFromChar(pData);
	pData += 2;

	/*��������linkƫ����(cm)*/
	pPointStru->dwMainLkOfst = LongFromChar(pData);
	pData += 4;

	/*��������link���*/
	pPointStru->wSideLkId = ShortFromChar(pData);
	pData += 2;

	/*��������linkƫ����(cm)*/
	pPointStru->dwSideLkOfst = LongFromChar(pData);
	pData += 4;

	/*�������link���*/
	pPointStru->wConfluentLkId = ShortFromChar(pData);
	pData += 2;

	/*�������linkƫ����(cm)*/
	pPointStru->dwConfluentLkOfst = LongFromChar(pData);
	pData += 4;

	/*����λ��̬�����ٶ�(cm/s)*/
	pPointStru->wSideResSpeed = ShortFromChar(pData);
	pData += 2;

}

/**************************************
�������ܣ��������ݽṹ���ʼ������
����˵����[in] pData ���ݵ�ַ���������ļ���
          [out]pPointStru ����ʼ���Ľṹ��ָ��
����ֵ����
�޸�˵�������ݳ������ݽṹV1.0.0�����޸ģ�2019-7-10 by sds
***************************/
void POINT_STRU_init(UINT8 *pDataAdd,DSU_POINT_STRU *pPointStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType)
		||(DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		POINT_STRU_init_DFT(pDataAdd, pPointStru);
	} 
	else if(DQU_CBTC_CC == cbtcVersionType)
	{
		POINT_STRU_init_CC(pDataAdd, pPointStru);
	}
	else
	{
		/*���ɴ��֧*/
	}

}

/**************************************
�������ܣ��źŻ����ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pSignalStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע����������Ŀ��
	UINT16  wManageZC;					����ZC������
	UINT16  wManagePhysicalCI;		��������CI������
	UINT16  wManageLogicCI;			�����߼�CI������
***************************/
void SIGNAL_STRU_init(UINT8 * pDataAdd, DSU_SIGNAL_STRU * pSignalStru)
{
	UINT8 cbtcVersionType = 0;
	cbtcVersionType = GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType))
	{
		SIGNAL_STRU_init_DFT(pDataAdd, pSignalStru);
	}
	else if (DQU_CBTC_CC == cbtcVersionType)
	{
		SIGNAL_STRU_init_CC(pDataAdd, pSignalStru);
	}
	else
	{
		/*��֧���ɴ�*/
	}
}

/**************************************
�������ܣ�DFT�źŻ����ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
[out] pSignalStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע����������Ŀ��
UINT16  wManageZC;					����ZC������
UINT16  wManagePhysicalCI;		��������CI������
UINT16  wManageLogicCI;			�����߼�CI������
***************************/
void SIGNAL_STRU_init_DFT(UINT8 * pDataAdd, DSU_SIGNAL_STRU * pSignalStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	/*���*/
	pSignalStru->wId = ShortFromChar(pData);
    pData += 2;
	
	/*�źŻ�����*/

	for(i=0;i<12;i++)
	{
		pSignalStru->szCaption[i] = *pData;
       pData += 1;
	}

	/*�źŻ�����*/
	pSignalStru->wType = ShortFromChar(pData);
    pData += 2;
	
	/*�źŻ�����*/
	pSignalStru->wAttribute = ShortFromChar(pData);
    pData += 2;
	
	/*�źŻ�����link���*/
	pSignalStru->wLkId = ShortFromChar(pData);
    pData += 2;
	
	/*�źŻ�����linkƫ����(cm)*/
	pSignalStru->dwLkOfst = LongFromChar(pData);
    pData += 4;

	/*�źŻ���������(16����)*/
	pSignalStru->wProtectDir = ShortFromChar(pData);
    pData += 2;
	
	/*�źŻ�����������link���*/
	pSignalStru->wProtectLinkId = ShortFromChar(pData);
    pData += 2;
	
	/*�źŻ�����������linkƫ����(cm)*/
	pSignalStru->dwProtectLinkOfst = LongFromChar(pData);
    pData += 4;
	
	/*�����źŻ���־*/	

	pSignalStru->wPassSignal = ShortFromChar(pData);
    pData += 2;

	/*�źŻ��жϴ��źŹ��ܱ�־*/
	pSignalStru->wEnableSignal = ShortFromChar(pData);
    pData += 2;
		
	/*�źŻ������ܱ�־*/
	pSignalStru->wShineSignal = ShortFromChar(pData);
    pData += 2;	   

	/*�źŻ�����*/
	pSignalStru->dwSignalDengLie = LongFromChar(pData);
	pData += 4;

	/*�źŻ���λ���*/
	pSignalStru->sDengWeiFengBi = *pData;
	pData++;
}

/**************************************
�������ܣ�CC�źŻ����ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
[out] pSignalStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע����������Ŀ��
UINT16  wManageZC;					����ZC������
UINT16  wManagePhysicalCI;		��������CI������
UINT16  wManageLogicCI;			�����߼�CI������
***************************/
void SIGNAL_STRU_init_CC(UINT8 * pDataAdd, DSU_SIGNAL_STRU * pSignalStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i = 0;
	/*���*/
	pSignalStru->wId = ShortFromChar(pData);
	pData += 2;

	/*�źŻ�����*/

	for (i = 0; i<12; i++)
	{
		pSignalStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*�źŻ�����*/
	pSignalStru->wType = ShortFromChar(pData);
	pData += 2;

	/*�źŻ�����*/
	pSignalStru->wAttribute = ShortFromChar(pData);
	pData += 2;

	/*�źŻ�����link���*/
	pSignalStru->wLkId = ShortFromChar(pData);
	pData += 2;

	/*�źŻ�����linkƫ����(cm)*/
	pSignalStru->dwLkOfst = LongFromChar(pData);
	pData += 4;

	/*�źŻ���������(16����)*/
	pSignalStru->wProtectDir = ShortFromChar(pData);
	pData += 2;

	/*�źŻ�����*/
	pSignalStru->dwSignalDengLie = LongFromChar(pData);
    pData += 4;	 

	/*�źŻ���λ���*/
	pSignalStru->sDengWeiFengBi = *pData;
    pData ++;	
}

/**************************************
�������ܣ�Ӧ�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pBaliseStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ýṹ������Ķ�
***************************/
/*Ĭ�ϰ汾����Ӧ��������Ϊ10��*/
static void BALISE_STRU_init_DFT(UINT8 * pDataAdd,DSU_BALISE_STRU * pBaliseStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*���*/
	pBaliseStru->wId = ShortFromChar(pData);
    pData += 2;

	/*Ӧ����ID*/
	pBaliseStru->dwBaliseId = LongFromChar(pData);
    pData += 4;

	/*Ӧ��������*/
	for(i=0;i<8;i++)
	{
	   pBaliseStru->szCaption[i] = *pData;
       pData += 1;
	}

	/*Ӧ��������link���*/
	pBaliseStru->wLkId = ShortFromChar(pData);
    pData += 2;

	/*Ӧ��������linkƫ����(cm)*/
	pBaliseStru->dwLkOfst = LongFromChar(pData);
    pData += 4;

	/*��Ӧ�����������źŻ����*/
	pBaliseStru->wRelateSignalID = ShortFromChar(pData);
    pData += 2;

	/*Ӧ�������÷���()*/
	pBaliseStru->wBaliseDir = ShortFromChar(pData);
    pData += 2;

	/*Ӧ��������*/
	pBaliseStru->wBaliseType = ShortFromChar(pData);
    pData += 2;

	/*������link�߼��������������ڵ�Ӧ��������*/
	pBaliseStru->wOrgnBaliseNum = ShortFromChar(pData);
    pData += 2;

	
	for(i=0;i<10;i++)
	{
		/*������link�߼�������������ڵ�Ӧ�����ı��*/
	   pBaliseStru->dwOrgnBaliseId[i] = LongFromChar(pData);
       pData += 4;

	   /*����·����㷽���뱾Ӧ���������Ҷ��г���Ч����ֱ�ӵ����Ӧ������·���ϣ��г�������Ӧ�����پ���������Ӧ����ʱ�ķ���*/
		pBaliseStru->wPassOrgnBaliseDir[i] = ShortFromChar(pData);
        pData += 2;

	  /*������link�߼�������������ڵ�Ӧ�����ľ���*/	
	   pBaliseStru->dwDistanceToOrgnBalise[i] = LongFromChar(pData);
       pData += 4;

	    /*�������Ӧ��������������*/
		pBaliseStru->wOrgnRelatePointID[i] = ShortFromChar(pData);
        pData += 2;

	   /*�������Ӧ������������״̬*/		
		pBaliseStru->wOrgnRelatePointStatus[i] = ShortFromChar(pData);
        pData += 2;
	}

	/*������link�߼��������յ�����ڵ�Ӧ��������*/
	pBaliseStru->wTmnlBaliseNum = ShortFromChar(pData);
    pData += 2;

	/*������link�߼��������յ����ڵ�Ӧ�����ı��*/
	for(i=0;i<10;i++)
	{
	   pBaliseStru->dwTmnlBaliseId[i] = LongFromChar(pData);
       pData += 4;	

	   /*����·���յ㷽���뱾Ӧ���������Ҷ��г���Ч����ֱ�ӵ����Ӧ������·���ϣ��г�������Ӧ�����پ���������Ӧ����ʱ�ķ���*/	
		pBaliseStru->wPassTmnlBaliseDir[i] = ShortFromChar(pData);
        pData += 2;

	   /*������link�߼��������յ����ڵ�Ӧ�����ľ���*/		
	   pBaliseStru->dwDistanceToTmnlBalise[i] = LongFromChar(pData);
       pData += 4;

	    /*�յ�����Ӧ��������������*/
		pBaliseStru->wTmnlRelatePointID[i] = ShortFromChar(pData);
        pData += 2;
	
	    /*�յ�����Ӧ������������״̬*/	
	
		pBaliseStru->wTmnlRelatePointStatus[i] = ShortFromChar(pData);
        pData += 2;
	}

	/*LEU���*/
	pBaliseStru->wLeuId = ShortFromChar(pData);
    pData += 2;
}
/*������ͨ�汾����Ӧ�����������ӵ���16��*/
static void BALISE_STRU_init_HLT(UINT8 * pDataAdd,DSU_BALISE_STRU * pBaliseStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*���*/
	pBaliseStru->wId = ShortFromChar(pData);
	pData += 2;

	/*Ӧ����ID*/
	pBaliseStru->dwBaliseId = LongFromChar(pData);
	pData += 4;

	/*Ӧ��������*/
	for(i=0;i<8;i++)
	{
		pBaliseStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*Ӧ��������link���*/
	pBaliseStru->wLkId = ShortFromChar(pData);
	pData += 2;

	/*Ӧ��������linkƫ����(cm)*/
	pBaliseStru->dwLkOfst = LongFromChar(pData);
	pData += 4;

	/*��Ӧ�����������źŻ����*/
	pBaliseStru->wRelateSignalID = ShortFromChar(pData);
	pData += 2;

	/*Ӧ�������÷���()*/
	pBaliseStru->wBaliseDir = ShortFromChar(pData);
	pData += 2;

	/*Ӧ��������*/
	pBaliseStru->wBaliseType = ShortFromChar(pData);
	pData += 2;

	/*������link�߼��������������ڵ�Ӧ��������*/
	pBaliseStru->wOrgnBaliseNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<16;i++)
	{
		/*������link�߼�������������ڵ�Ӧ�����ı��*/
		pBaliseStru->dwOrgnBaliseId[i] = LongFromChar(pData);
		pData += 4;

		/*����·����㷽���뱾Ӧ���������Ҷ��г���Ч����ֱ�ӵ����Ӧ������·���ϣ��г�������Ӧ�����پ���������Ӧ����ʱ�ķ���*/
		pBaliseStru->wPassOrgnBaliseDir[i] = ShortFromChar(pData);
		pData += 2;

		/*������link�߼�������������ڵ�Ӧ�����ľ���*/	
		pBaliseStru->dwDistanceToOrgnBalise[i] = LongFromChar(pData);
		pData += 4;

		/*�������Ӧ��������������*/
		pBaliseStru->wOrgnRelatePointID[i] = ShortFromChar(pData);
		pData += 2;

		/*�������Ӧ������������״̬*/		
		pBaliseStru->wOrgnRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*������link�߼��������յ�����ڵ�Ӧ��������*/
	pBaliseStru->wTmnlBaliseNum = ShortFromChar(pData);
	pData += 2;

	/*������link�߼��������յ����ڵ�Ӧ�����ı��*/
	for(i=0;i<16;i++)
	{
		pBaliseStru->dwTmnlBaliseId[i] = LongFromChar(pData);
		pData += 4;	

		/*����·���յ㷽���뱾Ӧ���������Ҷ��г���Ч����ֱ�ӵ����Ӧ������·���ϣ��г�������Ӧ�����پ���������Ӧ����ʱ�ķ���*/	
		pBaliseStru->wPassTmnlBaliseDir[i] = ShortFromChar(pData);
		pData += 2;

		/*������link�߼��������յ����ڵ�Ӧ�����ľ���*/		
		pBaliseStru->dwDistanceToTmnlBalise[i] = LongFromChar(pData);
		pData += 4;

		/*�յ�����Ӧ��������������*/
		pBaliseStru->wTmnlRelatePointID[i] = ShortFromChar(pData);
		pData += 2;

		/*�յ�����Ӧ������������״̬*/	

		pBaliseStru->wTmnlRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*LEU���*/
	pBaliseStru->wLeuId = ShortFromChar(pData);
	pData += 2;

	/*���ӵ�ͼ�汾��Ϣ---���ݻ�����ͨ�������� by hgj 20150507*/
	pBaliseStru->wEmapVersion = ShortFromChar(pData);
	pData += 2;

}


/**************************************
�������ܣ�Ӧ�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pAxleStru ����ʼ���Ľṹ��ָ��
����ֵ����
�޸�˵�������ݳ������ݽṹV1.0.0�������ӣ�2019-7-10 sds
***************************/
static void BALISE_STRU_init_CC(UINT8 * pDataAdd,DSU_BALISE_STRU * pBaliseStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*���*/
	pBaliseStru->wId = ShortFromChar(pData);
	pData += 2;

	/*Ӧ����ID*/
	pBaliseStru->dwBaliseId = LongFromChar(pData);
	pData += 4;

	/*Ӧ��������*/
	for(i=0;i<8;i++)
	{
		pBaliseStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*Ӧ��������link���*/
	pBaliseStru->wLkId = ShortFromChar(pData);
	pData += 2;

	/*Ӧ��������linkƫ����(cm)*/
	pBaliseStru->dwLkOfst = LongFromChar(pData);
	pData += 4;

	/*Ӧ��������*/
	pBaliseStru->wBaliseType = ShortFromChar(pData);
	pData += 2;

	/*������link�߼��������������ڵ�Ӧ��������*/
	pBaliseStru->wOrgnBaliseNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<10;i++)
	{
		/*������link�߼�������������ڵ�Ӧ�����ı��*/
		pBaliseStru->dwOrgnBaliseId[i] = LongFromChar(pData);
		pData += 4;

		/*����·����㷽���뱾Ӧ���������Ҷ��г���Ч����ֱ�ӵ����Ӧ������·���ϣ��г�������Ӧ�����پ���������Ӧ����ʱ�ķ���*/
		pBaliseStru->wPassOrgnBaliseDir[i] = ShortFromChar(pData);
		pData += 2;

		/*������link�߼�������������ڵ�Ӧ�����ľ���*/	
		pBaliseStru->dwDistanceToOrgnBalise[i] = LongFromChar(pData);
		pData += 4;

		/*�������Ӧ��������������*/
		pBaliseStru->wOrgnRelatePointID[i] = ShortFromChar(pData);
		pData += 2;

		/*�������Ӧ������������״̬*/		
		pBaliseStru->wOrgnRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*������link�߼��������յ�����ڵ�Ӧ��������*/
	pBaliseStru->wTmnlBaliseNum = ShortFromChar(pData);
	pData += 2;

	/*������link�߼��������յ����ڵ�Ӧ�����ı��*/
	for(i=0;i<10;i++)
	{
		pBaliseStru->dwTmnlBaliseId[i] = LongFromChar(pData);
		pData += 4;	

		/*����·���յ㷽���뱾Ӧ���������Ҷ��г���Ч����ֱ�ӵ����Ӧ������·���ϣ��г�������Ӧ�����پ���������Ӧ����ʱ�ķ���*/	
		pBaliseStru->wPassTmnlBaliseDir[i] = ShortFromChar(pData);
		pData += 2;

		/*������link�߼��������յ����ڵ�Ӧ�����ľ���*/		
		pBaliseStru->dwDistanceToTmnlBalise[i] = LongFromChar(pData);
		pData += 4;

		/*�յ�����Ӧ��������������*/
		pBaliseStru->wTmnlRelatePointID[i] = ShortFromChar(pData);
		pData += 2;

		/*�յ�����Ӧ������������״̬*/	

		pBaliseStru->wTmnlRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*���ӵ�ͼ�汾��Ϣ---���ݻ�����ͨ�������� by hgj 20150507*/
	pBaliseStru->wEmapVersion = ShortFromChar(pData);
	pData += 2;

}

/*���ýӿ�*/
/*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726*/
void BALISE_STRU_init(UINT8 *pDataAdd,DSU_BALISE_STRU *pBaliseStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO ==  cbtcVersionType)||(DQU_CBTC_CPK == cbtcVersionType) )
	{
		BALISE_STRU_init_DFT(pDataAdd, pBaliseStru);/*FAO�ͺϿ⹲�õĴ���*/
	} 
	else if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType))
	{
		BALISE_STRU_init_HLT(pDataAdd, pBaliseStru);/*������ͨ������Ĵ���*/
	}
	else if (DQU_CBTC_CC == cbtcVersionType)
	{
		BALISE_STRU_init_CC(pDataAdd, pBaliseStru);/*�����Ĵ���*/
	}
	else
	{
       /*���ɴ��֧*/
	}
}

/**************************************
�������ܣ����������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pAxleStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ýṹ������Ķ�
�޸�˵�������ݺϿ����ݽṹV19.0.0�����޸ģ�20180208 lmy
***************************/
void AXLE_STRU_init(UINT8 *pDataAdd,DSU_AXLE_STRU *pAxleStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i=0;

	/*���*/
	pAxleStru->wId = ShortFromChar(pData);
	pData += 2;

	/*����������*/		
	for(i=0;i<16;i++)
	{
		pAxleStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*����������Ϣ*/
	pAxleStru->wType = ShortFromChar(pData);
	pData += 2;

	/*�����������*/
	pAxleStru->dwAxleKilo = LongFromChar(pData);
	pData += 4;

	/*����������link���*/	
	pAxleStru->wLkId = ShortFromChar(pData);
	pData += 2;

	/*����������linkƫ����(cm)*/
	pAxleStru->dwLkOfst = LongFromChar(pData);
	pData += 4;
}

/**************************************
�������ܣ���ȫ���������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pScreenStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������
	UINT16  wManageLogicCI;						�����߼�CI������
***************************/
void SCREEN_STRU_init(UINT8 * pDataAdd,DSU_SCREEN_STRU * pScreenStru)
{
	UINT8 * pData = pDataAdd;
	pScreenStru->wId = ShortFromChar(pData);
    pData += 2;

	pScreenStru->wLocalPlatformId = ShortFromChar(pData);
    pData += 2;

	pScreenStru->wRelatLkId = ShortFromChar(pData);
    pData += 2;

	pScreenStru->wwRelatLkOrient = ShortFromChar(pData);
    pData += 2;

	pScreenStru->wVobcCiID = ShortFromChar(pData);
    pData += 2;
	
}

/**************************************
�������ܣ�����ͣ����ť���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pEmergStopStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������
	UINT16  wManageLogicCI;						�����߼�CI������
***************************/
void EMERG_STOP_STRU_init(UINT8 * pDataAdd,DSU_EMERG_STOP_STRU * pEmergStopStru)
{
	UINT8 * pData = pDataAdd;
	pEmergStopStru->wId = ShortFromChar(pData);
    pData += 2;

	pEmergStopStru->wLocalPlatformId = ShortFromChar(pData);
    pData += 2;

	pEmergStopStru->wRelatLkId = ShortFromChar(pData);
    pData += 2;

}

/**************************************
�������ܣ�ͣ��������Ϣ���ݽṹ���ʼ������(ԭվ̨��)
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pStationStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ����������
   UINT32 dwStatTmnlLinkOfst;				վ̨����Χ��ֹ��linkƫ����
   UINT16  wManagePhysicalCI            ��������CI������
	UINT16  wManageLogicCI;						�����߼�CI������
***************************/
static void STATION_STRU_init_DFT(UINT8 * pDataAdd,DSU_STATION_STRU * pStationStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	pStationStru->wId = ShortFromChar(pData);
    pData += 2;

	for(i=0;i<20;i++)
	{
		pStationStru->szCaption[i] = *pData;
        pData += 1;
	}
    /*Ŀ�ĵر���*/
	for(i=0;i<4;i++)
	{
		pStationStru->bDesId[i] = *pData;
        pData += 1;
	}

	pStationStru->wLinkId = ShortFromChar(pData);
    pData += 2;

	pStationStru->wAttribute = ShortFromChar(pData);
    pData += 2;

	pStationStru->wMinParkTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wMaxParkTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wParkTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wScreenNum = ShortFromChar(pData);
    pData += 2;

	for(i=0;i<2;i++)
	{
		pStationStru->wScreenId[i] = ShortFromChar(pData);
        pData += 2;
	}

	pStationStru->wEmergStopNum = ShortFromChar(pData);
    pData += 2;

	for(i=0;i<2;i++)
	{
		pStationStru->wEmergStopId[i] = ShortFromChar(pData);
        pData += 2;
	}

	pStationStru->wStopPointNum = ShortFromChar(pData);
    pData += 2;

	for(i=0;i<3;i++)
	{
		pStationStru->wStopPointID[i] = ShortFromChar(pData);
        pData += 2;
	}

	pStationStru->wDoorOpenMode = ShortFromChar(pData);
    pData += 2;

	pStationStru->wDoorOpenTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wDoorCloseMode = ShortFromChar(pData);
    pData += 2;

	pStationStru->wDoorCloseTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wParkSteadyTime = ShortFromChar(pData);
    pData += 2;

	pStationStru->wStationID = ShortFromChar(pData); /*2013��8��13��,�ɳ����������*/
    pData += 2;
}

static void STATION_STRU_init_HLT(UINT8 * pDataAdd,DSU_STATION_STRU * pStationStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	pStationStru->wId = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<20;i++)
	{
		pStationStru->szCaption[i] = *pData;
		pData += 1;
	}
	/*Ŀ�ĵر���*/
	for(i=0;i<4;i++)
	{
		pStationStru->bDesId[i] = *pData;
		pData += 1;
	}

	pStationStru->wLinkId = ShortFromChar(pData);
	pData += 2;

	pStationStru->wAttribute = ShortFromChar(pData);
	pData += 2;

	pStationStru->wMinParkTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wMaxParkTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wParkTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wScreenNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<2;i++)
	{
		pStationStru->wScreenId[i] = ShortFromChar(pData);
		pData += 2;
	}

	pStationStru->wEmergStopNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<2;i++)
	{
		pStationStru->wEmergStopId[i] = ShortFromChar(pData);
		pData += 2;
	}

	pStationStru->wStopPointNum = ShortFromChar(pData);
	pData += 2;
	/*������ͣ����,������ͨΪ�ĸ�*/
	for(i=0;i<4;i++)
	{
		pStationStru->wStopPointID[i] = ShortFromChar(pData);
		pData += 2;
	}

	pStationStru->wDoorOpenMode = ShortFromChar(pData);
	pData += 2;

	pStationStru->wDoorOpenTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wDoorCloseMode = ShortFromChar(pData);
	pData += 2;

	pStationStru->wDoorCloseTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wParkSteadyTime = ShortFromChar(pData);
	pData += 2;

	pStationStru->wStationID = ShortFromChar(pData); /*2013��8��13��,�ɳ����������*/
	pData += 2;

	pStationStru->wLuggageOpenMode = ShortFromChar(pData);	/*����ŷ�ʽ�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
	pData += 2;

	pStationStru->wLuggageCloseMode = ShortFromChar(pData);	/*������ŷ�ʽ�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
	pData += 2;

	pStationStru->wCrashResSpeed = ShortFromChar(pData);	/*������ײ����ֵ�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312*/
	pData += 2;

	pStationStru->wATSStationId = ShortFromChar(pData);	    /*ATSͣ������ID*/
	pData += 2;

}

/***********************************************************************************
�������ܣ�ͣ��������Ϣ���ݽṹ���ʼ������(ԭվ̨��)
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pStationStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݻ�����ͨ���ݽṹV12.0.0�޸ĺ�
	  DG��HLT����ʹ����ͬ�ĳ�ʼ�������������Ӵ˺�����add by lmy 20180312
***********************************************************************************/
static void STATION_STRU_init_DG(UINT8 *pDataAdd, DSU_STATION_STRU *pStationStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i = 0;
	/*�������*/
	pStationStru->wId = ShortFromChar(pData);
	pData += 2;
	/*������վ����*/
	for(i=0;i<20;i++)
	{
		pStationStru->szCaption[i] = *pData;
		pData += 1;
	}
	/*Ŀ�ĵر���*/
	for(i=0;i<4;i++)
	{
		pStationStru->bDesId[i] = *pData;
		pData += 1;
	}
	/*ͣ������Link���*/
	pStationStru->wLinkId = ShortFromChar(pData);
	pData += 2;
	/*ͣ����������*/
	pStationStru->wAttribute = ShortFromChar(pData);
	pData += 2;
	/*վ̨��Сͣվʱ��*/
	pStationStru->wMinParkTime = ShortFromChar(pData);
	pData += 2;
	/*վ̨���ͣվʱ��*/
	pStationStru->wMaxParkTime = ShortFromChar(pData);
	pData += 2;
	/*վ̨Ĭ��ͣվʱ��*/
	pStationStru->wParkTime = ShortFromChar(pData);
	pData += 2;
	/*����������*/
	pStationStru->wScreenNum = ShortFromChar(pData);
	pData += 2;
	/*�����ű��*/
	for(i=0;i<2;i++)
	{
		pStationStru->wScreenId[i] = ShortFromChar(pData);
		pData += 2;
	}
	/*����ͣ����ť����*/
	pStationStru->wEmergStopNum = ShortFromChar(pData);
	pData += 2;
	/*����ͣ����ť���*/
	for(i = 0; i < 2; i++)
	{
		pStationStru->wEmergStopId[i] = ShortFromChar(pData);
		pData += 2;
	}
	/*����ͣ������Ŀ*/
	pStationStru->wStopPointNum = ShortFromChar(pData);
	pData += 2;
	/*������ͣ����*/
	for(i = 0; i < 4; i++)
	{
		pStationStru->wStopPointID[i] = ShortFromChar(pData);
		pData += 2;
	}
	/*վ̨���ŷ�ʽ*/
	pStationStru->wDoorOpenMode = ShortFromChar(pData);
	pData += 2;
	/*վ̨����ʱ����*/
	pStationStru->wDoorOpenTime = ShortFromChar(pData);
	pData += 2;
	/*վ̨���ŷ�ʽ*/
	pStationStru->wDoorCloseMode = ShortFromChar(pData);
	pData += 2;
	/*վ̨����ʱ����*/
	pStationStru->wDoorCloseTime = ShortFromChar(pData);
	pData += 2;
	/*Ĭ��ͣ��ʱ��*/
	pStationStru->wParkSteadyTime = ShortFromChar(pData);
	pData += 2;
	/*վ̨������վ���*/
	pStationStru->wStationID = ShortFromChar(pData); 
	pData += 2;
}

/***********************************************************************************
�������ܣ�ͣ��������Ϣ���ݽṹ���ʼ������-����ϵͳʹ�� add by sds 2019-6-13
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pStationStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע��
***********************************************************************************/
static void STATION_STRU_init_CC(UINT8 *pDataAdd, DSU_STATION_STRU *pStationStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i = 0;
	/*�������*/
	pStationStru->wId = ShortFromChar(pData);
	pData += 2;

	/*������վ����*/
	for(i=0;i<20;i++)
	{
		pStationStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*Ŀ�ĵر���*/
	for(i=0;i<4;i++)
	{
		pStationStru->bDesId[i] = *pData;
		pData += 1;
	}

	/*ͣ������Link���*/
	pStationStru->wLinkId = ShortFromChar(pData);
	pData += 2;

	/*ͣ����������*/
	pStationStru->wAttribute = ShortFromChar(pData);
	pData += 2;

	/*վ̨��Сͣվʱ��*/
	pStationStru->wMinParkTime = ShortFromChar(pData);
	pData += 2;

	/*վ̨���ͣվʱ��*/
	pStationStru->wMaxParkTime = ShortFromChar(pData);
	pData += 2;

	/*վ̨Ĭ��ͣվʱ��*/
	pStationStru->wParkTime = ShortFromChar(pData);
	pData += 2;

	/*����������*/
	pStationStru->wScreenNum = ShortFromChar(pData);
	pData += 2;

	/*�����ű��*/
	for(i=0;i<2;i++)
	{
		pStationStru->wScreenId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*����ͣ����ť����*/
	pStationStru->wEmergStopNum = ShortFromChar(pData);
	pData += 2;

	/*����ͣ����ť���*/
	for(i = 0; i < 2; i++)
	{
		pStationStru->wEmergStopId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*����ͣ������Ŀ*/
	pStationStru->wStopPointNum = ShortFromChar(pData);
	pData += 2;

	/*������ͣ����*/
	for(i = 0; i < 4; i++)
	{
		pStationStru->wStopPointID[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*վ̨���ŷ�ʽ*/
	pStationStru->wDoorOpenMode = ShortFromChar(pData);
	pData += 2;

	/*վ̨����ʱ����*/
	pStationStru->wDoorOpenTime = ShortFromChar(pData);
	pData += 2;

	/*վ̨���ŷ�ʽ*/
	pStationStru->wDoorCloseMode = ShortFromChar(pData);
	pData += 2;

	/*վ̨����ʱ����*/
	pStationStru->wDoorCloseTime = ShortFromChar(pData);
	pData += 2;

	/*Ĭ��ͣ��ʱ��*/
	pStationStru->wParkSteadyTime = ShortFromChar(pData);
	pData += 2;

	/*վ̨������վ���*/
	pStationStru->wStationID = ShortFromChar(pData); 
	pData += 2;

	/*ITSͣ��������*/
	pStationStru->wITSStationId = ShortFromChar(pData); 
	pData += 2;

}

/*���ýӿ�*/
/*��ע�����ݻ�����ͨ���ݽṹV12.0.0�޸ģ�modify by lmy 20180312*/
void STATION_STRU_init(UINT8 *pDataAdd,DSU_STATION_STRU *pStationStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType) )
	{
		STATION_STRU_init_DFT(pDataAdd, pStationStru);
	} 
	else if (DQU_CBTC_HLT == cbtcVersionType)
	{
		STATION_STRU_init_HLT(pDataAdd, pStationStru);
	}
	else if (DQU_CBTC_DG == cbtcVersionType)
	{
		STATION_STRU_init_DG(pDataAdd, pStationStru);
	}
	else if (DQU_CBTC_CC == cbtcVersionType)
	{
		STATION_STRU_init_CC(pDataAdd, pStationStru);
	}
	else
	{
       /*���ɴ��֧*/
	}
}

/**************************************
�������ܣ�ͣ�������ݽṹ���ʼ������--����ϵͳר��
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pStoppointStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע��

***************************/
static void STOPPOINT_STRU_init_CC(UINT8 * pDataAdd,DSU_STOPPOINT_STRU * pStoppointStru)
{
	UINT8 * pData = pDataAdd;
	pStoppointStru->wId = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->wAttribute = LongFromChar(pData);
    pData += 4;

	pStoppointStru->wDir = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->wLinkId = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->dwLinkOfst = LongFromChar(pData);
    pData += 4;

	pStoppointStru->dwATOParkArea = LongFromChar(pData);
    pData += 4;

	pStoppointStru->dwATPParkArea = LongFromChar(pData);
    pData += 4;
	pStoppointStru->dwATPParkArea = 150;
	pStoppointStru->wStationID = ShortFromChar(pData);
    pData += 2;
	
}

/**************************************
�������ܣ�ͣ�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pStoppointStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ����������
	UINT16  wMinStopTime;					��Сͣվʱ��
	UINT16  wMaxStopTime;					���ͣվʱ��
	UINT16  wStopTime;						Ĭ��ͣվʱ��

***************************/
static void STOPPOINT_STRU_init_DFT(UINT8 * pDataAdd,DSU_STOPPOINT_STRU * pStoppointStru)
{
	UINT8 * pData = pDataAdd;
	pStoppointStru->wId = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->wAttribute = LongFromChar(pData);
    pData += 4;

	pStoppointStru->wDir = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->wLinkId = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->dwLinkOfst = LongFromChar(pData);
    pData += 4;

	pStoppointStru->wOverLapId = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->dwATOParkArea = LongFromChar(pData);
    pData += 4;

	pStoppointStru->dwATPParkArea = LongFromChar(pData);
    pData += 4;

	pStoppointStru->wStationID = ShortFromChar(pData);
    pData += 2;

	pStoppointStru->wStationDoorID = *pData;
	pData += 1;
	
}

/*��ע�����ݻ�����ͨ���ݽṹV12.0.0�޸ģ�modify by lmy 20180312*/
/**************************************
�������ܣ�ͣ�������ݽṹ���ʼ������-�����ӳ���ϵͳ �޸ĺ�������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pStoppointStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ����������
	UINT16  wMinStopTime;					��Сͣվʱ��
	UINT16  wMaxStopTime;					���ͣվʱ��
	UINT16  wStopTime;						Ĭ��ͣվʱ��

***************************/
void STOPPOINT_STRU_init(UINT8 * pDataAdd,DSU_STOPPOINT_STRU * pStoppointStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType)
		|| (DQU_CBTC_HLT == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType) )
	{
		STOPPOINT_STRU_init_DFT(pDataAdd, pStoppointStru);
	} 
	else if (DQU_CBTC_CC == cbtcVersionType)
	{
		STOPPOINT_STRU_init_CC(pDataAdd, pStoppointStru);
	}
	else
	{
		/*���ɴ��֧*/
	}
}

/**************************************
�������ܣ������������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pAxleSgmtStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ����������
	UINT16  wMiddleLinkId;							���������м����link���
	UINT32 dwOrgnLinkOfst;							�������linkƫ����
	UINT32 dwTmnlLinkOfst;							�յ�����linkƫ����
	UINT16  wOrgnRelatPointId;					��������������������
	UINT16  wOrgnMainAxleSgmtId;					��������������Ӷ�λ�������α��
	UINT16  wOrgnSideAxleSgmtId;					��������������ӷ�λ�������α��
	UINT16  wTmnlRelatPointId;					���������յ����������
	UINT16  wTmnlMainAxleSgmtId;					���������յ����Ӷ�λ�������α��
	UINT16  wTmnlSideAxleSgmtId;					���������յ����ӷ�λ�������α��
	UINT16  wRelatAxleSgmtNum;					��������������Ŀ
	UINT16  wRelatAxleSgmtId[3];					�����������α��
	UINT16  wManageZC;							����ZC������
	UINT16  wManagePhysicalCI;					��������CI������

***************************/
void AXLE_SGMT_STRU_init(UINT8 * pDataAdd,DSU_AXLE_SGMT_STRU * pAxleSgmtStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	/*���*/
	pAxleSgmtStru->wId = ShortFromChar(pData);
    pData += 2;

	/*������������*/
	for(i=0;i<16;i++)
	{
		pAxleSgmtStru->szCaption[i] = *pData;
        pData += 1;
	}

	/*�����������*/
	pAxleSgmtStru->wOrgnAxleId = ShortFromChar(pData);
    pData += 2;

	/*�յ���������*/
	pAxleSgmtStru->wTmnlAxleId = ShortFromChar(pData);
    pData += 2;

	/*�������link���*/
	pAxleSgmtStru->wOrgnLkId = ShortFromChar(pData);
    pData += 2;

	/*�յ�����link���*/
	pAxleSgmtStru->wTmnlLkId = ShortFromChar(pData);
    pData += 2;

	/*�������ΰ������߼����εĸ���*/
	pAxleSgmtStru->wIncludeLogicSgmtNum = ShortFromChar(pData);
    pData += 2;

	/*�������ΰ������߼����εı��*/
	for(i=0;i<20;i++)
	{
		pAxleSgmtStru->wLogicSgmtId[i] = ShortFromChar(pData);
        pData += 2;
	}

	/*����������Ŀ*/
	pAxleSgmtStru->wRelatPointNum = ShortFromChar(pData);
    pData += 2;

	for(i=0;i<4;i++)
	{
		/*����������*/
		pAxleSgmtStru->wRelatPointId[i] = ShortFromChar(pData);
        pData += 2;
	    /*��������״̬*/
		pAxleSgmtStru->wRelatPointStatus[i] = ShortFromChar(pData);
        pData += 2;
	}

	/*����������Ŀ*/
	pAxleSgmtStru->wRelatePhysicalSgmtId = ShortFromChar(pData);
    pData += 2;

}


/**************************************
�������ܣ���·�����ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pRouteStru ����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������
			UINT16  wCBTCAccessLink1RelatePointNum;		CBTC�ӽ�����1����������Ŀ
	UINT16  wCBTCAccessLink1RelatePointId[];	CBTC�ӽ�����1����������	
	UINT16  wCBTCAccessLink1RelatePointSta[];   CBTC�ӽ�����1��������״̬
	UINT16  wCBTCAccessLink2RelatePointNum;		CBTC�ӽ�����2����������Ŀ
	UINT16  wCBTCAccessLink2RelatePointId[];	CBTC�ӽ�����2����������	
	UINT16  wCBTCAccessLink2RelatePointSta[];   CBTC�ӽ�����2��������״̬
	UINT16  wCBTCAccessLink3RelatePointNum;		CBTC�ӽ�����3����������Ŀ	
	UINT16  wCBTCAccessLink3RelatePointId[];	CBTC�ӽ�����3����������	
	UINT16  wCBTCAccessLink3RelatePointSta[];   CBTC�ӽ�����3��������״̬
	UINT16  wCBTCAccessLink4RelatePointNum;		CBTC�ӽ�����4����������Ŀ
	UINT16  wCBTCAccessLink4RelatePointId[];	CBTC�ӽ�����4����������
	UINT16  wCBTCAccessLink4RelatePointSta[];   CBTC�ӽ�����4��������״̬
	UINT16  wIncludeAxleSgmtNum;				��·�����ļ���������Ŀ
	UINT16  wIncludeAxleSgmtId[8];				�����ļ������α��
	UINT16  wAxleSgmtLockStatus[8];				�����ļ�����������״̬
	UINT16  wIncludeLinkNum;					������link��Ŀ
	UINT16  wLinkId[14];						link���
	UINT16  wObsNum;							��·�����ϰ�����
	UINT16  wObsType[8];						�ϰ�������
	UINT16  wObsID[8];							�ϰ���ID
	UINT16	wObsNeedLockStatus[8];			    �ϰ�����Ҫ����״̬
***************************/
static void ROUTE_STRU_init_FAO(UINT8 * pDataAdd,DSU_ROUTE_STRU * pRouteStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	pRouteStru->wId = ShortFromChar(pData);
	pData += 2;

	/*��·����*/
	for(i=0;i<24;i++)
	{
		pRouteStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*��·����*/
	pRouteStru->wRouteType = ShortFromChar(pData);
	pData += 2;

	/*��ʼ�źŻ����*/
	pRouteStru->wStartSignalId = ShortFromChar(pData);
	pData += 2;

	/*��ֹ�źŻ����*/
	pRouteStru->wEndSignalId = ShortFromChar(pData);
	pData += 2;

	/*�������link���*/		/*����---�Ϲ�ѧ*/
	pRouteStru->wOrgnLkId = ShortFromChar(pData);
	pData += 2;

	/*�յ�����link���*/		/*����---�Ϲ�ѧ*/
	pRouteStru->wTmnlLkId = ShortFromChar(pData);
	pData += 2;

	/*����������Ŀ*/
	pRouteStru->wIncludePointNum = ShortFromChar(pData);
	pData += 2;


	for(i=0;i<16;i++)
	{
		/*������*/
		pRouteStru->wPointId[i] = ShortFromChar(pData);
		pData += 2;

		/*����״̬*/	
		pRouteStru->wPointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*����������Ŀ*/
	pRouteStru->wProtectLinkNum = ShortFromChar(pData);
	pData += 2;


	for(i=0;i<4;i++)
	{
		/*�������α��*/
		pRouteStru->wProtectLinkId[i] = ShortFromChar(pData);
		pData += 2;

		/*�������ι���������*/    
		pRouteStru->wProtectLinkRelatePointId[i] = ShortFromChar(pData);
		pData += 2;

		/*�������ι�������״̬*/
		pRouteStru->wProtectLinkRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*��ʽ�ӽ�������Ŀ*/					
	pRouteStru->wBlogAccessLinkNum = ShortFromChar(pData);
	pData += 2;

	/*��ʽ�ӽ����α��,��ԺϿ����ݽṹV16.0.0��FAO��HLTҲ���������ӽ����α�����ӡ�5����by my 20170220*/			
	for(i=0;i<5;i++)
	{
		pRouteStru->wBlogAccessLinkId[i] = ShortFromChar(pData);
		pData += 2;
	} 

	/*CBTC�ӽ�������Ŀ*/					
	pRouteStru-> wCBTCAccessLinkNum = ShortFromChar(pData);
	pData += 2;

	/*CBTC�ӽ����α��,��ԺϿ����ݽṹV16.0.0��FAO��HLTҲ���������ӽ����α�����ӡ�5����by my 20170220*/				
	for(i=0;i<5;i++)
	{
		pRouteStru->wCBTCAccessLinkId[i] = ShortFromChar(pData);
		pData += 2;
	} 

	/*����CI������*/						
	pRouteStru->wManageCI = ShortFromChar(pData);
	pData += 2;
	/*��·����*/						
	pRouteStru->wAttribute = ShortFromChar(pData);
	pData += 2;

	/*��ʽ������������	*/					
	pRouteStru->wBlocTrgAreaNum = ShortFromChar(pData);
	pData += 2;

	/*��ʽ��������ID*/
	for(i=0;i<13;i++)
	{
		pRouteStru->wBlocTrgAreaId[i] = ShortFromChar(pData);
		pData += 2;
	}
	/*cbtc������������	*/					
	pRouteStru->wCbtcTrgAreaNum = ShortFromChar(pData);
	pData += 2;

	/*cbtc��������ID*/
	for(i=0;i<13;i++)
	{
		pRouteStru->wCbtcTrgAreaId[i] = ShortFromChar(pData);
		pData += 2;
	}
}

/**************************************
�������ܣ���·�����ݽṹ���ʼ������-������ͨ
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pRouteStru ����ʼ���Ľṹ��ָ��
����ֵ����
�޸�˵����
���������ݽṹ�仯����·�����ĵ�ʽ�ӽ����Ρ�CBTC�ӽ����Ρ���ʽ�������Ρ�CBTC��������������������Ϊ0-30�Ķ�̬��Χ by sds 2019-5-15
***************************/
static void ROUTE_STRU_init_HLT(UINT8 * pDataAdd,DSU_ROUTE_STRU * pRouteStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	static UINT8 runCount=0;/*ִ�иú����Ĵ��� �����ж��Ƿ�Ϊ��һ�ν���*/
	static UINT8 maxBlocAccessLinkNum=MAX_BLOC_ACCESS_LINK_NUM;/*��·������ʵ�ʵ�ʽ�ӽ������������*/
	static UINT8 maxCBTCAccessLinkNum=MAX_BLOC_ACCESS_LINK_NUM;/*��·������ʵ��CBTC�ӽ������������*/
	static UINT8 maxBlocTRGAreLinkNum=MAX_BLOC_ACCESS_LINK_NUM;/*��·������ʵ�ʵ�ʽ���������������*/

	/*��·ID*/
	pRouteStru->wId = ShortFromChar(pData);
	pData += 2;

	/*��·����*/
	for(i=0;i<24;i++)
	{
		pRouteStru->szCaption[i] = *pData;
		pData += 1;
	}

	/*��·����*/
	pRouteStru->wRouteType = ShortFromChar(pData);
	pData += 2;

	/*��ʼ�źŻ����*/
	pRouteStru->wStartSignalId = ShortFromChar(pData);
	pData += 2;

	/*��ֹ�źŻ����*/
	pRouteStru->wEndSignalId = ShortFromChar(pData);
	pData += 2;

	/*�������link���*/		/*����---�Ϲ�ѧ*/
	pRouteStru->wOrgnLkId = ShortFromChar(pData);
	pData += 2;

	/*�յ�����link���*/		/*����---�Ϲ�ѧ*/
	pRouteStru->wTmnlLkId = ShortFromChar(pData);
	pData += 2;

	/*����������Ŀ*/
	pRouteStru->wIncludePointNum = ShortFromChar(pData);
	pData += 2;


	for(i=0;i<16;i++)
	{
		/*������*/
		pRouteStru->wPointId[i] = ShortFromChar(pData);
		pData += 2;

		/*����״̬*/	
		pRouteStru->wPointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*����������Ŀ*/
	pRouteStru->wProtectLinkNum = ShortFromChar(pData);
	pData += 2;


	for(i=0;i<4;i++)
	{
		/*�������α��*/
		pRouteStru->wProtectLinkId[i] = ShortFromChar(pData);
		pData += 2;

		/*�������ι���������*/    
		pRouteStru->wProtectLinkRelatePointId[i] = ShortFromChar(pData);
		pData += 2;

		/*�������ι�������״̬*/
		pRouteStru->wProtectLinkRelatePointStatus[i] = ShortFromChar(pData);
		pData += 2;
	}


	/* ������"��ʽ�ӽ�������Ŀ"��"CBTC�ӽ�������Ŀ"��"��ʽ������������"��"cbtc������������" Ϊ�̶�ֵ,
		����һ�����ݱ����������Ӿ�Ҫ�޸Ĵ���,Ϊ�Ż��������޸Ľṹ������߼�, by sds  2019-5-16 */
 	
	/*�ж��Ƿ��һ��ִ��  ��һ��ִ��ʱ�����������������   ��������ʱ���õ�һ�μ���ĳɹ�*/
	if(0==runCount)
	{
		/* ��ȡ��ʽ�ӽ�������Ŀ */					
		pRouteStru->wBlogAccessLinkNum = ShortFromChar(pData);
		maxBlocAccessLinkNum= ShortFromChar(pData);
		pData += 2;

		/* ���ݻ�ȡ���ĵ�ʽ�ӽ�������Ŀ����������� */	
		for(i=0; i<pRouteStru->wBlogAccessLinkNum; i++)
		{
			pRouteStru->wBlogAccessLinkId[i] = ShortFromChar(pData);
			pData += 2;
		} 
		 
		/*��ȡCBTC�ӽ�������Ŀ*/	
		for(i=0; i<MAX_BLOC_ACCESS_LINK_NUM; i++)
		{
			pRouteStru-> wCBTCAccessLinkNum = ShortFromChar(pData); 
			pData += 2;

			/* ���϶�ȡ���ݣ�������Чֵ��ֱ����ȡ����ȷ��ֵʱ���˳� */
			if((pRouteStru-> wCBTCAccessLinkNum != DSU_NULL_16) 
				&& (pRouteStru-> wCBTCAccessLinkNum <= MAX_CBTC_ACCESS_LINK_NUM))
			{
				/*��·��CBTC�ӽ�������Ч������ֵ*/
				maxCBTCAccessLinkNum=pRouteStru-> wCBTCAccessLinkNum;
				break;
			}
			else
			{	
				/*��·�ĵ�ʽ�ӽ�����������+1*/
				maxBlocAccessLinkNum++;
			}
		}

		/* ���ݻ�ȡ����CBTC�ӽ�������Ŀ����������� */	
		for(i=0;i<pRouteStru-> wCBTCAccessLinkNum;i++)
		{
			pRouteStru->wCBTCAccessLinkId[i] = ShortFromChar(pData);
			pData += 2;
		} 

		/*����CI������*/	
		for(i=0; i<MAX_CBTC_ACCESS_LINK_NUM; i++)
		{					
			pRouteStru->wManageCI = ShortFromChar(pData);
			pData += 2;

			/* ���϶�ȡ���ݣ�������Чֵ��ֱ����ȡ����ȷ��ֵʱ���˳� */
			if (pRouteStru->wManageCI != DSU_NULL_16)
			{
				break;
			}
			else
			{	
				/*��·��CBTC�ӽ�����������+1*/
				maxCBTCAccessLinkNum++;
			}
		}

		/*��·����*/						
		pRouteStru->wAttribute = ShortFromChar(pData);
		pData += 2;
	 

		/*��ʽ������������	*/					
		pRouteStru->wBlocTrgAreaNum = ShortFromChar(pData);
		maxBlocTRGAreLinkNum= ShortFromChar(pData);
		pData += 2;

		/*��ʽ��������ID*/
		for(i=0;i<pRouteStru->wBlocTrgAreaNum;i++)
		{
			pRouteStru->wBlocTrgAreaId[i] = ShortFromChar(pData);
			pData += 2;
		}

		/*cbtc������������	*/	
		for(i=0; i<MAX_BLOC_TRG_AREA_NUM; i++)
		{
			pRouteStru->wCbtcTrgAreaNum = ShortFromChar(pData);
			pData += 2;

			/* ���϶�ȡ���ݣ�������Чֵ��ֱ����ȡ����ȷ��ֵʱ���˳� */
			if((pRouteStru->wCbtcTrgAreaNum != DSU_NULL_16)
				&& (pRouteStru-> wCbtcTrgAreaNum <= MAX_CBTC_TRG_AREA_NUM))
			{
				break;
			}
			else
			{	
				/*��·�ĵ�ʽ��������������+1*/
				maxBlocTRGAreLinkNum++;
			}
		}

		/*cbtc��������ID*/
		for(i=0;i<pRouteStru->wCbtcTrgAreaNum;i++)
		{
			pRouteStru->wCbtcTrgAreaId[i] = ShortFromChar(pData);
			pData += 2;
		}

		/*ִ�д���+1*/
		runCount++;
	}
	else
	{

		/* ��ȡ��ʽ�ӽ�������Ŀ */					
		pRouteStru->wBlogAccessLinkNum = ShortFromChar(pData);
		pData += 2;

		/* ���ݻ�ȡ���ĵ�ʽ�ӽ�������Ŀ����������� */	
		for(i=0; i<maxBlocAccessLinkNum; i++)
		{
			pRouteStru->wBlogAccessLinkId[i] = ShortFromChar(pData);
			pData += 2;
		} 

		/*��ȡCBTC�ӽ�������Ŀ*/	
		pRouteStru-> wCBTCAccessLinkNum = ShortFromChar(pData); 
		pData += 2;

		/* ���ݻ�ȡ����CBTC�ӽ�������Ŀ����������� */	
		for(i=0;i<maxCBTCAccessLinkNum;i++)
		{
			pRouteStru->wCBTCAccessLinkId[i] = ShortFromChar(pData);
			pData += 2;
		} 

		/*����CI������*/	
		pRouteStru->wManageCI = ShortFromChar(pData);
		pData += 2;

		/*��·����*/						
		pRouteStru->wAttribute = ShortFromChar(pData);
		pData += 2;


		/*��ʽ������������	*/					
		pRouteStru->wBlocTrgAreaNum = ShortFromChar(pData);
		pData += 2;

		/*��ʽ��������ID*/
		for(i=0;i<maxBlocTRGAreLinkNum;i++)
		{
			pRouteStru->wBlocTrgAreaId[i] = ShortFromChar(pData);
			pData += 2;
		}

		/*cbtc������������	*/	
		pRouteStru->wCbtcTrgAreaNum = ShortFromChar(pData);
		pData += 2;

		/*cbtc��������ID-����ʵ�������������ݻ�ȡ*/
		for(i=0;i<pRouteStru->wCbtcTrgAreaNum;i++)
		{
			pRouteStru->wCbtcTrgAreaId[i] = ShortFromChar(pData);
			pData += 2;
		}

	}

}

static void ROUTE_STRU_init_CPK(UINT8 * pDataAdd,DSU_ROUTE_STRU * pRouteStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	pRouteStru->wId = ShortFromChar(pData);
    pData += 2;

   /*��·����*/
	for(i=0;i<24;i++)
	{
		pRouteStru->szCaption[i] = *pData;
       pData += 1;
	}

    /*��·����*/
	pRouteStru->wRouteType = ShortFromChar(pData);
    pData += 2;

	/*��ʼ�źŻ����*/
	pRouteStru->wStartSignalId = ShortFromChar(pData);
    pData += 2;

    /*��ֹ�źŻ����*/
	pRouteStru->wEndSignalId = ShortFromChar(pData);
    pData += 2;

    /*�������link���*/		/*����---�Ϲ�ѧ*/
	pRouteStru->wOrgnLkId = ShortFromChar(pData);
    pData += 2;

    /*�յ�����link���*/		/*����---�Ϲ�ѧ*/
	pRouteStru->wTmnlLkId = ShortFromChar(pData);
    pData += 2;

    /*����������Ŀ*/
	pRouteStru->wIncludePointNum = ShortFromChar(pData);
    pData += 2;

    
	for(i=0;i<16;i++)
	{
		/*������*/
		pRouteStru->wPointId[i] = ShortFromChar(pData);
        pData += 2;

        /*����״̬*/	
		pRouteStru->wPointStatus[i] = ShortFromChar(pData);
        pData += 2;
	}

    /*����������Ŀ*/
	pRouteStru->wProtectLinkNum = ShortFromChar(pData);
    pData += 2;

															
   for(i=0;i<4;i++)
	{
		/*�������α��*/
		pRouteStru->wProtectLinkId[i] = ShortFromChar(pData);
        pData += 2;
	
       /*�������ι���������*/    
		pRouteStru->wProtectLinkRelatePointId[i] = ShortFromChar(pData);
        pData += 2;

        /*�������ι�������״̬*/
		pRouteStru->wProtectLinkRelatePointStatus[i] = ShortFromChar(pData);
        pData += 2;
	}

    /*��ʽ�ӽ�������Ŀ*/					
	pRouteStru->wBlogAccessLinkNum = ShortFromChar(pData);
    pData += 2;

   /*��ʽ�ӽ����α��,��ԺϿ����ݽṹV16.0.0��FAO��HLTҲ���������ӽ����α�����ӡ�5����by my 20170220*/			
	 for(i=0;i<5;i++)
	{
		pRouteStru->wBlogAccessLinkId[i] = ShortFromChar(pData);
        pData += 2;
	} 

  /*CBTC�ӽ�������Ŀ*/					
	pRouteStru-> wCBTCAccessLinkNum = ShortFromChar(pData);
    pData += 2;

    /*CBTC�ӽ����α��,��ԺϿ����ݽṹV16.0.0��FAO��HLTҲ���������ӽ����α�����ӡ�5����by my 20170220*/				
     for(i=0;i<5;i++)
	{
		pRouteStru->wCBTCAccessLinkId[i] = ShortFromChar(pData);
        pData += 2;
	} 

	/*����CI������*/						
	pRouteStru->wManageCI = ShortFromChar(pData);
    pData += 2;
	/*��·����*/						
	pRouteStru->wAttribute = ShortFromChar(pData);
    pData += 2;

	/*��ʽ������������	*/					
	pRouteStru->wBlocTrgAreaNum = ShortFromChar(pData);
    pData += 2;

	/*��ʽ��������ID*/
	for(i=0;i<4;i++)
	{
		pRouteStru->wBlocTrgAreaId[i] = ShortFromChar(pData);
		pData += 2;
	}
	/*cbtc������������	*/					
	pRouteStru->wCbtcTrgAreaNum = ShortFromChar(pData);
    pData += 2;

	/*cbtc��������ID*/
	for(i=0;i<4;i++)
	{
		pRouteStru->wCbtcTrgAreaId[i] = ShortFromChar(pData);
		pData += 2;
	}
}

/*���ýӿ�
��ע��
�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726
���޸Ļ�����ͨ��Ŀ���ݽṹ��·�����߼������ݶԽ�·�ڰ����ĵ�ʽ/CBTC�ӽ����Σ���ʽ/CBTC�������ζ�̬�����Ĵ���m by sds 2019-5-16*/
void ROUTE_STRU_init(UINT8 *pDataAdd,DSU_ROUTE_STRU *pRouteStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType) )
	{
		ROUTE_STRU_init_FAO(pDataAdd, pRouteStru);
	} 
	else if (DQU_CBTC_CPK == cbtcVersionType)
	{
		ROUTE_STRU_init_CPK(pDataAdd, pRouteStru);
	}
	else if(DQU_CBTC_HLT == cbtcVersionType)
	{
		ROUTE_STRU_init_HLT(pDataAdd, pRouteStru);
	}
	else
	{
      /*���ɴ��֧*/
	}
}

/**************************************
�������ܣ��߼��������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pProtectLinkStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������
	  UINT16  wRouteId;							��·���
	  UINT16  wIncludeAxleSgmtId;					�����ļ������α��
	  UINT16  wObsNum;							�����ϰ�����Ŀ
	  UINT16  wObsType;							�ϰ�������	
***************************/

void LOGIC_SGMT_STRU_init(UINT8 * pDataAdd,DSU_LOGIC_SGMT_STRU * pLogicSgmtStru)
{
	UINT8 * pData = pDataAdd;
	UINT32 i = 0;
	/*���*/
	pLogicSgmtStru->wId = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<16;i++)
	{
		/*�߼���������*/
		pLogicSgmtStru->szCaption[i] = *pData;
		pData++;
	}
	
	/*�������link���*/
	pLogicSgmtStru->wOrgnLkId = ShortFromChar(pData);
	pData += 2;
		
  /*�������linkƫ����(cm)*/
	pLogicSgmtStru->dwOrgnLkOfst = LongFromChar(pData);
	pData += 4;

		/*�յ�����link���*/
	pLogicSgmtStru->wTmnlLkId = ShortFromChar(pData);
	pData += 2;
		
  /*�յ�����linkƫ����(cm)*/
	pLogicSgmtStru->dwTmnlLkOfst = LongFromChar(pData);
	pData += 4;

	/*�����������α��*/
	pLogicSgmtStru->wAxleSgmtId = ShortFromChar(pData);
	pData += 2;

	/*ZC-CI�߼�����ռ��״̬��λ
	pLogicSgmtStru->wZcCiLogicStatusId = ShortFromChar(pData);
	pData += 2;*/
	
	/*ZC-CI�߼�����CT�г�ռ��״̬��λ��Ԥ����
	pLogicSgmtStru->wZcCiCTLogicStatusId = ShortFromChar(pData);
	pData += 2;*/

	/*ZC-CI�߼�����UT�г�ռ��״̬��λ��Ԥ����
	pLogicSgmtStru->wZcCiUTLogicStatusId = ShortFromChar(pData);
	pData += 2;*/
}

/**************************************
�������ܣ���ʽ�ӽ�������Ϣ���ݽṹ���ʼ������(ԭBLOCģʽ�ӽ����α�)
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pBlocAccessLinkStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������

***************************/
void BLOC_ACCESS_LINK_STRU_init(UINT8 * pDataAdd,DSU_BLOC_ACCESS_LINK_STRU * pBlocAccessLinkStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*���*/
	pBlocAccessLinkStru->wId = ShortFromChar(pData);
    pData += 2;

	/*�����ļ���������Ŀ*/
	pBlocAccessLinkStru->wIncludeAxleSgmtNum = ShortFromChar(pData);
    pData += 2;
	
    /*������������Ϣ*/
	for(i=0;i<(pBlocAccessLinkStru->wIncludeAxleSgmtNum);i++)
	{
		pBlocAccessLinkStru->wIncludeAxleSgmtId[i] = ShortFromChar(pData);
        pData += 2;
	}
}

/**************************************
�������ܣ�CBTC�ӽ�������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out] pCBTCAccessLinkStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������

***************************/
void CBTC_ACCESS_LINK_STRU_init(UINT8 *pDataAdd, DSU_CBTC_ACCESS_LINK_STRU *pCBTCAccessLinkStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*���*/
	pCBTCAccessLinkStru->wId = ShortFromChar(pData);
	pData += 2;

	/*�����ļ���������Ŀ*/
	pCBTCAccessLinkStru->wIncludeLogicSgmtNum = ShortFromChar(pData);
	pData += 2;
	
	/*�����ļ������α��*/
	for(i=0;i<(pCBTCAccessLinkStru->wIncludeLogicSgmtNum);i++)
	{
		pCBTCAccessLinkStru->wIncludeLogicSgmtId[i] = ShortFromChar(pData);
		pData += 2;
	}
}

/**************************************
�������ܣ��¶����ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pGradeStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�������ӣ�
UINT32  dwGradeLength;							�¶ȳ���
UINT16  wIncludeLinkId[3];							�¶���·�м����link���


***************************/
void GRADE_STRU_init(UINT8 * pDataAdd,DSU_GRADE_STRU * pGradeStru)
{
	UINT8 * pData = pDataAdd;
	/*���*/
	pGradeStru->wId = ShortFromChar(pData);
    pData += 2;
	
	/*�¶��������link���*/
	pGradeStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;	

	/*�¶��������linkƫ����*/
    pGradeStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;		
	
	/*�¶��յ�����link���*/
	pGradeStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;							
									
	/*�¶��յ�����linkƫ����*/
	pGradeStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;	

	/*������������*/
	pGradeStru->wOrgnRelatePointId = ShortFromChar(pData);
    pData += 2;	

	/*��������¶ȱ��*/
	pGradeStru->wOrgnJointMainGradeId = ShortFromChar(pData);
    pData += 2;

	/*�������¶ȱ��*/
	pGradeStru->wOrgnJointSideGradeId = ShortFromChar(pData);
    pData += 2;

	/*�յ����������*/
    pGradeStru->wTmnlRelatePointId  = ShortFromChar(pData);
    pData += 2;
					
	/*�յ������¶ȱ��*/
	pGradeStru->wTmnlJointMainGradeId  = ShortFromChar(pData);
    pData += 2;
						
	/*�յ�����¶ȱ��*/
	pGradeStru->wTmnlJointSideGradeId = ShortFromChar(pData);
    pData += 2;
				
	/*�¶�ֵ*/
	pGradeStru->wGradValue = ShortFromChar(pData);
    pData += 2;
							
	/*�¶��������·�߼��������б����*/
    pGradeStru->wInclineDir = ShortFromChar(pData);
    pData += 2;

   /*�����߰뾶*/		/*����---�Ϲ�ѧ*/
	pGradeStru->dwRadius = LongFromChar(pData);
    pData += 4;								
}
/**************************************
�������ܣ���·��̬�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pStaticResSpeedStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���޸���

***************************/
void STATIC_RES_SPEED_STRU_init(UINT8 * pDataAdd,DSU_STATIC_RES_SPEED_STRU * pStaticResSpeedStru)
{
	UINT8 * pData = pDataAdd;
	/*���*/
	pStaticResSpeedStru->wId = ShortFromChar(pData);
    pData += 2;

    /*��������������link���*/
	pStaticResSpeedStru->wLinkId = ShortFromChar(pData);
    pData += 2;							
					
	/*�������linkƫ����(cm)*/
	pStaticResSpeedStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;
					
	/*�յ�����linkƫ����(cm)*/
    pStaticResSpeedStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;
				
	/*����������*/
    pStaticResSpeedStru->wRelatePointId = ShortFromChar(pData);
    pData += 2;
					
	/*��̬����ֵ*/
	pStaticResSpeedStru->wResSpeed = ShortFromChar(pData);
    pData += 2;					
}
/**************************************
�������ܣ�Ĭ���г��������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pDefaultrouteStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�������
	UINT16  wCurrentPlatformId;				��ǰվ̨���
	UINT16  wCurrentPlatformFlag;			��ǰվ̨��־λ

***************************/
void DEFAULTROUTE_STRU_init(UINT8 * pDataAdd,DSU_DEFAULTROUTE_STRU * pDefaultrouteStru)
{
	UINT8 * pData = pDataAdd;
	/*���*/
	pDefaultrouteStru->wId = ShortFromChar(pData);
    pData += 2;					

	/*link���*/
	pDefaultrouteStru->wLinkId = ShortFromChar(pData);
    pData += 2;	

	/*���з���*/
    pDefaultrouteStru->wLinkDir = ShortFromChar(pData);
    pData += 2;	
						
	/*ǰ��վ̨���*/
    pDefaultrouteStru->wNextPlatformId = ShortFromChar(pData);
    pData += 2;

	/*Ŀ��վ̨���*/
	pDefaultrouteStru->wDestinationId = ShortFromChar(pData);
    pData += 2;		
}
/**************************************
�������ܣ�վ����Ӫ�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pRunLevelStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������


***************************/
static void RUN_LEVEL_STRU_init_CPK(UINT8 * pDataAdd,DSU_RUN_LEVEL_STRU * pRunLevelStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	/*�������*/
	pRunLevelStru->wId = ShortFromChar(pData);
    pData += 2;		
												
	/*���Ŀ�ĵر��*/
    pRunLevelStru->wOrgnDesId = ShortFromChar(pData);
    pData += 2;	
					
	/*�յ�Ŀ�ĵر��*/
    pRunLevelStru->wTmnlDesId = ShortFromChar(pData);
    pData += 2;
									
	/*���м���ּ���Ŀ*/		/*����---�Ϲ�ѧ*/
    pRunLevelStru->wRunLevelNum = ShortFromChar(pData);
    pData += 2;
				
	
	for(i=0;i<10;i++)
	{
		/*��Ӫ������*/
		pRunLevelStru->wRunLevelID[i] = ShortFromChar(pData);
        pData += 2;
						
	   /*����Ӫ����ʱ��*/
		pRunLevelStru->wRunTime[i] = ShortFromChar(pData);
        pData += 2;
							
        /*����Ӫ�����ٶ�*/
		pRunLevelStru->wRunSpeed[i] = ShortFromChar(pData);
        pData += 2;
	}			 			
}

/**************************************
�������ܣ�վ����Ӫ�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pRunLevelStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע��FAO��HLT����
***************************/
static void RUN_LEVEL_STRU_init_DFT(UINT8 *pDataAdd,DSU_RUN_LEVEL_STRU *pRunLevelStru)
{
	UINT8 *pData = pDataAdd;
	UINT8 i=0;
	
	/*�������*/
	pRunLevelStru->wId = ShortFromChar(pData);
	pData += 2;		

	/*���Ŀ�ĵر��*/
	pRunLevelStru->wOrgnDesId = ShortFromChar(pData);
	pData += 2;	

	/*�յ�Ŀ�ĵر��*/
	pRunLevelStru->wTmnlDesId = ShortFromChar(pData);
	pData += 2;

	/*���м���ּ���Ŀ*/		/*����---�Ϲ�ѧ*/
	pRunLevelStru->wRunLevelNum = ShortFromChar(pData);
	pData += 2;


	for(i=0;i<5;i++)
	{
		/*��Ӫ������*/
		pRunLevelStru->wRunLevelID[i] = ShortFromChar(pData);
		pData += 2;

		/*����Ӫ����ʱ��*/
		pRunLevelStru->wRunTime[i] = ShortFromChar(pData);
		pData += 2;

		/*����Ӫ�����ٶ�*/
		pRunLevelStru->wRunSpeed[i] = ShortFromChar(pData);
		pData += 2;
	}	
}

/**************************************
�������ܣ�վ����Ӫ�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pRunLevelStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ýӿ�
���ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726
***************************/
void RUN_LEVEL_STRU_init(UINT8 *pDataAdd,DSU_RUN_LEVEL_STRU *pRunLevelStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType) ||(DQU_CBTC_DG == cbtcVersionType)||(DQU_CBTC_CC == cbtcVersionType))
	{
		RUN_LEVEL_STRU_init_DFT(pDataAdd, pRunLevelStru);
	} 
	else if (DQU_CBTC_CPK == cbtcVersionType )
	{
		RUN_LEVEL_STRU_init_CPK(pDataAdd, pRunLevelStru);
	}
	else
	{
		/*���ɴ��֧*/
	}
}

/**************************************
�������ܣ��˳�CBTC�������ݽṹ���ʼ������(ԭ�˳���·����)
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pExitRouteStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������


***************************/
void EXIT_ROUTE_STRU_init(UINT8 * pDataAdd,DSU_EXIT_ROUTE_STRU * pExitRouteStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;
	/*�������*/
	pExitRouteStru->wId = ShortFromChar(pData);
    pData += 2;
												
	/*�������link���*/
    pExitRouteStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;
					
	/*���linkƫ����(cm)*/
    pExitRouteStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;

	/*�յ�����link���*/
    pExitRouteStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;
						
	/*�յ�linkƫ����(cm)*/
    pExitRouteStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;
								
	/*�˳�CBTC����ʱ�г����з���*/
	pExitRouteStru->wDIR = ShortFromChar(pData);
    pData += 2;	

	/*�˳�CBTC����ʱ�г����з���*/
	pExitRouteStru->wType = ShortFromChar(pData);
    pData += 2;

	/*������������*/
	pExitRouteStru->wIncludePointNum=ShortFromChar(pData);
	pData+=2;

	for (i=0;i<10;i++)
	{
		/*��������id*/
		pExitRouteStru->wPointId[i]=ShortFromChar(pData);
		pData+=2;

		/*��������״̬*/
		pExitRouteStru->wPointStatus[i]=ShortFromChar(pData);
		pData+=2;
	}
}
/**************************************
�������ܣ���CBTC�������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pTrainLineStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���������

***************************/
void TRAIN_LINE_STRU_init(UINT8 * pDataAdd,DSU_TRAIN_LINE_STRU * pTrainLineStru)
{
	UINT8 * pData = pDataAdd;
	/*���*/
	pTrainLineStru->wId= ShortFromChar(pData);
    pData += 2;			 
										
	/*�������link���*/
    pTrainLineStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;		

	/*���linkƫ����(cm)*/
    pTrainLineStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;

	/*�յ�����link���*/
    pTrainLineStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;	
							
	/*�յ�linkƫ����(cm)*/
    pTrainLineStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;

	/*��CBTC�����г����з���*/
    pTrainLineStru->wDir = ShortFromChar(pData);
    pData += 2;			 					
}
/**************************************
�������ܣ������۷������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pARStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ����ԭ���������۷�����������۷��˳���ϲ�����

***************************/
void AR_STRU_init(UINT8 * pDataAdd,DSU_AR_STRU * pARStru)
{
	UINT8 * pData = pDataAdd;
	/*�������*/
	pARStru->wId = ShortFromChar(pData);
    pData += 2;			 
											
	/*�����۷�����վ̨���*/
	pARStru->wStartPlartformId = ShortFromChar(pData);
    pData += 2;	
				
	/*�����۷��������*/
	pARStru->wARInStopPointId = ShortFromChar(pData);
    pData += 2;	
								
							
	/*�����۷����˵���*/
    pARStru->wRevertPointId = ShortFromChar(pData);
    pData += 2;
		
	/*�����۷��˳�վ̨���*/
    pARStru->wEndPlartformId = ShortFromChar(pData);
    pData += 2;

	/*�����۷��˳�����*/
    pARStru->wAROutStopPointId = ShortFromChar(pData);
    pData += 2;

			
	/*�����۷���ID*/
    pARStru->wARLampID = ShortFromChar(pData);
    pData += 2;
		
	/*�����۷���ťD*/
    pARStru->wARButtonId = ShortFromChar(pData);
    pData += 2;

	
		
}
/**************************************
�������ܣ�ZC�����������ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pZCComZoneStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע��ZC���������    ������ǰ����zc�ֽ���

***************************/
void ZC_COM_ZONE_STRU_init(UINT8 * pDataAdd,DSU_ZC_COM_ZONE_STRU * pZCComZoneStru)
{
	UINT8 * pData = pDataAdd;
	UINT8 i=0;

	/*�������*/ 
	pZCComZoneStru->wId = ShortFromChar(pData);
    pData += 2;				
	                                    
	/*���������������link���*/
    pZCComZoneStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;

	  /*���������������linkƫ����*/  
	pZCComZoneStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;
	               
	/*���������յ�����link���*/ 
    pZCComZoneStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;
	                      
	/*���������յ�����linkƫ����*/
   pZCComZoneStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;
	
	/*���������л�����ؽ�·���*/
	pZCComZoneStru->wRelateRouteID = ShortFromChar(pData);
    pData += 2;

	                
	/*���������ڰ����ĵ�����Ŀ*/
    pZCComZoneStru->wIncludePointNum = ShortFromChar(pData);
    pData += 2;
	                           
	
	for(i=0;i<8;i++)
	{
		/*���������ڰ����ĵ�����*/ 
		pZCComZoneStru->wPointId[i]= ShortFromChar(pData);
        pData += 2;

	   /*���������ڰ����ĵ���״̬*/ 
		pZCComZoneStru->wPointStatus[i]= ShortFromChar(pData);
        pData += 2;
	}           
}
/**************************************
�������ܣ�ZC�ֽ����Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pZCDividPointStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���޸���

***************************/
void ZC_DIVID_POINT_STRU_init(UINT8 * pDataAdd,DSU_ZC_DIVID_POINT_STRU * pZCDividPointStru)
{
	UINT16 j = 0;
	UINT8 * pData = pDataAdd;
	/*�������*/
	pZCDividPointStru->wId = ShortFromChar(pData);
    pData += 2;	

	/*��صķ���ֽ����*/
	pZCDividPointStru->wIdconnection = ShortFromChar(pData);
    pData += 2;	

	/*���ֽ������*/
    pZCDividPointStru->wType = ShortFromChar(pData);
    pData += 2;	
	                                  
	/*�ƽ�ZC�豸���*/
    pZCDividPointStru->wOutputZcIndex = ShortFromChar(pData);
    pData += 2;	
	                    
	/*�ƽ�ZC�豸ID*/ 
    pZCDividPointStru->wOutputZcId = ShortFromChar(pData);
    pData += 2;	
	                        
	/*�ӹ�ZC�豸���*/ 
    pZCDividPointStru->wInputZcIndex = ShortFromChar(pData);
    pData += 2;	
	                      
	/*�ӹ�ZC�豸ID*/
    pZCDividPointStru->wInputZcId = ShortFromChar(pData);
    pData += 2;	
	                              
	/*ZC�ֽ������link���*/
    pZCDividPointStru-> wDividPointLinkId = ShortFromChar(pData);
    pData += 2;	
	              
	/*ZC�ֽ������linkƫ����*/
    pZCDividPointStru->dwDividPointLinkOfst  = LongFromChar(pData);
    pData += 4;
	                       
	/*�г������л���ʱ���з���*/
    pZCDividPointStru->wDir = ShortFromChar(pData);
    pData += 2;		

	/*�ֽ����صĹ���������Ŀ*/
	pZCDividPointStru->wRelateZCAreaNum = ShortFromChar(pData);
    pData += 2;	

	for(j=0;j<RELATE_ZC_AREA_NUM;j++)
	{
		/*�ֽ����صĹ���������*/
		pZCDividPointStru->wRelateZCAreaId[j] = ShortFromChar(pData);
		pData += 2;	
	}
	
}
/**************************************
�������ܣ�CI��Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pCIStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���޸���

***************************/
void CI_STRU_init_DFT(UINT8 * pDataAdd,DSU_CI_STRU * pCIStru)
{
	UINT8 * pData = pDataAdd;
	/*�������*/
	pCIStru->wIndex = ShortFromChar(pData);
    pData += 2;
									
	/*CI�豸ID*/
	pCIStru->wID = ShortFromChar(pData);
    pData += 2;		
	
	/*��������*/
	pCIStru->wType = ShortFromChar(pData);
    pData += 2;	
}

void CI_STRU_init_HLT(UINT8 * pDataAdd,DSU_CI_STRU * pCIStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i=0;

	/*�������*/
	pCIStru->wIndex = ShortFromChar(pData);
	pData += 2;

	/*CI�豸ID*/
	pCIStru->wID = ShortFromChar(pData);
	pData += 2;		

	/*��������*/
	pCIStru->wType = ShortFromChar(pData);
	pData += 2;		

	/*CI-VOBC��������У����Ϣ*/
	pCIStru->dwVersionOfCIToVOBC = LongFromChar(pData);
	pData += 4;	

	/*CI-ZC��������У����Ϣ*/
	pCIStru->dwVersionOfCIToZC = LongFromChar(pData);
	pData += 4;	

	/*CI-ATS��������У����Ϣ*/
	pCIStru->dwVersionOfCIToATS = LongFromChar(pData);
	pData += 4;	

	/*����CI����*/
	pCIStru->dwAdjoinCICount = *pData;
	pData += 1;	

	for(i=0;i<6;i++)
	{
		/*����CI[1-6]��ID*/
		pCIStru->dwAdjoinCIId[i] =ShortFromChar(pData);
		pData += 2;

		/*������CI[1-6]�Ĺ�������У����Ϣ*/
		pCIStru->dwAdjoinCIShareCheckData[i] =LongFromChar(pData);
		pData += 4;

	}

}


/*���ýӿ�*/
/*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726*/
void CI_STRU_init(UINT8 *pDataAdd,DSU_CI_STRU *pCIStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType) )
	{
		CI_STRU_init_DFT(pDataAdd, pCIStru);
	} 
	else if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		CI_STRU_init_HLT(pDataAdd, pCIStru);
	}
	else
	{
       /*���ɴ��֧*/
	}
}


/**************************************
�������ܣ�ATS��Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pATSStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���޸���

***************************/
void ATS_STRU_init_DFT(UINT8 * pDataAdd,DSU_ATS_STRU * pATSStru)
{
	UINT8 * pData = pDataAdd;
	/*�������*/
	pATSStru->wIndex = ShortFromChar(pData);
    pData += 2;
									
	 /*ATS�豸ID*/
	 pATSStru->wID = ShortFromChar(pData);
     pData += 2;	
}

void ATS_STRU_init_HLT(UINT8 * pDataAdd,DSU_ATS_STRU * pATSStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i = 0;

	/*�������*/
	pATSStru->wIndex = ShortFromChar(pData);
	pData += 2;

	/*ATS�豸ID*/
	pATSStru->wID = ShortFromChar(pData);
	pData += 2;	

	/*ATS-VOBC��������У����Ϣ*/
	pATSStru->dwVersionOfATSToVOBC = LongFromChar(pData);
	pData += 4;	

	/*ATS-DSU��������У����Ϣ*/
	pATSStru->dwVersionOfATSToDSU = LongFromChar(pData);
	pData += 4;	

	/*����ATS����*/
	pATSStru->dwAdjoinATSCount = *pData;
	pData += 1;	

	for(i=0;i<6;i++)
	{
		/*����ATS[1-6]��ID*/
		pATSStru->dwAdjoinATSId[i] =ShortFromChar(pData);
		pData += 2;

		/*������ATS[1-6]�Ĺ�������У����Ϣ*/
		pATSStru->dwAdjoinATSShareCheckData[i] =LongFromChar(pData);
		pData += 4;

	}

}


/*���ýӿ�*/
/*��ע�����ӶԵ�����Ŀ��ʶ�Ĵ���modify by qxt 20170726*/
void ATS_STRU_init(UINT8 * pDataAdd,DSU_ATS_STRU * pATSStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_CPK == cbtcVersionType) )
	{
		ATS_STRU_init_DFT(pDataAdd, pATSStru);
	} 
	else if ((DQU_CBTC_HLT == cbtcVersionType)||(DQU_CBTC_DG == cbtcVersionType) )
	{
		ATS_STRU_init_HLT(pDataAdd, pATSStru);
	}
	else
	{
       /*���ɴ��֧*/
	}
}


/**************************************
�������ܣ�����������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pProtectLinkStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ�������ӣ�
	  UINT16  wRouteId;							��·���
	  UINT16  wIncludeAxleSgmtId;			�����ļ������α�ţ�����Ϊ���飩
	  UINT16  wObsNum;							�����ϰ�����Ŀ
	  UINT16  wObsType;							�ϰ�������

***************************/
void PROTECT_LINK_STRU_init(UINT8 * pDataAdd,DSU_PROTECT_LINK_STRU * pProtectLinkStru)
{
	UINT8 * pData = pDataAdd;
	UINT32 i=0;
	/*���*/
	 pProtectLinkStru->wId = ShortFromChar(pData);
     pData += 2;

	 /*�����ļ���������Ŀ*/		/*����---�Ϲ�ѧ*/
	 pProtectLinkStru->wIncludeAxleSgmtNum = ShortFromChar(pData);
     pData += 2;

	 for(i=0;i<4;i++)
	 {
		 /*�����ļ������α��*/		/*����---�Ϲ�ѧ*/  /*����ֶ�ԭ����������*/
	    pProtectLinkStru->wIncludeAxleSgmtId[i] = ShortFromChar(pData);
        pData += 2;
	 }

}

/*��·ͳһ������Ϣ��*/
void CONRESSPEED_STRU_init(UINT8 * pDataAdd, DSU_CONRESSPEED_STRU * pConRESSpeedStru)
{
	UINT8 * pData = pDataAdd;
	/*�������*/
	pConRESSpeedStru->wIndex = ShortFromChar(pData);
    pData += 2;
									
	 /*ATS�豸ID*/
	 pConRESSpeedStru->wSpeed = ShortFromChar(pData);
     pData += 2;	
}  

/*��·ͳһ�¶���Ϣ��*/
void CONGRADE_STRU_init(UINT8 * pDataAdd, DSU_CONGRADE_STRU * pConGradeStru)
{
	UINT8 * pData = pDataAdd;
	/*�������*/
	pConGradeStru->wIndex = ShortFromChar(pData);
    pData += 2;
									
	 /*ATS�豸ID*/
	 pConGradeStru->wGrade = ShortFromChar(pData);
     pData += 2;	
}  



/*����������Ϣ��*/
void PHYSICAL_SGMT_STRU_init(UINT8 * pDataAdd, DSU_PHYSICAL_SGMT_STRU * pPhysicalSgmtStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i = 0;
	/*ID*/
	pPhysicalSgmtStru->wId = ShortFromChar(pData);
    pData += 2;
									
	 /*������������*/
	for(i=0;i<16;i++)
	{
      pPhysicalSgmtStru->szName[i] = *pData;
	   pData ++;
	}
	
	   pPhysicalSgmtStru->Type = *pData;
	   pData ++;

	   pPhysicalSgmtStru->LjkFlag = *pData;
       pData ++;
} 

/**************************************
�������ܣ���ʽ�������α�-��ʼ��
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [in] pBlocTrgAreaStru ����ʼ���Ľṹ��ָ��
����ֵ����
***************************/
void DSU_BLOC_TRG_AREA_STRU_init(UINT8 * pDataAdd, DSU_BLOC_TRG_AREA_STRU * pBlocTrgAreaStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 i = 0;

	/*ID*/
	pBlocTrgAreaStru->wId = ShortFromChar(pData);
    pData += 2;

	pBlocTrgAreaStru->wAxleSgmtNum = ShortFromChar(pData);
    pData += 2;
									
	/*������������Ϣ*/
	for(i=0;i<(pBlocTrgAreaStru->wAxleSgmtNum);i++)
	{
		pBlocTrgAreaStru->wAxleSgmtId[i] = ShortFromChar(pData);
		pData += 2;
	}
} 


/**************************************
�������ܣ�CBTC�������α�-��ʼ��
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [in] pCbtcTrgAreaStru ����ʼ���Ľṹ��ָ��
����ֵ����
***************************/
void DSU_CBTC_TRG_AREA_STRU_init(UINT8 * pDataAdd, DSU_CBTC_TRG_AREA_STRU * pCbtcTrgAreaStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i = 0;

	/*ID*/
	pCbtcTrgAreaStru->wId = ShortFromChar(pData);
    pData += 2;

	pCbtcTrgAreaStru->wLogicSgmtNum = ShortFromChar(pData);
    pData += 2;
 									
	/*������������Ϣ*/
	for(i=0;i<(pCbtcTrgAreaStru->wLogicSgmtNum);i++)
	{
		pCbtcTrgAreaStru->wLogicSgmtId[i] = ShortFromChar(pData);
	    pData += 2;
	}
} 

/*վ̨���α�*/
static void DSU_PLAT_FORM_STRU_init_FAO(UINT8 * pDataAdd, DSU_PLAT_FORM_STRU * pPlatFormStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i = 0;
	/*ID*/
	pPlatFormStru->wId = ShortFromChar(pData);
	pData += 2;


	/*���Ĺ����*/
	for(i=0;i<16;i++)
	{
		pPlatFormStru->szGongLiBiao[i] = *pData;
		pData ++;
	}
	pPlatFormStru->wStationId = ShortFromChar(pData);
	pData += 2;

	pPlatFormStru->wDir = *pData;
	pData ++;

	pPlatFormStru->wLogicSgmtNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<10;i++)
	{
		pPlatFormStru->wLogicSgmtId[i] = ShortFromChar(pData);
		pData += 2;
	}
}

static void DSU_PLAT_FORM_STRU_init_CPK(UINT8 * pDataAdd, DSU_PLAT_FORM_STRU * pPlatFormStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i = 0;
	/*ID*/
	pPlatFormStru->wId = ShortFromChar(pData);
    pData += 2;

								
	 /*������������*/
	for(i=0;i<16;i++)
	{
		pPlatFormStru->szGongLiBiao[i] = *pData;
	   pData ++;
	}
	pPlatFormStru->wStationId = ShortFromChar(pData);
    pData += 2;
	/*����*/		
	pPlatFormStru->wDir = *pData;
	pData ++;

	/*�߼����θ���*/
	pPlatFormStru->wLogicSgmtNum = ShortFromChar(pData);
	pData += 2;

	for(i=0;i<10;i++)
	{
		pPlatFormStru->wLogicSgmtId[i] = ShortFromChar(pData);
		pData += 2;
	}
	
	/*վ̨����*/
	for(i=0;i<48;i++)
	{
		pPlatFormStru->szStationName[i] = *pData;
	   pData ++;
	}
	   
}

/*���ʽӿ�*/
/*��ע�����ݻ�����ͨ���ݽṹV11.0.0�����޸ģ�modify by lmy 20180208*/
void DSU_PLAT_FORM_STRU_init(UINT8 *pDataAdd, DSU_PLAT_FORM_STRU *pPlatFormStru)
{
	UINT8 cbtcVersionType=0;
	cbtcVersionType=GetCbtcSysType();

	if ((DQU_CBTC_FAO == cbtcVersionType) || (DQU_CBTC_DG == cbtcVersionType))
	{
		DSU_PLAT_FORM_STRU_init_FAO(pDataAdd, pPlatFormStru);
	} 
	else if ((DQU_CBTC_CPK == cbtcVersionType) || (DQU_CBTC_HLT == cbtcVersionType)|| (DQU_CBTC_CC == cbtcVersionType))
	{
		DSU_PLAT_FORM_STRU_init_CPK(pDataAdd, pPlatFormStru);
	}
	else
	{
       /*���ɴ��֧*/
	}
}

/*������·��,2013.10.23*/
void DSU_COME_GO_STRU_init(UINT8 * pDataAdd, DSU_COME_GO_STRU * pComeGoStru)
{
    UINT8 * pData = pDataAdd;
    
    /*���*/
    pComeGoStru->wId = ShortFromChar(pData);
    pData += 2;
    
    /*��·1���*/
    pComeGoStru->route1Id = ShortFromChar(pData);
    pData += 2;
    
    /*��·2���*/
    pComeGoStru->route2Id = ShortFromChar(pData);
    pData += 2;
    
    /*��·3���*/
    pComeGoStru->route3Id = ShortFromChar(pData);
    pData += 2;
    
    /*��·4���*/
    pComeGoStru->route4Id = ShortFromChar(pData);
    pData += 2;
    
    /*��Ӧͣ������ID*/
    pComeGoStru->stationId = ShortFromChar(pData);
    pData += 2;
}

/*�����ű�,2014.7.18*/
void DSU_FLOOD_GATE_STRU_init(UINT8 * pDataAdd, DSU_FLOOD_GATE_STRU * pFloodGateStru)
{
	UINT8 * pData = pDataAdd;

	/*���*/
	pFloodGateStru->wId = ShortFromChar(pData);
	pData += 2;

	/*Link��*/
	pFloodGateStru->wFloodGateLinkID = ShortFromChar(pData);
	pData += 2;

	/*ƫ����*/
	pFloodGateStru->dwFloodGateLinkoffset = LongFromChar(pData);
	pData += 4;
	/*��������*/
	pFloodGateStru->wDir = *pData;
	pData += 1;
}

/*SPKS���ر�*/
void DSU_SPKS_BUTTON_STRU_init(UINT8 *pDataAdd, DSU_SPKS_BUTTON_STRU *pSpksButtonStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 i = 0;

	/*���*/
	pSpksButtonStru->wId = ShortFromChar(pData);
	pData += 2;

	pSpksButtonStru->wPhysicalSgmtNum = ShortFromChar(pData);
	pData += 2;

	/*��������ID*/
	for(i=0;i<30;i++)
	{
		pSpksButtonStru->wRelatePhysicalSgmtId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*����CI������*/
	pSpksButtonStru->wManageCI = ShortFromChar(pData); 
	pData += 2;
}

/*�����ű�*/
void DEPOT_GATE_STRU_init(UINT8 *pDataAdd, DSU_DEPOT_GATE_STRU *pDepotGateStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 i = 0;

	/*�����ű��*/
	pDepotGateStru->wId = ShortFromChar(pData);
	pData += 2;

	/*����������Link*/
	pDepotGateStru->wDepotGateLinkID = ShortFromChar(pData);
	pData += 2;

	/*Linkƫ����*/
	pDepotGateStru->dwDepotGateLinkoffset = LongFromChar(pData);
	pData += 4;

	/*�����Ŷ�ӦA�����������α��*/
	pDepotGateStru->wAPhysicalSgmtId = ShortFromChar(pData);
	pData += 2;

	/*�����Ŷ�ӦB�����������α��*/
	pDepotGateStru->wBPhysicalSgmtId = ShortFromChar(pData);
	pData += 2;

	/*����������*/
	pDepotGateStru->sDepotAttribute = ShortFromChar(pData);
	pData += 2;

	/*����·����*/
	pDepotGateStru->wRouteInsideNum = ShortFromChar(pData);
	pData += 2;

	/*����·���*/
	for (i=0;i<10;i++)
	{
		pDepotGateStru->wRouteInsideId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*�����·����*/
	pDepotGateStru->wRouteOutsideNum = ShortFromChar(pData);
	pData += 2;

	/*�����·���*/
	for (i=0;i<8;i++)
	{
		pDepotGateStru->wRouteOutsideId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*�����Ŷ�ӦSPKS��������*/
	pDepotGateStru->wSpksNum = ShortFromChar(pData);
	pData += 2;

	/*SPKS���*/
	for (i=0;i<4;i++)
	{
		pDepotGateStru->wSpksId[i] = ShortFromChar(pData);
		pData += 2;
	}

	/*��Ӧ�����źŻ�����*/
	pDepotGateStru->wProectSignalNum = ShortFromChar(pData);
	pData += 2;

	/*��Ӧ�����źŻ����*/
	for (i=0;i<6;i++)
	{
		pDepotGateStru->wProtectSignalId[i] = ShortFromChar(pData);
		pData += 2;
	}
}

/*�����*/
void TUNNEL_STRU_init(UINT8 *pDataAdd, DSU_TUNNEL_STRU *pTunnelStru)
{
	UINT8 *pData = pDataAdd;
	
	/*�������*/
	pTunnelStru->wId = ShortFromChar(pData);
	pData += 2;

	/*�������link���*/
	pTunnelStru->wTunLinkId = ShortFromChar(pData);
	pData += 2;

	/*�������linkƫ����*/
	pTunnelStru->dwBeginTunLinkOffset = LongFromChar(pData);
	pData += 4;

	/*�������linkƫ����*/
	pTunnelStru->dwEndTunLinkOffset = LongFromChar(pData);
	pData += 4;
}


/**************************************
�������ܣ�ZC��Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
		  [out]pZCStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע���ṹ���޸���

***************************/
void ZC_STRU_init(UINT8 * pDataAdd,DSU_ZC_STRU * pZCStru)
{
	UINT8 * pData = pDataAdd;
	UINT16 i=0;

	/*�������*/
	pZCStru->wIndex = ShortFromChar(pData);
	pData += 2;

	/*ZC�豸ID*/
	pZCStru->wZCId = ShortFromChar(pData);
	pData += 2;		

	/*ZC-VOBC��������У����Ϣ*/
	pZCStru->dwVersionOfZCToVOBC = LongFromChar(pData);
	pData += 4;	

	/*ZC-DSU��������У����Ϣ*/
	pZCStru->dwVersionOfZCToDSU = LongFromChar(pData);
	pData += 4;	

	/*ZC-ATS��������У����Ϣ*/
	pZCStru->dwVersionOfZCToATS = LongFromChar(pData);
	pData += 4;	

	/*����ZC����*/
	pZCStru->dwAdjoinZCCount = *pData;
	pData += 1;	

	for(i=0;i<6;i++)
	{
		/*����ZC[1-6]��ID*/
		pZCStru->dwAdjoinZCId[i] =ShortFromChar(pData);
		pData += 2;

		/*������ZC[1-6]�Ĺ�������У����Ϣ*/
		pZCStru->dwAdjoinZCShareCheckData[i] =LongFromChar(pData);
		pData += 4;

	}
}

/**************************************
�������ܣ��࿪�������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pMultiPointStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע��������Ŀ�������� add by qxt 20170622

***************************/
void MULTI_POINT_STRU_init(UINT8 *pDataAdd,DSU_MULTI_SWITCH_STRU *pMultiPointStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 i=0;
   /*������*/
	pMultiPointStru->wId=ShortFromChar(pData);
	pData+=2;
   /*��������*/
	pMultiPointStru->wType=*pData;
	pData+=1;
	for (i=0;i<MAX_VIRTUAL_SWITCH_NUM;i++)
	{
		/*���������*/
		pMultiPointStru->wSwitchIds[i]=ShortFromChar(pData);
		pData+=2;
		/*�������״̬*/
		pMultiPointStru->wSwitchStates[i]=ShortFromChar(pData);
		pData+=2;
	}
}


/**********************************************************
�������ܣ�����������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pPowerlessZoneStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݻ�����ͨ���ݽṹV12.0.0��ӣ�add by lmy 20180312
***********************************************************/
void POWERLESS_ZONE_STRU_init(UINT8 *pDataAdd, DSU_POWERLESS_ZONE_STRU *pPowerlessZoneStru)
{
	UINT8 *pData = pDataAdd;

	/*���*/
	pPowerlessZoneStru->wId = ShortFromChar(pData);
	pData += 2;
	/*���������Link���*/
	pPowerlessZoneStru->wOrgnLinkId = ShortFromChar(pData);
	pData += 2;
	/*���������Linkƫ����*/
	pPowerlessZoneStru->dwOrgnLinkOfst = LongFromChar(pData);
	pData += 4;
	/*�������յ�Link���*/
	pPowerlessZoneStru->wTmnlLinkId = ShortFromChar(pData);
	pData += 2;
	/*�������յ�Linkƫ����*/
	pPowerlessZoneStru->dwTmnlLinkOfst = LongFromChar(pData);
	pData += 4;
	/*��������*/
	pPowerlessZoneStru->wProtectDir = *pData;
	pData++;
	/*������ͨ������ٶȣ�km/h��*/
	pPowerlessZoneStru->wEntrySpeed = ShortFromChar(pData);
	pData += 2;
}

/**********************************************************
�������ܣ���������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pCarStopStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݻ�����ͨ���ݽṹV15.0.0��ӣ�add by sds 20181031
***********************************************************/
void CARSTOP_STRU_init(UINT8 *pDataAdd, DSU_CARSTOP_STRU *pCarStopStru)
{
	UINT8 *pData = pDataAdd;

	/*���*/
	pCarStopStru->carsId = ShortFromChar(pData);
	pData += 2;

	/*����Link���*/
	pCarStopStru->carsLinkId = ShortFromChar(pData);
	pData += 2;

	/*����Linkƫ����*/
	pCarStopStru->carsLinkOfst = LongFromChar(pData);
	pData += 4;

}

/**********************************************************
�������ܣ��������α���Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pResSpeedSgmt����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݳ������ݽṹ��ӣ�add by sds 2019-6-14
***********************************************************/
void RESPEED_SGMT_STRU_init(UINT8 *pDataAdd, DSU_RESPEED_SGMT_STRU *pResSpeedSgmtStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 i = 0;

	/*���*/
	pResSpeedSgmtStru->wId = ShortFromChar(pData);
    pData += 2;

	/*����*/
	for(i=0;i<16;i++)
	{
		pResSpeedSgmtStru->szCaption[i] = *pData;
		pData ++;
	}

	/*�������link���*/
	pResSpeedSgmtStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;

	/*�������linkƫ����*/
	pResSpeedSgmtStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;

	/*�յ�����link���*/
	pResSpeedSgmtStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;

	/*�յ�����linkƫ����*/
	pResSpeedSgmtStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;

	/*�����������α��*/
	pResSpeedSgmtStru->wManageAxleSgmtId = ShortFromChar(pData);
    pData += 2;
}

/**********************************************************
�������ܣ�OC����Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pOCStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݳ������ݽṹ��ӣ�add by sds 2019-6-14
***********************************************************/
void OC_STRU_init(UINT8 *pDataAdd, DSU_OC_STRU *pOCStru)
{
	UINT8 *pData = pDataAdd;

	/*�������*/
	pOCStru->wIndex = ShortFromChar(pData);
    pData += 2;
		
	/*OC��ID*/
	pOCStru->wId = ShortFromChar(pData);
    pData += 2;

	/*OC����*/
	pOCStru->wType = *pData;
    pData += 1;

	/*TMC-IVOC��������У����Ϣ*/
	pOCStru->wParityMsgOfTMC_IVOC = LongFromChar(pData);
	pData += 4;

	/*TMC-ITS��������У����Ϣ*/
	pOCStru->wParityMsgOfTMC_ITS = LongFromChar(pData);
	pData += 4;

}

/**********************************************************
�������ܣ�ITS����Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pITSStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݳ������ݽṹ��ӣ�add by sds 2019-6-14
***********************************************************/
void ITS_STRU_init(UINT8 *pDataAdd, DSU_ITS_STRU *pITSStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 loopIndex = 0;

	/*�������*/
	pITSStru->wIndex = ShortFromChar(pData);
    pData += 2;
		
	/*ITS��ID*/
	pITSStru->wId = ShortFromChar(pData);
    pData += 2;

    /*ITS_IVOC��������У����Ϣ*/
	pITSStru->wParityMsgOfITS_IVOC=LongFromChar(pData);	
	pData += 4;

	/*ITS_TMC��������У����Ϣ*/
	pITSStru->wParityMsgOfITS_TMC=LongFromChar(pData);	
	pData += 4;

	/*����ITS����*/
	pITSStru->wLinkITSCount = *pData;
	pData += 1;

	/*����ITS�����Ϣ*/
	for(loopIndex=0;loopIndex<6;loopIndex++)
	{
		/*����ITS��ID*/		        
		pITSStru->wLinkITSID[loopIndex]	= ShortFromChar(pData);	
		pData += 2;

		/*������ITS�Ĺ�������У����Ϣ*/
		pITSStru->wLinkITSParityMsg[loopIndex]=LongFromChar(pData);	
		pData += 4;
	}

}

/**********************************************************
�������ܣ���ͨ�ų����������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pNoComTrainArea����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݳ������ݽṹ��ӣ�add by sds 2019-6-14
***********************************************************/
void NOCOM_TRAIN_AREA_STRU_init(UINT8 *pDataAdd, DSU_NOCOM_TRAIN_AREA_STRU *pNoComTrainAreaStru)
{
	UINT8 *pData = pDataAdd;

	/*��ͨ�ų������ID*/
	pNoComTrainAreaStru->wId = ShortFromChar(pData);
    pData += 2;
		
	/*����*/
	pNoComTrainAreaStru->wDir = *pData;
	pData+=1;

	/*�������link���*/
	pNoComTrainAreaStru->wOrgnLinkId = ShortFromChar(pData);
    pData += 2;

	/*�������linkƫ����*/
	pNoComTrainAreaStru->dwOrgnLinkOfst = LongFromChar(pData);
    pData += 4;

	/*�յ�����link���*/
	pNoComTrainAreaStru->wTmnlLinkId = ShortFromChar(pData);
    pData += 2;

	/*�յ�����linkƫ����*/
	pNoComTrainAreaStru->dwTmnlLinkOfst = LongFromChar(pData);
    pData += 4;
}

/**********************************************************
�������ܣ����ɷ�������Ϣ���ݽṹ���ʼ������
����˵����[in] pDataAdd ���ݵ�ַ���������ļ���
          [out]pTransitZoneStru����ʼ���Ľṹ��ָ��
����ֵ����
��ע�����ݳ������ݽṹ��ӣ�add by sds 2019-6-14
***********************************************************/
void TRANSIT_ZONE_STRU_init(UINT8 *pDataAdd, DSU_TRANSIT_ZONE_STRU  *pTransitZoneStru)
{
	UINT8 *pData = pDataAdd;
	UINT16 loopIndex = 0;

	/*���ֶ��߱��*/
	pTransitZoneStru->wId = ShortFromChar(pData);
    pData += 2;

	/*��������ID*/
	for(loopIndex=0;loopIndex<4;loopIndex++)
	{				        
		pTransitZoneStru->wLinkBalise[loopIndex]= ShortFromChar(pData);	
		pData += 2;
	}

	/*��������*/
	//pTransitZoneStru->wProtecDistance= *pData;	
	//pData += 1;

	/*����OC���*/
	//pTransitZoneStru->wManagerOCID = ShortFromChar(pData);
	//pData += 2;


}