#pragma once
#include "sprite_render.h"

class CPUStrengthSelect : public IGameObject
{
public:
    CPUStrengthSelect();
    ~CPUStrengthSelect();
    bool Start() override final;
    void Update() override final;



private:
    /**
     * @brief ���삷��v���C���[�̐l���̑I��
    */
    void SelectTheNumberOfCPUStrength();

    /**
     * @brief ���̃N���X�̏����̏I������
    */
    void FinishCPUStrengthSelect();


public: //Get�֐�
    /**
     * @brief m_flagFinish��Get�֐�
     * @return ���̃N���X�̏������I��������
    */
    bool GetFlagFinish()
    {
        return m_flagFinish;
    }


private: //constant
    static const int m_NUMBER_OF_CHOICES = 3;     //�I�����̐�



private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_cursorPosition = 0;       //�J�[�\���̏ꏊ

    int m_numberOfPlayer = 0;       //�v���C���[�̐l��

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagDecision = false;    //�l�������肵�����̃t���O

    bool m_flagFinish = false;      //���̃N���X�ł���ׂ��������I�������

    bool m_flagInput = false;       //�O�̃t���[���œ��͂��Ă��邩�ǂ����̃t���O


};