#include "stdafx.h"
#include "goal.h"

#include "constant.h"

Goal::Goal()
{
	m_spriteGameEnd = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteGameEnd->Init(filePath::dds::GAME_END);
	m_spriteGameEnd->Deactivate();
}

Goal::~Goal()
{
	DeleteGO(m_spriteGameEnd);
}

bool Goal::Start()
{


    return true;
}

void Goal::Init()
{
    m_flagProcess = true;

	m_spriteGameEnd->Deactivate();

}

void Goal::Finish()
{
    m_flagProcess = false;

	m_spriteGameEnd->Deactivate();

}

void Goal::Update()
{
    if (false == m_flagProcess) {
        return;
    }

	GameEnd();

	//if (con::PlayerNumberMax == m_goalPlayer || m_finishSuddenDeath == true) {
	//	m_spriteGameEnd->Activate();
	//	m_endTimer++;
	//	if (m_endTimer > 180) {
	//		//サドンデスモードのとき所持ラウンド勝利数に応じて順位を確定
	//		if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
	//			SuddenDeathRank();
	//		}

	//		for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
	//			m_findMainProcessing->SetRank(playerNum, m_goalRanking[playerNum]);
	//		}

	//		m_gameEnd = true;
	//		m_findMainProcessing->SetGameEnd(m_gameEnd);
	//	}
	//}
}

void Goal::GameEnd()
{
	if (false == m_flagGameEnd) {
		return;
	}

	m_spriteGameEnd->Activate();

}