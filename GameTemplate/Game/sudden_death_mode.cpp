#include "stdafx.h"
#include "sudden_death_mode.h"

#include "stage.h"
#include "score.h"
#include "player.h"
#include "game_start_countdown.h"
#include "CPU_player_controller.h"

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

	//if (con::PlayerNumberMax == m_goalPlayer || m_finishSuddenDeath == true) {
	//	m_spriteGameEnd->Activate();
	//	m_endTimer++;
	//	if (m_endTimer > 180) {
	//		//�T�h���f�X���[�h�̂Ƃ��������E���h�������ɉ����ď��ʂ��m��
	//		if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
	//			SuddenDeathRank();
	//		}

	//		for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
	//			m_findMainProcessing->SetRank(playerNum, m_goalRanking[playerNum]);
	//		}

	//		m_gameEnd = true;
	//		m_findMainProcessing->SetGameEnd(m_gameEnd);
	//	}
	//}
}

void SuddenDeathMode::NextRound()
{
	//�e�N���X�̏�Ԃ����̃��E���h�Ɉړ����邽�߂ɕύX����
	m_findStage->NextRound();
	m_findScore->NextRound();
	m_findPlayer->NextRound();
	m_findGameStartCountdown->NextRound();
	m_findCPUPlayerController->NextRound();
}