#pragma once
#include "sprite_render.h"
#include "sound_SE.h"

class Player;

class GameStartCountdown : public IGameObject
{
public:
    GameStartCountdown();
    ~GameStartCountdown();
    bool Start() override final;
    void Init();
    void Finish();
    void Update() override final;

private:
    /**
     * @brief �J�E���g�_�E���̏���
    */
    void Countdown();


public:
    /**
     * @brief ���̃��E���h�Ɉړ����鏈��
    */
    void NextRound();

public: //Get�֐�
    /**
     * @brief ���̃Q�[���֘A�̏������~�߂邩
     * @return ���̃Q�[���֘A�̏������~�߂邩
    */
    const bool& GetFlagStopGameProcess() const
    {
        return m_flagStopGameProcess;
    }



private: //enum
    enum Countdown
    {
        count_gameStart,
        count_1,
        count_2,
        count_3,
        CountdownMax
    };

private: //constant
    static const int m_INIT_COUNT_START_COUNTDOWN = 240;        //m_countStartCountdown�̏����l


private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    SpriteRender* m_spriteCountdown[CountdownMax] = { nullptr };
    SoundSE* m_seCount = nullptr;
    SoundSE* m_seGameStart = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////

    Player* m_findPlayer = nullptr;

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    bool m_flagCountdown = true; //�J�E���g�_�E���̏��������邩
    bool m_flagStopGameProcess = true; //�Q�[���̏������~���邩



    float m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;       //�J�E���g�_�E���Ŏg�p�����^�C�}�[

    bool m_flagCount3SE = false;
    bool m_flagCount2SE = false;
    bool m_flagCount1SE = false;
    bool m_flagStartCountdownSE = false;

};