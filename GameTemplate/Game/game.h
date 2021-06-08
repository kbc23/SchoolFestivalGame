#pragma once

class Player;
class GameCamera;
class Stage;

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start();
    void Update();



private:
    Player* m_player = nullptr;
    GameCamera* m_gameCamera = nullptr;
    Stage* m_stage = nullptr;

};

