/************************************************************************
*
* �ļ���   ��  dsuQuery.h
* ��Ȩ˵�� ��  �������ؿƼ��Ƽ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2009.11.21
* ����     ��  �����
* �������� ��  dsu��ѯ������������ͷ�ļ�
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef _DSU_QUERYEXP_H
#define _DSU_QUERYEXP_H

#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "CommonTypes.h"
#include "dsuStruct.h"
#include "dquVobcCommonStru.h"


#define DSU_MAX_LINK_BETWEEN 400


#ifdef __cplusplus
extern "C" {
#endif

/*ATO��������*/

/*2.�������Ծ����ѯ*/
UINT8 dsuDispCalcuByLOCExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODAStru, const LOD_STRU *pLODBStru, 
						UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, INT32 *Displacement);

/*���ܺ��������ݵ���״̬��ȡ��ǰLink������Link*/
UINT8 dsuGetAdjacentLinkIDExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId, UCHAR Direction, UCHAR PointNum,const DQU_POINT_STATUS_STRU *pPointStru, UINT16 *pLinkId);

/*���ܺ�������λ��+λ��=��λ��*/
UCHAR dsuLODCalcuExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, INT32 Displacement, UCHAR PointNum, const DQU_POINT_STATUS_STRU *pPointStru, LOD_STRU *pNewLODStru);

/*���ܺ�������ȡ����֮���link����*/
UINT8 dsuGetLinkBtwLocExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
					   UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
					   UINT16 wLinkID[DSU_MAX_LINK_BETWEEN], UINT16 *wLinkNumber);

/*���ܺ������ж�һ�����Ƿ�������Χ��*/
UINT8 dsuCheckLocInAreaExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStartStru, const LOD_STRU *pLODEndStru,
						UINT8 PointNum, const DQU_POINT_STATUS_STRU *pPointStru, 
						const LOD_STRU *pCurrentLODStru,UINT8 *pLocInArea); 

/*���ܺ�������ȡlink����*/
UINT32 LinkLengthExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 wLinkID);

/*
* �������ܣ� �˺����Ĺ������жϵ�ǰλ���Ƿ���ָ������Χ�ڣ��˺����Ƕ�dsuCheckLocInAreaQuick
             ��һ�����䡣���ڽ���ǰ����ȡ�����ټ����Ƿ��������ڡ�
* ��ڲ����� const LOD_STRU * pLODStartStru,	����ʼ��λ��
*            const LOD_STRU * pLODEndStru,	�����ն�λ��
* ���ڲ����� UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link��������
*            UINT16 *wLinkNumber                             Link����
*			 const LOD_STRU * pCurrentLODStru,��ǰλ��
*            UINT8 *pLocInArea         1,��ָ������
*                                     0,����ָ������
* ����ֵ��   0����ѯʧ��
*            1����ѯ�ɹ�
*/
UINT8 dsuCheckLocInAreaQuickExp2(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
							 UINT16 wLinkNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
							 const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea);
/*
* �������ܣ� �˺����Ĺ����ǽ��жϵ�ǰλ���Ƿ���ָ������Χ�ڡ�
             �˺����Ĺ�����CheckLocInArea��������ġ�CheckLocInArea�Է���û��Ҫ�󣬼��������漰�ķ�������ȷ�ġ�
			 ��CheckLocInAreaQuickҪ���StartLOD�ܹ�����EndLOD��
* ****************************************************************************************************
*
*       --------------*--------------------*---------------------*-----------------
*                     S                    C                     E
*       �����㷨Ϊ�������жϵ�ǰλ���������S��E�Ƿ���ͬһLink�ϣ�
*       �����ͬһLink�ϣ����ݷ����λ���ж��Ƿ��������ڣ��������ͬһLink���ж�CLinkId�Ƿ���SE֮���ĳһLinkId��ͬ
*       ��������֮���Link���Ա������������Զ��ڶ�ε��ñ�������ѯ�Ƿ��������ڵĺ���Ч�ʺ��а���
*
* ****************************************************************************************************
* ��ڲ����� const LOD_STRU * pLODStartStru,	����ʼ��λ��
*            const LOD_STRU * pLODEndStru,	�����ն�λ��
* ���ڲ�����	UINT16 wLinkID[DSU_MAX_LINK_BETWEEN]				Link��������
*               UINT16 *wLinkNumber                             Link����
*			 const LOD_STRU * pCurrentLODStru,��ǰλ��
* ���ڲ����� UINT8 *pLocInArea         1,��ָ������
*                                     0,����ָ������
* ����ֵ��   0����ѯʧ��
*            1����ѯ�ɹ�
* ʹ��ע�⣺1.�ڵ�ǰλ��������˵�λ��ʱ����ʱ�����¹����жϣ�
*             1) ��ǰλ����������㣬��Ϊ�õ����ڱ�����
*             2) ��ǰλ���������յ㣬��Ϊ�õ㲻���ڱ�����
*             3) 3���غϷ����������ڡ�
*           2.��������ҵ�������յ㶼�������ڣ����Ե������α�����������ʼ�����ն�λ�ã�ֻҪ���ε�����һ���������ڣ�˵���ڵ���ʼ�����ն��γɵı�������
*           3.���SE���򲻴��ڣ���������0ʧ�ܡ����SC,����SE�����ڣ���������1�ɹ������ǵ�C����������
*/
UINT8 dsuCheckLocInAreaQuickExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
							 UINT16 wLinkNumber,UINT16 wLinkID[DSU_MAX_LINK_BETWEEN],
							 const LOD_STRU * pCurrentLODStru,UINT8 *pLocInArea);
/*���ܺ���,��ѯ��ǰGarde������Grade*/
UINT16 FindAdjacentGradeIDExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 wCurrentGradeID, const UCHAR chDirection, const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pstPointStrut);

/*����������¶�*/
UINT8 dsuGetGradeIdbyLocExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 linkId,const UINT32 offset,UINT16* GradeID);
/*����㵽�����¶����ľ���*/
UINT8 dsuGetDistanceFromOriginExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 GradeId,UINT16 linkId,UINT32 Ofst,UINT32* length);
/*�����¶ȱ��µ����¶����ľ���*/
UINT8 dsuDividPointDistanceFromOriginExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 GradeId,UINT16 linkId,UINT32* endPointDistance,UINT32* startPointDistance);
/*����������¶�ֵ*/
UINT8 dsuCalculateGradeValueExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 LinkId,UINT32 ofst, FLOAT32* GradeVal);
/*��������¶�*/
UCHAR dsuGradientGetExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU * pLODStartStru, const LOD_STRU * pLODEndStru,
					 const UCHAR PointNum, const DQU_POINT_STATUS_STRU * pPointStru, FLOAT32 * Gradient);
/*�жϺ�һ�¶ȵ��¶�ֵ���ǰһ���¶ȵ��¶�ֵ�����ӣ����仯���Ǽ��� */
UINT8 dquGradeTrendExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 fistGradeID,UINT16 secondGradeID,UINT8 *GradeTrend);

/*
�������ܣ���ȡǰ����¶�
���������UINT16 linkId,��ǰlink
���������gradeFrnt �¶� 
			Null��  ��·�յ��ǰ���¶�Ϊͳһ�¶�  
			��Null��ǰ���¶����¶ȱ��пɲ�
  ����ֵ��0ʧ�� 1�ɹ�
*/
UINT8 dsuGetGradeFrntExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeFrnt);
/*
�������ܣ���ȡ������¶�
���������UINT16 linkId,��ǰlink
���������gradeRear �¶� 
			Null��  ��·�յ������¶�Ϊͳһ�¶�  
			��Null�������¶����¶ȱ��пɲ�
  ����ֵ��0ʧ�� 1�ɹ�
*/
UINT8 dsuGetGradeRearExp(DSU_EMAP_STRU *pDsuEmapStru,UINT16 linkId, DSU_GRADE_STRU** gradeRear);
/**
�������ܣ���ȡ��ǰLINK�ľ�̬���ٸ���
���������
@linkId����ǰlink
�����������
����ֵ����̬���ٸ���
*/
UINT8 GetStcLmtSpdCntByLnkIdExp(DSU_EMAP_STRU *pDsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *pDsuLimitStcLinkIdx,const UINT16 linkId);

/**
�������ܣ���ȡ�ƶ�LINK�ĵ�ָ���±�ľ�̬��ʱ���ٵĶ���ָ��
���������
@linkId����ǰLink���
@lmtSpdIdx����ӦLink�ĵڼ������٣�ȡֵ��Χ1-10
����ֵ��
@NULL����ȡʧ�� 
@�ǿգ���Ӧ���ٽṹ��ָ��
*/
DSU_STATIC_RES_SPEED_STRU *GetStcLmtSpdInfoByLnkIdExp(DSU_EMAP_STRU *pDsuEmapStru,DSU_STC_LIMIT_LINKIDX_STRU *pDsuLimitStcLinkIdx,const UINT16 linkId,const UINT8 idx);

/*
�������ܣ����࿪����תΪ�������
���������multiPointId �࿪������
          multiPointState �࿪����״̬

���������wVirtualPointIds ��ֺ���������� 
          wVirtualPointStates ��ֺ��������״̬
����ֵ������ֵ����ֺ�����������, 0ʧ��  

�����㷨���࿪���������������� Nλ��R1λ��L1λ��R2λ��L2λ
            ����Ӧ�������е������� 0��  1��   2��   3��   4
		    ����Ӧ�Ķ࿪����״̬�� 1��  2��   3��   4��   5
		   ��˵� ����=�࿪����״̬multiPointState-1 ʱ������Ӧ�����������ԭֵ�������������״̬ȡ������λ<=>��λ��

ע�������������������wVirtualPointIds��״̬wVirtualPointStates��������Ч���������״̬����Ϊ3����
         
		               / L                                      / L
		              /                                        /
		             /                                        /       101                
		-|------------------------         ===��   ---|------/----------------- N      
		   1         \           N                          102       \
		              \                                                \
					   \                                                \
		              R                                                 R
		           ͼ1.ʵ����������                            ͼ2.���ⵥ������

		   ��ͼ1ʵ����������1��״̬Ϊ3ʱ���Ƴ�ͼ2���ⵥ������ΪLλ����ʱ101Ϊ��Ч���������״̬Ϊ3������ֵ�������Ϊ2������
		   wVirtualPointIds[0]=102;
		   wVirtualPointStates[0]=2;
		   wVirtualPointIds[1]=101;
		   wVirtualPointStates[1]=3;
author:������Ŀ add by qxt 20170622
*/
UINT8 ConvertToVirtualSwitchesExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 multiPointId,const UINT8 multiPointState,UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM]);

/*
�������ܣ����������תΪ�࿪����
���������wVirtualPointIds[] ��������� 
          wVirtualPointStates[] �������״̬
		  wVirtualPointNum    �����������
���������multiPointId ��Ӧ�࿪������
          multiPointState ��Ӧ�࿪����״̬
����ֵ��0ʧ�� 1�ɹ�

ע�������������������wVirtualPointIds��״̬wVirtualPointStates��������Ч�������������Ҫ���롣��
           
		 		      /L                                                 L
		             /                                                /
		            /                                                /
		           /                                                /
		---|------/-----------\------ N      =====>        ------------------ N
		          1          2 \                                    \
							    \                                    \
		                          R                                      R
		         ͼ1.���ⵥ������                             ͼ2.ʵ����������
		 
		 ��ͼ1���������1��״̬Ϊ2ʱ���Ƴ�ͼ2�࿪�����״̬ΪLλ����ʱͼһ�������2Ϊ��Ч�ģ����������������Ϊ���£�
         wVirtualPointIds[0]=1;
		 wVirtualPointStates[0]=2;
         wVirtualPointNum=1;
author:������Ŀ add by qxt 20170622
*/
UINT8 ConvertToMultiSwitchExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 wVirtualPointIds[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointStates[MAX_VIRTUAL_SWITCH_NUM],const UINT8 wVirtualPointNum,UINT16 *multiPointId,UINT8 *multiPointState);

/************************************************
 *��������:����link��ȡ��Ӧ��OC��ID(���������ӿ�)
 *�������:linkId
 *�������:ocId
 *����ֵ:0ʧ��,1�ɹ�
 *��ע��add by sds 2019-6-17
 ************************************************/
UINT8 dsuOcByLinkExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 linkId,UINT16 *ocId);


/************************************************
 *��������:��ȡ�źŻ�λ��(���������ӿ�)
 *�������:wId:�źŻ��������
 *�������:position:�źŻ�����link�Լ�ƫ����
 *����ֵ:0:ʧ�� 1:�ɹ�
 *��ע��add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetSignalPositionExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 wId, LOD_STRU *position);


/************************************************
 *��������:��ȡ��ǰlink�Ƿ����۷�ͣ����(�����۷���ͣ����)(���������ӿ�)
 *�������:wId:Link�������
 *�������:result:0x55��;0xAA:��
 *����ֵ:0:ʧ�� 1:�ɹ�
 *��ע��add by sds 2019-6-17
 ************************************************/
UINT8 dsuCheckArStopOnLinkExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 wId, UINT8 *result);


/************************************************
 *��������:��ѯlink�����ڵĵ�����Ϣ(���������ӿ�)
 *�������:pLinkStru:Link������Ϣ
 *�������:pBaliseStru:link�����ڰ����ĵ�����Ϣ
 *����ֵ:0:ʧ�� 1:�ɹ�
 *��ע��add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetPointsBetweenLinkExp(DSU_EMAP_STRU *pDsuEmapStru, const COM_DQU_OBJECT_STRU *pLinkStru,COM_DQU_POINT_STRU *pPointStru);	

/************************************************
 *��������:����һ��λ�ú�һ�γ��ȣ����Ҵ˳��ȷ�Χ�ڵĵ�����Ϣ��link����(���������ӿ�)
 * ���������	const LOD_STRU * pLODStru,	ʼ��λ��     const INT32     disPlacement,	���ҳ���
 * ���������	DQU_LINKS_STRU *pLinkStru		Link���� DQU_BALISE_STRU *pBaliseStru    ��������
 * ����ֵ��   	0,  ��ѯʧ��  1,  ��ѯ�ɹ� 
 *��ע��  �����ĵ��������λ���� add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetLinkAndBaliseBtwLocExp(DSU_EMAP_STRU *pDsuEmapStru,const LOD_STRU *pLODStru, const INT32 Displacement, 
								COM_DQU_OBJECT_STRU *pLinkStru,COM_DQU_POINT_STRU *pBaliseStru, LOD_STRU *pEndLodStru);
/************************************************
 *��������:��������Ӧ����֮��ľ���(���������ӿ�)
 *���������wBaliseId1 Ӧ����1ID wBaliseId2 Ӧ����2ID  wMaxLen	 ����Ӧ����֮���������
 *���������*len ����Ӧ����֮���ʵ�ʾ��� 
 *����ֵ��0��ʧ��  1���ɹ�
 *��ע��  �����ĵ��������λ���� add by sds 2019-6-17
 ************************************************/
UINT8 GetLenBetweenBaliseExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT32 wBaliseId1, const UINT32 wBaliseId2, const UINT32 wMaxLen, UINT32 *len);

/************************************************
 *��������:��ָ�������������λ�ü�ĵ�������(���������ӿ�)
 *���������pStart �������λ���Լ�����(0x55:˳��link����;0xAA:����link����)  pEnd   �����յ�λ��(������Ч) pPointAll ȫ��������Ϣ(�Ȳ��ҵĽ����Χ��������)
 *���������pPointExp ���ҵ������� (0x55:��λ 0xAA:��λ)
 *����ֵ��0:ʧ�ܻ�δ�ҵ� 1:�ɹ��ҵ�
 *��ע�� add by sds 2019-6-17
 ************************************************/
UINT8 GetPointsInfoInAreaExp(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_POINT_STRU *pPointExp);


/************************************************
 *��������:��ָ�������������λ�ü��PSD����(���������ӿ�)
 *���������pStart �������λ���Լ�����(0x55:˳��link����;0xAA:����link����)  pEnd   �����յ�λ��(������Ч)  pPointAll ȫ��������Ϣ(�Ȳ��ҵĽ����Χ��������)
 *���������pObjectExp ���ҵ�������
 *����ֵ��0:ʧ�ܻ�δ�ҵ� 1:�ɹ��ҵ�
 *��ע�� add by sds 2019-6-17
 ************************************************/
UINT8 GetPsdsInfoInAreaExp(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_OBJECT_STRU *pObjectExp);

/************************************************
 *��������:��ָ�������������λ�ü��ESB����(���������ӿ�)
 *���������pStart �������λ���Լ�����(0x55:˳��link����;0xAA:����link����) pEnd   �����յ�λ��(������Ч)  pPointAll ȫ��������Ϣ(�Ȳ��ҵĽ����Χ��������)
 *���������pObjectExp ���ҵ�������
 *����ֵ��0:ʧ�ܻ�δ�ҵ� 1:�ɹ��ҵ�
 *��ע�� add by sds 2019-6-17
 ************************************************/
UINT8 GetEsbsInfoInAreaExp(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_OBJECT_STRU *pObjectExp);

/************************************************
 *��������:ͨ�������link���С���βλ�ú����þ��룬��ѯ��βλ�������þ��뷶Χ�ڵ�link������ID(���������ӿ�)
 *�������:pLinkIn:�����link����
 		   pTailLoc:��βλ��
		   dis:���þ���		  
 *�������:pLinkOut:��ѯ����link������ID
 *����ֵ:0:ʧ�� 1:�ɹ�
 *��ע�� add by sds 2019-6-17
 ************************************************/
UINT8 dsuLinksWithCfgDisInInputLinksExp(DSU_EMAP_STRU *pDsuEmapStru, const COM_DQU_OBJECT_STRU *pLinkIn,const LOD_STRU *pTailLoc,const INT32 dis,COM_DQU_OBJECT_STRU *pLinkOut);
		


/************************************************
 *��������:ͨ�������link���С���βλ�ú����þ��룬��ѯ��βλ�������þ��뷶Χ�ڵ�link������OC������ID(���������ӿ�)
 *�������:pLinkIn:�����link����
		   pTailLoc:��βλ��
		   dis:���þ���		  
 *�������:pLinkOut:��ѯ����OC������ID
 *����ֵ:0:ʧ�� 1:�ɹ�
 *��ע�� add by sds 2019-6-17
 ************************************************/
UINT8 dsuOcsWithCfgDisInInputLinksExp(DSU_EMAP_STRU *pDsuEmapStru, const COM_DQU_OBJECT_STRU *pLinkIn,const LOD_STRU *pTailLoc,const INT32 dis, COM_BASE_COMM_OC_STRU *pOcOut);


/************************************************
 *��������:��ѯ�豸����OC(���������ӿ�)
 *�������:devId:�豸ID		  
 *�������:devType:�豸����(1:����,2:�źŻ�,3:����ͣ����ť)(���ڵ�����ʱȡ���linkΪ׼)
 *����ֵ:0:ʧ�� 1:�ɹ�
 *��ע�� add by sds 2019-6-17
 ************************************************/
UINT8 dsuOcWithInputDevIdExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 devId,const UINT8 devType, UINT16 *ocId);

/************************************************
 *��������:��ѯһ����Χ�����г�������ͬ���źŻ�(���������ӿ�)
 *���������pStart �������λ���Լ�����(0x55:˳��link����;0xAA:����link����)
		    pEnd   �����յ�λ��(������Ч)
		    pPointAll ����㵽�յ�ĵ�����Ϣ(��������)
 *���������pObjectExp ���ҵ�������
 *����ֵ:0:ʧ�� 1:�ɹ�
 *��ע�� add by sds 2019-6-17
 ************************************************/
UINT8 dsuSignalBetweenPisitionsExp(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pStart, const LOD_STRU *pEnd, const COM_DQU_POINT_STRU *pPointAll, COM_DQU_OBJECT_STRU *pObjectExp);


/************************************************
 *��������:���ݵ�ǰλ�û�ȡ����OC(���������ӿ�)
 *�����������ǰλ��
 *��������������λ��
 *����ֵ��0:ʧ�ܻ�δ�ҵ� 1:�ɹ��ҵ�
 *��ע�� add by sds 2019-6-17
 ************************************************/
UINT8 GetLocInOdIdExp(DSU_EMAP_STRU *pDsuEmapStru, const LOD_STRU *pCurrLoc, UINT16 *pOcID);

/************************************************
 *��������:���ݵ���ID��ȡ�����λ��(���������ӿ�)
 *�����������ǰλ��
 *��������������λ��
 *����ֵ��0:ʧ�ܻ�δ�ҵ� 1:�ɹ��ҵ�
 *��ע�� add by sds 2019-6-17
 ************************************************/
UINT8 GetPointLocFromIdExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 PointId, DQU_POINT_LOC_STRU *pPointLoc);


/************************************************
 *��������:��ѯ2��link����Է�����LINK B��LINK A���յ�����LINK  ��A��BΪ����(���������ӿ�)
 *�������:		linkAID:LinkA������ID
				linkBID:LinkB������ID
 *�������:dir:2��link����Է���  0x55=���� 0xAA=����  0xFF=��Ч
 *����ֵ:0:ʧ�� 1:�ɹ�
 *��ע�� add by sds 2019-6-17
 ************************************************/
UINT8 dsuGetDirOfLinkToLinkExp(DSU_EMAP_STRU *pDsuEmapStru,const UINT16 linkAID,const UINT16 linkBID,UINT8 *dir);

/************************************************
 *��������:���������������У���ѯ������link����(���������ӿ�)
 *�������:		PhysicalSgmt	����������Ϣ
 *�������:		pLinks link������Ϣ
 *����ֵ:0:ʧ�� 1:�ɹ�
 *��ע�� add by sds 2019-6-21
 ************************************************/
UINT8 dsuRegionBlockTolinkExp(DSU_EMAP_STRU *pDsuEmapStru, const COM_DQU_OBJECT_STRU *PhysicalSgmtStru, COM_DQU_OBJECT_STRU *LinksStru);

/************************************************
 *��������:����link��ţ���ѯ����ITS�豸ID(���������ӿ�)
 *�������:		UINT16 LinkWid	link���
 *�������:		UINT16  ItsId ����ITS�豸ID
 *����ֵ:0:ʧ�� 1:�ɹ�
 *��ע�� add by sds 2019-6-21
 ************************************************/
UINT8 dsuItsIdOfLinkExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 LinkWid, UINT16 *ItsId);

/************************************************
*��������:��ȡ����������Ϣ(���������ӿ�)
*�������:wId:�źŻ��������
*�������:position:�źŻ�����link�Լ�ƫ����
*����ֵ:0:ʧ�� 1:�ɹ�
*��ע��add by sds 2019-6-17
************************************************/
UINT8 dsuGetRespeedSgmtExp(DSU_EMAP_STRU *pDsuEmapStru, const UINT16 wId, DSU_RESPEED_SGMT_STRU *RespeedSgmtStru);

/************************************************
*��������:���ݼ����������У���ѯ������link����(���������ӿ�)
*�������:		PhysicalSgmt	����������Ϣ
*�������:		pLinks link������Ϣ
*����ֵ:0:ʧ�� 1:�ɹ�
*��ע�� add by sds 2019-6-21
************************************************/
UINT8 dsuAxleSgmtTolinkExp(DSU_EMAP_STRU *pDsuEmapStru, const COM_DQU_OBJECT_STRU *pAxleSgmtStru, COM_DQU_OBJECT_STRU *LinksStru);

#ifdef __cplusplus
}
#endif

#endif