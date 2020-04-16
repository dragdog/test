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

void getCiNameAndVersion(UINT8 ciid);/*根据联锁ID获取联锁集中区名称和软件版本号*/

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
 * 消息处理
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

		//创建逻辑字体
		hFont = CreateFont(
			-15/*高度*/, -7.5/*宽度*/, 0, 0, 400 /*一般这个值设为400*/,
			FALSE/*不带斜体*/, FALSE/*不带下划线*/, FALSE/*不带删除线*/,
			DEFAULT_CHARSET, //使用默认字符集
			OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, //这行参数不用管
			DEFAULT_QUALITY, //默认输出质量
			FF_DONTCARE, //不指定字体族*/
			TEXT("微软雅黑") //字体名
			);
		//创建静态文本控件
		hStatic = CreateWindow(
			TEXT("static"), //静态文本框的类名
			TEXT("当前为VBTC模式"), //控件的文本
			WS_CHILD /*子窗口*/ | WS_VISIBLE /*创建时显示*/ | WS_BORDER /*带边框*/| BS_CENTER | BS_VCENTER,
			30 /*X坐标*/, 60/*Y坐标*/, 150/*宽度*/, 50/*高度*/, hwnd/*父窗口句柄*/,
			(HMENU)1, //为控件指定一个唯一标识符
			hInst, //当前实例句柄
			NULL
			);
		//创建按钮控件
		hBtn = CreateWindow(
			TEXT("button"), //按钮控件的类名
			TEXT("切换模式"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_FLAT/*扁平样式*/,
			30 /*X坐标*/, 120 /*Y坐标*/, 150 /*宽度*/, 50/*高度*/,
			hwnd, (HMENU)2 /*控件唯一标识符*/, hInst, NULL
			);
		SendMessage(hStatic,WM_SETFONT,(WPARAM)hFont,NULL);//设置文本框字体
		SendMessage(hBtn, WM_SETFONT, (WPARAM)hFont, NULL);//设置按钮字体

		break;

	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId){
		case 2: //按下按钮
			if(MODE_VBTC == g_stOC.SysRunMode)
			{
				//更改文本框的内容
				SetWindowText( hStatic, TEXT("当前为影子模式") );
				g_stOC.SysRunMode = MODE_SHADOW;
			}
			else
			{
				//更改文本框的内容
				SetWindowText( hStatic, TEXT("当前为VBTC模式") );
				g_stOC.SysRunMode = MODE_VBTC;
			}
			
			break;
		default:
			//不处理的消息一定要交给 DefWindowProc 处理。
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);
		GetClientRect(hwnd,&rect);
		getCiNameAndVersion(OCId);/*根据联锁ID获取联锁集中区名称和软件版本号*/
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
		wsprintf(tmp_str_buf,TEXT("OC1\n Disney\n 软件版本：V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	case 2:
		wsprintf(tmp_str_buf,TEXT("OC2\n Disney\n 软件版本：V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	case 3:
		wsprintf(tmp_str_buf,TEXT("OC3\n Disney\n 软件版本：V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	case 4:
		wsprintf(tmp_str_buf,TEXT("OC4\n Disney\n 软件版本：V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	case 5:
		wsprintf(tmp_str_buf,TEXT("OC5\n Disney\n 软件版本：V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	case 6:
		wsprintf(tmp_str_buf,TEXT("OC6\n Disney\n 软件版本：V%d.%d.%d.%d\n Build: %02d/%02d/%02d"),VER_SOFT_PRODUCT,VER_SOFT_MODUAL,VER_SOFT_MIANVERSION,VER_SOFT_SUBVERSION, BUILD_YEAR, BUILD_MONTH, BUILD_DATA);
		break;
	default:
		break;
	}
}
 
