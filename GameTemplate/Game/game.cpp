#include "stdafx.h"
#include "game.h"

#include "player.h"
#include "game_camera.h"
#include "stage.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
    m_stage = NewGO<Stage>(0, "stage");
    m_player = NewGO<Player>(0, "player");
    m_gameCamera = NewGO<GameCamera>(0);


    return true;
}

void Game::Update()
{

}