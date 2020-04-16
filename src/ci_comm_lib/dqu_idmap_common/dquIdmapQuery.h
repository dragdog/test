/************************************************************************
*
* �ļ���   ��  dsuIdmapQuery.h
* ��Ȩ˵�� ��  �������ؿƼ��Ƽ����޹�˾ 
* �汾��   ��  1.0
* ����ʱ�� ��  2016.04.12
* ����     ��  �����
* �������� ��  dsu��ѯ������������ͷ�ļ�
* ʹ��ע�� �� 
* �޸ļ�¼ ��  
*
************************************************************************/

#ifndef _DSU_IDMAP_QUERY_H
#define _DSU_IDMAP_QUERY_H

#ifdef ZCAPP_VXWORKS_CODECONTROL
#include <vxWorks.h>
#endif /*End of ZCAPP_VXWORKS_CODECONTROL*/

#include "CommonTypes.h"
#include "dsuIdmapStruct.h"
#include "dquVobcCommonStru.h"


#define QFUN_IDMAP_ZC_ID             1    /*ZC�������ID*/
#define QFUN_IDMAP_CI_ID             2    /*CI�������ID*/
#define QFUN_IDMAP_ATS_ID            3    /*ATS�������ID*/
#define QFUN_IDMAP_SIGNAL_ID         4    /*�źŻ��������ID*/
/*#define QFUN_IDMAP_TRACKSEC_ID			������α������ID  --by hgj �˽ӿ������ݽṹ�汾4.0���Ѿ����� */
#define QFUN_IDMAP_SWITCH_ID		 6	 /*����������ID*/
#define QFUN_IDMAP_STATION_ID		 7	 /*վ̨�������ID*/
#define QFUN_IDMAP_EMERGBTN_ID		 8	 /*�����رհ�ť�������ID*/
#define QFUN_IDMAP_BALISE_ID		 9	 /*Ӧ�����������ID*/
#define QFUN_IDMAP_PSD_ID			 10	 /*�����ű������ID*/
/*#define QFUN_IDMAP_VOBC_ID		     	VOBC�������ID      --by qxt �˽ӿ������ݽṹ�汾7.0���Ѿ�����*/
#define QFUN_IDMAP_FPD_ID			 12	 /*�����ű������ID*/
#define QFUN_IDMAP_DSU_ID			 13  /*DSU�������ID*/
#define QFUN_IDMAP_LOGICSEC_ID		 14  /*�߼����α������ID*/

#define QFUN_IDMAP_SPKS_ID		 16  /*SPKS�������ID*/
#define QFUN_IDMAP_GOD_ID		 17  /*�����ű������ID*/
#define QFUN_IDMAP_STA_ID		 18  /*��վ�������ID*/
#define QFUN_IDMAP_PHY_ID		 19  /*�������α������ID*/

#define IDMAP_TYPE_MIN 1   /*��ѯ�ṹ������Сֵ add by qxt 20170509*/
#define IDMAP_TYPE_MAX 19  /*��ѯ�ṹ�������ֵ�����ڷ�����ѯ����struType�ķ�Χ*/

#ifdef __cplusplus
extern "C" {
#endif

/*��Ŷ��ձ��ѯ�������ʽӿ�*/
	
/*���õ�ǰ����Դ*/
UINT8 dquSetCurIdMapQueryDataSource(DSU_IDMAP_STRU *pDsuIdmapStru);

/**************************************************
*�������ܣ���ѯ�豸����
*���������
*	struType����ѯ�Ľṹ����
*	devId���豸���
*���������
*	pInDevIndex,���ص�����ֵ
*����ֵ��1�ɹ���0ʧ��
***************************************************/
UINT8 dquQueryInDevIndex(UINT8 struType,UINT32 devId, UINT16 *pInDevIndex);


/**************************************************
*�������ܣ���ѯ�豸���
*���������
*	struType����ѯ�Ľṹ����
*	devId���豸����ֵ
*���������
*	pOutDevId����ȡ�Ļ�����ͨ�豸���
*����ֵ��1�ɹ���0ʧ��
***************************************************/
UINT8 dquQueryOutDevId(UINT8 struType,UINT16 devIndex, UINT32 *pOutDevId);


/**************************************************
*�������ܣ���֪������εı�ź�ƫ��������ѯLink�ı�ź�ƫ����
*���������
*	trackId��������α��
*	trackOffset���������ƫ����
*���������
*	pLinkId����ȡ��LINK���
*	pLinkOffset����ȡ��LINKƫ����
*����ֵ��1�ɹ���0ʧ��
******************************************************/
UINT8 dquQueryLinkIdOffset(UINT32 trackId, UINT32 trackOffset, UINT16 *pLinkId, UINT32 *pLinkOffset);

/*****************************************************
*�������ܣ���֪Link�ı�ź�ƫ��������ѯ������εı�ź�ƫ����
*���������
*	linkId��link���
*	linkOffset��linkƫ����
*���������
*	pTrackSecId��������α��
*	pTrackOffset���������ƫ����
*����ֵ��1�ɹ���ʧ��
******************************************************/
UINT8 dquQueryTrackIdOffset(UINT16 linkId, UINT32 linkOffset, UINT32 *pTrackSecId, UINT32 *pTrackOffset);


/**************************************************
*�������ܣ������豸����λ�ã���ȡvobc���
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ��vobc�������ָ��
* ע���ڲ�ʹ�ú��� add by qxt 20170117
***************************************************/
UINT32* dquGetVobcDevIdArray(UINT16 dataPos);

/**************************************************
*�������ܣ������豸����λ�ã���ȡAOM���
*���������
*	dataPos���豸�ṹ��λ��
*���������
*	��
*����ֵ��AOM�������ָ��
* ע���ڲ�ʹ�ú��� add by sds 2018-6-14
***************************************************/
UINT32* dquGetAOMDevIdArray(UINT16 dataPos);

/**************************************************
*�������ܣ���ѯvobc����
*���������
*	devId���豸���
*���������
*	pInDevIndex,���ص�����ֵ
*����ֵ��1�ɹ���0ʧ��
*ע��vobc�������ֶΡ�vobc���2�����赥������  add by qxt 20170117
***************************************************/
UINT8 dquQueryInVobcDevIndex(UINT32 devId, UINT16 *pInDevIndex);

/**************************************************
*�������ܣ���ѯvobc�豸���
*���������
*	devId���豸����ֵ
*���������
*	arOutDevIds����ȡ�Ļ�����ͨ�豸�������
*����ֵ��1�ɹ���0ʧ��
*ע��vobc�������ֶΡ�vobc���2�����赥������  add by qxt 20170117
***************************************************/
UINT8 dquQueryOutVobcDevId(UINT16 devIndex, UINT32 arOutDevIds[]);

/**************************************************
*�������ܣ���ѯAOM����
*���������
*	devId���豸���
*���������
*	pInDevIndex,���ص�����ֵ
*����ֵ��1�ɹ���0ʧ��  add by sds 2018-6-14
***************************************************/
UINT8 dquQueryInAOMDevIndex(UINT32 devId, UINT16 *pInDevIndex);

/**************************************************
*�������ܣ���ѯAOM�豸���
*���������
*	devId���豸����ֵ
*���������
*	arOutDevIds����ȡ�Ļ�����ͨ�豸�������
*����ֵ��1�ɹ���0ʧ�� add by sds 2018-6-14
***************************************************/
UINT8 dquQueryOutAOMDevId(UINT16 devIndex, UINT32 arOutDevIds[]);


/**************************************************
*�������ܣ�ȡ�豸�����Ӧλ������ѯ�豸����ֵ
*���������
*	struType����ѯ�Ľṹ����
*	devId���豸���
*   bitMask,�豸�����Чλ��
*���������
*	pInDevIndex����ȡ�Ļ�����ͨ�豸����ֵ
*����ֵ��1�ɹ���0ʧ��
*ʹ��������������1��devId �豣֤������ȷ��(�ڱ��д�����Ψһ)
               ��2��bitMask����Ϊ��ȡ�������devId����ЧbitMaskλ�����ٲ�ѯ������Ӧ�����������磺��ȥ��devId�ĸ�5λʱ���ɽ�Bits����0x7FFFFFF;
*              ���øú�������Ҫ��֤ȡ��devId����Чλ��������Ӧ������ֵ������Ψһ�ģ�����
*add by qxt 20170421
***************************************************/
UINT8 dquQueryDevBits2Index(UINT8 struType,UINT32 devId, UINT32 bitMask,UINT16 *pInDevIndex);

/**************************************************
*�������ܣ�ȡVOBC�豸�����Ӧλ������ѯvobc����ֵ
*���������
*	devId��vobc�豸���
*   bitMask,�豸��ŵ���Чλ��
*���������
*	pInDevIndex����ȡ��vobc�豸����ֵ
*����ֵ��1�ɹ���0ʧ��
*ע����vobc���������豸ID���赥������  add by qxt 20170421
***************************************************/
UINT8 dquQueryVobcDevBits2Index(UINT32 devId,UINT32 bitMask,UINT16 *pInDevIndex);

/**************************************************
*�������ܣ�ȡAOM�豸�����Ӧλ������ѯAOM����
*���������
*devId   AOM�豸���
*bitMask �豸��ŵ���Чλ�� 
*���������
*	pInDevIndex���豸����
*����ֵ��1�ɹ���0ʧ��  add by sds 2018-6-25
***************************************************/
UINT8 dquQueryAOMDevBits2Index(UINT32 devId,UINT32 bitMask,UINT16 *pInDevIndex);

/**************************************************
*�������ܣ���ѯӦ�����豸����
*���������
*	lineId����·���
*	devId���豸���
*���������
*	pInDevIndex,���ص�����ֵ
*����ֵ��1�ɹ���0ʧ��
*ע����ѯӦ�����豸����ʱ��ʹ�øú����ӿڡ�������·�ź��豸���ȷ������ֵ��add by qxt 2018-7-25 RDGG47
***************************************************/
UINT8 dquQueryInBaliseDevIndex(UINT16 lineId,UINT32 devId, UINT16 *pInDevIndex);

#ifdef __cplusplus
}
#endif

#endif