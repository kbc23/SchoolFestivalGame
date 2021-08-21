#include "stdafx.h"
#include "pause.h"
#include "main_processing.h"

namespace //constant
{
	////////////////////////////////////////////////////////////
	// 位置情報
	////////////////////////////////////////////////////////////

	const Vector2 CHOICES_POSITION[3] = 
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

	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
		m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteChoices[spriteNum]->Init(filePath::dds::PAUSE_UI[spriteNum]);
		m_spriteChoices[spriteNum]->Deactivate();
	}
}

Pause::~Pause()
{
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
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

	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
		m_spriteChoices[spriteNum]->SetPosition(CHOICES_POSITION[spriteNum]);
		m_spriteChoices[spriteNum]->Deactivate();
	}

	//////////
	// メンバ変数の初期化
	//////////

	m_cursorPosition = 0;       //カーソルの場所
	m_numberOfPlayer = 0;       //プレイヤーの人数
	m_flagDecision = false;    //人数を決定したかのフラグ
	m_flagFinish = false;      //このクラスでするべき処理が終わったか

}

void Pause::Finish()
{
	m_flagProcess = false;

	//////////
	// スプライトの非表示
	//////////

	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
		m_spriteChoices[spriteNum]->Deactivate();
	}
}

void Pause::PauseInit()
{
	m_spriteChoices[0]->SetPosition(CHOICES_POSITION[0]);
	m_spriteChoices[0]->SetMulColor(SRns::COLOR_NORMAL);
	m_spriteChoices[0]->Activate();

	m_spriteChoices[1]->SetPosition(CHOICES_POSITION[1]);
	m_spriteChoices[1]->SetMulColor(SRns::COLOR_GRAY);
	m_spriteChoices[1]->Activate();

	m_spriteChoices[2]->SetPosition(CHOICES_POSITION[2]);
	m_spriteChoices[2]->SetMulColor(SRns::COLOR_GRAY);
	m_spriteChoices[2]->Activate();

	m_cursorPosition = 0;       //カーソルの場所
	m_numberOfPlayer = 0;       //プレイヤーの人数
	m_flagDecision = false;    //人数を決定したかのフラグ
	m_flagFinish = false;      //このクラスでするべき処理が終わったか
}

void Pause::PauseFinish()
{
	m_spriteChoices[0]->Deactivate();
	m_spriteChoices[1]->Deactivate();
	m_spriteChoices[2]->Deactivate();
}

void Pause::Update()
{
	if (false == m_flagProcess) {
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		SetGamePaused(true);
		PauseInit();
	}
}

void Pause::AlwaysUpdate()
{
	
}

void Pause::UpdateOnlyPaused()
{
	//決定
	if (g_pad[con::player_1]->IsTrigger(enButtonA) == true)
	{

		if (m_cursorPosition == 0)
		{
			SetGamePaused(false);
			PauseFinish();
		}
		else if (m_cursorPosition == 1)
		{
			SetGamePaused(false);
			m_mainProcessing->SetPause_Stage(true);
		}
		else if (m_cursorPosition == 2)
		{
			SetGamePaused(false);
			m_mainProcessing->SetPause_Title(true);
		}

		m_flagDecision = true;
	}
	//上に移動
	else if (g_pad[con::player_1]->IsTrigger(enButtonUp) == true) {
		if (m_cursorPosition == UP_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);

		--m_cursorPosition;

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
	}
	//下に移動
	else if (g_pad[con::player_1]->IsTrigger(enButtonDown) == true)
	{
		if (m_cursorPosition == DOWN_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);

		++m_cursorPosition;

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
	}
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		SetGamePaused(false);
		PauseFinish();
	}
}

void Pause::FinishPause()
{
	m_flagFinish = true;
}