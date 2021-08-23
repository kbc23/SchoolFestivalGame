#pragma once

class Stage;
class Score;
class Player;
class GameStartCountdown;
class CPUPlayerController;
class Rank;

class SuddenDeathMode : public IGameObject
{
public:
	SuddenDeathMode();
	~SuddenDeathMode();
	bool Start() override final;
	void Init();
	void Finish();
	void Update() override final;



public:
	/**
	 * @brief 次のラウンドに移動する処理
	*/
	void NextRound();


public: //Get関数
	/**
	 * @brief サドンデスモードかどうか
	 * @return サドンデスモードかどうか
	*/
	const bool GetFlagSuddenDeathMode() const
	{
		return m_flagSuddenDeathMode;
	}

	/**
	 * @brief 次のラウンドに移行するかを取得
	 * @return 次のラウンドに移行するか
	*/
	const bool GetFlagNextRound() const
	{
		return m_flagNextRound;
	}


public: //Set関数
	/**
	 * @brief サドンデスモードかどうかをセット
	 * @param flagSuddenDeathMode サドンデスモードかどうか
	*/
	void SetFlagSuddenDeathMode(const bool flagSuddenDeathMode)
	{
		m_flagSuddenDeathMode = flagSuddenDeathMode;
	}

	/**
	 * @brief 次のラウンドに移行するかをセット
	 * @param flag 次のラウンドに移行するか
	*/
	void SetFlagNextRound(const bool flag)
	{
		m_flagNextRound = flag;
	}


private: //data member
	////////////////////////////////////////////////////////////
	// クラスのオブジェクト
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// FindGO
	//////////////////////////////

	Stage* m_findStage = nullptr;
	Score* m_findScore = nullptr;
	Player* m_findPlayer = nullptr;
	GameStartCountdown* m_findGameStartCountdown = nullptr;
	CPUPlayerController* m_findCPUPlayerController = nullptr;
	Rank* m_findRank = nullptr;

	////////////////////////////////////////////////////////////
	// フラグ
	////////////////////////////////////////////////////////////

	bool m_flagSuddenDeathMode = false; //サドンデスモードか

	bool m_finishSuddenDeath = false; //サドンデスモードが終了したか

	bool m_flagNextRound = false; //次のラウンドに移行するか

	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////

	int m_timerNextRound = 0; //次のラウンドに移行するときのタイマー
};