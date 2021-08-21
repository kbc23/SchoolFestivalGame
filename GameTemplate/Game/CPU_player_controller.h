#pragma once
#include "model_render.h"
#include "constant.h"

class Stage;
class SuddenDeathMode;

class CPUPlayerController : public IGameObject
{
public:
	CPUPlayerController();
	~CPUPlayerController();
	bool Start() override final;
	void Init();
	void Finish();
	void Update() override final;

	/**
	 * @brief CPUの操作処理
	 * @param playerNum プレイヤー番号
	 * @return CPUが何ボタンの操作をしたか
	*/
	int CPUController(const int playerNum);

	/**
	 * @brief CPUのコントローラーの操作を停止する処理
	 * @param playerNum プレイヤー番号
	 * @return CPUのコントローラーの操作を停止するか
	*/
	bool StopController(const int playerNum);

	/**
	 * @brief CPUがミスをするときの操作
	 * @param playerNum プレイヤー番号
	 * @return CPUが何ボタンの操作をしたか
	*/
	const int& MissMove(const int playerNum);

	/**
	 * @brief CPUがミスをしないときの操作
	 * @param playerNum プレイヤー番号
	 * @return CPUが何ボタンの操作をしたか
	*/
	const int& OKMove(const int playerNum);



	void NextRound();



public: //Get関数

public://Set関数
	/**
	 * @brief CPUの強さを取得
	 * @param level CPUの強さ
	*/
	void SetCPULevel(const con::CPULevel& level)
	{
		m_CPULevel = level;
	}

	/**
	 * @brief 青色のブロックのところでミスをしたかを取得
	 * @param playerNum プレイヤー番号
	 * @param flagMiss 青色のブロックのところでミスをしたか
	*/
	void SetFlagBlueBlockMiss(int playerNum, bool flagMiss)
	{
		m_flagBlueBlockMiss[playerNum] = flagMiss;
	}

	/**
	 * @brief CPUのコントローラーの処理を停止するかを取得
	 * @param playerNum プレイヤー番号
	 * @param flagStop CPUのコントローラーの処理を停止するか
	*/
	void SetStopController(int playerNum, bool flagStop)
	{
		m_stopController[playerNum] = flagStop;
	}



private: //constant
	static const int m_MISS_INVALID = 3; //サドンデスモードのときにミスをしないようにする回数


private: //data menber
	////////////////////////////////////////////////////////////
	// クラスのオブジェクト
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// FindGO
	//////////////////////////////

	Stage* m_findStage = nullptr;
	SuddenDeathMode* m_findSuddenDeathMode = nullptr;

	////////////////////////////////////////////////////////////
	// フラグ関連
	////////////////////////////////////////////////////////////

	bool m_flagBlueBlockMiss[con::PlayerNumberMax] = { false,false,false,false }; //青ブロックでミスをしたか

	bool m_stopController[con::PlayerNumberMax] = { false,false,false,false }; //CPUのコントローラーの処理を止めるか

	int m_missInvalidCount[con::PlayerNumberMax] = 
	{ m_MISS_INVALID,m_MISS_INVALID,m_MISS_INVALID,m_MISS_INVALID }; //サドンデス時のミスを無効にする回数

	////////////////////////////////////////////////////////////
	// その他
	////////////////////////////////////////////////////////////

	con::CPULevel m_CPULevel = con::easy; //CPUの強さ
	int m_stopControllerTimer[con::PlayerNumberMax] = { 0,0,0,0 }; //CPUのコントローラーの処理を止めてるときのタイマー

};