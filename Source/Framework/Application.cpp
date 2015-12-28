/************************************************************************/
/* The General Interface Of Application
/*
/* ByeDream
/* Dec-20-2015
/************************************************************************/
#include "stdafx.h"
#include "Utility/Utility.h"

#include "Application.h"

#include "Framework/Modules/ModulePlugin.h"

HV::Application::Application()
	: mIsRunning(false)
	, mTerminal(NULL)
	, mRenderer(NULL)
	, mDirector(NULL)
	, mTimer(NULL)
{

}

HV::Application::~Application()
{

}

void HV::Application::boot()
{
	setRunningFlag(true);
	attachModulues();

	mTerminal->show();
	mTimer->reset();
	mDirector->switchToScene(0);  // Start with scene 0(main scene)
}

void HV::Application::doFrame()
{
	DECIMAL deltaTime = 0;
	if (mTimer)
	{
		mTimer->tick();
		// TODO lock FPS
		deltaTime = mTimer->getSuspendableFrameDeltaTimeInSecond();
	}

	if (mDirector)
	{
		mDirector->update(deltaTime);
	}

	if (mRenderer)
	{
		mRenderer->clear();
		mRenderer->beginScene();

		if (mDirector)
		{
			mDirector->render(mRenderer);
		}

		mRenderer->endScene();
		mRenderer->present();
	}
}

void HV::Application::shutDown()
{
	detachModulues();
	setRunningFlag(false);
}

void HV::Application::attachModulues()
{
	mTerminal = sCreateInitTerminal(this);
	mRenderer = sCreateInitRenderer(this);
	mTimer = sCreateInitTimer(this);
	mDirector = sCreateInitDirector(this);
}

void HV::Application::detachModulues()
{
	sDestroyFinitDirector(mDirector);
	sDestroyFinitTimer(mTimer);
	sDestroyFinitRenderer(mRenderer);
	sDestroyFinitTerminal(mTerminal);
}

// EOF