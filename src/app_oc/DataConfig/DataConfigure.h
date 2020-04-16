#ifndef _DATACONFIGURE_H_
#define _DATACONFIGURE_H_


#define NOWS
//#define DEBUGINPUT

#ifdef __cplusplus
extern "C" {
#endif
/**=============================================================================
    函数名: DataConfigure()
      描述: OC数据初始化     
      功能: 根据7号线的电子地图进行数据配置，规定当前软件的OC管理的设备数量、ID等。
      参数：无
    返回值: 无
==============================================================================*/
void DataConfigure(void);

#ifdef __cplusplus
}
#endif

#endif