#include "stdafx.h"
#include "rank.h"



namespace
{
	const Vector2 PLAYER_RANK_SPRITE[con::PlayerNumberMax] = {	//�v���C���[�̏��ʂ̏������W
		{ -490.0f, -70.0f },										//�v���C���[�P
		{ -180.0f, -70.0f },										//�v���C���[�Q
		{ 140.0f, -70.0f },											//�v���C���[�R
		{ 460.0f, -70.0f }											//�v���C���[�S
	};
}



Rank::Rank()
{
	//////////
	// �X�v���C�g��NewGO
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum] = NewGO<SpriteRender>(igo::PRIORITY_UI); //NewGO
			m_spriteGoalRank[playerNum][rankNum]->Init(filePath::dds::RANK[rankNum]); //������
			m_spriteGoalRank[playerNum][rankNum]->Deactivate(); //��\��
		}
	}
}

Rank::~Rank()
{
	//�X�v���C�g�̔�\��
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			DeleteGO(m_spriteGoalRank[playerNum][rankNum]);
		}
	}
}

bool Rank::Start()
{


	return true;
}

void Rank::Init()
{
    m_flagProcess = true;

	//////////
	// �X�v���C�g�̏�����
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->SetPosition(PLAYER_RANK_SPRITE[playerNum]);
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}
}

void Rank::Finish()
{
    m_flagProcess = false;

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}
}

void Rank::Update()
{
    if (false == m_flagProcess) {
        return;
    }
}

void Rank::NextRound()
{
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}
}