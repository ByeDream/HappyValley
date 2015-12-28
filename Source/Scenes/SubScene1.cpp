/************************************************************************/
/* Logic Sub Scene 1
/*
/* ByeDream
/* Dec-25-2015
/************************************************************************/
#include "stdafx.h"
#include "Framework/Utility/Utility.h"

#include "SubScene1.h"

#include "ScenesHeader.h"
#include "Framework/Modules/Director/Director.h"

void SubScene1::update(DECIMAL deltaTime)
{
	int i = 0;
}

void SubScene1::render(HV::Renderer *renderer)
{
	int i = 0;
	//mDirector->ShutDown();
}

HV::FACTORY::REGISTER_AUTO_WORKER(SubScene1, SID_SubScene0, HV::Director::sGetSceneFactory())

// EOF
