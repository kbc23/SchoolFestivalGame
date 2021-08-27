#include "stdafx.h"
#include "rank.h"

#include "constant.h"
#include "sudden_death_mode.h"
#include "result.h"
#include "main_processing.h"



namespace
{
	const Vector2 PLAYER_RANK_SPRITE[con::PlayerNumberMax] = {	//プレイヤーの順位の初期座標
		{ -490.0f, -70.0f },										//プレイヤー１
		{ -180.0f, -70.0f },										//プレイヤー２
		{ 140.0f, -70.0f },											//プレイヤー３
		{ 460.0f, -70.0f }											//プレイヤー４
	};

	const int TIME_GAME_END = 120; //ゲーム終了時のタイム
	const int TIME_NEXT_ROUND = 120; //次のラウンドに移行するタイム
}



Rank::Rank()
{
	////////////////////////////////////////////////////////////
	// スプライトのNewGO
	////////////////////////////////////////////////////////////

	//プレイヤー順位のスプライトのNewGO
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum] = NewGO<SpriteRender>(igo::PRIORITY_UI); //NewGO
			m_spriteGoalRank[playerNum][rankNum]->Init(filePath::dds::RANK[rankNum]); //初期化
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //非表示
		}
	}

	//ラウンドのポイントのUIのNewGO
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
			m_spriteRoundWin[playerNum][roundNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
			m_spriteRoundWin[playerNum][roundNum]->Init(filePath::dds::ROUND_WIN[playerNum][roundNum]); //初期化
			m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //非表示
		}
	}

	//ゲーム終了時のスプライトのNewGO
	m_spriteGameEnd = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteGameEnd->Init(filePath::dds::GAME_END); //初期化
	m_spriteGameEnd->Deactivate(); //非表示
}

Rank::~Rank()
{
	//スプライトの削除
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			DeleteGO(m_spriteGoalRank[playerNum][rankNum]); //削除
		}
	}

	//ラウンドのUIの削除
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
			DeleteGO(m_spriteRoundWin[playerNum][roundNum]); //削除
		}
	}

	DeleteGO(m_spriteGameEnd); //削除
}

bool Rank::Start()
{
	m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);
	m_findResult = FindGO<Result>(igo::CLASS_NAME_RESULT);
	m_findMainProcessing = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);

	return true;
}

void Rank::Init()
{
    m_flagProcess = true;

	////////////////////////////////////////////////////////////
	// スプライトの初期化
	////////////////////////////////////////////////////////////

	//プレイヤーの順位のスプライトの初期化
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->SetPosition(PLAYER_RANK_SPRITE[playerNum]);
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //非表示
		}
	}

	//ラウンドのUIの初期化
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
			m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //非表示
		}
	}

	//ゲーム終了時のスプライトの初期化
	m_spriteGameEnd->Deactivate(); //非表示

	////////////////////////////////////////////////////////////
	// メンバ変数の初期化
	////////////////////////////////////////////////////////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_goalRanking[playerNum] = con::rank_notClear;
		m_roundPoint[playerNum] = 0; //プレイヤーのラウンドのポイント
	}

	m_flagGameEnd = false; //ゲームが終了したか
	m_flagNextRound = false; //次のラウンドに移行するか

	m_timerGameEnd = con::TIME_RESET_ZERO; //ゲーム終了時のタイマー
	m_timerNextRound = con::TIME_RESET_ZERO; //次のラウンドに移行するタイマー
}

void Rank::Finish()
{
    m_flagProcess = false;

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //非表示
		}
	}

	//ラウンドのUIの非表示
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
			m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //非表示
		}
	}

	m_spriteGameEnd->Deactivate(); //非表示
}

void Rank::Update()
{
    if (false == m_flagProcess) {
        return;
    }

	//サドンデスモードのとき
	if (true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
		//次のラウンドに移行するかの確認
		CheckNextRound();
	}
	//レースモードのとき
	else {
		//ゲームが終了したかの確認
		CheckGameEnd();
	}

	//取得したラウンドポイントの表示
	DrawRoundPoint();
}

void Rank::DrawRoundPoint()
{
	//サドンデスモードでないとき
	if (false == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
		return;
	}

	//取得ラウンド数のUIの表示
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
			if (roundNum < m_roundPoint[playerNum]) {
				m_spriteRoundWin[playerNum][roundNum]->Activate(); //表示
			}
		}
	}
}

void Rank::CheckGameEnd()
{
	//ゲームが終了したか
	if (false == m_flagGameEnd) {
		return;
	}

	m_spriteGameEnd->Activate(); //表示

	++m_timerGameEnd; //タイマーのカウント

	//タイマーを超える
	if (TIME_GAME_END <= m_timerGameEnd) {
		//ゲームの終了処理
		GameEnd();
	}
}

void Rank::CheckNextRound()
{
	//次のラウンドに移行するか
	if (false == m_flagNextRound) {
		return;
	}

	//ゲームが終了したか（誰かが、３ラウンドを先取する）
	if (true == m_flagGameEnd) {
		CheckGameEnd();
		return;
	}

	++m_timerNextRound; //タイマーのカウント

	//タイマーを超える
	if (TIME_NEXT_ROUND <= m_timerNextRound) {
		//次のラウンドに移行する
		m_findSuddenDeathMode->SetFlagNextRound(true);
	}
}

void Rank::GameEnd()
{
	//サドンデスモードのとき
	if (true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
		//サドンデスモード時の最終順位の決定
		SuddenDeathRank();

		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
			m_findResult->SetRank(playerNum, m_goalRanking[playerNum]); //リザルトシーンに順位を渡す
		}

		m_findMainProcessing->SetGameEnd(true); //ゲームシーンが終了したことを伝える
	}
	//レースモードのとき
	else {
		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
			m_findResult->SetRank(playerNum, m_goalRanking[playerNum]); //リザルトシーンに順位を渡す
		}

		m_findMainProcessing->SetGameEnd(true); //ゲームシーンが終了したことを伝える
	}
}

void Rank::SuddenDeathRank()
{
	int Ranking = con::rank_1; //順位
	bool checkAddRank = false; //次の取得ラウンド数のプレイヤーの順位が変わるか

	//取得ラウンド数が多い順に高い順位にする
	for (int roundPointNum = 3; roundPointNum >= con::FIRST_ELEMENT_ARRAY; roundPointNum--) {
		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
			//順位を確定
			if (m_roundPoint[playerNum] == roundPointNum) {
				m_goalRanking[playerNum] = Ranking;
				checkAddRank = true;
			}
		}

		//次のプレイヤーの順位のために順位を増加する
		if (checkAddRank == true) {
			++Ranking; //順位を変更
			checkAddRank = false;
		}
	}
}

////////////////////////////////////////////////////////////
// 次のラウンドへ移行
////////////////////////////////////////////////////////////

void Rank::NextRound()
{
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //非表示
		}
	}

	//終了スプライトを非表示
	m_spriteGameEnd->Deactivate();

	////////////////////////////////////////////////////////////
	// メンバ変数
	////////////////////////////////////////////////////////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_goalRanking[playerNum] = con::rank_notClear;
	}

	m_flagNextRound = false; //次のラウンドに移行するか

	m_timerNextRound = con::TIME_RESET_ZERO; //次のラウンドに移行するタイマー
}