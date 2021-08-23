#include "stdafx.h"
#include "result.h"
#include "main_processing.h"



namespace
{
	const Vector3 PLAYER_POSITION[con::PlayerNumberMax] = {	//プレイヤーの初期座標
		{ 1200.0f, -1000.0f, -240.0f },											//プレイヤー１
		{ 400.0f, -1000.0f, -240.0f },											//プレイヤー２
		{ -400.0f, -1000.0f, -240.0f },											//プレイヤー３
		{ -1200.0f, -1000.0f, -240.0f }											//プレイヤー４
	};
	const Vector2 PLAYER_RANK_SPRITE[con::PlayerNumberMax] = {	//プレイヤーの初期座標
		{ -420.0f, -170.0f},											//プレイヤー１
		{ -140.0f, -170.0f },											//プレイヤー２
		{ 140.0f, -170.0f },											//プレイヤー３
		{ 420.0f,-170.0f }											//プレイヤー４
	};

	const Vector2 MODE_SELECT_SPRITE[con::PlayerNumberMax] = {	//プレイヤーの初期座標
		{ 0.0f, 200.0f },											//プレイヤー１
		{ 0.0f, 100.0f },											//プレイヤー２
		{ 0.0f, 0.0f },											//プレイヤー３
		{ 0.0f, -100.0f }											//プレイヤー４
	};

	const int UP_END = 0;     //一番上
	const int DOWN_END = 3;    //一番下
}



Result::Result()
{
	//////////////////////////////
	// NewGO
	//////////////////////////////

	//////////
	// 背景のNewGO
	//////////

	m_spriteBackground = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
	m_spriteBackground->Init(filePath::dds::BACKGROUND);
	m_spriteBackground->Deactivate();

	//////////
	// モデルのNewGO
	//////////

	//アニメーションの設定
	m_animationPlayer[win].Load(filePath::tka::WIN);
	m_animationPlayer[stand].Load(filePath::tka::STAND);
	m_animationPlayer[lose].Load(filePath::tka::LOSE);
	//ループ再生
	m_animationPlayer[stand].SetLoopFlag(true);
	m_animationPlayer[win].SetLoopFlag(false);
	m_animationPlayer[lose].SetLoopFlag(false);

	//プレイヤーモデルのNewGO
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
		m_modelCharacter[playerNum]->Init(
			filePath::tkm::CHAEACTER_MODEL, modelUpAxis::enModelUpAxisZ,
			m_animationPlayer, Animation_Max);
		m_modelCharacter[playerNum]->Deactivate();
		m_modelCharacter[playerNum]->SetRotationX(0.5f);

		m_modelCharacter[playerNum]->SetPosition(PLAYER_POSITION[playerNum]);
		m_modelCharacter[playerNum]->SetScale({ 0.2f,0.2f,0.2f });
	}

	//////////
	// スプライトのNewGO
	//////////

	//選択肢のNewGO
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
		m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI_2);
		m_spriteChoices[spriteNum]->Init(filePath::dds::RESULT_COMMAND[spriteNum]);
		m_spriteChoices[spriteNum]->Deactivate();
	}

	//PressANextのNewGO
	m_spritePressANext = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spritePressANext->Init(filePath::dds::PRESS_A_NEXT);
	m_spritePressANext->Deactivate();

	//////////
	// SEのNewGO
	//////////

	m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seDecision->Init(filePath::se::DECISION);
	m_seMoveCursor = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seMoveCursor->Init(filePath::se::MOVE_CURSOR);
}

Result::~Result()
{
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		DeleteGO(m_modelCharacter[playerNum]);
		DeleteGO(m_spriteGoalRank[playerNum]);
		DeleteGO(m_spriteChoices[playerNum]);
	}
	DeleteGO(m_spritePressANext);
	DeleteGO(m_spriteBackground);

	DeleteGO(m_seDecision);
	DeleteGO(m_seMoveCursor);
}

bool Result::Start()
{
	//////////////////////////////
	// FindGO
	//////////////////////////////

	m_game = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);
	return true;
}

void Result::Init()
{
	m_flagProcess = true;

	//////////
	// 背景の初期化
	//////////

	m_spriteBackground->Deactivate();

	//////////
	// モデルの初期化
	//////////

	//プレイヤーモデルの初期化
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {	
		m_modelCharacter[playerNum]->Activate();
		m_modelCharacter[playerNum]->SetRotationX(0.5f);
		m_modelCharacter[playerNum]->SetPosition(PLAYER_POSITION[playerNum]);
		m_modelCharacter[playerNum]->SetScale({ 0.2f,0.2f,0.2f });
	}

	//////////
	// スプライトのNewGO、初期化
	//////////

	//プレイヤーごとの順位に応じてNewGO、初期化
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		switch (m_rank[playerNum]) {
		case con::rank_1:
			m_spriteGoalRank[playerNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
			m_spriteGoalRank[playerNum]->Init(filePath::dds::RANK[con::rank_1]);
			m_spriteGoalRank[playerNum]->SetPosition(PLAYER_RANK_SPRITE[playerNum]);
			m_modelCharacter[playerNum]->PlayAnimation(win);
			break;
		case con::rank_2:
			m_spriteGoalRank[playerNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
			m_spriteGoalRank[playerNum]->Init(filePath::dds::RANK[con::rank_2]);
			m_spriteGoalRank[playerNum]->SetPosition(PLAYER_RANK_SPRITE[playerNum]);
			m_modelCharacter[playerNum]->PlayAnimation(stand);
			break;
		case con::rank_3:
			m_spriteGoalRank[playerNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
			m_spriteGoalRank[playerNum]->Init(filePath::dds::RANK[con::rank_3]);
			m_spriteGoalRank[playerNum]->SetPosition(PLAYER_RANK_SPRITE[playerNum]);
			m_modelCharacter[playerNum]->PlayAnimation(stand);
			break;
		case con::rank_4:
			m_spriteGoalRank[playerNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
			m_spriteGoalRank[playerNum]->Init(filePath::dds::RANK[con::rank_4]);
			m_spriteGoalRank[playerNum]->SetPosition(PLAYER_RANK_SPRITE[playerNum]);
			m_modelCharacter[playerNum]->PlayAnimation(lose);
			break;
		}
	}

	//選択肢の初期化
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
		//m_spriteChoices[spriteNum]->Activate();
		m_spriteChoices[spriteNum]->SetPosition(MODE_SELECT_SPRITE[spriteNum]);

		if (con::FIRST_ELEMENT_ARRAY == spriteNum) {
			m_spriteChoices[spriteNum]->SetMulColor(SRns::COLOR_NORMAL);
		}
		else {
			m_spriteChoices[spriteNum]->SetMulColor(SRns::COLOR_GRAY);
		}
	}

	//PressANextの初期化
	m_spritePressANext->Activate();

	//////////
	// メンバ変数の初期化
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_rank[playerNum] = con::GoalRankMax;
	}

	m_flagDecision = false;    //決定したかのフラグ
	m_cursorPosition = 0; //カーソルの場所
	m_flagFinish = false;      //このクラスでするべき処理が終わったか
	m_flagBlinking = true; //m_pressAButtonの透過処理に使用

	m_displayStatus = DisplayStatus::result;
}

void Result::Finish()
{
	m_flagProcess = false;

	//////////
	// 背景の非表示
	//////////

	m_spriteBackground->Deactivate();

	//////////
	// モデルの非表示
	//////////

	//プレイヤーモデルの非表示
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->Deactivate();
	}

	//////////
	// スプライトのDeleteGO、非表示
	//////////

	//プレイヤーごとの順位に応じてDeleteGO
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		DeleteGO(m_spriteGoalRank[playerNum]);
	}

	//選択肢の初期化
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
		m_spriteChoices[spriteNum]->Deactivate();
	}

	//PressANextの初期化
	m_spritePressANext->Deactivate();
}

void Result::Update()
{
	if (false == m_flagProcess) {
		return;
	}

	switch (m_displayStatus) {
	case DisplayStatus::result:
		ResultDisplay();
		break;
	case DisplayStatus::commandSelect:
		SelectDisplay();
		break;
	case DisplayStatus::finish:
		if (false == m_flagFinish) {
			FinishResult();
		}
		break;
	}
}

void Result::ResultDisplay()
{
	if (g_pad[con::player_1]->IsTrigger(enButtonA) == true) {
		m_seDecision->Play(false); //再生
		m_spritePressANext->Deactivate();

		for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
			m_spriteChoices[spriteNum]->Activate();
		}

		m_displayStatus = DisplayStatus::commandSelect;
	}
}

void Result::SelectDisplay()
{
	ResultSelect();
}

void Result::ResultSelect() {
	//決定
	if (true == g_pad[con::player_1]->IsTrigger(enButtonA)) {
		m_seDecision->Play(false); //再生
		m_game->SetResultSelect(m_cursorPosition);

		m_displayStatus = DisplayStatus::finish;
	}
	//上に移動
	else if (true == g_pad[con::player_1]->IsTrigger(enButtonUp)) {
		m_seMoveCursor->Play(false);

		if (m_cursorPosition == UP_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);

		--m_cursorPosition;

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
	}
	//下に移動
	else if (true == g_pad[con::player_1]->IsTrigger(enButtonDown)) {
		m_seMoveCursor->Play(false);

		if (m_cursorPosition == DOWN_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);

		++m_cursorPosition;

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
	}

}

void Result::FinishResult()
{
	m_flagFinish = true;
}