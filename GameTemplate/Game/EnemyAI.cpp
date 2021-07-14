#include "stdafx.h"
#include "EnemyAI.h"
#include "player.h"
#include "stage.h"
#include "main_processing.h"
#include <stdio.h>
#include <stdlib.h> // rand, srand�֐�
#include <time.h>   // time�֐�


//�萔�̒�`
const int EnemyAI::m_MOVE_2 = 2;		//�ړ���2
const int EnemyAI::m_MOVE_1 = 1;		//�ړ���1
const int EnemyAI::m_MISS_INVALID = 3;//�~�X�𖳌��ɂ����,���݂�3��ɐݒ�
const int EnemyAI::m_RAND_TIMES = 3;//���݂�3�ApNum*RAND_TIMES��rand�̎��s��
const int EnemyAI::m_RANDOM_ZERO_TO_NINE = 10;//10�Arand�������ė]��0�`9���o��
const int EnemyAI::m_MISSPLAY_EASY = 8;//���݂�8�A��Փx��킢�̃~�X�v���C�ɂȂ�\���̂���s�������銄��
const int EnemyAI::m_MISSPLAY_NORMAL = 6;//���݂�6�A��Փx�ӂ��̃~�X�v���C�ɂȂ�\���̂���s�������銄��
const int EnemyAI::m_MISSPLAY_DIFFICULT = 3;//���݂�3�A��Փx�悢�̃~�X�v���C�ɂȂ�\���̂���s�������銄��
EnemyAI::EnemyAI() {

}

EnemyAI::~EnemyAI() {

}
bool EnemyAI::Start() {
	m_player = FindGO<Player>(igo::CLASS_NAME_PLAYER);
	m_game = FindGO<MainProcessing>(igo::CLASS_NAME_GAME);
	m_stage = FindGO<Stage>(igo::CLASS_NAME_STAGE);

	return true;
}

void EnemyAI::Init()
{
	m_flagProcessing = true;

	for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
		m_activePlayer[playerNum] = false;
		m_activeOperation[playerNum] = true;
		m_flagAnimationJump[playerNum] = false;
		m_flagGoal[playerNum] = false;
		m_JumpFlag[playerNum] = false;
	}
}

void EnemyAI::Finish()
{
	m_flagProcessing = false;
}

void EnemyAI::Update()
{
	if (m_flagProcessing == false) {
		return;
	}
}
void EnemyAI::Move(const int pNum) {
	m_JumpFlag[pNum] = false;

	m_flagAnimationJump[pNum] = m_player->GetmFlagAnimationJump(pNum);
	if (m_player->GetFlagGoal(pNum) == false) {

		DifficultyMove(pNum);
		m_player->SetCPUJumpFlag(pNum, m_JumpFlag[pNum]);
	}
}

void EnemyAI::Moverule1(const int pNum) {
	m_JumpFlag[pNum] = false;

	m_flagAnimationJump[pNum] = m_player->GetmFlagAnimationJump(pNum);
	if (m_player->GetFlagGoal(pNum) == false) {

		DifficultyMoverule1(pNum);
		m_player->SetCPUJumpFlag(pNum, m_JumpFlag[pNum]);
	}
}



void EnemyAI::DifficultyMove(const int pNum) {
	// ��������
	unsigned int    now = (unsigned int)time(0);
	srand(now);
	for (int i = 0; i < pNum * m_RAND_TIMES; i++) {
		rand();
	}
	m_randomNumber = rand() % m_RANDOM_ZERO_TO_NINE;//��������0�`9

	switch (m_difficultyLevel) {
	case 0:
		if (m_randomNumber < m_MISSPLAY_EASY) {//8���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoController1(pNum);
		}
		else {//2���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
	case 1:
		if (m_randomNumber < m_MISSPLAY_NORMAL) {//6���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoController1(pNum);
		}
		else {//4���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
	case 2:
		if (m_randomNumber < m_MISSPLAY_DIFFICULT) {//3���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoController1(pNum);

		}
		else {//7���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;		
	}
}

void EnemyAI::DifficultyMoverule1(const int pNum) {
	// ��������
	unsigned int    now = (unsigned int)time(0);
	srand(now);
	for (int i = 0; i < pNum * m_RAND_TIMES; i++) {
		rand();
	}
	 m_randomNumber = rand() % m_RANDOM_ZERO_TO_NINE;//��������0�`9

	switch (m_difficultyLevel) {
	case 0:
		if (m_randomNumber < m_MISSPLAY_EASY) {//8���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoControllerRule1(pNum);
		}		
		else {//2���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
	case 1:
		if (m_randomNumber < m_MISSPLAY_NORMAL) {//6���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoControllerRule1(pNum);
		}
		else {//4���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
	case 2:
		if (m_randomNumber < m_MISSPLAY_DIFFICULT) {//3���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoControllerRule1(pNum);
		}
		else {//7���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
	}
}

void EnemyAI::AutoController1(const int pNum)
{

	if (m_game->GetStopOperation() == true) {
		return;
	}

	if (m_flagAnimationJump[pNum] == true ||
		m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}
	m_bluemiss[pNum] = m_player->GetBlueMiss(pNum);
	if (m_bluemiss[pNum] == true) {
		m_bluemiss[pNum] = false;

		//1�}�X�i��
		if (m_stage->MoveBlock(pNum, m_MOVE_1) == false) {
			return;
		}
	}
	
	//�Q�}�X�i��
	else if (m_stage->MoveBlock(pNum, m_MOVE_2) == false) {
		return;
	}
	//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
	m_JumpFlag[pNum] = true;
	m_flagAnimationJump[pNum] = true;
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);
}

void EnemyAI::AutoControllerRule1(const int pNum)
{

	if (m_game->GetStopOperation() == true) {
		return;
	}

	if (m_flagAnimationJump[pNum] == true ||
		m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}
	//3��܂Ń~�X���铮�����~�X���Ȃ������ɕς���
		if (m_missInvalidCount[pNum] < m_MISS_INVALID &&m_stage->GetStageDatePuls2(pNum) != 0) {//1�}�X�i��
			m_missInvalidCount[pNum]++;
			if (m_stage->MoveBlock(pNum, m_MOVE_1) == false) {		
				return;
			}
		}
	
	//�Q�}�X�i��
	else if (m_stage->MoveBlock(pNum, m_MOVE_2) == false) {
		return;
	}
	//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
	m_JumpFlag[pNum] = true;
	m_flagAnimationJump[pNum] = true;
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);	
}

void EnemyAI::AutoController3(const int pNum)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	if (m_game->GetStopOperation() == true) {
		return;
	}

	if (m_flagAnimationJump[pNum] == true ||
		m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}
	if (m_stage->GetStageDatePuls2(pNum) == 0||m_stage->GetStageDatePuls2(pNum) == 3) {//2�}�X�i��
		if (m_stage->MoveBlock(pNum, m_MOVE_2) == false) {
			return;
		}
	}
	else {//1�}�X�i��
		if (m_stage->MoveBlock(pNum, m_MOVE_1) == false) {
			return;
		}
	}
	//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
	m_JumpFlag[pNum] = true;
	m_flagAnimationJump[pNum] = true;
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);
}