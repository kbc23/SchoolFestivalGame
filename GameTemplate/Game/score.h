#pragma once
#include "sprite_render.h"
#include "font_render.h"
#include "constant.h"

class MainProcessing;
class Player;
class Stage;
class Rule1;

class Score : public IGameObject
{
public:
    Score();
    ~Score();
    bool Start() override final;
    void Init() override final;
    void Finish() override final;

    ////////////////////////////////////////////////////////////
    // ���t���[���̏���
    ////////////////////////////////////////////////////////////

    void Update() override final;

    /**
     * @brief �X�R�A���Ԃ𑝉�����֐�
     * @param pNum �v���C���[�ԍ�
    */
    void AddTime(const int pNum);

    /**
     * @brief �X�R�A���Ԃ̑������~����֐�
     * @param pNum �v���C���[�ԍ�
    */
    void FinishTime(const int pNum);

    /**
     * @brief �X�R�A���Ԃ̕`�揈��
     * @param pNum �v���C���[�ԍ�
    */
    void DrawTime(const int pNum);

    //////////////////////////////
    // ���E���h�ύX�̏���
    //////////////////////////////

    /**
     * @brief ���E���h�ύX�̏���
    */
    void NextRound();

private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////
    
    Player* m_player = nullptr;
    MainProcessing* m_game = nullptr;
    SpriteRender* m_spriteUI = nullptr;
    FontRender* m_fontScoreTime[con::PlayerNumberMax] = { nullptr }; //�e�v���C���[�̃X�R�A�^�C���̃t�H���g
    Stage* m_stage = nullptr;

    ////////////////////////////////////////////////////////////
    // �X�R�A�^�C���֘A
    ////////////////////////////////////////////////////////////

    int m_scoreTime[con::PlayerNumberMax] = { 0,0,0,0 };                     //�X�R�A�^�C���̃J�E���g

    bool m_flagScoreTimeProcessing[con::PlayerNumberMax] = { true,true,true,true };   //�X�R�A�^�C���̃J�E���g�̏����������Ȃ��Ă��邩

    int m_scoreTimeMinutes[con::PlayerNumberMax] = { 0,0,0,0 };              //��
    int m_scoreTimeSeconds[con::PlayerNumberMax] = { 0,0,0,0 };              //�b
    int m_scoreTimeCommaSeconds[con::PlayerNumberMax] = { 0,0,0,0 };         //�R���}�b

};

