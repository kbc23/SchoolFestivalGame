#pragma once
#include "sprite_render.h"

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override final;
	void Update() override final;

private:



public: //Get関数
	const bool& GetFlagFinish()
	{
		return m_flagFinish;
	}


private: //data menber
	////////////////////////////////////////////////////////////
	// クラスのオブジェクト
	////////////////////////////////////////////////////////////

	SpriteRender* m_spritePressAButton = nullptr;

	////////////////////////////////////////////////////////////
	// フラグ関連
	////////////////////////////////////////////////////////////

	bool m_flagFinish = false; //このクラスでするべき処理が終わったか

};