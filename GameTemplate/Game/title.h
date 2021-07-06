#pragma once
#include "sprite_render.h"
#include "sound_SE.h"

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override final;
	void Update() override final;

private:



public: //Get�֐�
	const bool& GetFlagFinish()
	{
		return m_flagFinish;
	}


private: //data menber
	////////////////////////////////////////////////////////////
	// �N���X�̃I�u�W�F�N�g
	////////////////////////////////////////////////////////////

	SpriteRender* m_spriteTitle = nullptr;
	SpriteRender* m_spritePressAButton = nullptr;
	SoundSE* m_seDecision = nullptr;

	////////////////////////////////////////////////////////////
	// �t���O�֘A
	////////////////////////////////////////////////////////////

	bool m_flagFinish = false; //���̃N���X�ł���ׂ��������I�������

};