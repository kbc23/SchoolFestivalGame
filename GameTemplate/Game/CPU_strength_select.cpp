#include "stdafx.h"
#include "CPU_strength_select.h"

#include "main_processing.h"
#include "constant.h"



namespace //constant
{
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

}

CPUStrengthSelect::~CPUStrengthSelect()
{
    DeleteGO(m_spriteChoices[0]);
    DeleteGO(m_spriteChoices[1]);
    DeleteGO(m_spriteChoices[2]);

    DeleteGO(m_font);

    DeleteGO(m_seDecision);
    DeleteGO(m_seMoveCursor);
}

bool CPUStrengthSelect::Start()
{
    m_spriteChoices[0] = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteChoices[0]->Init(filePath::dds::CPU_WEAK);
    m_spriteChoices[0]->Deactivate();

    m_spriteChoices[1] = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteChoices[1]->Init(filePath::dds::CPU_USUALLY);
    m_spriteChoices[1]->Deactivate();

    m_spriteChoices[2] = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteChoices[2]->Init(filePath::dds::CPU_STRONG);
    m_spriteChoices[2]->Deactivate();

    m_font = NewGO<FontRender>(igo::PRIORITY_CLASS);
    m_font->Init(L"CPUの強さを選択してください", { -400.0f,300.0f }, 1.3f);
    m_font->Deactivate();

    m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seDecision->Init(filePath::se::DECISION);
    m_seMoveCursor = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seMoveCursor->Init(filePath::se::MOVE_CURSOR);
    m_game = FindGO<MainProcessing>(igo::CLASS_NAME_GAME);
    return true;
}

void CPUStrengthSelect::Init()
{
    m_flagProcessing = true;

    m_spriteChoices[0]->SetPosition(CHOICES_POSITION[0]);
    m_spriteChoices[0]->SetMulColor(SRns::COLOR_NORMAL);
    m_spriteChoices[0]->Activate();

    m_spriteChoices[1]->SetPosition(CHOICES_POSITION[1]);
    m_spriteChoices[1]->SetMulColor(SRns::COLOR_GRAY);
    m_spriteChoices[1]->Activate();

    m_spriteChoices[2]->SetPosition(CHOICES_POSITION[2]);
    m_spriteChoices[2]->SetMulColor(SRns::COLOR_GRAY);
    m_spriteChoices[2]->Activate();

    m_font->Activate();

    m_cursorPosition = 0;       //カーソルの場所
    m_numberOfPlayer = 0;       //プレイヤーの人数
    m_flagDecision = false;    //人数を決定したかのフラグ
    m_flagFinish = false;      //このクラスでするべき処理が終わったか
    m_flagMove = true;
}

void CPUStrengthSelect::Finish()
{
    m_flagProcessing = false;

    m_spriteChoices[0]->Deactivate();
    m_spriteChoices[1]->Deactivate();
    m_spriteChoices[2]->Deactivate();

    m_font->Deactivate();
}

void CPUStrengthSelect::Update()
{
    if (m_flagProcessing == false) {
        return;
    }

    SelectTheNumberOfCPUStrength();

    if (m_flagDecision == true && m_flagFinish == false) {
        m_game->SetDiLevel(m_cursorPosition);
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

        if (m_cursorPosition == LEFT_END) {
            return;
        }

        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);
        m_spriteChoices[m_cursorPosition]->SetPositionY(0.0f);

        --m_cursorPosition;

        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_flagMove = true;
    }
    //右に移動
    else if (g_pad[con::player_1]->IsTrigger(enButtonRight) == true) {
        m_seMoveCursor->Play(false);

        if (m_cursorPosition == RIGHT_END) {
            return;
        }

        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);
        m_spriteChoices[m_cursorPosition]->SetPositionY(0.0f);

        ++m_cursorPosition;

        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_flagMove = true;
    }
}

void CPUStrengthSelect::FinishCPUStrengthSelect()
{
    m_flagFinish = true;
}

void CPUStrengthSelect::DrawMove()
{
    if (m_flagMove == true) {
        m_spriteChoices[m_cursorPosition]->SetPositionY(m_spriteChoices[m_cursorPosition]->GetPosition().y + 0.5f);
        if (m_spriteChoices[m_cursorPosition]->GetPosition().y >= 10.0f) {
            m_spriteChoices[m_cursorPosition]->SetPositionY(10.0f);
            m_flagMove = false;
        }
    }
    else {
        m_spriteChoices[m_cursorPosition]->SetPositionY(m_spriteChoices[m_cursorPosition]->GetPosition().y - 0.5f);
        if (m_spriteChoices[m_cursorPosition]->GetPosition().y <= -10.0f) {
            m_spriteChoices[m_cursorPosition]->SetPositionY(-10.0f);
            m_flagMove = true;
        }
    }
}