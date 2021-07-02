#include "stdafx.h"
#include "player_select.h"

#include "constant.h"
#include "game.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // 位置情報
    ////////////////////////////////////////////////////////////

    const Vector3 CHOICES_POSITION[4] = {
        {-300.0f,0.0f,0.0f},
        {-100.0f,0.0f,0.0f},
        {100.0f,0.0f,0.0f},
        {300.0f,0.0f,0.0f}
    };

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    const int ADD_ONE = 1;      //プレイ人数の値を渡すときの数合わせ

    const int LEFT_END = 0;     //左端
    const int RIGHT_END = 3;    //右端
}



PlayerSelect::PlayerSelect()
{

}

PlayerSelect::~PlayerSelect()
{
    for (int choicesNum = con::FIRST_OF_THE_ARRAY; choicesNum < m_NUMBER_OF_CHOICES; choicesNum++) {
        DeleteGO(m_spriteChoices[choicesNum]);
    }

}

bool PlayerSelect::Start()
{

    m_spriteChoices[0] = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
    m_spriteChoices[0]->Init(filePath::DDS_NUMBER_OF_PLAYERS_1);
    m_spriteChoices[0]->SetPosition(CHOICES_POSITION[0]);
    m_spriteChoices[1] = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
    m_spriteChoices[1]->Init(filePath::DDS_NUMBER_OF_PLAYERS_2);
    m_spriteChoices[1]->SetPosition(CHOICES_POSITION[1]);
    m_spriteChoices[1]->SetMulColor(srName::COLOR_GRAY);
    m_spriteChoices[2] = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
    m_spriteChoices[2]->Init(filePath::DDS_NUMBER_OF_PLAYERS_3);
    m_spriteChoices[2]->SetPosition(CHOICES_POSITION[2]);
    m_spriteChoices[2]->SetMulColor(srName::COLOR_GRAY);
    m_spriteChoices[3] = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
    m_spriteChoices[3]->Init(filePath::DDS_NUMBER_OF_PLAYERS_4);
    m_spriteChoices[3]->SetPosition(CHOICES_POSITION[3]);
    m_spriteChoices[3]->SetMulColor(srName::COLOR_GRAY);


    m_game = FindGO<Game>(igo::CLASS_NAME_GAME);

    return true;
}

void PlayerSelect::Update()
{
    SelectTheNumberOfPlayers();

    if (m_flagDecision == true && m_flagFinish == false) {
        FinishPlayerSelect();
    }
}

void PlayerSelect::SelectTheNumberOfPlayers()
{
    if (g_pad[con::player_1]->GetLStickXF() == con::FLOAT_ZERO) {
        m_flagInput = false;
    }

    //前フレームから入力し続けていたら処理をしない。
    if (m_flagInput == true) {
        return;
    }

    //決定
    if (g_pad[con::player_1]->IsTrigger(enButtonA)) {
        m_flagDecision = true;
    }
    //左に移動
    else if (g_pad[con::player_1]->GetLStickXF() < con::FLOAT_ZERO) {
        if (m_cursorPosition == LEFT_END) {
            return;
        }

        m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_GRAY);

        --m_cursorPosition;

        m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_NORMAL);

        m_flagInput = true;
    }
    //右に移動
    else if (g_pad[con::player_1]->GetLStickXF() > con::FLOAT_ZERO) {
        if (m_cursorPosition == RIGHT_END) {
            return;
        }

        m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_GRAY);

        ++m_cursorPosition;

        m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_NORMAL);

        m_flagInput = true;
    }
}

void PlayerSelect::FinishPlayerSelect()
{
    m_game->SetMaxPlayer(m_cursorPosition + ADD_ONE);

    m_flagFinish = true;
}