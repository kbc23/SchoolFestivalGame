#include "stdafx.h"
#include "title.h"

#include "constant.h"

Title::Title()
{

}

Title::~Title()
{
	DeleteGO(m_spriteBackground);
	DeleteGO(m_spritePressAButton);
}

bool Title::Start()
{
	m_spriteBackground = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
	m_spriteBackground->Init(filePath::DDS_BACKGROUND);
	m_spritePressAButton = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
	m_spritePressAButton->Init(filePath::DDS_PRESS_A_BUTTON);

	return true;
}

void Title::Update()
{
	if (g_pad[con::player_1]->IsTrigger(enButtonA) == true) {
		m_flagFinish = true;
	}
}