#ifndef _MACDEFINE_H_
#define _MACDEFINE_H_
#define   TestType 0
/*定义测试案例*/
/*0:与文件对应的固定案例
*/

/*定义测试案例ID*/
/*1：测试案例1
  2：测试案例2
  31-33：测试案例3.1-3.3
  4：测试案例4
*/

/* ************************************************************
*        应用宏定义区   
**************************************************************/
#define OCNO 1
#if (1 == OCNO)
#define OCID 0x3d01
#define OCIP "5.12.1.13"
#elif (2 == OCNO)
#define OCID 0x3d02
#define OCIP "5.12.3.13"
#elif (3 == OCNO)
#define OCID 0x3d03
#define OCIP "5.12.5.13"
#elif (4 == OCNO)
#define OCID 0x3d04
#define OCIP "5.12.7.13"
#elif (5 == OCNO)
#define OCID 0x3d05
#define OCIP "5.12.9.13"
#elif (6 == OCNO)
#define OCID 0x3d06
#define OCIP "5.12.11.13"
#elif (7 == OCNO)
#define OCID 0x3d07
#define OCIP "5.12.13.13"
#elif (8 == OCNO)
#define OCID 0x3d08
#define OCIP "5.12.15.13"
#endif

#endif