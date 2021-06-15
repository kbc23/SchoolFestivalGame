#include "stdafx.h"
#include "game.h"

#include "player_select.h"
#include "player.h"
#include "game_camera.h"
#include "stage.h"
#include "score.h"

Game::Game()
{

}

Game::~Game()
{
    DeleteGO(m_stage);
    DeleteGO(m_player);
    DeleteGO(m_gameCamera);
    DeleteGO(m_score);
}

bool Game::Start()
{
    m_playerSelect = NewGO<PlayerSelect>(0);



    return true;
}

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
    m_stage = NewGO<Stage>(0, "stage");
    m_player = NewGO<Player>(0, "player");
    m_gameCamera = NewGO<GameCamera>(0);
    m_score = NewGO<Score>(0);
    m_fontStartCountdown = NewGO<FontRender>(0);
    m_fontStartCountdown->Init(L"");
}

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

    if (m_countStartCountdown <= -60) {
        m_fontStartCountdown->Deactivate();
        m_flagStartCountdown = false;
    }
    else if (m_countStartCountdown <= 0) {
        m_fontStartCountdown->SetText(L"Start!");
        m_StopOperation = false;
    }
    else if (m_countStartCountdown <= 60) {
        m_fontStartCountdown->SetText(1);
    }
    else if (m_countStartCountdown <= 120) {
        m_fontStartCountdown->SetText(2);
    }
    else if (m_countStartCountdown <= 180) {
        m_fontStartCountdown->SetText(3);
    }
}