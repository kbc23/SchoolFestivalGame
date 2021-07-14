#pragma once
#include "model_render.h"
#include "sprite_render.h"
#include "font_render.h"
#include "sound_SE.h"
#include "constant.h"
class MainProcessing;
class Result : public IGameObject
{
public:
	Result();
	~Result();
	bool Start() override final;
	bool StartIndividual(const int pNum);
	void DeleteIndividual(const int pNum);
	void Init() override final;
	void InitIndividual(const int& pNum);
	void Finish() override final;
	void Update() override final;
	
	void SelectDisplay();

	void ResultSelect();

	void FinishResult();


public://Get関数
	bool& GetFlagFinish() {
		return m_flagFinish;
	}
	bool GetSelect() {
		return m_cursorPosition;
	}
public://Set関数
	void SetRank(const int& pNum, const int& rank) {
		m_rank[pNum] = rank;
	}
private:
	static const int m_NUMBER_OF_CHOICES = 4;     //選択肢の数
	enum AnimationEnum
	{
		
		win,
		stand,
		lose,
		Animation_Max
	};

	AnimationClip m_animationPlayer[Animation_Max];
	MainProcessing* m_game = nullptr;
	ModelRender* m_modelRender[con::PlayerNumberMax] = { nullptr };	//プレイヤーキャラクターのモデル
	SpriteRender* m_spriteGoalRank[con::PlayerNumberMax] = { nullptr };	//ゴール順位を表示する
	SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };	//選択肢UI表示
	SpriteRender* m_spriteBackground =  nullptr ;
	SpriteRender* m_spritePressANext = nullptr;
	SoundSE* m_seDecision = nullptr;
	SoundSE* m_seMoveCursor = nullptr;

	int m_rank[con::PlayerNumberMax] = { 0,0,0,0 };
	bool m_spriteChoicesNewGO = false;
	bool m_spriteChoicesNewGORE = false;

	bool m_flagDecision = false;    //決定したかのフラグ
	int m_cursorPosition = 0;//カーソルの場所
	bool m_flagFinish = false;      //このクラスでするべき処理が終わったか
	bool m_flagBlinking = true; //m_pressAButtonの透過処理に使用
};

