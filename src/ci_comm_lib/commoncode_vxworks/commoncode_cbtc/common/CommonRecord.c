
/*******************************************************************************
* 文件名    ：  CommonRecord.c
* 版权说明  ：  
* 版本号    ：  1.0  
* 创建时间  ：  2009.11.12
* 作者      ：  车载及协议部
* 功能描述  ：  记录函数的源文件，可调用该文件中的函数将字符型或二进制型的数据写入记录数组
                记录数组前两位为当前写入数组的字节数，从第三个字节开始为需要写入记录数组的数据
* 使用注意  ： 
                无
* 修改记录  ：  
                无
*******************************************************************************/


#include "CommonRecord.h"

#define WR_FILE_PROTECT

static const UINT8 HexTable[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

/******************************************************************************************
* 功能描述      : 以字符形式向文件记录数组中写入一个字符，该种形式的记录可直接查看，
                  记录数组中存储的是需要记录的数据的ASCII码。
                  如{'a'}，存储为{97,即0x61}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT8 c                        需要写入的字符
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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
* 功能描述      : 以字符形式向文件记录数组中写入N个Hex，该种形式的记录可直接查看，
                  记录数组中存储的是需要记录的数据的ASCII码。
                  如{0x12,0x34}，存储为{0x31，0x32，0x33，0x34}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT8 *HexSrc                  需要写入Hex数据的头指针
                  UINT16 Length                  需要写入Hex数据的长度
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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
            /*写入成功*/    
        }
        
        if(PrintFilec(RecArray, RecArraySize, HexTable[(*HexSrc) & 0x0f]) == 0)
        {
            ret = 0;
            return ret;
        }
        else
        {
            /*写入成功*/    
        }
        
        if(PrintFilec(RecArray, RecArraySize,' ') == 0)
        {
            ret = 0;
            return ret;
        }
        else
        {
            /*写入成功*/    
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
* 功能描述      : 以字符形式向文件记录数组中写入一个字符串，该种形式的记录可直接查看，
                  记录数组中存储的是需要记录的数据的ASCII码。
                  如"1234"，存储为{0x31，0x32，0x33，0x34}={"1234"}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  CHAR *StringSrc                需要写入字符串的头指针
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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
            /*写入成功*/    
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
* 功能描述      : 以字符形式向文件记录数组中写入一个Hex型的short(16bit)，该种形式的记录可直接查看，
                  记录数组中存储的是需要记录的数据的ASCII码。
                  如{0x1234}，存储为{0x31，0x32，0x33，0x34}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT16 HexSrc                  需要写入的Short型数据
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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
        /*写入成功*/    
    }
#else
    PrintFiles(RecArray, RecArraySize, TempChar);
#endif

    ret = 1;
    return ret;
}


/******************************************************************************************
* 功能描述      : 以字符形式向文件记录数组中写入一个Hex型的long(32bit)，该种形式的记录可直接查看，
                  记录数组中存储的是需要记录的数据的ASCII码。
                  如{0x12345678}，存储为{0x31，0x32，0x33，0x34,0x35,0x36,0x37,0x38}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT32 HexSrc                  需要写入的long型数据
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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
        /*无操作*/    
    }
#else
    PrintFiles(RecArray, RecArraySize, TempChar);
#endif

    ret = 1;
    return ret;
}



/******************************************************************************************
* 功能描述      : 以字符形式向文件记录数组中写入一个Int型的long(16bit)，该种形式的记录可直接查看，
                  记录数组中存储的是需要记录的数据的ASCII码。
                  如{305397760，十六进制表示为0x12340000}，存储为{0x31，0x32，0x33，0x34,0x30,0x30,0x30,0x30}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT32 IntSrc                  需要写入的int型数据
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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
            /*无操作*/    
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
        /*无操作*/    
    }
#else
    PrintFiles(RecArray, RecArraySize, &char_temp[i]);
#endif

    ret = 1;
    return ret;
}



/******************************************************************************************
* 功能描述      : 以二进制形式向文件记录数组中写入一个字节
                  二进制形式举例：{0x12}，存储为{0x12}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT8 ByteData                 需要写入的字节
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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

    if(RecLength + 1 > RecArraySize)   /*得到数组长度，前一语句长度+ 2，是记录的数据加长度占据的空间再加1字节写入的 > RecArraySize 则不能继续写入*/
    {
        ret = 0;
        return ret;
    }
    else
    {
        RecArray[RecLength] = ByteData;
        RecLength--;    /*减去长度字节再+1*/
        ShortToChar(RecLength,RecArray);
        ret = 1;
        return ret;
    }
}



/******************************************************************************************
* 功能描述      : 以二进制形式小端方式向文件记录数组中写入两个字节
                  二进制形式举例：{0x1234}，存储为{0x34，0x12}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT16 BytesData               需要写入的字节
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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
* 功能描述      : 以二进制形式大端方式向文件记录数组中写入两个字节
                  二进制形式举例：{0x1234}，存储为{0x12，0x34}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT16 BytesData               需要写入的字节
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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
* 功能描述      : 以二进制形式小端方式向文件记录数组中写入四个字节
                  二进制形式举例：{0x12345678}，存储为{0x78，0x56，0x34，0x12}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT32 BytesData               需要写入的字节
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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
* 功能描述      : 以二进制形式大端方式向文件记录数组中写入四个字节
                  二进制形式举例：{0x12345678}，存储为{0x12，0x34,0x56,0x78}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT32 BytesData               需要写入的字节
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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
* 功能描述      : 以二进制形式向文件记录数组中写入N个字节
                  二进制形式举例：{0x12,0x34}，存储为{0x12，0x34}
* 输入参数      : UINT16 RecArraySize            记录数组大小
                  UINT8 *pBytesData              需要写入的数据的头指针
                  UINT16 Length                  需要写入的字节数
* 输入输出参数  : UINT8 *RecArray                记录数组的头指针
* 输出参数      :
* 全局变量      : 
* 返回值        : 写入正常返回1，记录空间不够返回0        
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

