#include "stdafx.h"
#include "game.h"

#include "player.h"
#include "stage.h"
#include "score.h"
#include "CPU_player_controller.h"
#include "sudden_death_mode.h"
#include "pause.h"
#include "game_start_countdown.h"
#include "goal.h"
#include "rank.h"

Game::Game()
{
    //////////////////////////////
    // NewGO
    //////////////////////////////

    m_stage = NewGO<Stage>(igo::PRIORITY_CLASS, igo::CLASS_NAME_STAGE);
    m_player = NewGO<Player>(igo::PRIORITY_CLASS, igo::CLASS_NAME_PLAYER);
    m_score = NewGO<Score>(igo::PRIORITY_CLASS, igo::CLASS_NAME_SCORE);
    m_pause = NewGO<Pause>(igo::PRIORITY_CLASS, igo::CLASS_NAME_PAUSE);
    m_gameData = NewGO<GameData>(igo::PRIORITY_CLASS, igo::CLASS_NAME_GAME_DATA);
    m_CPUPlayerController = NewGO<CPUPlayerController>(igo::PRIORITY_CLASS, igo::CLASS_NAME_CPU_PLAYER_CONTROLLER);
    m_gameStartCountdown = NewGO<GameStartCountdown>(igo::PRIORITY_CLASS, igo::CLASS_NAME_GAME_START_COUNTDOWN);
    m_suddenDeathMode = NewGO<SuddenDeathMode>(igo::PRIORITY_CLASS, igo::CLASS_NAME_SUDDEN_DEATH);
    m_goal = NewGO<Goal>(igo::PRIORITY_CLASS, igo::CLASS_NAME_GOAL);
    m_rank = NewGO<Rank>(igo::PRIORITY_CLASS, igo::CLASS_NAME_RANK);
}

Game::~Game()
{
    DeleteGO(m_stage);
    DeleteGO(m_suddenDeathMode);
    DeleteGO(m_player);
    DeleteGO(m_score);
    DeleteGO(m_pause);
    DeleteGO(m_gameData);
    DeleteGO(m_CPUPlayerController);
    DeleteGO(m_gameStartCountdown);
    DeleteGO(m_goal);
    DeleteGO(m_rank);
}

bool Game::Start()
{

    return true;
}

void Game::Init()
{
    m_flagProcess = true;

    m_stage->Init();
    m_player->Init();
    m_score->Init();
    m_pause->Init();
    m_CPUPlayerController->Init();
    m_gameStartCountdown->Init();
    //m_goal->Init();
    //m_rank->Init();

    if (m_suddenDeathMode->GetFlagSuddenDeathMode() == true) {
        m_suddenDeathMode->Init();
    }

    //Playerクラスに選択されたプレイヤー人数を渡す。
    m_player->SetMaxPlayer(m_gameData->GetMaxPlayer());
    m_stage->SetMaxPlayer(m_gameData->GetMaxPlayer());
    //m_player->SetCPULevel(m_CPULevel);
    m_CPUPlayerController->SetCPULevel(m_gameData->GetCPULevel());
}

void Game::Finish()
{
    m_flagProcess = false;

    m_stage->Finish();
    m_player->Finish();
    m_score->Finish();
    m_pause->Finish();
    m_CPUPlayerController->Finish();
    m_gameStartCountdown->Finish();
    m_goal->Finish();
    m_rank->Finish();

    if (m_suddenDeathMode->GetFlagSuddenDeathMode() == true) {
        m_suddenDeathMode->Finish();
    }
}

void Game::Update()
{
    if (false == m_flagProcess) {
        return;
    }
}