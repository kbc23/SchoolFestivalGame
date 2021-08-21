#include "stdafx.h"
#include "mode_select.h"

#include "sudden_death_mode.h"
#include "constant.h"


namespace //constant
{
    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////

    const Vector2 CHOICES_POSITION[2] = //UI画像の位置
    {
        {-150.0f,0.0f},
        {150.0f,0.0f}
    };

    const Vector2 FONT_CHOICES_POSITION[2] = //フォントの位置
    {
        { -500.0f,-200.0f },
        { -600.0f,-200.0f }
    };

    constexpr const wchar_t* FONT_CHOICES[2] =
    {
        L"誰が最初にゴールにたどり着けるかを競うレース!",
        L"一回ミスをしたらそこで終了!ハラハラドキドキのサドンデス!"
    };

    const Vector2 FONT_POSITION = { -500.0f, 300.0f };

    const float FONT_SCALE = 1.3f;

    constexpr const wchar_t* FONT = L"プレイするモードを選択してください";

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    const int LEFT_END = 0;     //左端
    const int RIGHT_END = 1;    //右端
}



ModeSelect::ModeSelect()
{
    //////////////////////////////
    // NewGO
    //////////////////////////////

    //////////
    // UIのNewGO
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteChoices[spriteNum]->Init(filePath::dds::MODE_UI[spriteNum]);
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // フォントのNewGO
    //////////

    //選択肢ごとのフォント
    for (int fontNum = con::FIRST_ELEMENT_ARRAY; fontNum < m_NUMBER_OF_CHOICES; fontNum++) {
        m_fontChoices[fontNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontChoices[fontNum]->Init(FONT_CHOICES[fontNum], FONT_CHOICES_POSITION[fontNum]);
        m_fontChoices[fontNum]->Deactivate();
    }

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

ModeSelect::~ModeSelect()
{
    //////////
    // UIのDeleteGO
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        DeleteGO(m_spriteChoices[spriteNum]);
    }

    //////////
    // フォントのDeleteGO
    //////////

    //選択肢ごとのフォント
    for (int fontNum = con::FIRST_ELEMENT_ARRAY; fontNum < m_NUMBER_OF_CHOICES; fontNum++) {
        DeleteGO(m_fontChoices[fontNum]);
    }

    DeleteGO(m_font);

    //////////
    // SEのDeleteGO
    //////////

    DeleteGO(m_seDecision);
    DeleteGO(m_seMoveCursor);
}

bool ModeSelect::Start()
{
    //////////////////////////////
    // FindGO
    //////////////////////////////

    m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);

    return true;
}

void ModeSelect::Init()
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

    //選択肢ごとのフォント
    for (int fontNum = con::FIRST_ELEMENT_ARRAY; fontNum < m_NUMBER_OF_CHOICES; fontNum++) {
        if (con::FIRST_ELEMENT_ARRAY == fontNum) { //１番最初のだけアクティブにする
            m_fontChoices[fontNum]->Activate();
        }
        else {
            m_fontChoices[fontNum]->Deactivate();
        }
    }

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

void ModeSelect::Finish()
{
    m_flagProcess = false;

    //////////
    // UIの非表示
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // フォントの非表示
    //////////

    //選択肢ごとのフォント
    for (int fontNum = con::FIRST_ELEMENT_ARRAY; fontNum < m_NUMBER_OF_CHOICES; fontNum++) {
        m_fontChoices[fontNum]->Deactivate();
    }

    m_font->Deactivate();
}

void ModeSelect::Update()
{
    if (false == m_flagProcess) {
        return;
    }

    SelectTheNumberOfCPUStrength();

    if (m_flagDecision == true && m_flagFinish == false) {
        FinishCPUStrengthSelect();
    }

    DrawMove();
}

void ModeSelect::SelectTheNumberOfCPUStrength()
{
    //決定
    if (g_pad[con::player_1]->IsTrigger(enButtonA) == true) {
        m_seDecision->Play(false);

        //レースモード
        if (m_cursorPosition == raceMode) {
            m_findSuddenDeathMode->SetFlagSuddenDeathMode(false);
        }
        //サドンデスモード
        else if (m_cursorPosition == suddenDeathMode) {
            m_findSuddenDeathMode->SetFlagSuddenDeathMode(true);
        }

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
        m_fontChoices[m_cursorPosition]->Deactivate();

        --m_cursorPosition; //カーソルを移動

        //新しく選択していたものを選択している状態にする
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_fontChoices[m_cursorPosition]->Activate();
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
        m_fontChoices[m_cursorPosition]->Deactivate();

        ++m_cursorPosition; //カーソルを移動

        //新しく選択していたものを選択している状態にする
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_fontChoices[m_cursorPosition]->Activate();
        m_flagDrawMove = true;
    }
}

void ModeSelect::FinishCPUStrengthSelect()
{
    m_flagFinish = true;
}

void ModeSelect::DrawMove()
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