#include "stdafx.h"
#include "CPU_strength_select.h"

#include "constant.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // �ʒu���
    ////////////////////////////////////////////////////////////

    const Vector3 CHOICES_POSITION[3] = {
        {-300.0f,0.0f,0.0f},
        {0.0f,0.0f,0.0f},
        {300.0f,0.0f,0.0f}
    };

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    const int LEFT_END = 0;     //���[
    const int RIGHT_END = 2;    //�E�[
}



CPUStrengthSelect::CPUStrengthSelect()
{

}

CPUStrengthSelect::~CPUStrengthSelect()
{
    DeleteGO(m_spriteChoices[0]);
    DeleteGO(m_spriteChoices[1]);
    DeleteGO(m_spriteChoices[2]);
    DeleteGO(m_spriteBackground);
}

bool CPUStrengthSelect::Start()
{
    m_spriteBackground = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
    m_spriteBackground->Init(filePath::DDS_BACKGROUND);
    m_spriteChoices[0] = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
    m_spriteChoices[0]->Init(filePath::DDS_CPU_WEAK);
    m_spriteChoices[0]->SetPosition(CHOICES_POSITION[0]);
    m_spriteChoices[1] = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
    m_spriteChoices[1]->Init(filePath::DDS_CPU_USUALLY);
    m_spriteChoices[1]->SetPosition(CHOICES_POSITION[1]);
    m_spriteChoices[1]->SetMulColor(srName::COLOR_GRAY);
    m_spriteChoices[2] = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
    m_spriteChoices[2]->Init(filePath::DDS_CPU_STRONG);
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

        m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_GRAY);

        --m_cursorPosition;

        m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_NORMAL);

        m_flagInput = true;
    }
    //�E�Ɉړ�
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