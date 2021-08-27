#pragma once
#include "constant.h"

class Player;
class Stage;
class Score;
class CPUPlayerController;
class SuddenDeathMode;
class Pause;
class GameStartCountdown;
class Rank;

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;
    void Init();
    void Finish();
    void Update() override final;


private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    Player* m_player = nullptr;
    Stage* m_stage = nullptr;
    Score* m_score = nullptr;
    SuddenDeathMode* m_suddenDeathMode = nullptr;
    Pause* m_pause = nullptr;
    GameStartCountdown* m_gameStartCountdown = nullptr;
    CPUPlayerController* m_CPUPlayerController = nullptr;
    Rank* m_rank = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////


};