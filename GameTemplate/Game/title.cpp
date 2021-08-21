#include "stdafx.h"
#include "title.h"

#include "constant.h"



namespace //constant
{
	////////////////////////////////////////////////////////////
	// ������
	////////////////////////////////////////////////////////////

	const Vector2 INIT_SPRITE_PRESS_A_BUTTON_POSITION = { 0.0f,-200.0f };
	const float INIT_SPRITE_PRESS_A_BUTTON_MUL_COLOR_W = 0.0f;


	////////////////////////////////////////////////////////////
	// Press_A_Button�̓_�ŏ����Ɏg�p����萔
	////////////////////////////////////////////////////////////

	const float CHANGE_MUL_COLOR_W = 0.02f; //�����x�̕ω���

	const float MINIMUM_MUL_COLOR_W = 0.0f; //�����x�̍ŏ��l
	const float MAXIMUM_MUL_COLOR_W = 0.8f; //�����x�̍ő�l
}



Title::Title()
{
	//////////////////////////////
	// NewGO
	//////////////////////////////

	//////////
	// �^�C�g���A�C�R����NewGO
	//////////

	m_spriteTitle = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteTitle->Init(filePath::dds::TITLE);
	m_spriteTitle->Deactivate();

	//////////
	// Press_A_Button��NewGO
	//////////

	m_spritePressAButton = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spritePressAButton->Init(filePath::dds::PRESS_A_BUTTON);
	m_spritePressAButton->Deactivate();

	//////////
	// SE��NewGO
	//////////

	m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seDecision->Init(filePath::se::DECISION);
}

Title::~Title()
{
	//////////
	// �^�C�g���A�C�R����DeleteGO
	//////////

	DeleteGO(m_spriteTitle);

	//////////
	// Press_A_Button��DeleteGO
	//////////

	DeleteGO(m_spritePressAButton);

	//////////
	// SE��DeleteGO
	//////////

	DeleteGO(m_seDecision);
}

bool Title::Start()
{
	

	return true;
}

void Title::Init()
{
	m_flagProcess = true;

	//////////
	// �^�C�g���A�C�R���̏�����
	//////////

	m_spriteTitle->Activate();

	//////////
	// Press_A_Button�̏�����
	//////////

	m_spritePressAButton->Activate();
	m_spritePressAButton->SetPosition(INIT_SPRITE_PRESS_A_BUTTON_POSITION);
	m_spritePressAButton->SetMulColorW(INIT_SPRITE_PRESS_A_BUTTON_MUL_COLOR_W);

	//////////
	// �����o�ϐ��̏�����
	//////////

	m_flagFinish = false; //���̃N���X�ł���ׂ��������I�������
	m_flagBlinking = true; //m_pressAButton�̓��ߏ����Ɏg�p
}

void Title::Finish()
{
	m_flagProcess = false;

	//////////
	// �^�C�g���A�C�R���̔�\��
	//////////

	m_spriteTitle->Deactivate();

	//////////
	// Press_A_Button�̔�\��
	//////////

	m_spritePressAButton->Deactivate();
}

void Title::Update()
{
	if (false == m_flagProcess) {
		return;
	}

	//����
	if (g_pad[con::player_1]->IsTrigger(enButtonA) == true) {
		m_seDecision->Play(false);
		m_flagFinish = true;
	}

	//Press_A_Button�̓_�ŏ���
	if (m_flagBlinking == true) { //�����ɂȂ鏈��
		m_spritePressAButton->SetMulColorW(m_spritePressAButton->GetMulColorW() - CHANGE_MUL_COLOR_W);

		if (m_spritePressAButton->GetMulColorW() <= MINIMUM_MUL_COLOR_W) {
			m_flagBlinking = false;
		}
	}
	else { //���̂ɂȂ鏈��
		m_spritePressAButton->SetMulColorW(m_spritePressAButton->GetMulColorW() + CHANGE_MUL_COLOR_W);

		if (m_spritePressAButton->GetMulColorW() >= MAXIMUM_MUL_COLOR_W) {
			m_flagBlinking = true;
		}
	}
}