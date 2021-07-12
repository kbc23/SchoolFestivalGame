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
	const int MIDLE = 1;       //中
	const int DOWN_END = 2;   //下端
}

Pause::Pause()
{

}

Pause::~Pause()
{
	DeleteGO(m_spriteChoices[0]);
	DeleteGO(m_spriteChoices[1]);
	DeleteGO(m_spriteChoices[2]);
}

bool Pause::Start()
{
	m_spriteChoices[0] = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteChoices[0]->Init(filePath::dds::PAUSE_RETURN_TO_GAME);
	m_spriteChoices[0]->SetPosition(CHOICES_POSITION[0]);
	m_spriteChoices[0]->Deactivate();

	m_spriteChoices[1] = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteChoices[1]->Init(filePath::dds::PAUSE_RETRY);
	m_spriteChoices[1]->SetPosition(CHOICES_POSITION[1]);
	m_spriteChoices[1]->Deactivate();

	m_spriteChoices[2] = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteChoices[2]->Init(filePath::dds::PAUSE_QUIT_THE_GAME);
	m_spriteChoices[2]->SetPosition(CHOICES_POSITION[2]);
	m_spriteChoices[2]->Deactivate();

	m_game = FindGO<MainProcessing>(igo::CLASS_NAME_GAME);

	return true;
}

void Pause::Init()
{
	m_flagProcessing = true;
}

void Pause::Finish()
{
	m_flagProcessing = false;

	m_spriteChoices[0]->Deactivate();
	m_spriteChoices[1]->Deactivate();
	m_spriteChoices[2]->Deactivate();
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
	m_flagMove = true;
	bool m_pause_title = true;
}

void Pause::PauseFinish()
{
	m_spriteChoices[0]->Deactivate();
	m_spriteChoices[1]->Deactivate();
	m_spriteChoices[2]->Deactivate();
}

void Pause::Update()
{
	if (m_flagProcessing == false) {
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
	if (m_flagProcessing == false)
	{
		return;
	}

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
			m_game->SetPause_Stage(true);
		}
		else if (m_cursorPosition == 2)
		{
			SetGamePaused(false);
			m_game->SetPause_Title(true);
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