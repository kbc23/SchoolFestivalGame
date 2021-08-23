#pragma once
#include "sprite_render.h"
#include "font_render.h"
#include "constant.h"

class MainProcessing;
class Player;
class Pause;
class GameStartCountdown;
class Stage;
class Rank;

class Score : public IGameObject
{
public:
    Score();
    ~Score();
    bool Start() override final;
    void Init();
    void Finish();

    ////////////////////////////////////////////////////////////
    // 毎フレームの処理
    ////////////////////////////////////////////////////////////

    void Update() override final;

    /**
     * @brief スコア時間を増加する関数
     * @param pNum プレイヤー番号
    */
    void AddTime(const int playerNum);

    /**
     * @brief スコア時間の増加を停止する関数
     * @param pNum プレイヤー番号
    */
    void FinishTime(const int playerNum);

    /**
     * @brief スコア時間の描画処理
     * @param pNum プレイヤー番号
    */
    void DrawTime(const int playerNum);

    //////////////////////////////
    // ラウンド変更の処理
    //////////////////////////////

    /**
     * @brief ラウンド変更の処理
    */
    void NextRound();

private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    SpriteRender* m_spriteUI = nullptr;
    FontRender* m_fontScoreTime[con::PlayerNumberMax] = { nullptr }; //各プレイヤーのスコアタイムのフォント

    //////////////////////////////
    // FindGO
    //////////////////////////////

    Player* m_findPlayer = nullptr;
    MainProcessing* m_findMainProcessing = nullptr;
    Pause* m_findPause = nullptr;
    GameStartCountdown* m_findGameStartCountdown = nullptr;
    Stage* m_findStage = nullptr;
    Rank* m_findRank = nullptr;

    ////////////////////////////////////////////////////////////
    // スコアタイム関連
    ////////////////////////////////////////////////////////////

    int m_scoreTime[con::PlayerNumberMax] = { 0,0,0,0 };                     //スコアタイムのカウント

    bool m_flagScoreTimeProcessing[con::PlayerNumberMax] = { true,true,true,true };   //スコアタイムのカウントの処理をおこなっているか

    int m_scoreTimeMinutes[con::PlayerNumberMax] = { 0,0,0,0 };              //分
    int m_scoreTimeSeconds[con::PlayerNumberMax] = { 0,0,0,0 };              //秒
    int m_scoreTimeCommaSeconds[con::PlayerNumberMax] = { 0,0,0,0 };         //コンマ秒

};

