#include "stdafx.h"
#include "player.h"

#include "game.h"
#include "EnemyAI.h"
#include "stage.h"
#include "rule1.h"

namespace //constant
{
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

	const int MOVE_BUTTON_A = 2;		//Aボタンを押したときの移動量
	const int MOVE_BUTTON_B = 1;		//Bボタンを押したときの移動量

	////////////////////////////////////////////////////////////
	// タイマー関連
	////////////////////////////////////////////////////////////

	const int TIMER_RESET = 0; //タイマーのリセット
	const int TIME_ANIMATION = 30; //ジャンプアニメーションの時間（0.5秒）

	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////

	const float JUMP_MOVE = 2.0f;
}



Player::Player()
{

}

////////////////////////////////////////////////////////////
// デストラクタ関連
////////////////////////////////////////////////////////////

Player::~Player()
{
	//プレイヤーごとに処理
	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		DeleteIndividual(playerNum);
	}

	DeleteGO(m_enemyAI);
	DeleteGO(m_fontEnd);
	DeleteGO(m_seJump);
	DeleteGO(m_seFall);
	DeleteGO(m_seSrip);
}



void Player::DeleteIndividual(const int pNum)
{
	//p_numはプレイヤーのコントローラー番号

	DeleteGO(m_modelRender[pNum]);
	DeleteGO(m_fontGoalRank[pNum]);

	//DeleteGO(m_skinModelRender[pNum]);
}

////////////////////////////////////////////////////////////
// Start関数関連
////////////////////////////////////////////////////////////

bool Player::Start()
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
	//m_animationPlayer[win].SetLoopFlag(true);
	m_animationPlayer[stand].SetLoopFlag(true);
	//m_animationPlayer[lose].SetLoopFlag(true);
	//アニメーションの設定
	//m_animationPlayer[Animation_jump].Load("Assets/animData/UnityChanJump.tka");
	//ループ再生をtrueにする
	//m_animationPlayer[Animation_jump].SetLoopFlag(true);

	//プレイヤーごとに処理
	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		bool check = StartIndividual(playerNum);

		//StartIndividual関数がfalseを返したらfalseを返して処理を終了させる。
		if (check == false) {
			return false;
		}
	}

	m_fontEnd = NewGO<FontRender>(igo::PRIORITY_FONT);
	m_fontEnd->Init(L"終了!");
	m_fontEnd->Deactivate();

	m_seJump = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seJump->Init(filePath::se::JUMP);
	m_seJump->SetVolume(0.3f);

	m_seFall = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seFall->Init(filePath::se::FALL);
	m_seFall->SetVolume(2.0f);

	m_seSrip = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seSrip->Init(filePath::se::SRIP);
	m_seSrip->SetVolume(0.7f);

	m_stage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
	m_game = FindGO<Game>(igo::CLASS_NAME_GAME);
	m_enemyAI = FindGO<EnemyAI>(igo::CLASS_NAME_ENEMYAI);//tuika
	m_rule1 = FindGO<Rule1>(igo::CLASS_NAME_RULE1);


	return true;
}

bool Player::StartIndividual(const int pNum)
{
	//p_numはプレイヤーのコントローラー番号

	m_modelRender[pNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
	m_modelRender[pNum]->Init(filePath::tkm::CHAEACTER_MODEL, modelUpAxis::enModelUpAxisY, m_animationPlayer, Animation_Max);
	m_modelRender[pNum]->SetPosition(PLAYER_START_POSITION[pNum]);
	m_modelRender[pNum]->SetScale({ 0.03f,0.03f,0.03f });
	m_modelRender[pNum]->PlayAnimation(idle);
	
	m_fontGoalRank[pNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
	m_fontGoalRank[pNum]->Init(L"", GOAL_RANK_FONT_POSITION[pNum]);
	m_fontGoalRank[pNum]->Deactivate();

	return true;
}

////////////////////////////////////////////////////////////
// 毎フレームの処理
////////////////////////////////////////////////////////////

void Player::Update()
{
	//プレイヤーごとに操作
	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		if (m_maxPlayer > playerNum) {
			if (m_flagGoal[playerNum] == false) {
				Controller(playerNum);
				Animation(playerNum);
			}
			else {
				Animation(playerNum);
			}
		}
		else {
			if (m_flagGoal[playerNum] == false) {
				m_enemyAI->Move(playerNum);
				if (m_EJumpFlag[playerNum] == true) {
					m_seJump->Play(false);
					m_modelRender[playerNum]->PlayAnimation(jump);
				}
				Animation(playerNum);
			}
			else {
				Animation(playerNum);
			}
		}

	}//henkou

	if (con::PlayerNumberMax == m_goalPlayer || m_finishSuddenDeath == true) {
		m_fontEnd->Activate();
		m_endTimer++;
		if (m_endTimer > 180) {
			//サドンデスモードのとき所持ラウンド勝利数に応じて順位を確定
			if (rule1NewGO == true) {
				SuddenDeathRank();
			}

			for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
				m_game->SetRank(playerNum, m_goalRanking[playerNum]);
			}

			m_gameEnd = true;
		}
	}//henkou

	bool check[4] = { false,false,false,false };

	bool check2[4] = { false,false,false,false };
	
	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < m_maxPlayer; playerNum++) {
		check[playerNum] = m_modelRender[playerNum]->IsInited();

		check2[playerNum] = m_modelRender[playerNum]->IsPlayingAnimation();
	}
}

//////////////////////////////
// プレイヤーの操作処理
//////////////////////////////

void Player::Controller(const int pNum)
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

	//２マス進む
	if (g_pad[pNum]->IsTrigger(enButtonA) == true) {
		if (m_stage->MoveBlock(pNum, MOVE_BUTTON_A) == false) {
			return;
		}
		//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
		m_seJump->Play(false);
		m_modelRender[pNum]->PlayAnimation(jump);
		m_flagAnimationJump[pNum] = true;
	}
	//１マス進む
	else if (g_pad[pNum]->IsTrigger(enButtonB) == true) {
		if (m_stage->MoveBlock(pNum, MOVE_BUTTON_B) == false) {
			return;
		}
		//キャラクターが移動したらアニメーションをジャンプアニメーションを再生
		m_seJump->Play(false);
		m_modelRender[pNum]->PlayAnimation(jump);
		m_flagAnimationJump[pNum] = true;
	}
}

//////////////////////////////
// プレイヤーのアニメーション
//////////////////////////////

void Player::Animation(const int pNum)
{
	JumpAnimation(pNum);

	ImpossibleOperationAnimation(pNum);
}

void Player::JumpAnimation(const int pNum)
{
	if (m_flagAnimationJump[pNum] == false) {
		return;
	}

	++m_timerAnimation[pNum];

	//ここのマジックナンバーを後で解消する。
	if (m_timerAnimation[pNum] >= 0 && m_timerAnimation[pNum] <= 15) {
		m_modelRender[pNum]->UpPositionY(JUMP_MOVE);
	}
	else if (m_timerAnimation[pNum] >= 16 && m_timerAnimation[pNum] <= 30) {
		m_modelRender[pNum]->DownPositionY(JUMP_MOVE);
	}


	if (m_timerAnimation[pNum] >= TIME_ANIMATION) {
		m_flagAnimationJump[pNum] = false;
		m_timerAnimation[pNum] = TIMER_RESET;

		if (m_flagGoal[pNum] == false) {
			m_modelRender[pNum]->PlayAnimation(idle);
		}
		else {
			if (m_goalRanking[pNum] == 1) {
				m_modelRender[pNum]->PlayAnimation(win);
			}
			else if (m_goalRanking[pNum] == 4) {
				m_modelRender[pNum]->PlayAnimation(lose);
			}
			else {
				m_modelRender[pNum]->PlayAnimation(stand);
			}
		}
	}
}

void Player::ImpossibleOperationAnimation(const int pNum)
{

}

void Player::NextRound()
{
	while (fontDeavtive<120)
	{
		fontDeavtive += 1;
	}
	
	if (fontDeavtive >= 120) {
		m_fontEnd->Deactivate();
		m_goalPlayer = 0;
		for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < m_maxPlayer; playerNum++) {
			m_flagGoal[playerNum] = false;
		}
		fontDeavtive = 0;
		for (int i = 0; i < con::PlayerNumberMax; i++) {
			m_fontGoalRank[i]->Deactivate();
		}
	}

	for (int i = 0; i < con::PlayerNumberMax; i++) {

		m_activePlayer[i] = true;	
		//m_maxPlayer = i;		
		m_goalRanking[i] = 0;
		m_flagGoal[i] = false;
		m_flagAnimationJump[i] = false;	
		m_timerAnimation[i] = 0;
	}
	m_goalPlayer = 0;												
	fontDeavtive = 0;
	m_goalPlayerZero = 0;
}

////////////////////////////////////////////////////////////
// サドンデスモードの関数
////////////////////////////////////////////////////////////

void Player::SuddenDeathRank()
{
	int Ranking = 1;

	for (int roundPointNum = 3; roundPointNum >= 0; roundPointNum--) {
		for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
			if (m_roundPoint[playerNum] == roundPointNum) {
				m_goalRanking[playerNum] = Ranking;
			}
		}

		++Ranking;
	}
}