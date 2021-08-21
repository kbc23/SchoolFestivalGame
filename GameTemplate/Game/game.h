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





public: //Get�֐�
    /**
     * @brief GameData�N���X�̃I�u�W�F�N�g���擾����
     * @return GameData�N���X�̃I�u�W�F�N�g
    */
    GameData* GetGameDataObject()
    {
        return m_gameData;
    }



private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
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