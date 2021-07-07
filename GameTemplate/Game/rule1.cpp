// 一度失敗したら死亡ルール

#include "stdafx.h"

#include "rule1.h"
#include "stage.h"
#include "player.h"

Rule1::Rule1()
{
}

Rule1::~Rule1()
{
}

/////////////////////////////////////////////////////////////////////////
// 黄色、青色オブジェクトに乗ってしまった時に操作不能にするフラグを立てる
/////////////////////////////////////////////////////////////////////////

bool Rule1::Start()
{
	m_stage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
	m_player = FindGO<Player>(igo::CLASS_NAME_PLAYER);

	m_stage->stop = true;
	m_stage->rule1NewGO = true;
	m_player->rule1NewGO = true;

	return true;
}

void Rule1::Update()
{
   
}