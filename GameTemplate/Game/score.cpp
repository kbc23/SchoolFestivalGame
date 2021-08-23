#include "stdafx.h"
#include "score.h"

#include <string>

#include "main_processing.h"
#include "player.h"
#include "pause.h"
#include "game_start_countdown.h"
#include "stage.h"
#include "rank.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // 位置情報
    ////////////////////////////////////////////////////////////

    const Vector2 SCORE_TIME_FONT_POSITION[con::PlayerNumberMax] = {	//スコアタイムの表示位置
        { -570.0f, -300.0f },										        //プレイヤー１
        { -250.0f, -300.0f },												//プレイヤー２
        { 70.0f, -300.0f },												    //プレイヤー３
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
    ////////////////////////////////////////////////////////////
    // NewGO
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // フォントのNewGO
    //////////////////////////////

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontScoreTime[playerNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontScoreTime[playerNum]->Init(INIT_FONT_SCORE_TIME, SCORE_TIME_FONT_POSITION[playerNum]); //初期化
        m_fontScoreTime[playerNum]->Deactivate(); //非表示
    }

    //////////////////////////////
    // スプライトのNewGO
    //////////////////////////////

    m_spriteUI = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteUI->Init(filePath::dds::SCORE_TIME_UI); //初期化
    m_spriteUI->Deactivate(); //非表示
}

Score::~Score()
{
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        DeleteGO(m_fontScoreTime[playerNum]);
    }

    DeleteGO(m_spriteUI);
}

bool Score::Start()
{
    ////////////////////////////////////////////////////////////
    // FindGO
    ////////////////////////////////////////////////////////////

    m_findPlayer = FindGO<Player>(igo::CLASS_NAME_PLAYER);
    m_findMainProcessing = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);
    m_findPause = FindGO<Pause>(igo::CLASS_NAME_PAUSE);
    m_findGameStartCountdown = FindGO<GameStartCountdown>(igo::CLASS_NAME_GAME_START_COUNTDOWN);
    m_findStage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
    m_findRank = FindGO<Rank>(igo::CLASS_NAME_RANK);

    return true;
}

void Score::Init()
{
    m_flagProcess = true;

    ////////////////////////////////////////////////////////////
    // フォントの初期化
    ////////////////////////////////////////////////////////////

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontScoreTime[playerNum]->Activate(); //表示
    }

    ////////////////////////////////////////////////////////////
    // スプライトの初期化
    ////////////////////////////////////////////////////////////

    m_spriteUI->Activate(); //表示

    ////////////////////////////////////////////////////////////
    // メンバ変数の初期化
    ////////////////////////////////////////////////////////////

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_scoreTime[playerNum] = 0; //スコアタイムのカウント
        m_flagScoreTimeProcessing[playerNum] = true; //スコアタイムのカウントの処理をおこなっているか
        m_scoreTimeMinutes[playerNum] = 0; //分
        m_scoreTimeSeconds[playerNum] = 0; //秒
        m_scoreTimeCommaSeconds[playerNum] = 0; //コンマ秒

        //表示時間を０秒にセット
        m_fontScoreTime[playerNum]->SetText(INIT_FONT_SCORE_TIME);
    }
}

void Score::Finish()
{
    m_flagProcess = false;

    //////////
    // フォントの非表示
    //////////

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontScoreTime[playerNum]->Deactivate(); //非表示
    }

    //////////
    // スプライトの非表示
    //////////

    m_spriteUI->Deactivate(); //非表示
}

////////////////////////////////////////////////////////////
// 毎フレームの処理
////////////////////////////////////////////////////////////

void Score::Update()
{
    if (false == m_flagProcess) {
        return;
    }

    //最初のカウントダウンが終わってないか
    if (m_findGameStartCountdown->GetFlagStopGameProcess() == true) {
        return;
    }

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        if (m_flagScoreTimeProcessing[playerNum] == true) {
            //タイムのカウント
            AddTime(playerNum);
            //タイムのカウントを停止するか
            FinishTime(playerNum);
            //現在のタイムの描画
            DrawTime(playerNum);
        }
    }

}

void Score::AddTime(const int playerNum)
{
    ++m_scoreTime[playerNum]; //カウント
}

void Score::FinishTime(const int playerNum)
{
    //ゴールしていない場合
    if (false == m_findRank->GetDoingGoal(playerNum)) {
        return;
    }

    //カウント処理を停止する
    m_flagScoreTimeProcessing[playerNum] = false;
}

void Score::DrawTime(const int playerNum)
{
    std::wstring scoreTimeString; //描画する文字列

    //分の計算
    if (m_scoreTime[playerNum] == ONE_MINUTES_FLAME) {
        m_scoreTime[playerNum] = con::TIME_RESET_ZERO;
        m_scoreTimeSeconds[playerNum] = con::TIME_RESET_ZERO;
        ++m_scoreTimeMinutes[playerNum];
    }

    //秒の計算
    m_scoreTimeSeconds[playerNum] = m_scoreTime[playerNum] / SIXTY_FLAME;

    //コンマ秒の計算
    double conmaSeconds = m_scoreTime[playerNum] % SIXTY_FLAME;
    conmaSeconds = conmaSeconds / SIXTY_FLAME * ADJUSTMENT_CONMA_SECONDS;
    m_scoreTimeCommaSeconds[playerNum] = static_cast<int>(conmaSeconds);

    //表示するタイムの文字列を作成
    //秒が１桁
    if (m_scoreTimeSeconds[playerNum] < ONE_DIGIT_CHECK) {
        //コンマ秒が１桁
        if (m_scoreTimeCommaSeconds[playerNum] < ONE_DIGIT_CHECK) {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[playerNum]) + L":0" +
                std::to_wstring(m_scoreTimeSeconds[playerNum]) + L":0" +
                std::to_wstring(m_scoreTimeCommaSeconds[playerNum]);
        }
        //コンマ秒が２桁
        else {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[playerNum]) + L":0" +
                std::to_wstring(m_scoreTimeSeconds[playerNum]) + L":" +
                std::to_wstring(m_scoreTimeCommaSeconds[playerNum]);
        }
    }
    //秒が２桁
    else {
        //コンマ秒が１桁
        if (m_scoreTimeCommaSeconds[playerNum] < ONE_DIGIT_CHECK) {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[playerNum]) + L":" +
                std::to_wstring(m_scoreTimeSeconds[playerNum]) + L":0" +
                std::to_wstring(m_scoreTimeCommaSeconds[playerNum]);
        }
        //コンマ秒が２桁
        else {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[playerNum]) + L":" +
                std::to_wstring(m_scoreTimeSeconds[playerNum]) + L":" +
                std::to_wstring(m_scoreTimeCommaSeconds[playerNum]);
        }
    }

    //表示する文字列を更新
    m_fontScoreTime[playerNum]->SetText(scoreTimeString.c_str());
}

void Score::NextRound()
{
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_scoreTime[playerNum] = con::TIME_RESET_ZERO;
        m_flagScoreTimeProcessing[playerNum] = true;
        m_scoreTimeMinutes[playerNum] = con::TIME_RESET_ZERO;
        m_scoreTimeSeconds[playerNum] = con::TIME_RESET_ZERO;
        m_scoreTimeCommaSeconds[playerNum] = con::TIME_RESET_ZERO;
        
        m_fontScoreTime[playerNum]->SetText(INIT_FONT_SCORE_TIME); //テキスト
    }
}