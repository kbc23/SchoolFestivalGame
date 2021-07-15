#pragma once
#include "model_render.h"
#include "constant.h"
class Player;
class MainProcessing;
class Stage;
class EnemyAI : public IGameObject
{
public:
	EnemyAI();
	~EnemyAI();
	bool Start() override final;
	void Init() override final;
	void Finish() override final;
	void Update() override final;

	void Move(const int pNum);
	void Moverule1(const int pNum);

	void DifficultyMove(const int pNum);
	void DifficultyMoverule1(const int pNum);
	void AutoController1(const int pNum);
	void AutoControllerRule1(const int pNum); 
	void AutoController3(const int pNum);

public: //Get関数

public://Set関数
	//難易度受け取り
	void SetDifficultyLevel(const int& i) {
		m_difficultyLevel = i;
	}
	void SetmissInvalidCount(const int& pNum, const int& i) {
		m_missInvalidCount[pNum] = i;
	}
private:
	Player* m_player = nullptr;
	MainProcessing* m_game = nullptr;
	Stage* m_stage = nullptr;

	//定数
	static const int m_MOVE_2;		//移動量2
	static const int m_MOVE_1;		//移動量1

	static const int m_MISS_INVALID;//ミスを無効にする回数
	static const int m_RANDOM_ZERO_TO_NINE;//乱数を何で割るか
	static const int m_RAND_TIMES;//randの実行回数

	static const int m_MISSPLAY_EASY;//難易度よわいのミスプレイになる可能性のある行動をする割合
	static const int m_MISSPLAY_NORMAL;//難易度ふつうのミスプレイになる可能性のある行動をする割合
	static const int m_MISSPLAY_DIFFICULT;//難易度つよいのミスプレイになる可能性のある行動をする割合


	//変数
	int m_difficultyLevel = 0;//難易度0よわい1ふつう2つよい
	int m_randomNumber = 0;//乱数結果0～9、数値によってCPUの行動が決まる

	//cpu関係
	int m_missInvalidCount[con::PlayerNumberMax] = { 0,0,0,0 };//stopのカウント一定でstopがtrue
	bool m_activePlayer[con::PlayerNumberMax] = { false,false,false,false };	//CPU割り当てるか
	bool m_activeOperation[con::PlayerNumberMax] = { true, true, true, true };   //CPUが操作可能か
	bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//ジャンプアニメーション中か
	bool m_JumpFlag[con::PlayerNumberMax] = { false };//ジャンプフラグ	
	bool m_flagGoal[con::PlayerNumberMax] = { false, false, false, false };	//ゴールしたか
	bool m_bluemiss[con::PlayerNumberMax] = { false };//青ブロックでミスしたか	
};