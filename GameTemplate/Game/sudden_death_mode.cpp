#include "stdafx.h"
#include "sudden_death_mode.h"

#include "stage.h"
#include "score.h"
#include "player.h"
#include "game_start_countdown.h"
#include "CPU_player_controller.h"



namespace
{
	const int NEXT_ROUND_TIME = 120;
}



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

	//次のラウンドへの移行処理
	NextRound();
}

void SuddenDeathMode::NextRound()
{
	//次のラウンドへ移行するか
	if (false == m_flagNextRound) {
		return;
	}

	++m_timerNextRound;

	//タイマーが超えたか
	if (NEXT_ROUND_TIME > m_timerNextRound) {
		return;
	}

	//各クラスの状態を次のラウンドに移動するために変更する
	m_findStage->NextRound();
	m_findScore->NextRound();
	m_findPlayer->NextRound();
	m_findGameStartCountdown->NextRound();
	m_findCPUPlayerController->NextRound();

	m_flagNextRound = false;
	m_timerNextRound = con::TIME_RESET_ZERO;
}