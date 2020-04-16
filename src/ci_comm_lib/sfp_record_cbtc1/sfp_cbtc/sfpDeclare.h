
/*******************************************************************************
* �ļ���    ��  Declare.h
* ��Ȩ˵��  ��  
* �汾��    ��  1.0  
* ����ʱ��  ��  2008.07.29
* ����      ��  
* ��������  ��  ���ļ�ΪЭ�鹫��ͷ�ļ������ֵĺ궨�嶼�ڴ��ļ�����������
                ���ļ���Ӧ�������ļ�
* ʹ��ע��  �� 
                ��
* �޸ļ�¼  ��  
                ��
*******************************************************************************/


#ifndef _SFPDECLARE_H
#define _SFPDECLARE_H


/********************************************************************************
�궨��
********************************************************************************/



/******��·�����궨��**************/
#define ROLE_SPON                           0x00                    /*����*/
#define ROLE_FLW                            0x01                    /*���淽*/	

#define STATUS_DATA                         0x17                    /*��·״̬������״̬*/
#define STATUS_HALT                         0x2b                    /*��·״̬��ͨ���ж�״̬*/
#define STATUS_PRECON                       0x33                    /*��·״̬��׼����������״̬*/
#define STATUS_WCONACK                      0x4e                    /*��·״̬���ȴ���������״̬*/

#define COMMTYPE_MULTICOMM                  0x33                    /*ͨ�����ͣ��鲥*/
#define COMMTYPE_P2P                        0x55                    /*ͨ��״̬����Ե�*/

/*֡����*/
#define FRAME_RFC                           0x17                    /*��������֡*/
#define FRAME_ACK                           0x2b                    /*����ȷ��֡*/
#define FRAME_DATA                          0x35                    /*����֡*/
#define FRAME_IDLE                          0x59                    /*ά�����ӱ���*/

/************************************************************************
 * ���������ԭ��Э�������һ��16bit
 * D15~D12������
 * D11~D10������ȼ�
 * D9~D8��ģ��λ��
 * D7~D4����������ģ�����еĴ�����ģ�鲻ͬʱ��4λ����ĺ��岻ͬ
 * �����ڶ�������ȵײ����
 * D3~D0����������������ģ���Ƿ���ͬ��4λ����ĺ�����ͬ                                                                     
************************************************************************/
/*����ȼ�*/
#define ERR                                 0x0800
#define WARN                                0x0C00

/*ģ��λ��*/
#define  NET_STATUS                         0x0000                  /*���������*/
#define  REDUN                              0x0100                  /*����ģ��*/
#define  RECEIVE                            0x0200                  /*����ģ��*/
#define  OUTPUT                             0x0300                  /*���ģ��*/

/*��ģ�����д���*/
#define ERROR_NULL                          0x0000
/*����*/
#define FRAMETYPE_ROLE_CONFLICT             0x0010                  /*֡��������󲻷�*/
#define SN_ILLEGAL                          0x0020                  /*���кŲ��Ϸ�*/
#define FRAMETYPE_STATUS_CONFLICT           0x0030                  /*֡���ͺ���·״̬���Ϸ�*/
#define RE_RECV_RFC                         0x0040                  /*�ظ��յ�RFC*/
#define CRC_ERR                             0x0050                  /*CRC����*/
#define OBJ_ILLEAGL                         0x0060                  /*ͨ�Ŷ��󲻺Ϸ�*/
#define DESTNAME_ERR                        0x0070                  /*Ŀ��Name����*/
#define OUTNETDATA_TOO_LONG                 0x0080                  /*�������ݳ��ȹ���*/

/*����*/
#define NODE_LOST                           0x0010                  /*������������������ݵ���������û��*/
/*���*/
#define ROLE_STATUS_CONFLICT                0x0010                  /*������ɫ����·״̬�����*/
#define APPDATA_LENGTH_ERR                  0x0020                  /*Ӧ�ø��������ݹ���*/

/*�����������ײ㴦�����*/
#define TIMEOUT                             0x0001                  /*ģ�鳬ʱ*/
#define QUEUE_READ                          0x0002                  /*���ж��д�*/
#define QUEUE_WRITE                         0x0003                  /*����д�д�*/
#define QUEUE_INTEGRITY                     0x0004                  /*���������ݲ�����������scan�д�Ӧ�ø�������*/
#define LNKMANAGE_PUSH                      0x0005                  /*��·�����ڵ�ѹջ����*/
#define LNKMANAGE_POP                       0x0006                  /*��·�����ڵ��ջ����*/
#define INPUT_LNK_PUSH                      0x0007                  /*һά���ݱ�ڵ�ѹջ����*/
#define INPUT_LNK_POP                       0x0008                  /*һά���ݱ�ڵ��ջ����*/
#define OUTPUT_LNK_PUSH                     0x0009                  /*�����������ѹջ����*/
#define OUTPUT_LNK_POP                      0x000a                  /*������������ջ����*/
#define ARRAY_FULL                          0x000b                  /*�������*/



/******������궨��**************/
 
#define ERR_REDUN_INPUT_LNK_POP							0x00000001		/*����ģ��ڵ��ջ����*/
#define ERR_REDUN_TIMEOUT						    			0x00000002		/*����ģ�鳬ʱ����*/
#define WARN_REDUN_QUEUE_INTEGRITY		    			0x00000004		/*����ģ����������ݲ���������*/
#define WARN_REDUN_OUTNETDATA_TOO_LONG    			0x00000008		/*����ģ���������ݳ��ȹ���*/
#define WARN_REDUN_QUEUE_READ								0x00000010		/*����ģ����ж��д���*/
#define WARN_REDUN_FRAMETYPE_ROLE_CONFLICT 		0x00000020		/*����ģ��֡��������󲻷�*/
#define WARN_REDUN_DESTNAME_ERR 							0x00000040		/*����ģ��Ŀ��������*/
#define WARN_REDUN_SN_ILLEGAL								0x00000080		/*����ģ�����кŲ��Ϸ�*/
#define WARN_REDUN_FRAMETYPE_STATUS_CONFLICT 		0x00000100 		/*����ģ��֡���ͺ���·״̬���Ϸ�*/
#define WARN_REDUN_RECV_RFC									0x00000200 		/*����ģ���ظ��յ�RFC*/
#define WARN_REDUN_CRC_ERR										0x00000400		/*����ģ��CRC����*/
#define WARN_REDUN_OBJ_ILLEAGL								0x00000800 		/*����ģ��ͨ�Ŷ��󲻺Ϸ�*/

#define ERR_OUTPUT_TIMEOUT										0x00001000		/*���ģ�鳬ʱ*/
#define ERR_OUTPUT_OUT_LNK_POP								0x00002000 		/*���ģ��������������ջ����*/
#define ERR_OUTPUT_OUT_LNK_PUSH							0x00004000 		/*���ģ�������������ѹջ����*/
#define ERR_OUTPUT_QUEUE_INTEGRITY						0x00008000 		/*���ģ����������ݲ���������*/
#define ERR_OUTPUT_APPDATA_LENGTH_ERR  				0x00010000 		/*���ģ��Ӧ�ø��������ݹ���*/
#define ERR_OUTPUT_QUEUE_READ       						0x00020000		/*���ģ����ж��д�*/
#define ERR_OUTPUT_ARRAY_FULL								0x00040000 		/*���ģ���������*/
#define ERR_OUTPUT_LNKMANAGE_POP							0x00080000 		/*���ģ����·�����ڵ��ջ����*/
#define ERR_OUTPUT_ROLE_STATUS_CONFLICT 				0x00100000 		/*���ģ�鱾����ɫ����·״̬�����*/
#define WARN_OUTPUT_OBJ_ILLEAGL							0x00200000 		/*���ģ��ͨ�Ŷ��󲻺Ϸ�*/

#define ERR_RECEIVE_LNKMANAGE_POP							0x00400000 		/*����ģ����·���������ջ����*/
#define WARN_RECEIVE_FRAMETYPE_STATUS_CONFLICT	0x00800000 		/*����ģ��֡���ͺ���·״̬���Ϸ�*/
#define WARN_RECEIVE_QUEUE_WRITE							0x01000000 		/*����ģ�����д�д���*/
#define WARN_RECEIVE_STATUS_DATATOHALT				0x02000000 		/*����ģ�鳬ʱ������֡תΪ�ж�*/
#define WARN_RECEIVE_DEL_LNK									0x04000000 		/*��ʱɾ��*/

/************************************************/


/****************************Э�����ദ��ʹ�õĺ궨��*********************/
/*�������յ�������������*/
#define RCV_DATA_LEN                        0                       /*0,�������������ݵĳ���*/
#define RCV_SRC_SN                          (RCV_DATA_LEN+2)        /*2,�������������ݵķ��ͷ����к�*/
#define RCV_AIM_SN                          (RCV_SRC_SN+4)          /*6,�������������ݵĽ��շ����к�*/
#define RCV_FRAME_TYPE                      (RCV_AIM_SN+4)          /*10,�������������ݵı�������*/
#define	RCV_AIM_DN                          (RCV_FRAME_TYPE+1)      /*11,�������������ݵ�Ŀ���豸NAME*/
#define	RCV_SRC_DN                          (RCV_AIM_DN+4)          /*15,�������������ݵ�Դ�豸NAME*/
#define	RCV_APP_DATA                        (RCV_SRC_DN+4)          /*19,��������������*/

/*�����ദ������һά�����е�����*/
#define LINK_DATA_LEN                       0                       /*0,һά����ڵ��е����ݵĳ���*/
#define LINK_SRC_SN                         (LINK_DATA_LEN+2)       /*2,һά����ڵ��е����ݵķ��ͷ����к�*/
#define	LINK_AIM_SN                         (LINK_SRC_SN+4)         /*6,һά����ڵ��е����ݵĽ��շ����к�*/
#define	LINK_FRAME_TYPE                     (LINK_AIM_SN+4)         /*10,һά����ڵ��е����ݵı�������*/
#define	LINK_SRC_DN                         (LINK_FRAME_TYPE+1)     /*11,һά����ڵ��е����ݵ�Դ�豸ID*/
#define	LINK_APP_DATA                       (LINK_SRC_DN+4)         /*15,һά����ڵ��е����ݵ�Դ�豸����*/
/***********************************/


/**************Э������궨��*****************************************/
/************Э������ӿ�һά�����и�Ԫ���±�*******/
#define NODATA                              0                       /*����������û��Ӧ������*/
#define LEN_IDX                             0                       /*���������б�֡���ݳ����±�*/
#define SENDSN_IDX                          2                       /*���������з������к��±�*/
#define REVSN_IDX                           6                       /*���������з������к��±�*/
#define FRAMETYPE_IDX                       10                      /*���������б��������±�*/
#define DESTID_IDX                          11                      /*����������Դ�߼�ID�±�*/
#define DESTTYPE_IDX                        14                      /*����������Դ�߼�ID�±�*/
#define DESTData_IDX                        DESTID_IDX+4            /*����������Ӧ�������±�*/
/*#define MAX_TIMEOUTREV                      0x04             */       /*���淽���ʱ���������ݶ�10��*/

/*����*/
#define COM_ERROR                           0                       /*ƽ̨����*/
#define DONE                                0x01                    /*Э�����봦�����*/
/******************************/



/*******************Э������ú궨��*******************/
#define ITEMSRC_BOTH                        0x17                    /*�����������Դ�������ݶ��к���·�����*/
#define ITEMSRC_DATABUFF                    0x33                    /*�����������Դ�������ݶ���*/
#define ITEMSRC_LNKMNGR                     0x55                    /*�����������Դ������·�����*/

#define NEED2SEND                           0x33                    /*��Ҫ��������*/
#define SENT                                0x55                    /*�Ѿ����͹�*/
/**********************************/


#define LNKFRAMELEN                         0x24                    /* �м����һ֡���ݳ��� */
#endif
