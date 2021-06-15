#pragma once

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



private: //data menber
    Player* m_player = nullptr;
    GameCamera* m_gameCamera = nullptr;
    Stage* m_stage = nullptr;
    Score* m_score = nullptr;
};

