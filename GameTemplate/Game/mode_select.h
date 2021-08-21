#pragma once
#include "sprite_render.h"
#include "font_render.h"
#include "sound_SE.h"

class MainProcessing;
class SuddenDeathMode;

class ModeSelect : public IGameObject
{
public:
    ModeSelect();
    ~ModeSelect();
    bool Start() override final;
    /**
     * @brief �f�[�^�̏�����
    */
    void Init();
    void Finish();
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
    
    /**
     * @brief �I�����Ă���UI���㉺�Ɉړ����鏈��
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



private: //enum
    enum ModeName
    {
        raceMode,
        suddenDeathMode,
        ModeNameMax
    };




private: //constant
    static const int m_NUMBER_OF_CHOICES = 2;     //�I�����̐�



private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };
    FontRender* m_fontChoices[m_NUMBER_OF_CHOICES] = { nullptr };
    FontRender* m_font = nullptr;
    SoundSE* m_seDecision = nullptr;
    SoundSE* m_seMoveCursor = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////

    SuddenDeathMode* m_findSuddenDeathMode = nullptr;

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