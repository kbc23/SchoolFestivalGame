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
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		SetGamePaused(true);
	}
}

void Pause::AlwaysUpdate()
{

}

void Pause::UpdateOnlyPaused()
{
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		SetGamePaused(false);
	}
}