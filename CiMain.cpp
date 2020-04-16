#include <Windows.h>
#include "stdio.h"
#include "CiSimulation.h"
/*#include ".\\src\ciapp\System\SystemConfigDefine.h"*/
#include "SystemParameterManage.h"
#include "GLOBALVAR.h"


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
UINT32 count = 0;
WCHAR tmp_str_buf[100];

UINT8 OCId = ReadLocalOCId();
WCHAR CIName[20];

UINT8 LineId = 0;
UINT8 LineType = 0;

void getCiNameAndVersion(UINT8 ciid);/*��������ID��ȡ�������������ƺ�����汾��*/

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
{
	static TCHAR szAppName[] = TEXT("OcSimulation");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = szAppName;
	

	if (!RegisterClass(&wndClass))
	{
		MessageBox(NULL,TEXT("error init"),szAppName,MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,
						TEXT("OcSimulation"),
						WS_OVERLAPPEDWINDOW|WS_VSCROLL,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						NULL,
						NULL,
						hInstance,
						NULL);
	ShowWindow(hwnd,nShowCmd);
	UpdateWindow(hwnd);

	
	while (GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return msg.wParam;
} 


/*
 * ��Ϣ����
 */
LRESULT CALLBACK WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;
	LRESULT ret = 0;
	
	int wmId, wmEvent; 
	static HFONT hFont;
	static HWND hBtn;
	static HWND hStatic;
	static HINSTANCE hInst;

	switch (msg)
	{
	case WM_CREATE:
		if (0 != OCSimulationInit())
		{
			MessageBoxA(NULL,"error","error",MB_ICONERROR );
			OCSimulationRelease();
			PostQuitMessage(0);			
		}
		else
		{
			OCSimulationStart();
		}

		//�����߼�����
		hFont = CreateFont(
			-15/*�߶�*/, -7.5/*���*/, 0, 0, 400 /*һ�����ֵ��Ϊ400*/,
			FALSE/*����б��*/, FALSE/*�����»���*/, FALSE/*����ɾ����*/,
			DEFAULT_CHARSET, //ʹ��Ĭ���ַ���
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, //���в������ù�
			DEFAULT_QUALITY, //Ĭ���������
			FF_DONTCARE, //��ָ��������*/
			TEXT("΢���ź�") //������
			);
		//������̬�ı��ؼ�
		hStatic = CreateWindow(
			TEXT("static"), //��̬�ı��������
			TEXT("��ǰΪVBTCģʽ"), //�ؼ����ı�
			WS_CHILD /*�Ӵ���*/ | WS_VISIBLE /*����ʱ��ʾ*/ | WS_BORDER /*���߿�*/| BS_CENTER | BS_VCENTER,
			30 /*X����*/, 60/*Y����*/, 150/*���*/, 50/*�߶�*/, hwnd/*�����ھ��*/,
			(HMENU)1, //Ϊ�ؼ�ָ��һ��Ψһ��ʶ��
			hInst, //��ǰʵ�����
			NULL
			);
		//������ť�ؼ�
		hBtn = CreateWindow(
			TEXT("button"), //��ť�ؼ�������
			TEXT("�л�ģʽ"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*��ƽ��ʽ*/,
			30 /*X����*/, 120 /*Y����*/, 150 /*���*/, 50/*�߶�*/,
			hwnd, (HMENU)2 /*�ؼ�Ψһ��ʶ��*/, hInst, NULL
			);
		SendMessage(hStatic,WM_SETFONT,(WPARAM)hFont,NULL);//�����ı�������
		SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, NULL);//���ð�ť����

		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId){
		case 2: //���°�ť
			if(MODE_VBTC == g_stOC.SysRunMode)
			{
				//�����ı��������
				SetWindowText( hStatic, TEXT("��ǰΪӰ��ģʽ") );
				g_stOC.SysRunMode = MODE_SHADOW;
			}
			else
			{
				//�����ı��������
				SetWindowText( hStatic, TEXT("��ǰΪVBTCģʽ") );
				g_stOC.SysRunMode = MODE_VBTC;
			}
			
			break;
		default:
			//���������Ϣһ��Ҫ���� DefWindowProc ����
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);
		GetClientRect(hwnd,&rect);
		getCiNameAndVersion(OCId);/*��������ID��ȡ�������������ƺ�����汾��*/
		DrawText(hdc,tmp_str_buf,-1,&rect,DT_TOP|DT_CENTER|DT_LEFT);
		EndPaint(hwnd,&ps);	
				
		break;

	case WM_DESTROY:
		DeleteObject(hFont);
		OCSimulationRelease();
		PostQuitMessage(0);
		break;

	default:
		ret = DefWindowProc(hwnd,msg,wParam,lParam);
		break;
	}
	return ret;
}


void getCiNameAndVersion(UINT8 ciid)
{
	switch(ciid)
	{
	case 1:
		wsprintf(tmp_str_buf,TEXT("OC1\n Disney\n ����汾��V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	case 2:
		wsprintf(tmp_str_buf,TEXT("OC2\n Disney\n ����汾��V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	case 3:
		wsprintf(tmp_str_buf,TEXT("OC3\n Disney\n ����汾��V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	case 4:
		wsprintf(tmp_str_buf,TEXT("OC4\n Disney\n ����汾��V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	case 5:
		wsprintf(tmp_str_buf,TEXT("OC5\n Disney\n ����汾��V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	case 6:
		wsprintf(tmp_str_buf,TEXT("OC6\n Disney\n ����汾��V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	default:
		break;
	}
}
 
