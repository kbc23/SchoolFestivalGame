#include "stdafx.h"
#include "EnemyAI.h"
#include "player.h"
#include "stage.h"
#include "main_processing.h"
#include <stdio.h>
#include <stdlib.h> // rand, srand関数
#include <time.h>   // time関数


//定数の定義
const int EnemyAI::m_MOVE_2 = 2;		//移動量2
const int EnemyAI::m_MOVE_1 = 1;		//移動量1
const int EnemyAI::m_MISS_INVALID = 3;//ミスを無効にする回数,現在は3回に設定
const int EnemyAI::m_RAND_TIMES = 3;//現在は3、pNum*RAND_TIMESがrandの実行回数
const int EnemyAI::m_RANDOM_ZERO_TO_NINE = 10;//10、randを割って余り0～9を出す
const int EnemyAI::m_MISSPLAY_EASY = 8;//現在は8、難易度よわいのミスプレイになる可能性のある行動をする割合
const int EnemyAI::m_MISSPLAY_NORMAL = 6;//現在は6、難易度ふつうのミスプレイになる可能性のある行動をする割合
const int EnemyAI::m_MISSPLAY_DIFFICULT = 3;//現在は3、難易度つよいのミスプレイになる可能性のある行動をする割合
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
	// 乱数生成
	unsigned int    now = (unsigned int)time(0);
	srand(now);
	for (int i = 0; i < pNum * m_RAND_TIMES; i++) {
		rand();
	}
	m_randomNumber = rand() % m_RANDOM_ZERO_TO_NINE;//乱数結果0～9

	switch (m_difficultyLevel) {
	case 0:
		if (m_randomNumber < m_MISSPLAY_EASY) {//8割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoController1(pNum);
		}
		else {//2割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;
	case 1:
		if (m_randomNumber < m_MISSPLAY_NORMAL) {//6割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoController1(pNum);
		}
		else {//4割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;
	case 2:
		if (m_randomNumber < m_MISSPLAY_DIFFICULT) {//3割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoController1(pNum);

		}
		else {//7割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;		
	}
}

void EnemyAI::DifficultyMoverule1(const int pNum) {
	// 乱数生成
	unsigned int    now = (unsigned int)time(0);
	srand(now);
	for (int i = 0; i < pNum * m_RAND_TIMES; i++) {
		rand();
	}
	 m_randomNumber = rand() % m_RANDOM_ZERO_TO_NINE;//乱数結果0～9

	switch (m_difficultyLevel) {
	case 0:
		if (m_randomNumber < m_MISSPLAY_EASY) {//8割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoControllerRule1(pNum);
		}		
		else {//2割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;
	case 1:
		if (m_randomNumber < m_MISSPLAY_NORMAL) {//6割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoControllerRule1(pNum);
		}
		else {//4割の行動(最適な行動)
			AutoController3(pNum);
		}
		break;
	case 2:
		if (m_randomNumber < m_MISSPLAY_DIFFICULT) {//3割の行動(ミスプレイの可能性ある動き、移動先のブロックがどれでも2マス移動)
			AutoControllerRule1(pNum);
		}
		else {//7割の行動(最適な行動)
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

		//1マス進む
		if (m_stage->MoveBlock(pNum, m_MOVE_1) == false) {
			return;
		}
	}
	
	//２マス進む
	else if (m_stage->MoveBlock(pNum, m_MOVE_2) == false) {
		return;
	}
	//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
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
	//3回までミスする動きをミスしない動きに変える
		if (m_missInvalidCount[pNum] < m_MISS_INVALID &&m_stage->GetStageDatePuls2(pNum) != 0) {//1マス進む
			m_missInvalidCount[pNum]++;
			if (m_stage->MoveBlock(pNum, m_MOVE_1) == false) {		
				return;
			}
		}
	
	//２マス進む
	else if (m_stage->MoveBlock(pNum, m_MOVE_2) == false) {
		return;
	}
	//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
	m_JumpFlag[pNum] = true;
	m_flagAnimationJump[pNum] = true;
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);	
}

void EnemyAI::AutoController3(const int pNum)
{
	//p_numはプレイヤーのコントローラー番号

	if (m_game->GetStopOperation() == true) {
		return;
	}

	if (m_flagAnimationJump[pNum] == true ||
		m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}
	if (m_stage->GetStageDatePuls2(pNum) == 0||m_stage->GetStageDatePuls2(pNum) == 3) {//2マス進む
		if (m_stage->MoveBlock(pNum, m_MOVE_2) == false) {
			return;
		}
	}
	else {//1マス進む
		if (m_stage->MoveBlock(pNum, m_MOVE_1) == false) {
			return;
		}
	}
	//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
	m_JumpFlag[pNum] = true;
	m_flagAnimationJump[pNum] = true;
	m_player->SetFlagAnimationJump(pNum, m_flagAnimationJump[pNum]);
}