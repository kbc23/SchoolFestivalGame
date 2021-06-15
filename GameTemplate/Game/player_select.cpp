#include "stdafx.h"
#include "player_select.h"

#include "constant.h"

namespace
{
    const Vector2 FONT_POSITION[3] = {          //�t�H���g�̈ʒu���
        {-100.0f,0.0f},                             //�Q�l
        {0.0f,0.0f},                                //�R�l
        {100.0f,0.0f}                               //�S�l
    };

    const Vector2 CURSOR_POSITION[3] = {          //�t�H���g�̈ʒu���
        {-100.0f,-60.0f},                           //�Q�l
        {0.0f,-60.0f},                              //�R�l
        {100.0f,-60.0f}                             //�S�l
    };

    const wchar_t* FONT_DISPLAY[3] = {
        L"�Q�l",
        L"�R�l",
        L"�S�l"
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

    //�O�t���[��������͂������Ă����珈�������Ȃ��B
    if (m_flagInput == true) {
        return;
    }

    //����
    if (g_pad[0]->IsTrigger(enButtonA)) {
        m_flagDecision = true;
    }
    //���Ɉړ�
    else if (g_pad[0]->GetLStickXF() < con::FLOAT_ZERO) {
        if (m_cursorPosition == 0) {
            return;
        }

        --m_cursorPosition;

        m_fontCursor->SetPosition(CURSOR_POSITION[m_cursorPosition]);

        m_flagInput = true;
    }
    //�E�Ɉړ�
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