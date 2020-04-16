/********************************************************
*                                                                                                            
* 文 件 名： SpksConfigDataManage.c   
* 版权说明： 北京交控科技有限公司 
* 创建时间： 2014-12-05
* 作    者： 联锁组
* 功能描述： SPKS开关相关配置  
* 函数包括： 
* 使用注意：
* 修改记录：   
*
********************************************************/ 
#include "SpksConfigDataManage.h"
#include "../System/CiSystemDefine.h"

SpksDataConfigStruct spksDataConfigBuff[SPKS_SUM_MAX];		/* 配置数据 */
UINT16 spksDataConfigIdxBuff[SPKS_SUM_MAX] = {0};					/* 索引表 */
UINT16 spksSum = 0;
UINT16 totalSpksNum = 0;/*本联锁和相邻联锁发送给本联锁Spks总数*/


	/*
	* 功能描述： 获取指定ID的SPKS的索引号
	* 参数说明： const UINT16 spksId, spksId
	* 返回值  ： SPKS_SUM_MAX: 获取数据失败
	*			<SPKS_SUM_MAX: 查找成功
	*/
	UINT16 GetSpksConfigBufIndex(const UINT16 spksId)
	{
		UINT16 index = SPKS_SUM_MAX;
		
		if((spksId >= 1) && (spksId < SPKS_SUM_MAX))
		{
			/* 由于目前索引映射方式直接是设备ID，因此该处直接使用设备ID */
			index = spksId;
		}

		return index;
	}
	

	/*
	* 功能描述：  获取指定ID的SPKS对应物理区段个数
	* 参数说明：  SPKS对应的索引
	* 返回值  ： 
	* 返回值  ： 0: 获取数据失败
	*			>0: 查找成功
	*/
	UINT16 GetSpksPhySecSumByIndex(UINT16 spksIndex)
	{
		UINT16 phySecCount = 0;
		
		if((spksIndex >= 1) && (spksIndex < SPKS_SUM_MAX))
		{
			phySecCount = spksDataConfigBuff[spksIndex].phySecNum;
		}
		else
		{
			phySecCount = 0;
		}
		return phySecCount;
	}

	/*
	* 功能描述：  获取指定ID的SPKS的制定索引的物理区段ID
	* 参数说明：spksIndex:spks索引，secIndex：物理区段序号  
	* 返回值  ： 
	*		 0: 返回
	*		>0: 对应物理区段ID
	*/
	UINT16 GetSpksPhySecId(UINT16 spksIndex,UINT16 secIndex)
	{
		UINT16 phySecId = 0;

		if((spksIndex >= 1) && (spksIndex < SPKS_SUM_MAX) && (secIndex < SPKS_MAX_SEC_NUM))
		{
			phySecId = spksDataConfigBuff[spksIndex].spksPhySecIdBuff[secIndex];
		}
		else
		{
			phySecId = 0;
		}

		return phySecId;
	}

	/*
	* 功能描述： 获取SPKS当前总数
	* 参数说明： void    
	* 返回值  ： 0: 获取数据失败
	*			>0: SPKS当前总数
	*/
	UINT16 GetSpksSum(void)
	{
		UINT16 sum = 0;
		if (spksSum < SPKS_SUM_MAX)
		{
			sum = spksSum ;
		}
		else
		{
			sum = 0;
		}

		return sum;
	}

	/* 功能描述： 获取指定索引号的SPKS的ID
	* 参数说明： const UINT16 spksIndex, spks索引
	* 返回值  ： 0: 获取数据失败
	*			>0: 查找成功
	*/
	UINT16 GetSpksIdByIndex(const UINT16 spksIndex)
	{
		UINT16 ret = 0;
		if (spksIndex < GetTotalSpksCurNum() )
		{
			ret = spksDataConfigIdxBuff[spksIndex];
		}

		return ret;
	}

	/*
	* 功能描述：	整理读入的SPKS配置数据
	* 参数说明：	无
	* 返回值  ：	CI_SUCCESS,整理成功
	*				CI_ERROR,整理失败
	*/
	UINT8 SpksConfigDataManage(void)
	{
		UINT16 ii = 0;      /*游标*/
		UINT16 jj = 0;      /*游标*/
		UINT16 spksDataSum = 0;      /*信号机设备配置数据总数*/
		UINT8 ret = CI_ERROR;
		UINT8 localCiId = GetSystemParaLocalOcId();

		/*保存信号机ID索引数组*/
		spksDataSum = GetSpksSum();
		totalSpksNum = spksDataSum;
		if (spksDataSum < SPKS_SUM_MAX)
		{/*配置数据总数错误*/

			for (ii = 0; ii < SPKS_SUM_MAX; ii++)
			{
				if (jj == spksDataSum)
				{/*遍历有效数据完毕*/

					break;
				}
				if(localCiId == spksDataConfigBuff[ii].belongCiId)				
				{/*该部分为有效数据*/

					spksDataConfigIdxBuff[jj] = spksDataConfigBuff[ii].spksId;
					jj++; 
				}
			}
			ret = CI_SUCCESS; 
		}
				
		return ret;

	}

	/*
	* 功能描述： 获取指定ID对应的采集继电器ID
	* 参数说明： UINT16 spksId    
	* 返回值  ： 0: 获取数据失败
	*			>0: 继电器ID
	*/
	UINT16 GetSpksColRelayId(UINT16 spksId)
	{
		UINT16 bufIndex = 0;  /*数组下标*/
		UINT16 ret = 0;

		bufIndex = GetSpksConfigBufIndex(spksId);  /*获取区段数组下标*/

		if (bufIndex < SPKS_SUM_MAX)
		{
			ret =  spksDataConfigBuff[bufIndex].colRelayId;
		}
		
		return ret;
	}

	/*
	* 功能描述： 获取指定ID对应的旁路继电器ID
	* 参数说明： UINT16 spksId    
	* 返回值  ： 0: 获取数据失败
	*			>0: 继电器ID
	*/
	UINT16 GetSpksCutRelayId(UINT16 spksId)
	{
		UINT16 bufIndex = 0;  /*数组下标*/
		UINT16 ret = 0;

		bufIndex = GetSpksConfigBufIndex(spksId);  /*获取区段数组下标*/

		if (bufIndex < SPKS_SUM_MAX)
		{
			ret =  spksDataConfigBuff[bufIndex].cutRelayId;
		}

		return ret;
	}

	/*
	* 功能描述： 获取指定ID对应的表示继电器ID
	* 参数说明： void    
	* 返回值  ： 0: 获取数据失败
	*			>0: 继电器ID
	*/
	UINT16 GetSpksDisRelayId(UINT16 spksId)
	{
		UINT16 bufIndex = 0;  /*数组下标*/
		UINT16 ret = 0;

		bufIndex = GetSpksConfigBufIndex(spksId);  /*获取区段数组下标*/

		if (bufIndex < SPKS_SUM_MAX)
		{
			ret =  spksDataConfigBuff[bufIndex].displayRelayId;
		}

		return ret;
	}

	/*
	* 功能描述： 获取指定IDSpks所属联锁ID
	* 参数说明： const UINT16 spksId, spksId    
	* 返回值  ： 0: 获取数据失败
	*			>0: 所属联锁ID      
	*/
	UINT8 GetSpksBelongCiId(const UINT16 spksId)
	{
		UINT16 bufIndex = 0;  /*数组下标*/
		UINT8 ciId = 0;


		bufIndex = GetSpksConfigBufIndex(spksId);  /*获取区段数组下标*/

		if (bufIndex < SPKS_SUM_MAX)
		{
			ciId = spksDataConfigBuff[bufIndex].belongCiId;
		}
		else
		{
			ciId = 0;
		}

		return ciId;
	}

	/*
	* 功能描述：	整理读入的相邻联锁Spks配置数据
	* 参数说明：	spksSum SPKS数量
	*				spksIdBuff 相邻联锁发送给本联锁的SpksID配置数据
	* 返回值  ：	1,整理成功
	*				0,整理失败
	*/
	UINT8 AddExtendSpksIdDataManage(UINT16 spksNum,const UINT16 *spksIdBuff)
	{
		UINT16  index = 0;  /*数组下标*/
		UINT16  ii = 0;
		UINT8 retVal = 1;

		if ((totalSpksNum + spksNum) >= SPKS_SUM_MAX)
		{/*入口防护*/
			retVal = 0;
		}
		else
		{
			index = totalSpksNum;
			totalSpksNum += spksNum;
			for(ii = 0;ii< spksNum;ii++)
			{
				spksDataConfigIdxBuff[index+ii] = spksIdBuff[ii];/*追加到spksDataConfigIdxBuff*/
			}
		}

		return retVal;
	}

	/*
	* 功能描述： 获取相邻联锁发送给本联锁以及本联锁Spks当前总数
	* 参数说明： void    
	* 返回值  ： 0: 获取数据失败
	*			>0: Spks当前总数
	*/
	UINT16 GetTotalSpksCurNum(void)
	{
		UINT16 retVal  = 0;
		if (totalSpksNum >= SPKS_SUM_MAX)
		{
			retVal = 0;
		}
		else
		{
			retVal = totalSpksNum;
		}
		return retVal;
	}

	/*
	* 功能描述： 获取指定ID SPKS的虚拟属性
	* 参数说明： const UINT16 spksId, SPKS ID    
	* 返回值  ： 0: 获取数据失败
	*			>0: SPKS的虚拟属性      
	*/
	UINT16 GetSpksVirtualAttribute(const UINT16 spksId)
	{
		UINT16 bufIndex = 0;  /*数组下标*/
		UINT16 retVal = CI_ERROR;

		bufIndex = GetSpksConfigBufIndex(spksId);  /*获取SPKS数组下标*/

		if (bufIndex < SPKS_SUM_MAX)
		{
			retVal = spksDataConfigBuff[bufIndex].VirtualAttribute;
		}
		else
		{
			retVal = CI_ERROR;
		}
		return retVal;
	}