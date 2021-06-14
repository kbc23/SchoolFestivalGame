#pragma once
#include "model_render.h"
#include "font_render.h"

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

	void Update() override final;

	////////////////////////////////////////////////////////////
    // プレイヤーの操作処理
    ////////////////////////////////////////////////////////////

	/**
	 * @brief プレイヤーの操作処理の土台
	 * @param pNum プレイヤー番号
	*/
	void Controller(const int pNum);

	////////////////////////////////////////////////////////////
	// プレイヤーのアニメーション
	////////////////////////////////////////////////////////////

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


public: //Set関数
	/**
	 * @brief プレイヤーのモデルをX座標を軸に回転させる。
	 * @param pNum プレイヤー番号
	 * @param f 回転量
	*/
	void SetRotationX(const int pNum, float f) {
		m_modelRender[pNum]->SetRotationX(f);
	}

	/**
	 * @brief プレイヤーが操作できるかどうかのSet関数
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
		m_goalRankFont[pNum]->SetText(rank);
		m_goalRankFont[pNum]->Activate();
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



public: //enum
	//プレイヤーの番号
	enum PlayerNumber
	{
		player_1,
		player_2,
		player_3,
		player_4,
		PlayerNumberMax
	};



private: //data menber
	Stage* m_stage = nullptr;

	//プレイヤーが最大４人だから、メンバ変数は要素数４の配列で管理する。
	//ModelRender* m_modelRender = nullptr;
	ModelRender* m_modelRender[PlayerNumberMax] = { nullptr }; //モデルレンダー
	FontRender* m_goalRankFont[PlayerNumberMax] = { nullptr }; //フォントレンダー

	//Vector3 m_position[PlayerNumberMax]; //キャラクターの座標
	//Quaternion m_rotation[PlayerNumberMax] = { Quaternion::Identity }; //キャラクターの回転情報

	bool m_activePlayer[PlayerNumberMax] = { true, true, true, true }; //プレイヤーが操作できるかどうか

	int m_maxPlayer = PlayerNumberMax; //プレイヤーの最大数
	//※プレイヤー数選択画面を作成したら、そこからこの変数にプレイヤーの最大数を代入する。

	int m_goalRanking[PlayerNumberMax] = { 0, 0, 0, 0 }; //プレイヤーのゴール順位
	bool m_flagGoal[PlayerNumberMax] = { false, false, false, false }; //ゴールしたか

	bool m_flagAnimationJump[PlayerNumberMax] = { false, false, false, false }; //ジャンプアニメーション中か
	//bool m_flagDoingAnimation[PlayerNumberMax] = { false, false, false, false }; //アニメーション中か
	int m_timerAnimation[PlayerNumberMax] = { 0, 0, 0, 0 }; //アニメーションのタイマー
};