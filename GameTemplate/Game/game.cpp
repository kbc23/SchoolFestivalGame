#include "stdafx.h"
#include "game.h"

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
    m_stage = NewGO<Stage>(0, "stage");
    m_player = NewGO<Player>(0, "player");
    m_gameCamera = NewGO<GameCamera>(0);
    m_score = NewGO<Score>(0);
    m_fontStartCountdown = NewGO<FontRender>(0);
    m_fontStartCountdown->Init(L"");


    return true;
}

void Game::Update()
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