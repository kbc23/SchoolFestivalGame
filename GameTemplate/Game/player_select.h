#pragma once
#include "font_render.h"
#include "sprite_render.h"
#include "sound_SE.h"

class Player;

class PlayerSelect : public IGameObject
{
public:
    PlayerSelect();
    ~PlayerSelect();
    bool Start() override final;
    void Init();
    void Finish();
    void Update() override final;
    

private:
    /**
     * @brief ���삷��v���C���[�̐l���̑I��
    */
    void SelectTheNumberOfPlayers();

    /**
     * @brief ���̃N���X�̏����̏I������
    */
    void FinishPlayerSelect();

    /**
     * @brief �I�����Ă���UI���㉺�ɓ�������
    */
    void DrawMove();



public: //Get�֐�
    /**
     * @brief m_flagFinish��Get�֐�
     * @return ���̃N���X�̏������I��������
    */
    const bool GetFlagFinish() const
    {
        return m_flagFinish;
    }


private: //constant
    static const int m_NUMBER_OF_CHOICES = 4;     //�I�����̐�


private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };
    FontRender* m_font = nullptr;
    SoundSE* m_seDecision = nullptr;
    SoundSE* m_seMoveCursor = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////

    Player* m_findPlayer = nullptr;

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

    bool m_flagDrawMove = true;     //UI���㉺�Ɉړ����鏈���Ɏg�p����t���O
};