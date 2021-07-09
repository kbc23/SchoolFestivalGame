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
    // 位置情報
    ////////////////////////////////////////////////////////////

    const Vector2 SCORE_TIME_FONT_POSITION[con::PlayerNumberMax] = {	//スコアタイムの表示位置
        { -570.0f, -300.0f },										        //プレイヤー１
        { -250.0f, -300.0f },												//プレイヤー２
        { 70.0f, -300.0f },												//プレイヤー３
        { 400.0f, -300.0f }													//プレイヤー４
    };

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    const wchar_t* INIT_FONT_SCORE_TIME = L"0:00:00";   //初期表示のスコアタイム
    const int ONE_MINUTES_FLAME = 3600;                 //フレームカウントでの１分
    const int ONE_DIGIT_CHECK = 10;                     //１桁かの確認に使用する定数
    const int SIXTY_FLAME = 60;                         //60フレーム
    const int ADJUSTMENT_CONMA_SECONDS = 100;           //コンマ秒の表示の調整に使用
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
// 毎フレームの処理
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

    //分の計算
    if (m_scoreTime[pNum] == ONE_MINUTES_FLAME) {
        m_scoreTime[pNum] = con::TIME_RESET_ZERO;
        m_scoreTimeSeconds[pNum] = con::TIME_RESET_ZERO;
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

    //表示する文字列を更新
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