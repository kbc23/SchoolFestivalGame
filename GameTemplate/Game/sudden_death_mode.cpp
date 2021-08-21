#include "stdafx.h"
#include "sudden_death_mode.h"

#include "stage.h"
#include "score.h"
#include "player.h"
#include "game_start_countdown.h"
#include "CPU_player_controller.h"

SuddenDeathMode::SuddenDeathMode()
{
}

SuddenDeathMode::~SuddenDeathMode()
{
}

/////////////////////////////////////////////////////////////////////////
// 黄色、青色オブジェクトに乗ってしまった時に操作不能にするフラグを立てる
/////////////////////////////////////////////////////////////////////////

bool SuddenDeathMode::Start()
{
	//////////////////////////////
	// FindGO
	//////////////////////////////

	m_findStage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
	m_findScore = FindGO<Score>(igo::CLASS_NAME_SCORE);
	m_findPlayer = FindGO<Player>(igo::CLASS_NAME_PLAYER);
	m_findGameStartCountdown = FindGO<GameStartCountdown>(igo::CLASS_NAME_GAME_START_COUNTDOWN);
	m_findCPUPlayerController = FindGO<CPUPlayerController>(igo::CLASS_NAME_CPU_PLAYER_CONTROLLER);

	return true;
}

void SuddenDeathMode::Init()
{
	m_flagProcess = true;

	//////////////////////////////
	// メンバ変数の初期化
	//////////////////////////////

	m_finishSuddenDeath = false;
}

void SuddenDeathMode::Finish()
{
	m_flagProcess = false;

	//////////////////////////////
	// メンバ変数のFinishの処理
	//////////////////////////////

	m_flagSuddenDeathMode = false; //サドンデスモードか
}

void SuddenDeathMode::Update()
{
	if (false == m_flagProcess) {
		return;
	}

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

void SuddenDeathMode::NextRound()
{
	//各クラスの状態を次のラウンドに移動するために変更する
	m_findStage->NextRound();
	m_findScore->NextRound();
	m_findPlayer->NextRound();
	m_findGameStartCountdown->NextRound();
	m_findCPUPlayerController->NextRound();
}