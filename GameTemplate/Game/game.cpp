#include "stdafx.h"
#include "game.h"

#include "player.h"
#include "stage.h"
#include "score.h"
#include "EnemyAI.h"
#include "rule1.h"

Game::Game()
{

}

Game::~Game()
{

}

bool Game::Start()
{
    m_stage = NewGO<Stage>(igo::PRIORITY_CLASS, igo::CLASS_NAME_STAGE);
    m_rule1 = NewGO<Rule1>(igo::PRIORITY_CLASS, igo::CLASS_NAME_RULE1);
    m_player = NewGO<Player>(igo::PRIORITY_CLASS, igo::CLASS_NAME_PLAYER);
    m_enemyAI = NewGO<EnemyAI>(igo::PRIORITY_CLASS, igo::CLASS_NAME_ENEMYAI);
    m_score = NewGO<Score>(igo::PRIORITY_CLASS, igo::CLASS_NAME_SCORE);

    return true;
}

void Game::Init()
{
    m_flagProcessing = true;

    m_player->Init();
    m_stage->Init();

    if (m_ruleSuddenDeath == true) {
        m_rule1->Init();
    }
    m_enemyAI->Init();
    m_score->Init();

    //Playerクラスに選択されたプレイヤー人数を渡す。
    m_player->SetMaxPlayer(m_maxPlayer);
    m_stage->SetMaxPlayer(m_maxPlayer);
    m_enemyAI->SetDifficultyLevel(m_dilevel);
}

void Game::Finish()
{
    m_flagProcessing = false;
    
    m_stage->Finish();
    if (m_ruleSuddenDeath == true) {
        m_rule1->Finish();
    }
    m_player->Finish();
    m_enemyAI->Finish();
    m_score->Finish();
}

void Game::Update()
{
    if (m_flagProcessing == false) {
        return;
    }
}

