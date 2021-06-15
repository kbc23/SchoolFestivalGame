#include "stdafx.h"
#include "score.h"

#include <string>

namespace {
    const Vector2 SCORE_TIME_FONT_POSITION[Player::PlayerNumberMax] = {	//�X�R�A�^�C���̕\���ʒu
        { -390.0f, -290.0f },										        //�v���C���[�P
        { -130.0f, -290.0f },												    //�v���C���[�Q
        { 130.0f, -290.0f },												    //�v���C���[�R
        { 390.0f, -290.0f }													//�v���C���[�S
    };
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
    if (m_scoreTime[pNum] == 3600) {
        m_scoreTime[pNum] = 0;
        m_scoreTimeSeconds[pNum] = 0;
        ++m_scoreTimeMinutes[pNum];
    }

    //�b�̌v�Z
    m_scoreTimeSeconds[pNum] = m_scoreTime[pNum] / 60;

    //�R���}�b�̌v�Z
    double conmaSeconds = m_scoreTime[pNum] % 60;

    conmaSeconds = conmaSeconds / 60 * 100;

    m_scoreTimeCommaSeconds[pNum] = conmaSeconds;

    //�\������^�C���̕�������쐬
    if (m_scoreTimeSeconds[pNum] < 10) {
        if (m_scoreTimeCommaSeconds[pNum] < 10) {
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
        if (m_scoreTimeCommaSeconds[pNum] < 10) {
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