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
    // �^�C�}�[�֘A
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
        MessageBoxA(nullptr, "�Q�[���̑J�ڂɂăG���[���������܂����B", "�G���[", MB_OK);
        return;
    }
}

////////////////////////////////////////////////////////////
// �^�C�g���V�[���̏���
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
// ���[�h�Z���N�g�V�[���̏���
////////////////////////////////////////////////////////////

void Game::ModeSelectScene()
{
    if (m_modeSelect->GetFlagFinish() == false) {
        //�L�����Z��
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
// �v���C���[�Z���N�g�V�[���̏���
////////////////////////////////////////////////////////////

void Game::PlayerSelectScene()
{
    if (m_playerSelect->GetFlagFinish() == false) {
        //�L�����Z��
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
// CPU�̓�Փx�I���V�[���̏���
////////////////////////////////////////////////////////////

void Game::CPUStrengthSelectScene()
{
    if (m_CPUStrengthSelect->GetFlagFinish() == false) {
        //�L�����Z��
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
// �Q�[���V�[���̂��߂̃��[�h
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
        MessageBoxA(nullptr, "�Q�[���̃��[�h�ɂăG���[���������܂����B", "�G���[", MB_OK);
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

    //Player�N���X�ɑI�����ꂽ�v���C���[�l����n���B
    m_player->SetMaxPlayer(m_maxPlayer);

    //�I����ʂ̔w�i���폜
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
// �Q�[���V�[���̏���
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

    //�J�E���g�_�E���t�H���g���\��
    if (m_countStartCountdown <= COUNTDOWN_DEACTIVATE) {
        m_fontStartCountdown->Deactivate();
        m_flagStartCountdown = false;
    }
    //�J�E���g�O
    else if (m_countStartCountdown <= COUNTDOWN_0) {
        m_fontStartCountdown->SetText(L"Start!");
        m_StopOperation = false;
    }
    //�J�E���g�P
    else if (m_countStartCountdown <= COUNTDOWN_1) {
        m_fontStartCountdown->SetText(1);
    }
    //�J�E���g�Q
    else if (m_countStartCountdown <= COUNTDOWN_2) {
        m_fontStartCountdown->SetText(2);
    }
    //�J�E���g�R
    else if (m_countStartCountdown <= COUNTDOWN_3) {
        m_fontStartCountdown->SetText(3);
    }
}