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
    void Init();
    void Finish();
    void Update() override final;

	/**
	 * @brief �|�[�Y���ɂ����Ă΂��A�b�v�f�[�g�֐�
	*/
	void UpdateOnlyPaused()override final;


private:
    /**
     * @brief �|�[�Y��ʂ��J���Ƃ��ɂ����Ȃ�����
    */
    void PauseInit();

    /**
     * @brief �|�[�Y��ʂ����Ƃ��ɂ����Ȃ�����
    */
    void PauseFinish();


public: //enum
    enum Choices
    {
        returnGame, //�Q�[���ɖ߂�
        retry,      //���g���C
        finish,     //�Q�[������߂�
        ChoicesMax
    };



private: //constant
	//static const int m_NUMBER_OF_CHOICES = 3;     //�I�����̐�



private: //data menber
	SpriteRender* m_spriteChoices[ChoicesMax] = { nullptr };
    Stage* m_stage = nullptr;
    MainProcessing* m_mainProcessing = nullptr;

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_cursorPosition = 0;       //�J�[�\���̏ꏊ

    int m_pausePlayer = con::player_1; //�|�[�Y��Ԃɂ����v���C���[

    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_flagDecision = false;    //�l�������肵�����̃t���O
};