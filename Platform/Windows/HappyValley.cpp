/************************************************************************/
/* The Entrance Of Windows Platform
/*
/* ByeDream
/* Dec-20-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "resource.h"

#include "Framework/Application.h"

using namespace HV;

int WINAPI WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	Application _app;
	MSG msg;
	HACCEL hAccelTable;
	INT32 returnValue = 0;

	try
	{
		_app.boot();

		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HAPPYVALLEY));

		while (_app.isRunning())
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					returnValue = (INT32)msg.wParam;
					break;
				}
				else
				{
					if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			}
			else
			{
				_app.doFrame();
			}
		}
	}
	catch (PSimpleException e)
	{
		const SIZE_T EXCEPTION_LABEL_LENGTH = 1024;
		char msgLabel[EXCEPTION_LABEL_LENGTH] = { 0 };
		sprintf_s(msgLabel, EXCEPTION_LABEL_LENGTH - 1, "[%s]%s", e->getTypeString(), e->getErrorMsg());
		printf("%s\n", msgLabel);
		MessageBox(0, msgLabel, 0, 0);
	}

	_app.shutDown();

	return returnValue;
}

// EOF