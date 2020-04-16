#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include "readxmltodata.h"
#include ".\\tinyxml\\tinyxml.h"/*��ȡXML�ĵ�������*/
#include "Convert.h"
#include "dsuRssp2Func.h"


#define   PATH_IPCONFIG                "ini\\ZC.xml"                /*��ŵ���IP���ñ��·��*/

static CiAtsCfg ciAtsCfgInfo;
DeviceIpInfoStru gDeviceIpInfoArray[200];
DeviceIpInfoStru gDeviceIpInfoArrayLeu[100];

UINT16 gDeviceNum = 0;//�豸������Ŀ
UINT16 gDeviceNumLeu = 0;//LEUCI�����ļ����豸������Ŀ


UINT16 StrToIntData(char chTempData[20],UINT16 wDataLen)
{
	UINT16  wTempID = DEFAULT_ZERO;
	UINT16  wCycle = DEFAULT_ZERO;
	UINT8  wOneData = DEFAULT_ZERO;
	UINT16  wDataMultiple = 1;

	for (wCycle = wDataLen - 1;wCycle >= (UINT8)DEFAULT_ZERO;wCycle--)
	{
		switch (chTempData[wCycle])
		{
		case '0':
			wOneData = 0;
			break;
		case '1':
			wOneData = 1;
			break;
		case '2':
			wOneData = 2;
			break;
		case '3':
			wOneData = 3;
			break;
		case '4':
			wOneData = 4;
			break;
		case '5':
			wOneData = 5;
			break;
		case '6':
			wOneData = 6;
			break;
		case '7':
			wOneData =7;
			break;
		case '8':
			wOneData = 8;
			break;
		case '9':
			wOneData = 9;
			break;
		case 'a':
			wOneData = 10;
			break;
		case 'b':
			wOneData = 11;
			break;
		case 'c':
			wOneData = 12;
			break;
		case 'd':
			wOneData = 13;
			break;
		case 'e':
			wOneData = 14;
			break;
		case 'f':
			wOneData = 15;
			break;
		}

		if (wCycle == (wDataLen - 1))
		{
			wDataMultiple = 1;
		} 
		else
		{
			wDataMultiple = wDataMultiple * 16;
		}

		wTempID = wOneData * wDataMultiple + wTempID;

		if (wCycle == 0)
		{
			break;
		} 
		else
		{
		}
	}

	return wTempID;
}

void SetDataOfConfig(char chTempData[20],char chTempName[200],UINT16 wDataLen)
{
	static int wConfigCycle = DEFAULT_ZERO;
	char chChangeData[20];
	int wTempID = DEFAULT_ZERO;

	memset(chChangeData,DEFAULT_ZERO,sizeof(char) * 20);

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_DeviceIdAndTpye>"))
	{
		if (('0' == chTempData[0]) && ('x' == chTempData[1]))
		{
			memset(chChangeData,DEFAULT_ZERO,sizeof(char) * 20);
			memcpy(chChangeData,&chTempData[2],4);	

			gDeviceIpInfoArray[wConfigCycle].deviceName = StrToIntData(chChangeData,4);
		}
		else
		{
			printf("Id and Tpye error!\n");
		}
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_LogicId>"))
	{
		gDeviceIpInfoArray[wConfigCycle].logicId = atoi(chTempData);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_Red_IP>"))
	{
		sscanf(chTempData,"%d.%d.%d.%d",&gDeviceIpInfoArray[wConfigCycle].redIp[0],&gDeviceIpInfoArray[wConfigCycle].redIp[1],&gDeviceIpInfoArray[wConfigCycle].redIp[2],&gDeviceIpInfoArray[wConfigCycle].redIp[3]);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_Blue_IP>"))
	{
		sscanf(chTempData,"%d.%d.%d.%d",&gDeviceIpInfoArray[wConfigCycle].blueIp[0],&gDeviceIpInfoArray[wConfigCycle].blueIp[1],&gDeviceIpInfoArray[wConfigCycle].blueIp[2],&gDeviceIpInfoArray[wConfigCycle].blueIp[3]);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_SFP_BluePort>"))
	{
		gDeviceIpInfoArray[wConfigCycle].bluePort[SFP_IDX] = (UINT16)atoi(chTempData);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_SFP_RedPort>"))
	{
		gDeviceIpInfoArray[wConfigCycle].redPort[SFP_IDX] = atoi(chTempData);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_RP_BluePort>"))
	{
		gDeviceIpInfoArray[wConfigCycle].bluePort[RP_IDX] = atoi(chTempData);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_RP_RedPort>"))
	{
		gDeviceIpInfoArray[wConfigCycle].redPort[RP_IDX] = atoi(chTempData);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_RSSP_BluePort>"))
	{
		gDeviceIpInfoArray[wConfigCycle].bluePort[RSSP1_IDX] = atoi(chTempData);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_RSSP_RedPort>"))
	{
		gDeviceIpInfoArray[wConfigCycle].redPort[RSSP1_IDX] = atoi(chTempData);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_RSSP2_BluePort>"))
	{
		gDeviceIpInfoArray[wConfigCycle].bluePort[RSSP2_IDX] = atoi(chTempData);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<str_RSSP2_RedPort>"))
	{
		gDeviceIpInfoArray[wConfigCycle].redPort[RSSP2_IDX] = atoi(chTempData);

		wConfigCycle++;
		gDeviceNum = wConfigCycle;
	} 
	else
	{
	}
}

void GetOneData(char chDataBuff[200],const UINT16 strLen)
{
	int wCycle = DEFAULT_ZERO;

	UINT8  chStartFlag = DEFAULT_ZERO;
	UINT8  chEndFlag = DEFAULT_ZERO;
	char  chTempData[20];
	char  chTempName[200];
	UINT8  chDataIndex = DEFAULT_ZERO;
	UINT8  chNameIndex = DEFAULT_ZERO;
	UINT8  chNameFlag = DEFAULT_ZERO;

	memset(chTempData,DEFAULT_ZERO,sizeof(char) * 20);
	memset(chTempName,DEFAULT_ZERO,sizeof(char) * 200);

	for (wCycle = (UINT16)DEFAULT_ZERO;wCycle < strLen;wCycle++)
	{
		if ((chDataBuff[wCycle] == '<') && ((UINT8)FLAG_SET != chStartFlag))
		{
			chNameFlag = (UINT8)FLAG_SET;
		} 
		else
		{
			if ((UINT8)FLAG_SET == chStartFlag)
			{
				chNameFlag = (UINT8)FLAG_UNSET;
			} 
			else
			{
			}
		}

		if (FLAG_SET == chNameFlag)
		{
			chTempName[chNameIndex] = chDataBuff[wCycle];
			chNameIndex++;
		} 
		else
		{
		}

		if (chDataBuff[wCycle] == '>')
		{
			if (wCycle == strLen - 1)
			{
				break;
			} 
			else
			{
				chStartFlag = FLAG_SET;
				continue;
			}
		} 
		else
		{
		}

		if ((UINT8)FLAG_SET == chStartFlag)
		{
			if ((chDataBuff[wCycle] == '<') && (chDataBuff[wCycle + 1] == '/'))
			{
				break;
			} 
			else
			{
				chTempData[chDataIndex] = chDataBuff[wCycle];
				chDataIndex++;
			}
		} 
		else
		{
		}
	}

	if ((chDataIndex > 0) && (chNameIndex > 0))
	{
		SetDataOfConfig(chTempData,chTempName,chDataIndex);
	} 
	else
	{
	}
}

void ReadXml(const char * pPath)
{
	FILE *pFile = NULL;
	char ch;
	char chDataBuff[200];
	UINT16 wIndex = DEFAULT_ZERO;

	memset(gDeviceIpInfoArray, DEFAULT_ZERO, sizeof(gDeviceIpInfoArray));
	memset(chDataBuff, DEFAULT_ZERO, sizeof(char) * 200);

	if(gDeviceNum == 0 )
	{
		if (NULL == (pFile = fopen(pPath,"rt")))
		{
			printf("Open File error!\n");

			return;
		} 
		else
		{
		}

		while (EOF != (ch=fgetc(pFile)))
		{
			if (ch != '\n')
			{
				if (ch != ' ')
				{
					chDataBuff[wIndex] = ch;
					wIndex++;
				} 
				else
				{
				}
			} 
			else
			{
				GetOneData(chDataBuff,wIndex);
				memset(chDataBuff,0,sizeof(char) * 200);
				wIndex = 0;
			}
		}


		fclose(pFile);

	}
}

/*********************************************
*�������ܣ������豸ID������+ID�����ָ��Э���ŵ�����EndPoint��Ϣ
*����˵������
*��ڲ�����
		devId���豸ID
		prtlIdx��Э����
*���ڲ�����
		pIpInfos���ҵ�������EndPoint��Ϣ
		ipCnt������EndPoint����
*����ֵ��ʧ�ܻ�ɹ�
*********************************************/
UINT8 GetNetEndPoint(UINT16 devId, UINT16 logicId, UINT8 prtlIdx, IpInfo pIpInfos[], UINT8 *ipCnt)
{
	UINT16 idx = 0;
	UINT8 retVal = ERR_DEF;
	UINT8 tempIpCnt = 0;
	*ipCnt=0;

	if(prtlIdx<PRTL_CNT)
	{
		for(idx = 0; idx < gDeviceNum; idx++)
		{
			if(devId == gDeviceIpInfoArray[idx].deviceName && logicId == gDeviceIpInfoArray[idx].logicId)
			{
				pIpInfos[tempIpCnt].blueIp = LongFromChar(gDeviceIpInfoArray[idx].blueIp);
				pIpInfos[tempIpCnt].redIp = LongFromChar(gDeviceIpInfoArray[idx].redIp);
				pIpInfos[tempIpCnt].bluePort = gDeviceIpInfoArray[idx].bluePort[prtlIdx];
				pIpInfos[tempIpCnt].redPort = gDeviceIpInfoArray[idx].redPort[prtlIdx];

				tempIpCnt++;
			}
		}
	}
	if(tempIpCnt>0)
	{
		retVal = SUC_DEF;
	}

	*ipCnt=tempIpCnt;

	return retVal;
}



/* �����豸���ƺ�Э�����Ͳ���IP�Ͷ˿ڣ�
*���ҳɹ�����0������ʧ�ܷ��� ����ֵ 
*һ���豸�ж��IP��numΪ���ҵ���IP����
*/
UINT8 GetIpAndPortByDeviceName(UINT16 deviceName,UINT8 protocolType,IpInfo pIpInfo[],UINT8 *num)
{
	UINT16 index = 0;/*���ҷ�Χ��Ŀ����*/
	UINT8 idxPrtcl = 0;/*Э���Ӧ�Ķ˿�*/
	UINT8 retVal = 1;/*����ֵ�����ҳɹ������㣬��ʼ��Ϊ1*/
	UINT8 ipIndex = 0;/*���ҵ���IP��Ŀ*/

	UINT8 flag = 0;/*���Ҫ���ҵ�Э�������Ƿ����*/
	switch (protocolType)
	{
	case 1:		/* sfp Э�� */
		idxPrtcl = 0;
		flag = 1;
		break;
	case 2:	/* rp */
		idxPrtcl = 1;
		flag = 1;
		break;
	case 3: /* rsr */
		idxPrtcl = 2;
		flag = 1;
		break;
	case 4: /* rssp */
		idxPrtcl = 2;
		flag = 1;
		break;
	case 5: /* rssp */
		idxPrtcl = 3;
		flag = 1;
		break;
	default:
		flag = 0;/*û��Ҫ���ҵ�Э������*/
		break; /* Ĭ�� sfp */
	}
	
	if (0 == gDeviceNum)
	{
		/*printf("��ȡXML�����ļ�ʧ�ܣ�\n");*/
		retVal = 2;/*��ȡXML�����ļ�ʧ��*/
	}
	for (index = 0; index < gDeviceNum; index++)
	{
		/* �������豸���ƺ�Э���Ƿ���� */		
		if ((flag == 1)&&(deviceName == gDeviceIpInfoArray[index].deviceName))
		{
			/*pIpInfo[ipIndex].blueIp ��ֵ */
			pIpInfo[ipIndex].blueIp = LongFromChar(gDeviceIpInfoArray[index].blueIp);
			pIpInfo[ipIndex].redIp = LongFromChar(gDeviceIpInfoArray[index].redIp);
			pIpInfo[ipIndex].bluePort = gDeviceIpInfoArray[index].bluePort[idxPrtcl];
			pIpInfo[ipIndex].redPort = gDeviceIpInfoArray[index].redPort[idxPrtcl];
			ipIndex++;
		}

	}
	*num = ipIndex;
	if (ipIndex == 0)
	{
		retVal = 1;/*û�в��ҵ�*/
	}
	else
	{
		retVal = 0;
	}
	return retVal;
}

/*����IP�Ͷ˿ڲ����豸���ƣ�
���ҳɹ������㣬����ʧ�ܷ�������ֵ*/
UINT8 GetDeviceNameByIpAndPort(UINT16 port,UINT32 ip,UINT16 *deviceName)
{
	UINT8 retVal = 1;/*���ҳɹ�����0����ʼ��Ϊ1*/
	UINT16 index = 0;
	UINT8 flag = 0;

	if (0 == gDeviceNum)
	{
		retVal = 2;/*��ȡXML�ļ�ʧ��*/
	}
	for (index = 0;index < gDeviceNum;index++)
	{
		if ((ip == LongFromChar(gDeviceIpInfoArray[index].blueIp))||(ip == LongFromChar(gDeviceIpInfoArray[index].redIp)))
		{
			flag = 1;
		} 
		else
		{
			flag = 0;
		}
		if ((flag == 1)&&((port == gDeviceIpInfoArray[index].bluePort[0])||(port == gDeviceIpInfoArray[index].bluePort[1])
			||(port == gDeviceIpInfoArray[index].bluePort[2])||(port == gDeviceIpInfoArray[index].bluePort[3])||(port == gDeviceIpInfoArray[index].redPort[0])||(port == gDeviceIpInfoArray[index].redPort[1])
			||(port == gDeviceIpInfoArray[index].redPort[2])||(port == gDeviceIpInfoArray[index].redPort[3])))
		{
			*deviceName = gDeviceIpInfoArray[index].deviceName;
			retVal = 0;
			break;
		} 
		else
		{
			retVal = 1;
		}
	}
	return retVal;
}

/*����IP�Ͷ˿ڲ����豸���ƣ�
���ҳɹ������㣬����ʧ�ܷ�������ֵ*/
UINT8 GetEtcsIdByIpAndPort(UINT16 port, UINT32 ip, UINT32 *etcsId)
{
	UINT8 retVal = 1;/*���ҳɹ�����0����ʼ��Ϊ1*/
	UINT16 index = 0;
	UINT8 flag = 0;
	UINT16 devName = 0;
	UINT16 logicId = 0;
	

	if (0 == gDeviceNum)
	{
		retVal = 2;/*��ȡXML�ļ�ʧ��*/
	}
	for (index = 0;index < gDeviceNum;index++)
	{
		if ((ip == LongFromChar(gDeviceIpInfoArray[index].blueIp))||(ip == LongFromChar(gDeviceIpInfoArray[index].redIp)))
		{
			flag = 1;
		} 
		else
		{
			flag = 0;
		}
		if ((flag == 1)&&((port == gDeviceIpInfoArray[index].bluePort[0])||(port == gDeviceIpInfoArray[index].bluePort[1])
			||(port == gDeviceIpInfoArray[index].bluePort[2])||(port == gDeviceIpInfoArray[index].bluePort[3])||(port == gDeviceIpInfoArray[index].redPort[0])||(port == gDeviceIpInfoArray[index].redPort[1])
			||(port == gDeviceIpInfoArray[index].redPort[2])||(port == gDeviceIpInfoArray[index].redPort[3])))
		{
			devName = gDeviceIpInfoArray[index].deviceName;
			logicId = gDeviceIpInfoArray[index].logicId;

			dquDevName2EtcsId(devName, logicId, etcsId);

			retVal = 0;

			break;
		} 
		else
		{
			retVal = 1;
		}
	}
	return retVal;
}

void SetDataOfConfigLeu(char chTempData[20],char chTempName[200],UINT16 wDataLen)
{
	static int wConfigCycle = DEFAULT_ZERO;
	char chChangeData[20];
	int wTempID = DEFAULT_ZERO;

	memset(chChangeData,DEFAULT_ZERO,sizeof(char) * 20);

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<DeviceTypeAndId>"))
	{
		if (('0' == chTempData[0]) && ('x' == chTempData[1]))
		{
			memset(chChangeData,DEFAULT_ZERO,sizeof(char) * 20);
			memcpy(chChangeData,&chTempData[2],4);	

			//str_NativeIpConfig.strNativeIPConfig[wConfigCycle].wDevicdTpye = StrToIntData(chChangeData,2);

			gDeviceIpInfoArrayLeu[wConfigCycle].deviceName = StrToIntData(chChangeData,4);
		}

		/*if(('0' == chTempData[0]) && ('x' == chTempData[1]))
		{
			memset(chChangeData,DEFAULT_ZERO,sizeof(char) * 20);
			memcpy(chChangeData,&chTempData[2],4);
			gDeviceIpInfoArray[wConfigCycle].deviceName = StrToIntData(chChangeData,4);
		}*/
		else
		{
			printf("Id and Tpye error!\n");
		}
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<RedIP>"))
	{
		//memcpy(str_NativeIpConfig.strNativeIPConfig[wConfigCycle].chRedIP,chTempData,wDataLen);
		sscanf(chTempData,"%d.%d.%d.%d",&gDeviceIpInfoArrayLeu[wConfigCycle].redIp[0],&gDeviceIpInfoArrayLeu[wConfigCycle].redIp[1],&gDeviceIpInfoArrayLeu[wConfigCycle].redIp[2],&gDeviceIpInfoArrayLeu[wConfigCycle].redIp[3]);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<BlueIP>"))
	{
		//memcpy(str_NativeIpConfig.strNativeIPConfig[wConfigCycle].chBlueIP,chTempData,wDataLen);
		sscanf(chTempData,"%d.%d.%d.%d",&gDeviceIpInfoArrayLeu[wConfigCycle].blueIp[0],&gDeviceIpInfoArrayLeu[wConfigCycle].blueIp[1],&gDeviceIpInfoArrayLeu[wConfigCycle].blueIp[2],&gDeviceIpInfoArrayLeu[wConfigCycle].blueIp[3]);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<BluePort>"))
	{
		//str_NativeIpConfig.strNativeIPConfig[wConfigCycle].wSFPRedPort = atoi(chTempData);
		gDeviceIpInfoArrayLeu[wConfigCycle].bluePort[0] = atoi(chTempData);
	} 
	else
	{
	}

	if ((UINT8)DEFAULT_ZERO == strcmp(chTempName,"<RedPort>"))
	{
		//		str_NativeIpConfig.strNativeIPConfig[wConfigCycle].wSFPBluePort = atoi(chTempData);
		gDeviceIpInfoArrayLeu[wConfigCycle].redPort[0] = (UINT16)atoi(chTempData);
		wConfigCycle++;
		gDeviceNumLeu = wConfigCycle;
	} 
	else
	{
	}
}

void GetOneDataLeu(char chDataBuff[200],const UINT16 strLen)
{
	int wCycle = DEFAULT_ZERO;

	UINT8  chStartFlag = DEFAULT_ZERO;
	UINT8  chEndFlag = DEFAULT_ZERO;
	char  chTempData[20];
	char  chTempName[200];
	UINT8  chDataIndex = DEFAULT_ZERO;
	UINT8  chNameIndex = DEFAULT_ZERO;
	UINT8  chNameFlag = DEFAULT_ZERO;

	memset(chTempData,DEFAULT_ZERO,sizeof(char) * 20);
	memset(chTempName,DEFAULT_ZERO,sizeof(char) * 200);

	for (wCycle = (UINT16)DEFAULT_ZERO;wCycle < strLen;wCycle++)
	{
		if ((chDataBuff[wCycle] == '<') && ((UINT8)FLAG_SET != chStartFlag))
		{
			chNameFlag = (UINT8)FLAG_SET;
		} 
		else
		{
			if ((UINT8)FLAG_SET == chStartFlag)
			{
				chNameFlag = (UINT8)FLAG_UNSET;
			} 
			else
			{
			}
		}

		if (FLAG_SET == chNameFlag)
		{
			chTempName[chNameIndex] = chDataBuff[wCycle];
			chNameIndex++;
		} 
		else
		{
		}

		if (chDataBuff[wCycle] == '>')
		{
			if (wCycle == strLen - 1)
			{
				break;
			} 
			else
			{
				chStartFlag = FLAG_SET;
				continue;
			}
		} 
		else
		{
		}

		if ((UINT8)FLAG_SET == chStartFlag)
		{
			if ((chDataBuff[wCycle] == '<') && (chDataBuff[wCycle + 1] == '/'))
			{
				break;
			} 
			else
			{
				chTempData[chDataIndex] = chDataBuff[wCycle];
				chDataIndex++;
			}
		} 
		else
		{
		}
	}

	if ((chDataIndex > 0) && (chNameIndex > 0))
	{
		SetDataOfConfigLeu(chTempData,chTempName,chDataIndex);
	} 
	else
	{
	}
}
void ReadXmlLeuCICfg(const char * pPath)
{
	FILE *pFile = NULL;
	char ch;
	char chDataBuff[200];
	UINT16 wIndex = DEFAULT_ZERO;

	memset(gDeviceIpInfoArrayLeu,DEFAULT_ZERO,sizeof(gDeviceIpInfoArrayLeu));
	memset(chDataBuff,DEFAULT_ZERO,sizeof(char) * 200);

	if(gDeviceNumLeu == 0 )
	{
		if (NULL == (pFile = fopen(pPath,"rt")))
		{
			printf("Open File error!\n");

			return;
		} 
		else
		{
		}

		while (EOF != (ch=fgetc(pFile)))
		{
			if (ch != '\n')
			{
				if (ch != ' ')
				{
					chDataBuff[wIndex] = ch;
					wIndex++;
				} 
				else
				{
				}
			} 
			else
			{
				GetOneDataLeu(chDataBuff,wIndex);
				memset(chDataBuff,0,sizeof(char) * 200);
				wIndex = 0;
			}
		}


		fclose(pFile);

	}
}

/* �����豸���Ʋ���IP�Ͷ˿ڣ�
*���ҳɹ�����0������ʧ�ܷ��� ����ֵ 
*һ���豸�ж��IP��numΪ���ҵ���IP����
*/
UINT8 GetIpAndPortByDeviceNameLEUCI(UINT16 deviceName,IpInfo pIpInfo[],UINT8 *num)
{
	UINT16 index = 0;/*���ҷ�Χ��Ŀ����*/
	UINT8 retVal = 1;/*����ֵ�����ҳɹ������㣬��ʼ��Ϊ1*/
	UINT8 ipIndex = 0;/*���ҵ���IP��Ŀ*/

	if (0 == gDeviceNumLeu)
	{
		/*printf("��ȡXML�����ļ�ʧ�ܣ�\n");*/
		retVal = 2;/*��ȡXML�����ļ�ʧ��*/
	}
	for (index = 0; index < gDeviceNumLeu; index++)
	{
		/* �������豸���ƺ�Э���Ƿ���� */		
		if (deviceName == gDeviceIpInfoArrayLeu[index].deviceName)
		{
			/*pIpInfo[ipIndex].blueIp ��ֵ */
			pIpInfo[ipIndex].blueIp = LongFromChar(gDeviceIpInfoArrayLeu[index].blueIp);
			pIpInfo[ipIndex].redIp = LongFromChar(gDeviceIpInfoArrayLeu[index].redIp);
			pIpInfo[ipIndex].bluePort = gDeviceIpInfoArrayLeu[index].bluePort[0];
			pIpInfo[ipIndex].redPort = gDeviceIpInfoArrayLeu[index].redPort[0];
			ipIndex++;
		}

	}
	*num = ipIndex;
	if (ipIndex == 0)
	{
		retVal = 1;/*û�в��ҵ�*/
	}
	else
	{
		retVal = 0;
	}
	return retVal;
}

/*����IP�Ͷ˿ڲ����豸���ƣ�
���ҳɹ������㣬����ʧ�ܷ�������ֵ*/
UINT8 GetDeviceNameByIpAndPortLEU(UINT16 port,UINT32 ip,UINT16 *deviceName)
{
	UINT8 retVal = 1;/*���ҳɹ�����0����ʼ��Ϊ1*/
	UINT16 index = 0;
	UINT8 flag = 0;

	if (0 == gDeviceNumLeu)
	{
		retVal = 2;/*��ȡXML�ļ�ʧ��*/
	}
	for (index = 0;index < gDeviceNumLeu;index++)
	{
		if ((ip == LongFromChar(gDeviceIpInfoArrayLeu[index].blueIp))||(ip == LongFromChar(gDeviceIpInfoArrayLeu[index].redIp)))
		{
			flag = 1;
		} 
		else
		{
			flag = 0;
		}
		if ((flag == 1)&&((port == gDeviceIpInfoArrayLeu[index].bluePort[0])||(port == gDeviceIpInfoArrayLeu[index].redPort[0])))
		{
			*deviceName = gDeviceIpInfoArrayLeu[index].deviceName;
			retVal = 0;
			break;
		} 
		else
		{
			retVal = 1;
		}
	}
	return retVal;
}

/*********************************************
*�������ܣ���ȡCI����ATS��������
*����˵����
*��ڲ�����
		pPath�������ļ�·��
*���ڲ�����
		��
*����ֵ��ʧ�ܻ�ɹ� 0:�ɹ� 1��ʧ��
*********************************************/
UINT8 ReadCiAtsCfg(const char *pPath)
{
	UINT8 retResult=ERR_DEF;

	TiXmlNode* rootNode;
	TiXmlNode* tNode;
	TiXmlNode* tNode1;
	const char* pChar;
	char charArr[100];
	int nodeType;
	UINT8 ciCnt=0;

	TiXmlDocument doc(pPath);
	if(doc.LoadFile())
	{
		rootNode=doc.RootElement();

		tNode=rootNode->FirstChild("InAts");
		pChar=tNode->FirstChild()->Value();
		ciAtsCfgInfo.InAts=(UINT8)atoi(pChar);

		tNode=rootNode->FirstChild("XdInAts");
		pChar=tNode->FirstChild()->Value();
		ciAtsCfgInfo.XdInAts=(UINT8)atoi(pChar);

		tNode = rootNode->FirstChild("IpS");
		for( tNode = tNode->FirstChild();tNode;tNode = tNode->NextSibling() )
		{
			nodeType = tNode->Type();/*��ýڵ�����*/
			switch(nodeType)
			{
			case TiXmlNode::TINYXML_ELEMENT:
				tNode1=tNode->FirstChild("DevId");
				pChar=tNode1->FirstChild()->Value();
				memcpy(charArr,&(pChar[2]),4);	
				ciAtsCfgInfo.IpS[ciCnt].DevId=(UINT16)StrToIntData(charArr,4);

				tNode1=tNode->FirstChild("RedIp");
				pChar=tNode1->FirstChild()->Value();
				sscanf(pChar,"%d.%d.%d.%d",&(charArr[0]),&(charArr[1]),&(charArr[2]),&(charArr[3]));
				ciAtsCfgInfo.IpS[ciCnt].RedIp=LongFromChar((UINT8 *)charArr);

				tNode1=tNode->FirstChild("BlueIp");
				pChar=tNode1->FirstChild()->Value();
				sscanf(pChar,"%d.%d.%d.%d",&(charArr[0]),&(charArr[1]),&(charArr[2]),&(charArr[3]));
				ciAtsCfgInfo.IpS[ciCnt].BlueIp=LongFromChar((UINT8 *)charArr);

				tNode1=tNode->FirstChild("RedPort");
				pChar=tNode1->FirstChild()->Value();
				ciAtsCfgInfo.IpS[ciCnt].RedPort=(UINT16)atoi(pChar);

				tNode1=tNode->FirstChild("BluePort");
				pChar=tNode1->FirstChild()->Value();
				ciAtsCfgInfo.IpS[ciCnt].BluePort=(UINT16)atoi(pChar);

				ciCnt++;/*ci��������*/
				break;
			default:
				break;
			}
		}
		retResult=SUC_DEF;/*�������쳣�жϣ�����Ϊ�ɹ��������Ż�*/
	}

	ciAtsCfgInfo.CiCount=ciCnt;

	return retResult;
}

/*********************************************
*�������ܣ���ȡָ��CI��ATS�����е�����ڵ���Ϣ
*����˵����
*��ڲ�����
		devId��CI�豸ID
*���ڲ�����
		pIpInfo��CI��ATS�����е�����ڵ���Ϣ
*����ֵ��ʧ�ܻ�ɹ� 0:�ɹ� 1��ʧ��
*********************************************/
UINT8 ReadCiAtsIpInfo(UINT16 devId,CiAtsIpInfo *pIpInfo)
{
	UINT8 retResult=ERR_DEF;
	UINT8 i;
	for(i=0;i<ciAtsCfgInfo.CiCount;i++)
	{
		if(devId==ciAtsCfgInfo.IpS[i].DevId)
		{/*�豸���ƥ��*/
			pIpInfo->DevId=devId;
			pIpInfo->RedIp=ciAtsCfgInfo.IpS[i].RedIp;
			pIpInfo->BlueIp=ciAtsCfgInfo.IpS[i].BlueIp;
			pIpInfo->RedPort=ciAtsCfgInfo.IpS[i].RedPort;
			pIpInfo->BluePort=ciAtsCfgInfo.IpS[i].BluePort;

			retResult=SUC_DEF;
			break;
		}
	}

	return retResult;
}

/*********************************************
*�������ܣ���ȡCI�Ƿ���ATS����־
*����˵����
*��ڲ�����
		��
*���ڲ�����
		��
*����ֵ��CI�Ƿ���ATS����־
*********************************************/
UINT8 ReadCiInAtsFlag(void)
{
	return ciAtsCfgInfo.InAts;
}

/*********************************************
*�������ܣ���ȡCI���ֵ�ͨ�ŷ�ʽ��־
*����˵����
*��ڲ�����
		��
*���ڲ�����
		��
*����ֵ��CI���ֵ�ͨ�ŷ�ʽ��־
*********************************************/
UINT8 ReadXdInAtsFlag(void)
{
	return ciAtsCfgInfo.XdInAts;
}