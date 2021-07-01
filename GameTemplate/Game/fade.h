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
    void IncreaseFade();

    /**
     * @brief �t�F�[�h�ŉ�ʂ��B������Ԃőҋ@
    */
    void WaitFade();

    /**
     * @brief �t�F�[�h�̗ʂ�����
    */
    void DecreaseFade();

public: //Get�֐�


public: //Set�֐�
    /**
     * @brief �t�F�[�h�̑����ʂ�ύX
     * @param f �t�F�[�h�̑�����
    */
    void SetIncreaseAmountOfChange(const float& f)
    {
        m_increaseAmountOfChange = f;
    }

    /**
     * @brief �t�F�[�h�̌����ʂ�ύX
     * @param f �t�F�[�h�̌�����
    */
    void SetmDecreaseAmountOfChange(const float& f)
    {
        m_decreaseAmountOfChange = f;
    }

    /**
     * @brief �t�F�[�h�̑ҋ@���Ԃ�ύX
     * @param i �t�F�[�h�̑ҋ@���ԁi60�łP�b�j
    */
    void SetWaitTime(const int& i)
    {
        m_waitTime = i;
    }


public: //���̑�
    /**
     * @brief �t�F�[�h���n�߂�
    */
    void StartFade()
    {
        m_flagFade = true;
    }

    /**
     * @brief ���݃t�F�[�h�����Ă��邩
     * @return ���݃t�F�[�h�����Ă��邩
    */
    const bool& IsFadeInProgress()
    {
        return m_flagFade;
    }

    /**
     * @brief �t�F�[�h�ً̋}��~
    */
    void EmergencyStop()
    {
        m_flagFade = false;
        m_stateFade = CurrentFadeState::increase;
        m_amountOfFade = 0.0f;
        m_waitTimer = 0;
        m_spriteFade->SetMulColorW(m_amountOfFade);
    }


private: //enum
    /**
     * @brief ���݂̃t�F�[�h�̏��
    */
    enum class CurrentFadeState
    {
        increase,               //������
        wait,                   //�ҋ@��
        decrease,               //������
        CurrentFadeStateMax
    };


private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    SpriteRender* m_spriteFade = nullptr;

    ////////////////////////////////////////////////////////////
    // �t�F�[�h�����̕ϐ�
    ////////////////////////////////////////////////////////////

    bool m_flagFade = false;                    //�t�F�[�h�����Ă��邩�̃t���O
    float m_amountOfFade = 0.0f;                //���݂̃t�F�[�h�̗�
    float m_increaseAmountOfChange = 0.1f;      //�t�F�[�h�̑����ʁi1�t���[���j
    float m_decreaseAmountOfChange = 0.1f;      //�t�F�[�h�̌����ʁi1�t���[���j
    int m_waitTime = 60;                        //�t�F�[�h�̑ҋ@���ԁi60�łP�b�j
    int m_waitTimer = 0;                        //�t�F�[�h�̑ҋ@���Ԃ̃^�C�}�[
    CurrentFadeState m_stateFade = CurrentFadeState::increase;    //���݂̃t�F�[�h�̏��

};