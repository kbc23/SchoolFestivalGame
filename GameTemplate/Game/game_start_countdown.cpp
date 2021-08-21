#include "stdafx.h"
#include "game_start_countdown.h"

#include "constant.h"
#include "player.h"


namespace //constant
{
    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    const float COUNTDOWN_3 = 180.0f;
    const float COUNTDOWN_2 = 120.0f;
    const float COUNTDOWN_1 = 60.0f;
    const float COUNTDOWN_0 = 0.0f;
    const float COUNTDOWN_DEACTIVATE = -60.0f;
}



GameStartCountdown::GameStartCountdown()
{
    //////////////////////////////
    // NewGO
    //////////////////////////////

    //////////
    // �X�v���C�g��NewGO
    //////////

    //�J�E���g�_�E����UI��NewGO
    for (int countdownNum = con::FIRST_ELEMENT_ARRAY; countdownNum < CountdownMax; countdownNum++) {
        m_spriteCountdown[countdownNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteCountdown[countdownNum]->Init(filePath::dds::COUNT[countdownNum]);
        m_spriteCountdown[countdownNum]->Deactivate();
    }

    //////////
    // SE��NewGO
    //////////

    m_seCount = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seCount->Init(filePath::se::COUNT);
    m_seGameStart = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seGameStart->Init(filePath::se::GAME_START);
}

GameStartCountdown::~GameStartCountdown()
{
    for (int countdownNum = con::FIRST_ELEMENT_ARRAY; countdownNum < CountdownMax; countdownNum++) {
        DeleteGO(m_spriteCountdown[countdownNum]);
    }

    DeleteGO(m_seCount);
    DeleteGO(m_seGameStart);
}

bool GameStartCountdown::Start()
{
    //////////////////////////////
    // FindGO
    //////////////////////////////

    m_findPlayer = FindGO<Player>(igo::CLASS_NAME_PLAYER);

    return true;
}

void GameStartCountdown::Init()
{
    m_flagProcess = true;

    //////////
    // �X�v���C�g�̏�����
    //////////

    //�J�E���g�_�E����UI�̏�����
    for (int countdownNum = con::FIRST_ELEMENT_ARRAY; countdownNum < CountdownMax; countdownNum++) {
        m_spriteCountdown[countdownNum]->Deactivate();
    }

    //////////
    // �����o�ϐ��̏�����
    //////////

    m_flagCountdown = true; //�J�E���g�_�E���̏��������邩
    m_flagStopGameProcess = true; //�Q�[���̏������~���邩
    m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN; //�J�E���g�_�E���Ŏg�p�����^�C�}�[
    m_flagCount3SE = false;
    m_flagCount2SE = false;
    m_flagCount1SE = false;
    m_flagStartCountdownSE = false;
}

void GameStartCountdown::Finish()
{
    m_flagProcess = false;

    //////////
    // �X�v���C�g�̔�\��
    //////////

    //�J�E���g�_�E����UI�̔�\��
    for (int countdownNum = con::FIRST_ELEMENT_ARRAY; countdownNum < CountdownMax; countdownNum++) {
        m_spriteCountdown[countdownNum]->Deactivate();
    }
}

void GameStartCountdown::Update()
{
    if (false == m_flagProcess) {
        return;
    }

    Countdown();
}

void GameStartCountdown::Countdown()
{
    if (m_flagCountdown == false) {
        return;
    }

    --m_countStartCountdown;

    m_countStartCountdown -= g_gameTime->GetFrameDeltaTime();

    //�J�E���g�_�E���t�H���g���\��
    if (m_countStartCountdown <= COUNTDOWN_DEACTIVATE) {
        m_spriteCountdown[count_gameStart]->Deactivate();
        m_flagCountdown = false; //�J�E���g�_�E���̏������~�߂�
    }
    //�J�E���g�O
    else if (m_countStartCountdown <= COUNTDOWN_0) {
        if (m_flagStartCountdownSE == false) {
            m_spriteCountdown[count_1]->Deactivate();
            m_spriteCountdown[count_gameStart]->Activate();
            m_flagStopGameProcess = false;
            m_seGameStart->Play(false);
            m_flagStartCountdownSE = true;
        }
    }
    //�J�E���g�P
    else if (m_countStartCountdown <= COUNTDOWN_1) {
        if (m_flagCount1SE == false) {
            m_spriteCountdown[count_2]->Deactivate();
            m_spriteCountdown[count_1]->Activate();
            m_seCount->Play(false);
            m_flagCount1SE = true;
        }
    }
    //�J�E���g�Q
    else if (m_countStartCountdown <= COUNTDOWN_2) {
        if (m_flagCount2SE == false) {
            m_spriteCountdown[count_3]->Deactivate();
            m_spriteCountdown[count_2]->Activate();
            m_seCount->Play(false);
            m_flagCount2SE = true;
        }
    }
    //�J�E���g�R
    else if (m_countStartCountdown <= COUNTDOWN_3) {
        if (m_flagCount3SE == false) {
            m_spriteCountdown[count_3]->Activate();
            m_seCount->Play(false);
            m_flagCount3SE = true;
        }
    }
}

void GameStartCountdown::NextRound()
{
    m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;
    m_flagCountdown = true;
    m_flagStopGameProcess = true;

    m_flagCount3SE = false;
    m_flagCount2SE = false;
    m_flagCount1SE = false;
    m_flagStartCountdownSE = false;
}