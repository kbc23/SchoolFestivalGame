#pragma once
#include "font_render.h"
#include "game.h"

class PlayerSelect : public IGameObject
{
public:
    PlayerSelect();
    ~PlayerSelect();
    bool Start() override final;
    void Update() override final;



    void SelectTheNumberOfPlayers();


    void Kari();



public: //Get�֐�

    bool GetmFlagFinish()
    {
        return m_flagFinish;
    }




private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    Game* m_game = nullptr;
    FontRender* m_fontNumberOfPlayer[3] = { nullptr };
    FontRender* m_fontCursor = nullptr;

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