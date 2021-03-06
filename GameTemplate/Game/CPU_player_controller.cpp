#include "stdafx.h"
#include "CPU_player_controller.h"

#include <random>

#include "constant.h"
#include "stage.h"
#include "sudden_death_mode.h"



namespace //constant
{
	////////////////////////////////////////////////////////////
	// 確率関連
	////////////////////////////////////////////////////////////

	const int EASY_MISS = 80; //「よわい」のミスプレイの確率
	const int NORMAL_MISS = 60; //「ふつう」のミスプレイの確率
	const int HARD_MISS = 30; //「つよい」のミスプレイの確率

	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////

	const int EASY_JUMP_COOL_TIME = 60; //「よわい」のジャンプのクールタイム
	const int NORMAL_JUMP_COOL_TIME = 45; //「ふつう」のジャンプのクールタイム
	const int HARD_JUMP_COOL_TIME = 20; //「つよい」のジャンプのクールタイム
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
	// メンバ変数の初期化
	//////////

	for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
		m_flagBlueBlockMiss[playerNum] = false; //青ブロックでミスをしたか
		m_stopController[playerNum] = false; //CPUのコントローラーの処理を止めるか
		m_stopControllerTimer[playerNum] = con::TIME_RESET_ZERO; //CPUのコントローラーの処理を止めてるときのタイマー
	}
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
	//操作を停止しているかどうか
	if (StopController(playerNum) == true) {
		return con::NOT_MOVE;
	}

	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<int> randomNum(con::MINIMUM_RANDOM_NUMBER, con::MAXIMUM_RANDOM_NUMBER);

	const int random = randomNum(mt); //乱数

	switch (m_CPULevel) {
	//よわい
	case con::easy:
		//ミス
		if (random < EASY_MISS) {
			return MissMove(playerNum);
		}
		//ミスしない
		else {
			return OKMove(playerNum);
		}

		break;
	//ふつう
	case con::normal:
		//ミス
		if (random < NORMAL_MISS) {
			return MissMove(playerNum);
		}
		//ミスしない
		else {
			return OKMove(playerNum);
		}

		break;
	//つよい
	case con::hard:
		//ミス
		if (random < HARD_MISS) {
			return MissMove(playerNum);
		}
		//ミスしない
		else {
			return OKMove(playerNum);
		}

		break;
	}

	return con::NOT_MOVE;
}

bool CPUPlayerController::StopController(const int playerNum)
{
	//操作を止めないとき
	if (m_stopController[playerNum] == false) {
		return false;
	}
		
	++m_stopControllerTimer[playerNum];

	switch (m_CPULevel) {
	//よわい
	case con::easy:
		if (m_stopControllerTimer[playerNum] >= EASY_JUMP_COOL_TIME) {
			m_stopController[playerNum] = false;
			m_stopControllerTimer[playerNum] = con::TIME_RESET_ZERO;

			return false;
		}

		break;
	//ふつう
	case con::normal:
		if (m_stopControllerTimer[playerNum] >= NORMAL_JUMP_COOL_TIME) {
			m_stopController[playerNum] = false;
			m_stopControllerTimer[playerNum] = con::TIME_RESET_ZERO;

			return false;
		}

		break;
	//つよい
	case con::hard:
		if (m_stopControllerTimer[playerNum] >= HARD_JUMP_COOL_TIME) {
			m_stopController[playerNum] = false;
			m_stopControllerTimer[playerNum] = con::TIME_RESET_ZERO;

			return false;
		}

		break;
	}

	//操作を止める
	return true;
}

const int& CPUPlayerController::MissMove(const int playerNum)
{
	//サドンデスモードの場合
	//条件: ミスを無効にできる回数が残っているかつサドンデスモード時
	if (m_MISS_INVALID > m_missInvalidCount[playerNum] && true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
		++m_missInvalidCount[playerNum];
		//ミスしない
		return OKMove(playerNum);
	}

	//前回青ブロックでミスをしていたら
	if (true == m_flagBlueBlockMiss[playerNum]) {
		//２マス先が青ブロック
		if (m_findStage->GetStageDateNext2Block(playerNum) == con::blueBlock) {
			//１マス進む
			return con::MOVE_1;
		}
		//それ以外
		else {
			m_flagBlueBlockMiss[playerNum] = false;

			//２マス進む
			return con::MOVE_2;
		}
	}

	//２マス進む
	return con::MOVE_2;
}

const int& CPUPlayerController::OKMove(const int playerNum)
{
	//２マス先が緑ブロックまたはゴールブロック
	if (m_findStage->GetStageDateNext2Block(playerNum) == con::greenBlock ||
		m_findStage->GetStageDateNext2Block(playerNum) == con::goalBlock) {
		//２マス進む
		return con::MOVE_2;
	}
	//それ以外
	else {
		//１マス進む
		return con::MOVE_1;		
	}
}

void CPUPlayerController::NextRound()
{
	for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
		m_missInvalidCount[playerNum] = 0;
	}
}