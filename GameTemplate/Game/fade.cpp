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
    //�t�F�[�h�Ɏg�p����摜�̏�����
    m_spriteFade = NewGO<SpriteRender>(igo::PRIORITY_FADE);
    m_spriteFade->Init(filePath::dds::FADE);
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
        MessageBoxA(nullptr, "�t�F�[�h�ɂăG���[���������܂����B", "�G���[", MB_OK);
        EmergencyStop();
        return;
    }
}

void Fade::FadeOut()
{
    //�t�F�[�h�A�E�g����
    m_amountOfFade += m_amountOfChangeFadeOut;

    //�t�F�[�h�A�E�g����������
    if (m_amountOfFade >= 1.0f) {
        m_amountOfFade = 1.0f;
        m_statusFade = CurrentFadeState::wait;
    }

    //�摜�̃t�F�[�h�ʂ�ύX
    m_spriteFade->SetMulColorW(m_amountOfFade);
}

void Fade::FadeIn()
{
    //�t�F�[�h�C������
    m_amountOfFade -= m_amountOfChangeFadeIn;
    
    //�t�F�[�h�C������������
    if (m_amountOfFade <= 0.0f) {
        m_amountOfFade = 0.0f;
        m_statusFade = CurrentFadeState::wait;
    }

    //�摜�̃t�F�[�h�ʂ�ύX
    m_spriteFade->SetMulColorW(m_amountOfFade);
}