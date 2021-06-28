#pragma once
#include "font_render.h"
#include "sprite_render.h"

class Game;

class PlayerSelect : public IGameObject
{
public:
    PlayerSelect();
    ~PlayerSelect();
    bool Start() override final;
    
    ////////////////////////////////////////////////////////////
    // ���t���[���̏���
    ////////////////////////////////////////////////////////////

    void Update() override final;

    /**
     * @brief ���삷��v���C���[�̐l���̑I��
    */
    void SelectTheNumberOfPlayers();

    /**
     * @brief ���̃N���X�̏����̏I������
    */
    void FinishPlayerSelect();



public: //Get�֐�
    /**
     * @brief m_flagFinish��Get�֐�
     * @return ���̃N���X�̏������I��������
    */
    bool GetmFlagFinish()
    {
        return m_flagFinish;
    }


private: //constant
    static const int m_NUMBER_OF_FONTS = 3;     //�t�H���g�̐�


private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    Game* m_game = nullptr;
    FontRender* m_fontNumberOfPlayer[m_NUMBER_OF_FONTS] = { nullptr };      //���삷��v���C���[�̐l����I������t�H���g
    FontRender* m_fontCursor = nullptr;                     //�J�[�\���̃t�H���g
    SpriteRender* m_spriteBackground = nullptr;

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_cursorPosition = 0;       //�J�[�\���̏ꏊ

    int m_numberOfPlayer = 0;       //�v���C���[�̐l��

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagDecision = false;    //�l�������肵�����̃t���O

    bool m_flagFinish = false;      //�l�������肵����̏������I���������̃t���O

    bool m_flagInput = false;       //�O�̃t���[���œ��͂��Ă��邩�ǂ����̃t���O
};