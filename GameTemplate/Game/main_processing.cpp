#include "stdafx.h"
#include "main_processing.h"

#include "title.h"
#include "mode_select.h"
#include "player_select.h"
#include "CPU_strength_select.h"
#include "game.h"
#include "player.h"
#include "EnemyAI.h"

#include "game_camera.h"


#include "Result.h"
#include "mode_select.h"
#include "pause.h"



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



MainProcessing::MainProcessing()
{

}

MainProcessing::~MainProcessing()
{
    //タイトルシーン
    DeleteGO(m_title);

    //モードセレクトシーン
    DeleteGO(m_modeSelect);

    //プレイヤーセレクトシーン
    DeleteGO(m_playerSelect);

    //CPUの難易度選択シーン
    DeleteGO(m_CPUStrengthSelect);

    //選択画面の背景を削除
    for (int i = 0; i < 7; i++) {
        DeleteGO(m_spriteBackground[i]);
    }
    DeleteGO(m_bgmTitle);

    //ゲームシーン
    DeleteGO(m_game);
    DeleteGO(m_fontStartCountdown);

    //リザルトシーン
    DeleteGO(m_result);

    DeleteGO(m_gameCamera);

    DeleteGO(m_seCancel);
    DeleteGO(m_seCount);
    DeleteGO(m_seGameStart);
}

bool MainProcessing::Start()
{
    m_flagProcessing = true;

    //背景の初期化
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

    //フェードの処理をNewGO
    m_fade = NewGO<Fade>(igo::PRIORITY_CLASS);

    //選択画面のBGMを初期化、再生
    m_bgmTitle = NewGO<SoundBGM>(igo::PRIORITY_CLASS);
    m_bgmTitle->Init(filePath::bgm::TITLE);
    m_bgmTitle->SetVolume(0.5f);
    m_bgmTitle->Play(true);

    //キャンセルのSEを初期化    
    m_seCancel = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seCancel->Init(filePath::se::CANCEL);

    //////////////////////////////
    // シーンごとの処理に使用するもののNewGO
    //////////////////////////////

    //タイトルシーン
    m_title = NewGO<Title>(igo::PRIORITY_CLASS);

    //モードセレクトシーン
    m_modeSelect = NewGO<ModeSelect>(igo::PRIORITY_CLASS);

    //プレイヤーセレクトシーン
    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_CLASS);

    //CPUの難易度選択シーン
    m_CPUStrengthSelect = NewGO<CPUStrengthSelect>(igo::PRIORITY_CLASS);

    //ゲームシーン
    m_game = NewGO<Game>(igo::PRIORITY_CLASS);
    m_gameCamera = NewGO<GameCamera>(igo::PRIORITY_CLASS);

    m_player = NewGO<Player>(igo::PRIORITY_CLASS, igo::CLASS_NAME_PLAYER);
    //m_enemyAI = NewGO<EnemyAI>(igo::PRIORITY_CLASS, igo::CLASS_NAME_ENEMYAI);

    m_fontStartCountdown = NewGO<FontRender>(igo::PRIORITY_FONT);
    m_fontStartCountdown->Init(L"");
    m_fontStartCountdown->Deactivate();

    //リザルトシーン
    m_result = NewGO<Result>(igo::PRIORITY_CLASS);

    
    for (int countNum = 0; countNum < 4; countNum++) {
        m_spriteCountdown[countNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteCountdown[countNum]->Init(filePath::dds::COUNT[countNum]);
        m_spriteCountdown[countNum]->Deactivate();
    }


    //カウントダウンのSE
    m_seCount = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seCount->Init(filePath::se::COUNT);
    m_seGameStart = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seGameStart->Init(filePath::se::GAME_START);

    m_title->Init();

    return true;
}

////////////////////////////////////////////////////////////
// 毎フレームする処理
////////////////////////////////////////////////////////////

void MainProcessing::Update()
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

    if (m_pause_stage==true)
    {
        m_spriteCountdown[0]->Deactivate();
        m_spriteCountdown[1]->Deactivate();
        m_spriteCountdown[2]->Deactivate();
        m_spriteCountdown[3]->Deactivate();
        m_game->Finish();
        m_game->Init();
        NextRound();
        m_pause_stage = false;
    }

    if (m_pause_title==true)
    {
        m_spriteCountdown[0]->Deactivate();
        m_spriteCountdown[1]->Deactivate();
        m_spriteCountdown[2]->Deactivate();
        m_spriteCountdown[3]->Deactivate();
        m_game->Finish();
        NextRound();
        m_title->Init();
        m_gameStatus = GameStatus::title;
        m_pause_title = false();

        //背景を初期状態に戻す
        m_spriteBackground[0]->SetPosition({ 0.0f,0.0f });
        m_spriteBackground[1]->SetPosition({ 1280.0f,0.0f });
        m_spriteBackground[2]->SetPosition({ 0.0f,-720.0f });
        m_spriteBackground[3]->SetPosition({ 1280.0f,-720.0f });
        m_spriteBackground[4]->SetPosition({ 2560.0f,-720.0f });
        m_spriteBackground[5]->SetPosition({ 1280.0f,-1440.0f });
        m_spriteBackground[6]->SetPosition({ 2560.0f,-1440.0f });

        //選択画面の背景を表示
        for (int i = 0; i < 7; i++) {
            m_spriteBackground[i]->Activate();
        }

        m_flagGameStart = false;
    }
}

void MainProcessing::DrawBackground()
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

void MainProcessing::TitleScene()
{
    if (m_title->GetFlagFinish() == false) {
        return;
    }

    DecisionTitleScene();
}

void MainProcessing::DecisionTitleScene()
{
    m_modeSelect->Init();
    m_title->Finish();
    
    m_gameStatus = GameStatus::modeSelect;
}

////////////////////////////////////////////////////////////
// モードセレクトシーンの処理
////////////////////////////////////////////////////////////

void MainProcessing::ModeSelectScene()
{
    if (m_modeSelect->GetFlagFinish() == false) {
        //キャンセル
        if (g_pad[con::player_1]->IsTrigger(enButtonB)) {
            CancelModeSelectScene();
        }

        return;
    }

    DecisionModeSelectScene();
}

void MainProcessing::DecisionModeSelectScene()
{
    m_playerSelect->Init();
    m_modeSelect->Finish();
   
    m_gameStatus = GameStatus::playerSelect;
}

void MainProcessing::CancelModeSelectScene()
{
    m_seCancel->Play(false);

    m_title->Init();
    m_modeSelect->Finish();

    m_gameStatus = GameStatus::title;
}

////////////////////////////////////////////////////////////
// プレイヤーセレクトシーンの処理
////////////////////////////////////////////////////////////

void MainProcessing::PlayerSelectScene()
{
    if (m_playerSelect->GetFlagFinish() == false) {
        //キャンセル
        if (g_pad[con::player_1]->IsTrigger(enButtonB)) {
            CancelPlayerSelectScene();
        }

        return;
    }

    DecisionPlayerSelectScene();
}

void MainProcessing::DecisionPlayerSelectScene()
{
    m_CPUStrengthSelect->Init();
    m_playerSelect->Finish();
    m_gameStatus = GameStatus::CPUStrengthSelect;
}

void MainProcessing::CancelPlayerSelectScene()
{
    m_seCancel->Play(false);

    m_modeSelect->Init();;
    m_playerSelect->Finish();

    m_gameStatus = GameStatus::modeSelect;
}

////////////////////////////////////////////////////////////
// CPUの難易度選択シーンの処理
////////////////////////////////////////////////////////////

void MainProcessing::CPUStrengthSelectScene()
{
    if (m_CPUStrengthSelect->GetFlagFinish() == false) {
        //キャンセル
        if (g_pad[con::player_1]->IsTrigger(enButtonB)) {
            CancelCPUStrengthSelectScene();
        }

        return;
    }

    DecisionCPUStrengthSelectScene();
}

void MainProcessing::DecisionCPUStrengthSelectScene()
{
    m_CPUStrengthSelect->Finish();

    m_gameStatus = GameStatus::loadingGame;
    m_loadStatus = LoadingStatus::preparingForLoading;
}

void MainProcessing::CancelCPUStrengthSelectScene()
{
    m_seCancel->Play(false);

    m_playerSelect->Init();;
    m_CPUStrengthSelect->Finish();

    m_gameStatus = GameStatus::playerSelect;
}

////////////////////////////////////////////////////////////
// ゲームシーンのためのロード
////////////////////////////////////////////////////////////

void MainProcessing::LoadingGameScene()
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

void MainProcessing::PreparingForLoading()
{
    if (m_startPreparingForLoading == false) {
        m_fade->StartFadeOut();
        m_startPreparingForLoading = true;
    }

    if (m_fade->IsFadeOutProgress() == false) {
        m_loadStatus = LoadingStatus::loading;
    }
}

void MainProcessing::Loading()
{
    //モードの選択情報を渡す
    m_game->SetRuleSuddenDeath(m_ruleSelect);
    m_game->SetMaxPlayer(m_maxPlayer);
    m_player->SetDifficultyLevel(m_dilevel);
 // m_enemyAI->SetDifficultyLevel(m_dilevel);
    m_game->SetDilevel(m_dilevel);

    m_game->Init();



    //選択画面の背景を非表示
    for (int i = 0; i < 7; i++) {
        m_spriteBackground[i]->Deactivate();
    }
    //BGMを止める
    m_bgmTitle->Stop();


    m_loadStatus = LoadingStatus::endOfLoading;
    m_flagGameStart = true;
    m_gameEnd = false;
}

void MainProcessing::EndOfLoading()
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

void MainProcessing::GameScene()
{
    if (m_gameEnd == false) {
        if (m_flagStartCountdown == false) {
            return;
        }


        StartCountdown();
        return;
    }

    FinishGameScene();
}

void MainProcessing::StartCountdown()
{
    --m_countStartCountdown;

    //カウントダウンフォントを非表示
    if (m_countStartCountdown == COUNTDOWN_DEACTIVATE) {
        m_spriteCountdown[0]->Deactivate();
        m_flagStartCountdown = false;
    }
    //カウント０
    else if (m_countStartCountdown == COUNTDOWN_0) {
        m_spriteCountdown[1]->Deactivate();
        m_spriteCountdown[0]->Activate();
        m_StopOperation = false;
        m_seGameStart->Play(false);
    }
    //カウント１
    else if (m_countStartCountdown == COUNTDOWN_1) {
        m_spriteCountdown[2]->Deactivate();
        m_spriteCountdown[1]->Activate();
        m_seCount->Play(false);
    }
    //カウント２
    else if (m_countStartCountdown == COUNTDOWN_2) {
        m_spriteCountdown[3]->Deactivate();
        m_spriteCountdown[2]->Activate();
        m_seCount->Play(false);
    }
    //カウント３
    else if (m_countStartCountdown == COUNTDOWN_3) {
        m_spriteCountdown[3]->Activate();
        m_seCount->Play(false);
    }
}

void MainProcessing::FinishGameScene()
{
    m_game->Finish();

    m_spriteCountdown[0]->Deactivate();
    m_spriteCountdown[1]->Deactivate();
    m_spriteCountdown[2]->Deactivate();
    m_spriteCountdown[3]->Deactivate();

    //背景を初期状態に戻す
    m_spriteBackground[0]->SetPosition({ 0.0f,0.0f });
    m_spriteBackground[1]->SetPosition({ 1280.0f,0.0f });
    m_spriteBackground[2]->SetPosition({ 0.0f,-720.0f });
    m_spriteBackground[3]->SetPosition({ 1280.0f,-720.0f });
    m_spriteBackground[4]->SetPosition({ 2560.0f,-720.0f });
    m_spriteBackground[5]->SetPosition({ 1280.0f,-1440.0f });
    m_spriteBackground[6]->SetPosition({ 2560.0f,-1440.0f });

    //選択画面の背景を表示
    for (int i = 0; i < 7; i++) {
        m_spriteBackground[i]->Activate();
    }

    m_flagGameStart = false;

    m_startPreparingForLoading = false;
    m_startEndOfLoading = false;

    for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
        m_result->SetRank(playerNum, m_rank[playerNum]);
    }

    m_result->Init();

    m_bgmTitle->Play(true);

    m_gameStatus = GameStatus::result;
}

////////////////////////////////////////////////////////////
// リザルトシーンの処理
////////////////////////////////////////////////////////////

void MainProcessing::ResultScene()
{
    if (m_result->GetFlagFinish() == false) {
        return;
    }

    m_result->Finish();

    m_StopOperation = true;
    m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;
    m_flagStartCountdown = true;

    switch (m_resultselect) {
    case 0: //もう一度プレイ
        DecisionCPUStrengthSelectScene();
        m_gameStatus = GameStatus::loadingGame;
        m_loadStatus = LoadingStatus::preparingForLoading;
        break;
    case 1: //プレイヤー数選択
        DecisionModeSelectScene();
        m_gameStatus = GameStatus::playerSelect;
        break;
    case 2: //モード選択
        DecisionTitleScene();
        m_gameStatus = GameStatus::modeSelect;
        break;
    case 3: //ゲーム終了
        exit(EXIT_SUCCESS);
        break;
    }
}

void MainProcessing::NextRound()
{
    m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;
    m_flagStartCountdown = true;
    m_StopOperation = true;
    //m_flagTitleScene = true;
    //m_flagPlayerSelectScene = false;
    //m_flagGameScene = false;
    //m_maxPlayer = 0;
}