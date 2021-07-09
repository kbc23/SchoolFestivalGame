#include "stdafx.h"
#include "title.h"

#include "constant.h"

Title::Title()
{
	m_spriteTitle = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteTitle->Init(filePath::dds::TITLE);
	m_spriteTitle->Deactivate();

	m_spritePressAButton = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spritePressAButton->Init(filePath::dds::PRESS_A_BUTTON);
	m_spritePressAButton->SetPosition({ 0.0f,-200.0f });
	m_spritePressAButton->Deactivate();

	m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seDecision->Init(filePath::se::DECISION);
}

Title::~Title()
{
	DeleteGO(m_spriteTitle);
	DeleteGO(m_spritePressAButton);
}

bool Title::Start()
{


	return true;
}

void Title::Init()
{
	m_flagProcessing = true;

	m_spriteTitle->Activate();
	m_spritePressAButton->SetMulColorW(0.0f);
	m_spritePressAButton->Activate();

	m_flagFinish = false; //このクラスでするべき処理が終わったか
	m_flagBlinking = true; //m_pressAButtonの透過処理に使用

}

void Title::Finish()
{
	m_flagProcessing = false;

	m_spriteTitle->Deactivate();
	m_spritePressAButton->Deactivate();
}

void Title::Update()
{
	if (m_flagProcessing == false) {
		return;
	}

	if (g_pad[con::player_1]->IsTrigger(enButtonA) == true) {
		m_seDecision->Play(false);
		m_flagFinish = true;
	}


	//Press_A_Buttonの点滅処理
	if (m_flagBlinking == true) {
		m_spritePressAButton->SetMulColorW(m_spritePressAButton->GetMulColorW() - 0.02f);

		if (m_spritePressAButton->GetMulColorW() <= 0.0f) {
			m_flagBlinking = false;
		}
	}
	else {
		m_spritePressAButton->SetMulColorW(m_spritePressAButton->GetMulColorW() + 0.02f);

		if (m_spritePressAButton->GetMulColorW() >= 0.8f) {
			m_flagBlinking = true;
		}
	}

}