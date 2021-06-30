#include "stdafx.h"
#include "game.h"

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
    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_FIRST);

    return true;
}

////////////////////////////////////////////////////////////
// 毎フレームの処理
////////////////////////////////////////////////////////////

void Game::Update()
{
    //プレイヤーセレクトシーンの処理
    if (m_flagPlayerSelectScene == true) {
        PlayerSelectScene();
    }

    //ゲームシーンでの処理
    if (m_flagGameScene == true) {
        GameScene();
    }

}

//////////////////////////////
// プレイヤーセレクトシーンの処理
//////////////////////////////

void Game::PlayerSelectScene()
{
    if (m_playerSelect->GetmFlagFinish() == false) {
        return;
    }

    NewGOGame();

    DeleteGO(m_playerSelect);

    //Playerクラスに選択されたプレイヤー人数を渡す。
    m_player->SetMaxPlayer(m_maxPlayer);
    

    m_flagPlayerSelectScene = false;
    m_flagGameScene = true;
}

void Game::NewGOGame()
{
    m_stage = NewGO<Stage>(igo::PRIORITY_FIRST, igo::CLASS_NAME_STAGE);
    //m_rule1 = NewGO<Rule1>(igo::PRIORITY_FIRST, igo::CLASS_NAME_RULE1);     //ワンデスモード
    m_player = NewGO<Player>(igo::PRIORITY_FIRST, igo::CLASS_NAME_PLAYER);
    m_gameCamera = NewGO<GameCamera>(igo::PRIORITY_FIRST);
    m_score = NewGO<Score>(igo::PRIORITY_FIRST);
    m_fontStartCountdown = NewGO<FontRender>(igo::PRIORITY_FIRST);
    m_fontStartCountdown->Init(L"");

}


//////////////////////////////
// ゲームシーンの処理
//////////////////////////////

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
        m_fontStartCountdown->SetText(3);
    }
}