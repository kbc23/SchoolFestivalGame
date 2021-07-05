#include "stdafx.h"
#include "mode_select.h"

#include "constant.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // 位置情報
    ////////////////////////////////////////////////////////////

    const Vector3 CHOICES_POSITION[2] = {
        {-150.0f,0.0f,0.0f},
        {150.0f,0.0f,0.0f}
    };

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    const int LEFT_END = 0;     //左端
    const int RIGHT_END = 1;    //右端
}



ModeSelect::ModeSelect()
{

}

ModeSelect::~ModeSelect()
{
    DeleteGO(m_spriteChoices[0]);
    DeleteGO(m_spriteChoices[1]);

    DeleteGO(m_fontChoices[0]);
    DeleteGO(m_fontChoices[1]);
    DeleteGO(m_font);

    DeleteGO(m_seDecision);
    DeleteGO(m_seMoveCursor);
}

bool ModeSelect::Start()
{
    m_spriteChoices[0] = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteChoices[0]->Init(filePath::dds::COMMAND_RACE);
    m_spriteChoices[0]->SetPosition(CHOICES_POSITION[0]);
    m_spriteChoices[1] = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteChoices[1]->Init(filePath::dds::COMMAND_RACE);
    m_spriteChoices[1]->SetPosition(CHOICES_POSITION[1]);
    m_spriteChoices[1]->SetMulColor(srName::COLOR_GRAY);

    m_fontChoices[0] = NewGO<FontRender>(igo::PRIORITY_FONT);
    m_fontChoices[0]->Init(L"誰が最初にゴールにたどり着けるかを競うレース!", { -500.0f,-200.0f });
    m_fontChoices[1] = NewGO<FontRender>(igo::PRIORITY_FONT);
    m_fontChoices[1]->Init(L"一回ミスをしたらそこで終了!ハラハラドキドキのサドンデス!", { -600.0f,-200.0f });
    m_fontChoices[1]->Deactivate();

    m_font = NewGO<FontRender>(igo::PRIORITY_FONT);
    m_font->Init(L"プレイするモードを選択してください", { -500.0f,300.0f }, 1.3f);
    

    m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seDecision->Init(filePath::se::DECISION);
    m_seMoveCursor = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seMoveCursor->Init(filePath::se::MOVE_CURSOR);

    return true;
}

void ModeSelect::Update()
{
    SelectTheNumberOfCPUStrength();

    if (m_flagDecision == true && m_flagFinish == false) {
        FinishCPUStrengthSelect();
    }
}

void ModeSelect::SelectTheNumberOfCPUStrength()
{
    //決定
    if (g_pad[con::player_1]->IsTrigger(enButtonA) == true) {
        m_seDecision->Play(false);

        m_flagDecision = true;
    }
    //左に移動
    else if (g_pad[con::player_1]->IsTrigger(enButtonLeft) == true) {
        m_seMoveCursor->Play(false);

        if (m_cursorPosition == LEFT_END) {
            return;
        }

        m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_GRAY);
        m_fontChoices[m_cursorPosition]->Deactivate();

        --m_cursorPosition;

        m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_NORMAL);
        m_fontChoices[m_cursorPosition]->Activate();
    }
    //右に移動
    else if (g_pad[con::player_1]->IsTrigger(enButtonRight) == true) {
        m_seMoveCursor->Play(false);

        if (m_cursorPosition == RIGHT_END) {
            return;
        }

        m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_GRAY);
        m_fontChoices[m_cursorPosition]->Deactivate();

        ++m_cursorPosition;

        m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_NORMAL);
        m_fontChoices[m_cursorPosition]->Activate();
    }
}

void ModeSelect::FinishCPUStrengthSelect()
{
    m_flagFinish = true;
}