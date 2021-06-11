#pragma once
#include "model_render.h"

#include "player.h"

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start() override final;
    void Update() override final;


public:
    /**
     * @brief プレイヤーの移動に応じてブロックを動かす。
     * @param pNum プレイヤーの番号
     * @param moveNum ブロックの動く量
    */
    void MoveBlock(const int pNum, const int moveNum);

    /**
     * @brief ゴール時の処理
    */
    void GoalBlock();



private: //constant
    static const int m_MAX_BLOCK = 20;       //１レーンのブロックの最大数
    static const int m_START_BLOCK = 0;       //スタート位置のブロックの番号
    static const int m_INIT_RANK = 1;

private: //data menber

    Player* m_player = nullptr;
    //[プレイヤー番号][ステージのマスの数]
    ModelRender* m_modelRender[Player::PlayerNumberMax][m_MAX_BLOCK] = { nullptr };
 
    //プレイヤーの位置
    int m_playerBlockPosition[Player::PlayerNumberMax] = { m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };

    int m_nowRank = m_INIT_RANK; //プレイヤーの順位データに渡すデータ
};