
/************************************************************************
* 文件名：DSURelatedDeal.c
* 文件概述：CBTC系统中DSU协议相关处理的函数
主要用于存储判断能够建链以及具体建链细节信息的函数
* 创建时间：2009.01.17  
* 文件描述：
1.根据应用提供的数据库中的信息
2.判断给出的两个设备链路关系是否合法，也即能否建链；
3.根据给出的本方和对方的对象名得到建链的具体信息；
4.ZC建链查询函数，需要输入一个设备，查找和他同类型的有通信关系且输入设备为发起方的链路。
5.提供Type+ID，即设备名的比较函数；
*          
************************************************************************/
#include "CommonMemory.h"
#include "dfsDataRead.h"
#include "dsuRelatedDeal.h"			/*本文件的.h文件*/
#include "dquDataTypeDefine.h"
#include "string.h"

#define DSU_OBJ_INFO_LENGTH 14	   /*DSU通信对象信息长度（单位：字节）*/
#define DSU_DIFF_RELATION_LENGTH 15	   /*DSU不同设备类型通信关系记录长度（单位：字节）*/
#define DSU_SAME_RELATION_LENGTH 17	   /*DSU相同设备类型通信关系记录长度（单位：字节）*/

/*结构体内部数据类型总和*/
#define DSU_IP_INFO_STRU_SIZE 22

#define DSU_OBJ_TYPE_SUBINDEX 0	   /*DSU通信对象类型子索引*/
#define DSU_OBJ_ID_SUBINDEX 1	   /*DSU通信对象ID子索引*/
#define DSU_OBJ_BLUEIP_SUBINDEX 2	   /*DSU通信对象蓝网IP子索引*/
#define DSU_OBJ_BLUEPORT_SUBINDEX 6	   /*DSU通信对象蓝网端口子索引*/
#define DSU_OBJ_REDIP_SUBINDEX 8	   /*DSU通信对象红网IP子索引*/
#define DSU_OBJ_REDPORT_SUBINDEX 12	   /*DSU通信对象红网端口子索引*/


#define DSU_DIFF_SPON_TYPE_SUBINDEX 0	   /*DSU不同类型设备通信关系表发起方类型子索引*/
#define DSU_DIFF_SPON_COMTYPE_SUBINDEX 1          /*DSU不同类型设备通信关系表发起方通信类型子索引*/
#define DSU_DIFF_SPON_CYCLE_SUBINDEX  2                                /*DSU不同类型设备通信关系表发起方周期子索引*/
#define DSU_DIFF_SPON_MSG_MAXDELAY_SUBINDEX 4              /*DSU不同类型设备通信关系表发起方最大报文延时*/
#define DSU_DIFF_SPON_ACK_MAXDELAY_SUBINDEX 6              /*DSU不同类型设备通信关系表发起方接收ACK最大延时*/
#define DSU_DIFF_FOLL_TYPE_SUBINDEX 8	   /*DSU不同类型设备通信关系表跟随方类型子索引*/
#define DSU_DIFF_FOLL_COMTYPE_SUBINDEX 9         /*DSU不同类型设备通信关系表跟随方通信类型子索引*/
#define DSU_DIFF_FOLL_CYCLE_SUBINDEX  10                                /*DSU不同类型设备通信关系表跟随方周期子索引*/
#define DSU_DIFF_FOLL_MSG_MAXDELAY_SUBINDEX 12              /*DSU不同类型设备通信关系表跟随方最大报文延时*/
#define DSU_DIFF_SENDMSG_MINDELAY_SUBINDEX 14              /*DSU此通信关系中成功发送一包数据所需最小周期数*/

#define DSU_SAME_SPON_TYPE_SUBINDEX 0	   /*DSU相同类型设备通信关系表发起方类型子索引*/
#define DSU_SAME_SPON_ID_SUBINDEX 1	   /*DSU相同类型设备通信关系表发起方ID子索引*/
#define DSU_SAME_SPON_COMTYPE_SUBINDEX 2          /*DSU相同类型设备通信关系表发起方通信类型子索引*/
#define DSU_SAME_SPON_CYCLE_SUBINDEX  3                                /*DSU相同类型设备通信关系表发起方周期子索引*/
#define DSU_SAME_SPON_MSG_MAXDELAY_SUBINDEX 5              /*DSU相同类型设备通信关系表发起方最大报文延时*/
#define DSU_SAME_SPON_ACK_MAXDELAY_SUBINDEX 7              /*DSU相同类型设备通信关系表发起方接收ACK最大延时*/
#define DSU_SAME_FOLL_TYPE_SUBINDEX 9	   /*DSU相同类型设备通信关系表跟随方类型子索引*/
#define DSU_SAME_FOLL_ID_SUBINDEX 10	               /*DSU相同类型设备通信关系表跟随方ID子索引*/
#define DSU_SAME_FOLL_COMTYPE_SUBINDEX 11         /*DSU相同类型设备通信关系表跟随方通信类型子索引*/
#define DSU_SAME_FOLL_CYCLE_SUBINDEX  12                    /*DSU相同类型设备通信关系表跟随方周期子索引*/
#define DSU_SAME_FOLL_MSG_MAXDELAY_SUBINDEX 14              /*DSU相同类型设备通信关系表跟随方最大报文延时*/
#define DSU_SAME_SENDMSG_MINDELAY_SUBINDEX 16              /*DSU此通信关系中成功发送一包数据所需最小周期数*/


#define DSU_DATA_PROTCL_DIFF_RELATION     0x04   /*DSU不同设备类型通信关系tableId*/
#define DSU_DATA_PROTCL_SAME_RELATION     0x05   /*DSU相同设备类型通信关系tableId*/
/***************************************************************************************************************
函数描述：将设备TYPE和设备ID转换成long型的设备名
参数说明：
[in]UINT8 devType 设备类型
[in]UINT8 devID  设备ID
返回值：unsigned INT32 型的设备名
功能描述：将设备TYPE和设备ID转换成long型的设备名
***************************************************************************************************************/
static UINT32 dsuTypeIdToName(UINT8 devType,UINT8 devID)
{
	UINT32 Templong;
	Templong = 0;
	Templong = devType;
	Templong = (Templong*0x100) + devID;
	Templong = Templong*0x10000  ;
	return Templong;
}

/*	
功能描述:判断设备能否建链，（对方设备在DSU通信对象中（需要去掉），且对方设备在相同设备关系表或不同设备关系表中 add by lcj)
参数说明:
[in]struc_Unify_Info * LocalInfo：本对象本地信息统一结构体
[in]UINT32 DestDevName：对方设备Name
[out]UINT8* role    本方角色
返回值:能够建链1,否则返回0
*/
UINT8 dsuIsLnkLegal(struc_Unify_Info * LocalInfo, UINT32 DestDevName,UINT8* role)
{
	register UINT32 lTemp;   /*临时long型变量*/
	register UINT8 * data;  /*数据库地址临时变量*/

	UINT32 i; 
	UINT8 ret = 0;

	/*UINT16 nObjNumber;*/   /*通信对象个数*/
	UINT16 nDiffRelation;   /*不同通信类型通信关系个数*/
	UINT16 nSameRelation;   /*相同通信类型通信关系个数*/
	UINT32 lSameRelationIndex;   /*相同通信类型通信关系表起始索引*/
	UINT32 lDiffRelationIndex;   /*不同通信类型通信关系表起始索引*/
	UINT32 lDevName;   /*通信对象名临时变量*/
	UINT32 lDevName2;   /*通信对象名临时变量*/
	UINT32 lLocalDevName;   /*本对象名临时变量*/
	UINT32 lDestDevType;   /*对方对象设备类型临时变量*/

	if(dsuDataCheck((UINT8 *) LocalInfo->DataBase) == 1)
	{
		data = (UINT8 *) LocalInfo->DataBase;

		lTemp=0;
#ifdef SFP_CKECK_DSU_IP 
		/*得到DSU中通信对象个数*/
		nObjNumber = ShortFromChar(&data[lTemp]);
		lTemp+=2;

		/*得到DSU中不同类型通信关系个数索引*/
		lTemp +=nObjNumber* DSU_OBJ_INFO_LENGTH;
#endif
		/*得到DSU中不同类型通信关系个数*/
		nDiffRelation = ShortFromChar(&data[lTemp]);
		lTemp+=2;
		lDiffRelationIndex = lTemp;

		/*得到DSU中相同类型通信关系个数索引*/
		lTemp =  lTemp+nDiffRelation * DSU_DIFF_RELATION_LENGTH;

		/*得到DSU中相同类型通信关系个数*/
		nSameRelation = ShortFromChar(&data[lTemp]);
		lTemp+=2;
		lSameRelationIndex = lTemp;
#ifdef SFP_CKECK_DSU_IP 
		/*判断本地设备合法性*/
		lLocalDevName = dsuTypeIdToName(LocalInfo->LocalType,LocalInfo->LocalID);
		lTemp=dsuIsDevExist(LocalInfo,lLocalDevName);
		if (lTemp == 0)
		{
			/*本地设备不合法*/
			ret = 0;
			return ret;
	    }


		lTemp=dsuIsDevExist(LocalInfo,DestDevName);/* 返回通信对象起始地址 */

		if(lTemp!=0)
	
		{
		#endif
			/*目的设备合法*/

			/*查相同类型通信对象关系表*/
			if(LocalInfo->LocalType  == ((DestDevName>>24) & 0xFF))
			{
				lTemp = lSameRelationIndex;
				lLocalDevName = ((UINT32) LocalInfo->LocalType << 24) + ((UINT32) LocalInfo->LocalID<<16);
				for(i = 0; i < nSameRelation; i++)
				{
					/*得发起方设备名*/
					lDevName =  ((UINT32) data[lTemp]<<24) + ((UINT32) data[lTemp + 1] << 16);
					/*得跟随方设备名*/
					lDevName2 =  ((UINT32) data[lTemp + DSU_SAME_FOLL_TYPE_SUBINDEX] << 24) + 
						((UINT32) data[lTemp + DSU_SAME_FOLL_ID_SUBINDEX] << 16);
					/*本方为发起方，对方为跟随方*/
					if((lLocalDevName  == lDevName )&& (DestDevName == lDevName2))
					{
						(*role) = DSU_ROLE_SPON;
						ret = 1;
						return ret;
					}
					/*本方为跟随方，对方为发起方*/
					else if((lLocalDevName  == lDevName2) && (DestDevName == lDevName))
					{
						(*role) = DSU_ROLE_FLW;
						ret = 1;
						return ret;
					}
					else
					{
						lTemp = lTemp + DSU_SAME_RELATION_LENGTH;
					}

				}
			}
			else  /*查不同类型通信对象关系表*/ 
			{
				lTemp = lDiffRelationIndex;
				lLocalDevName =  LocalInfo->LocalType;
				lDestDevType = (DestDevName>>24) & 0xff;
				for(i = 0; i < nDiffRelation; i++)
				{
					/*得发起方设备类型名，借用设备名变量*/
					lDevName =  data[lTemp];
					/*得跟随方设备类型名，借用设备名变量*/
					lDevName2 =  data[lTemp +   DSU_DIFF_FOLL_TYPE_SUBINDEX];

					if((lLocalDevName  == lDevName) && (lDestDevType == lDevName2))
					{
						(*role) = DSU_ROLE_SPON;
						ret = 1;
						return ret;
					}
					else if((lLocalDevName  == lDevName2) && (lDestDevType == lDevName))
					{
						(*role) = DSU_ROLE_FLW;
						ret = 1;
						return ret;
					}
					else
					{
						lTemp = lTemp + DSU_DIFF_RELATION_LENGTH;
					}
				}
			}
			ret = 0;
			return ret;
#ifdef SFP_CKECK_DSU_IP 
		}
		else
		{
			ret = 0;
			return ret;
		}
#endif
	}
	else
	{
		ret = 0;
		return ret;
	}
}


/*
* 功能描述:获取lnk信息
* 参数说明:
*  [in]UINT32 DestDevName         设备类型+id
*  [in]struc_Unify_Info * LocalInfo 本对象本地信息统一结构体
* 使用到的全局变量：
* 返回值:如果出现返回1，否则返回0
*/
UINT8 dsuGetLnkInfo(struc_Unify_Info* LocalInfo, UINT32 DestDevName, struc_LnkBasicInfo* lnk_basic_info )
{
	register UINT32 lTemp;   /*临时long型变量*/
	register UINT8 * data;  /*数据库地址临时变量*/

	UINT16 i;
	/*UINT16 nObjNumber;*/   /*通信对象个数*/
	UINT16 nDiffRelation;   /*不同通信类型通信关系个数*/
	UINT16 nSameRelation;   /*相同通信类型通信关系个数*/
	UINT32 lSameRelationIndex;   /*相同通信类型通信关系表起始索引*/
	UINT32 lDiffRelationIndex;   /*不同通信类型通信关系表起始索引*/
	UINT32 lDestDevType;   /*对方对象设备类型临时变量*/
	UINT32 lLocalDevName;   /*本对象名临时变量*/
	UINT32 lDevName;
	UINT32 lDevName2;
	UINT8 ret;
#ifdef SFP_CKECK_DSU_IP 
	/*判断本地设备合法性*/
	lLocalDevName = dsuTypeIdToName(LocalInfo->LocalType,LocalInfo->LocalID);

	lTemp=dsuIsDevExist(LocalInfo,lLocalDevName);
	if (lTemp == 0)
	{
		/*本地设备不合法*/
		ret = 0;
		return ret;
	}
#endif
	lLocalDevName = dsuTypeIdToName(LocalInfo->LocalType,LocalInfo->LocalID);
#ifdef SFP_CKECK_DSU_IP 
	/*判断设备是否在DSU数据表中存在，如果存在，则返回这个设备信息的第一个字节在DSU数组中的位置索引*/
	if(dsuIsDevExist(LocalInfo , DestDevName) != 0)
	{
#endif
		data = (UINT8 *) LocalInfo->DataBase;
		lTemp=0;
#ifdef SFP_CKECK_DSU_IP 
		/*得到DSU中通信对象个数*/
		nObjNumber = ShortFromChar(&data[lTemp]);
		lTemp+=2;

		/*得到DSU中不同类型通信关系个数索引*/
		lTemp = lTemp + nObjNumber* DSU_OBJ_INFO_LENGTH;
#endif
		/*得到DSU中不同类型通信关系个数*/
		nDiffRelation = ShortFromChar(&data[lTemp]);
		lTemp+=2;
		lDiffRelationIndex = lTemp;

		/*得到DSU中相同类型通信关系个数索引*/
		lTemp = lTemp + nDiffRelation * DSU_DIFF_RELATION_LENGTH;

		/*得到DSU中相同类型通信关系个数*/
		nSameRelation = ShortFromChar(&data[lTemp]);
		lTemp+=2;
		lSameRelationIndex = lTemp;

		if(LocalInfo->LocalType  == ((DestDevName>>24) & 0xFF))/* 相同设备类型 */
		{
			lTemp = lSameRelationIndex;
			lLocalDevName = ((UINT32) LocalInfo->LocalType << 24) + ((UINT32) LocalInfo->LocalID<<16);
			for(i = 0; i < nSameRelation; i++)
			{
				/*得发起方设备名*/
				lDevName =  ((UINT32) data[lTemp]<<24) + ((UINT32) data[lTemp + 1] << 16);
				/*得跟随方设备名*/
				lDevName2 =  ((UINT32) data[lTemp + DSU_SAME_FOLL_TYPE_SUBINDEX]<<24) + 
					((UINT32) data[lTemp + DSU_SAME_FOLL_ID_SUBINDEX] << 16);

				if((lLocalDevName  == lDevName )&& (DestDevName == lDevName2))
				{
					lnk_basic_info->Role = DSU_ROLE_SPON;	/*本机角色*/
					/*此通信关系中允许的最大合法丢包数*/
					lnk_basic_info->NCyclesPerPack = data[lTemp + DSU_SAME_SENDMSG_MINDELAY_SUBINDEX];
					/* 发起方发送数据通信类型（也是跟随方接收数据的通信类型）*/
					lnk_basic_info->LocalCommType = data[lTemp + DSU_SAME_SPON_COMTYPE_SUBINDEX];
					/*本方报文发送周期*/
					lnk_basic_info->LocalCycle = ShortFromChar(&data[lTemp + DSU_SAME_SPON_CYCLE_SUBINDEX]);
					/*本方报文最大延时*/
					lnk_basic_info->LocalMaxDelay = ShortFromChar(&data[lTemp + DSU_SAME_SPON_MSG_MAXDELAY_SUBINDEX]);
					/*发起方需要知道Ack最大延时*/
					lnk_basic_info->AckMaxDelay = ShortFromChar(&data[lTemp + DSU_SAME_SPON_ACK_MAXDELAY_SUBINDEX]);
					/*对方报文发送周期*/
					lnk_basic_info->DestCycle = ShortFromChar(&data[lTemp + DSU_SAME_FOLL_CYCLE_SUBINDEX]);
					/*对方报文最大延时*/
					lnk_basic_info->DestMaxDelay = ShortFromChar(&data[lTemp + DSU_SAME_FOLL_MSG_MAXDELAY_SUBINDEX]);
					/* 跟随方发送数据通信类型（也是发起方接收数据的通信类型）*/
					lnk_basic_info->DestCommType = data[lTemp + DSU_SAME_FOLL_COMTYPE_SUBINDEX];
#ifdef SFP_CKECK_DSU_IP 
					/*填写其他基本信息*/
					dsuFillLnkCommConfig(LocalInfo, DestDevName,lnk_basic_info);
#endif
					ret = 1;
					return ret;

				}
				else if((lLocalDevName  == lDevName2) && (DestDevName == lDevName))
				{
					lnk_basic_info->Role = DSU_ROLE_FLW;	/*本机角色*/
					/*此通信关系中允许的最大合法丢包数*/
					lnk_basic_info->NCyclesPerPack = data[lTemp + DSU_SAME_SENDMSG_MINDELAY_SUBINDEX];
					/* 发起方发送数据通信类型（也是跟随方接收数据的通信类型）*/
					lnk_basic_info->LocalCommType = data[lTemp + DSU_SAME_FOLL_COMTYPE_SUBINDEX];
					/*本方报文发送周期*/
					lnk_basic_info->LocalCycle = ShortFromChar(&data[lTemp + DSU_SAME_FOLL_CYCLE_SUBINDEX]);
					/*本方报文最大延时*/
					lnk_basic_info->LocalMaxDelay = ShortFromChar(&data[lTemp + DSU_SAME_FOLL_MSG_MAXDELAY_SUBINDEX]);
					/*对方报文发送周期*/
					lnk_basic_info->DestCycle = ShortFromChar(&data[lTemp + DSU_SAME_SPON_CYCLE_SUBINDEX]);
					/*对方报文最大延时*/
					lnk_basic_info->DestMaxDelay = ShortFromChar(&data[lTemp + DSU_SAME_SPON_MSG_MAXDELAY_SUBINDEX]);
					/* 跟随方发送数据通信类型（也是发起方接收数据的通信类型）*/
					lnk_basic_info->DestCommType = data[lTemp + DSU_SAME_SPON_COMTYPE_SUBINDEX];
					
					#ifdef SFP_CKECK_DSU_IP 
					/*填写其他基本信息*/
					dsuFillLnkCommConfig(LocalInfo, DestDevName,lnk_basic_info);
                    #endif
					ret = 1;
					return ret;
				}
				else
				{
					lTemp = lTemp + DSU_SAME_RELATION_LENGTH;
				} 
			}
		}
		else
		{
			lTemp = lDiffRelationIndex;
			lLocalDevName =  LocalInfo->LocalType;
			lDestDevType = ((DestDevName>>24) & 0xff);
			for(i = 0; i < nDiffRelation; i++)
			{
				/*得发起方设备类型名*/
				lDevName =  data[lTemp];
				/*得跟随方设备类型名*/
				lDevName2 =  data[lTemp +   DSU_DIFF_FOLL_TYPE_SUBINDEX];
				if((lLocalDevName  == lDevName) && (lDestDevType == lDevName2))
				{
					lnk_basic_info->Role = DSU_ROLE_SPON;	/*本机角色*/
					lnk_basic_info->NCyclesPerPack = data[lTemp + DSU_DIFF_SENDMSG_MINDELAY_SUBINDEX];/*此通信关系中允许的最大合法丢包数*/
					lnk_basic_info->LocalCommType = data[lTemp + DSU_DIFF_SPON_COMTYPE_SUBINDEX];/* 发起方发送数据通信类型（也是跟随方接收数据的通信类型）*/
					lnk_basic_info->LocalCycle = ShortFromChar(&data[lTemp + DSU_DIFF_SPON_CYCLE_SUBINDEX]);    /*本方报文发送周期*/
					lnk_basic_info->LocalMaxDelay = ShortFromChar(&data[lTemp + DSU_DIFF_SPON_MSG_MAXDELAY_SUBINDEX]);/*本方报文最大延时*/
					lnk_basic_info->AckMaxDelay = ShortFromChar(&data[lTemp + DSU_DIFF_SPON_ACK_MAXDELAY_SUBINDEX]);/*发起方需要知道Ack最大延时*/
					lnk_basic_info->DestCycle = ShortFromChar(&data[lTemp + DSU_DIFF_FOLL_CYCLE_SUBINDEX]);     /*对方报文发送周期*/
					lnk_basic_info->DestMaxDelay = ShortFromChar(&data[lTemp + DSU_DIFF_FOLL_MSG_MAXDELAY_SUBINDEX]);	/*对方报文最大延时*/
					lnk_basic_info->DestCommType = data[lTemp + DSU_DIFF_FOLL_COMTYPE_SUBINDEX];/* 跟随方发送数据通信类型（也是发起方接收数据的通信类型）*/
					#ifdef SFP_CKECK_DSU_IP 
					/*填写其他基本信息*/
					dsuFillLnkCommConfig(LocalInfo, DestDevName,lnk_basic_info) ;
					#endif
					ret = 1;
					return ret;
				}
				else if((lLocalDevName  == lDevName2) && (lDestDevType == lDevName))
				{
					lnk_basic_info->Role = DSU_ROLE_FLW;	/*本机角色*/
					lnk_basic_info->NCyclesPerPack = data[lTemp + DSU_DIFF_SENDMSG_MINDELAY_SUBINDEX];/*此通信关系中允许的最大合法丢包数*/
					lnk_basic_info->LocalCommType = data[lTemp + DSU_DIFF_FOLL_COMTYPE_SUBINDEX];/* 发起方发送数据通信类型（也是跟随方接收数据的通信类型）*/
					lnk_basic_info->LocalCycle = ShortFromChar(&data[lTemp + DSU_DIFF_FOLL_CYCLE_SUBINDEX]);    /*本方报文发送周期*/
					lnk_basic_info->LocalMaxDelay = ShortFromChar(&data[lTemp + DSU_DIFF_FOLL_MSG_MAXDELAY_SUBINDEX]);/*本方报文最大延时*/
					lnk_basic_info->DestCycle = ShortFromChar(&data[lTemp + DSU_DIFF_SPON_CYCLE_SUBINDEX]);     /*对方报文发送周期*/
					lnk_basic_info->DestMaxDelay = ShortFromChar(&data[lTemp + DSU_DIFF_SPON_MSG_MAXDELAY_SUBINDEX]);	/*对方报文最大延时*/
					lnk_basic_info->DestCommType = data[lTemp + DSU_DIFF_SPON_COMTYPE_SUBINDEX];/* 跟随方发送数据通信类型（也是发起方接收数据的通信类型）*/
					#ifdef SFP_CKECK_DSU_IP 
					/*填写其他基本信息*/
					dsuFillLnkCommConfig(LocalInfo, DestDevName,lnk_basic_info);
					#endif
					ret = 1;
					return ret;
				}
				else
				{
					lTemp = lTemp + DSU_DIFF_RELATION_LENGTH;
				}
			}
		}
		ret = 0;
		return ret;   /*查半天，所有的表都没查到合适表项*/
		#ifdef SFP_CKECK_DSU_IP 
	}
	else
	{
		ret = 0;
		return ret;   /*数据库数组有错*/
	}
    #endif
}
#ifdef SFP_CKECK_DSU_IP 
/*
* 功能描述:将DSU的某一条记录的所有信息填充到链路基本信息结构体元素中
* 参数说明:
*   [in]UINT32	DevName	设备name
*   [out]struc_LnkBasicInfo* lnk_Comm_info            描述链路所需要的基本信息
*   [in]struc_Unify_Info * LocalInfo 本对象本地信息统一结构体
* 返回值:成功返回1否则返回0
*/
UINT8 dsuFillLnkCommConfig(struc_Unify_Info * LocalInfo, UINT32 DevName,struc_LnkBasicInfo* lnk_Comm_info)
{
	/*****************2012.4.1根据微联项目修改***********************************/
	register UINT32 lTemp;   /*临时long型变量*/
	register UINT8 * data;  /*数据库地址临时变量*/

	UINT8 ret;
	UINT32 i;
	UINT16 nObjNumber;   /*通信对象个数*/
	UINT32 lDevName;   /*通信对象名临时变量*/

	/*默认置找到ip信息为0个*/
	lnk_Comm_info->dstNum=0;

	if(dsuDataCheck((UINT8 *) LocalInfo->DataBase) == 1)
	{
		data = (UINT8 *) LocalInfo->DataBase;
		lTemp=0;

		/*得到DSU中通信对象个数*/
		nObjNumber = ShortFromChar(&data[lTemp]);
		lTemp+=2;

		for(i = 0; i < nObjNumber; i++)
		{
			lDevName =  ((UINT32) data[lTemp]<<24) + ((UINT32) data[lTemp + 1] << 16);
			if(lDevName == DevName)
			{
				/*本行设备name等于待查找设备name*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].RedIp[0] = data[lTemp + DSU_OBJ_REDIP_SUBINDEX];/*红网IP地址*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].RedIp[1] = data[lTemp + DSU_OBJ_REDIP_SUBINDEX + 1];/*红网IP地址*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].RedIp[2] = data[lTemp + DSU_OBJ_REDIP_SUBINDEX + 2];/*红网IP地址*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].RedIp[3] = data[lTemp + DSU_OBJ_REDIP_SUBINDEX + 3];/*红网IP地址*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].PortRed = ShortFromChar(&data[lTemp + DSU_OBJ_REDPORT_SUBINDEX]);/*红网端口*/


				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].BlueIp[0] = data[lTemp + DSU_OBJ_BLUEIP_SUBINDEX];/*蓝网IP地址*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].BlueIp[1] = data[lTemp + DSU_OBJ_BLUEIP_SUBINDEX + 1];/*蓝网IP地址*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].BlueIp[2] = data[lTemp + DSU_OBJ_BLUEIP_SUBINDEX + 2];/*蓝网IP地址*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].BlueIp[3] = data[lTemp + DSU_OBJ_BLUEIP_SUBINDEX + 3];/*蓝网IP地址*/
				lnk_Comm_info->DstIpInfo[lnk_Comm_info->dstNum].PortBlue = ShortFromChar(&data[lTemp + DSU_OBJ_BLUEPORT_SUBINDEX]);/*蓝网端口*/

				lnk_Comm_info->dstNum++;
				
			}
			else
			{
				/*本行设备name不等于待查找设备name*/


			}

			lTemp = lTemp + DSU_OBJ_INFO_LENGTH;
		}


	}
	else
	{
		/*数据不合法，直接返回失败*/

	}

	/*判断是否找到ip地址返回*/
	if (lnk_Comm_info->dstNum==0)
	{
		ret = 0;

	}
	else
	{
		ret = 1;
	}

	return ret;

}
#endif
/*
*DSU数据校验：检验DSU数据的正确性
* 入口：存储DSU的数组
* 出口：获得正确返回1错误返回0
*/
UINT8 dsuDataCheck(const UINT8* data)
{
	UINT8 ret=1;
	UINT8 tempData;

	/*读一下数据，可以关闭警告。*/
	tempData=data[0];
	return ret;
}
#ifdef SFP_CKECK_DSU_IP 
/*
* 功能描述:判断设备是否是数据库中的对象（判断设备是否在DSU中通信对象中）
* 参数说明:
*   [in]UINT32 DevName         设备类型+id
*   [in]struc_Unify_Info * LocalInfo 本对象本地信息统一结构体
* 使用到的全局变量：
* 返回值:如果出现返回它在DSU数据库中的内存位置偏移索引，否则返回0	
*/
UINT32 dsuIsDevExist(struc_Unify_Info * LocalInfo , UINT32 DevName)
{
	register UINT32 lTemp;   /*临时long型变量*/
	register UINT8 * data;  /*数据库地址临时变量*/

	UINT32 ret;
	UINT32 i;
	UINT16 nObjNumber;   /*通信对象个数*/
	UINT32 lDevName;   /*通信对象名临时变量*/

	if(dsuDataCheck((UINT8 *) LocalInfo->DataBase) == 1)
	{
		data = (UINT8 *) LocalInfo->DataBase;
		lTemp=0;

		/*得到DSU中通信对象个数*/
		nObjNumber = ShortFromChar(&data[lTemp]);
		lTemp+=2;

		for(i = 0; i < nObjNumber; i++)
		{
			lDevName =  ((UINT32) data[lTemp]<<24) + ((UINT32) data[lTemp + 1] << 16);
			if(lDevName != DevName)
			{
				lTemp = lTemp + DSU_OBJ_INFO_LENGTH;
			}
			else
			{
				return lTemp;
			}
		}
		ret = 0;
		return ret;

	}
	else
	{
		ret = 0;
		return ret;
	}
}
#endif
/*
* 功能描述:获取DSU中通信对象个数
* 参数说明:
*   [in] struc_Unify_Info * LocalInfo 当前运行的对象统一结构体内容
* 返回值:  >0DSU中通信对象个数,
           0发生错误。
*/
UINT16 dsuGetObjNum(struc_Unify_Info * LocalInfo)
{
#ifdef SFP_CKECK_DSU_IP 
	UINT8 *data;  /*数据库地址临时变量*/
	UINT16 nObjNumber;     /*DSU中通信对象个数*/

	data = (UINT8 *) LocalInfo->DataBase;
	/*得到DSU中通信对象个数*/

	if(dsuDataCheck(data) == 1)
	{
		nObjNumber = ShortFromChar(&data[0]);
	}
	else
	{
		nObjNumber=0;
	}
#else
	UINT16 nObjNumber;     /*DSU中通信对象个数*/
    nObjNumber = LocalInfo->MaxNumLink;/* 需要设置一个最大值 */
#endif
	return nObjNumber;
	
}

/*
* 功能描述:根据传入参数数据的文件名以或者数据的地址，读取协议的配置数据到内存中。将开辟的内存地址通过pDataBase返回。
* 参数说明:
UINT8* fileName  文件名或地址
UINT8** pDataBase  数据库地址
返回值:获得正确返回1否则返回0	
*/
UINT8 dsuSfpInit(UINT8* fileName,UINT8** pDataBase)
{
	UINT8 chReturnValue =0 ; /*返回值*/
	UINT8 TempRet;
	UINT8* pData=NULL; /*返回的数据*/
	UINT32 dataOff=0;  /*数据偏移量*/
	UINT32 dataOffIp=0;  /*ip表数据偏移量*/
	UINT32 dataLineNum=0;  /*ip表数据个数*/

#ifdef SFP_CKECK_DSU_IP 
	UINT8* pData1=NULL;  /*DSU通信对象数据*/
	UINT32 data1Len=0;  /*DSU通信对象数据长度*/
#endif
	UINT8* pData2=NULL;  /*DSU不同设备类型通信关系数据*/ 
	UINT32 data2Len=0;  /*DSU不同设备类型通信关系数据长度 */
	UINT8* pData3=NULL;  /*DSU相同设备类型通信关系数据*/
	UINT32 data3Len=0;  /*DSU相同设备类型通信关系数据长度*/

	/*UINT32 i;*/
#ifdef SFP_CKECK_DSU_IP 
	/*读取数据*/
	TempRet=dquGetConfigData((CHAR *)fileName, DSU_DEVICE_IP_CONFIG_TYPE, DSU_DATA_IP_IP_ID, &pData1 , &data1Len);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*继续执行*/
	}
#endif
	/*读取数据*/
	TempRet=dquGetConfigData((CHAR *)fileName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_DIFF_RELATION, &pData2 , &data2Len);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*继续执行*/
	}

	/*读取数据*/
	TempRet=dquGetConfigData((CHAR *)fileName, DSU_DATA_PROTCL_TYPE, DSU_DATA_PROTCL_SAME_RELATION, &pData3 , &data3Len);
	if (TempRet!=1)
	{
		/*数据读取失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
		/*继续执行*/
	}


	/*动态申请数据空间，后面加100是考虑了足够的余量*/
#ifdef SFP_CKECK_DSU_IP 
	pData=(UINT8*)malloc(data1Len+data2Len+data3Len+100);
#else
    pData=(UINT8*)malloc(data2Len+data3Len+100);
#endif
	if (pData==NULL)
	{
		/*申请空间失败*/
		chReturnValue=0;
		return chReturnValue;
	}
	else
	{
#ifdef SFP_CKECK_DSU_IP 
		/*程序继续执行*/
		CommonMemSet(pData,(data1Len+data2Len+data3Len+100),0,(data1Len+data2Len+data3Len+100));
#else
        CommonMemSet(pData,(data2Len+data3Len+100),0,(data2Len+data3Len+100));
#endif
	}

	/*拷贝生成数据*/
	dataOff=0;
#ifdef SFP_CKECK_DSU_IP 
	/*ip表数据拷贝*/

	/*计算ip数据的行数*/
	dataLineNum=data1Len/DSU_IP_INFO_STRU_SIZE;

	if (dataLineNum>65535)
	{
		/*判断数据越界*/
		chReturnValue=0;
		return chReturnValue;
	}

	ShortToChar((UINT16)dataLineNum,pData+dataOff);
	dataOff+=2;

	/*初始化ip偏移量变量*/
	dataOffIp=0;
	/*循环复制数据*/
	for (i=0;i<dataLineNum;i++)
	{
		/*复制数据*/
		memcpy(pData+dataOff,pData1+dataOffIp,2);/* 类型+ID*/
		dataOff+=2;

		memcpy(pData+dataOff,pData1+dataOffIp+2,4);/* 蓝网IP*/
		dataOff+=4;

		memcpy(pData+dataOff,pData1+dataOffIp+10,2);/* SFP蓝网端口*/
		dataOff+=2;

		memcpy(pData+dataOff,pData1+dataOffIp+6,4);/* 红网IP*/
		dataOff+=4;

		memcpy(pData+dataOff,pData1+dataOffIp+12,4);/* SFP红网端口*/
		dataOff+=2;

		/*偏移量变量后移*/
		dataOffIp+=DSU_IP_INFO_STRU_SIZE;
	}

#endif

	/*计算数据的行数*/
	dataLineNum=data2Len/DSU_DIFF_RELATION_LENGTH;

	ShortToChar((UINT16)dataLineNum,pData+dataOff);
	dataOff+=2;

	/*复制数据*/
	memcpy(pData+dataOff,pData2,data2Len);
	dataOff+=data2Len;

	/*计算数据的行数*/
	dataLineNum=data3Len/DSU_SAME_RELATION_LENGTH;

	ShortToChar((UINT16)dataLineNum,pData+dataOff);
	dataOff+=2;

	/*复制数据*/
	memcpy(pData+dataOff,pData3,data3Len);
	dataOff+=data3Len;

	*pDataBase=pData;

	chReturnValue=1;
	return chReturnValue;
}

