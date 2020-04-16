/******************************************************************
** ��Ŀ���ƣ�
** ��Ȩ��Ϣ��
** �ļ����ƣ�typeDefine.h �汾�ţ�V0.0.0
** �����ˣ�           �������ڣ�2010.11.23
** ����ժҪ��
** �ο����ף�
*******************************************************************
** �޸����ڣ�
** �޸����ݣ�
******************************************************************/

#ifndef  _TYPE_DEFINE_H_
#define  _TYPE_DEFINE_H_
#include <assert.h>
#define TICKTIME 250 /* Ӧ�ó�����������ʱ��(ms) */


typedef unsigned char       BYTE;    /*�޷���8λ*/
typedef unsigned char       *LPBYTE; /*�޷���8λָ��*/
typedef unsigned short      WORD;
typedef unsigned short      *LPWORD;
typedef unsigned long       DWORD;
typedef unsigned long       *LPDWORD;
typedef int                 INT;
typedef int					*PINT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef int                 BOOL;
typedef void				*PVOID;
/*typedef char				CHAR;*/
typedef short				SHORT;
typedef long				LONG;
typedef unsigned short		USHORT;
typedef double DOUBLE;

/* ASSERT */
#ifndef ASSERT
#ifdef WIN32
#define ASSERT(a) assert(a)
#else
#define ASSERT(a)
#endif
#endif

/* VOID */
#ifndef VOID
#define VOID				void
#endif

/* FALSE */
#ifndef FALSE
#define FALSE				0
#endif

/* TRUE */
#ifndef TRUE
#define TRUE				1
#endif

/* STATUS */
#ifndef STATUS
#define STATUS int
#endif

/* OK */
#ifndef OK
#define OK					0
#endif

/* ERR */
#ifndef ERR
#define ERR					(-1)
#endif

/* NULL */
#ifndef NULL
#define NULL				0
#endif

/* LOCAL */
#ifndef LOCAL
#define LOCAL				static
#endif

/* CONST */
#ifndef CONST
#define CONST				const
#endif

/* MAX */
#ifndef MAX
#define MAX(a,b)			(((a) > (b)) ? (a) : (b))
#endif

/* MIN */
#ifndef MIN
#define MIN(a,b)			(((a) < (b)) ? (a) : (b))
#endif

/* MAKEWORD */
#ifndef MAKEWORD
#define MAKEWORD(a, b)		((WORD)(((BYTE)(((DWORD)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD)(b)) & 0xff))) << 8))
#endif

/* MAKELONG */
#ifndef MAKELONG
#define MAKELONG(a, b)		((LONG)(((WORD)(((DWORD)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#endif

/* LOWORD */
#ifndef LOWORD
#define LOWORD(l)			((WORD)(((DWORD)(l)) & 0xffff))
#endif

/* HIWORD */
#ifndef HIWORD
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#endif

/* LOBYTE */
#ifndef LOBYTE
#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))
#endif

/* HIBYTE */
#ifndef HIBYTE
#define HIBYTE(w)           ((BYTE)((((DWORD)(w)) >> 8) & 0xff))
#endif

/* ABS */
#ifndef ABS
#define ABS(a)				(((a) < 0) ? -(a) : (a))
#endif

#endif /*_TYPE_DEFINE_H_*/
