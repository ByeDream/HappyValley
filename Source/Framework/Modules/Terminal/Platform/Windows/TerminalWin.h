/************************************************************************/
/* The Terminal Windows Implementation
/*
/* ByeDream
/* Dec-26-2015
/************************************************************************/
#ifndef __TERMINAL_WIN_H__
#define __TERMINAL_WIN_H__

#include "Framework/Modules/Terminal/Terminal.h"

namespace HV
{
	class TerminalWin : public Terminal
	{
	public:
		TerminalWin(Application *app);
		virtual ~TerminalWin();

		virtual void				initialize();
		virtual void				finitialize();

		virtual void				show();

		LRESULT 					WndProc(UINT msg, WPARAM wParam, LPARAM lParam);

		inline HWND					getWindow() const { return mHWnd; }
		inline const char *			getName() const { return mName; }

	private:
		void						registerWindowClass();
		void						createWindow();

	private:
		HINSTANCE					mHInstance;
		HWND						mHWnd;
		char *						mName;
	};
}

#endif // !__TERMINAL_WIN_H__
