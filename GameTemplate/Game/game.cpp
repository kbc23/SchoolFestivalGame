#include "stdafx.h"
#include "game.h"

#include "player.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
    m_player = NewGO<Player>(0);

    return true;
}

void Game::Update()
{

}