#include "stdafx.h"
#include "title.h"

#include "constant.h"



namespace //constant
{
	////////////////////////////////////////////////////////////
	// 初期化
	////////////////////////////////////////////////////////////

	const Vector2 INIT_SPRITE_PRESS_A_BUTTON_POSITION = { 0.0f,-200.0f };
	const float INIT_SPRITE_PRESS_A_BUTTON_MUL_COLOR_W = 0.0f;


	////////////////////////////////////////////////////////////
	// Press_A_Buttonの点滅処理に使用する定数
	////////////////////////////////////////////////////////////

	const float CHANGE_MUL_COLOR_W = 0.02f; //透明度の変化量

	const float MINIMUM_MUL_COLOR_W = 0.0f; //透明度の最小値
	const float MAXIMUM_MUL_COLOR_W = 0.8f; //透明度の最大値
}



Title::Title()
{
	//////////////////////////////
	// NewGO
	//////////////////////////////

	//////////
	// タイトルアイコンのNewGO
	//////////

	m_spriteTitle = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteTitle->Init(filePath::dds::TITLE);
	m_spriteTitle->Deactivate();

	//////////
	// Press_A_ButtonのNewGO
	//////////

	m_spritePressAButton = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spritePressAButton->Init(filePath::dds::PRESS_A_BUTTON);
	m_spritePressAButton->Deactivate();

	//////////
	// SEのNewGO
	//////////

	m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seDecision->Init(filePath::se::DECISION);
}

Title::~Title()
{
	//////////
	// タイトルアイコンのDeleteGO
	//////////

	DeleteGO(m_spriteTitle);

	//////////
	// Press_A_ButtonのDeleteGO
	//////////

	DeleteGO(m_spritePressAButton);

	//////////
	// SEのDeleteGO
	//////////

	DeleteGO(m_seDecision);
}

bool Title::Start()
{
	

	return true;
}

void Title::Init()
{
	m_flagProcess = true;

	//////////
	// タイトルアイコンの初期化
	//////////

	m_spriteTitle->Activate();

	//////////
	// Press_A_Buttonの初期化
	//////////

	m_spritePressAButton->Activate();
	m_spritePressAButton->SetPosition(INIT_SPRITE_PRESS_A_BUTTON_POSITION);
	m_spritePressAButton->SetMulColorW(INIT_SPRITE_PRESS_A_BUTTON_MUL_COLOR_W);

	//////////
	// メンバ変数の初期化
	//////////

	m_flagFinish = false; //このクラスでするべき処理が終わったか
	m_flagBlinking = true; //m_pressAButtonの透過処理に使用
}

void Title::Finish()
{
	m_flagProcess = false;

	//////////
	// タイトルアイコンの非表示
	//////////

	m_spriteTitle->Deactivate();

	//////////
	// Press_A_Buttonの非表示
	//////////

	m_spritePressAButton->Deactivate();
}

void Title::Update()
{
	if (false == m_flagProcess) {
		return;
	}

	//決定
	if (g_pad[con::player_1]->IsTrigger(enButtonA) == true) {
		m_seDecision->Play(false);
		m_flagFinish = true;
	}

	//Press_A_Buttonの点滅処理
	if (m_flagBlinking == true) { //透明になる処理
		m_spritePressAButton->SetMulColorW(m_spritePressAButton->GetMulColorW() - CHANGE_MUL_COLOR_W);

		if (m_spritePressAButton->GetMulColorW() <= MINIMUM_MUL_COLOR_W) {
			m_flagBlinking = false;
		}
	}
	else { //実体になる処理
		m_spritePressAButton->SetMulColorW(m_spritePressAButton->GetMulColorW() + CHANGE_MUL_COLOR_W);

		if (m_spritePressAButton->GetMulColorW() >= MAXIMUM_MUL_COLOR_W) {
			m_flagBlinking = true;
		}
	}
}