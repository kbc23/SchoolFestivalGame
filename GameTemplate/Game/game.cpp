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
#include "mode_select.h"



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
    m_spriteBackground[0] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[0]->Init(filePath::dds::BACKGROUND);
    m_spriteBackground[1] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[1]->Init(filePath::dds::BACKGROUND);
    m_spriteBackground[2] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[2]->Init(filePath::dds::BACKGROUND_2);
    m_spriteBackground[3] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[3]->Init(filePath::dds::BACKGROUND_2);
    m_spriteBackground[4] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[4]->Init(filePath::dds::BACKGROUND_2);
    m_spriteBackground[5] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[5]->Init(filePath::dds::BACKGROUND);
    m_spriteBackground[6] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[6]->Init(filePath::dds::BACKGROUND);

    //画像の配列番号と表示位置（0の位置が画面）
    //0 1
    //2 3 4
    //  5 6

    m_spriteBackground[1]->SetPosition({ 1280.0f,0.0f });
    m_spriteBackground[2]->SetPosition({ 0.0f,-720.0f });
    m_spriteBackground[3]->SetPosition({ 1280.0f,-720.0f });
    m_spriteBackground[4]->SetPosition({ 2560.0f,-720.0f });
    m_spriteBackground[5]->SetPosition({ 1280.0f,-1440.0f });
    m_spriteBackground[6]->SetPosition({ 2560.0f,-1440.0f });




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

    if (m_flagGameStart == false) {
        DrawBackground();
    }
}

////////////////////////////////////////////////////////////
// 毎フレームする処理
////////////////////////////////////////////////////////////

void Game::DrawBackground()
{
    for (int i = 0; i < 7; i++) {
        m_spriteBackground[i]->SetPosition({ m_spriteBackground[i]->GetPosition().x - 1280.0f / 1010.0f,
                                                m_spriteBackground[i]->GetPosition().y + 720.0f / 1010.0f }
        );
    }


    if (m_spriteBackground[6]->GetPosition().x <= 0.0f) {
        m_spriteBackground[0]->SetPosition({ 0.0f,0.0f });
        m_spriteBackground[1]->SetPosition({ 1280.0f,0.0f });
        m_spriteBackground[2]->SetPosition({ 0.0f,-720.0f });
        m_spriteBackground[3]->SetPosition({ 1280.0f,-720.0f });
        m_spriteBackground[4]->SetPosition({ 2560.0f,-720.0f });
        m_spriteBackground[5]->SetPosition({ 1280.0f,-1440.0f });
        m_spriteBackground[6]->SetPosition({ 2560.0f,-1440.0f });
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
    //Stageクラスに選択されたプレイヤー人数を渡す。
    //m_stage->SetMaxPlayer(m_maxPlayer);
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
    if (m_ruleSelect==true) {
        m_rule1 = NewGO<Rule1>(igo::PRIORITY_CLASS, igo::CLASS_NAME_RULE1);
    }
    m_player = NewGO<Player>(igo::PRIORITY_CLASS, igo::CLASS_NAME_PLAYER);
    m_enemyAI = NewGO<EnemyAI>(igo::PRIORITY_CLASS, igo::CLASS_NAME_ENEMYAI);
    m_gameCamera = NewGO<GameCamera>(igo::PRIORITY_CLASS);
    m_score = NewGO<Score>(igo::PRIORITY_CLASS, igo::CLASS_NAME_SCORE);
    m_fontStartCountdown = NewGO<FontRender>(igo::PRIORITY_FONT);
    m_fontStartCountdown->Init(L"");

    //Playerクラスに選択されたプレイヤー人数を渡す。
    m_player->SetMaxPlayer(m_maxPlayer);
    m_stage->SetMaxPlayer(m_maxPlayer);
    m_enemyAI->SetDifficultyLevel(m_dilevel);

    //選択画面の背景を削除
    for (int i = 0; i < 7; i++) {
        DeleteGO(m_spriteBackground[i]);
    }
    DeleteGO(m_bgmTitle);

    m_loadStatus = LoadingStatus::endOfLoading;
    m_flagGameStart = true;
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
        m_fontStartCountdown->Activate();
        m_fontStartCountdown->SetText(3);
        m_seCount->Play(false);
    }
}

void Game::NewGOResultScene() {
    m_result = NewGO<Result>(igo::PRIORITY_CLASS);

    m_spriteBackground[0] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[0]->Init(filePath::dds::BACKGROUND);
    m_spriteBackground[1] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[1]->Init(filePath::dds::BACKGROUND);
    m_spriteBackground[2] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[2]->Init(filePath::dds::BACKGROUND_2);
    m_spriteBackground[3] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[3]->Init(filePath::dds::BACKGROUND_2);
    m_spriteBackground[4] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[4]->Init(filePath::dds::BACKGROUND_2);
    m_spriteBackground[5] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[5]->Init(filePath::dds::BACKGROUND);
    m_spriteBackground[6] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground[6]->Init(filePath::dds::BACKGROUND);
    m_spriteBackground[1]->SetPosition({ 1280.0f,0.0f });
    m_spriteBackground[2]->SetPosition({ 0.0f,-720.0f });
    m_spriteBackground[3]->SetPosition({ 1280.0f,-720.0f });
    m_spriteBackground[4]->SetPosition({ 2560.0f,-720.0f });
    m_spriteBackground[5]->SetPosition({ 1280.0f,-1440.0f });
    m_spriteBackground[6]->SetPosition({ 2560.0f,-1440.0f });

    m_flagGameStart = false;

    m_startPreparingForLoading = false;
    m_startEndOfLoading = false;

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
   // m_spriteBackground[0] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
  //  m_spriteBackground[0]->Init(filePath::dds::BACKGROUND);
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

void Game::NextRound()
{
    m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;
    m_flagStartCountdown = true;
    m_StopOperation = true;
    //m_flagTitleScene = true;
    //m_flagPlayerSelectScene = false;
    //m_flagGameScene = false;
    //m_maxPlayer = 0;
}