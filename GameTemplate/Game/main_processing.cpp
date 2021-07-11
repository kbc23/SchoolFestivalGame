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
    //�^�C�g���V�[��
    DeleteGO(m_title);

    //���[�h�Z���N�g�V�[��
    DeleteGO(m_modeSelect);

    //�v���C���[�Z���N�g�V�[��
    DeleteGO(m_playerSelect);

    //CPU�̓�Փx�I���V�[��
    DeleteGO(m_CPUStrengthSelect);

    //�I����ʂ̔w�i���폜
    for (int i = 0; i < 7; i++) {
        DeleteGO(m_spriteBackground[i]);
    }
    DeleteGO(m_bgmTitle);

    //�Q�[���V�[��
    DeleteGO(m_game);
    DeleteGO(m_fontStartCountdown);

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

    m_spriteBackground[1]->SetPosition({ 1280.0f,0.0f });
    m_spriteBackground[2]->SetPosition({ 0.0f,-720.0f });
    m_spriteBackground[3]->SetPosition({ 1280.0f,-720.0f });
    m_spriteBackground[4]->SetPosition({ 2560.0f,-720.0f });
    m_spriteBackground[5]->SetPosition({ 1280.0f,-1440.0f });
    m_spriteBackground[6]->SetPosition({ 2560.0f,-1440.0f });

    //�t�F�[�h�̏�����NewGO
    m_fade = NewGO<Fade>(igo::PRIORITY_CLASS);

    //�I����ʂ�BGM���������A�Đ�
    m_bgmTitle = NewGO<SoundBGM>(igo::PRIORITY_CLASS);
    m_bgmTitle->Init(filePath::bgm::TITLE);
    m_bgmTitle->SetVolume(0.5f);
    m_bgmTitle->Play(true);

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

    m_player = NewGO<Player>(igo::PRIORITY_CLASS, igo::CLASS_NAME_PLAYER);
    //m_enemyAI = NewGO<EnemyAI>(igo::PRIORITY_CLASS, igo::CLASS_NAME_ENEMYAI);

    m_fontStartCountdown = NewGO<FontRender>(igo::PRIORITY_FONT);
    m_fontStartCountdown->Init(L"");
    m_fontStartCountdown->Deactivate();

    //���U���g�V�[��
    m_result = NewGO<Result>(igo::PRIORITY_CLASS);

    
    for (int countNum = 0; countNum < 4; countNum++) {
        m_spriteCountdown[countNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteCountdown[countNum]->Init(filePath::dds::COUNT[countNum]);
        m_spriteCountdown[countNum]->Deactivate();
    }


    //�J�E���g�_�E����SE
    m_seCount = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seCount->Init(filePath::se::COUNT);
    m_seGameStart = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seGameStart->Init(filePath::se::GAME_START);

    m_title->Init();

    return true;
}

////////////////////////////////////////////////////////////
// ���t���[�����鏈��
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

        //�w�i��������Ԃɖ߂�
        m_spriteBackground[0]->SetPosition({ 0.0f,0.0f });
        m_spriteBackground[1]->SetPosition({ 1280.0f,0.0f });
        m_spriteBackground[2]->SetPosition({ 0.0f,-720.0f });
        m_spriteBackground[3]->SetPosition({ 1280.0f,-720.0f });
        m_spriteBackground[4]->SetPosition({ 2560.0f,-720.0f });
        m_spriteBackground[5]->SetPosition({ 1280.0f,-1440.0f });
        m_spriteBackground[6]->SetPosition({ 2560.0f,-1440.0f });

        //�I����ʂ̔w�i��\��
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
    m_player->SetDifficultyLevel(m_dilevel);
 // m_enemyAI->SetDifficultyLevel(m_dilevel);
    m_game->SetDilevel(m_dilevel);

    m_game->Init();



    //�I����ʂ̔w�i���\��
    for (int i = 0; i < 7; i++) {
        m_spriteBackground[i]->Deactivate();
    }
    //BGM���~�߂�
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

    //�J�E���g�_�E���t�H���g���\��
    if (m_countStartCountdown == COUNTDOWN_DEACTIVATE) {
        m_spriteCountdown[0]->Deactivate();
        m_flagStartCountdown = false;
    }
    //�J�E���g�O
    else if (m_countStartCountdown == COUNTDOWN_0) {
        m_spriteCountdown[1]->Deactivate();
        m_spriteCountdown[0]->Activate();
        m_StopOperation = false;
        m_seGameStart->Play(false);
    }
    //�J�E���g�P
    else if (m_countStartCountdown == COUNTDOWN_1) {
        m_spriteCountdown[2]->Deactivate();
        m_spriteCountdown[1]->Activate();
        m_seCount->Play(false);
    }
    //�J�E���g�Q
    else if (m_countStartCountdown == COUNTDOWN_2) {
        m_spriteCountdown[3]->Deactivate();
        m_spriteCountdown[2]->Activate();
        m_seCount->Play(false);
    }
    //�J�E���g�R
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

    //�w�i��������Ԃɖ߂�
    m_spriteBackground[0]->SetPosition({ 0.0f,0.0f });
    m_spriteBackground[1]->SetPosition({ 1280.0f,0.0f });
    m_spriteBackground[2]->SetPosition({ 0.0f,-720.0f });
    m_spriteBackground[3]->SetPosition({ 1280.0f,-720.0f });
    m_spriteBackground[4]->SetPosition({ 2560.0f,-720.0f });
    m_spriteBackground[5]->SetPosition({ 1280.0f,-1440.0f });
    m_spriteBackground[6]->SetPosition({ 2560.0f,-1440.0f });

    //�I����ʂ̔w�i��\��
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
    //m_flagTitleScene = true;
    //m_flagPlayerSelectScene = false;
    //m_flagGameScene = false;
    //m_maxPlayer = 0;
}