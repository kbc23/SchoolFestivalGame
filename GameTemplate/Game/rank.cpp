#include "stdafx.h"
#include "rank.h"

#include "constant.h"
#include "sudden_death_mode.h"
#include "result.h"
#include "main_processing.h"



namespace
{
	const Vector2 PLAYER_RANK_SPRITE[con::PlayerNumberMax] = {	//�v���C���[�̏��ʂ̏������W
		{ -490.0f, -70.0f },										//�v���C���[�P
		{ -180.0f, -70.0f },										//�v���C���[�Q
		{ 140.0f, -70.0f },											//�v���C���[�R
		{ 460.0f, -70.0f }											//�v���C���[�S
	};
}



Rank::Rank()
{
	////////////////////////////////////////////////////////////
	// �X�v���C�g��NewGO
	////////////////////////////////////////////////////////////

	//�v���C���[���ʂ̃X�v���C�g��NewGO
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum] = NewGO<SpriteRender>(igo::PRIORITY_UI); //NewGO
			m_spriteGoalRank[playerNum][rankNum]->Init(filePath::dds::RANK[rankNum]); //������
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //��\��
		}
	}

	//�Q�[���I�����̃X�v���C�g��NewGO
	m_spriteGameEnd = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteGameEnd->Init(filePath::dds::GAME_END); //������
	m_spriteGameEnd->Deactivate(); //��\��
}

Rank::~Rank()
{
	//�X�v���C�g�̔�\��
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			DeleteGO(m_spriteGoalRank[playerNum][rankNum]);
		}
	}

	DeleteGO(m_spriteGameEnd);
}

bool Rank::Start()
{
	m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);
	m_findResult = FindGO<Result>(igo::CLASS_NAME_RESULT);
	m_findMainProcessing = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);

	return true;
}

void Rank::Init()
{
    m_flagProcess = true;

	////////////////////////////////////////////////////////////
	// �X�v���C�g�̏�����
	////////////////////////////////////////////////////////////

	//�v���C���[�̏��ʂ̃X�v���C�g�̏�����
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->SetPosition(PLAYER_RANK_SPRITE[playerNum]);
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //��\��
		}
	}

	//�Q�[���I�����̃X�v���C�g�̏�����
	m_spriteGameEnd->Deactivate(); //��\��

	////////////////////////////////////////////////////////////
	// �����o�ϐ��̏�����
	////////////////////////////////////////////////////////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_goalRanking[playerNum] = con::rank_notClear;
	}
}

void Rank::Finish()
{
    m_flagProcess = false;

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //��\��
		}
	}

	m_spriteGameEnd->Deactivate(); //��\��
}

void Rank::Update()
{
    if (false == m_flagProcess) {
        return;
    }

	//�Q�[�����I���������̊m�F
	CheckGameEnd();

	m_spriteGameEnd->Activate();
}

void Rank::CheckGameEnd()
{
	//�Q�[�����I��������
	if (false == m_flagGameEnd) {
		return;
	}

	//�T�h���f�X���[�h�̂Ƃ�
	if (true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
		m_findSuddenDeathMode->SetFlagNextRound(true); //���̃��E���h�Ɉڍs����悤�ɂ���
	}
	//���[�X���[�h�̂Ƃ�
	else {
		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
			m_findResult->SetRank(playerNum, m_goalRanking[playerNum]); //���U���g�V�[���ɏ��ʂ�n��
			m_findMainProcessing->SetGameEnd(true); //�Q�[���V�[�����I���������Ƃ�`����
		}
	}
}

void Rank::GameEnd()
{

}

////////////////////////////////////////////////////////////
// ���̃��E���h�ֈڍs
////////////////////////////////////////////////////////////

void Rank::NextRound()
{
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //��\��
		}
	}

	//�I���X�v���C�g���\��
	m_spriteGameEnd->Deactivate();

	////////////////////////////////////////////////////////////
	// �����o�ϐ�
	////////////////////////////////////////////////////////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_goalRanking[playerNum] = con::rank_notClear;
	}
}