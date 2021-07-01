#include "stdafx.h"
#include "player_select.h"

#include "constant.h"
#include "game.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // 位置情報
    ////////////////////////////////////////////////////////////

    const Vector2 FONT_POSITION[3] = {          //フォントの位置情報
        {-100.0f,0.0f},                             //２人
        {0.0f,0.0f},                                //３人
        {100.0f,0.0f}                               //４人
    };

    const Vector2 CURSOR_POSITION[3] = {        //フォントの位置情報
        {-100.0f,-60.0f},                           //２人
        {0.0f,-60.0f},                              //３人
        {100.0f,-60.0f}                             //４人
    };

    const wchar_t* FONT_DISPLAY[3] = {          //フォントの表示内容
        L"2人",
        L"3人",
        L"4人"
    };

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    const int ADD_TWO = 2;      //プレイ人数の値を渡すときの数合わせ

    const int LEFT_END = 0;     //左端
    const int RIGHT_END = 2;    //右端
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

        --m_cursorPosition;

        m_fontCursor->SetPosition(CURSOR_POSITION[m_cursorPosition]);

        m_flagInput = true;
    }
    //右に移動
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