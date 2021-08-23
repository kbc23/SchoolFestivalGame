#pragma once
#include "sprite_render.h"
#include "font_render.h"
#include "constant.h"

class MainProcessing;
class Player;
class Pause;
class GameStartCountdown;
class Stage;
class Rank;

class Score : public IGameObject
{
public:
    Score();
    ~Score();
    bool Start() override final;
    void Init();
    void Finish();

    ////////////////////////////////////////////////////////////
    // ���t���[���̏���
    ////////////////////////////////////////////////////////////

    void Update() override final;

    /**
     * @brief �X�R�A���Ԃ𑝉�����֐�
     * @param pNum �v���C���[�ԍ�
    */
    void AddTime(const int playerNum);

    /**
     * @brief �X�R�A���Ԃ̑������~����֐�
     * @param pNum �v���C���[�ԍ�
    */
    void FinishTime(const int playerNum);

    /**
     * @brief �X�R�A���Ԃ̕`�揈��
     * @param pNum �v���C���[�ԍ�
    */
    void DrawTime(const int playerNum);

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

    //////////////////////////////
    // NewGO
    //////////////////////////////

    SpriteRender* m_spriteUI = nullptr;
    FontRender* m_fontScoreTime[con::PlayerNumberMax] = { nullptr }; //�e�v���C���[�̃X�R�A�^�C���̃t�H���g

    //////////////////////////////
    // FindGO
    //////////////////////////////

    Player* m_findPlayer = nullptr;
    MainProcessing* m_findMainProcessing = nullptr;
    Pause* m_findPause = nullptr;
    GameStartCountdown* m_findGameStartCountdown = nullptr;
    Stage* m_findStage = nullptr;
    Rank* m_findRank = nullptr;

    ////////////////////////////////////////////////////////////
    // �X�R�A�^�C���֘A
    ////////////////////////////////////////////////////////////

    int m_scoreTime[con::PlayerNumberMax] = { 0,0,0,0 };                     //�X�R�A�^�C���̃J�E���g

    bool m_flagScoreTimeProcessing[con::PlayerNumberMax] = { true,true,true,true };   //�X�R�A�^�C���̃J�E���g�̏����������Ȃ��Ă��邩

    int m_scoreTimeMinutes[con::PlayerNumberMax] = { 0,0,0,0 };              //��
    int m_scoreTimeSeconds[con::PlayerNumberMax] = { 0,0,0,0 };              //�b
    int m_scoreTimeCommaSeconds[con::PlayerNumberMax] = { 0,0,0,0 };         //�R���}�b

};

