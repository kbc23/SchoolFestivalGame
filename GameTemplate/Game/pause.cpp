#include "stdafx.h"
#include "pause.h"

Pause::Pause()
{

}

Pause::~Pause()
{

}

bool Pause::Start()
{
	return true;
}

void Pause::Update()
{
	if (pauseFlag == false) {
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			pauseFlag = true;

		}
	}

	if (pauseFlag == true)
	{
		if (g_pad[0]->IsTrigger(enButtonY))
		{
			pauseFlag = false;
		}
	}
}

void Pause::AlwaysUpdate()
{

}

void Pause::UpdateOnlyPaused()
{

}