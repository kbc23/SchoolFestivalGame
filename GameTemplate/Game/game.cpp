#include "stdafx.h"
#include "game.h"

#include "player_select.h"
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
    m_playerSelect = NewGO<PlayerSelect>(igo::PRIORITY_FIRST);

    return true;
}

////////////////////////////////////////////////////////////
// ���t���[���̏���
////////////////////////////////////////////////////////////

void Game::Update()
{
    //�v���C���[�Z���N�g�V�[���̏���
    if (m_flagPlayerSelectScene == true) {
        PlayerSelectScene();
    }

    //�Q�[���V�[���ł̏���
    if (m_flagGameScene == true) {
        GameScene();
    }

}

//////////////////////////////
// �v���C���[�Z���N�g�V�[���̏���
//////////////////////////////

void Game::PlayerSelectScene()
{
    if (m_playerSelect->GetmFlagFinish() == false) {
        return;
    }

    NewGOGame();

    DeleteGO(m_playerSelect);

    //Player�N���X�ɑI�����ꂽ�v���C���[�l����n���B
    m_player->SetMaxPlayer(m_maxPlayer);
    

    m_flagPlayerSelectScene = false;
    m_flagGameScene = true;
}

void Game::NewGOGame()
{
    m_stage = NewGO<Stage>(igo::PRIORITY_FIRST, igo::CLASS_NAME_STAGE);
    //m_rule1 = NewGO<Rule1>(igo::PRIORITY_FIRST, igo::CLASS_NAME_RULE1);     //�����f�X���[�h
    m_player = NewGO<Player>(igo::PRIORITY_FIRST, igo::CLASS_NAME_PLAYER);
    m_gameCamera = NewGO<GameCamera>(igo::PRIORITY_FIRST);
    m_score = NewGO<Score>(igo::PRIORITY_FIRST);
    m_fontStartCountdown = NewGO<FontRender>(igo::PRIORITY_FIRST);
    m_fontStartCountdown->Init(L"");

}


//////////////////////////////
// �Q�[���V�[���̏���
//////////////////////////////

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