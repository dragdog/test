/************************************************************************
*
* 文件名   ：  dquDataTypeDefine.h
* 版权说明 ：  北京交控科技有限公司
* 版本号   ：  1.0
* 创建时间 ：  2011.09.22
* 作者     ：  软件部
* 功能描述 ：  文件系统文件预处理过程  
* 使用注意 ： 
* 修改记录 ：  
*
************************************************************************/


#ifndef    DQU_DATA_TYPE_DEFINE_H
#define    DQU_DATA_TYPE_DEFINE_H


#define EMAP_DATA_TYPE                 0x10   /*电子地图ID*/

/*电子地图各数据ID*/
#define EMAP_NOP_0					0	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_LINK_ID				1	/*link数据ID*/
#define EMAP_POINT_ID				2	/*道岔数据ID*/
#define EMAP_SIGNAL_ID				3	/*信号机数据ID*/
#define EMAP_BALISE_ID				4	/*应答器数据ID*/
#define EMAP_AXLE_ID				5	/*记轴器数据ID*/
#define EMAP_SCREEN_ID				6	/*安全屏蔽门数据ID*/
#define EMAP_EMERG_STOP_BUTTON_ID	7	/*紧急停车按钮数据ID*/
#define EMAP_STATION_ID				8	/*停车区域数据ID*/
#define EMAP_STOPPOINT_ID			9	/*停车点数据ID*/
#define EMAP_AXLE_SGMT_ID			10	/*记轴区段数据ID*/
#define EMAP_LOGIC_SGMT_ID			11	/*逻辑区段数据ID*/
#define EMAP_ROUTE_ID				12	/*进路数据ID*/
#define EMAP_PROTECT_LINK_ID		13	/*保护区段数据ID*/
#define EMAP_BLOC_ACCESS_LINK_ID	14	/*点式接近区段数据ID*/
#define EMAP_CBTC_ACCESS_LINK_ID	15	/*CBTC接近区段数据ID*/
#define EMAP_GRADE_ID				16	/*坡度数据ID*/
#define EMAP_STATIC_RES_SPEED_ID	17	/*静态限速数据ID*/
#define EMAP_DEFAULT_ROUTE_ID		18	/*默认行车序列数据ID*/
#define EMAP_RUN_LEVEL_ID			19	/*站间运营级别数据ID*/
#define EMAP_EXIT_ROUTE_ID			20	/*退出CBTC区域数据ID*/
#define EMAP_TRAIN_LINE_ID			21	/*非CBTC区域数据ID*/
#define EMAP_AR_ID					22	/*自动折返数据ID*/
#define EMAP_ZC_DIVID_POINT_ID		23	/*ZC分界点数据ID*/
#define EMAP_ZC_COM_ZONE_ID			24	/*ZC共管区域数据ID*/
#define EMAP_CI_ID					25	/*CI数据ID*/
#define EMAP_ATS_ID					26	/*ATS数据ID*/
#define EMAP_CONRESSPEED_ID			27	/*统一限速数据ID*/
#define EMAP_CONGRADE_ID			28	/*统一坡度数据ID*/
#define EMAP_PHYSICAL_SGMT_ID		29	/*物理区段数据ID*/
#define EMAP_BLOC_TRG_AREA_ID		30	/*点式触发区段数据ID*/
#define EMAP_CBTC_TRG_AREA_ID		31	/*CBTC触发区段数据ID*/
#define EMAP_PLAT_FORM_ID			32	/*站台数据ID*/
#define EMAP_COME_GO_ID				33	/*往复进路表*/
#define EMAP_FLOOD_GATE_ID			34	/*防淹门---20140718刘天一添加*/
#define EMAP_SPKS_BUTTON_ID			35	/*SPKS按钮---20141217刘天一添加*/
#define EMAP_DEPOT_GATE_ID			36	/*车库门---20141217刘天一添加*/
#define EMAP_TUNNEL_ID				37	/*隧道--20160405侯国建添加*/
#define EMAP_ZC_ID					38	/*ZC数据ID*/
#define EMAP_MULTI_POINT_ID			39  /*多开道岔数据ID add by qxt 20170621*/
#define EMAP_NOP_40					40	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_POWERLESSZONE_ID	    41	/*分相区表ID，依据互联互通数据结构V12.0.0添加，add by lmy 20180312*/
#define EMAP_RES_SPEED_SGMT_ID	    42	/*限速区段表ID，依据车车数据结构V1.0添加，add by sds 2019-6-13*/
#define EMAP_NOCOM_TRAIN_AREA_ID    43	/*非通信车检测区表ID，依据车车数据结构V1.0添加，add by sds 2019-6-13*/
#define EMAP_TRANSIT_ZONE_ID        44	/*交渡分区表ID，依据车车数据结构V1.0添加，add by sds 2019-6-13*/
#define EMAP_OC_ID                  45	/*OC表ID，依据车车数据结构V1.0添加，add by sds 2019-6-13*/
#define EMAP_ITS_ID                 46	/*ITS表ID，依据车车数据结构V1.0添加，add by sds 2019-6-13*/
#define EMAP_NOP_47					47	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_48					48	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_49					49	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_50					50	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_CARSTOP_ID			    51    /*车档表ID，依据互联互通数据结构V15.0.0添加，add by sds 20181031*/
#define EMAP_NOP_52					52	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_53					53	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_54					54	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_55					55	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_56					56	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_57					57	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_58					58	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_59					59	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/
#define EMAP_NOP_60					60	/*为实现数据表与实际表数值一一对应，所以从0开始 补全数据 ID  sds*/



/*系统配置数据  宏定义*/
#define  CBTC_CONFIGDATA_TYPE              0x20      /*系统配置数据*/
#define  CBTC_TRAIN_TYPE_CONFIG_ID         1       /*列车车型配置数据ID*/
#define  CBTC_TRAIN_INFO_ID                2       /*车型性能数据ID*/
#define  CBTC_CONFIG_DATA_ID               3       /*各子系统公用数据ID*/
#define  CBTC_CONFIG_DEPOT_ID              4       /*车辆段配置信息表ID*/

/*协议配置 宏定义*/
#define DSU_DATA_PROTCL_TYPE              0x30   /*协议表bookId*/
#define DSU_DATA_PROTCL_RSSP_LNK_ID        1   /*rssp lnk 信息tableId*/
#define DSU_DATA_PROTCL_CI_PLATFORM_ID     2   /*平台数据结构体tableId*/
#define DSU_DATA_PROTCL_COMM_INFO_ID       3   /*设备通信属性表tableId*/
#define DSU_DATA_PROTCL_FSFB_LNK_ID		   6	 /*FSFB协议链路信息表tableId*/

/*设备IP配置 宏定义*/
#define DSU_DEVICE_IP_CONFIG_TYPE        0x40     /*设备ip表bookId*/ 
#define DSU_DATA_IP_IP_ID               1       /*ip地址表tableId*/
#define DSU_DATA_IP_COMM_PROTCL_ID      2       /*设备类型与通信协议关系表tableId*/
#define DSU_DATA_IP_CCOV_COMM_ID        3       /*CCOV通信关系配置表tableId*/
#define DSU_DATA_IP_ZCDSU_COMM_ID       4       /*ZCDSU通信关系配置表tableId*/
#define DSU_DATA_IP_GATEWAY_ID          5     /*hxq,设备网关表tableId*/
#define DSU_CI_ATS_DEVICE_IP_ADDR       6     /*CI_ATS网设备IP地址 add by qxt 20160811 */

/*ATP/ATO公共配置数据 宏定义*/
#define  VOBC_CONFIGDATA_TYPE            0x50    /*ATP/ATO二级子系统公共配置数据*/
#define  VOBC_RAIL_INFO_ID            1       /*ATP/ATO公共配置线路数据ID*/
#define  VOBC_TRAIN_INFO_ID           2       /*ATP/ATO公共配置车辆数据ID*/
#define  VOBC_ACC_TABLE_ID            3       /*最大牵引加速度ID*/
#define  VOBC_RATIO_ACC_TABLE_ID      4       /*速度拟合阈值的ID 针对单轨数据结构V3.0.0 add by lmy 20180305*/
#define  VOBC_AWAKEN_CONFIG_DATA      5       /*唤醒配置数据 针对互联互通数据结构V13.0.0 add by sds 20180625*/
#define  VOBC_IO_TABLE_ID			  6		  /*开关量配置输入输出配置表ID add by sds 2019-6-20 车车数据结构V1.0*/

/*TMS公共配置数据 宏定义*/
#define  TMS_CONFIGDATA_TYPE            0x81
#define  TMS_PORT_INFO_ID            1       /*端口配置信息表索引*/
#define  TMS_VOBC_SEND_INFO_ID            2       /*端口对应VOBC->TMS组包信息*/
#define  TMS_VOBC_RECV_INFO_ID            3       /*端口对应TMS->VOBC组包信息*/

/*ATP配置数据  宏定义*/
#define  VOBC_ATP_CONFIGDATA_TYPE        0x60            /*ATP配置数据 类型*/
#define  VOBC_ATP_TRAIN_CONFIGDATA_ID            1       /*ATP车辆参数表ID*/
#define  VOBC_ATP_SECURITY_CONFIGDATA_ID         2       /*ATP安全防护表ID*/
#define	 VOBC_ATP_KEY_CFG_ID					 3		 /*ATP安全防护功能开关配置ID  add by sds 2019-6-20 车车数据结构V1.0*/
#define  VOBC_ATP_MA_CFG_ID						 4		 /*ATP产品参数配置表（移动授权计算功能参数配置）add by sds 2019-6-20 车车数据结构V1.0*/
/*数据ID宏定义结束*/

/*可变应答器版本号 宏定义*/
#define DSU_BALISE_VER_DATA_TYPE   0x80
#define DSU_BALISE_VER_DATA_ID    1                      /*可变应答器版本ID*/

/*ATO配置数据  宏定义*/
#define  VOBC_ATO_CONFIGDATA_TYPE            0x90          /*ATO配置数据 类型*/
#define  VOBC_ATO_APP_CONFIGDATA_ID         1            /*ATO软件功能配置参数*/
#define  VOBC_ATO_TRACTIONSAMPLE_ID         2            /*车辆牵引性能拟合曲线表*/
#define  VOBC_ATO_BREAKSAMPLE_ID            3            /*车辆制动性能拟合曲线表*/
#define  VOBC_ATO_ESPECIALTRIPNUM_ID        4            /*特殊车次号段*/
#define  VOBC_ATO_SPEED_CONFIGDATA_ID       5            /*ATO速度参数表*/

/*ZC子系统配置数据*/
#define  ZC_CONFIGDATA_TYPE          0xA0
#define  ZC_CONFIGDATA_ID          1           /*ZC公用配置数据*/
#define  ZC_TESTSEGMENT_ID         2           /*动态测试区段关系表*/
#define  ZC_LOGIC_SEGMET_LOCAL     3           /*逻辑区段位置关系表*/
#define  ZC_FunctionConfig         4           /*功能配置表，依据互联互通数据结构V15.0.0添加，add by sds 20181031*/
#define  ZC_ESBLINKAGESTATION_ID   5		   /*ESB联动车站编号表，依据互联互通数据结构V12.0.0添加，add by lmy 20180319*/

/*DSU子系统配置数据*/
#define  DSU_CONFIGDATA_TYPE         0xC0        
#define  DSU_CONFIGDATA_ID          1           /*DSU配置数据*/

/*编号对照表*/
#define  IDMAP_DATA_TYPE			0xF1  /*编号对照表ID*/

/*编号对照表各数据ID*/
#define IDMAP_ZC_ID             1    /*ZC编号数据ID*/
#define IDMAP_CI_ID             2    /*CI编号数据ID*/
#define IDMAP_ATS_ID            3    /*ATS编号数据ID*/
#define IDMAP_SIGNAL_ID         4    /*信号机编号数据ID*/
#define IDMAP_TRACKSEC_ID		5	/*轨道区段编号数据ID*/
#define IDMAP_SWITCH_ID			6	/*道岔编号数据ID*/
#define IDMAP_STATION_ID		7	/*站台编号数据ID*/
#define IDMAP_EMERGBTN_ID		8	/*紧急关闭按钮编号数据ID*/
#define IDMAP_BALISE_ID			9	/*应答器编号数据ID*/
#define IDMAP_PSD_ID			10	/*屏蔽门编号数据ID*/
#define IDMAP_VOBC_ID			11	/*VOBC编号数据ID*/
#define IDMAP_FPD_ID			12	/*屏蔽门编号数据ID*/
#define IDMAP_DSU_ID			13	/*DSU编号数据ID*/
#define IDMAP_LOGICSEC_ID       14  /*逻辑区段编号数据ID  add by qxt 20160811*/
#define IDMAP_AOMSEC_ID         15  /*AOM编号数据ID  add by sds 2018-6-14*/
#define IDMAP_SPKSSEC_ID        16  /*SPKS编号数据ID  add by sds 2018-6-14*/
#define IDMAP_GODSEC_ID         17  /*车库门编号数据ID  add by sds 2018-6-14*/
#define IDMAP_STASEC_ID         18  /*车站编号数据ID  add by slm 2018-6-14*/
#define IDMAP_PHYSEC_ID         19  /*物理区段编号数据ID  add by slm 2018-6-14*/

/*VOBC重叠区配置数据  互联互通需求新增， add by qxt 20160811*/
#define  VOBC_OVERLAP_CONFIGDATA_TYPE			0xF3  /*VOBC重叠区配置数据表ID*/

/*VOBC重叠区配置数据表各数据ID*/
#define VOBC_OVERLAP_GROUNDDEV_ID             1    /*车载所属及接管的地面设备信息ID*/
#define VOBC_OVERLAP_LINECONNECT_ID           2    /*线路连接关系ID*/

/*NDSU子系统配置数据*/
#define NDSU_CONFIGDATA_TYPE  0xF4
#define NDSU_CONFIGDATA_ID  1  /*NDSU配置数据*/

#endif