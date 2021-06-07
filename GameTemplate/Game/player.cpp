#include "stdafx.h"
#include "player.h"

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

void Player::DeleteIndividual(const int p_num)
{
	//p_numはプレイヤーのコントローラー番号

	DeleteGO(m_modelRender[p_num]);

	//DeleteGO(m_skinModelRender[p_num]);
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

	return true;
}

bool Player::StartIndividual(const int p_num)
{
	//p_numはプレイヤーのコントローラー番号

	m_modelRender[p_num] = NewGO<ModelRender>(0);
	m_modelRender[p_num]->Init("Assets/modelData/unityChan.tkm");
	m_modelRender[p_num]->SetPosition({ 0.0f, 10.0f * p_num, 0.0f });

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
		if (m_activePlayer[i] == true) {
			DrawIndividual(i);
		}
	}
}

void Player::DrawIndividual(const int p_num)
{
	//p_numはプレイヤーのコントローラー番号

	//m_skinModelRender[p_num]->SetPosition(m_position[p_num]);
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

void Player::ControllerIndividual(const int p_num)
{
	//p_numはプレイヤーのコントローラー番号

	


}