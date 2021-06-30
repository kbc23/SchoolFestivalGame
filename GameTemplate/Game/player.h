#pragma once
#include "model_render.h"
#include "font_render.h"
#include "constant.h"

class Game;
class Stage;

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

	//////////////////////////////
	// ラウンド変更の処理
	//////////////////////////////

	/**
	 * @brief ラウンド変更の処理
	*/
	void NextRound();


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

public: //Set関数
	/**
	 * @brief m_modelRender[pNum]のX軸の回転量を設定するSet関数
	 * @param pNum プレイヤー番号
	 * @param f 回転量
	*/
	void SetRotationX(const int pNum, float f) {
		m_modelRender[pNum]->SetRotationX(f);
	}

	/**
	 * @brief m_activePlayer[pNum]のSet関数
	 * @param pNum プレイヤー番号
	 * @param b 操作できるかどうか
	*/
	void SetActivePlayer(const int pNum, const bool b) {
		m_activePlayer[pNum] = b;
	}

	/**
	 * @brief ゴール時の順位を確定
	 * @param pNum プレイヤー番号
	 * @param rank 順位
	*/
	void SetGoalRanking(const int pNum, const int rank) {
		m_goalRanking[pNum] = rank;

		SetAndActivateGoalRankFont(pNum, rank);
	}

	/**
	 * @brief 順位の描画処理ための値の設定と描画状態にする。
	 * @param pNum プレイヤー番号
	 * @param rank 順位
	*/
	void SetAndActivateGoalRankFont(const int pNum, const int rank)
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
	void SetFlagGoal(const int pNum, const bool b)
	{
		m_flagGoal[pNum] = b;
	}

	/**
	 * @brief 操作するプレイヤーの人数を保存する変数のSet関数
	 * @param i 操作するプレイヤーの人数
	*/
	void SetMaxPlayer(const int i)
	{
		m_maxPlayer = i;
	}


private:
	enum AnimationEnum
	{
		Animation_idle,
		//Animation_jump,
		Animation_Max
	};



private: //data menber
	////////////////////////////////////////////////////////////
	// クラスのオブジェクト
	////////////////////////////////////////////////////////////

	Stage* m_stage = nullptr;
	Game* m_game = nullptr;

	ModelRender* m_modelRender[con::playerNumberMax] = { nullptr };	//プレイヤーキャラクターのモデル
	FontRender* m_fontGoalRank[con::playerNumberMax] = { nullptr };	//ゴール順位を表示するフォント
	FontRender* m_fontEnd = nullptr;							//「終了！」を表示するフォント

	AnimationClip m_animationPlayer[Animation_Max];

	////////////////////////////////////////////////////////////
	// プレイヤー情報
	////////////////////////////////////////////////////////////

	int m_activePlayer[con::playerNumberMax] = { true, true, true, true };	//このプレイヤーは操作しているか
	int m_maxPlayer = con::playerNumberMax;									//プレイヤーの最大数

	int m_goalRanking[con::playerNumberMax] = { 0, 0, 0, 0 };				//プレイヤーのゴール順位
	bool m_flagGoal[con::playerNumberMax] = { false, false, false, false };	//ゴールしたか
	int m_goalPlayer = 0;												//ゴールしたプレイヤーの人数

	////////////////////////////////////////////////////////////
	// タイマー関連
	////////////////////////////////////////////////////////////

	bool m_flagAnimationJump[con::playerNumberMax] = { false, false, false, false };	//ジャンプアニメーション中か
	int m_timerAnimation[con::playerNumberMax] = { 0, 0, 0, 0 };						//アニメーションのタイマー
};