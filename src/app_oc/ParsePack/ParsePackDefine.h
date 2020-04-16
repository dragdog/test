/********************************************************
*                                                                                                            
* �� �� ���� ParsePackDefine.h   
* ��Ȩ˵���� �������ؿƼ����޹�˾ 
* ����ʱ�䣺 2012-03-28
* ��    �ߣ� ������
* ���������� �����������ͷ�ļ�  
* ����������
* ʹ��ע�⣺
* �޸ļ�¼��   
*
********************************************************/ 

#ifndef _PARSE_PACK_DEFINE_H_
#define _PARSE_PACK_DEFINE_H_




/*CI��IO*/
#define IOTOCI_DATA_STRU_SUM_MAX		        50     /*IO��CI���ݽṹ���������ֵ*/
#define IOTOCI_COLL_BYTE_SUM_MAX		        4      /*IO��CIһ֡�ɼ������ֽ����ֵ*/
#define CITOIO_DATA_STRU_SUM_MAX		        50     /*CI��IO���ݽṹ���������ֵ*/
#define CITOIO_DRV_BYTE_SUM_MAX		            2      /*CI��IOһ֡���������ֽ����ֵ*/
#define DRV_BOARD_POSITION_MAX					16		/*������λ�����ֵ*/
#define COL_BOARD_POSITION_MAX					32		/*�ɼ���λ�����ֵ*/


#define IO_BYTE_MAX		                        4      /*CI��IOһ֡���������ֽ����ֵ*/


/*CI��ATS*/
#define ATSORXIANDI_TO_CI_CMD_DATA_STRU_SUM_MAX		    50		/*ATS���ֵص�CI���ݽṹ���������ֵ*/
#define CI_TO_ATSORXIANDI_CMD_DATA_STRU_SUM_MAX		    100		/*CI��ATS���ֵ����ݽṹ���������ֵ*/
#define CI_TO_ATS_DATA_LEN_MAX					50000	/*CI��ATS������󳤶�*/
#define CI_TO_XIANDI_DATA_LEN_MAX				50000	/*CI���ֵ�������󳤶�*/

/*CI �� CI*/
#define RECVCI_CMD_DATA_STRU_SUM_MAX		    50		/*CI���������������ݽṹ���������ֵ*/
#define SENDCI_CMD_DATA_STRU_SUM_MAX		    50		/*CI���͵������������ݽṹ���������ֵ*/
#define CI_TO_CI_DATA_LEN_MAX					5000	/*������CI����������ֵ*/
/*#define PHYSEC_IDBUF_SUM_OF_OTHERCI				10*/		/*���������й���Ȩ����������������������*/	

/*CI �� ZC*/
#define CI_TO_ZC_DATA_LEN_MAX					3000	/*������ZC����������ֵ*/
#define ZC_TO_CI_TRAIN_MAX                      50     /*ZC���͸������г������Ŀ*/
#define TRAIN_OCCUPY_SECTION_MAX                20      /*�г�ռ���߼�������Ŀ���ֵ*/


/*CI �� LEU*/
#define LEU_TO_CI_SUM_MAX						9	/*������������ͨ�ŵ�LEU���ֵ*/
#define LEU_TO_CI_DATA_LEN_MAX					500	/*LEU��CI����������ֵ*/
#define CI_TO_LEU_DATA_LEN_MAX					36	/*������LEU������ݶ���ֵ*/

/*CI �� VOBC*/
#define VOBC_TO_CI_SUM_MAX                      50      /*������������ͨ�ŵ�VOBC���ֵ*/
#define VOBC_TO_CI_TRAIN_PSD_MAX                8   /*�����г���������������Ŀ*/
#define VOBC_TO_CI_DATA_LEN_MAX                 10000   /*������VOBC�������ֵ*/

/*CI �� TMC*/
#define TMC_TO_CI_DATA_LEN_MAX                 10000   /*������VOBC�������ֵ*/

/*CI��PLATFORM*/
#define PLATFORM_TO_CI_SUM_MAX                  60      /*������������ͨ�ŵ�PLATFORMϵͳ���ֵ*/
#define PLATFORM_TO_CI_DATA_LEN_MAX             1000
#define PLATFORM_INCLUDE_PSD_MAX_SUM            8       /*һ����վ�����������Ÿ���*/

#endif


