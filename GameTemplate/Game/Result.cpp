#include "stdafx.h"
#include "result.h"
#include "main_processing.h"



namespace
{
	const Vector3 PLAYER_POSITION[con::PlayerNumberMax] = {	//�v���C���[�̏������W
		{ 1200.0f, -1000.0f, -240.0f },											//�v���C���[�P
		{ 400.0f, -1000.0f, -240.0f },											//�v���C���[�Q
		{ -400.0f, -1000.0f, -240.0f },											//�v���C���[�R
		{ -1200.0f, -1000.0f, -240.0f }											//�v���C���[�S
	};
	const Vector2 PLAYER_RANK_SPRITE[con::PlayerNumberMax] = {	//�v���C���[�̏������W
		{ -420.0f, -170.0f},											//�v���C���[�P
		{ -140.0f, -170.0f },											//�v���C���[�Q
		{ 140.0f, -170.0f },											//�v���C���[�R
		{ 420.0f,-170.0f }											//�v���C���[�S
	};

	const Vector2 MODE_SELECT_SPRITE[con::PlayerNumberMax] = {	//�v���C���[�̏������W
		{ 0.0f, 200.0f },											//�v���C���[�P
		{ 0.0f, 100.0f },											//�v���C���[�Q
		{ 0.0f, 0.0f },											//�v���C���[�R
		{ 0.0f, -100.0f }											//�v���C���[�S
	};

	const int UP_END = 0;     //��ԏ�
	const int DOWN_END = 3;    //��ԉ�
}



Result::Result()
{
	//////////////////////////////
	// NewGO
	//////////////////////////////

	//////////
	// �w�i��NewGO
	//////////

	m_spriteBackground = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
	m_spriteBackground->Init(filePath::dds::BACKGROUND);
	m_spriteBackground->Deactivate();

	//////////
	// ���f����NewGO
	//////////

	//�A�j���[�V�����̐ݒ�
	m_animationPlayer[win].Load(filePath::tka::WIN);
	m_animationPlayer[stand].Load(filePath::tka::STAND);
	m_animationPlayer[lose].Load(filePath::tka::LOSE);
	//���[�v�Đ�
	m_animationPlayer[stand].SetLoopFlag(true);
	m_animationPlayer[win].SetLoopFlag(false);
	m_animationPlayer[lose].SetLoopFlag(false);

	//�v���C���[���f����NewGO
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
	// �X�v���C�g��NewGO
	//////////

	//�I������NewGO
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
		m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI_2);
		m_spriteChoices[spriteNum]->Init(filePath::dds::RESULT_COMMAND[spriteNum]);
		m_spriteChoices[spriteNum]->Deactivate();
	}

	//PressANext��NewGO
	m_spritePressANext = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spritePressANext->Init(filePath::dds::PRESS_A_NEXT);
	m_spritePressANext->Deactivate();

	//////////
	// SE��NewGO
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
	// �w�i�̏�����
	//////////

	m_spriteBackground->Deactivate();

	//////////
	// ���f���̏�����
	//////////

	//�v���C���[���f���̏�����
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {	
		m_modelCharacter[playerNum]->Activate();
		m_modelCharacter[playerNum]->SetRotationX(0.5f);
		m_modelCharacter[playerNum]->SetPosition(PLAYER_POSITION[playerNum]);
		m_modelCharacter[playerNum]->SetScale({ 0.2f,0.2f,0.2f });
	}

	//////////
	// �X�v���C�g��NewGO�A������
	//////////

	//�v���C���[���Ƃ̏��ʂɉ�����NewGO�A������
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

	//�I�����̏�����
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

	//PressANext�̏�����
	m_spritePressANext->Activate();

	//////////
	// �����o�ϐ��̏�����
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_rank[playerNum] = con::GoalRankMax;
	}

	m_flagDecision = false;    //���肵�����̃t���O
	m_cursorPosition = 0; //�J�[�\���̏ꏊ
	m_flagFinish = false;      //���̃N���X�ł���ׂ��������I�������
	m_flagBlinking = true; //m_pressAButton�̓��ߏ����Ɏg�p

	m_displayStatus = DisplayStatus::result;
}

void Result::Finish()
{
	m_flagProcess = false;

	//////////
	// �w�i�̔�\��
	//////////

	m_spriteBackground->Deactivate();

	//////////
	// ���f���̔�\��
	//////////

	//�v���C���[���f���̔�\��
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->Deactivate();
	}

	//////////
	// �X�v���C�g��DeleteGO�A��\��
	//////////

	//�v���C���[���Ƃ̏��ʂɉ�����DeleteGO
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		DeleteGO(m_spriteGoalRank[playerNum]);
	}

	//�I�����̏�����
	for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
		m_spriteChoices[spriteNum]->Deactivate();
	}

	//PressANext�̏�����
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
		m_seDecision->Play(false); //�Đ�
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
	//����
	if (true == g_pad[con::player_1]->IsTrigger(enButtonA)) {
		m_seDecision->Play(false); //�Đ�
		m_game->SetResultSelect(m_cursorPosition);

		m_displayStatus = DisplayStatus::finish;
	}
	//��Ɉړ�
	else if (true == g_pad[con::player_1]->IsTrigger(enButtonUp)) {
		m_seMoveCursor->Play(false);

		if (m_cursorPosition == UP_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);

		--m_cursorPosition;

		m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
	}
	//���Ɉړ�
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