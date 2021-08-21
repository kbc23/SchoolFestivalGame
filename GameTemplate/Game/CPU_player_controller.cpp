#include "stdafx.h"
#include "CPU_player_controller.h"

#include <random>

#include "constant.h"
#include "stage.h"
#include "sudden_death_mode.h"



namespace //constant
{
	////////////////////////////////////////////////////////////
	// �m���֘A
	////////////////////////////////////////////////////////////

	const int EASY_MISS = 80; //�u��킢�v�̃~�X�v���C�̊m��
	const int NORMAL_MISS = 60; //�u�ӂ��v�̃~�X�v���C�̊m��
	const int HARD_MISS = 30; //�u�悢�v�̃~�X�v���C�̊m��

	////////////////////////////////////////////////////////////
	// ���̑�
	////////////////////////////////////////////////////////////

	const int EASY_JUMP_COOL_TIME = 60; //�u��킢�v�̃W�����v�̃N�[���^�C��
	const int NORMAL_JUMP_COOL_TIME = 45; //�u�ӂ��v�̃W�����v�̃N�[���^�C��
	const int HARD_JUMP_COOL_TIME = 20; //�u�悢�v�̃W�����v�̃N�[���^�C��
}



CPUPlayerController::CPUPlayerController()
{

}

CPUPlayerController::~CPUPlayerController()
{

}

bool CPUPlayerController::Start()
{
	//////////////////////////////
	// FindGO
	//////////////////////////////

	m_findStage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
	m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);

	return true;
}

void CPUPlayerController::Init()
{
	m_flagProcess = true;

	//////////
	// �����o�ϐ��̏�����
	//////////

	for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
		m_flagBlueBlockMiss[playerNum] = false; //�u���b�N�Ń~�X��������
		m_stopController[playerNum] = false; //CPU�̃R���g���[���[�̏������~�߂邩
		m_stopControllerTimer[playerNum] = con::TIME_RESET_ZERO; //CPU�̃R���g���[���[�̏������~�߂Ă�Ƃ��̃^�C�}�[
	}

	m_CPULevel = con::easy; //CPU�̋���
}

void CPUPlayerController::Finish()
{
	m_flagProcess = false;
}

void CPUPlayerController::Update()
{
	if (false == m_flagProcess) {
		return;
	}
}

int CPUPlayerController::CPUController(const int playerNum)
{
	//return enButtonA;

	//������~���Ă��邩�ǂ���
	if (StopController(playerNum) == true) {
		return con::NOT_MOVE;
	}

	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<int> randomNum(con::MINIMUM_RANDOM_NUMBER, con::MAXIMUM_RANDOM_NUMBER);

	const int random = randomNum(mt);
	//EnButton button = enButtonNum;

	switch (m_CPULevel) {
	case con::easy:
		if (random < EASY_MISS) {
			return MissMove(playerNum);
		}
		else {
			return OKMove(playerNum);
		}

		break;
	case con::normal:
		if (random < NORMAL_MISS) {
			return MissMove(playerNum);
		}
		else {
			return OKMove(playerNum);
		}

		break;
	case con::hard:
		if (random < HARD_MISS) {
			return MissMove(playerNum);
		}
		else {
			return OKMove(playerNum);
		}

		break;
	}

	return con::NOT_MOVE;
}

bool CPUPlayerController::StopController(const int playerNum)
{
	//������~�߂Ȃ��Ƃ�
	if (m_stopController[playerNum] == false) {
		return false;
	}
		
	++m_stopControllerTimer[playerNum];

	switch (m_CPULevel) {
	case con::easy:
		if (m_stopControllerTimer[playerNum] >= EASY_JUMP_COOL_TIME) {
			m_stopController[playerNum] = false;
			m_stopControllerTimer[playerNum] = con::TIME_RESET_ZERO;

			return false;
		}

		break;
	case con::normal:
		if (m_stopControllerTimer[playerNum] >= NORMAL_JUMP_COOL_TIME) {
			m_stopController[playerNum] = false;
			m_stopControllerTimer[playerNum] = con::TIME_RESET_ZERO;

			return false;
		}

		break;
	case con::hard:
		if (m_stopControllerTimer[playerNum] >= HARD_JUMP_COOL_TIME) {
			m_stopController[playerNum] = false;
			m_stopControllerTimer[playerNum] = con::TIME_RESET_ZERO;

			return false;
		}

		break;
	}

	//������~�߂�
	return true;
}

const int& CPUPlayerController::MissMove(const int playerNum)
{
	//�T�h���f�X���[�h�̏ꍇ
	//����: �~�X�𖳌��ɂł���񐔂��P��ȏォ�T�h���f�X���[�h��
	if (0 < m_missInvalidCount[playerNum] && true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
		--m_missInvalidCount[playerNum];
		return OKMove(playerNum);
	}

	if (m_flagBlueBlockMiss[playerNum] == false) {
		//�Q�}�X�i��
		return con::MOVE_2;
	}

	if (m_findStage->GetStageDateNext2Block(playerNum) == con::blueBlock) {
		//�P�}�X�i��
		return con::MOVE_1;
	}
	else {
		m_flagBlueBlockMiss[playerNum] = false;

		//�Q�}�X�i��
		return con::MOVE_2;
	}

	//�P�}�X�i��
	return con::MOVE_1;
}

const int& CPUPlayerController::OKMove(const int playerNum)
{
	if (m_findStage->GetStageDateNext2Block(playerNum) == con::greenBlock ||
		m_findStage->GetStageDateNext2Block(playerNum) == con::goalBlock) {
		//�Q�}�X�i��
		return con::MOVE_2;
	}
	else {
		//�P�}�X�i��
		return con::MOVE_1;		
	}
}

void CPUPlayerController::NextRound()
{
	for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
		m_missInvalidCount[playerNum] = m_MISS_INVALID;
	}
}