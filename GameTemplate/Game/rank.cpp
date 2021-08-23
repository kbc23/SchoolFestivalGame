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

	//ゲーム終了時のスプライトのNewGO
	m_spriteGameEnd = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteGameEnd->Init(filePath::dds::GAME_END); //初期化
	m_spriteGameEnd->Deactivate(); //非表示
}

Rank::~Rank()
{
	//スプライトの非表示
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			DeleteGO(m_spriteGoalRank[playerNum][rankNum]);
		}
	}

	DeleteGO(m_spriteGameEnd);
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

	//ゲーム終了時のスプライトの初期化
	m_spriteGameEnd->Deactivate(); //非表示

	////////////////////////////////////////////////////////////
	// メンバ変数の初期化
	////////////////////////////////////////////////////////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_goalRanking[playerNum] = con::rank_notClear;
	}
}

void Rank::Finish()
{
    m_flagProcess = false;

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //非表示
		}
	}

	m_spriteGameEnd->Deactivate(); //非表示
}

void Rank::Update()
{
    if (false == m_flagProcess) {
        return;
    }

	//ゲームが終了したかの確認
	CheckGameEnd();

	m_spriteGameEnd->Activate();
}

void Rank::CheckGameEnd()
{
	//ゲームが終了したか
	if (false == m_flagGameEnd) {
		return;
	}

	//サドンデスモードのとき
	if (true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
		m_findSuddenDeathMode->SetFlagNextRound(true); //次のラウンドに移行するようにする
	}
	//レースモードのとき
	else {
		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
			m_findResult->SetRank(playerNum, m_goalRanking[playerNum]); //リザルトシーンに順位を渡す
			m_findMainProcessing->SetGameEnd(true); //ゲームシーンが終了したことを伝える
		}
	}
}

void Rank::GameEnd()
{

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
}