#include "stdafx.h"
#include "pause.h"
#include "main_processing.h"

namespace //constant
{
	////////////////////////////////////////////////////////////
	// �ʒu���
	////////////////////////////////////////////////////////////

	const Vector2 CHOICES_POSITION[Pause::Choices::ChoicesMax] =
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

	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteChoices[spriteNum]->Init(filePath::dds::PAUSE_UI[spriteNum]); //������
		m_spriteChoices[spriteNum]->Deactivate(); //��\��
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
	// �X�v���C�g�̏�����
	//////////

	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		m_spriteChoices[spriteNum]->SetPosition(CHOICES_POSITION[spriteNum]); //�ʒu
		m_spriteChoices[spriteNum]->Deactivate(); //��\��
	}

	//////////
	// �����o�ϐ��̏�����
	//////////

	m_cursorPosition = UP_END; //�J�[�\���̏ꏊ
	m_flagDecision = false; //�l�������肵�����̃t���O
	m_pausePlayer = con::player_1; //�|�[�Y��Ԃɂ����v���C���[
}

void Pause::Finish()
{
	m_flagProcess = false;

	//////////
	// �X�v���C�g�̔�\��
	//////////

	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		m_spriteChoices[spriteNum]->Deactivate(); //��\��
	}
}

void Pause::PauseInit()
{
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		m_spriteChoices[spriteNum]->Activate(); //�\��

		//�ŏ��̑I����
		if (con::FIRST_ELEMENT_ARRAY == spriteNum) {
			m_spriteChoices[spriteNum]->SetMulColor(SRns::COLOR_NORMAL); //�J���[
		}
		else {
			m_spriteChoices[spriteNum]->SetMulColor(SRns::COLOR_GRAY); //�J���[
		}
	}

	m_cursorPosition = UP_END; //�J�[�\���̏ꏊ
	m_flagDecision = false; //�l�������肵�����̃t���O
}

void Pause::PauseFinish()
{
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < ChoicesMax; spriteNum++) {
		m_spriteChoices[spriteNum]->Deactivate(); //��\��
	}
}

void Pause::Update()
{
	if (false == m_flagProcess) {
		return;
	}

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		if (true == g_pad[playerNum]->IsTrigger(enButtonStart)) {
			//�|�[�Y��Ԃɂ���
			SetGamePaused(true);
			//�|�[�Y��ʂ̂��߂ɏ�����
			PauseInit();

			m_pausePlayer = playerNum; //�|�[�Y�����v���C���[���Z�b�g
		}
	}
}

void Pause::UpdateOnlyPaused()
{
	//����
	if (g_pad[m_pausePlayer]->IsTrigger(enButtonA) == true) {
		SetGamePaused(false); //�|�[�Y��Ԃ��I��点��

		//�Q�[���ɖ߂�
		if (m_cursorPosition == returnGame) {
			PauseFinish(); //�|�[�Y��ʂ��I������Ƃ��̌�n��
		}
		//���g���C
		else if (m_cursorPosition == retry) {
			m_mainProcessing->PauseRetry();
		}
		//�Q�[������߂�
		else if (m_cursorPosition == finish) {
			m_mainProcessing->PauseTitle();
		}

		m_flagDecision = true;
	}
	//��Ɉړ�
	else if (g_pad[m_pausePlayer]->IsTrigger(enButtonUp) == true) {
		//�J�[�\������[
		if (m_cursorPosition == UP_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY); //�J���[

		--m_cursorPosition; //�J�[�\�����ړ�

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL); //�J���[
	}
	//���Ɉړ�
	else if (g_pad[m_pausePlayer]->IsTrigger(enButtonDown) == true) {
		//�J�[�\�������[
		if (m_cursorPosition == DOWN_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY); //�J���[

		++m_cursorPosition; //�J�[�\�����ړ�

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL); //�J���[
	}
	//�X�^�[�g�{�^��
	if (true == g_pad[m_pausePlayer]->IsTrigger(enButtonStart)) {
		SetGamePaused(false); //�|�[�Y��Ԃ��I��点��
		PauseFinish(); //�|�[�Y��ʂ��I������Ƃ��̌�n��
	}
}