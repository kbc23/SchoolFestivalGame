#pragma once
#include "model_render.h"
#include "sprite_render.h"
#include "font_render.h"
#include "sound_SE.h"
#include "constant.h"

class MainProcessing;
class Stage;
class CPUPlayerController;
class SuddenDeathMode;
class GameStartCountdown;
class Result;
class Rank;

class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override final;
	void Init();
	void Finish();

	////////////////////////////////////////////////////////////
	// 毎フレームの処理
	////////////////////////////////////////////////////////////

	void Update() override final;

	//////////////////////////////
    // プレイヤーの操作処理
	//////////////////////////////

	/**
	 * @brief キャラクターの操作処理
	 * @param pNum プレイヤー番号
	*/
	void Controller(const int playerNum);

	/**
	 * @brief プレイヤーの操作処理
	 * @param playerNum プレイヤー番号
	*/
	void ControllerPlayer(const int playerNum);

	/**
	 * @brief CPUの操作処理
	 * @param playerNum プレイヤー番号
	*/
	void ControllerCPU(const int playerNum);

	//////////////////////////////
	// プレイヤーのアニメーション
	//////////////////////////////

	/**
	 * @brief プレイヤーのアニメーションの処理
	 * @param pNum プレイヤー番号
	*/
	void Animation(const int playerNum);

	/**
	 * @brief ジャンプ時のアニメーション
	 * @param pNum プレイヤー番号
	*/
	void JumpAnimation(const int playerNum);

	/**
	 * @brief 青色のブロックの上に行ったときのアニメーション
	 * @param playerNum プレイヤー番号
	*/
	void BlueBlockAnimation(const int playerNum);

	/**
	 * @brief 青色のブロックの上に行ったとき、サドンデス時のアニメーション
	 * @param playerNum プレイヤー番号
	*/
	void SuddenDeathBlueBlockAnimation(const int playerNum);

	/**
	 * @brief 黄色のブロックの上に行ったときのアニメーション
	 * @param playerNum プレイヤー番号
	*/
	void YellowBlockAnimation(const int playerNum);

	/**
	 * @brief 青色のブロックの上に行ったとき、サドンデス時のアニメーション
	 * @param playerNum プレイヤー番号
	*/
	void SuddenDeathYellowBlockAnimation(const int playerNum);

	////////////////////////////////////////////////////////////
	// ラウンド変更の処理
	////////////////////////////////////////////////////////////

	/**
	 * @brief ラウンド変更の処理
	*/
	void NextRound();


private: //追加
	void GoalAnimation(const int playerNum);


public: //Get関数
	/**
	 * @brief m_activePlayer[pNum]のGet関数
	 * @param playerNum プレイヤー番号
	 * @return プレイヤー番号[pNum]が、操作できるかどうか
	*/
	const bool GetActivePlayer(const int playerNum) const
	{
		return m_activePlayer[playerNum];
	}

	/**
	 * @brief m_flagAnimationJump[pNum]のGet関数
	 * @param playerNum プレイヤー番号
	 * @return プレイヤー番号[pNum]が、ジャンプアニメーションを再生しているかどうか
	*/
	const bool GetmFlagAnimationJump(const int playerNum) const
	{
		return m_flagAnimationJump[playerNum];
	}

	/**
	 * @brief サドンデスモードが終了したかを取得
	 * @return サドンデスモードが終了したか
	*/
	const bool GetFinishSuddenDeath() const
	{
		return m_finishSuddenDeath;
	}

	/**
	 * @brief キャラクターのモデルが描画されているかを取得
	 * @param playerNum プレイヤー番号
	 * @return キャラクターのモデルが描画されているか
	*/
	const bool GetModelIsActive(const int playerNum) const
	{
		return m_modelCharacter[playerNum]->IsActive();
	}

	/**
	 * @brief プレイヤーが操作できない状態かを取得
	 * @param playerNum プレイヤー番号
	 * @return プレイヤーが操作できない状態か
	*/
	const bool GetStopController(const int playerNum) const
	{
		return m_stopController[playerNum];
	}



public: //Set関数

	/**
	 * @brief プレイヤーモデルのY座標を下げる
	 * @param playerNum Y座標を下げるプレイヤー番号
	 * @param downPosition 下げる量
	*/
	void DownPositionY(const int playerNum, const float downPosition)
	{
		m_modelCharacter[playerNum]->DownPositionY(downPosition);
	}

	/**
	 * @brief プレイヤーモデルのY座標を0.0fにリセットする
	 * @param playerNum Y座標を0.0fにリセットするプレイヤー番号
	*/
	void ResetPositionY(const int playerNum)
	{
		m_modelCharacter[playerNum]->ResetPositionY();
	}

	/**
	 * @brief プレイヤーモデルのX軸の回転量を変更する
	 * @param playerNum プレイヤー番号
	 * @param rotation 回転量
	*/
	void SetRotationX(const int playerNum, const float rotation)
	{
		m_modelCharacter[playerNum]->SetRotationX(rotation);
	}

	/**
	 * @brief m_activePlayer[pNum]のSet関数
	 * @param playerNum プレイヤー番号
	 * @param flagActive 操作できるかどうか
	*/
	void SetActivePlayer(const int playerNum, const bool flagActive)
	{
		m_activePlayer[playerNum] = flagActive;
	}

	/**
	 * @brief 操作するプレイヤーの人数を保存する変数のSet関数
	 * @param maxPlayer 操作するプレイヤーの人数
	*/
	void SetMaxPlayer(const int maxPlayer)
	{
		m_maxPlayer = maxPlayer;
	}

	/**
	 * @brief 何人ゴールしたかをセット
	 * @param goalPlayer 何人ゴールしたか
	*/
	void SetGoalPlayer(const int goalPlayer)
	{
		m_goalPlayer = goalPlayer;
	}

	/**
	 * @brief プレイヤーのモデルのアニメーションを[idle]に設定する
	 * @param playerNum プレイヤー番号
	*/
	void SetAnimationIdle(const int playerNum)
	{
		m_modelCharacter[playerNum]->PlayAnimation(idle);
	}

	/**
	 * @brief プレイヤーのモデルのアニメーションを[fall]に設定し、SEを再生する
	 * @param playerNum プレイヤー番号
	*/
	void SetAnimationFall(const int playerNum)
	{
		m_modelCharacter[playerNum]->PlayAnimation(fall);
		m_seFall->Play(false);
	}

	/**
	 * @brief プレイヤーのモデルのアニメーションを[srip]に設定し、SEを再生する
	 * @param playerNum プレイヤー番号
	*/
	void SetAnimationSrip(const int playerNum)
	{
		m_modelCharacter[playerNum]->PlayAnimation(srip);
		m_seSrip->Play(false);
	}

	/**
	 * @brief プレイヤーのモデルのアニメーションを[win]に設定する
	 * @param playerNum プレイヤー番号
	*/
	void SetAnimationWin(const int playerNum)
	{
		m_modelCharacter[playerNum]->PlayAnimation(win);
	}

	/**
	 * @brief 取得ラウンド数を増加する。３ラウンド取得した場合、サドンデスモードを終了する。
	 * @param playerNum プレイヤー番号
	*/
	//void AddRoundPoint(const int playerNum)
	//{
	//	++m_roundPoint[playerNum];

	//	//３ラウンド取得したとき
	//	if (m_roundPoint[playerNum] == 3) {
	//		//サドンデスモード終了
	//		m_finishSuddenDeath = true;
	//	}
	//}

	/**
	 * @brief プレイヤーのコントローラーの操作可能状態を変更
	 * @param playerNum プレイヤー番号
	 * @param stopController コントローラーが操作可能か
	*/
	void SetStopController(const int playerNum, const bool stopController)
	{
		m_stopController[playerNum] = stopController;
	}

	/**
	 * @brief プレイヤーのモデルのブロック依存のアニメーション再生状況を変更する
	 * @param playerNum プレイヤー番号
	 * @param block ブロックの種類
	*/
	void SetNowAnimationBlock(const int playerNum, const con::BlockData& block)
	{
		m_nowAnimationBlock[playerNum] = block;

		//SEの再生
		if (block == con::BlockData::blueBlock) {
			m_seFall->Play(false); //落下時のSEを再生
		}
		if (block == con::BlockData::yellowBlock) {
			m_seSrip->Play(false); //スリップ時のSEを再生
		}
	}


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
		AnimationMax
	};

	AnimationClip m_animationPlayer[AnimationMax];


private: //data menber
	////////////////////////////////////////////////////////////
	// クラスのオブジェクト
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// NewGO
	//////////////////////////////

	ModelRender* m_modelCharacter[con::PlayerNumberMax] = { nullptr };	//プレイヤーキャラクターのモデル
	SoundSE* m_seJump = nullptr; //ジャンプ時のSE
	SoundSE* m_seFall = nullptr; //落下時のSE
	SoundSE* m_seSrip = nullptr; //スリップ時のSE

	//////////////////////////////
	// FindGO
	//////////////////////////////

	Stage* m_findStage = nullptr;
	MainProcessing* m_findMainProcessing = nullptr;
	CPUPlayerController* m_findCPUPlayerController = nullptr;
	SuddenDeathMode* m_findSuddenDeathMode = nullptr;
	GameStartCountdown* m_findGameStartCountdown = nullptr;
	Result* m_findResult = nullptr;
	Rank* m_findRank = nullptr;

	////////////////////////////////////////////////////////////
	// プレイヤー情報
	////////////////////////////////////////////////////////////

	int m_activePlayer[con::PlayerNumberMax] = { true, true, true, true };	//このプレイヤーは操作しているか
	int m_maxPlayer = con::PlayerNumberMax;									//プレイヤーの最大数

	int m_goalPlayer = 0;				
	
	int m_endTimer = 0; //ゴールしてからの時間

	bool m_gameEnd = false; //ゴールしたプレイヤーの人数

	bool m_stopController[con::PlayerNumberMax] = { false,false,false,false }; //操作不能か
	
	con::BlockData m_nowAnimationBlock[con::PlayerNumberMax] =					//プレイヤーの現在のアニメーション
	{ con::greenBlock,con::greenBlock ,con::greenBlock ,con::greenBlock };

	bool m_flagStopAnimation[con::PlayerNumberMax] = { false,false,false,false }; //アニメーションの処理が止まっているか

	////////////////////////////////////////////////////////////
	// タイマー関連
	////////////////////////////////////////////////////////////

	bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//ジャンプアニメーション中か
	int m_timerAnimation[con::PlayerNumberMax] = { 0, 0, 0, 0 };						//アニメーションのタイマー

	////////////////////////////////////////////////////////////
	// サドンデスモードのメンバ変数
	////////////////////////////////////////////////////////////

	bool m_finishSuddenDeath = false;							//サドンデスモードが終了したか
};