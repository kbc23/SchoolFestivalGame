#include "stdafx.h"
#include "player_select.h"

#include "constant.h"
#include "game.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // �ʒu���
    ////////////////////////////////////////////////////////////

    const Vector2 FONT_POSITION[3] = {          //�t�H���g�̈ʒu���
        {-100.0f,0.0f},                             //�Q�l
        {0.0f,0.0f},                                //�R�l
        {100.0f,0.0f}                               //�S�l
    };

    const Vector2 CURSOR_POSITION[3] = {        //�t�H���g�̈ʒu���
        {-100.0f,-60.0f},                           //�Q�l
        {0.0f,-60.0f},                              //�R�l
        {100.0f,-60.0f}                             //�S�l
    };

    const wchar_t* FONT_DISPLAY[3] = {          //�t�H���g�̕\�����e
        L"2�l",
        L"3�l",
        L"4�l"
    };

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    const int ADD_TWO = 2;      //�v���C�l���̒l��n���Ƃ��̐����킹

    const int LEFT_END = 0;     //���[
    const int RIGHT_END = 2;    //�E�[
}



PlayerSelect::PlayerSelect()
{

}

PlayerSelect::~PlayerSelect()
{
    for (int fontNum = con::FIRST_OF_THE_ARRAY; fontNum < m_NUMBER_OF_CHOICES; fontNum++) {
        DeleteGO(m_fontNumberOfPlayer[fontNum]);
    }

    DeleteGO(m_fontCursor);
    DeleteGO(m_spriteBackground);
}

bool PlayerSelect::Start()
{
    for (int fontNum = con::FIRST_OF_THE_ARRAY; fontNum < m_NUMBER_OF_CHOICES; fontNum++) {
        m_fontNumberOfPlayer[fontNum] = NewGO<FontRender>(igo::PRIORITY_SECOND);
        m_fontNumberOfPlayer[fontNum]->Init(FONT_DISPLAY[fontNum], FONT_POSITION[fontNum]);
    }

    m_fontCursor = NewGO<FontRender>(igo::PRIORITY_SECOND);
    m_fontCursor->Init(L"^\n|", CURSOR_POSITION[LEFT_END]);

    m_spriteBackground = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
    m_spriteBackground->Init(filePath::DDS_BACKGROUND);

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

    //�O�t���[��������͂������Ă����珈�������Ȃ��B
    if (m_flagInput == true) {
        return;
    }

    //����
    if (g_pad[con::player_1]->IsTrigger(enButtonA)) {
        m_flagDecision = true;
    }
    //���Ɉړ�
    else if (g_pad[con::player_1]->GetLStickXF() < con::FLOAT_ZERO) {
        if (m_cursorPosition == LEFT_END) {
            return;
        }

        --m_cursorPosition;

        m_fontCursor->SetPosition(CURSOR_POSITION[m_cursorPosition]);

        m_flagInput = true;
    }
    //�E�Ɉړ�
    else if (g_pad[con::player_1]->GetLStickXF() > con::FLOAT_ZERO) {
        if (m_cursorPosition == RIGHT_END) {
            return;
        }

        ++m_cursorPosition;

        m_fontCursor->SetPosition(CURSOR_POSITION[m_cursorPosition]);

        m_flagInput = true;
    }
}

void PlayerSelect::FinishPlayerSelect()
{
    m_game->SetMaxPlayer(m_cursorPosition + ADD_TWO);

    m_flagFinish = true;
}