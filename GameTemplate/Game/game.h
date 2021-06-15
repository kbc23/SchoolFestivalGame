#pragma once
#include "font_render.h"

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

    
    void StartCountdown();


public: //Getä÷êî
    bool GetStopOperation()
    {
        return m_StopOperation;
    }


private: //constant
    static const int m_INIT_COUNT_START_COUNTDOWN = 180;

private: //data menber
    Player* m_player = nullptr;
    GameCamera* m_gameCamera = nullptr;
    Stage* m_stage = nullptr;
    Score* m_score = nullptr;
    FontRender* m_fontStartCountdown = nullptr;

    int m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;
    bool m_flagStartCountdown = true;
    bool m_StopOperation = true;
};

