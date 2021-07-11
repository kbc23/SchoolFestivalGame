#include "stdafx.h"
#include "EnemyAI.h"
#include "player.h"
#include "stage.h"
#include "main_processing.h"
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
	m_game = FindGO<MainProcessing>(igo::CLASS_NAME_GAME);
	m_stage = FindGO<Stage>(igo::CLASS_NAME_STAGE);

	return true;
}

void EnemyAI::Init()
{
	m_flagProcessing == true;

	for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
		m_activePlayer[playerNum] = false;
		m_activeOperation[playerNum] = true;
		m_flagAnimationJump[playerNum] = false;
		m_flagGoal[playerNum] = false;
		m_JumpFlag[playerNum] = false;
	}

	//m_difficultyLevel = 0;//難易度1簡単2普通3難しい

	m_maxPlayer = 0;//プレイヤー最大数
	m_addEnemyNumber = 2;//CPU用番号=2
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
	//	m_player->Animation(pNum);
	}
	/*else {
		m_player->Animation(pNum);
	}*/
}

void EnemyAI::Moverule1(const int pNum) {
	m_JumpFlag[pNum] = false;

	m_flagAnimationJump[pNum] = m_player->GetmFlagAnimationJump(pNum);
	if (m_player->GetFlagGoal(pNum) == false) {

		DifficultyMoverule1(pNum);
		m_player->SetCPUJumpFlag(pNum, m_JumpFlag[pNum]);
		//	m_player->Animation(pNum);
	}
	/*else {
		m_player->Animation(pNum);
	}*/
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
		if (ran1 <=2) {//3割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
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
		if (ran1 <= 7) {//9割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
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
		if (ran1 <= 5) {//7割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
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
		if (ran1 <= 2) {//5割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
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
	m_bluemiss[pNum] = m_player->GetBlueMiss(pNum);
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
	//3回までミスする動きをミスしない動きに変える
		if (m_missInvalidCount[pNum] < 3&&m_stage->GetStageDatePuls2(pNum) != 0) {//1マス進む
			m_missInvalidCount[pNum]++;
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