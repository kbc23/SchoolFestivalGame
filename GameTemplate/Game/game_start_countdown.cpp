#include "stdafx.h"
#include "game_start_countdown.h"

#include "constant.h"
#include "player.h"


namespace //constant
{
    ////////////////////////////////////////////////////////////
    // タイマー関連
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
    // スプライトのNewGO
    //////////

    //カウントダウンのUIのNewGO
    for (int countdownNum = con::FIRST_ELEMENT_ARRAY; countdownNum < CountdownMax; countdownNum++) {
        m_spriteCountdown[countdownNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteCountdown[countdownNum]->Init(filePath::dds::COUNT[countdownNum]);
        m_spriteCountdown[countdownNum]->Deactivate();
    }

    //////////
    // SEのNewGO
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
    // スプライトの初期化
    //////////

    //カウントダウンのUIの初期化
    for (int countdownNum = con::FIRST_ELEMENT_ARRAY; countdownNum < CountdownMax; countdownNum++) {
        m_spriteCountdown[countdownNum]->Deactivate();
    }

    //////////
    // メンバ変数の初期化
    //////////

    m_flagCountdown = true; //カウントダウンの処理をするか
    m_flagStopGameProcess = true; //ゲームの処理を停止するか
    m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN; //カウントダウンで使用されるタイマー
    m_flagCount3SE = false;
    m_flagCount2SE = false;
    m_flagCount1SE = false;
    m_flagStartCountdownSE = false;
}

void GameStartCountdown::Finish()
{
    m_flagProcess = false;

    //////////
    // スプライトの非表示
    //////////

    //カウントダウンのUIの非表示
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

    //カウントダウンフォントを非表示
    if (m_countStartCountdown <= COUNTDOWN_DEACTIVATE) {
        m_spriteCountdown[count_gameStart]->Deactivate();
        m_flagCountdown = false; //カウントダウンの処理を止める
    }
    //カウント０
    else if (m_countStartCountdown <= COUNTDOWN_0) {
        if (m_flagStartCountdownSE == false) {
            m_spriteCountdown[count_1]->Deactivate();
            m_spriteCountdown[count_gameStart]->Activate();
            m_flagStopGameProcess = false;
            m_seGameStart->Play(false);
            m_flagStartCountdownSE = true;
        }
    }
    //カウント１
    else if (m_countStartCountdown <= COUNTDOWN_1) {
        if (m_flagCount1SE == false) {
            m_spriteCountdown[count_2]->Deactivate();
            m_spriteCountdown[count_1]->Activate();
            m_seCount->Play(false);
            m_flagCount1SE = true;
        }
    }
    //カウント２
    else if (m_countStartCountdown <= COUNTDOWN_2) {
        if (m_flagCount2SE == false) {
            m_spriteCountdown[count_3]->Deactivate();
            m_spriteCountdown[count_2]->Activate();
            m_seCount->Play(false);
            m_flagCount2SE = true;
        }
    }
    //カウント３
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