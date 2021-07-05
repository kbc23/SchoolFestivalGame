#include "stdafx.h"
#include "CPU_strength_select.h"

#include "constant.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // 位置情報
    ////////////////////////////////////////////////////////////

    const Vector3 CHOICES_POSITION[3] = {
        {-300.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {300.0f,0.0f,0.0f}
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
}

bool CPUStrengthSelect::Start()
{
    m_spriteChoices[0] = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteChoices[0]->Init(filePath::dds::CPU_WEAK);
    m_spriteChoices[0]->SetPosition(CHOICES_POSITION[0]);
    m_spriteChoices[1] = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteChoices[1]->Init(filePath::dds::CPU_USUALLY);
    m_spriteChoices[1]->SetPosition(CHOICES_POSITION[1]);
    m_spriteChoices[1]->SetMulColor(srName::COLOR_GRAY);
    m_spriteChoices[2] = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteChoices[2]->Init(filePath::dds::CPU_STRONG);
    m_spriteChoices[2]->SetPosition(CHOICES_POSITION[2]);
    m_spriteChoices[2]->SetMulColor(srName::COLOR_GRAY);


    return true;
}

void CPUStrengthSelect::Update()
{
    SelectTheNumberOfCPUStrength();

    if (m_flagDecision == true && m_flagFinish == false) {
        FinishCPUStrengthSelect();
    }
}

void CPUStrengthSelect::SelectTheNumberOfCPUStrength()
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

void CPUStrengthSelect::FinishCPUStrengthSelect()
{
    m_flagFinish = true;
}