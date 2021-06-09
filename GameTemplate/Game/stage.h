#pragma once
#include "model_render.h"

#include "player.h"



class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start();
    void Update();


public:
    void MoveBlock(const int pNum, const int moveNum);
    void GoalBlock();



private: //定数
 
    static const int MAX_BLOCK = 20;       //１レーンのブロックの最大数
    static const int START_BLOCK = 0;       //スタート位置のブロックの番号

private: //data menber
    //[プレイヤー番号][ステージのマスの数]
    ModelRender* m_modelRender[Player::PlayerNumberMax][MAX_BLOCK] = { nullptr };
    //プレイヤーの位置
    int m_playerBlockPosition[Player::PlayerNumberMax] = { START_BLOCK, START_BLOCK, START_BLOCK, START_BLOCK };

};

