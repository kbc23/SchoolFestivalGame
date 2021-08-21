#pragma once

class Stage;
class Score;
class Player;
class GameStartCountdown;
class CPUPlayerController;

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


public: //Set関数
	/**
	 * @brief サドンデスモードかどうかをセット
	 * @param flagSuddenDeathMode サドンデスモードかどうか
	*/
	void SetFlagSuddenDeathMode(const bool flagSuddenDeathMode)
	{
		m_flagSuddenDeathMode = flagSuddenDeathMode;
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

	////////////////////////////////////////////////////////////
	// フラグ
	////////////////////////////////////////////////////////////

	bool m_flagSuddenDeathMode = false; //サドンデスモードか

	bool m_finishSuddenDeath = false; //サドンデスモードが終了したか

	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////
};

