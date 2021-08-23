#pragma once
#include "sprite_render.h"

class Fade : public IGameObject
{
public:
    Fade();
    ~Fade();
    bool Start() override final;
    void Update() override final;



private:
    /**
     * @brief �t�F�[�h�̏���
    */
    void ChangeAmountOfFade();

    /**
     * @brief �t�F�[�h�̗ʂ𑝉�
    */
    void FadeOut();

    /**
     * @brief �t�F�[�h�̗ʂ�����
    */
    void FadeIn();

public: //Get�֐�


public: //Set�֐�
    /**
     * @brief �t�F�[�h�A�E�g�̗ʂ�ύX
     * @param amountOfChangeFade �t�F�[�h�A�E�g�̗�
    */
    void SetAmountOfChangeFadeOut(const float amountOfChangeFade)
    {
        m_amountOfChangeFadeOut = amountOfChangeFade;
    }

    /**
     * @brief �t�F�[�h�C���̗ʂ�ύX
     * @param amountOfChangeFade �t�F�[�h�C���̗�
    */
    void SetAmountOfChangeFadeIn(const float amountOfChangeFade)
    {
        m_amountOfChangeFadeIn = amountOfChangeFade;
    }


public: //���̑�
    /**
     * @brief �t�F�[�h�A�E�g���n�߂�
    */
    void StartFadeOut()
    {
        m_statusFade = CurrentFadeState::fadeOut;
    }

    /**
     * @brief �t�F�[�h�C�����n�߂�
    */
    void StartFadeIn()
    {
        m_statusFade = CurrentFadeState::fadeIn;
    }

    /**
     * @brief ���݃t�F�[�h�����Ă��邩
     * @return ���݃t�F�[�h�����Ă��邩
    */
    const bool IsFadeProgress() const
    {
        if (m_statusFade == CurrentFadeState::wait) {
            return false;
        }
        else {
            return true;
        }
    }

    /**
     * @brief ���݃t�F�[�h�A�E�g�����Ă��邩
     * @return ���݃t�F�[�h�A�E�g�����Ă��邩
    */
    const bool IsFadeOutProgress() const
    {
        if (m_statusFade == CurrentFadeState::fadeOut) {
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @brief ���݃t�F�[�h�C�������Ă��邩
     * @return ���݃t�F�[�h�C�������Ă��邩
    */
    const bool IsFadeInProgress() const
    {
        if (m_statusFade == CurrentFadeState::fadeIn) {
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @brief �t�F�[�h�ً̋}��~
    */
    void EmergencyStop()
    {
        m_statusFade = CurrentFadeState::wait;
        m_amountOfFade = 0.0f;
        m_spriteFade->SetMulColorW(m_amountOfFade);
    }


private: //enum
    /**
     * @brief ���݂̃t�F�[�h�̏��
    */
    enum class CurrentFadeState
    {
        wait,                   //�ҋ@��
        fadeOut,                //�t�F�[�h�A�E�g��
        fadeIn,                 //�t�F�[�h�C����
        CurrentFadeStateMax
    };


private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    SpriteRender* m_spriteFade = nullptr; //�t�F�[�h�Ɏg�p����摜

    ////////////////////////////////////////////////////////////
    // �t�F�[�h�����̕ϐ�
    ////////////////////////////////////////////////////////////

    float m_amountOfFade = 0.0f;                //���݂̃t�F�[�h�̗�
    float m_amountOfChangeFadeOut = 0.1f;      //�t�F�[�h�̑����ʁi1�t���[���j
    float m_amountOfChangeFadeIn = 0.1f;      //�t�F�[�h�̌����ʁi1�t���[���j
    CurrentFadeState m_statusFade = CurrentFadeState::wait;    //���݂̃t�F�[�h�̏��

};