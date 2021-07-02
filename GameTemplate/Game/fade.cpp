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
    m_spriteFade = NewGO<SpriteRender>(igo::PRIORITY_FADE);
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
    switch (m_statusFade) {
    case CurrentFadeState::wait:

        break;
    case CurrentFadeState::fadeOut:
        FadeOut();
        break;
    case CurrentFadeState::fadeIn:
        FadeIn();
        break;
    default:
        MessageBoxA(nullptr, "フェードにてエラーが発生しました。", "エラー", MB_OK);
        EmergencyStop();
        return;
    }
}

void Fade::FadeOut()
{
    m_amountOfFade += m_amountOfChangeFadeOut;

    if (m_amountOfFade >= 1.0f) {
        m_amountOfFade = 1.0f;
        m_statusFade = CurrentFadeState::wait;
    }

    m_spriteFade->SetMulColorW(m_amountOfFade);
}

void Fade::FadeIn()
{
    m_amountOfFade -= m_amountOfChangeFadeIn;
    
    if (m_amountOfFade <= 0.0f) {
        m_amountOfFade = 0.0f;
        m_statusFade = CurrentFadeState::wait;
    }

    m_spriteFade->SetMulColorW(m_amountOfFade);
}