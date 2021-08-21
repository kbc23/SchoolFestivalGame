#include "stdafx.h"
#include "rank.h"



namespace
{
	const Vector2 PLAYER_RANK_SPRITE[con::PlayerNumberMax] = {	//プレイヤーの順位の初期座標
		{ -490.0f, -70.0f },										//プレイヤー１
		{ -180.0f, -70.0f },										//プレイヤー２
		{ 140.0f, -70.0f },											//プレイヤー３
		{ 460.0f, -70.0f }											//プレイヤー４
	};
}



Rank::Rank()
{
	//////////
	// スプライトのNewGO
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum] = NewGO<SpriteRender>(igo::PRIORITY_UI); //NewGO
			m_spriteGoalRank[playerNum][rankNum]->Init(filePath::dds::RANK[rankNum]); //初期化
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //非表示
		}
	}
}

Rank::~Rank()
{
	//スプライトの非表示
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			DeleteGO(m_spriteGoalRank[playerNum][rankNum]);
		}
	}
}

bool Rank::Start()
{


	return true;
}

void Rank::Init()
{
    m_flagProcess = true;

	//////////
	// スプライトの初期化
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->SetPosition(PLAYER_RANK_SPRITE[playerNum]);
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}
}

void Rank::Finish()
{
    m_flagProcess = false;

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}
}

void Rank::Update()
{
    if (false == m_flagProcess) {
        return;
    }
}

void Rank::NextRound()
{
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}
}