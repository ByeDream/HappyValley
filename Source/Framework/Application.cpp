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
#include "Framework/EventDriver/EventCenter.h"

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
	EventType subscriptionList[] =
	{
		EVENT_SHUTDOWN_APP,
	};
	SIZE_T numSubscriptions = SIZE_OF_ARRAY(subscriptionList);
	registerToEventCenter(subscriptionList, numSubscriptions);

	attachModulues();

	mTerminal->show();
	mTimer->reset();
	mDirector->switchToScene(0);  // Start with scene 0(main scene)
	mIsRunning = true;
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

	EventCenter::getInstance()->publishEvent();
}

void HV::Application::shutDown()
{
	unregisterFromEventCenter();

	EventCenter::destory();

	detachModulues();

	mIsRunning = false;
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

bool HV::Application::handleEvent(CPEvent e)
{
	bool swallowEvent = false;
	switch (e->type)
	{
	case EVENT_SHUTDOWN_APP:
		mIsRunning = false;
		swallowEvent = true;
		break;
	default:
		break;
	}
	return swallowEvent;
}

// EOF