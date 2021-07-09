#pragma once
#include "sprite_render.h"
#include "font_render.h"
#include "constant.h"

class Game;
class Player;
class Stage;
class Rule1;
class Pause;

class Score : public IGameObject
{
public:
    Score();
    ~Score();
    bool Start() override final;

    ////////////////////////////////////////////////////////////
    // 毎フレームの処理
    ////////////////////////////////////////////////////////////

    void Update() override final;

    /**
     * @brief スコア時間を増加する関数
     * @param pNum プレイヤー番号
    */
    void AddTime(const int pNum);

    /**
     * @brief スコア時間の増加を停止する関数
     * @param pNum プレイヤー番号
    */
    void FinishTime(const int pNum);

    /**
     * @brief スコア時間の描画処理
     * @param pNum プレイヤー番号
    */
    void DrawTime(const int pNum);

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
    
    Player* m_player = nullptr;
    Game* m_game = nullptr;
    SpriteRender* m_spriteUI = nullptr;
    FontRender* m_fontScoreTime[con::PlayerNumberMax] = { nullptr }; //各プレイヤーのスコアタイムのフォント
    Stage* m_stage = nullptr;
    Pause* m_pause = nullptr;

    ////////////////////////////////////////////////////////////
    // スコアタイム関連
    ////////////////////////////////////////////////////////////

    int m_scoreTime[con::PlayerNumberMax] = { 0,0,0,0 };                     //スコアタイムのカウント

    bool m_flagProcessing[con::PlayerNumberMax] = { true,true,true,true };   //スコアタイムのカウントの処理をおこなっているか

    int m_scoreTimeMinutes[con::PlayerNumberMax] = { 0,0,0,0 };              //分
    int m_scoreTimeSeconds[con::PlayerNumberMax] = { 0,0,0,0 };              //秒
    int m_scoreTimeCommaSeconds[con::PlayerNumberMax] = { 0,0,0,0 };         //コンマ秒

};

