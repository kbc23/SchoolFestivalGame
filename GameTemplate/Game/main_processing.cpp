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
    // ������
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
    // �{�����[��
    ////////////////////////////////////////////////////////////

    const float TITLE_BGM_VOLUME = 0.5f; //�^�C�g����ʂ�BGM�̃{�����[��

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    const float MOVE_BACKGROUND = 1010.0f;      //�w�i�̈ړ���
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

    //�Q�[���V�[��
    DeleteGO(m_game);

    //���U���g�V�[��
    DeleteGO(m_result);

    //�I����ʂ̔w�i���폜
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        DeleteGO(m_spriteBackground[backgroundNum]);
    }

    //�^�C�g����ʂ�BGM
    DeleteGO(m_bgmTitle);

    //�J����
    DeleteGO(m_gameCamera);

    //�L�����Z���{�^����SE
    DeleteGO(m_seCancel);
}

bool MainProcessing::Start()
{
    ////////////////////////////////////////////////////////////
    // �w�i
    ////////////////////////////////////////////////////////////

    //�摜�̔z��ԍ��ƕ\���ʒu�i0�̈ʒu����ʁj
    //0 1
    //2 3 4
    //  5 6

    //�w�i�̏�����
    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_MAX_BACKGROUND; spriteNum++) {
        m_spriteBackground[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND); //NewGO
        m_spriteBackground[spriteNum]->Init(SPRITE_BACKGROUND[spriteNum]); //������
        m_spriteBackground[spriteNum]->SetPosition(BACKGROUND_START_POSITION[spriteNum]); //�ʒu
    }

    //�t�F�[�h�̏�����������
    m_fade = NewGO<Fade>(igo::PRIORITY_CLASS);

    ////////////////////////////////////////////////////////////
    // BGM
    ////////////////////////////////////////////////////////////

    //�I����ʂ�BGM���������A�Đ�
    m_bgmTitle = NewGO<SoundBGM>(igo::PRIORITY_CLASS); //NewGO
    m_bgmTitle->Init(filePath::bgm::TITLE); //������
    m_bgmTitle->SetVolume(TITLE_BGM_VOLUME); //���ʒ���
    m_bgmTitle->Play(true); //�Đ�

    //BGM�S�̂̉��ʂ�ݒ�
    m_bgmTitle->SetBGMVolume(0.0f);

    ////////////////////////////////////////////////////////////
    // SE
    ////////////////////////////////////////////////////////////

    //�L�����Z����SE��������    
    m_seCancel = NewGO<SoundSE>(igo::PRIORITY_CLASS); //NewGO
    m_seCancel->Init(filePath::se::CANCEL); //������

    //SE�S�̂̉��ʂ�ݒ�
    m_seCancel->SetSEVolume(0.5f);

    ////////////////////////////////////////////////////////////
    // �N���X
    ////////////////////////////////////////////////////////////

    //�J������������
    m_gameCamera = NewGO<GameCamera>(igo::PRIORITY_CLASS);

    //�V�[����NewGO
    m_title = NewGO<Title>(igo::PRIORITY_CLASS);
    m_modeSelect = NewGO<ModeSelect>(igo::PRIORITY_CLASS);
    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_CLASS);
    m_CPUStrengthSelect = NewGO<CPUStrengthSelect>(igo::PRIORITY_CLASS);
    m_game = NewGO<Game>(igo::PRIORITY_CLASS, igo::CLASS_NAME_GAME);
    m_result = NewGO<Result>(igo::PRIORITY_CLASS, igo::CLASS_NAME_RESULT);

    //�V�[���̏�Ԃ̐؂�ւ�
    m_gameStatus = GameStatus::startLoading;

    return true;
}

////////////////////////////////////////////////////////////
// ���t���[�����鏈��
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

    //�Q�[���V�[�����n�܂��Ă��邩
    if (m_flagGameStart == false) {
        //�w�i�̕`��
        DrawBackground();
    }
}

void MainProcessing::DrawBackground()
{
    //�w�i�̃A�j���[�V����
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->SetPosition(
            { (m_spriteBackground[backgroundNum]->GetPosition().x - con::GAME_SCREEN_W / MOVE_BACKGROUND),
              m_spriteBackground[backgroundNum]->GetPosition().y + con::GAME_SCREEN_H / MOVE_BACKGROUND
            }
        );
    }

    //�w�i�������ʒu�Ƀ��Z�b�g
    if (m_spriteBackground[m_MAX_BACKGROUND - 1]->GetPosition().x <= 0.0f) {
        for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
            m_spriteBackground[backgroundNum]->SetPosition(BACKGROUND_START_POSITION[backgroundNum]);
        }
    }
}

////////////////////////////////////////////////////////////
// �V�[�����Ƃ̏���
////////////////////////////////////////////////////////////


//////////////////////////////
// �Q�[���J�n���̃��[�h�̏���
//////////////////////////////

void MainProcessing::StartLoading()
{
    m_title->Init();

    m_gameStatus = GameStatus::title;
}

//////////////////////////////
// �^�C�g���V�[���̏���
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
// ���[�h�Z���N�g�V�[���̏���
//////////////////////////////

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

//////////////////////////////
// �v���C���[�Z���N�g�V�[���̏���
//////////////////////////////

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

//////////////////////////////
// CPU�̓�Փx�I���V�[���̏���
//////////////////////////////

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

//////////////////////////////
// �Q�[���V�[���̂��߂̃��[�h
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
    m_game->GetGameDataObject()->SetMaxPlayer(m_maxPlayer);
    m_game->GetGameDataObject()->SetCPULevel(m_CPULevel);

    m_game->Init();

    //�I����ʂ̔w�i���\��
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->Deactivate();
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

//////////////////////////////
// �Q�[���V�[���̏���
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

    //�w�i��������Ԃɖ߂��āA�\��
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->SetPosition(BACKGROUND_START_POSITION[backgroundNum]);
        m_spriteBackground[backgroundNum]->Activate();
    }

    m_flagGameStart = false;

    m_startPreparingForLoading = false;
    m_startEndOfLoading = false;

    m_result->Init();

    m_bgmTitle->Play(true); //�Đ�

    m_gameStatus = GameStatus::result;
}

//////////////////////////////
// ���U���g�V�[���̏���
//////////////////////////////

void MainProcessing::ResultScene()
{
    if (m_result->GetFlagFinish() == false) {
        return;
    }

    m_result->Finish();

    switch (m_result->GetCursorPosition()) {
    //������x�v���C
    case choice::ResultCommand::playOneMoreTime:
        //�Q�[���̃��[�h�����������Ȃ�
        m_gameStatus = GameStatus::loadingGame;
        m_loadStatus = LoadingStatus::preparingForLoading;
        break;
    //�v���C���[���I��
    case choice::ResultCommand::chooseTheNumberOfPlayers:
        m_playerSelect->Init();
        m_gameStatus = GameStatus::playerSelect;
        break;
    //���[�h�I��
    case choice::ResultCommand::chooseARule:
        m_modeSelect->Init();
        m_gameStatus = GameStatus::modeSelect;
        break;
    //�Q�[���I��
    case choice::ResultCommand::exitGame:
        exit(EXIT_SUCCESS);
        break;
    }
}

////////////////////////////////////////////////////////////
// �|�[�Y��ʂƊ֌W
////////////////////////////////////////////////////////////

void MainProcessing::PauseRetry()
{
    //�Q�[���V�[������蒼��
    m_game->Finish();
    m_game->Init();
}

void MainProcessing::PauseTitle()
{
    //�Q�[���V�[������^�C�g���V�[���Ɉڍs
    m_game->Finish();
    m_title->Init();

    m_gameStatus = GameStatus::title;

    //�w�i��������Ԃɖ߂��āA�\��
    for (int backgroundNum = con::FIRST_ELEMENT_ARRAY; backgroundNum < m_MAX_BACKGROUND; backgroundNum++) {
        m_spriteBackground[backgroundNum]->SetPosition(BACKGROUND_START_POSITION[backgroundNum]); //�ʒu
        m_spriteBackground[backgroundNum]->Activate(); //�\��
    }

    //BGM���Đ�
    m_bgmTitle->Play(true);

    m_flagGameStart = false;
}