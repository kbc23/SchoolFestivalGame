#include "stdafx.h"
#include "game.h"

#include "title.h"
#include "player_select.h"
#include "player.h"
#include "game_camera.h"
#include "stage.h"
#include "score.h"
#include "rule1.h"



namespace
{
    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    const int COUNTDOWN_3 = 180;
    const int COUNTDOWN_2 = 120;
    const int COUNTDOWN_1 = 60;
    const int COUNTDOWN_0 = 0;
    const int COUNTDOWN_DEACTIVATE = -60;
}



Game::Game()
{

}

Game::~Game()
{
    DeleteGO(m_stage);
    DeleteGO(m_player);
    DeleteGO(m_gameCamera);
    DeleteGO(m_score);
    DeleteGO(m_fontStartCountdown);
}

bool Game::Start()
{
    m_title = NewGO<Title>(igo::PRIORITY_FIRST);

    return true;
}

void Game::Update()
{
    //このif文をフラグで管理しているのを、enum型で管理するように変更すること

    //タイトルシーンの処理
    if (m_flagTitleScene == true) {
        TitleScene();
    }
    //プレイヤーセレクトシーンの処理
    if (m_flagPlayerSelectScene == true) {
        PlayerSelectScene();
    }

    //ゲームシーンでの処理
    if (m_flagGameScene == true) {
        GameScene();
    }
    
}

////////////////////////////////////////////////////////////
// タイトルシーンの処理
////////////////////////////////////////////////////////////

void Game::TitleScene()
{
    if (m_title->GetFlagFinish() == false) {
        return;
    }

    NewGOPlayerSelectScene();

    DeleteGO(m_title);

    m_flagTitleScene = false;
    m_flagPlayerSelectScene = true;
}

void Game::NewGOPlayerSelectScene()
{
    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_FIRST);
}

////////////////////////////////////////////////////////////
// プレイヤーセレクトシーンの処理
////////////////////////////////////////////////////////////

void Game::PlayerSelectScene()
{
    if (m_playerSelect->GetFlagFinish() == false) {
        return;
    }

    NewGOGameScene();

    DeleteGO(m_playerSelect);

    //Playerクラスに選択されたプレイヤー人数を渡す。
    m_player->SetMaxPlayer(m_maxPlayer);

    //Stageクラスに選択されたプレイヤー人数を渡す。
    m_stage->SetMaxPlayer(m_maxPlayer);

    m_flagPlayerSelectScene = false;
    m_flagGameScene = true;
}

void Game::NewGOGameScene()
{
    m_rule1 = NewGO<Rule1>(igo::PRIORITY_FIRST, igo::CLASS_NAME_RULE1);     //ワンデスモード
    m_stage = NewGO<Stage>(igo::PRIORITY_FIRST, igo::CLASS_NAME_STAGE);
    m_player = NewGO<Player>(igo::PRIORITY_FIRST, igo::CLASS_NAME_PLAYER);
    m_gameCamera = NewGO<GameCamera>(igo::PRIORITY_FIRST);
    m_score = NewGO<Score>(igo::PRIORITY_FIRST, igo::CLASS_NAME_SCORE);
    m_fontStartCountdown = NewGO<FontRender>(igo::PRIORITY_FIRST);
    m_fontStartCountdown->Init(L"");
}


////////////////////////////////////////////////////////////
// ゲームシーンの処理
////////////////////////////////////////////////////////////

void Game::GameScene()
{
    if (m_flagStartCountdown == false) {
        return;
    }

    StartCountdown();

}

void Game::StartCountdown()
{
    --m_countStartCountdown;

    //カウントダウンフォントを非表示
    if (m_countStartCountdown <= COUNTDOWN_DEACTIVATE) {
        m_fontStartCountdown->Deactivate();
        m_flagStartCountdown = false;
    }
    //カウント０
    else if (m_countStartCountdown <= COUNTDOWN_0) {
        m_fontStartCountdown->SetText(L"Start!");
        m_StopOperation = false;
    }
    //カウント１
    else if (m_countStartCountdown <= COUNTDOWN_1) {
        m_fontStartCountdown->SetText(1);
    }
    //カウント２
    else if (m_countStartCountdown <= COUNTDOWN_2) {
        m_fontStartCountdown->SetText(2);
    }
    //カウント３
    else if (m_countStartCountdown <= COUNTDOWN_3) {
        m_fontStartCountdown->Activate();
        m_fontStartCountdown->SetText(3);
    }
}

void Game::NextRound()
{
    m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;
    m_flagStartCountdown = true;
    m_StopOperation = true;
    //m_flagTitleScene = true;
    //m_flagPlayerSelectScene = false;
    //m_flagGameScene = false;
    //m_maxPlayer = 0;
}