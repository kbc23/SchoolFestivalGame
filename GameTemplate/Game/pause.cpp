#include "stdafx.h"
#include "pause.h"
#include "main_processing.h"

namespace //constant
{
	////////////////////////////////////////////////////////////
	// �ʒu���
	////////////////////////////////////////////////////////////

	const Vector2 CHOICES_POSITION[3] = 
	{
		{0.0f,150.0f},
		{0.0f,0.0f},
		{0.0f,-150.0f}
	};

	////////////////////////////////////////////////////////////
	// ���̑�
	////////////////////////////////////////////////////////////

	const int UP_END = 0;      //��[
	const int DOWN_END = 2;   //���[
}

Pause::Pause()
{
	//////////////////////////////
	// NewGO
	//////////////////////////////

	//////////
	// �X�v���C�g��NewGO
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
	// �X�v���C�g�̏�����
	//////////

	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
		m_spriteChoices[spriteNum]->SetPosition(CHOICES_POSITION[spriteNum]);
		m_spriteChoices[spriteNum]->Deactivate();
	}

	//////////
	// �����o�ϐ��̏�����
	//////////

	m_cursorPosition = 0;       //�J�[�\���̏ꏊ
	m_numberOfPlayer = 0;       //�v���C���[�̐l��
	m_flagDecision = false;    //�l�������肵�����̃t���O
	m_flagFinish = false;      //���̃N���X�ł���ׂ��������I�������

}

void Pause::Finish()
{
	m_flagProcess = false;

	//////////
	// �X�v���C�g�̔�\��
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

	m_cursorPosition = 0;       //�J�[�\���̏ꏊ
	m_numberOfPlayer = 0;       //�v���C���[�̐l��
	m_flagDecision = false;    //�l�������肵�����̃t���O
	m_flagFinish = false;      //���̃N���X�ł���ׂ��������I�������
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
	//����
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
	//��Ɉړ�
	else if (g_pad[con::player_1]->IsTrigger(enButtonUp) == true) {
		if (m_cursorPosition == UP_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);

		--m_cursorPosition;

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
	}
	//���Ɉړ�
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