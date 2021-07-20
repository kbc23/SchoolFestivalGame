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
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    const float COUNTDOWN_3 = 180.0f;
    const float COUNTDOWN_2 = 120.0f;
    const float COUNTDOWN_1 = 60.0f;
    const float COUNTDOWN_0 = 0.0f;
    const float COUNTDOWN_DEACTIVATE = -60.0f;

    const int SPRITE_COUNTDOWN_0 = 0;
    const int SPRITE_COUNTDOWN_1 = 1;
    const int SPRITE_COUNTDOWN_2 = 2;
    const int SPRITE_COUNTDOWN_3 = 3;

    ////////////////////////////////////////////////////////////
    // �ʒu���
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


    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    const float MOVE_BACKGROUND = 1010.0f;      //�w�i�̈ړ���
    const float TITLE_BGM_VOLUME = 0.5f;
}



MainProcessing::MainProcessing()
{

}

MainProcessing::~MainProcessing()
{
    DeleteGO(m_spriteLoading);

    //�^�C�g���V�[��
    DeleteGO(m_title);

    //���[�h�Z���N�g�V�[��
    DeleteGO(m_modeSelect);

    //�v���C���[�Z���N�g�V�[��
    DeleteGO(m_playerSelect);

    //CPU�̓�Փx�I���V�[��
    DeleteGO(m_CPUStrengthSelect);

    //�I����ʂ̔w�i���폜
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        DeleteGO(m_spriteBackground[backgroundNum]);
    }
    DeleteGO(m_bgmTitle);

    //�Q�[���V�[��
    DeleteGO(m_game);

    //���U���g�V�[��
    DeleteGO(m_result);

    DeleteGO(m_gameCamera);

    DeleteGO(m_seCancel);
    DeleteGO(m_seCount);
    DeleteGO(m_seGameStart);
}

bool MainProcessing::Start()
{
    m_flagProcessing = true;

    m_spriteLoading = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteLoading->Init(filePath::dds::START_LOADING);
    m_spriteLoading->SetMulColorW(SRns::TRANSPARENCY_0);

    return true;
}

////////////////////////////////////////////////////////////
// ���t���[�����鏈��
////////////////////////////////////////////////////////////

void MainProcessing::Update()
{
    switch (m_gameStatus) {
    case GameStatus::startLoading:
        StartLoadingScene();
        break;
    case GameStatus::title:
        TitleScene();
        break;
    case GameStatus::modeSelect:
        ModeSelectScene();
        break;
    case GameStatus::playerSelect: //�v���C���[�̐l�����S�l�̂Ƃ��ACPU�̋����̑I����ʂ��X�L�b�v���鏈����ǉ����邱�ƁB
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
        MessageBoxA(nullptr, "�Q�[���̑J�ڂɂăG���[���������܂����B", "�G���[", MB_OK);
        return;
    }

    if (m_flagGameStart == false && m_gameStatus != GameStatus::startLoading) {
        DrawBackground();
    }

    // TODO: ���̉��ɂ���|�[�Y�֘A�̏�����Pause�N���X�ŏ����������Ȃ��悤�ɕύX���邱��

    if (m_pause_stage == true)
    {
        for (int countdownNum = con::FIRST_ELEMENT_ARRAY; countdownNum < m_MAX_COUNTDOWN; countdownNum++) {
            m_spriteCountdown[countdownNum]->Deactivate();
        }

        m_game->Finish();
        m_game->Init();
        NextRound(); //�J�E���g�_�E���̏�����
        m_pause_stage = false;
    }

    if (m_pause_title == true)
    {
        for (int countdownNum = con::FIRST_ELEMENT_ARRAY; countdownNum < m_MAX_COUNTDOWN; countdownNum++) {
            m_spriteCountdown[countdownNum]->Deactivate();
        }

        m_game->Finish();
        NextRound(); //�J�E���g�_�E���̏�����
        m_title->Init();
        m_gameStatus = GameStatus::title;
        m_pause_title = false();

        //�w�i��������Ԃɖ߂��āA�\��
        for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
            m_spriteBackground[backgroundNum]->SetPosition(BACKGROUND_START_POSITION[backgroundNum]);
            m_spriteBackground[backgroundNum]->Activate();
        }

        //BGM���Đ�
        m_bgmTitle->Play(true);

        m_flagGameStart = false;
    }

    // TODO END
}

void MainProcessing::DrawBackground()
{
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->SetPosition(
            { (m_spriteBackground[backgroundNum]->GetPosition().x - con::GAME_SCREEN_W / MOVE_BACKGROUND),
              m_spriteBackground[backgroundNum]->GetPosition().y + con::GAME_SCREEN_H / MOVE_BACKGROUND
            }
        );
    }

    //�w�i�������ʒu�Ƀ��Z�b�g
    if (m_spriteBackground[6]->GetPosition().x <= 0.0f) {
        for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
            m_spriteBackground[backgroundNum]->SetPosition(BACKGROUND_START_POSITION[backgroundNum]);
        }
    }
}

////////////////////////////////////////////////////////////
// �ŏ��̓ǂݍ���
////////////////////////////////////////////////////////////

void MainProcessing::StartLoadingScene()
{
    switch (m_startLoadingStatus) {
    case StartLoadingStatus::preparation:
        StartLoadingPreparation();
        break;
    case StartLoadingStatus::loading:
        StartLoading();
        break;
    case StartLoadingStatus::finish:
        StartLoadingFinish();
        break;
    default:
        MessageBoxA(nullptr, "�ǂݍ��݂ɂăG���[���������܂����B", "�G���[", MB_OK);
        return;

    }
}

void MainProcessing::StartLoadingPreparation()
{
    m_spriteLoading->SetMulColorW(m_spriteLoading->GetMulColorW() + 0.05f);

    if (m_spriteLoading->GetMulColorW() >= SRns::TRANSPARENCY_100) {
        m_spriteLoading->SetMulColorW(SRns::TRANSPARENCY_100);
        m_startLoadingStatus = StartLoadingStatus::loading;
    }
}

void MainProcessing::StartLoading()
{
    //�w�i�̏�����
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

    //�摜�̔z��ԍ��ƕ\���ʒu�i0�̈ʒu����ʁj
    //0 1
    //2 3 4
    //  5 6

    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->SetPosition(BACKGROUND_START_POSITION[backgroundNum]);
        m_spriteBackground[backgroundNum]->Deactivate();
    }
    


    //�t�F�[�h�̏�����NewGO
    m_fade = NewGO<Fade>(igo::PRIORITY_CLASS);

    //�I����ʂ�BGM���������A�Đ�
    m_bgmTitle = NewGO<SoundBGM>(igo::PRIORITY_CLASS);
    m_bgmTitle->Init(filePath::bgm::TITLE);
    m_bgmTitle->SetVolume(TITLE_BGM_VOLUME);

    //�L�����Z����SE��������    
    m_seCancel = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seCancel->Init(filePath::se::CANCEL);

    //////////////////////////////
    // �V�[�����Ƃ̏����Ɏg�p������̂�NewGO
    //////////////////////////////

    //�^�C�g���V�[��
    m_title = NewGO<Title>(igo::PRIORITY_CLASS);

    //���[�h�Z���N�g�V�[��
    m_modeSelect = NewGO<ModeSelect>(igo::PRIORITY_CLASS);

    //�v���C���[�Z���N�g�V�[��
    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_CLASS);

    //CPU�̓�Փx�I���V�[��
    m_CPUStrengthSelect = NewGO<CPUStrengthSelect>(igo::PRIORITY_CLASS);

    //�Q�[���V�[��
    m_game = NewGO<Game>(igo::PRIORITY_CLASS);
    m_gameCamera = NewGO<GameCamera>(igo::PRIORITY_CLASS);

    //���U���g�V�[��
    m_result = NewGO<Result>(igo::PRIORITY_CLASS);

    //�J�E���g�_�E����UI�̏�����
    for (int countdownNum = con::FIRST_ELEMENT_ARRAY; countdownNum < m_MAX_COUNTDOWN; countdownNum++) {
        m_spriteCountdown[countdownNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteCountdown[countdownNum]->Init(filePath::dds::COUNT[countdownNum]);
        m_spriteCountdown[countdownNum]->Deactivate();
    }


    //�J�E���g�_�E����SE
    m_seCount = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seCount->Init(filePath::se::COUNT);
    m_seGameStart = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seGameStart->Init(filePath::se::GAME_START);

    m_startLoadingStatus = StartLoadingStatus::finish;
}

void MainProcessing::StartLoadingFinish()
{
    m_spriteLoading->Deactivate();

    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->Activate();
    }

    m_title->Init();

    m_bgmTitle->Play(true);

    m_gameStatus = GameStatus::title;

}

////////////////////////////////////////////////////////////
// �^�C�g���V�[���̏���
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
// ���[�h�Z���N�g�V�[���̏���
////////////////////////////////////////////////////////////

void MainProcessing::ModeSelectScene()
{
    if (m_modeSelect->GetFlagFinish() == false) {
        //�L�����Z��
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
// �v���C���[�Z���N�g�V�[���̏���
////////////////////////////////////////////////////////////

void MainProcessing::PlayerSelectScene()
{
    if (m_playerSelect->GetFlagFinish() == false) {
        //�L�����Z��
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
// CPU�̓�Փx�I���V�[���̏���
////////////////////////////////////////////////////////////

void MainProcessing::CPUStrengthSelectScene()
{
    if (m_CPUStrengthSelect->GetFlagFinish() == false) {
        //�L�����Z��
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
// �Q�[���V�[���̂��߂̃��[�h
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
        MessageBoxA(nullptr, "�Q�[���̃��[�h�ɂăG���[���������܂����B", "�G���[", MB_OK);
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
    //���[�h�̑I������n��
    m_game->SetRuleSuddenDeath(m_ruleSelect);
    m_game->SetMaxPlayer(m_maxPlayer);
    //m_player->SetDifficultyLevel(m_dilevel);
    // m_enemyAI->SetDifficultyLevel(m_dilevel);
    m_game->SetDilevel(m_dilevel);

    m_game->Init();

    //�I����ʂ̔w�i���\��
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->Deactivate();
    }

    //BGM���~�߂�
    m_bgmTitle->Stop();

    m_flagCountSE3 = false;
    m_flagCountSE2 = false;
    m_flagCountSE1 = false;
    m_flagStartCountdownSE = false;


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
// �Q�[���V�[���̏���
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

    m_countStartCountdown -= g_gameTime->GetFrameDeltaTime();

    //�J�E���g�_�E���t�H���g���\��
    if (m_countStartCountdown <= COUNTDOWN_DEACTIVATE) {
        m_spriteCountdown[0]->Deactivate();
        m_flagStartCountdown = false;
    }
    //�J�E���g�O
    else if (m_countStartCountdown <= COUNTDOWN_0) {
        if (m_flagStartCountdownSE == false) {
            m_spriteCountdown[1]->Deactivate();
            m_spriteCountdown[0]->Activate();
            m_StopOperation = false;
            m_seGameStart->Play(false);
            m_flagStartCountdownSE = true;
        }
    }
    //�J�E���g�P
    else if (m_countStartCountdown <= COUNTDOWN_1) {
        if (m_flagCountSE1 == false) {
            m_spriteCountdown[2]->Deactivate();
            m_spriteCountdown[1]->Activate();
            m_seCount->Play(false);
            m_flagCountSE1 = true;
        }
    }
    //�J�E���g�Q
    else if (m_countStartCountdown <= COUNTDOWN_2) {
        if (m_flagCountSE2 == false) {
            m_spriteCountdown[3]->Deactivate();
            m_spriteCountdown[2]->Activate();
            m_seCount->Play(false);
            m_flagCountSE2 = true;
        }
    }
    //�J�E���g�R
    else if (m_countStartCountdown <= COUNTDOWN_3) {
        if (m_flagCountSE3 == false) {
            m_spriteCountdown[3]->Activate();
            m_seCount->Play(false);
            m_flagCountSE3 = true;
        }
    }
}

void MainProcessing::FinishGameScene()
{
    m_game->Finish();

    m_spriteCountdown[0]->Deactivate();
    m_spriteCountdown[1]->Deactivate();
    m_spriteCountdown[2]->Deactivate();
    m_spriteCountdown[3]->Deactivate();

    //�w�i��������Ԃɖ߂��āA�\��
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->SetPosition(BACKGROUND_START_POSITION[backgroundNum]);
        m_spriteBackground[backgroundNum]->Activate();
    }

    m_flagGameStart = false;

    m_startPreparingForLoading = false;
    m_startEndOfLoading = false;

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_result->SetRank(playerNum, m_rank[playerNum]);
    }

    m_result->Init();

    m_bgmTitle->Play(true);

    m_gameStatus = GameStatus::result;
}

////////////////////////////////////////////////////////////
// ���U���g�V�[���̏���
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
    case 0: //������x�v���C
        DecisionCPUStrengthSelectScene();
        m_gameStatus = GameStatus::loadingGame;
        m_loadStatus = LoadingStatus::preparingForLoading;
        break;
    case 1: //�v���C���[���I��
        DecisionModeSelectScene();
        m_gameStatus = GameStatus::playerSelect;
        break;
    case 2: //���[�h�I��
        DecisionTitleScene();
        m_gameStatus = GameStatus::modeSelect;
        break;
    case 3: //�Q�[���I��
        exit(EXIT_SUCCESS);
        break;
    }
}

void MainProcessing::NextRound()
{
    m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;
    m_flagStartCountdown = true;
    m_StopOperation = true;

    m_flagCountSE3 = false;
    m_flagCountSE2 = false;
    m_flagCountSE1 = false;
    m_flagStartCountdownSE = false;
    //m_flagTitleScene = true;
    //m_flagPlayerSelectScene = false;
    //m_flagGameScene = false;
    //m_maxPlayer = 0;
}