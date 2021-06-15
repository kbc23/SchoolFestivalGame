#include "stdafx.h"
#include "score.h"

#include <string>

#include "constant.h"

namespace //constant
{
    ////////////////////////////////////////////////////////////
    // 位置情報
    ////////////////////////////////////////////////////////////

    const Vector2 SCORE_TIME_FONT_POSITION[Player::PlayerNumberMax] = {	//スコアタイムの表示位置
        { -390.0f, -290.0f },										        //プレイヤー１
        { -130.0f, -290.0f },												//プレイヤー２
        { 130.0f, -290.0f },												//プレイヤー３
        { 390.0f, -290.0f }													//プレイヤー４
    };

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    const wchar_t* INIT_FONT_SCORE_TIME = L"0:00:00";
    const int ONE_MINUTES_FLAME = 3600;             //フレームカウントでの１分
    const int ONE_DIGIT_CHECK = 10;                 //１桁かの確認に使用する定数
    const int SIXTY_FLAME = 60;                     //60フレーム
    const int ADJUSTMENT_CONMA_SECONDS = 100;       //コンマ秒の表示の調整に使用
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
        m_fontScoreTime[i]->Init(INIT_FONT_SCORE_TIME, SCORE_TIME_FONT_POSITION[i]);
    }

    m_player = FindGO<Player>("player");
    m_game = FindGO<Game>("game");

    return true;
}

void Score::Update()
{
    if (m_game->GetStopOperation() == true) {
        return;
    }

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
    if (m_scoreTime[pNum] == ONE_MINUTES_FLAME) {
        m_scoreTime[pNum] = con::ZERO;
        m_scoreTimeSeconds[pNum] = con::ZERO;
        ++m_scoreTimeMinutes[pNum];
    }

    //秒の計算
    m_scoreTimeSeconds[pNum] = m_scoreTime[pNum] / SIXTY_FLAME;

    //コンマ秒の計算
    double conmaSeconds = m_scoreTime[pNum] % SIXTY_FLAME;

    conmaSeconds = conmaSeconds / SIXTY_FLAME * ADJUSTMENT_CONMA_SECONDS;

    m_scoreTimeCommaSeconds[pNum] = int(conmaSeconds);

    //表示するタイムの文字列を作成
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

    //表示する文字列を更新
    m_fontScoreTime[pNum]->SetText(str.c_str());
}