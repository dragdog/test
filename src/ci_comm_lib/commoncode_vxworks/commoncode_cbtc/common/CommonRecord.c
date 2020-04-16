
/*******************************************************************************
* �ļ���    ��  CommonRecord.c
* ��Ȩ˵��  ��  
* �汾��    ��  1.0  
* ����ʱ��  ��  2009.11.12
* ����      ��  ���ؼ�Э�鲿
* ��������  ��  ��¼������Դ�ļ����ɵ��ø��ļ��еĺ������ַ��ͻ�������͵�����д���¼����
                ��¼����ǰ��λΪ��ǰд��������ֽ������ӵ������ֽڿ�ʼΪ��Ҫд���¼���������
* ʹ��ע��  �� 
                ��
* �޸ļ�¼  ��  
                ��
*******************************************************************************/


#include "CommonRecord.h"

#define WR_FILE_PROTECT

static const UINT8 HexTable[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/******************************************************************************************
* ��������      : ���ַ���ʽ���ļ���¼������д��һ���ַ���������ʽ�ļ�¼��ֱ�Ӳ鿴��
                  ��¼�����д洢������Ҫ��¼�����ݵ�ASCII�롣
                  ��{'a'}���洢Ϊ{97,��0x61}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT8 c                        ��Ҫд����ַ�
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintFilec(UINT8 *RecArray, UINT16 RecArraySize, UINT8 c)/* %RELAX<MISRA_8_10> */
{
    UINT16  ArrayIndex;
    UINT8  ret;
    
    if(RecArray==NULL)
    {
    	ret = 0;
    	return ret;
    }
    
    ArrayIndex = ShortFromChar(RecArray) + 2;
#ifdef WR_FILE_PROTECT
    if((RecArraySize - ArrayIndex) < 10)
    {
        ret = 0;
        return ret;
    }
    else
    {
        RecArray[ArrayIndex++] = c;
        ShortToChar((UINT16) (ArrayIndex - 2),RecArray);
    }
#else
    RecArray[ArrayIndex++] = c;
    ShortToChar((UINT16) (ArrayIndex - 2),RecArray);
#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : ���ַ���ʽ���ļ���¼������д��N��Hex��������ʽ�ļ�¼��ֱ�Ӳ鿴��
                  ��¼�����д洢������Ҫ��¼�����ݵ�ASCII�롣
                  ��{0x12,0x34}���洢Ϊ{0x31��0x32��0x33��0x34}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT8 *HexSrc                  ��Ҫд��Hex���ݵ�ͷָ��
                  UINT16 Length                  ��Ҫд��Hex���ݵĳ���
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintFileHex(UINT8 *RecArray, UINT16 RecArraySize, UINT8 *HexSrc, UINT16 Length)/* %RELAX<MISRA_8_10> */
{
    UINT16 i;
    UINT8 ret;
    

	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

#ifdef WR_FILE_PROTECT
    for(i = 0; i < Length; i++)
    {
        if(PrintFilec(RecArray, RecArraySize, HexTable[(*HexSrc)>>4]) == 0)
        {
            ret = 0;
            return ret;
        }
        else
        {
            /*д��ɹ�*/    
        }
        
        if(PrintFilec(RecArray, RecArraySize, HexTable[(*HexSrc) & 0x0f]) == 0)
        {
            ret = 0;
            return ret;
        }
        else
        {
            /*д��ɹ�*/    
        }
        
        if(PrintFilec(RecArray, RecArraySize,' ') == 0)
        {
            ret = 0;
            return ret;
        }
        else
        {
            /*д��ɹ�*/    
        }
        
        HexSrc++;
    }
    ret = 1;
    return ret;
#else
    for(i = 0; i < Length; i++)
    {
        PrintFilec(RecArray, RecArraySize,HexTable[(*HexSrc)>>4]);
        PrintFilec(RecArray, RecArraySize,HexTable[(*HexSrc) & 0x0f]);
        PrintFilec(RecArray, RecArraySize,' ');
        HexSrc++;
    }
    ret = 1;
    return ret;
#endif
}


/******************************************************************************************
* ��������      : ���ַ���ʽ���ļ���¼������д��һ���ַ�����������ʽ�ļ�¼��ֱ�Ӳ鿴��
                  ��¼�����д洢������Ҫ��¼�����ݵ�ASCII�롣
                  ��"1234"���洢Ϊ{0x31��0x32��0x33��0x34}={"1234"}
* �������      : UINT16 RecArraySize            ��¼�����С
                  CHAR *StringSrc                ��Ҫд���ַ�����ͷָ��
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintFiles(UINT8 *RecArray, UINT16 RecArraySize, CHAR *StringSrc)/* %RELAX<MISRA_8_10> */
{
    UINT8 ret;
    
	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

#ifdef WR_FILE_PROTECT
    while(*StringSrc != 0)
    {
        if(PrintFilec(RecArray, RecArraySize, *StringSrc) == 0)
        {
            ret = 0;
            return ret;
        }
        else
        {
            /*д��ɹ�*/    
        }
        
        StringSrc++;
    }
    ret = 1;
    return ret;
#else
    while(*StringSrc != 0)
    {
        PrintFilec(RecArray, RecArraySize, *StringSrc);
        StringSrc++;
    }
    ret = 1;
    return ret;
#endif
}



/******************************************************************************************
* ��������      : ���ַ���ʽ���ļ���¼������д��һ��Hex�͵�short(16bit)��������ʽ�ļ�¼��ֱ�Ӳ鿴��
                  ��¼�����д洢������Ҫ��¼�����ݵ�ASCII�롣
                  ��{0x1234}���洢Ϊ{0x31��0x32��0x33��0x34}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT16 HexSrc                  ��Ҫд���Short������
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintFileU16Hex(UINT8 *RecArray, UINT16 RecArraySize, UINT16 HexSrc)/* %RELAX<MISRA_8_10> */
{
    UINT16 j;
    CHAR TempChar[7];
    UINT8 ret;
  
	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

    for(j = 4; j > 0; j--)
    {
        TempChar[j] = HexTable[HexSrc & 0x0f];
        HexSrc = HexSrc >> 4;
    }
    TempChar[0] = 'x';
    TempChar[5] = ' ';
    TempChar[6] = '\0';
    
#ifdef WR_FILE_PROTECT
    if(PrintFiles(RecArray, RecArraySize, TempChar) == 0)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*д��ɹ�*/    
    }
#else
    PrintFiles(RecArray, RecArraySize, TempChar);
#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* ��������      : ���ַ���ʽ���ļ���¼������д��һ��Hex�͵�long(32bit)��������ʽ�ļ�¼��ֱ�Ӳ鿴��
                  ��¼�����д洢������Ҫ��¼�����ݵ�ASCII�롣
                  ��{0x12345678}���洢Ϊ{0x31��0x32��0x33��0x34,0x35,0x36,0x37,0x38}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT32 HexSrc                  ��Ҫд���long������
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintFileU32Hex(UINT8 *RecArray, UINT16 RecArraySize, UINT32 HexSrc)/* %RELAX<MISRA_8_10> */
{
    UINT16 j;
    CHAR TempChar[11];
    UINT8 ret;
    
	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

    for(j = 8; j > 0; j--)
    {
        TempChar[j] = HexTable[HexSrc & 0x0f];
        HexSrc = HexSrc >> 4;
    }
    TempChar[0] = 'x';
    TempChar[9] = ' ';
    TempChar[10] = '\0';
    
#ifdef WR_FILE_PROTECT
    if(PrintFiles(RecArray, RecArraySize, TempChar) == 0)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/    
    }
#else
    PrintFiles(RecArray, RecArraySize, TempChar);
#endif

    ret = 1;
    return ret;
}



/******************************************************************************************
* ��������      : ���ַ���ʽ���ļ���¼������д��һ��Int�͵�long(16bit)��������ʽ�ļ�¼��ֱ�Ӳ鿴��
                  ��¼�����д洢������Ҫ��¼�����ݵ�ASCII�롣
                  ��{305397760��ʮ�����Ʊ�ʾΪ0x12340000}���洢Ϊ{0x31��0x32��0x33��0x34,0x30,0x30,0x30,0x30}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT32 IntSrc                  ��Ҫд���int������
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintFileU32Int(UINT8 *RecArray, UINT16 RecArraySize, UINT32 IntSrc)/* %RELAX<MISRA_8_10> */
{
    INT16 i;
    CHAR char_temp[11]={0};
    UINT8 ret;
    
	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

    for(i=9;i>=0;i--)
    {
        char_temp[i]='0'+ (UINT8)(IntSrc%10);
        IntSrc/=10;
    } 

    
    for(i=0;i<9;i++)
    {
        if(char_temp[i]!='0')
        {
            break;    
        }
        else
        {
            /*�޲���*/    
        }        
    }
    char_temp[10] = '\0';
#ifdef WR_FILE_PROTECT
    if(PrintFiles(RecArray, RecArraySize, &char_temp[i]) == 0)
    {
        ret = 0;
        return ret;
    }
    else
    {
        /*�޲���*/    
    }
#else
    PrintFiles(RecArray, RecArraySize, &char_temp[i]);
#endif

    ret = 1;
    return ret;
}



/******************************************************************************************
* ��������      : �Զ�������ʽ���ļ���¼������д��һ���ֽ�
                  ��������ʽ������{0x12}���洢Ϊ{0x12}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT8 ByteData                 ��Ҫд����ֽ�
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintRecArrayByte(UINT8 *RecArray, UINT16 RecArraySize,  UINT8 ByteData)/* %RELAX<MISRA_8_10> */
{
    register UINT16 RecLength;
    UINT8 ret;
    
	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

    RecLength = ShortFromChar(RecArray);
    RecLength += 2;

    if(RecLength + 1 > RecArraySize)   /*�õ����鳤�ȣ�ǰһ��䳤��+ 2���Ǽ�¼�����ݼӳ���ռ�ݵĿռ��ټ�1�ֽ�д��� > RecArraySize ���ܼ���д��*/
    {
        ret = 0;
        return ret;
    }
    else
    {
        RecArray[RecLength] = ByteData;
        RecLength--;    /*��ȥ�����ֽ���+1*/
        ShortToChar(RecLength,RecArray);
        ret = 1;
        return ret;
    }
}



/******************************************************************************************
* ��������      : �Զ�������ʽС�˷�ʽ���ļ���¼������д�������ֽ�
                  ��������ʽ������{0x1234}���洢Ϊ{0x34��0x12}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT16 BytesData               ��Ҫд����ֽ�
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintRecArray2ByteL(UINT8 *RecArray, UINT16 RecArraySize, UINT16 BytesData)/* %RELAX<MISRA_8_10> */
{
    register UINT16 RecLength;
    UINT8 ret;
    
	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

    RecLength = ShortFromChar(RecArray);
    RecLength += 2;
  
    if(RecLength + 2 > RecArraySize)
    {
        ret = 0;
        return ret;
    }
    else
    {
        RecArray[RecLength++] = BytesData & 0xff;
        RecArray[RecLength] = (BytesData >> 8) & 0xff;
        RecLength--;
        ShortToChar(RecLength,RecArray);
        ret = 1;
        return ret;
    }
}



/******************************************************************************************
* ��������      : �Զ�������ʽ��˷�ʽ���ļ���¼������д�������ֽ�
                  ��������ʽ������{0x1234}���洢Ϊ{0x12��0x34}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT16 BytesData               ��Ҫд����ֽ�
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintRecArray2ByteB(UINT8 *RecArray, UINT16 RecArraySize, UINT16 BytesData)/* %RELAX<MISRA_8_10> */
{
    register UINT16 RecLength;
    UINT8 ret;
    
	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

    RecLength = ShortFromChar(RecArray);
    RecLength += 2;

    if(RecLength + 2 > RecArraySize)
    {
        ret = 0;
        return ret;
    }
    else
    {
        RecArray[RecLength++] = (BytesData >> 8) & 0xff;
        RecArray[RecLength] = BytesData & 0xff;
        RecLength--;
        ShortToChar(RecLength,RecArray);
        ret = 1;
        return ret;
    }
}



/******************************************************************************************
* ��������      : �Զ�������ʽС�˷�ʽ���ļ���¼������д���ĸ��ֽ�
                  ��������ʽ������{0x12345678}���洢Ϊ{0x78��0x56��0x34��0x12}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT32 BytesData               ��Ҫд����ֽ�
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintRecArray4ByteL(UINT8 *RecArray, UINT16 RecArraySize, UINT32 BytesData)/* %RELAX<MISRA_8_10> */
{
    register UINT16 RecLength;
    UINT8 ret;
    
	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

    RecLength = ShortFromChar(RecArray);
    RecLength += 2;

    if(RecLength + 4 > RecArraySize)
    {
        ret = 0;
        return ret;
    }
    else
    {
        RecArray[RecLength++] = (UINT8) (BytesData & 0xff);
        RecArray[RecLength++] = (UINT8) ((BytesData >> 8) & 0xff);
        RecArray[RecLength++] = (UINT8) ((BytesData >> 16) & 0xff);
        RecArray[RecLength] = (UINT8) ((BytesData >> 24) & 0xff);
        RecLength--;
        ShortToChar(RecLength,RecArray);
        ret = 1;
        return ret;
    }
}



/******************************************************************************************
* ��������      : �Զ�������ʽ��˷�ʽ���ļ���¼������д���ĸ��ֽ�
                  ��������ʽ������{0x12345678}���洢Ϊ{0x12��0x34,0x56,0x78}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT32 BytesData               ��Ҫд����ֽ�
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintRecArray4ByteB(UINT8 *RecArray, UINT16 RecArraySize, UINT32 BytesData)/* %RELAX<MISRA_8_10> */
{
    register UINT16 RecLength;
    UINT8 ret;
    
	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

    RecLength = ShortFromChar(RecArray);
    RecLength += 2;

    if(RecLength + 4 > RecArraySize)
    {
        ret = 0;
        return ret;
    }
    else
    {
        RecArray[RecLength++] = (UINT8) ((BytesData >> 24) & 0xff);
        RecArray[RecLength++] = (UINT8) ((BytesData >> 16) & 0xff);
        RecArray[RecLength++] = (UINT8) ((BytesData >> 8) & 0xff);
        RecArray[RecLength] = (UINT8) (BytesData & 0xff);
        RecLength--;
        ShortToChar(RecLength,RecArray);
        ret = 1;
        return ret;
    }
}



/******************************************************************************************
* ��������      : �Զ�������ʽ���ļ���¼������д��N���ֽ�
                  ��������ʽ������{0x12,0x34}���洢Ϊ{0x12��0x34}
* �������      : UINT16 RecArraySize            ��¼�����С
                  UINT8 *pBytesData              ��Ҫд������ݵ�ͷָ��
                  UINT16 Length                  ��Ҫд����ֽ���
* �����������  : UINT8 *RecArray                ��¼�����ͷָ��
* �������      :
* ȫ�ֱ���      : 
* ����ֵ        : д����������1����¼�ռ䲻������0        
*******************************************************************************************/
UINT8 PrintRecArrayNByte(UINT8 *RecArray, UINT16 RecArraySize,UINT8 *pBytesData, UINT16 Length)/* %RELAX<MISRA_8_10> */
{
    register UINT16 RecLength;
    UINT16 i;
    UINT8 ret;
    
	if(RecArray==NULL)
	{
		ret = 0;
		return ret;
	}

    RecLength = ShortFromChar(RecArray);
    RecLength += 2;
  
    if(RecLength + Length > RecArraySize)
    {
        ret = 0;
        return ret;
    }
    else
    {
        for(i = 0; i < Length; i++)
        {
            RecArray[RecLength++] = *(pBytesData++);
        }
        RecLength -= 2;
        ShortToChar(RecLength,RecArray);
        ret = 1;
        return ret;
    }
}

