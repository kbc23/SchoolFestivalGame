#include "stdafx.h"
#include "stage.h"

#include "player.h"

namespace //constant
{
	//////////////////////////////
	// ファイルパス
	//////////////////////////////

	const char* FILE_PATH_TKM_CHAEACTER_MODEL = "Assets/modelData/unityChan.tkm";

	//////////////////////////////
	// 位置情報
	//////////////////////////////

	const Vector3 PLAYER_START_POSITION[Player::PlayerNumberMax] = {	//プレイヤーの初期座標
		{ 390.0f, 0.0f, -250.0f },											//プレイヤー１
		{ 130.0f, 0.0f, -250.0f },											//プレイヤー２
		{ -130.0f, 0.0f, -250.0f },											//プレイヤー３
		{ -390.0f, 0.0f, -250.0f }											//プレイヤー４
	};
	
	const Vector2 GOAL_RANK_FONT_POSITION[Player::PlayerNumberMax] = {	//ゴール順位の座標
		{ -390.0f, 0.0f },													//プレイヤー１
		{ -130.0f, 0.0f },													//プレイヤー２
		{ 130.0f, 0.0f },													//プレイヤー３
		{ 390.0f, 0.0f }													//プレイヤー４
	};

	//モデルとフォントのX座標の向きが逆っぽい

	//////////////////////////////
	// 入力関係
	//////////////////////////////

	const int MOVE_BUTTON_A = 2;		//Aボタンを押したときの移動量
	const int MOVE_BUTTON_B = 1;		//Bボタンを押したときの移動量

	//////////////////////////////
	// タイマー関連
	//////////////////////////////

	const int TIMER_RESET = 0; //タイマーのリセット
	const int TIME_ANIMATION = 30; //ジャンプアニメーションの時間（0.5秒）
}



Player::Player()
{

}

////////////////////////////////////////////////////////////
// デストラクタ関連
////////////////////////////////////////////////////////////

Player::~Player()
{
	//プレイヤーごとに処理
	for (int i = 0; i < m_maxPlayer; i++) {
		DeleteIndividual(i);
	}
}



void Player::DeleteIndividual(const int pNum)
{
	//p_numはプレイヤーのコントローラー番号

	DeleteGO(m_modelRender[pNum]);

	//DeleteGO(m_skinModelRender[pNum]);
}

////////////////////////////////////////////////////////////
// Start関数関連
////////////////////////////////////////////////////////////

bool Player::Start()
{
	//プレイヤーごとに処理
	for (int i = 0; i < m_maxPlayer; i++) {
		bool check = StartIndividual(i);

		//StartIndividual関数がfalseを返したらfalseを返して処理を終了させる。
		if (check == false) {
			return false;
		}
	}


	//フォントの初期化処理
	for (int i = 0; i < m_maxPlayer; i++) {
		//m_fontRank[i] = NewGO<Font>(0);

	}

	m_stage = FindGO<Stage>("stage");



	return true;
}

bool Player::StartIndividual(const int pNum)
{
	//p_numはプレイヤーのコントローラー番号

	m_modelRender[pNum] = NewGO<ModelRender>(0);
	m_modelRender[pNum]->Init(FILE_PATH_TKM_CHAEACTER_MODEL);
	m_modelRender[pNum]->SetPosition(PLAYER_START_POSITION[pNum]);

	m_goalRankFont[pNum] = NewGO<FontRender>(0);
	m_goalRankFont[pNum]->Init(L"", GOAL_RANK_FONT_POSITION[pNum]);
	m_goalRankFont[pNum]->Deactivate();

	//Quaternion q = g_quatIdentity;
	//q.Quaternion::SetRotationDegY(0);
	//m_modelRender[pNum]->SetRotation(q);

	return true;
}

////////////////////////////////////////////////////////////
// 毎フレームの処理
////////////////////////////////////////////////////////////

void Player::Update()
{
	//プレイヤーごとに操作
	for (int i = 0; i < m_maxPlayer; i++) {
		if (m_flagGoal[i] == false) {
			Controller(i);
			Animation(i);
		}
		else {
			Animation(i);
		}
	}
}

//////////////////////////////
// プレイヤーの操作処理
//////////////////////////////

void Player::Controller(const int pNum)
{
	//p_numはプレイヤーのコントローラー番号

	if (m_flagDoingAnimation[pNum] == true || m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}

	//２マス進む
	if (g_pad[pNum]->IsTrigger(enButtonA) == true) {
		m_stage->MoveBlock(pNum, MOVE_BUTTON_A);
		m_flagDoingAnimation[pNum] = true;
	}
	//１マス進む
	else if (g_pad[pNum]->IsTrigger(enButtonB) == true) {
		m_stage->MoveBlock(pNum, MOVE_BUTTON_B);
		m_flagDoingAnimation[pNum] = true;
	}
}

void Player::Animation(const int pNum)
{
	if (m_flagDoingAnimation[pNum] == false) {
		return;
	}

	++m_timerAnimation[pNum];

	//ここのマジックナンバーを後で解消する。
	if (m_timerAnimation[pNum] >= 0 && m_timerAnimation[pNum] <= 15) {
		m_modelRender[pNum]->UpPositionY(1.0f);
	}
	else if (m_timerAnimation[pNum] >= 16 && m_timerAnimation[pNum] <= 30) {
		m_modelRender[pNum]->DownPositionY(1.0f);
	}


	if (m_timerAnimation[pNum] >= TIME_ANIMATION) {
		m_flagDoingAnimation[pNum] = false;
		m_timerAnimation[pNum] = TIMER_RESET;
	}
}