#pragma once
#include "sprite_render.h"
#include "font_render.h"
#include "sound_SE.h"
class Game;

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

    void DrawMove();


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
    Game* m_game = nullptr;

    SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };
    FontRender* m_font = nullptr;
    SoundSE* m_seDecision = nullptr;
    SoundSE* m_seMoveCursor = nullptr;

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

    bool m_flagMove = true;
};