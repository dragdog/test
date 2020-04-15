/********************************************************
*                                                                                                            
* �� �� ���� SwitchConfigDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� �����������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _SWITCH_CONFIG_DATA_MANAGE_H_
#define _SWITCH_CONFIG_DATA_MANAGE_H_


#include "SwitchDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct S_SwitchConfigDataStruct
{
    UINT16 SwitchId;           /*����ID (�����ֶ�)*/
	UINT16 wBelongOcId;                 /*����OC ID*/
	UINT16 wSwitchModuleIdA;            /*Aϵ����ģ��ID*/
	UINT16 wSwitchPosA;                 /*Aϵת�޻�λ��*/
	UINT16 wSwitchModuleIdB;            /*Bϵ����ģ��ID*/
	UINT16 wSwitchPosB;                 /*Bϵת�޻�λ��*/

	UINT8 BelongCiId;          /*��������ID (�����ֶ�)*/
	UINT16 DSwitchOtherId;     /*˫��������һ������ID (�����ֶ�)*/
	UINT16 SwitchPhySecId;     /*������������ID (�����ֶ�)*/
	UINT16 SwitchPhySecIdBuf[SWITCH_PHYSECID_SUM_MAX];   /*������������ID���� (�����ֶ�)*/
	UINT16 SwitchPhySecIdNum;  /*�࿪�������������������� (�����ֶ�)*/
	UINT16 FcRelayId;          /*�����ټ̵���ID (�����ֶ�)*/
	UINT16 ZDbRelayId;          /*�����ܶ���̵���ID (�����ֶ�)*/
	UINT16 ZFbRelayId;          /*�����ܷ���̵���ID (�����ֶ�)*/
	/*��������й����ͨ���ü̵���*/
	UINT16 DbRelayId;          /*������̵���ID (�����ֶ�)  ��������λ��ʾ�̵���ID*/
	UINT16 FbRelayId;          /*������̵���ID (�����ֶ�)   �����������ʾ�̵���ID*/
	UINT16 YcRelayId;          /*�����ʲټ̵���ID (�����ֶ�) ��������λ�����̵���ID*/
	UINT16 DcRelayId;          /*�����ټ̵���ID (�����ֶ�) ��������λ�����̵���ID*/
	/*����Ϊ���������ؼ̵���*/
	UINT16 GZJId;				/*���ϼ̵���ID (�����ֶ�)*/
	UINT16 JTYJaId;				/*�ֳ�״̬�̵���ID (�����ֶ�)*/
	UINT16 QQJId;				/*�ֳ�����̵���ID(�����ֶ�)*/
	UINT16 SJFId;				/*���ռ̵���ID (�����ֶ�)*/
	UINT16 TYJId;				/*ͬ���ֳ����Ƽ̵���ID (�����ֶ�)*/
	UINT16 ThreeNId;			/*����Nλ��ʾ�̵���ID(�����ֶ�)*/
	UINT16 ThreeLId;			/*����Lλ��ʾ�̵���ID (�����ֶ�)*/
	UINT16 ThreeRId;			/*����Rλ��ʾ�̵���ID(�����ֶ�)*/
	UINT16 FiveNId;			/*�忪Nλ��ʾ�̵���ID (�����ֶ�)*/	
	UINT16 FiveL1Id;			/*�忪L1λ��ʾ�̵���ID(�����ֶ�)*/
	UINT16 FiveL2Id;			/*�忪L2λ��ʾ�̵���ID(�����ֶ�)*/
	UINT16 FiveR1Id;			/*�忪R1λ��ʾ�̵���ID (�����ֶ�)*/
	UINT16 FiveR2Id;			/*�忪R2λ��ʾ�̵���ID (�����ֶ�)*/
	UINT16 ThreeNYCId;			/*����Nλ�����̵���ID(�����ֶ�)*/
	UINT16 ThreeLYCId;			/*����Lλ�����̵���ID (�����ֶ�)*/
	UINT16 ThreeRYCId;			/*����Rλ�����̵���ID(�����ֶ�)*/
	UINT16 FiveNYCId;			/*�忪Nλ�����̵���ID (�����ֶ�)*/	
	UINT16 FiveL1YCId;			/*�忪L1λ�����̵���ID(�����ֶ�)*/
	UINT16 FiveL2YCId;			/*�忪L2λ�����̵���ID(�����ֶ�)*/
	UINT16 FiveR1YCId;			/*�忪R1λ�����̵���ID (�����ֶ�)*/
	UINT16 FiveR2YCId;			/*�忪R2λ�����̵���ID (�����ֶ�)*/
	UINT16 DFHRelayId;			/*�����DFH�̵���ID�������ֶΣ�*/
} SwitchConfigDataStruct;


/*
* ���������� ��ȡָ��ID����������
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� SWITCH_SUM_MAX: ��ȡ����ʧ��
*			<SWITCH_SUM_MAX: ���ҳɹ�
*/
UINT16 GetSwitchConfigBufIndex(const UINT16 switchId);

/*
* ���������� ��ȡ����ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchCurSum(void);

/*
* ���������� ��ȡָ�������±��Ӧ�ĵ���ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ID      
*/
UINT16 GetSwitchId(const UINT16 bufIndex);

/*
* ���������� ��ȡָ��ID��������
* ����˵����     
* ����ֵ  �� SwitchConfigDataStruct *   
*/
SwitchConfigDataStruct *GetSwitchConfigData(void);

/*
* ���������� ��ȡָ��ID������������ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetSwitchBelongCiId(const UINT16 switchId);

/*
* ���������� ��ȡ˫��������һ������ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��·��ǰ����
*/
UINT16 GetDSwitchOtherId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID��������
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������      
*/
UINT8 GetSwitchType(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID���������������ID
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ID      
*/
UINT16 GetSwitchPhySectionId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID����λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetSwitchDbRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID����λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetSwitchFbRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID�����ʲټ̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetSwitchYcRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID�����ټ̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetSwitchDcRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID�����ټ̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetSwitchFcRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID�����ܶ�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetSwitchZDbRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID�����ܷ�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetSwitchZFbRelayId(const UINT16 switchId);

/*
* ����������	�������ĵ�����������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 SwitchConfigDataManage(void);

/*
* ����������	����������������������������
* ����˵����	switchSum ��������
*				switchIdBuff �����������͸��������ĵ���ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendSwitchIdDataManage(UINT16 switchSum,const UINT16 *switchIdBuff);

/*
* ���������� ��ȡ�����������͸��������Լ�����������ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������������������͸��������źŻ�����
*/
UINT16 GetTotalSwitchCurNum(void);

/*
* ���������� ��ȡ���ϼ̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchGZJId(const UINT16 switchId);

/*
* ���������� ��ȡ�ֳ�״̬�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchJTYJaId(const UINT16 switchId);
/*
* ���������� ��ȡ�ֳ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchQQJId(const UINT16 switchId);
/*
* ���������� ��ȡ���ռ̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchSJFId(const UINT16 switchId);
/*
* ���������� ��ȡͬ���ֳ����Ƽ̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchTYJId(const UINT16 switchId);
/*
* ���������� ��ȡ����Nλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeNId(const UINT16 switchId);
/*
* ���������� ��ȡ����Lλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeLId(const UINT16 switchId);
/*
* ���������� ��ȡ����Rλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeRId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪Nλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveNId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪L1λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveL1Id(const UINT16 switchId);
/*
* ���������� ��ȡ�忪L2λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveL2Id(const UINT16 switchId);
/*
* ���������� ��ȡ�忪R1λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveR1Id(const UINT16 switchId);
/*
* ���������� ��ȡ�忪R2λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveR2Id(const UINT16 switchId);
/*
* ���������� ��ȡ����Nλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeNYCId(const UINT16 switchId);
/*
* ���������� ��ȡ����Lλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeLYCId(const UINT16 switchId);
/*
* ���������� ��ȡ����Rλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchThreeRYCId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪Nλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveNYCId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪L1λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveL1YCId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪L2λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveL2YCId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪R1λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveR1YCId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪R2λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ����ǰ����
*/
UINT16 GetSwitchFiveR2YCId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID����DFH�̵���ID
	* ����˵���� const UINT16 switchId, ����ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: DFH�̵���ID      
*/
UINT16 GetSwitchDFHRelayId(const UINT16 switchId);

/*
* ���������� ��ȡ������·ָ��ID�����������������Ŀ
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������������Ŀ      
*/
UINT16 GetMNRailSwitchPhySectionNum(const UINT16 switchId);

/*
* ���������� ��ȡ������·ָ��ID���������������ID����
* ����˵���� const UINT16 switchId, ����ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ID����      
*/
UINT16* GetMNRailSwitchPhySectionIdBuf(const UINT16 switchId);
#ifdef __cplusplus
}
#endif

#endif
