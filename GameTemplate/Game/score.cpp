#include "stdafx.h"
#include "score.h"

#include <string>

#include "game.h"
#include "player.h"
#include "stage.h"
#include "pause.h"

namespace //constant
{
    ////////////////////////////////////////////////////////////
    // �ʒu���
    ////////////////////////////////////////////////////////////

    const Vector2 SCORE_TIME_FONT_POSITION[con::PlayerNumberMax] = {	//�X�R�A�^�C���̕\���ʒu
        { -570.0f, -300.0f },										        //�v���C���[�P
        { -250.0f, -300.0f },												//�v���C���[�Q
        { 70.0f, -300.0f },												//�v���C���[�R
        { 400.0f, -300.0f }													//�v���C���[�S
    };

    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    const wchar_t* INIT_FONT_SCORE_TIME = L"0:00:00";   //�����\���̃X�R�A�^�C��
    const int ONE_MINUTES_FLAME = 3600;                 //�t���[���J�E���g�ł̂P��
    const int ONE_DIGIT_CHECK = 10;                     //�P�����̊m�F�Ɏg�p����萔
    const int SIXTY_FLAME = 60;                         //60�t���[��
    const int ADJUSTMENT_CONMA_SECONDS = 100;           //�R���}�b�̕\���̒����Ɏg�p
}



Score::Score()
{

}

Score::~Score()
{
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        DeleteGO(m_fontScoreTime[playerNum]);
    }

    DeleteGO(m_spriteUI);
}

bool Score::Start()
{
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontScoreTime[playerNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontScoreTime[playerNum]->Init(INIT_FONT_SCORE_TIME, SCORE_TIME_FONT_POSITION[playerNum]);
    }

    m_spriteUI = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteUI->Init(filePath::dds::SCORE_TIME_UI);

    m_player = FindGO<Player>(igo::CLASS_NAME_PLAYER);
    m_game = FindGO<Game>(igo::CLASS_NAME_GAME);
    m_pause = FindGO<Pause>(igo::CLASS_NAME_PAUSE);

    return true;
}

////////////////////////////////////////////////////////////
// ���t���[���̏���
////////////////////////////////////////////////////////////

void Score::Update()
{
    if (m_pause->GetPauseFlag() == false) {
        if (m_game->GetStopOperation() == true) {
            return;
        }

        for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
            if (m_flagProcessing[playerNum] == true) {
                AddTime(playerNum);
                FinishTime(playerNum);
                DrawTime(playerNum);
            }
        }
    }
}

void Score::AddTime(const int pNum)
{
    ++m_scoreTime[pNum];
}

void Score::FinishTime(const int pNum)
{
    if (m_player->GetFlagGoal(pNum) == false) {
        return;
    }

    m_flagProcessing[pNum] = false;
}

void Score::DrawTime(const int pNum)
{
    std::wstring scoreTimeString;

    //���̌v�Z
    if (m_scoreTime[pNum] == ONE_MINUTES_FLAME) {
        m_scoreTime[pNum] = con::TIME_RESET_ZERO;
        m_scoreTimeSeconds[pNum] = con::TIME_RESET_ZERO;
        ++m_scoreTimeMinutes[pNum];
    }

    //�b�̌v�Z
    m_scoreTimeSeconds[pNum] = m_scoreTime[pNum] / SIXTY_FLAME;

    //�R���}�b�̌v�Z
    double conmaSeconds = m_scoreTime[pNum] % SIXTY_FLAME;

    conmaSeconds = conmaSeconds / SIXTY_FLAME * ADJUSTMENT_CONMA_SECONDS;

    m_scoreTimeCommaSeconds[pNum] = int(conmaSeconds);

    //�\������^�C���̕�������쐬
    if (m_scoreTimeSeconds[pNum] < ONE_DIGIT_CHECK) {
        if (m_scoreTimeCommaSeconds[pNum] < ONE_DIGIT_CHECK) {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[pNum]) + L":0" +
                std::to_wstring(m_scoreTimeSeconds[pNum]) + L":0" +
                std::to_wstring(m_scoreTimeCommaSeconds[pNum]);
        }
        else {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[pNum]) + L":0" +
                std::to_wstring(m_scoreTimeSeconds[pNum]) + L":" +
                std::to_wstring(m_scoreTimeCommaSeconds[pNum]);
        }
    }
    else {
        if (m_scoreTimeCommaSeconds[pNum] < ONE_DIGIT_CHECK) {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[pNum]) + L":" +
                std::to_wstring(m_scoreTimeSeconds[pNum]) + L":0" +
                std::to_wstring(m_scoreTimeCommaSeconds[pNum]);
        }
        else {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[pNum]) + L":" +
                std::to_wstring(m_scoreTimeSeconds[pNum]) + L":" +
                std::to_wstring(m_scoreTimeCommaSeconds[pNum]);
        }
    }

    //�\�����镶������X�V
    m_fontScoreTime[pNum]->SetText(scoreTimeString.c_str());
}

void Score::NextRound()
{
    for (int i = 0; i < con::PlayerNumberMax; i++) {
        m_scoreTime[i] = 0;
        m_flagProcessing[i] = true;
        m_scoreTimeMinutes[i] = 0;
        m_scoreTimeSeconds[i] = 0;
        m_scoreTimeCommaSeconds[i] = 0;

        DrawTime(i);
    }
    
}