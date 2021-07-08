#include "stdafx.h"
#include "EnemyAI.h"
#include "player.h"
#include "stage.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h> // rand, srand�֐�
#include <time.h>   // time�֐�

const int MOVE_2 = 2;		//�ړ���2
const int MOVE_1 = 1;		//�ړ���1


EnemyAI::EnemyAI() {

}

EnemyAI::~EnemyAI() {

}
bool EnemyAI::Start() {
	m_player = FindGO<Player>(igo::CLASS_NAME_PLAYER);
	m_game = FindGO<Game>(igo::CLASS_NAME_GAME);
	m_stage = FindGO<Stage>(igo::CLASS_NAME_STAGE);

	return true;
}
void EnemyAI::Update() {


}
void EnemyAI::Move(const int pNum) {
	m_flagAnimationJump[pNum] = m_player->GetmFlagAnimationJump(pNum);
	if (m_player->GetFlagGoal(pNum) == false) {

		DifficultyMove(pNum);
	//	m_player->Animation(pNum);
	}
	/*else {
		m_player->Animation(pNum);
	}*/
}

void EnemyAI::Moverule1(const int pNum) {
	m_flagAnimationJump[pNum] = m_player->GetmFlagAnimationJump(pNum);
	if (m_player->GetFlagGoal(pNum) == false) {

		DifficultyMove(pNum);
		m_player->Animation(pNum);
	}
	else {
		m_player->Animation(pNum);
	}
}



void EnemyAI::DifficultyMove(const int pNum) {
	// ��������
	unsigned int    now = (unsigned int)time(0);
	srand(now);
	for (int i = 0; i < pNum * 2; i++) {
		rand();
	}
	int     ran1 = rand() % 10;//��������0�`9

	switch (m_difficultyLevel) {
	case 0:
		if (ran1 <= 6) {//7���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoController1(pNum);
		}
		//	else if (ran1 <= 6) {//3���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�1�}�X�ړ�)
		//		AutoController2(pNum);
		//	}
		else {//3���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
	case 1:
		if (ran1 <= 4) {//5���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoController1(pNum);
		}
		//	else if (ran1 <= 4) {//2���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�1�}�X�ړ�)
		//		AutoController2(pNum);
		//	}
		else {//5���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
	case 2:
		if (ran1 < 0) {//3���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoController1(pNum);
		}
		//	else if (ran1 <= 2) {//1���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�1�}�X�ړ�)
		//		AutoController2(pNum);
		//	}
		else {//7���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
		/*default:
			if (ran1 <= 2) {//3���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
				AutoController1(pNum);
			}
			//else if (ran1 <= 2) {//1���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�1�}�X�ړ�)
		//		AutoController2(pNum);
		//	}
			else {//7���̍s��(�œK�ȍs��)
				AutoController3(pNum);
			}
			//}
			*/
	}
}

void EnemyAI::DifficultyMoverule1(const int pNum) {
	// ��������
	unsigned int    now = (unsigned int)time(0);
	srand(now);
	for (int i = 0; i < pNum * 3; i++) {
		rand();
	}
	int     ran1 = rand() % 10;//��������0�`9

	switch (m_difficultyLevel) {
	case 0:
		if (ran1 <= 8) {//9���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoController11(pNum);
		}
		//	else if (ran1 <= 6) {//3���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�1�}�X�ړ�)
		//		AutoController2(pNum);
		//	}
		else {//1���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
	case 1:
		if (ran1 <= 6) {//7���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoController11(pNum);
		}
		//	else if (ran1 <= 4) {//2���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�1�}�X�ړ�)
		//		AutoController2(pNum);
		//	}
		else {//3���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
	case 2:
		if (ran1 <= 4) {//5���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
			AutoController11(pNum);
		}
		//	else if (ran1 <= 2) {//1���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�1�}�X�ړ�)
		//		AutoController2(pNum);
		//	}
		else {//5���̍s��(�œK�ȍs��)
			AutoController3(pNum);
		}
		break;
		/*default:
			if (ran1 <= 2) {//3���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�2�}�X�ړ�)
				AutoController11(pNum);
			}
			//else if (ran1 <= 2) {//1���̍s��(�~�X�v���C�̉\�����铮���A�ړ���̃u���b�N���ǂ�ł�1�}�X�ړ�)
		//		AutoController2(pNum);
		//	}
			else {//7���̍s��(�œK�ȍs��)
				AutoController3(pNum);
			}
			//}
			*/
	}
}

void EnemyAI::AutoController1(const int pNum)
{

	if (m_game->GetStopOperation() == true) {
		return;
	}

	if (m_flagAnimationJump[pNum] == true ||
		//m_flagCheckBlock[pNum] == true ||
		m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}
	//m_bluemiss = m_stage->GetBlueMiss();
	/*if (m_bluemiss == true) {
		//m_bluemiss = false;

		//1�}�X�i��
		if (m_stage->MoveBlock(pNum, MOVE_1) == false) {
			return;
		}
	}*/
	
	
	m_flagAnimationJump[pNum] = true;
	//�Q�}�X�i��
	if (m_stage->MoveBlock(pNum, MOVE_2) == false) {
		return;
	}
	//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
	
	m_flagAnimationJump[pNum] = true;
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);

}
void EnemyAI::AutoController11(const int pNum)
{

	if (m_game->GetStopOperation() == true) {
		return;
	}

	if (m_flagAnimationJump[pNum] == true ||
		//m_flagCheckBlock[pNum] == true ||
		m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}
	if (m_stopCount < 5) {
		if (m_stage->GetStageDatePuls2(pNum) != 0) {//1�}�X�i��
			if (m_stage->MoveBlock(pNum, MOVE_1) == false) {
				return;
			}
			m_stopCount++;
		}
	}
	//�Q�}�X�i��
	else if (m_stage->MoveBlock(pNum, MOVE_2) == false) {
		return;
	}
	//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
	m_flagAnimationJump[pNum] = true;
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);

}

/*void EnemyAI::AutoController2(const int pNum)
{

	if (m_game->GetStopOperation() == true) {
		return;
	}

	if (m_flagAnimationJump[pNum] == true ||
		//m_flagCheckBlock[pNum] == true ||
		m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}

	//1�}�X�i��
	if (m_stage->MoveBlock(pNum, MOVE_1) == false) {
		return;
	}
	//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
	m_flagAnimationJump[pNum] = true;
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);

}*/

void EnemyAI::AutoController3(const int pNum)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	if (m_game->GetStopOperation() == true) {
		return;
	}

	if (m_flagAnimationJump[pNum] == true ||
		//m_flagCheckBlock[pNum] == true ||
		m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}
	if (m_stage->GetStageDatePuls2(pNum) == 0||m_stage->GetStageDatePuls2(pNum) == 3) {//2�}�X�i��
		if (m_stage->MoveBlock(pNum, MOVE_2) == false) {
			return;
		}
	}
	else {//1�}�X�i��
		if (m_stage->MoveBlock(pNum, MOVE_1) == false) {
			return;
		}
	}

	//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
	m_flagAnimationJump[pNum] = true;
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);
}