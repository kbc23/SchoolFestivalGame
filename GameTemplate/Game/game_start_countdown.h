#pragma once
#include "sprite_render.h"
#include "sound_SE.h"

class Player;

class GameStartCountdown : public IGameObject
{
public:
    GameStartCountdown();
    ~GameStartCountdown();
    bool Start() override final;
    void Init();
    void Finish();
    void Update() override final;

private:
    /**
     * @brief カウントダウンの処理
    */
    void Countdown();


public:
    /**
     * @brief 次のラウンドに移動する処理
    */
    void NextRound();

public: //Get関数
    /**
     * @brief 他のゲーム関連の処理を止めるか
     * @return 他のゲーム関連の処理を止めるか
    */
    const bool& GetFlagStopGameProcess() const
    {
        return m_flagStopGameProcess;
    }



private: //enum
    enum Countdown
    {
        count_gameStart,
        count_1,
        count_2,
        count_3,
        CountdownMax
    };

private: //constant
    static const int m_INIT_COUNT_START_COUNTDOWN = 240;        //m_countStartCountdownの初期値


private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    SpriteRender* m_spriteCountdown[CountdownMax] = { nullptr };
    SoundSE* m_seCount = nullptr;
    SoundSE* m_seGameStart = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////

    Player* m_findPlayer = nullptr;

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    bool m_flagCountdown = true; //カウントダウンの処理をするか
    bool m_flagStopGameProcess = true; //ゲームの処理を停止するか



    float m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;       //カウントダウンで使用されるタイマー

    bool m_flagCount3SE = false;
    bool m_flagCount2SE = false;
    bool m_flagCount1SE = false;
    bool m_flagStartCountdownSE = false;

};