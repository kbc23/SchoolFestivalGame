#include "stdafx.h"
#include "Result.h"
#include "game.h"

const Vector3 PLAYER_POSITION[con::PlayerNumberMax] = {	//プレイヤーの初期座標
	{ 1200.0f, -1300.0f, -240.0f },											//プレイヤー１
	{ 400.0f, -1300.0f, -240.0f },											//プレイヤー２
	{ -400.0f, -1300.0f, -240.0f },											//プレイヤー３
	{ -1200.0f, -1300.0f, -240.0f }											//プレイヤー４
};
const Vector3 PLAYER_RANK_SPRITE[con::PlayerNumberMax] = {	//プレイヤーの初期座標
	{ 1200.0f, -1700.0f, -240.0f },											//プレイヤー１
	{ 400.0f, -1700.0f, -240.0f },											//プレイヤー２
	{ -400.0f, -1700.0f, -240.0f },											//プレイヤー３
	{ -1200.0f, -1700.0f, -240.0f }											//プレイヤー４
};


Result::Result() {
	/*for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
		m_rank[playerNum]=m_game->GetRank(playerNum);
	}*/
}

Result::~Result() {
	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		DeleteIndividual(playerNum);
	}


}

void Result::DeleteIndividual(const int pNum)
{
	//p_numはプレイヤーのコントローラー番号

	DeleteGO(m_modelRender[pNum]);

	//DeleteGO(m_skinModelRender[pNum]);
}
bool Result::Start() {
	//アニメーションの設定
	m_animationPlayer[Animation_idle].Load(filePath::tka::IDLE);
	//ループ再生をtrueにする
	m_animationPlayer[Animation_idle].SetLoopFlag(false);

	//プレイヤーごとに処理
	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		bool check = StartIndividual(playerNum);

		//StartIndividual関数がfalseを返したらfalseを返して処理を終了させる。
		if (check == false) {
			return false;
		}
	}

	m_game = FindGO<Game>(igo::CLASS_NAME_GAME);
	return true;
}

void Result::Update() {

}

bool Result::StartIndividual(const int pNum) {
	m_modelRender[pNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
	m_modelRender[pNum]->Init(filePath::tkm::CHAEACTER_MODEL, modelUpAxis::enModelUpAxisZ, m_animationPlayer, Animation_Max);
	m_modelRender[pNum]->SetPosition(PLAYER_POSITION[pNum]);
	m_modelRender[pNum]->SetScale({ 0.2f,0.2f,0.2f });
	m_modelRender[pNum]->PlayAnimation(Animation_idle);

	return true;
}

void Result::Display() {

}


void Result::AnimationDisplay() {

}

void Result::SelectDisplay() {

}