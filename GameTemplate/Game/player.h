#pragma once
#include "model_render.h"
#include "font_render.h"
#include "sound_SE.h"
#include "constant.h"

class Game;
class Stage;
class EnemyAI;//tuika



class Player : public IGameObject
{
public:
	Player();

	~Player();
	/**
	 * @brief デストラクタの処理をプレイヤーごとに個別でおこなう。
	 * @param pNum プレイヤー番号
	*/
	void DeleteIndividual(const int pNum);

	bool Start() override final;
	/**
	 * @brief Start関数の処理をプレイヤーごとに個別でおこなう。
	 * @param pNum プレイヤー番号
	 * @return 処理が正常に終了したかどうか
	*/
	bool StartIndividual(const int pNum);

	////////////////////////////////////////////////////////////
	// 毎フレームの処理
	////////////////////////////////////////////////////////////

	void Update() override final;

	//////////////////////////////
    // プレイヤーの操作処理
	//////////////////////////////

	/**
	 * @brief プレイヤーの操作処理の土台
	 * @param pNum プレイヤー番号
	*/
	void Controller(const int pNum);

	//////////////////////////////
	// プレイヤーのアニメーション
	//////////////////////////////

	/**
	 * @brief プレイヤーのアニメーションの処理
	 * @param pNum プレイヤー番号
	*/
	void Animation(const int pNum);

	/**
	 * @brief ジャンプ時のアニメーション
	 * @param pNum プレイヤー番号
	*/
	void JumpAnimation(const int pNum);

	/**
	 * @brief 操作不可時のアニメーション
	 * @param pNum プレイヤー番号
	*/
	void ImpossibleOperationAnimation(const int pNum);


public: //Get関数
	/**
	 * @brief m_activePlayer[pNum]のGet関数
	 * @param pNum プレイヤー番号
	 * @return プレイヤー番号[pNum]が、操作できるかどうか
	*/
	bool GetActivePlayer(const int pNum) {
		return m_activePlayer[pNum];
	}

	/**
	 * @brief m_flagAnimationJump[pNum]のGet関数
	 * @param pNum プレイヤー番号
	 * @return プレイヤー番号[pNum]が、ジャンプアニメーションを再生しているかどうか
	*/
	bool GetmFlagAnimationJump(const int pNum) {
		return m_flagAnimationJump[pNum];
	}

	/**
	 * @brief m_flagGoal[pNum]のGet関数
	 * @param pNum プレイヤー番号
	 * @return プレイヤーがゴールしたかどうか
	*/
	bool GetFlagGoal(const int pNum) {
		return m_flagGoal[pNum];
	}

	int GetTimerAnimation(const int pNum)
	{
		return m_timerAnimation[pNum];
	}

	bool GetFlagAnimationJump(const int pNum)
	{
		return m_flagAnimationJump[pNum];
	}

	bool GetGameEnd()//ゲーム終了
	{
		return m_gameEnd;
	}

	bool GetModel(int pNum)//ゲーム終了
	{
		return m_modelRender[pNum];
	}

public: //Set関数

	void DownPositionY(const int& pNum, const float& f)
	{
		m_modelRender[pNum]->DownPositionY(f);
	}

	void ResetPositionY(const int& pNum)
	{
		m_modelRender[pNum]->ResetPositionY();
	}

	/**
	 * @brief m_modelRender[pNum]のX軸の回転量を設定するSet関数
	 * @param pNum プレイヤー番号
	 * @param f 回転量
	*/
	void SetRotationX(const int& pNum, const float& f) {
		m_modelRender[pNum]->SetRotationX(f);
	}

	/**
	 * @brief m_activePlayer[pNum]のSet関数
	 * @param pNum プレイヤー番号
	 * @param b 操作できるかどうか
	*/
	void SetActivePlayer(const int& pNum, const bool& b) {
		m_activePlayer[pNum] = b;
	}

	/**
	 * @brief ゴール時の順位を確定
	 * @param pNum プレイヤー番号
	 * @param rank 順位
	*/
	void SetGoalRanking(const int& pNum, const int& rank) {
		m_goalRanking[pNum] = rank;

		SetAndActivateGoalRankFont(pNum, rank);
	}

	/**
	 * @brief 順位の描画処理ための値の設定と描画状態にする。
	 * @param pNum プレイヤー番号
	 * @param rank 順位
	*/
	void SetAndActivateGoalRankFont(const int& pNum, const int& rank)
	{
		m_fontGoalRank[pNum]->SetText(rank);
		m_fontGoalRank[pNum]->Activate();
		++m_goalPlayer;
	}

	/**
	 * @brief ゴールしたかどうかのSet関数
	 * @param pNum プレイヤー番号
	 * @param b ゴールしたかどうか
	*/
	void SetFlagGoal(const int& pNum, const bool& b)
	{
		m_flagGoal[pNum] = b;
	}

	/**
	 * @brief 操作するプレイヤーの人数を保存する変数のSet関数
	 * @param i 操作するプレイヤーの人数
	*/
	void SetMaxPlayer(const int& i)
	{
		m_maxPlayer = i;
	}

	void SetAnimationIdle(const int& pNum)
	{
		m_modelRender[pNum]->PlayAnimation(idle);
	}

	void SetAnimationFall(const int& pNum)
	{
		m_modelRender[pNum]->PlayAnimation(fall);
		m_seFall->Play(false);
	}

	void SetAnimationSrip(const int& pNum)
	{
		m_modelRender[pNum]->PlayAnimation(srip);
		m_seSrip->Play(false);
	}

	/**
* @brief cpuがジャンプ中か判定する変数のSet関数
* @param i ジャンプ中か真偽判定
*/
	void SetFlagAnimationJump(int pNum, const bool i)
	{
		m_flagAnimationJump[pNum] = i;
	}//tuika

private: //enum
	enum AnimationEnum
	{
		idle,
		jump,
		fall,
		srip,
		win,
		stand,
		lose,
		Animation_Max
	};

	AnimationClip m_animationPlayer[Animation_Max];



private: //data menber
	////////////////////////////////////////////////////////////
	// クラスのオブジェクト
	////////////////////////////////////////////////////////////

	Stage* m_stage = nullptr;
	Game* m_game = nullptr;
	EnemyAI* m_enemyAI = nullptr;//tuika

	ModelRender* m_modelRender[con::PlayerNumberMax] = { nullptr };	//プレイヤーキャラクターのモデル
	FontRender* m_fontGoalRank[con::PlayerNumberMax] = { nullptr };	//ゴール順位を表示するフォント
	FontRender* m_fontEnd = nullptr;							//「終了！」を表示するフォント
	SoundSE* m_seFall = nullptr;
	SoundSE* m_seSrip = nullptr;

	////////////////////////////////////////////////////////////
	// プレイヤー情報
	////////////////////////////////////////////////////////////

	int m_activePlayer[con::PlayerNumberMax] = { true, true, true, true };	//このプレイヤーは操作しているか
	int m_maxPlayer = con::PlayerNumberMax;									//プレイヤーの最大数

	int m_goalRanking[con::PlayerNumberMax] = { 0, 0, 0, 0 };				//プレイヤーのゴール順位
	bool m_flagGoal[con::PlayerNumberMax] = { false, false, false, false };	//ゴールしたか
	int m_goalPlayer = 0;					
	
	int m_endTimer = 0;//ゴールしてからの時間tuika

	bool m_gameEnd = false;//ゴールしたプレイヤーの人数tuika

	////////////////////////////////////////////////////////////
	// タイマー関連
	////////////////////////////////////////////////////////////

	bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//ジャンプアニメーション中か
	int m_timerAnimation[con::PlayerNumberMax] = { 0, 0, 0, 0 };						//アニメーションのタイマー
};