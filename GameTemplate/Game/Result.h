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
	void Init();
	void Finish();
	void Update() override final;
	
	/**
	 * @brief 結果の表示
	*/
	void ResultDisplay();

	/**
	 * @brief 結果順位の表示
	*/
	void SelectDisplay();

	/**
	 * @brief リザルト画面での選択肢の選択処理
	*/
	void ResultSelect();

	/**
	 * @brief リザルトシーンでの処理が終わった際にする処理
	*/
	void FinishResult();


public://Get関数
	/**
	 * @brief 処理が終わったかを取得
	 * @return 処理が終わったか
	*/
	const bool GetFlagFinish()
	{
		return m_flagFinish;
	}

	/**
	 * @brief 選択していた選択肢の番号を取得
	 * @return 選択していた選択肢の番号
	*/
	const bool GetSelect()
	{
		return m_cursorPosition;
	}

	const int GetCursorPosition()
	{
		return m_cursorPosition;
	}




public://Set関数
	/**
	 * @brief プレイヤーの順位をセット
	 * @param playNum プレイヤー番号
	 * @param rank 順位
	*/
	void SetRank(const int playNum, const int rank)
	{
		m_rank[playNum] = rank;
	}


private: //enum
	enum class DisplayStatus
	{
		result,
		commandSelect,
		finish,
		DisplayStatusMax
	};

	DisplayStatus m_displayStatus = DisplayStatus::result;


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
	ModelRender* m_modelCharacter[con::PlayerNumberMax] = { nullptr };	//プレイヤーキャラクターのモデル
	SpriteRender* m_spriteGoalRank[con::PlayerNumberMax] = { nullptr };	//ゴール順位を表示する
	SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };	//選択肢UI表示
	SpriteRender* m_spriteBackground =  nullptr ;
	SpriteRender* m_spritePressANext = nullptr;
	SoundSE* m_seDecision = nullptr;
	SoundSE* m_seMoveCursor = nullptr;

	int m_rank[con::PlayerNumberMax] = { con::GoalRankMax,con::GoalRankMax,con::GoalRankMax,con::GoalRankMax };

	bool m_flagDecision = false;    //決定したかのフラグ
	int m_cursorPosition = 0; //カーソルの場所
	bool m_flagFinish = false;      //このクラスでするべき処理が終わったか
	bool m_flagBlinking = true; //m_pressAButtonの透過処理に使用
};

