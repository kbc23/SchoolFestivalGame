#include "stdafx.h"
#include "player.h"

#include "main_processing.h"
#include "CPU_player_controller.h"
#include "stage.h"
#include "sudden_death_mode.h"
#include "pause.h"
#include "game_start_countdown.h"
#include "result.h"
#include "rank.h"

namespace //constant
{
	////////////////////////////////////////////////////////////
	// 初期化
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// SEの音量
	//////////////////////////////

	const float INIT_SE_JUMP = 0.3f; //ジャンプのSE
	const float INIT_SE_FALL = 2.0f; //落下時のSE
	const float INIT_SE_SRIP = 0.7f; //スリップ時のSE

	//////////////////////////////
	// 位置情報
	//////////////////////////////

	const Vector3 PLAYER_INIT_POSITION[con::PlayerNumberMax] = {	//プレイヤーの初期座標
		{ 390.0f, 0.0f, -240.0f },											//プレイヤー１
		{ 130.0f, 0.0f, -240.0f },											//プレイヤー２
		{ -130.0f, 0.0f, -240.0f },											//プレイヤー３
		{ -390.0f, 0.0f, -240.0f }											//プレイヤー４
	};
	
	const Vector2 PLAYER_RANK_SPRITE_INIT_POSITION[con::PlayerNumberMax] = {	//プレイヤーの順位の初期座標
		{ -490.0f, -70.0f},															//プレイヤー１
		{ -180.0f, -70.0f },														//プレイヤー２
		{ 140.0f, -70.0f },															//プレイヤー３
		{ 460.0f, -70.0f }															//プレイヤー４
	};

	////////////////////////////////////////////////////////////
	// タイマー関連
	////////////////////////////////////////////////////////////

	const int TIME_ANIMATION = 30; //ジャンプアニメーションの時間

	const int JUMP_UP_ANIMATION_START = 0; //ジャンプアニメーションの上昇の開始
	const int JUMP_UP_ANIMATION_END = 15; //ジャンプアニメーションの上昇の終わり
	const int JUMP_DOWN_ANIMATION_START = 16; //ジャンプアニメーションの下降の開始
	const int JUMP_DOWN_ANIMATION_END = 30; //ジャンプアニメーションの下降の終わり

	const int TIME_BLUE_BLOCK_ANIMATION = 30;   //青色のブロックに行ったときのアニメーションの時間

	const int TIME_YELLOW_BLOCK_ANIMATION = 30; //黄色のブロックに行ったときのアニメーションの時間

	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////

	const float JUMP_MOVE = 2.0f; //プレイヤーのジャンプ時の上下の移動量

	const float BLUE_BLOCK_ANIMATION_DOWN_POSITION = 30.0f; //青ブロック時の落下量
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

	////////////////////////////////////////////////////////////
	// NewGO
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// モデルのNewGO
	//////////////////////////////

	//プレイヤーモデルの初期化
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
		m_modelCharacter[playerNum]->Init(
			filePath::tkm::CHAEACTER_MODEL, modelUpAxis::enModelUpAxisY, m_animationPlayer, AnimationMax); //初期化
		m_modelCharacter[playerNum]->Deactivate(); //非表示
	}

	//////////////////////////////
	// SEのNewGO
	//////////////////////////////

	//ジャンプ時のSEの初期化
	m_seJump = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seJump->Init(filePath::se::JUMP); //初期化
	m_seJump->SetVolume(INIT_SE_JUMP); //音量

	//落下時のSEの初期化
	m_seFall = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seFall->Init(filePath::se::FALL); //初期化
	m_seFall->SetVolume(INIT_SE_FALL); //音量

	//スリップ時のSEの初期化
	m_seSrip = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seSrip->Init(filePath::se::SRIP); //初期化
	m_seSrip->SetVolume(INIT_SE_SRIP); //音量
}

////////////////////////////////////////////////////////////
// デストラクタ関連
////////////////////////////////////////////////////////////

Player::~Player()
{
	//プレイヤーごとに処理
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		DeleteGO(m_modelCharacter[playerNum]);
	}

	DeleteGO(m_seJump);
	DeleteGO(m_seFall);
	DeleteGO(m_seSrip);
}

////////////////////////////////////////////////////////////
// Start関数関連
////////////////////////////////////////////////////////////

bool Player::Start()
{
	////////////////////////////////////////////////////////////
	// FindGO
	////////////////////////////////////////////////////////////

	m_findCPUPlayerController = FindGO<CPUPlayerController>(igo::CLASS_NAME_CPU_PLAYER_CONTROLLER);
	m_findStage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
	m_findMainProcessing = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);
	m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);
	m_findGameStartCountdown = FindGO<GameStartCountdown>(igo::CLASS_NAME_GAME_START_COUNTDOWN);
	m_findResult = FindGO<Result>(igo::CLASS_NAME_RESULT);
	m_findRank = FindGO<Rank>(igo::CLASS_NAME_RANK);

	return true;
}

void Player::Init()
{
	m_flagProcess = true;

	////////////////////////////////////////////////////////////
	// モデルの初期化
	////////////////////////////////////////////////////////////

	//プレイヤーモデルの初期化
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->Activate(); //表示
		m_modelCharacter[playerNum]->SetPosition(PLAYER_INIT_POSITION[playerNum]); //位置
		m_modelCharacter[playerNum]->SetScale({ 0.03f,0.03f,0.03f }); //大きさ
		m_modelCharacter[playerNum]->PlayAnimation(idle); //アニメーション
	}

	////////////////////////////////////////////////////////////
	// メンバ変数の初期化
	////////////////////////////////////////////////////////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_activePlayer[playerNum] = true; //このプレイヤーは操作しているか
		m_goalRanking[playerNum] = con::rank_notClear; //プレイヤーのゴール順位
		m_stopController[playerNum] = false; //操作不能か
		m_nowAnimationBlock[playerNum] = con::greenBlock; //プレイヤーの現在のアニメーション
		m_flagStopAnimation[playerNum] = false; //アニメーションの処理が止まっているか
		m_flagAnimationJump[playerNum] = false;	//ジャンプアニメーション中か
		m_timerAnimation[playerNum] = 0; //アニメーションのタイマー
		m_roundPoint[playerNum] = 0; //プレイヤーのラウンドのポイント
	}

	m_maxPlayer = con::PlayerNumberMax; //プレイヤーの最大数
	m_goalPlayer = 0;
	m_endTimer = 0; //ゴールしてからの時間
	m_gameEnd = false; //ゴールしたプレイヤーの人数
	m_finishSuddenDeath = false; //サドンデスモードが終了したか
}

void Player::Finish()
{
	m_flagProcess = false;

	//プレイヤーモデルの非表示
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->Deactivate(); //非表示
	}
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
		//ゴールしたプレイヤーは操作できない
		if (false == m_findRank->GetDoingGoal(playerNum)) {
			//操作
			Controller(playerNum);
		}
		//アニメーション
		Animation(playerNum);
	}

	//終了時
	GameEnd();
}

void Player::GameEnd()
{
	//全員ゴールしたとき
	if (con::PlayerNumberMax == m_goalPlayer || m_finishSuddenDeath == true) {
		//終了
		
		m_endTimer++;
		if (m_endTimer > 180) {
			//サドンデスモードのとき所持ラウンド勝利数に応じて順位を確定
			if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
				//サドンデスモード時の最終順位の決定
				SuddenDeathRank();
			}
			
			//リザルトシーンに順位情報を渡す
			for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
				//m_findResult->SetRank(playerNum, m_goalRanking[playerNum]);
				m_findRank->SetFlagGameEnd(true); //Rankクラスにゲームが終了したことを伝える
			}

			//ゲームシーン終了
			m_gameEnd = true;
			m_findMainProcessing->SetGameEnd(m_gameEnd);
		}
	}

}

//////////////////////////////
// プレイヤーの操作処理
//////////////////////////////

void Player::Controller(const int playerNum)
{
	//スタート時のカウントダウンが終わっているか
	if (m_findGameStartCountdown->GetFlagStopGameProcess() == true) {
		return;
	}

	//操作できる状態か
	if (m_stopController[playerNum] == true) {
		return;
	}

	//ジャンプアニメーション中か
	if (m_flagAnimationJump[playerNum] == true) {
		return;
	}

	//操作処理

	//プレイヤーの操作
	if (m_maxPlayer > playerNum) {
		ControllerPlayer(playerNum);
	}
	//CPUの操作
	else {
		ControllerCPU(playerNum);
	}
}

void Player::ControllerPlayer(const int playerNum)
{
	//２マス進む
	if (g_pad[playerNum]->IsTrigger(enButtonA) == true) {
		//移動処理
		if (m_findStage->MoveBlock(playerNum, con::MOVE_2) == false) {
			//移動不可
			return;
		}

		//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
		m_seJump->Play(false); //再生
		m_modelCharacter[playerNum]->PlayAnimation(jump); //アニメーション
		m_flagAnimationJump[playerNum] = true; //ジャンプアニメーションをする
	}
	//１マス進む
	else if (g_pad[playerNum]->IsTrigger(enButtonB) == true) {
		//移動処理
		if (m_findStage->MoveBlock(playerNum, con::MOVE_1) == false) {
			//移動不可
			return;
		}

		//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
		m_seJump->Play(false); //再生
		m_modelCharacter[playerNum]->PlayAnimation(jump); //アニメーション
		m_flagAnimationJump[playerNum] = true; //ジャンプアニメーションをする
	}
}

void Player::ControllerCPU(const int playerNum)
{
	//CPUコントローラーによる操作の取得
	const int CPUMove = m_findCPUPlayerController->CPUController(playerNum);

	//２マス進む
	if (CPUMove == con::MOVE_2) {
		//移動処理
		if (m_findStage->MoveBlock(playerNum, con::MOVE_2) == false) {
			//移動不可
			return;
		}

		//CPUコントローラーの操作を停止する
		m_findCPUPlayerController->SetStopController(playerNum, true);

		//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
		m_seJump->Play(false); //再生
		m_modelCharacter[playerNum]->PlayAnimation(jump); //アニメーション
		m_flagAnimationJump[playerNum] = true; //ジャンプアニメーションをする
	}
	//１マス進む
	else if (CPUMove == con::MOVE_1) {
		//移動処理
		if (m_findStage->MoveBlock(playerNum, con::MOVE_1) == false) {
			//移動不可
			return;
		}

		//CPUコントローラーの操作を停止する
		m_findCPUPlayerController->SetStopController(playerNum, true);

		//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
		m_seJump->Play(false); //再生
		m_modelCharacter[playerNum]->PlayAnimation(jump); //アニメーション
		m_flagAnimationJump[playerNum] = true; //ジャンプアニメーションをする
	}
}

//////////////////////////////
// プレイヤーのアニメーション
//////////////////////////////

void Player::Animation(const int playerNum)
{
	//アニメーションの処理が止められているか
	if (m_flagStopAnimation[playerNum] == true) {
		return;
	}

	//ジャンプアニメーション
	JumpAnimation(playerNum);

	//ブロックごとのアニメーション
	switch (m_nowAnimationBlock[playerNum]) {
	//緑ブロック
	case con::greenBlock:
		// do nothing
		break;
	//青ブロック
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
	//黄色ブロック
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
	//ゴールブロック
	case con::goalBlock:
		// do nothing
		break;
	}
}

void Player::JumpAnimation(const int playerNum)
{
	//ジャンプアニメーション中か
	if (m_flagAnimationJump[playerNum] == false) {
		return;
	}

	++m_timerAnimation[playerNum]; //タイマーのカウント

	//位置の上昇
	if (m_timerAnimation[playerNum] >= JUMP_UP_ANIMATION_START &&
		m_timerAnimation[playerNum] <= JUMP_UP_ANIMATION_END) {
		m_modelCharacter[playerNum]->UpPositionY(JUMP_MOVE); //モデルの上昇
	}
	//位置の下降
	else if (m_timerAnimation[playerNum] >= JUMP_DOWN_ANIMATION_START &&
		m_timerAnimation[playerNum] <= JUMP_DOWN_ANIMATION_END) {
		m_modelCharacter[playerNum]->DownPositionY(JUMP_MOVE); //モデルの下降
	}

	//タイマーを超える
	if (m_timerAnimation[playerNum] >= TIME_ANIMATION) {
		m_flagAnimationJump[playerNum] = false; //ジャンプアニメーションをしない状態にする
		m_timerAnimation[playerNum] = con::TIME_RESET_ZERO; //タイマーをリセット

		//着地したブロックの判定
		m_findStage->CheckBlock(playerNum);

		//ゴール時のアニメーション
		GoalAnimation(playerNum);
	}
}

void Player::GoalAnimation(const int playerNum)
{
	//ゴールしていない場合
	if (m_findRank->GetDoingGoal(playerNum) == false) {
		m_modelCharacter[playerNum]->PlayAnimation(idle); //アニメーション
	}
	//ゴールした場合
	else {
		//１位
		if (m_goalRanking[playerNum] == con::rank_1) {
			m_modelCharacter[playerNum]->PlayAnimation(win); //アニメーション
		}
		//４位
		else if (m_goalRanking[playerNum] == con::rank_4) {
			m_modelCharacter[playerNum]->PlayAnimation(lose); //アニメーション
		}
		//２、３位
		else {
			m_modelCharacter[playerNum]->PlayAnimation(stand); //アニメーション
		}
	}
}

void Player::BlueBlockAnimation(const int playerNum)
{
	//落下アニメーションを再生
	m_modelCharacter[playerNum]->PlayAnimation(fall);

	++m_timerAnimation[playerNum]; //タイマーのカウント

	//モデルの降下
	m_modelCharacter[playerNum]->DownPositionY(BLUE_BLOCK_ANIMATION_DOWN_POSITION);

	//タイマーを超える
	if (m_timerAnimation[playerNum] >= TIME_BLUE_BLOCK_ANIMATION) {
		m_timerAnimation[playerNum] = con::TIME_RESET_ZERO; //タイマーをリセット

		//モデルを元に戻す
		m_modelCharacter[playerNum]->ResetPositionY(); //位置
		m_modelCharacter[playerNum]->PlayAnimation(idle); //アニメーション

		//キャラクターの位置を前の位置に戻す
		m_findStage->ReturnBlock(playerNum);

		//操作可能にする
		m_stopController[playerNum] = false;

		m_nowAnimationBlock[playerNum] = con::greenBlock; //ブロックに応じたアニメーションの状態
	}
}

void Player::SuddenDeathBlueBlockAnimation(const int playerNum)
{
	//落下アニメーションを再生
	m_modelCharacter[playerNum]->PlayAnimation(fall);

	++m_timerAnimation[playerNum]; //タイマーのカウント

	//モデルの降下
	m_modelCharacter[playerNum]->DownPositionY(BLUE_BLOCK_ANIMATION_DOWN_POSITION);

	//タイマーを超える
	if (m_timerAnimation[playerNum] >= TIME_BLUE_BLOCK_ANIMATION) {
		m_modelCharacter[playerNum]->Deactivate(); //非表示
		//アニメーション処理をしないようにする
		m_flagStopAnimation[playerNum] = true;
	}
}

void Player::YellowBlockAnimation(const int playerNum)
{
	//こけたアニメーションを再生
	m_modelCharacter[playerNum]->PlayAnimation(srip);

	++m_timerAnimation[playerNum]; //タイマーのカウント

	//タイマーを超える
	if (m_timerAnimation[playerNum] >= TIME_YELLOW_BLOCK_ANIMATION) {
		m_timerAnimation[playerNum] = con::TIME_RESET_ZERO;

		//モデルを元に戻す
		m_modelCharacter[playerNum]->PlayAnimation(idle); //アニメーション
		//操作可能にする
		m_stopController[playerNum] = false;

		m_nowAnimationBlock[playerNum] = con::greenBlock; //ブロックに応じたアニメーションの状態
	}
}

void Player::SuddenDeathYellowBlockAnimation(const int playerNum)
{
	//こけたアニメーションを再生
	m_modelCharacter[playerNum]->PlayAnimation(srip);

	++m_timerAnimation[playerNum]; //タイマーのカウント

	//タイマーを超える
	if (m_timerAnimation[playerNum] >= TIME_YELLOW_BLOCK_ANIMATION) {
		//アニメーション処理をしないようにする
		m_flagStopAnimation[playerNum] = true;
	}
}

void Player::NextRound()
{
	//プレイヤーのモデルを元に戻す
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->PlayAnimation(win); //アニメーション
		m_modelCharacter[playerNum]->PlayAnimation(idle); //アニメーション
		m_modelCharacter[playerNum]->ResetPositionY(); //Y座標の位置
		m_modelCharacter[playerNum]->Activate(); //表示
	}

	//ゴールしたプレイヤーを０にする
	m_goalPlayer = 0;

	//以下、メンバ変数をリセット
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_activePlayer[playerNum] = true;
		m_goalRanking[playerNum] = con::rank_notClear;
		m_flagAnimationJump[playerNum] = false;
		m_timerAnimation[playerNum] = 0;
		m_flagStopAnimation[playerNum] = false;
		m_stopController[playerNum] = false;
		m_nowAnimationBlock[playerNum] = con::greenBlock;
	}
}

////////////////////////////////////////////////////////////
// サドンデスモードの関数
////////////////////////////////////////////////////////////

void Player::SuddenDeathRank()
{
	int Ranking = con::rank_1; //順位
	bool checkAddRank = false; //次の取得ラウンド数のプレイヤーの順位が変わるか

	//取得ラウンド数が多い順に高い順位にする
	for (int roundPointNum = 3; roundPointNum >= con::FIRST_ELEMENT_ARRAY; roundPointNum--) {
		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
			//順位を確定
			if (m_roundPoint[playerNum] == roundPointNum) {
				m_goalRanking[playerNum] = Ranking;
				checkAddRank = true;
			}
		}

		//次のプレイヤーの順位のために順位を増加する
		if (checkAddRank == true) {
			++Ranking; //順位を変更
			checkAddRank = false;
		}
	}
}