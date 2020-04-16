/************************************************************************
*
* 文件名   ：  DataPackage.h
* 版权说明 ：  北京北京交控科技有限公司 
* 版本号   ：  1.0
* 创建时间 ：  2013.01.14
* 作者     ：  王佩佩
* 功能描述 ：  用来将多帧报文合成一帧报文
* 使用注意 ：  
* 修改记录 ：
************************************************************************/

#ifndef _DATA_PACKAGE_H_
#define _DATA_PACKAGE_H_

#include "CommonTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


/* 消息类型定义 */
typedef struct  
{
	UINT32 dataBuffLen;               	/* 缓存区长度，初始设置后不能改变 */
	UINT8* dataBuff;               /* 存放数据的指针地址 */
	UINT16 msgLen;               /* 读数据帧长度 */
	UINT8 msgType;               /* 消息帧类型*/
}MsgStru;

/*
 * 多帧数据合成一阵数据发送
 * 参数： 
 * 输入参数：   
            UINT8 msgCount  报文个数
			MsgStru msgList[]  报文数据列表
			UINT16 sndBuffLen  报文存放的数据缓存区大小
 * 输出参数：
 *          UINT8 snddataBuff[]  合并的数据帧
 * 返回值：
 *   >0：合并后的报文长度
 *   0: 数据长度为零
 *   -1: 没有分配空间
 *   -2: 分配空间不足
 *   -3:数组拷贝失败
 */
INT32 PackageMsg2SndBuff(UINT8 msgCount,MsgStru msgList[], UINT16 sndBuffLen,UINT8 snddataBuff[]);


/*
 * 功能：将收到一帧报文数据分解成多帧报文
 *
 * 参数： 
 * 输入参数：  
            UINT16 rcvDataLen     接受数据长度
            UINT8 rcvDataBuff[]   数据
            UINT8 msgListLen      MSG缓存区最大大小
			MsgStru msgList[]     MSG缓存区
 * 输出参数：  
            MsgStru msgList[]     报文数据列表
 * 返回值：
 *    >0 :报文数量
      0:处理失败或者没有数据
 */
INT16 UnPackageRcvBuff2MsgList (UINT16 rcvDataLen,UINT8 rcvDataBuff[],UINT8 msgListLen,MsgStru msgList[]);

#ifdef __cplusplus
}
#endif
#endif	/* #ifdef */