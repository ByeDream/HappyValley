/************************************************************************/
/* The Terminal Windows Implementation
/*
/* ByeDream
/* Dec-26-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "TerminalWin.h"

#include "resource.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

LRESULT CALLBACK _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HV::TerminalWin *_pTerminal = NULL;
	LRESULT result;

	if (WM_CREATE == msg)
	{
		_pTerminal = (HV::TerminalWin *)(((LPCREATESTRUCT)lParam)->lpCreateParams);
	}

	if (_pTerminal)
	{
		result = _pTerminal->WndProc(msg, wParam, lParam);
	}
	else
	{
		result = DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return result;
}

INT_PTR CALLBACK _About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

HV::TerminalWin::TerminalWin(Application *app)
	: Terminal(app)
	, mHInstance(NULL)
	, mHWnd(NULL)
	, mName(NULL)
{

}

HV::TerminalWin::~TerminalWin()
{

}

void HV::TerminalWin::initialize()
{
	mHInstance = GetModuleHandle(NULL);

	const SIZE_T MAX_LOADSTRING = 100;
	mName = new char[MAX_LOADSTRING];
	memset(mName, 0, MAX_LOADSTRING);
	LoadString(mHInstance, IDC_HAPPYVALLEY, mName, MAX_LOADSTRING);

	registerWindowClass();
	createWindow();
}

void HV::TerminalWin::finitialize()
{
	SAFE_DELETE(mName);
}

void HV::TerminalWin::show()
{
	ShowWindow(mHWnd, SW_SHOWDEFAULT);
	UpdateWindow(mHWnd);
}

void HV::TerminalWin::registerWindowClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = _WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = mHInstance;
	wcex.hIcon = LoadIcon(mHInstance, MAKEINTRESOURCE(IDI_HAPPYVALLEY));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_HAPPYVALLEY);
	wcex.lpszClassName = mName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	if (!RegisterClassEx(&wcex))
	{
		THROW_EXCEPTION(SimpleException, "register wnd class failed");
	}
}

void HV::TerminalWin::createWindow()
{
	RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);
	mHWnd = CreateWindow(mName, mName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, mHInstance, LPVOID(this));

	if (!mHWnd)
	{
		THROW_EXCEPTION(SimpleException, "create window failed");
	}
}

LRESULT HV::TerminalWin::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	WORD wmId, wmEvent;

	switch (msg)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(mHInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), mHWnd, _About);
			break;
		case IDM_EXIT:
			DestroyWindow(mHWnd);
			break;
		default:
			return DefWindowProc(mHWnd, msg, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(mHWnd, msg, wParam, lParam);
	}
	return 0;
}

// EOF
