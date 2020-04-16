#ifndef _SIMDEF_H_
#define _SIMDEF_H_

#include "CommonTypes.h"

#define PRTL_CNT 4
#define PRTL_UDP_CNT 3//使用UDP作为传输层协议的协议数量
#define PRTL_TCP_SER_CNT 1//使用TCP作为传输层协议，并且作为TCP服务器端的协议数量
#define SFP_IDX 0//SFP协议编号
#define RP_IDX 1//RP协议编号
#define RSSP1_IDX 2//RSSP1协议编号
#define RSSP2_IDX 3//RSSP2协议编号

#define ERR_DEF 1
#define SUC_DEF 0

#define RED_NET 0
#define BLUE_NET 1

#endif
