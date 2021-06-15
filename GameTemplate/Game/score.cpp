#include "stdafx.h"
#include "score.h"

#include <string>

#include "constant.h"

namespace //constant
{
    ////////////////////////////////////////////////////////////
    // �ʒu���
    ////////////////////////////////////////////////////////////

    const Vector2 SCORE_TIME_FONT_POSITION[Player::PlayerNumberMax] = {	//�X�R�A�^�C���̕\���ʒu
        { -390.0f, -290.0f },										        //�v���C���[�P
        { -130.0f, -290.0f },												//�v���C���[�Q
        { 130.0f, -290.0f },												//�v���C���[�R
        { 390.0f, -290.0f }													//�v���C���[�S
    };

    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    const int ONE_MINUTES_FLAME = 3600;             //�t���[���J�E���g�ł̂P��
    const int ONE_DIGIT_CHECK = 10;                 //�P�����̊m�F�Ɏg�p����萔
    const int SIXTY_FLAME = 60;                     //60�t���[��
    const int ADJUSTMENT_CONMA_SECONDS = 100;       //�R���}�b�̕\���̒����Ɏg�p
}



Score::Score()
{

}

Score::~Score()
{

}

bool Score::Start()
{
    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        m_fontScoreTime[i] = NewGO<FontRender>(0);
        m_fontScoreTime[i]->Init(L"", SCORE_TIME_FONT_POSITION[i]);
    }

    m_player = FindGO<Player>("player");

    return true;
}

void Score::Update()
{
    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        if (m_flagProcessing[i] == true) {
            AddTime(i);
            FinishTime(i);
            DrawTime(i);
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
    std::wstring str;

    //���̌v�Z
    if (m_scoreTime[pNum] == ONE_MINUTES_FLAME) {
        m_scoreTime[pNum] = con::ZERO;
        m_scoreTimeSeconds[pNum] = con::ZERO;
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
            str = std::to_wstring(m_scoreTimeMinutes[pNum]) + L":0" +
                std::to_wstring(m_scoreTimeSeconds[pNum]) + L":0" +
                std::to_wstring(m_scoreTimeCommaSeconds[pNum]);
        }
        else {
            str = std::to_wstring(m_scoreTimeMinutes[pNum]) + L":0" +
                std::to_wstring(m_scoreTimeSeconds[pNum]) + L":" +
                std::to_wstring(m_scoreTimeCommaSeconds[pNum]);
        }
    }
    else {
        if (m_scoreTimeCommaSeconds[pNum] < ONE_DIGIT_CHECK) {
            str = std::to_wstring(m_scoreTimeMinutes[pNum]) + L":" +
                std::to_wstring(m_scoreTimeSeconds[pNum]) + L":0" +
                std::to_wstring(m_scoreTimeCommaSeconds[pNum]);
        }
        else {
            str = std::to_wstring(m_scoreTimeMinutes[pNum]) + L":" +
                std::to_wstring(m_scoreTimeSeconds[pNum]) + L":" +
                std::to_wstring(m_scoreTimeCommaSeconds[pNum]);
        }
    }

    //�\�����镶������X�V
    m_fontScoreTime[pNum]->SetText(str.c_str());
}