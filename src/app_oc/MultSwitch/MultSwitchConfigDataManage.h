/********************************************************
*                                                                                                            
* �� �� ���� MultSwitchConfigDataManage.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-19
* ��    �ߣ� ������
* ���������� ��������������ݹ���ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _MULTSWITCH_CONFIG_DATA_MANAGE_H_
#define _MULTSWITCH_CONFIG_DATA_MANAGE_H_


#include "MultSwitchDefine.h"
#include "CommonTypes.h"
#include "../System/CiSystemDefine.h"
#include "../System/SystemParameterManage.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef struct S_MultSwitchConfigDataStruct
{
    UINT16 MultSwitchId;           /*�������ID (�����ֶ�)*/
	UINT16 wBelongOcId;                 /*����OC ID*/
	UINT16 wMultSwitchModuleIdA;            /*Aϵ�������ģ��ID*/
	UINT16 wMultSwitchPosA;                 /*Aϵת�޻�λ��*/
	UINT16 wMultSwitchModuleIdB;            /*Bϵ�������ģ��ID*/
	UINT16 wMultSwitchPosB;                 /*Bϵת�޻�λ��*/

	UINT8 BelongCiId;          /*��������ID (�����ֶ�)*/
	UINT16 DMultSwitchOtherId;     /*˫�����������һ���������ID (�����ֶ�)*/
	UINT16 MultSwitchPhySecId;     /*���������������ID (�����ֶ�)*/
	UINT16 MultSwitchPhySecIdBuf[MULTSWITCH_PHYSECID_SUM_MAX];   /*���������������ID���� (�����ֶ�)*/
	UINT16 MultSwitchPhySecIdNum;  /*�࿪����������������������� (�����ֶ�)*/
	UINT16 FcRelayId;          /*������߷��ټ̵���ID (�����ֶ�)*/
	UINT16 ZDbRelayId;          /*��������ܶ���̵���ID (�����ֶ�)*/
	UINT16 ZFbRelayId;          /*��������ܷ���̵���ID (�����ֶ�)*/
	/*��������й����ͨ���ü̵���*/
	UINT16 DbRelayId;          /*������߶���̵���ID (�����ֶ�)  ��������λ��ʾ�̵���ID*/
	UINT16 FbRelayId;          /*������߷���̵���ID (�����ֶ�)   �����������ʾ�̵���ID*/
	UINT16 YcRelayId;          /*��������ʲټ̵���ID (�����ֶ�) ��������λ�����̵���ID*/
	UINT16 DcRelayId;          /*������߶��ټ̵���ID (�����ֶ�) ��������λ�����̵���ID*/
	/*����Ϊ���콻�������ؼ̵���*/
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
	UINT16 DFHRelayId;			/*������ߵ�DFH�̵���ID�������ֶΣ�*/
} MultSwitchConfigDataStruct;


/*
* ���������� ��ȡָ��ID�������������
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� MULTSWITCH_SUM_MAX: ��ȡ����ʧ��
*			<MULTSWITCH_SUM_MAX: ���ҳɹ�
*/
UINT16 GetMultSwitchConfigBufIndex(const UINT16 switchId);

/*
* ���������� ��ȡ������ߵ�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchCurSum(void);

/*
* ���������� ��ȡָ�������±��Ӧ�Ľ������ID
* ����˵���� const UINT16 bufIndex, �����±�    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������ID      
*/
UINT16 GetMultSwitchId(const UINT16 bufIndex);

/*
* ���������� ��ȡָ��ID�����������
* ����˵����     
* ����ֵ  �� MultSwitchConfigDataStruct *   
*/
MultSwitchConfigDataStruct *GetMultSwitchConfigData(void);

/*
* ���������� ��ȡָ��ID���������������ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��������ID      
*/
UINT8 GetMultSwitchBelongCiId(const UINT16 switchId);

/*
* ���������� ��ȡ˫�����������һ���������ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��·��ǰ����
*/
UINT16 GetDMultSwitchOtherId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID�����������
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������      
*/
UINT8 GetMultSwitchType(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID������������������ID
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ID      
*/
UINT16 GetMultSwitchPhySectionId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID������߶�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetMultSwitchDbRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID������߷�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetMultSwitchFbRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID��������ʲټ̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetMultSwitchYcRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID������߶��ټ̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetMultSwitchDcRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID������߷��ټ̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �ʲټ̵���ID      
*/
UINT16 GetMultSwitchFcRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID��������ܶ�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetMultSwitchZDbRelayId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID��������ܷ�λ��ʾ�̵���ID
* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ��λ��ʾ�̵���ID      
*/
UINT16 GetMultSwitchZFbRelayId(const UINT16 switchId);

/*
* ����������	�������Ľ��������������
* ����˵����	��
* ����ֵ  ��	CI_SUCCESS,����ɹ�
*				CI_ERROR,����ʧ��
*/
UINT8 MultSwitchConfigDataManage(void);

/*
* ����������	�������������������������������
* ����˵����	switchSum �����������
*				switchIdBuff �����������͸��������Ľ������ID��������
* ����ֵ  ��	1,����ɹ�
*				0,����ʧ��
*/
UINT8 AddExtendMultSwitchIdDataManage(UINT16 switchSum,const UINT16 *switchIdBuff);

/*
* ���������� ��ȡ�����������͸��������Լ�������������ߵ�ǰ����
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������������������͸��������źŻ�����
*/
UINT16 GetTotalMultSwitchCurNum(void);

/*
* ���������� ��ȡ���ϼ̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchGZJId(const UINT16 switchId);

/*
* ���������� ��ȡ�ֳ�״̬�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchJTYJaId(const UINT16 switchId);
/*
* ���������� ��ȡ�ֳ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchQQJId(const UINT16 switchId);
/*
* ���������� ��ȡ���ռ̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchSJFId(const UINT16 switchId);
/*
* ���������� ��ȡͬ���ֳ����Ƽ̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchTYJId(const UINT16 switchId);
/*
* ���������� ��ȡ����Nλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeNId(const UINT16 switchId);
/*
* ���������� ��ȡ����Lλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeLId(const UINT16 switchId);
/*
* ���������� ��ȡ����Rλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeRId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪Nλ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveNId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪L1λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveL1Id(const UINT16 switchId);
/*
* ���������� ��ȡ�忪L2λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveL2Id(const UINT16 switchId);
/*
* ���������� ��ȡ�忪R1λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveR1Id(const UINT16 switchId);
/*
* ���������� ��ȡ�忪R2λ��ʾ�̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveR2Id(const UINT16 switchId);
/*
* ���������� ��ȡ����Nλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeNYCId(const UINT16 switchId);
/*
* ���������� ��ȡ����Lλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeLYCId(const UINT16 switchId);
/*
* ���������� ��ȡ����Rλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchThreeRYCId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪Nλ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveNYCId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪L1λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveL1YCId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪L2λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveL2YCId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪R1λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveR1YCId(const UINT16 switchId);
/*
* ���������� ��ȡ�忪R2λ�����̵���ID
* ����˵���� void    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: ������ߵ�ǰ����
*/
UINT16 GetMultSwitchFiveR2YCId(const UINT16 switchId);

/*
* ���������� ��ȡָ��ID�������DFH�̵���ID
	* ����˵���� const UINT16 switchId, �������ID    
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: DFH�̵���ID      
*/
UINT16 GetMultSwitchDFHRelayId(const UINT16 switchId);

/*
* ���������� ��ȡ������·ָ��ID��������������������Ŀ
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �������������Ŀ      
*/
UINT16 GetMNRailMultSwitchPhySectionNum(const UINT16 switchId);

/*
* ���������� ��ȡ������·ָ��ID������������������ID����
* ����˵���� const UINT16 switchId, �������ID
* ����ֵ  �� 0: ��ȡ����ʧ��
*			>0: �����������ID����      
*/
UINT16* GetMNRailMultSwitchPhySectionIdBuf(const UINT16 switchId);
#ifdef __cplusplus
}
#endif

#endif
