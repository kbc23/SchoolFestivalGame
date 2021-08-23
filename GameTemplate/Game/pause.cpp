#include "stdafx.h"
#include "pause.h"
#include "main_processing.h"

namespace //constant
{
	////////////////////////////////////////////////////////////
	// 位置情報
	////////////////////////////////////////////////////////////

	const Vector2 CHOICES_POSITION[Pause::Choices::ChoicesMax] =
	{
		{0.0f,150.0f},
		{0.0f,0.0f},
		{0.0f,-150.0f}
	};

	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////

	const int UP_END = 0;      //上端
	const int DOWN_END = 2;   //下端
}

Pause::Pause()
{
	//////////////////////////////
	// NewGO
	//////////////////////////////

	//////////
	// スプライトのNewGO
	//////////

	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteChoices[spriteNum]->Init(filePath::dds::PAUSE_UI[spriteNum]); //初期化
		m_spriteChoices[spriteNum]->Deactivate(); //非表示
	}
}

Pause::~Pause()
{
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		DeleteGO(m_spriteChoices[spriteNum]);
	}
}

bool Pause::Start()
{
	//////////////////////////////
	// FindGO
	//////////////////////////////

	m_mainProcessing = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);

	return true;
}

void Pause::Init()
{
	m_flagProcess = true;

	//////////
	// スプライトの初期化
	//////////

	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		m_spriteChoices[spriteNum]->SetPosition(CHOICES_POSITION[spriteNum]); //位置
		m_spriteChoices[spriteNum]->Deactivate(); //非表示
	}

	//////////
	// メンバ変数の初期化
	//////////

	m_cursorPosition = UP_END; //カーソルの場所
	m_flagDecision = false; //人数を決定したかのフラグ
	m_pausePlayer = con::player_1; //ポーズ状態にしたプレイヤー
}

void Pause::Finish()
{
	m_flagProcess = false;

	//////////
	// スプライトの非表示
	//////////

	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		m_spriteChoices[spriteNum]->Deactivate(); //非表示
	}
}

void Pause::PauseInit()
{
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		m_spriteChoices[spriteNum]->Activate(); //表示

		//最初の選択肢
		if (con::FIRST_ELEMENT_ARRAY == spriteNum) {
			m_spriteChoices[spriteNum]->SetMulColor(SRns::COLOR_NORMAL); //カラー
		}
		else {
			m_spriteChoices[spriteNum]->SetMulColor(SRns::COLOR_GRAY); //カラー
		}
	}

	m_cursorPosition = UP_END; //カーソルの場所
	m_flagDecision = false; //人数を決定したかのフラグ
}

void Pause::PauseFinish()
{
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		m_spriteChoices[spriteNum]->Deactivate(); //非表示
	}
}

void Pause::Update()
{
	if (false == m_flagProcess) {
		return;
	}

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		if (true == g_pad[playerNum]->IsTrigger(enButtonStart)) {
			//ポーズ状態にする
			SetGamePaused(true);
			//ポーズ画面のために初期化
			PauseInit();

			m_pausePlayer = playerNum; //ポーズしたプレイヤーをセット
		}
	}
}

void Pause::UpdateOnlyPaused()
{
	//決定
	if (g_pad[m_pausePlayer]->IsTrigger(enButtonA) == true) {
		SetGamePaused(false); //ポーズ状態を終わらせる

		//ゲームに戻る
		if (m_cursorPosition == returnGame) {
			PauseFinish(); //ポーズ画面を終わったときの後始末
		}
		//リトライ
		else if (m_cursorPosition == retry) {
			m_mainProcessing->PauseRetry();
		}
		//ゲームをやめる
		else if (m_cursorPosition == finish) {
			m_mainProcessing->PauseTitle();
		}

		m_flagDecision = true;
	}
	//上に移動
	else if (g_pad[m_pausePlayer]->IsTrigger(enButtonUp) == true) {
		//カーソルが上端
		if (m_cursorPosition == UP_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY); //カラー

		--m_cursorPosition; //カーソルを移動

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL); //カラー
	}
	//下に移動
	else if (g_pad[m_pausePlayer]->IsTrigger(enButtonDown) == true) {
		//カーソルが下端
		if (m_cursorPosition == DOWN_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY); //カラー

		++m_cursorPosition; //カーソルを移動

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL); //カラー
	}
	//スタートボタン
	if (true == g_pad[m_pausePlayer]->IsTrigger(enButtonStart)) {
		SetGamePaused(false); //ポーズ状態を終わらせる
		PauseFinish(); //ポーズ画面を終わったときの後始末
	}
}