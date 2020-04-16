/************************************************************************
*
* �ļ���   ��  FFFE.c
* ��Ȩ˵�� ��  ������ʱ���Ƽ��������ι�˾ 
* �汾��   ��  1.0  
* ����ʱ�� ��  2009.08.17
* ����     ��  ���ؼ�Э�鲿
* �������� ��  FFFEЭ��ת�弰�����ļ� 
* ʹ��ע�� ��  ���ܴ���ĳ���Ϊ65535����Ϊ�ܳ��ȵ�����UINT16���ơ�
* �޸ļ�¼ ��  20120706,������FFFEPack����
*
************************************************************************/

#include "FFFE.h"
#include "stdlib.h"

/*FFFE��ת��״̬*/
#define WAIT_FF_C1 0x00
#define WAIT_FF_C2 0x01
#define WAIT_NO_FF 0x02
#define WAIT_FF_POS 0x03
#define ABNORMAL_STATE 0x04

/*FFFE���ݽ���״̬*/
#define COM_WAIT_START_FF 0x00
#define COM_WAIT_START_FE 0x01
#define COM_WAIT_STOP_FF 0x02
#define COM_WAIT_STOP_FD 0x03

/*
* ���������� �����ݽ���FFFEת�崦������ͷFFFE��βFFFD����
* ��������� const UINT8 *pSrc    ����(��FFFEת��)
*            UINT16 SrcLen        ����(��FFFEת��)�ĳ���
* ��������� UINT8 *pTgt          ����(����FFFEת��)
* ����ֵ��   ת���ĳ���   
*/
UINT16 TranslateToFFFE(const UINT8 *pSrc, UINT8 *pTgt,UINT16 SrcLen)
{
    UINT16 SrcPos = 0;
    UINT16 TgtPos = 0;
    UINT16 Pos1 = 0;/*ǰһ��FF��λ��*/
    UINT16 Pos2 = 0;/*��һ��FF��λ��*/
    UINT16 iii = 0;/*��ʱ��������*/
    UINT16 Gap = 0;/*����FF֮����ֽ���*/
    UINT8 Got1stFF = 0;/*1-���ǰһ��FF��0-δ���ǰһ��FF*/
    UINT8 Got2ndFF = 0;/*1-��ú�һ��FF��0-δ��ú�һ��FF*/

    for(SrcPos=0;SrcPos<SrcLen;SrcPos++)
    {
        if(1 == Got1stFF)
        {
            if(0xFF == pSrc[SrcPos])
            {
                /*������ǰһ��FF���ҵ�ǰ������FF*/
                Got2ndFF = 1;
                Pos2 = SrcPos;
                pTgt[TgtPos]=Pos2-Pos1;
                TgtPos++;

                for(iii=Pos1+1;iii<Pos2;iii++)
                {
                    pTgt[TgtPos]=pSrc[iii];
                    TgtPos++;
                }

                Got1stFF=0;
                Got2ndFF=0;
                Pos1=0;
                Pos2=0;
                Gap=0;
            }
            else
            {
                /*������ǰһ��FF���ҵ�ǰ������FF*/
                Gap++;
                if(252 == Gap)
                {
                    Got1stFF=0;
                    Gap=0;
                    pTgt[TgtPos]=0;
                    TgtPos++;
                    for(iii=Pos1+1;iii<=SrcPos;iii++)
                    {
                        pTgt[TgtPos]=pSrc[iii];
                        TgtPos++;
                    }
                }
                else
                {
                }
            }
        }
        else
        {
            /*��δ����ǰһ��FF*/
            pTgt[TgtPos]=pSrc[SrcPos];
            TgtPos++;
            if(0xFF == pSrc[SrcPos])   
            {
                /*����ǰһ��FF*/
                Got1stFF=1;
                Pos1=SrcPos;
                Gap=0;
            }
            else
            {
            }
        }
    }
    /*�Ѿ�������ǰһ��FF, ��Դ�����ѵ���ĩβ��δ������һ��FF*/
    if(1 == Got1stFF)
    {
        pTgt[TgtPos] = 0;
        TgtPos++;

        for(iii=Pos1+1;iii<SrcLen;iii++)
        {
            pTgt[TgtPos]=pSrc[iii];
            TgtPos++;
        }
    }
    else
    {
    }
    return TgtPos;
}


/*
* ����������  ������FFFEת�崦�������ݣ��Ѿ�ȥ����ͷFFFE��βFFFD����ԭΪԭʼ����
* ���������  const UINT8 *pSrc,    ����(FFFEת��)
*             UINT16 SrcLen,        ����(FFFEת��)�ĳ���
* ���������  UINT8 *pTgt,          ����(ȥ��FFFEת��)
* ����ֵ��    ����0,  ת��ɹ�,����ת�������ݳ���
*             0,   �в���ת����ַ���ת��ʧ�ܡ�����ת�������ݳ���Ϊ0   
*/
UINT16 TranslateFromFFFE(const UINT8 *pSrc,UINT8 *pTgt,UINT16 SrcLen)
{
    UINT16 SrcPos=0;
    UINT16 TgtPos=0;
    UINT16 NonFFCount=0;/*����WAIT_NO_FF״̬�Ĵ���*/
    UINT16 NextFFPos=0;/*��һ��FF�ַ���λ��*/
    UINT8 Char=0;/*��ǰ�ַ�ֵ*/
    UINT8 State=WAIT_FF_C1;/*״̬*/

    for(SrcPos=0;SrcPos<SrcLen;SrcPos++)
    {
        Char=pSrc[SrcPos];
        switch(State)
        {
        case WAIT_FF_C1:
            /*�ȴ�FF���е�һFF�ַ�*/

            if(0xFF == Char)  
            {
                /*�յ�FF�ַ�*/
                pTgt[TgtPos]=Char;
                TgtPos++;
                State = WAIT_FF_C2;
            }
            else
            {
                /*��ǰ��FF�ַ�*/
                pTgt[TgtPos]=Char;
                TgtPos++;
            }

            break;
        case WAIT_FF_C2:
            /*�ȴ�FF���еڶ�FF�ַ�*/

            if( (0xFD==Char) || (0xFE==Char) || (0xFF==Char) )
            {
                State = ABNORMAL_STATE;
            }
            else
            {
                if(0 == Char)
                {
                    State = WAIT_NO_FF;
                }
                else
                {
                    if(1 == Char)
                    {
                        pTgt[TgtPos]=0xff;
                        TgtPos++;
                        State = WAIT_FF_C1;
                    }
                    else
                    {
                        NextFFPos=SrcPos+Char-1;
                        State = WAIT_FF_POS;
                    }
                }
            }
            break;
        case WAIT_NO_FF:
            /*��Ϣ�в�����FF�ַ���Ҫ����*/

            NonFFCount++;
            if( (0xFF==Char) && (NonFFCount<=252) )
            {
                /*�����252���ַ�֮�ڴ���FF������ʧ��*/
                State = ABNORMAL_STATE;
            }
            else
            {
                /*���ܷ�FF�ַ�*/
                pTgt[TgtPos]=Char;
                TgtPos++;

                if(252 == NonFFCount)
                {
                    NonFFCount=0;
                    State = WAIT_FF_C1;
                }
                else
                {
                }
            }
            break;
        case WAIT_FF_POS:
            /*�ȴ�����FF�ַ���λ��*/

            if(0xFF == Char)
            {
                /*��ǰ�ַ���FF�ַ�*/

                State = ABNORMAL_STATE;
            }
            else
            {
                /*��ǰ�ַ���FF�ַ�*/

                pTgt[TgtPos]=Char;
                TgtPos++;
                if(SrcPos == NextFFPos)
                {
                    pTgt[TgtPos]=0xff;
                    TgtPos++;
                    State = WAIT_FF_C1;
                }
                else
                {
                }
            }
            break;
        default:
            State = ABNORMAL_STATE;
            break;
        }
        if(State==ABNORMAL_STATE)
        {
            /*�Ƿ�״̬*/
            TgtPos=0;
            break;
        }
        else
        {
        }
    }

    /*�˳�ʱ��״̬�ж�*/
    if((State == WAIT_FF_C2)||(State ==WAIT_FF_POS))
    {
        TgtPos = 0;
    }
    else
    {

    }
    return TgtPos;
}

/*
* ����������  FFFE�ṹ���ʼ������
* ���������  UINT16 FFFEBufferSize, ���ڽ���һ֡���ݵĻ���������   
*             FFFE_STATE_STRU *pFFFEStatStru, FFFE״̬�ṹ��
* ����ֵ��    0,����ִ��ʧ��
*             1,����ִ�гɹ�
*/
UINT8 FFFEInit(FFFE_STATE_STRU *pFFFEStatStru,UINT16 FFFEBufferSize)
{
	
		UINT8 ret;
    /*FFFE״̬���㸴λ*/
    pFFFEStatStru->RawDataRcvBufferSize=FFFEBufferSize;
    pFFFEStatStru->pRawDataRcvBuf=(UINT8*)malloc(pFFFEStatStru->RawDataRcvBufferSize*sizeof(UINT8));
		if (pFFFEStatStru->pRawDataRcvBuf==NULL)
		{
			ret=0;
			return ret;
		}
    pFFFEStatStru->RcvState=COM_WAIT_START_FF;
    pFFFEStatStru->GotFrame=0;
    pFFFEStatStru->DataCount=0;

	ret=1;
	return ret;
}

/*
* ����������   ���ַ����մ���ת������ݣ������֡FFFE���ģ�����FFFE��FFFD����
* ���������   UINT8 RcvChar,      ��ǰ���յ��ַ�
*              PORT_STATE_STRU *pFFFEStatStru,
* ���������   UINT8 *pRawDataRcv,  ���ݽ��ջ�����ָ��
* ����ֵ��     ����0,  ��ʾ�յ�����֡�����ݳ��ȣ������յ�������֡���������ջ�������
*              0,   δ�յ���֡���ݣ� 
*/

UINT16 FFFERcv(FFFE_STATE_STRU *pFFFEStatStru,UINT8 RcvChar, UINT8 *pRawDataRcv)
{

    UINT16 ReturnVal = 0;
    UINT16 iii = 0;

    /*���н���״̬��*/
    switch(pFFFEStatStru->RcvState)
    {
    case COM_WAIT_START_FF:

        /*�ȴ����տ�ʼFF״̬*/
        if(0xFF != RcvChar)
        {

        }
        else
        {
            pFFFEStatStru->RcvState = COM_WAIT_START_FE;
        }
        break;
    case COM_WAIT_START_FE:

        /*�ȴ����տ�ʼFE״̬*/
        if(0xFE != RcvChar)
        {
            pFFFEStatStru->RcvState = COM_WAIT_START_FF;
        }
        else
        {
            pFFFEStatStru->DataCount = 0;
            pFFFEStatStru->GotFrame = 0;
            pFFFEStatStru->RcvState = COM_WAIT_STOP_FF;
        }            
        break;
    case COM_WAIT_STOP_FF:

        /*�ȴ�����ֹͣFF״̬*/
        if(0xFF == RcvChar)
        {
            pFFFEStatStru->RcvState = COM_WAIT_STOP_FD;
        }
        else
        {

            if(pFFFEStatStru->DataCount >= pFFFEStatStru->RawDataRcvBufferSize)
            {
                pFFFEStatStru->DataCount = 0;
                pFFFEStatStru->GotFrame = 0;
                pFFFEStatStru->RcvState = COM_WAIT_START_FF;
            }
            else
            {     
            	  pFFFEStatStru->pRawDataRcvBuf[pFFFEStatStru->DataCount] = RcvChar; 
                pFFFEStatStru->DataCount++;

            }
        }
        break;
    case COM_WAIT_STOP_FD:

        /*�ȴ�����ֹͣFD״̬*/
        switch(RcvChar)
        {
        case 0xFF:
            pFFFEStatStru->DataCount = 0;
            pFFFEStatStru->GotFrame = 0;
            pFFFEStatStru->RcvState = COM_WAIT_START_FF;                 
            break;
        case 0xFE:
            pFFFEStatStru->DataCount = 0;
            pFFFEStatStru->GotFrame = 0;
            pFFFEStatStru->RcvState = COM_WAIT_STOP_FF;                 
            break;
        case 0xFD:
            pFFFEStatStru->GotFrame = 1;
            pFFFEStatStru->RcvState = COM_WAIT_START_FF;
            break;
        default:

            if(pFFFEStatStru->DataCount >= (pFFFEStatStru->RawDataRcvBufferSize-1))
            {
                pFFFEStatStru->DataCount = 0;
                pFFFEStatStru->GotFrame = 0;
                pFFFEStatStru->RcvState = COM_WAIT_START_FF;
            }
            else
            {  
            	  pFFFEStatStru->pRawDataRcvBuf[pFFFEStatStru->DataCount] = 0xFF; 
                pFFFEStatStru->DataCount++;
                pFFFEStatStru->pRawDataRcvBuf[pFFFEStatStru->DataCount] = RcvChar; 
                pFFFEStatStru->DataCount++;
                pFFFEStatStru->RcvState = COM_WAIT_STOP_FF; 
            }
            break;
        }
        break;
    default:
        break;
    }

    /*�յ�����֡*/
    if(1 == pFFFEStatStru->GotFrame)
    {
        for(iii=0; iii<pFFFEStatStru->DataCount; iii++)
        {
            pRawDataRcv[iii] = pFFFEStatStru->pRawDataRcvBuf[iii];
        }

        ReturnVal = pFFFEStatStru->DataCount;
        pFFFEStatStru->DataCount = 0;
        pFFFEStatStru->GotFrame = 0;
    }
    else
    {
    }

    return ReturnVal;
}

/*
* ����������  FFFE״̬������
* ���������  PORT_STATE_STRU *pFFFEStatStru, FFFE״̬�ṹ��
* ����ֵ��     
*/
void FFFEFree(FFFE_STATE_STRU *pFFFEStatStru)
{
    /*FFFE����*/
    free (pFFFEStatStru->pRawDataRcvBuf);
    pFFFEStatStru->pRawDataRcvBuf=NULL;

}


/*
* ���������� �����ݽ���FFFEת�崦��,��ͷFFFE��βFFFD
* ��������� const UINT8 *pSrc    ����(��FFFEת��)
*            UINT16 SrcLen        ����(��FFFEת��)�ĳ���
* ��������� UINT8 *pTgt          ����(����FFFEת��),�����FFFEͷ��FFFDβ
* ����ֵ��   ת���ĳ���   
*/
UINT16 FFFEPack(const UINT8 *pSrc, UINT8 *pTgt,UINT16 SrcLen)
{
	UINT16 PackDataLen=0;
	
	/*���FFFEͷ*/
	pTgt[PackDataLen]=0xFF;
	PackDataLen++;
	pTgt[PackDataLen]=0xFE;
	PackDataLen++;

	/*����ת��*/
	PackDataLen += TranslateToFFFE(pSrc, pTgt+PackDataLen,SrcLen);

	/*���FFFDβ*/
	pTgt[PackDataLen]=0xFF;
	PackDataLen++;
	pTgt[PackDataLen]=0xFD;
	PackDataLen++;

	return PackDataLen;


}

