#pragma once
#include "model_render.h"
#include "sprite_render.h"
#include "sound_SE.h"
#include "constant.h"

class Stage;
class MainProcessing;

class Pause : public IGameObject
{
public:
    Pause();
    ~Pause();
    bool Start() override final;
	void Init() override final;
	void Finish() override final;
    void Update() override final;

	/**
	 * @brief �|�[�Y���Ă��邩�֌W�Ȃ��ɌĂ΂��A�b�v�f�[�g�֐�
	*/
	void AlwaysUpdate()override final;

	/**
	 * @brief �|�[�Y���ɂ����Ă΂��A�b�v�f�[�g�֐�
	*/
	void UpdateOnlyPaused()override final;

    /**
     * @brief ���̃N���X�̏����̏I������
    */
    void FinishPause();

private:
    //�|�[�Y��ʂ̂Ƃ��ɌĂԂ��
    void PauseInit();

    //�|�[�Y��ʂ���Ȃ��Ȃ�Ƃ��ɌĂԂ��
    void PauseFinish();

private: //constant
	static const int m_NUMBER_OF_CHOICES = 3;     //�I�����̐�

private: //data menber
	SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };
    Stage* m_stage = nullptr;
    MainProcessing* m_game = nullptr;

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