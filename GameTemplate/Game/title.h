#pragma once
#include "sprite_render.h"
#include "sound_SE.h"

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override final;
	void Init();
	void Finish();
	void Update() override final;


public: //Get関数
	const bool GetFlagFinish() const
	{
		return m_flagFinish;
	}


private: //data menber
	////////////////////////////////////////////////////////////
	// クラスのオブジェクト
	////////////////////////////////////////////////////////////

	SpriteRender* m_spriteTitle = nullptr;
	SpriteRender* m_spritePressAButton = nullptr;
	SoundSE* m_seDecision = nullptr;

	////////////////////////////////////////////////////////////
	// フラグ関連
	////////////////////////////////////////////////////////////

	bool m_flagFinish = false; //このクラスでするべき処理が終わったか
	bool m_flagBlinking = true; //m_pressAButtonの透過処理に使用
};