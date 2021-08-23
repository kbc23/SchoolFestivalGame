#include "stdafx.h"
#include "main_processing.h"

#include "title.h"
#include "mode_select.h"
#include "player_select.h"
#include "CPU_strength_select.h"
#include "game.h"
#include "game_camera.h"
#include "Result.h"
#include "pause.h"



namespace
{
    ////////////////////////////////////////////////////////////
    // 初期化
    ////////////////////////////////////////////////////////////

    const Vector2 BACKGROUND_START_POSITION[MainProcessing::m_MAX_BACKGROUND] =
    {
        { 0.0f,0.0f },
        { 1280.0f,0.0f },
        { 0.0f,-720.0f },
        { 1280.0f,-720.0f },
        { 2560.0f,-720.0f },
        { 1280.0f,-1440.0f },
        { 2560.0f,-1440.0f }
    };

    constexpr const char* SPRITE_BACKGROUND[MainProcessing::m_MAX_BACKGROUND] =
    {
        filePath::dds::BACKGROUND,
        filePath::dds::BACKGROUND,
        filePath::dds::BACKGROUND_2,
        filePath::dds::BACKGROUND_2,
        filePath::dds::BACKGROUND_2,
        filePath::dds::BACKGROUND,
        filePath::dds::BACKGROUND
    };

    ////////////////////////////////////////////////////////////
    // ボリューム
    ////////////////////////////////////////////////////////////

    const float TITLE_BGM_VOLUME = 0.5f; //タイトル画面のBGMのボリューム

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    const float MOVE_BACKGROUND = 1010.0f;      //背景の移動量
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

    //ゲームシーン
    DeleteGO(m_game);

    //リザルトシーン
    DeleteGO(m_result);

    //選択画面の背景を削除
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        DeleteGO(m_spriteBackground[backgroundNum]);
    }

    //タイトル画面のBGM
    DeleteGO(m_bgmTitle);

    //カメラ
    DeleteGO(m_gameCamera);

    //キャンセルボタンのSE
    DeleteGO(m_seCancel);
}

bool MainProcessing::Start()
{
    ////////////////////////////////////////////////////////////
    // 背景
    ////////////////////////////////////////////////////////////

    //画像の配列番号と表示位置（0の位置が画面）
    //0 1
    //2 3 4
    //  5 6

    //背景の初期化
    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_MAX_BACKGROUND; spriteNum++) {
        m_spriteBackground[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND); //NewGO
        m_spriteBackground[spriteNum]->Init(SPRITE_BACKGROUND[spriteNum]); //初期化
        m_spriteBackground[spriteNum]->SetPosition(BACKGROUND_START_POSITION[spriteNum]); //位置
    }

    //フェードの処理を初期化
    m_fade = NewGO<Fade>(igo::PRIORITY_CLASS);

    ////////////////////////////////////////////////////////////
    // BGM
    ////////////////////////////////////////////////////////////

    //選択画面のBGMを初期化、再生
    m_bgmTitle = NewGO<SoundBGM>(igo::PRIORITY_CLASS); //NewGO
    m_bgmTitle->Init(filePath::bgm::TITLE); //初期化
    m_bgmTitle->SetVolume(TITLE_BGM_VOLUME); //音量調整
    m_bgmTitle->Play(true); //再生

    //BGM全体の音量を設定
    m_bgmTitle->SetBGMVolume(0.0f);

    ////////////////////////////////////////////////////////////
    // SE
    ////////////////////////////////////////////////////////////

    //キャンセルのSEを初期化    
    m_seCancel = NewGO<SoundSE>(igo::PRIORITY_CLASS); //NewGO
    m_seCancel->Init(filePath::se::CANCEL); //初期化

    //SE全体の音量を設定
    m_seCancel->SetSEVolume(0.5f);

    ////////////////////////////////////////////////////////////
    // クラス
    ////////////////////////////////////////////////////////////

    //カメラを初期化
    m_gameCamera = NewGO<GameCamera>(igo::PRIORITY_CLASS);

    //シーンのNewGO
    m_title = NewGO<Title>(igo::PRIORITY_CLASS);
    m_modeSelect = NewGO<ModeSelect>(igo::PRIORITY_CLASS);
    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_CLASS);
    m_CPUStrengthSelect = NewGO<CPUStrengthSelect>(igo::PRIORITY_CLASS);
    m_game = NewGO<Game>(igo::PRIORITY_CLASS, igo::CLASS_NAME_GAME);
    m_result = NewGO<Result>(igo::PRIORITY_CLASS, igo::CLASS_NAME_RESULT);

    //シーンの状態の切り替え
    m_gameStatus = GameStatus::startLoading;

    return true;
}

////////////////////////////////////////////////////////////
// 毎フレームする処理
////////////////////////////////////////////////////////////

void MainProcessing::Update()
{
    switch (m_gameStatus) {
    case GameStatus::startLoading:
        StartLoading();
        break;
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

    //ゲームシーンが始まっているか
    if (m_flagGameStart == false) {
        //背景の描画
        DrawBackground();
    }
}

void MainProcessing::DrawBackground()
{
    //背景のアニメーション
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->SetPosition(
            { (m_spriteBackground[backgroundNum]->GetPosition().x - con::GAME_SCREEN_W / MOVE_BACKGROUND),
              m_spriteBackground[backgroundNum]->GetPosition().y + con::GAME_SCREEN_H / MOVE_BACKGROUND
            }
        );
    }

    //背景を初期位置にリセット
    if (m_spriteBackground[m_MAX_BACKGROUND - 1]->GetPosition().x <= 0.0f) {
        for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
            m_spriteBackground[backgroundNum]->SetPosition(BACKGROUND_START_POSITION[backgroundNum]);
        }
    }
}

////////////////////////////////////////////////////////////
// シーンごとの処理
////////////////////////////////////////////////////////////


//////////////////////////////
// ゲーム開始時のロードの処理
//////////////////////////////

void MainProcessing::StartLoading()
{
    m_title->Init();

    m_gameStatus = GameStatus::title;
}

//////////////////////////////
// タイトルシーンの処理
//////////////////////////////

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

//////////////////////////////
// モードセレクトシーンの処理
//////////////////////////////

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

//////////////////////////////
// プレイヤーセレクトシーンの処理
//////////////////////////////

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

//////////////////////////////
// CPUの難易度選択シーンの処理
//////////////////////////////

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

//////////////////////////////
// ゲームシーンのためのロード
//////////////////////////////

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
    m_game->GetGameDataObject()->SetMaxPlayer(m_maxPlayer);
    m_game->GetGameDataObject()->SetCPULevel(m_CPULevel);

    m_game->Init();

    //選択画面の背景を非表示
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->Deactivate();
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

//////////////////////////////
// ゲームシーンの処理
//////////////////////////////

void MainProcessing::GameScene()
{
    if (m_gameEnd == false) {
        return;
    }

    FinishGameScene();
}

void MainProcessing::FinishGameScene()
{
    m_game->Finish();

    //背景を初期状態に戻して、表示
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->SetPosition(BACKGROUND_START_POSITION[backgroundNum]);
        m_spriteBackground[backgroundNum]->Activate();
    }

    m_flagGameStart = false;

    m_startPreparingForLoading = false;
    m_startEndOfLoading = false;

    m_result->Init();

    m_bgmTitle->Play(true); //再生

    m_gameStatus = GameStatus::result;
}

//////////////////////////////
// リザルトシーンの処理
//////////////////////////////

void MainProcessing::ResultScene()
{
    if (m_result->GetFlagFinish() == false) {
        return;
    }

    m_result->Finish();

    switch (m_result->GetCursorPosition()) {
    //もう一度プレイ
    case choice::ResultCommand::playOneMoreTime:
        //ゲームのロード処理をおこなう
        m_gameStatus = GameStatus::loadingGame;
        m_loadStatus = LoadingStatus::preparingForLoading;
        break;
    //プレイヤー数選択
    case choice::ResultCommand::chooseTheNumberOfPlayers:
        m_playerSelect->Init();
        m_gameStatus = GameStatus::playerSelect;
        break;
    //モード選択
    case choice::ResultCommand::chooseARule:
        m_modeSelect->Init();
        m_gameStatus = GameStatus::modeSelect;
        break;
    //ゲーム終了
    case choice::ResultCommand::exitGame:
        exit(EXIT_SUCCESS);
        break;
    }
}

////////////////////////////////////////////////////////////
// ポーズ画面と関係
////////////////////////////////////////////////////////////

void MainProcessing::PauseRetry()
{
    //ゲームシーンをやり直す
    m_game->Finish();
    m_game->Init();
}

void MainProcessing::PauseTitle()
{
    //ゲームシーンからタイトルシーンに移行
    m_game->Finish();
    m_title->Init();

    m_gameStatus = GameStatus::title;

    //背景を初期状態に戻して、表示
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->SetPosition(BACKGROUND_START_POSITION[backgroundNum]); //位置
        m_spriteBackground[backgroundNum]->Activate(); //表示
    }

    //BGMを再生
    m_bgmTitle->Play(true);

    m_flagGameStart = false;
}