#pragma once
#include "model_render.h"
#include "sprite_render.h"
#include "font_render.h"
#include "constant.h"
class Game;
class Result : public IGameObject
{
public:
	Result();
	~Result();
	bool Start();
	bool StartIndividual(const int pNum);
	void DeleteIndividual(const int pNum);
	void Update();


	void Display();
	void AnimationDisplay();
	void SelectDisplay();


public://Get関数
	bool GetFlagFinish() {
		return m_getFlagFinish;
	}
public://Set関数
	void SetRank(int pNum, int rank) {
		m_rank[pNum] = rank;
	}
private:
	static const int m_NUMBER_OF_CHOICES = 4;     //選択肢の数
	enum AnimationEnum
	{
		Animation_idle,
		//Animation_jump,
		Animation_Max
	};
	Game* m_game = nullptr;
	ModelRender* m_modelRender[con::PlayerNumberMax] = { nullptr };	//プレイヤーキャラクターのモデル
	SpriteRender* m_spriteGoalRank[con::PlayerNumberMax] = { nullptr };	//ゴール順位を表示する
	SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };	//選択肢UI表示

	AnimationClip m_animationPlayer[Animation_Max];

	int m_rank[con::PlayerNumberMax] = { 0,0,0,0 };
	int m_getFlagFinish = false;//このクラスでするべき処理が終わっているか
};

