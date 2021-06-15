#pragma once
#include "font_render.h"

#include "player.h"
#include "game.h"

class Score : public IGameObject
{
public:
    Score();
    ~Score();
    bool Start() override final;
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



private: //data menber
    Player* m_player = nullptr;
    Game* m_game = nullptr;
    FontRender* m_fontScoreTime[Player::PlayerNumberMax] = { nullptr };

    int m_scoreTime[Player::PlayerNumberMax] = { 0,0,0,0 };

    bool m_flagProcessing[Player::PlayerNumberMax] = { true,true,true,true };


    int m_scoreTimeMinutes[Player::PlayerNumberMax] = { 0,0,0,0 };            //分
    int m_scoreTimeSeconds[Player::PlayerNumberMax] = { 0,0,0,0 };            //秒
    int m_scoreTimeCommaSeconds[Player::PlayerNumberMax] = { 0,0,0,0 };       //コンマ秒

};

