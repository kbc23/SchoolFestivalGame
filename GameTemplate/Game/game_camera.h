#pragma once

class GameCamera : public IGameObject
{
public:
    GameCamera();
    ~GameCamera();
    bool Start() override final;
    void Update() override final;

private: //data menber



};

