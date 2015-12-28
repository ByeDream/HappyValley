/************************************************************************/
/* Attach Modules Plug-in
/*
/* ByeDream
/* Dec-25-2015
/************************************************************************/
#ifndef __MODULE_PLUGIN_H__
#define __MODULE_PLUGIN_H__

#include "Framework/Modules/Director/Director.h"

#ifdef _WINDOWS
#include "Framework/Modules/Terminal/Platform/Windows/TerminalWin.h"
#include "Framework/Modules/Timer/Platform/Windows/TimerWin.h"
#include "Framework/Modules/Rendering/Platform/D3D9/RendererD3D9.h"
#include "Framework/Modules/Rendering/Platform/D3D11/RendererD3D11.h"
#else
// TODO
#endif

namespace HV
{
	class Application;

#	define DECLARE_CREATOR(module) \
	module *sCreate##module(Application *_app) { return new module(_app); }

#	define DECLARE_Destoryer(module) \
	void sDestroy##module(module *_m) { SAFE_DELETE(_m); }

#	define DECLARE_CREATOR_WITH_PLATFORM(module, platform) \
	module *sCreate##module(Application *_app) { \
 	return new module##platform(_app); }

	// _X : with initialize & finitialize functional
#	define DECLARE_CREATOR_X(module) \
	module *sCreateInit##module(Application *_app) { \
	module *_m = new module(_app); _m->initialize(); return _m; }

#	define DECLARE_Destoryer_X(module) \
	void sDestroyFinit##module(module *_m) { \
	if (_m) { _m->finitialize(); delete _m; } }

#	define DECLARE_CREATOR_WITH_PLATFORM_X(module, platform) \
	module *sCreateInit##module(Application *_app) { \
 	module *_m = new module##platform(_app); _m->initialize(); return _m; }

#	define DECLARE_FUNC(module) \
	DECLARE_CREATOR(module) \
	DECLARE_CREATOR_X(module) \
	DECLARE_Destoryer(module) \
	DECLARE_Destoryer_X(module)

#	define DECLARE_FUNC_WITH_PLATFORM(module, platform) \
	DECLARE_CREATOR_WITH_PLATFORM(module, platform) \
	DECLARE_CREATOR_WITH_PLATFORM_X(module, platform) \
	DECLARE_Destoryer(module) \
	DECLARE_Destoryer_X(module)

	///////////////////////////////////////////////////

	DECLARE_FUNC(Director)
#if defined _WINDOWS
	DECLARE_FUNC_WITH_PLATFORM(Terminal, Win)
#if defined _D3D11
	DECLARE_FUNC_WITH_PLATFORM(Renderer, D3D11)
#elif defined _D3D9
	DECLARE_FUNC_WITH_PLATFORM(Renderer, D3D9)
#else
	// TODO
#endif
	DECLARE_FUNC_WITH_PLATFORM(Timer, Win)
#else
	// TODO
#endif

}

#endif // !__MODULE_PLUGIN_H__