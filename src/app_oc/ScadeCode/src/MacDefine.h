#ifndef _MACDEFINE_H_
#define _MACDEFINE_H_
#define   TestType 0
/*������԰���*/
/*0:���ļ���Ӧ�Ĺ̶�����
*/

/*������԰���ID*/
/*1�����԰���1
  2�����԰���2
  31-33�����԰���3.1-3.3
  4�����԰���4
*/

/* ************************************************************
*        Ӧ�ú궨����   
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