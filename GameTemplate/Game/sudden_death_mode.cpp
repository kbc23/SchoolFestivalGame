#include "stdafx.h"
#include "sudden_death_mode.h"

#include "stage.h"
#include "score.h"
#include "player.h"
#include "game_start_countdown.h"
#include "CPU_player_controller.h"



namespace
{
	const int NEXT_ROUND_TIME = 120;
}



SuddenDeathMode::SuddenDeathMode()
{
}

SuddenDeathMode::~SuddenDeathMode()
{
}

/////////////////////////////////////////////////////////////////////////
// ���F�A�F�I�u�W�F�N�g�ɏ���Ă��܂������ɑ���s�\�ɂ���t���O�𗧂Ă�
/////////////////////////////////////////////////////////////////////////

bool SuddenDeathMode::Start()
{
	//////////////////////////////
	// FindGO
	//////////////////////////////

	m_findStage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
	m_findScore = FindGO<Score>(igo::CLASS_NAME_SCORE);
	m_findPlayer = FindGO<Player>(igo::CLASS_NAME_PLAYER);
	m_findGameStartCountdown = FindGO<GameStartCountdown>(igo::CLASS_NAME_GAME_START_COUNTDOWN);
	m_findCPUPlayerController = FindGO<CPUPlayerController>(igo::CLASS_NAME_CPU_PLAYER_CONTROLLER);

	return true;
}

void SuddenDeathMode::Init()
{
	m_flagProcess = true;

	//////////////////////////////
	// �����o�ϐ��̏�����
	//////////////////////////////

	m_finishSuddenDeath = false;
}

void SuddenDeathMode::Finish()
{
	m_flagProcess = false;

	//////////////////////////////
	// �����o�ϐ���Finish�̏���
	//////////////////////////////

	m_flagSuddenDeathMode = false; //�T�h���f�X���[�h��
}

void SuddenDeathMode::Update()
{
	if (false == m_flagProcess) {
		return;
	}

	//���̃��E���h�ւ̈ڍs����
	NextRound();
}

void SuddenDeathMode::NextRound()
{
	//���̃��E���h�ֈڍs���邩
	if (false == m_flagNextRound) {
		return;
	}

	++m_timerNextRound;

	//�^�C�}�[����������
	if (NEXT_ROUND_TIME > m_timerNextRound) {
		return;
	}

	//�e�N���X�̏�Ԃ����̃��E���h�Ɉړ����邽�߂ɕύX����
	m_findStage->NextRound();
	m_findScore->NextRound();
	m_findPlayer->NextRound();
	m_findGameStartCountdown->NextRound();
	m_findCPUPlayerController->NextRound();

	m_flagNextRound = false;
	m_timerNextRound = con::TIME_RESET_ZERO;
}