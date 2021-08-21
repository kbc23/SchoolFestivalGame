#pragma once
#include "constant.h"
#include "game_data.h"

class Player;
class Stage;
class Score;
class CPUPlayerController;
class SuddenDeathMode;
class Pause;
class GameStartCountdown;
class Goal;
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



public:



private:





public: //Get関数
    /**
     * @brief GameDataクラスのオブジェクトを取得する
     * @return GameDataクラスのオブジェクト
    */
    GameData* GetGameDataObject()
    {
        return m_gameData;
    }



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
    GameData* m_gameData = nullptr;
    GameStartCountdown* m_gameStartCountdown = nullptr;
    CPUPlayerController* m_CPUPlayerController = nullptr;
    Goal* m_goal = nullptr;
    Rank* m_rank = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////


};