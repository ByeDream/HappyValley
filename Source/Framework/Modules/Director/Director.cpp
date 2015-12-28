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

}

void HV::Director::finitialize()
{
	switchToScene(-1); 
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

void HV::Director::ShutDown()
{
	mApp->setRunningFlag(false);
}

// EOF