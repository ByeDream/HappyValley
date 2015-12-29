/************************************************************************/
/* The General Interface Of Scene
/*
/* ByeDream
/* Dec-22-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "Director.h"
#include "Framework/Application.h"
#include "Framework/Scene.h"

using namespace HV;

HV::Director::Director(Application *app)
	: Module(app)
	, mCurrentActiveScene(NULL)
{

}

HV::Director::~Director()
{

}

void HV::Director::initialize()
{
	EventType subscriptionList[] =
	{
		EVENT_SWITCH_TO_SCENE,
	};
	SIZE_T numSubscriptions = SIZE_OF_ARRAY(subscriptionList);
	registerToEventCenter(subscriptionList, numSubscriptions);
}

void HV::Director::finitialize()
{
	switchToScene(-1); 
	unregisterFromEventCenter();
}

void HV::Director::update(DECIMAL deltaTime)
{
	if (mCurrentActiveScene)
	{
		mCurrentActiveScene->update(deltaTime);
	}
}

void HV::Director::render(Renderer *renderer)
{
	if (mCurrentActiveScene)
	{
		mCurrentActiveScene->render(renderer);
	}
}

FACTORY::Factory * HV::Director::sGetSceneFactory()
{
	static FACTORY::Factory sceneFactory;
	return &sceneFactory;
}

void HV::Director::switchToScene(INDEX_T id)
{

	if (mCurrentActiveScene)
	{
		mCurrentActiveScene->onExit();
		SAFE_DELETE(mCurrentActiveScene);
	}

	mCurrentActiveScene = dynamic_cast<Scene *>(sGetSceneFactory()->Produce(id));
	if (mCurrentActiveScene)
	{
		mCurrentActiveScene->bindDirector(this);
		mCurrentActiveScene->onEnter();
	}
}

bool HV::Director::handleEvent(CPEvent e)
{
	bool swallowEvent = false;
	switch (e->type)
	{
	case EVENT_SWITCH_TO_SCENE:
		{
			UINT32 targetID = e->arg.ui32a[0];
			switchToScene(targetID);
			swallowEvent = true;
		}
		break;
	default:
		break;
	}
	return swallowEvent;
}

// EOF