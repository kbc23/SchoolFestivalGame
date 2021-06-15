#include "stdafx.h"
#include "score.h"

#include <string>

namespace {
    const Vector2 SCORE_TIME_FONT_POSITION[Player::PlayerNumberMax] = {	//スコアタイムの表示位置
        { -390.0f, -290.0f },										        //プレイヤー１
        { -130.0f, -290.0f },												    //プレイヤー２
        { 130.0f, -290.0f },												    //プレイヤー３
        { 390.0f, -290.0f }													//プレイヤー４
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

    //分の計算
    if (m_scoreTime[pNum] == 3600) {
        m_scoreTime[pNum] = 0;
        m_scoreTimeSeconds[pNum] = 0;
        ++m_scoreTimeMinutes[pNum];
    }

    //秒の計算
    m_scoreTimeSeconds[pNum] = m_scoreTime[pNum] / 60;

    //コンマ秒の計算
    double conmaSeconds = m_scoreTime[pNum] % 60;

    conmaSeconds = conmaSeconds / 60 * 100;

    m_scoreTimeCommaSeconds[pNum] = conmaSeconds;

    //表示するタイムの文字列を作成
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

    //表示する文字列を更新
    m_fontScoreTime[pNum]->SetText(str.c_str());
}