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
#include "EnemyAI.h"
#include "Result.h"



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
    DeleteGO(m_enemyAI);
    DeleteGO(m_player);
    DeleteGO(m_gameCamera);
    DeleteGO(m_score);
    DeleteGO(m_fontStartCountdown);

    DeleteGO(m_seCancel);
    DeleteGO(m_seCount);
    DeleteGO(m_seGameStart);
}

bool Game::Start()
{
    m_spriteBackground = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground->Init(filePath::dds::BACKGROUND);
    m_title = NewGO<Title>(igo::PRIORITY_CLASS);
    m_fade = NewGO<Fade>(igo::PRIORITY_CLASS);

    m_bgmTitle = NewGO<SoundBGM>(igo::PRIORITY_CLASS);
    m_bgmTitle->Init(filePath::bgm::TITLE);
    m_bgmTitle->SetVolume(0.5f);
    m_bgmTitle->Play(true);

    m_seCancel = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seCancel->Init(filePath::se::CANCEL);
    m_seCount = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seCount->Init(filePath::se::COUNT);
    m_seGameStart = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seGameStart->Init(filePath::se::GAME_START);

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
    case GameStatus::playerSelect: //プレイヤーの人数が４人のとき、CPUの強さの選択画面をスキップする処理を追加すること。
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
    case GameStatus::result:
        ResultScene();
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
    m_modeSelect = NewGO<ModeSelect>(igo::PRIORITY_CLASS);
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
    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_CLASS);
}

void Game::ReturnTitleScene()
{
    m_seCancel->Play(false);

    m_title = NewGO<Title>(igo::PRIORITY_CLASS);

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
    m_CPUStrengthSelect = NewGO<CPUStrengthSelect>(igo::PRIORITY_CLASS);
}

void Game::ReturnModeSelectScene()
{
    m_seCancel->Play(false);

    m_modeSelect = NewGO<ModeSelect>(igo::PRIORITY_CLASS);

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
    m_seCancel->Play(false);

    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_CLASS);

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
    m_stage = NewGO<Stage>(igo::PRIORITY_CLASS, igo::CLASS_NAME_STAGE);
    //m_rule1 = NewGO<Rule1>(igo::PRIORITY_FIRST, igo::CLASS_NAME_RULE1);
    m_player = NewGO<Player>(igo::PRIORITY_CLASS, igo::CLASS_NAME_PLAYER);
    m_enemyAI = NewGO<EnemyAI>(igo::PRIORITY_CLASS, igo::CLASS_NAME_ENEMYAI);
    m_gameCamera = NewGO<GameCamera>(igo::PRIORITY_CLASS);
    m_score = NewGO<Score>(igo::PRIORITY_CLASS);
    m_fontStartCountdown = NewGO<FontRender>(igo::PRIORITY_FONT);
    m_fontStartCountdown->Init(L"");

    //Playerクラスに選択されたプレイヤー人数を渡す。
    m_player->SetMaxPlayer(m_maxPlayer);
    m_enemyAI->SetDifficultyLevel(m_dilevel);

    //選択画面の背景を削除
    DeleteGO(m_spriteBackground);
    DeleteGO(m_bgmTitle);

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
    if (m_player->GetGameEnd() == false) {
        if (m_flagStartCountdown == false) {
            return;
        }


        StartCountdown();
        return;
    }

    NewGOResultScene();
    for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
        m_result->SetRank(playerNum, m_rank[playerNum]);
    }

    DeleteGO(m_stage);
    DeleteGO(m_player);
    DeleteGO(m_enemyAI);
    DeleteGO(m_score);
    DeleteGO(m_fontStartCountdown);


    m_gameStatus = GameStatus::result;
}

void Game::StartCountdown()
{
    --m_countStartCountdown;

    //カウントダウンフォントを非表示
    if (m_countStartCountdown == COUNTDOWN_DEACTIVATE) {
        m_fontStartCountdown->Deactivate();
        m_flagStartCountdown = false;
    }
    //カウント０
    else if (m_countStartCountdown == COUNTDOWN_0) {
        m_fontStartCountdown->SetText(L"Start!");
        m_StopOperation = false;
        m_seGameStart->Play(false);
    }
    //カウント１
    else if (m_countStartCountdown == COUNTDOWN_1) {
        m_fontStartCountdown->SetText(1);
        m_seCount->Play(false);
    }
    //カウント２
    else if (m_countStartCountdown == COUNTDOWN_2) {
        m_fontStartCountdown->SetText(2);
        m_seCount->Play(false);
    }
    //カウント３
    else if (m_countStartCountdown == COUNTDOWN_3) {
        m_fontStartCountdown->SetText(3);
        m_seCount->Play(false);
    }
}

void Game::NewGOResultScene() {
    m_result = NewGO<Result>(igo::PRIORITY_CLASS);
}
////////////////////////////////////////////////////////////
// リザルトシーンの処理
////////////////////////////////////////////////////////////
void Game::ResultScene() {
    if (m_result->GetFlagFinish() == false) {
        return;
    }
   // m_resultselect = m_result->GetSelect();
    DeleteGO(m_result);
    m_spriteBackground = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground->Init(filePath::dds::BACKGROUND);
    m_StopOperation = true;
    m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;
    m_flagStartCountdown = true;
    switch (m_resultselect) {
    case 0:
        NewGOGameScene();
        m_gameStatus = GameStatus::loadingGame;
        m_loadStatus = LoadingStatus::preparingForLoading;
        break;
    case 1:
        NewGOPlayerSelectScene();
        m_gameStatus = GameStatus::playerSelect;
        break;
    case 2:
        NewGOModeSelectScene();
        m_gameStatus = GameStatus::modeSelect;
        break;
    case 3:
        exit(EXIT_SUCCESS);
        break;
    }
}