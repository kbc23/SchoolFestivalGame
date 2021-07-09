#include "stdafx.h"
#include "EnemyAI.h"
#include "player.h"
#include "stage.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h> // rand, srand関数
#include <time.h>   // time関数

const int MOVE_2 = 2;		//移動量2
const int MOVE_1 = 1;		//移動量1


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
	m_JumpFlag[pNum] = false;

	m_flagAnimationJump[pNum] = m_player->GetmFlagAnimationJump(pNum);
	if (m_player->GetFlagGoal(pNum) == false) {

		DifficultyMove(pNum);
		m_player->SetCPUJumpFlag(pNum, m_JumpFlag[pNum]);
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
	// 乱数生成
	unsigned int    now = (unsigned int)time(0);
	srand(now);
	for (int i = 0; i < pNum * 2; i++) {
		rand();
	}
	int     ran1 = rand() % 10;//乱数結果0～9

	switch (m_difficultyLevel) {
	case 0:
		if (ran1 <=7) {//8割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoController1(pNum);
		}
		//	else if (ran1 <= 6) {//3割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも1マス移動)
		//		AutoController2(pNum);
		//	}
		else {//2割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;
	case 1:
		if (ran1 <= 5) {//6割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoController1(pNum);
		}
		//	else if (ran1 <= 4) {//2割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも1マス移動)
		//		AutoController2(pNum);
		//	}
		else {//4割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;
	case 2:
		if (ran1 <= 2) {//3割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoController1(pNum);
		}
		//	else if (ran1 <= 2) {//1割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも1マス移動)
		//		AutoController2(pNum);
		//	}
		else {//7割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;
		/*default:
			if (ran1 <= 2) {//3割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
				AutoController1(pNum);
			}
			//else if (ran1 <= 2) {//1割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも1マス移動)
		//		AutoController2(pNum);
		//	}
			else {//7割の行動(最適な行動)
				AutoController3(pNum);
			}
			//}
			*/
	}
}

void EnemyAI::DifficultyMoverule1(const int pNum) {
	// 乱数生成
	unsigned int    now = (unsigned int)time(0);
	srand(now);
	for (int i = 0; i < pNum * 3; i++) {
		rand();
	}
	int     ran1 = rand() % 10;//乱数結果0～9

	switch (m_difficultyLevel) {
	case 0:
		if (ran1 <= 8) {//9割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoController11(pNum);
		}
		//	else if (ran1 <= 6) {//3割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも1マス移動)
		//		AutoController2(pNum);
		//	}
		else {//1割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;
	case 1:
		if (ran1 <= 6) {//7割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoController11(pNum);
		}
		//	else if (ran1 <= 4) {//2割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも1マス移動)
		//		AutoController2(pNum);
		//	}
		else {//3割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;
	case 2:
		if (ran1 <= 4) {//5割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoController11(pNum);
		}
		//	else if (ran1 <= 2) {//1割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも1マス移動)
		//		AutoController2(pNum);
		//	}
		else {//5割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;
		/*default:
			if (ran1 <= 2) {//3割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
				AutoController11(pNum);
			}
			//else if (ran1 <= 2) {//1割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも1マス移動)
		//		AutoController2(pNum);
		//	}
			else {//7割の行動(最適な行動)
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
	m_bluemiss[pNum] = m_stage->GetBlueMiss(pNum);
	if (m_bluemiss[pNum] == true) {
		m_bluemiss[pNum] = false;

		//1マス進む
		if (m_stage->MoveBlock(pNum, MOVE_1) == false) {
			return;
		}
	}
	
	

	//２マス進む
	else if (m_stage->MoveBlock(pNum, MOVE_2) == false) {
		return;
	}
	//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
	
	m_JumpFlag[pNum] = true;

	m_flagAnimationJump[pNum] = true;

//	m_player->SetCPUJumpFlag(pNum, m_JumpFlag[pNum]);
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
		if (m_stage->GetStageDatePuls2(pNum) != 0) {//1マス進む
			if (m_stage->MoveBlock(pNum, MOVE_1) == false) {
				return;
			}
			m_stopCount++;
		}
	}
	//２マス進む
	else if (m_stage->MoveBlock(pNum, MOVE_2) == false) {
		return;
	}
	//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
	m_JumpFlag[pNum] = true;

	m_flagAnimationJump[pNum] = true;

	//	m_player->SetCPUJumpFlag(pNum, m_JumpFlag[pNum]);
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

	//1マス進む
	if (m_stage->MoveBlock(pNum, MOVE_1) == false) {
		return;
	}
	//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
	m_flagAnimationJump[pNum] = true;
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);

}*/

void EnemyAI::AutoController3(const int pNum)
{
	//p_numはプレイヤーのコントローラー番号

	if (m_game->GetStopOperation() == true) {
		return;
	}

	if (m_flagAnimationJump[pNum] == true ||
		//m_flagCheckBlock[pNum] == true ||
		m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}
	if (m_stage->GetStageDatePuls2(pNum) == 0||m_stage->GetStageDatePuls2(pNum) == 3) {//2マス進む
		if (m_stage->MoveBlock(pNum, MOVE_2) == false) {
			return;
		}
	}
	else {//1マス進む
		if (m_stage->MoveBlock(pNum, MOVE_1) == false) {
			return;
		}
	}

	//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
	

	m_JumpFlag[pNum] = true;

	m_flagAnimationJump[pNum] = true;

	//	m_player->SetCPUJumpFlag(pNum, m_JumpFlag[pNum]);
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);
}