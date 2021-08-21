#include "stdafx.h"
#include "CPU_strength_select.h"

#include "main_processing.h"
#include "constant.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////

    constexpr const wchar_t* FONT = L"CPUの強さを選択してください";
    const Vector2 FONT_POSITION = { -400.0f,300.0f };
    const float FONT_SCALE = 1.3f;

    ////////////////////////////////////////////////////////////
    // 位置情報
    ////////////////////////////////////////////////////////////

    const Vector2 CHOICES_POSITION[3] = {
        {-300.0f,0.0f},
        {0.0f,0.0f},
        {300.0f,0.0f}
    };

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    const int LEFT_END = 0;     //左端
    const int RIGHT_END = 2;    //右端
}



CPUStrengthSelect::CPUStrengthSelect()
{
    //////////////////////////////
    // NewGO
    //////////////////////////////

    //////////
    // 選択肢のUIをNewGO
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteChoices[spriteNum]->Init(filePath::dds::CPU_STRENGTH[spriteNum]);
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // フォントをNewGO
    //////////

    m_font = NewGO<FontRender>(igo::PRIORITY_CLASS);
    m_font->Init(FONT, FONT_POSITION, FONT_SCALE);
    m_font->Deactivate();

    //////////
    // SEをNewGO
    //////////

    m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seDecision->Init(filePath::se::DECISION);
    m_seMoveCursor = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seMoveCursor->Init(filePath::se::MOVE_CURSOR);
}

CPUStrengthSelect::~CPUStrengthSelect()
{
    //////////
    // 選択肢のUIをDeleteGO
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        DeleteGO(m_spriteChoices[spriteNum]);
    }

    //////////
    // フォントをDeleteGO
    //////////

    DeleteGO(m_font);

    //////////
    // SEをDeleteGO
    //////////

    DeleteGO(m_seDecision);
    DeleteGO(m_seMoveCursor);
}

bool CPUStrengthSelect::Start()
{
    //////////////////////////////
    // FindGO
    //////////////////////////////

    m_game = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);

    return true;
}

void CPUStrengthSelect::Init()
{
    m_flagProcess = true;

    //////////
    // 選択肢のUIを初期化
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
    // フォントを初期化
    //////////

    m_font->Activate();

    //////////
    // メンバ変数を初期化
    //////////

    m_cursorPosition = 0;       //カーソルの場所
    m_numberOfPlayer = 0;       //プレイヤーの人数
    m_flagDecision = false;     //人数を決定したかのフラグ
    m_flagFinish = false;       //このクラスでするべき処理が終わったか
    m_flagDrawMove = true;      //UIが上下に移動する処理に使用するフラグ
}

void CPUStrengthSelect::Finish()
{
    m_flagProcess = false;

    //////////
    // 選択肢のUIを非表示
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // フォントを非表示
    //////////

    m_font->Deactivate();
}

void CPUStrengthSelect::Update()
{
    if (false == m_flagProcess) {
        return;
    }

    SelectTheNumberOfCPUStrength();

    if (m_flagDecision == true && m_flagFinish == false) {
        m_game->SetCPULevel(static_cast<con::CPULevel>(m_cursorPosition));
        FinishCPUStrengthSelect();
    }

    DrawMove();
}

void CPUStrengthSelect::SelectTheNumberOfCPUStrength()
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

        --m_cursorPosition; //カーソルを移動

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

        ++m_cursorPosition; //カーソルを移動

        //新しく選択していたものを選択している状態にする
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_flagDrawMove = true;
    }
}

void CPUStrengthSelect::FinishCPUStrengthSelect()
{
    m_flagFinish = true;
}

void CPUStrengthSelect::DrawMove()
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