#include "stdafx.h"
#include "player.h"

#include "game_data.h"
#include "main_processing.h"
#include "CPU_player_controller.h"
#include "stage.h"
#include "sudden_death_mode.h"
#include "pause.h"
#include "game_start_countdown.h"
#include "Result.h"

namespace //constant
{
	////////////////////////////////////////////////////////////
	// 初期化
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// SEの音量
	//////////////////////////////

	const float INIT_SE_JUMP = 0.3f;
	const float INIT_SE_FALL = 2.0f;
	const float INIT_SE_SRIP = 0.7f;

	////////////////////////////////////////////////////////////
	// 位置情報
	////////////////////////////////////////////////////////////

	const Vector3 PLAYER_START_POSITION[con::PlayerNumberMax] = {	//プレイヤーの初期座標
		{ 390.0f, 0.0f, -240.0f },											//プレイヤー１
		{ 130.0f, 0.0f, -240.0f },											//プレイヤー２
		{ -130.0f, 0.0f, -240.0f },											//プレイヤー３
		{ -390.0f, 0.0f, -240.0f }											//プレイヤー４
	};
	
	const Vector2 GOAL_RANK_FONT_POSITION[con::PlayerNumberMax] = {	//ゴール順位の表示座標
		{ -390.0f, 0.0f },													//プレイヤー１
		{ -130.0f, 0.0f },													//プレイヤー２
		{ 130.0f, 0.0f },													//プレイヤー３
		{ 390.0f, 0.0f }													//プレイヤー４
	};

	//モデルとフォントのX座標の向きが逆っぽい

	////////////////////////////////////////////////////////////
	// 入力関係
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// タイマー関連
	////////////////////////////////////////////////////////////

	const int TIMER_RESET = 0; //タイマーのリセット
	const int TIME_ANIMATION = 30; //ジャンプアニメーションの時間

	const int TIME_BLUE_BLOCK_ANIMATION = 30;   //青色のブロックに行ったときのアニメーションの時間

	const int TIME_YELLOW_BLOCK_ANIMATION = 30; //黄色のブロックに行ったときのアニメーションの時間


	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////

	const float JUMP_MOVE = 2.0f;

	const Vector2 PLAYER_RANK_SPRITE[con::PlayerNumberMax] = {	//プレイヤーの順位の初期座標
		{ -490.0f, -70.0f},											//プレイヤー１
		{ -180.0f, -70.0f },										//プレイヤー２
		{ 140.0f, -70.0f },											//プレイヤー３
		{ 460.0f, -70.0f }											//プレイヤー４
	};
}



Player::Player()
{
	//アニメーションの設定
	m_animationPlayer[idle].Load(filePath::tka::IDLE);
	m_animationPlayer[jump].Load(filePath::tka::JUMP);
	m_animationPlayer[fall].Load(filePath::tka::FALL);
	m_animationPlayer[srip].Load(filePath::tka::SRIP);
	m_animationPlayer[win].Load(filePath::tka::WIN);
	m_animationPlayer[stand].Load(filePath::tka::STAND);
	m_animationPlayer[lose].Load(filePath::tka::LOSE);
	//ループ再生をtrueにする
	m_animationPlayer[idle].SetLoopFlag(true);
	m_animationPlayer[stand].SetLoopFlag(true);

	//////////////////////////////
	// NewGO
	//////////////////////////////

	//////////
	// モデルのNewGO
	//////////

	//プレイヤーごとに処理
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
		m_modelCharacter[playerNum]->Init(
			filePath::tkm::CHAEACTER_MODEL, modelUpAxis::enModelUpAxisY, m_animationPlayer, AnimationMax);
		m_modelCharacter[playerNum]->Deactivate();
	}

	//////////
	// スプライトのNewGO
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
			m_spriteGoalRank[playerNum][rankNum]->Init(filePath::dds::RANK[rankNum]);
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}

	m_spriteGameEnd = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteGameEnd->Init(filePath::dds::GAME_END);
	m_spriteGameEnd->Deactivate();

	//////////
	// SEのNewGO
	//////////

	m_seJump = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seJump->Init(filePath::se::JUMP);
	m_seJump->SetVolume(INIT_SE_JUMP);

	m_seFall = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seFall->Init(filePath::se::FALL);
	m_seFall->SetVolume(INIT_SE_FALL);

	m_seSrip = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seSrip->Init(filePath::se::SRIP);
	m_seSrip->SetVolume(INIT_SE_SRIP);
}

////////////////////////////////////////////////////////////
// デストラクタ関連
////////////////////////////////////////////////////////////

Player::~Player()
{
	//プレイヤーごとに処理
	for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
		DeleteGO(m_modelCharacter[playerNum]);
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			DeleteGO(m_spriteGoalRank[playerNum][rankNum]);
		}
	}

	DeleteGO(m_spriteGameEnd);
	DeleteGO(m_seJump);
	DeleteGO(m_seFall);
	DeleteGO(m_seSrip);
}

////////////////////////////////////////////////////////////
// Start関数関連
////////////////////////////////////////////////////////////

bool Player::Start()
{
	//////////////////////////////
	// FindGO
	//////////////////////////////

	m_findCPUPlayerController = FindGO<CPUPlayerController>(igo::CLASS_NAME_CPU_PLAYER_CONTROLLER);
	m_findGameData = FindGO<GameData>(igo::CLASS_NAME_GAME_DATA);
	m_findStage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
	m_findMainProcessing = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);
	m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);
	m_findGameStartCountdown = FindGO<GameStartCountdown>(igo::CLASS_NAME_GAME_START_COUNTDOWN);
	m_findResult = FindGO<Result>(igo::CLASS_NAME_RESULT);

	return true;
}

void Player::Init()
{
	m_flagProcess = true;

	//////////
	// モデルの初期化
	//////////

	//プレイヤーごとに処理
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->Activate();
		m_modelCharacter[playerNum]->SetPosition(PLAYER_START_POSITION[playerNum]);
		m_modelCharacter[playerNum]->SetScale({ 0.03f,0.03f,0.03f });
		m_modelCharacter[playerNum]->PlayAnimation(idle);
	}

	//////////
	// スプライトの初期化
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->SetPosition(PLAYER_RANK_SPRITE[playerNum]);
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}

	m_spriteGameEnd->Deactivate();

	//////////
	// メンバ変数の初期化
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_activePlayer[playerNum] = true; //このプレイヤーは操作しているか
		m_goalRanking[playerNum] = con::rank_notClear;	//プレイヤーのゴール順位
		m_flagGoal[playerNum] = false;	//ゴールしたか
		m_stopController[playerNum] = false; //操作不能か
		m_nowAnimationBlock[playerNum] = con::greenBlock; //プレイヤーの現在のアニメーション
		m_flagStopAnimation[playerNum] = false; //アニメーションの処理が止まっているか
		m_flagAnimationJump[playerNum] = false;	//ジャンプアニメーション中か
		m_timerAnimation[playerNum] = 0;						//アニメーションのタイマー
		m_roundPoint[playerNum] = 0;		//プレイヤーのラウンドのポイント
	}

	m_maxPlayer = con::PlayerNumberMax; //プレイヤーの最大数
	m_goalPlayer = 0;
	m_endTimer = 0; //ゴールしてからの時間
	fontDeavtive = 0;
	m_gameEnd = false; //ゴールしたプレイヤーの人数
	m_finishSuddenDeath = false; //サドンデスモードが終了したか
}

void Player::Finish()
{
	m_flagProcess = false;

	//プレイヤーごとに処理
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->Deactivate();
	}

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}

	m_spriteGameEnd->Deactivate();
}

////////////////////////////////////////////////////////////
// 毎フレームの処理
////////////////////////////////////////////////////////////

void Player::Update()
{
	if (false == m_flagProcess) {
		return;
	}

	//プレイヤーごとに操作
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		if (m_flagGoal[playerNum] == false) {
			Controller(playerNum);
		}

		Animation(playerNum);
	}

	if (con::PlayerNumberMax == m_goalPlayer || m_finishSuddenDeath == true) {
		m_spriteGameEnd->Activate();
		m_endTimer++;
		if (m_endTimer > 180) {
			//サドンデスモードのとき所持ラウンド勝利数に応じて順位を確定
			if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
				SuddenDeathRank();
			}

			//リザルトシーンに順位情報を渡す
			for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
				m_findResult->SetRank(playerNum, m_goalRanking[playerNum]);
			}

			m_gameEnd = true;
			m_findMainProcessing->SetGameEnd(m_gameEnd);
		}
	}
}

//////////////////////////////
// プレイヤーの操作処理
//////////////////////////////

void Player::Controller(const int& playerNum)
{
	//p_numはプレイヤーのコントローラー番号

	if (m_findGameStartCountdown->GetFlagStopGameProcess() == true) {
		return;
	}

	if (m_stopController[playerNum] == true) {
		return;
	}

	//if (m_findMainProcessing->GetStopOperation() == true) {
	//	return;
	//}

	if (m_flagAnimationJump[playerNum] == true) {
		return;
	}

	//プレイヤーの操作
	if (m_maxPlayer > playerNum) {
		//２マス進む
		if (g_pad[playerNum]->IsTrigger(enButtonA) == true) {
			if (m_findStage->MoveBlock(playerNum, con::MOVE_2) == false) {
				return;
			}
			//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
			m_seJump->Play(false);
			m_modelCharacter[playerNum]->PlayAnimation(jump);
			m_flagAnimationJump[playerNum] = true;
		}
		//１マス進む
		else if (g_pad[playerNum]->IsTrigger(enButtonB) == true) {
			if (m_findStage->MoveBlock(playerNum, con::MOVE_1) == false) {
				return;
			}
			//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
			m_seJump->Play(false);
			m_modelCharacter[playerNum]->PlayAnimation(jump);
			m_flagAnimationJump[playerNum] = true;
		}
	}
	//CPUの操作
	else {
		const int CPUMove = m_findCPUPlayerController->CPUController(playerNum);

		//２マス進む
		if (CPUMove == con::MOVE_2) {
			if (m_findStage->MoveBlock(playerNum, con::MOVE_2) == false) {
				return;
			}

			m_findCPUPlayerController->SetStopController(playerNum, true);

			//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
			m_seJump->Play(false);
			m_modelCharacter[playerNum]->PlayAnimation(jump);
			m_flagAnimationJump[playerNum] = true;
		}
		//１マス進む
		else if (CPUMove == con::MOVE_1) {
			if (m_findStage->MoveBlock(playerNum, con::MOVE_1) == false) {
				return;
			}

			m_findCPUPlayerController->SetStopController(playerNum, true);

			//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
			m_seJump->Play(false);
			m_modelCharacter[playerNum]->PlayAnimation(jump);
			m_flagAnimationJump[playerNum] = true;
		}
	}
}

//////////////////////////////
// プレイヤーのアニメーション
//////////////////////////////

void Player::Animation(const int& playerNum)
{
	if (m_flagStopAnimation[playerNum] == true) {
		return;
	}

	JumpAnimation(playerNum);

	switch (m_nowAnimationBlock[playerNum]) {
	case con::greenBlock:

		break;
	case con::blueBlock:
		//サドンデス時
		if (true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
			SuddenDeathBlueBlockAnimation(playerNum);
		}
		//レース時
		else {
			BlueBlockAnimation(playerNum);
		}
		break;
	case con::yellowBlock:
		//サドンデス時
		if (true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
			SuddenDeathYellowBlockAnimation(playerNum);
		}
		//レース時
		else {
			YellowBlockAnimation(playerNum);
		}
		break;
	case con::goalBlock:

		break;
	}
}

void Player::JumpAnimation(const int& playerNum)
{
	if (m_flagAnimationJump[playerNum] == false) {
		return;
	}

	++m_timerAnimation[playerNum];

	//ここのマジックナンバーを後で解消する。
	if (m_timerAnimation[playerNum] >= 0 && m_timerAnimation[playerNum] <= 15) {
		m_modelCharacter[playerNum]->UpPositionY(JUMP_MOVE);
	}
	else if (m_timerAnimation[playerNum] >= 16 && m_timerAnimation[playerNum] <= 30) {
		m_modelCharacter[playerNum]->DownPositionY(JUMP_MOVE);
	}


	if (m_timerAnimation[playerNum] >= TIME_ANIMATION) {
		m_flagAnimationJump[playerNum] = false;
		m_timerAnimation[playerNum] = TIMER_RESET;

		//着地したブロックの判定
		m_findStage->CheckBlock(playerNum);



		if (m_flagGoal[playerNum] == false) {
			m_modelCharacter[playerNum]->PlayAnimation(idle);
		}
		else {
			if (m_goalRanking[playerNum] == con::rank_1) {
				m_modelCharacter[playerNum]->PlayAnimation(win);
			}
			else if (m_goalRanking[playerNum] == con::rank_4) {
				m_modelCharacter[playerNum]->PlayAnimation(lose);
			}
			else {
				m_modelCharacter[playerNum]->PlayAnimation(stand);
			}
		}
	}
}

void Player::BlueBlockAnimation(const int& playerNum)
{
	//落下アニメーションを再生
	m_modelCharacter[playerNum]->PlayAnimation(fall);

	++m_timerAnimation[playerNum];
	m_modelCharacter[playerNum]->DownPositionY(30.0f);

	//前の位置に戻すためにフラグをfalseにする。
	if (m_timerAnimation[playerNum] >= TIME_BLUE_BLOCK_ANIMATION) {
		m_timerAnimation[playerNum] = TIMER_RESET;
		//モデルを元に戻す。
		m_modelCharacter[playerNum]->ResetPositionY();
		m_modelCharacter[playerNum]->PlayAnimation(idle);
		//キャラクターの位置を前の位置に戻す
		m_findStage->ReturnBlock(playerNum);

		//操作可能にする
		m_stopController[playerNum] = false;

		m_nowAnimationBlock[playerNum] = con::greenBlock;
	}
}

void Player::SuddenDeathBlueBlockAnimation(const int& playerNum)
{
	//落下アニメーションを再生
	m_modelCharacter[playerNum]->PlayAnimation(fall);

	++m_timerAnimation[playerNum];
	m_modelCharacter[playerNum]->DownPositionY(30.0f);

	//プレイヤーを非表示にする。
	if (m_timerAnimation[playerNum] >= TIME_BLUE_BLOCK_ANIMATION) {
		m_modelCharacter[playerNum]->Deactivate();
		//アニメーション処理をしないようにする
		m_flagStopAnimation[playerNum] = true;
	}
}

void Player::YellowBlockAnimation(const int& playerNum)
{
	//こけたアニメーションを再生
	m_modelCharacter[playerNum]->PlayAnimation(srip);

	++m_timerAnimation[playerNum];

	//操作可能にする。
	if (m_timerAnimation[playerNum] >= TIME_YELLOW_BLOCK_ANIMATION) {
		m_timerAnimation[playerNum] = TIMER_RESET;

		//モデルを元に戻す
		m_modelCharacter[playerNum]->PlayAnimation(idle);
		//操作可能にする
		m_stopController[playerNum] = false;

		m_nowAnimationBlock[playerNum] = con::greenBlock;
	}
}

void Player::SuddenDeathYellowBlockAnimation(const int& playerNum)
{
	//こけたアニメーションを再生
	m_modelCharacter[playerNum]->PlayAnimation(srip);

	++m_timerAnimation[playerNum];

	//操作可能にする。
	if (m_timerAnimation[playerNum] >= TIME_YELLOW_BLOCK_ANIMATION) {
		//アニメーション処理をしないようにする
		m_flagStopAnimation[playerNum] = true;
	}
}

void Player::NextRound()
{
	while (fontDeavtive < 120)
	{
		fontDeavtive += 1;
	}

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->PlayAnimation(idle);
		m_modelCharacter[playerNum]->ResetPositionY();
		m_modelCharacter[playerNum]->Activate();
	}
	
	if (fontDeavtive >= 120) {
		m_spriteGameEnd->Deactivate();
		m_goalPlayer = 0;

		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < m_maxPlayer; playerNum++) {
			m_flagGoal[playerNum] = false;
		}

		fontDeavtive = 0;

		for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
			for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < con::GoalRankMax; spriteNum++) {
				m_spriteGoalRank[playerNum][spriteNum]->Deactivate();
			}
		}
	}

	for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
		m_activePlayer[playerNum] = true;
		m_goalRanking[playerNum] = con::rank_notClear;
		m_flagGoal[playerNum] = false;
		m_flagAnimationJump[playerNum] = false;
		m_timerAnimation[playerNum] = 0;
		m_flagStopAnimation[playerNum] = false;
		m_stopController[playerNum] = false;
		m_nowAnimationBlock[playerNum] = con::greenBlock;
	}

	m_goalPlayer = 0;												
	fontDeavtive = 0;
}

////////////////////////////////////////////////////////////
// サドンデスモードの関数
////////////////////////////////////////////////////////////

void Player::SuddenDeathRank()
{
	int Ranking = con::rank_1;
	bool checkAddRank = false;

	//取得ラウンド数が多い順に高い順位にする
	for (int roundPointNum = 3; roundPointNum >= 0; roundPointNum--) {
		for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
			if (m_roundPoint[playerNum] == roundPointNum) {
				m_goalRanking[playerNum] = Ranking;
				checkAddRank = true;
			}
		}

		if (checkAddRank == true) {
			++Ranking;
			checkAddRank = false;
		}
	}
}