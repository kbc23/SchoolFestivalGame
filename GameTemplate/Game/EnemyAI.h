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

	void DifficultyMove(const int eNum);
	void DifficultyMoverule1(const int eNum);
	void AutoController1(const int pNum);
	void AutoController11(const int pNum);
	//void AutoController2(const int pNum); 
	void AutoController3(const int pNum);



public: //Get関数

public://Set関数
	//難易度受け取り
	void SetDifficultyLevel(int i) {
		m_difficultyLevel = i;
	}
	//青ブロックでミスしたか受け取り
	void SetBlueMiss(bool i) {
		m_bluemiss = i;
	}
private:
	Player* m_player = nullptr;
	MainProcessing* m_game = nullptr;
	Stage* m_stage = nullptr;


	int m_difficultyLevel = 0;//難易度1簡単2普通3難しい
	int m_stopCount = 0;//stopのカウント一定でstopがtrue

	bool m_activePlayer[con::PlayerNumberMax] = { false,false,false,false };	//CPU割り当てるか
	int m_maxPlayer = 0;//プレイヤー最大数
	int m_addEnemyNumber = 2;//CPU用番号=2
	bool m_activeOperation[con::PlayerNumberMax] = { true, true, true, true };   //CPUが操作可能か
	bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//ジャンプアニメーション中か
	bool m_flagGoal[con::PlayerNumberMax] = { false, false, false, false };	//ゴールしたか
	bool m_bluemiss = false;//青ブロックでミスしたか

	bool m_JumpFlag[con::PlayerNumberMax] = { false };//ジャンプフラグ
	
};