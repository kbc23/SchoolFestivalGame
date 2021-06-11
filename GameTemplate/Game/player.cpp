#include "stdafx.h"
#include "stage.h"

#include "player.h"

namespace {
	//////////////////////////////
	// ファイルパス
	//////////////////////////////

	const char* FILE_PATH_CHAEACTER_MODEL = "Assets/modelData/unityChan.tkm";

	//////////////////////////////
	// 位置情報
	//////////////////////////////

	const Vector3 PLAYER_START_POSITION[Player::PlayerNumberMax] = {	//プレイヤーの初期座標
		{ 390.0f, 0.0f, -250.0f },											//プレイヤー１
		{ 130.0f, 0.0f, -250.0f },											//プレイヤー２
		{ -130.0f, 0.0f, -250.0f },											//プレイヤー３
		{ -390.0f, 0.0f, -250.0f }											//プレイヤー４
	};

	//////////////////////////////
	// 入力関係
	//////////////////////////////

	const int MOVE_BUTTON_A = 2;		//Aボタンを押したときの移動量
	const int MOVE_BUTTON_B = 1;		//Bボタンを押したときの移動量
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
	m_modelRender[pNum]->Init(FILE_PATH_CHAEACTER_MODEL);
	m_modelRender[pNum]->SetPosition(PLAYER_START_POSITION[pNum]);

	m_goalRankFont[pNum] = NewGO<FontRender>(0);
	m_goalRankFont[pNum]->Init(L"", { PLAYER_START_POSITION[pNum].x,PLAYER_START_POSITION[pNum].y });
	m_goalRankFont[pNum]->Deactivate();

	//Quaternion q = g_quatIdentity;
	//q.Quaternion::SetRotationDegY(0);
	//m_modelRender[pNum]->SetRotation(q);

	return true;
}

////////////////////////////////////////////////////////////
// Update関数
////////////////////////////////////////////////////////////

void Player::Update()
{
	//プレイヤーの操作
	Controller();

	//プレイヤー関連の描画処理
	Draw();
}

////////////////////////////////////////////////////////////
// Draw関数関連
////////////////////////////////////////////////////////////

void Player::Draw()
{
	//プレイヤーごとに描画
	for (int i = 0; i < m_maxPlayer; i++) {
		DrawIndividual(i);
	}
}

void Player::DrawIndividual(const int pNum)
{

}

////////////////////////////////////////////////////////////
// プレイヤーの操作処理
////////////////////////////////////////////////////////////

void Player::Controller()
{
	//プレイヤーごとに操作
	for (int i = 0; i < m_maxPlayer; i++) {
		if (m_activePlayer[i] == true) {
			ControllerIndividual(i);
		}
	}
}

void Player::ControllerIndividual(const int pNum)
{
	//p_numはプレイヤーのコントローラー番号

	//２マス進む
	if (g_pad[pNum]->IsTrigger(enButtonA) == true) {
		m_stage->MoveBlock(pNum, MOVE_BUTTON_A);
	}
	//１マス進む
	else if (g_pad[pNum]->IsTrigger(enButtonB) == true) {
		m_stage->MoveBlock(pNum, MOVE_BUTTON_B);
	}
	


}