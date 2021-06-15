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


    return true;
}

void Game::Update()
{

}