#pragma once
#include "font_render.h"

class PlayerSelect;
class Player;
class GameCamera;
class Stage;
class Score;

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;
    void Update() override final;

    void PlayerSelectScene();
    void GameScene();


    void NewGOGame();

    
    void StartCountdown();


public: //Get関数
    bool GetStopOperation()
    {
        return m_StopOperation;
    }



public: //Set関数
    void SetFlagPlayerSelectScene(const bool b)
    {
        m_flagPlayerSelectScene = b;
    }

    void SetFlagGameScene(const bool b)
    {
        m_flagGameScene = b;
    }

    void SetActivePlayer(const int i)
    {
        m_maxPlayer = i;
    }



private: //constant
    static const int m_INIT_COUNT_START_COUNTDOWN = 180;        //m_countStartCountdownの初期値

private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    PlayerSelect* m_playerSelect = nullptr;
    Player* m_player = nullptr;
    GameCamera* m_gameCamera = nullptr;
    Stage* m_stage = nullptr;
    Score* m_score = nullptr;
    FontRender* m_fontStartCountdown = nullptr;

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    int m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;
    bool m_flagStartCountdown = true;
    bool m_StopOperation = true;

    ////////////////////////////////////////////////////////////
    // フラグ関連
    ////////////////////////////////////////////////////////////

    bool m_flagPlayerSelectScene = true;
    bool m_flagGameScene = false;

    int m_maxPlayer = 0;
};

