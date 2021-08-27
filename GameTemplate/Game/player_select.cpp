#include "stdafx.h"
#include "player_select.h"

#include "constant.h"
#include "player.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////

    constexpr const wchar_t* FONT = L"プレイする人数を選択してください";
    const Vector2 FONT_POSITION = { -450.0f,300.0f };
    const float FONT_SCALE = 1.3f;


    ////////////////////////////////////////////////////////////
    // 位置情報
    ////////////////////////////////////////////////////////////

    const Vector2 CHOICES_POSITION[4] = {
        {-300.0f,0.0f},
        {-100.0f,0.0f},
        {100.0f,0.0f},
        {300.0f,0.0f}
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
    //////////////////////////////
    // NewGO
    //////////////////////////////

    //////////
    // UIのNewGO
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteChoices[spriteNum]->Init(filePath::dds::NUMBER_OF_PLAYERS[spriteNum]);
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // フォントのNewGO
    //////////

    m_font = NewGO<FontRender>(igo::PRIORITY_FONT);
    m_font->Init(FONT, FONT_POSITION, FONT_SCALE);
    m_font->Deactivate();

    //////////
    // SEのNewGO
    //////////

    m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seDecision->Init(filePath::se::DECISION);
    m_seMoveCursor = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seMoveCursor->Init(filePath::se::MOVE_CURSOR);
}

PlayerSelect::~PlayerSelect()
{
    //////////
    // UIのDeleteGO
    //////////

    for (int choicesNum = con::FIRST_ELEMENT_ARRAY; choicesNum < m_NUMBER_OF_CHOICES; choicesNum++) {
        DeleteGO(m_spriteChoices[choicesNum]);
    }

    //////////
    // フォントのDeleteGO
    //////////

    DeleteGO(m_font);

    //////////
    // SEのDeleteGO
    //////////

    DeleteGO(m_seDecision);
    DeleteGO(m_seMoveCursor);
}

bool PlayerSelect::Start()
{
    //////////////////////////////
    // FindGO
    //////////////////////////////

    m_findPlayer = FindGO<Player>(igo::CLASS_NAME_PLAYER);

    return true;
}

void PlayerSelect::Init()
{
    m_flagProcess = true;

    //////////
    // UIの初期化
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum]->Activate();
        m_spriteChoices[spriteNum]->SetPosition(CHOICES_POSITION[spriteNum]);

        if (con::FIRST_ELEMENT_ARRAY == spriteNum) {
            m_spriteChoices[spriteNum]->SetMulColor(SRns::COLOR_NORMAL);
        }
        else {
            m_spriteChoices[spriteNum]->SetMulColor(SRns::COLOR_GRAY);
        }
    }

    //////////
    // フォントの初期化
    //////////

    m_font->Activate();

    //////////
    // メンバ変数の初期化
    //////////

    m_cursorPosition = 0;       //カーソルの場所
    m_numberOfPlayer = 0;       //プレイヤーの人数
    m_flagDecision = false;     //人数を決定したかのフラグ
    m_flagFinish = false;       //このクラスでするべき処理が終わったか
    m_flagDrawMove = true;      //UIが上下に移動する処理に使用するフラグ
}

void PlayerSelect::Finish()
{
    m_flagProcess = false;

    //////////
    // UIの非表示
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // フォントの初期化
    //////////

    m_font->Deactivate();
}

void PlayerSelect::Update()
{
    if (false == m_flagProcess) {
        return;
    }

    SelectTheNumberOfPlayers();

    if (m_flagDecision == true && m_flagFinish == false) {
        FinishPlayerSelect();
    }

    DrawMove();
}

void PlayerSelect::SelectTheNumberOfPlayers()
{
    //決定
    if (g_pad[con::player_1]->IsTrigger(enButtonA)) {
        m_seDecision->Play(false);

        m_flagDecision = true;
    }
    //左に移動
    else if (g_pad[con::player_1]->IsTrigger(enButtonLeft) == true) {
        m_seMoveCursor->Play(false);

        if (m_cursorPosition == LEFT_END) { //左端ならカーソル移動しない
            return;
        }

        //前に選択していたものを選択していない状態にする
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);
        m_spriteChoices[m_cursorPosition]->SetPositionY(0.0f);

        --m_cursorPosition; //カーソルの移動

        //新しく選択していたものを選択している状態にする
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_flagDrawMove = true;
    }
    //右に移動
    else if (g_pad[con::player_1]->IsTrigger(enButtonRight) == true) {
        m_seMoveCursor->Play(false);

        if (m_cursorPosition == RIGHT_END) { //右端ならカーソル移動しない
            return;
        }

        //前に選択していたものを選択していない状態にする
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);
        m_spriteChoices[m_cursorPosition]->SetPositionY(0.0f);

        ++m_cursorPosition; //カーソルの移動

        //新しく選択していたものを選択している状態にする
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_flagDrawMove = true;
    }
}

void PlayerSelect::FinishPlayerSelect()
{
    m_findPlayer->SetMaxPlayer(m_cursorPosition + ADD_ONE);

    m_flagFinish = true;
}

void PlayerSelect::DrawMove()
{
    if (m_flagDrawMove == true) { //選択しているUIが上に移動する処理
        m_spriteChoices[m_cursorPosition]->SetPositionY(m_spriteChoices[m_cursorPosition]->GetPosition().y + 0.5f);
        if (m_spriteChoices[m_cursorPosition]->GetPosition().y >= 10.0f) {
            m_spriteChoices[m_cursorPosition]->SetPositionY(10.0f);
            m_flagDrawMove = false;
        }
    }
    else { //選択しているUIが下に移動する処理
        m_spriteChoices[m_cursorPosition]->SetPositionY(m_spriteChoices[m_cursorPosition]->GetPosition().y - 0.5f);
        if (m_spriteChoices[m_cursorPosition]->GetPosition().y <= -10.0f) {
            m_spriteChoices[m_cursorPosition]->SetPositionY(-10.0f);
            m_flagDrawMove = true;
        }
    }
}