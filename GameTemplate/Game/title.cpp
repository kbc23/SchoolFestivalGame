#include "stdafx.h"
#include "title.h"

#include "constant.h"

Title::Title()
{

}

Title::~Title()
{
	DeleteGO(m_spriteTitle);
	DeleteGO(m_spritePressAButton);
}

bool Title::Start()
{
	m_spriteTitle = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteTitle->Init(filePath::dds::TITLE);
	m_spritePressAButton = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spritePressAButton->Init(filePath::dds::PRESS_A_BUTTON);
	m_spritePressAButton->SetPosition({ 0.0f,-200.0f});
	m_spritePressAButton->SetMulColorW(0.0f);

	m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seDecision->Init(filePath::se::DECISION);

	return true;
}

void Title::Update()
{
	if (g_pad[con::player_1]->IsTrigger(enButtonA) == true) {
		m_seDecision->Play(false);
		m_flagFinish = true;
	}


	//Press_A_Button�̓_�ŏ���
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