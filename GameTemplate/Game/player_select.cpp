#include "stdafx.h"
#include "player_select.h"

#include "constant.h"

namespace
{
    const Vector2 FONT_POSITION[3] = {          //フォントの位置情報
        {-100.0f,0.0f},                             //２人
        {0.0f,0.0f},                                //３人
        {100.0f,0.0f}                               //４人
    };

    const Vector2 CURSOR_POSITION[3] = {          //フォントの位置情報
        {-100.0f,-60.0f},                           //２人
        {0.0f,-60.0f},                              //３人
        {100.0f,-60.0f}                             //４人
    };

    const wchar_t* FONT_DISPLAY[3] = {
        L"２人",
        L"３人",
        L"４人"
    };
}



PlayerSelect::PlayerSelect()
{

}

PlayerSelect::~PlayerSelect()
{
    for (int i = 0; i < 3; i++) {
        DeleteGO(m_fontNumberOfPlayer[i]);
    }

    DeleteGO(m_fontCursor);
}

bool PlayerSelect::Start()
{
    for (int i = 0; i < 3; i++) {
        m_fontNumberOfPlayer[i] = NewGO<FontRender>(0);
        m_fontNumberOfPlayer[i]->Init(FONT_DISPLAY[i], FONT_POSITION[i]);
    }

    m_fontCursor = NewGO<FontRender>(0);
    m_fontCursor->Init(L"^\n|", CURSOR_POSITION[0]);

    m_game = FindGO<Game>("game");

    return true;
}

void PlayerSelect::Update()
{
    SelectTheNumberOfPlayers();

    if (m_flagDecision == true && m_flagFinish == false) {
        Kari();
    }
}

void PlayerSelect::SelectTheNumberOfPlayers()
{
    if (g_pad[0]->GetLStickXF() == con::FLOAT_ZERO) {
        m_flagInput = false;
    }

    //前フレームから入力し続けていたら処理をしない。
    if (m_flagInput == true) {
        return;
    }

    //決定
    if (g_pad[0]->IsTrigger(enButtonA)) {
        m_flagDecision = true;
    }
    //左に移動
    else if (g_pad[0]->GetLStickXF() < con::FLOAT_ZERO) {
        if (m_cursorPosition == 0) {
            return;
        }

        --m_cursorPosition;

        m_fontCursor->SetPosition(CURSOR_POSITION[m_cursorPosition]);

        m_flagInput = true;
    }
    //右に移動
    else if (g_pad[0]->GetLStickXF() > con::FLOAT_ZERO) {
        if (m_cursorPosition == 2) {
            return;
        }

        ++m_cursorPosition;

        m_fontCursor->SetPosition(CURSOR_POSITION[m_cursorPosition]);

        m_flagInput = true;
    }
}

void PlayerSelect::Kari()
{
    m_game->SetActivePlayer(m_cursorPosition + 2);

    m_flagFinish = true;
}