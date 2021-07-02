#include "stdafx.h"
#include "game.h"

#include "title.h"
#include "mode_select.h"
#include "player_select.h"
#include "CPU_strength_select.h"
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
    m_spriteBackground = NewGO<SpriteRender>(igo::PRIORITY_FIRST);
    m_spriteBackground->Init(filePath::DDS_BACKGROUND);
    m_title = NewGO<Title>(igo::PRIORITY_FIRST);
    m_fade = NewGO<Fade>(igo::PRIORITY_THIRD);

    return true;
}

void Game::Update()
{
    switch (m_gameStatus) {
    case GameStatus::title:
        TitleScene();
        break;
    case GameStatus::modeSelect:
        ModeSelectScene();
        break;
    case GameStatus::playerSelect:
        PlayerSelectScene();
        break;
    case GameStatus::CPUStrengthSelect:
        CPUStrengthSelectScene();
        break;
    case GameStatus::loadingGame:
        LoadingGameScene();
        break;
    case GameStatus::game:
        GameScene();
        break;
    default:
        MessageBoxA(nullptr, "ゲームの遷移にてエラーが発生しました。", "エラー", MB_OK);
        return;
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

    NewGOModeSelectScene();

    DeleteGO(m_title);

    m_gameStatus = GameStatus::modeSelect;
}

void Game::NewGOModeSelectScene()
{
    m_modeSelect = NewGO<ModeSelect>(igo::PRIORITY_FIRST);
}

////////////////////////////////////////////////////////////
// モードセレクトシーンの処理
////////////////////////////////////////////////////////////

void Game::ModeSelectScene()
{
    if (m_modeSelect->GetFlagFinish() == false) {
        //キャンセル
        if (g_pad[con::player_1]->IsTrigger(enButtonB)) {
            ReturnTitleScene();
        }

        return;
    }

    NewGOPlayerSelectScene();

    DeleteGO(m_modeSelect);

    m_gameStatus = GameStatus::playerSelect;
}

void Game::NewGOPlayerSelectScene()
{
    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_FIRST);
}

void Game::ReturnTitleScene()
{
    m_title = NewGO<Title>(igo::PRIORITY_FIRST);

    DeleteGO(m_modeSelect);

    m_gameStatus = GameStatus::title;
}

////////////////////////////////////////////////////////////
// プレイヤーセレクトシーンの処理
////////////////////////////////////////////////////////////

void Game::PlayerSelectScene()
{
    if (m_playerSelect->GetFlagFinish() == false) {
        //キャンセル
        if (g_pad[con::player_1]->IsTrigger(enButtonB)) {
            ReturnModeSelectScene();
        }

        return;
    }

    NewGOCPUStrengthSelectScene();

    DeleteGO(m_playerSelect);

    m_gameStatus = GameStatus::CPUStrengthSelect;
}

void Game::NewGOCPUStrengthSelectScene()
{
    m_CPUStrengthSelect = NewGO<CPUStrengthSelect>(igo::PRIORITY_FIRST);
}

void Game::ReturnModeSelectScene()
{
    m_modeSelect = NewGO<ModeSelect>(igo::PRIORITY_FIRST);

    DeleteGO(m_playerSelect);

    m_gameStatus = GameStatus::modeSelect;
}

////////////////////////////////////////////////////////////
// CPUの難易度選択シーンの処理
////////////////////////////////////////////////////////////

void Game::CPUStrengthSelectScene()
{
    if (m_CPUStrengthSelect->GetFlagFinish() == false) {
        //キャンセル
        if (g_pad[con::player_1]->IsTrigger(enButtonB)) {
            ReturnPlayerSelectScene();
        }

        return;
    }

    NewGOGameScene();

    DeleteGO(m_CPUStrengthSelect);

    m_gameStatus = GameStatus::loadingGame;
    m_loadStatus = LoadingStatus::preparingForLoading;
}

void Game::NewGOGameScene()
{

}

void Game::ReturnPlayerSelectScene()
{
    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_FIRST);

    DeleteGO(m_CPUStrengthSelect);

    m_gameStatus = GameStatus::playerSelect;
}

////////////////////////////////////////////////////////////
// ゲームシーンのためのロード
////////////////////////////////////////////////////////////

void Game::LoadingGameScene()
{
    switch (m_loadStatus) {
    case LoadingStatus::preparingForLoading:
        PreparingForLoading();
        break;
    case LoadingStatus::loading:
        Loading();
        break;
    case LoadingStatus::endOfLoading:
        EndOfLoading();
        break;
    default:
        MessageBoxA(nullptr, "ゲームのロードにてエラーが発生しました。", "エラー", MB_OK);
        return;
    }
}

void Game::PreparingForLoading()
{
    if (m_startPreparingForLoading == false) {
        m_fade->StartFadeOut();
        m_startPreparingForLoading = true;
    }

    if (m_fade->IsFadeOutProgress() == false) {
        m_loadStatus = LoadingStatus::loading;
    }
}

void Game::Loading()
{
    m_stage = NewGO<Stage>(igo::PRIORITY_FIRST, igo::CLASS_NAME_STAGE);
    //m_rule1 = NewGO<Rule1>(igo::PRIORITY_FIRST, igo::CLASS_NAME_RULE1);
    m_player = NewGO<Player>(igo::PRIORITY_FIRST, igo::CLASS_NAME_PLAYER);
    m_gameCamera = NewGO<GameCamera>(igo::PRIORITY_FIRST);
    m_score = NewGO<Score>(igo::PRIORITY_FIRST);
    m_fontStartCountdown = NewGO<FontRender>(igo::PRIORITY_SECOND);
    m_fontStartCountdown->Init(L"");

    //Playerクラスに選択されたプレイヤー人数を渡す。
    m_player->SetMaxPlayer(m_maxPlayer);

    //選択画面の背景を削除
    DeleteGO(m_spriteBackground);

    m_loadStatus = LoadingStatus::endOfLoading;
}

void Game::EndOfLoading()
{
    if (m_startEndOfLoading == false) {
        m_fade->StartFadeIn();
        m_startEndOfLoading = true;
    }
    
    if (m_fade->IsFadeInProgress() == false) {
        m_gameStatus = GameStatus::game;
        m_loadStatus = LoadingStatus::doNothing;
    }
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
        m_fontStartCountdown->SetText(3);
    }
}