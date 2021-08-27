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

	const int TIME_GAME_END = 120; //�Q�[���I�����̃^�C��
	const int TIME_NEXT_ROUND = 120; //���̃��E���h�Ɉڍs����^�C��
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

	//���E���h�̃|�C���g��UI��NewGO
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
			m_spriteRoundWin[playerNum][roundNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
			m_spriteRoundWin[playerNum][roundNum]->Init(filePath::dds::ROUND_WIN[playerNum][roundNum]); //������
			m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //��\��
		}
	}

	//�Q�[���I�����̃X�v���C�g��NewGO
	m_spriteGameEnd = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteGameEnd->Init(filePath::dds::GAME_END); //������
	m_spriteGameEnd->Deactivate(); //��\��
}

Rank::~Rank()
{
	//�X�v���C�g�̍폜
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			DeleteGO(m_spriteGoalRank[playerNum][rankNum]); //�폜
		}
	}

	//���E���h��UI�̍폜
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
			DeleteGO(m_spriteRoundWin[playerNum][roundNum]); //�폜
		}
	}

	DeleteGO(m_spriteGameEnd); //�폜
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

	//���E���h��UI�̏�����
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
			m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //��\��
		}
	}

	//�Q�[���I�����̃X�v���C�g�̏�����
	m_spriteGameEnd->Deactivate(); //��\��

	////////////////////////////////////////////////////////////
	// �����o�ϐ��̏�����
	////////////////////////////////////////////////////////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_goalRanking[playerNum] = con::rank_notClear;
		m_roundPoint[playerNum] = 0; //�v���C���[�̃��E���h�̃|�C���g
	}

	m_flagGameEnd = false; //�Q�[�����I��������
	m_flagNextRound = false; //���̃��E���h�Ɉڍs���邩

	m_timerGameEnd = con::TIME_RESET_ZERO; //�Q�[���I�����̃^�C�}�[
	m_timerNextRound = con::TIME_RESET_ZERO; //���̃��E���h�Ɉڍs����^�C�}�[
}

void Rank::Finish()
{
    m_flagProcess = false;

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //��\��
		}
	}

	//���E���h��UI�̔�\��
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
			m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //��\��
		}
	}

	m_spriteGameEnd->Deactivate(); //��\��
}

void Rank::Update()
{
    if (false == m_flagProcess) {
        return;
    }

	//�T�h���f�X���[�h�̂Ƃ�
	if (true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
		//���̃��E���h�Ɉڍs���邩�̊m�F
		CheckNextRound();
	}
	//���[�X���[�h�̂Ƃ�
	else {
		//�Q�[�����I���������̊m�F
		CheckGameEnd();
	}

	//�擾�������E���h�|�C���g�̕\��
	DrawRoundPoint();
}

void Rank::DrawRoundPoint()
{
	//�T�h���f�X���[�h�łȂ��Ƃ�
	if (false == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
		return;
	}

	//�擾���E���h����UI�̕\��
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
			if (roundNum < m_roundPoint[playerNum]) {
				m_spriteRoundWin[playerNum][roundNum]->Activate(); //�\��
			}
		}
	}
}

void Rank::CheckGameEnd()
{
	//�Q�[�����I��������
	if (false == m_flagGameEnd) {
		return;
	}

	m_spriteGameEnd->Activate(); //�\��

	++m_timerGameEnd; //�^�C�}�[�̃J�E���g

	//�^�C�}�[�𒴂���
	if (TIME_GAME_END <= m_timerGameEnd) {
		//�Q�[���̏I������
		GameEnd();
	}
}

void Rank::CheckNextRound()
{
	//���̃��E���h�Ɉڍs���邩
	if (false == m_flagNextRound) {
		return;
	}

	//�Q�[�����I���������i�N�����A�R���E���h���悷��j
	if (true == m_flagGameEnd) {
		CheckGameEnd();
		return;
	}

	++m_timerNextRound; //�^�C�}�[�̃J�E���g

	//�^�C�}�[�𒴂���
	if (TIME_NEXT_ROUND <= m_timerNextRound) {
		//���̃��E���h�Ɉڍs����
		m_findSuddenDeathMode->SetFlagNextRound(true);
	}
}

void Rank::GameEnd()
{
	//�T�h���f�X���[�h�̂Ƃ�
	if (true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
		//�T�h���f�X���[�h���̍ŏI���ʂ̌���
		SuddenDeathRank();

		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
			m_findResult->SetRank(playerNum, m_goalRanking[playerNum]); //���U���g�V�[���ɏ��ʂ�n��
		}

		m_findMainProcessing->SetGameEnd(true); //�Q�[���V�[�����I���������Ƃ�`����
	}
	//���[�X���[�h�̂Ƃ�
	else {
		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
			m_findResult->SetRank(playerNum, m_goalRanking[playerNum]); //���U���g�V�[���ɏ��ʂ�n��
		}

		m_findMainProcessing->SetGameEnd(true); //�Q�[���V�[�����I���������Ƃ�`����
	}
}

void Rank::SuddenDeathRank()
{
	int Ranking = con::rank_1; //����
	bool checkAddRank = false; //���̎擾���E���h���̃v���C���[�̏��ʂ��ς�邩

	//�擾���E���h�����������ɍ������ʂɂ���
	for (int roundPointNum = 3; roundPointNum >= con::FIRST_ELEMENT_ARRAY; roundPointNum--) {
		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
			//���ʂ��m��
			if (m_roundPoint[playerNum] == roundPointNum) {
				m_goalRanking[playerNum] = Ranking;
				checkAddRank = true;
			}
		}

		//���̃v���C���[�̏��ʂ̂��߂ɏ��ʂ𑝉�����
		if (checkAddRank == true) {
			++Ranking; //���ʂ�ύX
			checkAddRank = false;
		}
	}
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

	m_flagNextRound = false; //���̃��E���h�Ɉڍs���邩

	m_timerNextRound = con::TIME_RESET_ZERO; //���̃��E���h�Ɉڍs����^�C�}�[
}