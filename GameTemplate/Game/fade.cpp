#include "stdafx.h"
#include "fade.h"

#include "constant.h"

Fade::Fade()
{

}

Fade::~Fade()
{
    DeleteGO(m_spriteFade);
}

bool Fade::Start()
{
    m_spriteFade = NewGO<SpriteRender>(igo::PRIORITY_THIRD);
    m_spriteFade->Init(filePath::DDS_FADE);
    m_spriteFade->SetMulColorW(m_amountOfFade);

    return true;
}

void Fade::Update()
{
    ChangeAmountOfFade();
}

void Fade::ChangeAmountOfFade()
{
    if (m_flagFade == false) {
        return;
    }

    switch (m_stateFade) {
    case CurrentFadeState::increase:
        IncreaseFade();
        break;
    case CurrentFadeState::wait:
        WaitFade();
        break;
    case CurrentFadeState::decrease:
        DecreaseFade();
        break;
    default:
        MessageBoxA(nullptr, "フェードにてエラーが発生しました。", "エラー", MB_OK);
        EmergencyStop();
        return;
    }
}

void Fade::IncreaseFade()
{
    m_amountOfFade += m_increaseAmountOfChange;

    if (m_amountOfFade >= 1.0f) {
        m_amountOfFade = 1.0f;
        m_stateFade = CurrentFadeState::wait;
    }

    m_spriteFade->SetMulColorW(m_amountOfFade);
}

void Fade::WaitFade()
{
    ++m_waitTimer;

    if (m_waitTimer >= m_waitTime) {
        m_waitTimer = 0;
        m_stateFade = CurrentFadeState::decrease;
    }
}

void Fade::DecreaseFade()
{
    m_amountOfFade -= m_decreaseAmountOfChange;
    
    if (m_amountOfFade <= 0.0f) {
        m_amountOfFade = 0.0f;
        m_stateFade = CurrentFadeState::increase;
        m_flagFade = false;
    }

    m_spriteFade->SetMulColorW(m_amountOfFade);
}