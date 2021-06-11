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
     * @brief ステージの作成処理
    */
    void StageCreate();

    /**
     * @brief プレイヤーの移動に応じてブロックを動かす。
     * @param pNum プレイヤーの番号
     * @param moveNum ブロックの動く量
    */
    void MoveBlock(const int pNum, const int moveNum);

    
    void ReturnBlock(const int pNum);

    void ReturnOperationTimer(const int pNum);


    /**
     * @brief プレイヤーが乗っているブロックを判別
     * @param pNum 
    */
    void CheckBlock(const int pNum);



    /**
     * @brief ゴール時の処理
    */
    void GoalBlock();



public: //Get関数
    const bool GetmActiveOperation(const int pNum)
    {
        return m_activeOperation[pNum];
    }


public: //enum
    //ブロックの種類
    enum StageData
    {
        greenBlock,
        blueBlock,
        yellowBlock,
        StageDataMax
    };



private: //constant
    static const int m_MAX_BLOCK = 30;       //１レーンのブロックの最大数
    static const int m_START_BLOCK = 0;       //スタート位置のブロックの番号
    static const int m_INIT_RANK = 1;

private: //data menber

    Player* m_player = nullptr;

    int m_stageData[m_MAX_BLOCK] = { greenBlock }; //ステージのデータを保存する配列
    //[プレイヤー番号][ステージのマスの数]
    ModelRender* m_modelRender[Player::PlayerNumberMax][m_MAX_BLOCK] = { nullptr };
 
    //プレイヤーの位置
    int m_playerBlockPosition[Player::PlayerNumberMax] = { m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };
    //プレイヤーの前にいた位置
    int m_playerBeforeBlockPosition[Player::PlayerNumberMax] = { m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };
    //プレイヤーが操作可能か
    bool m_activeOperation[Player::PlayerNumberMax] = { true, true, true, true };
    //プレイヤーの操作復帰のタイマー
    int m_timerReturnOperation[Player::PlayerNumberMax] = { 0, 0, 0, 0 };
    //プレイヤーの操作不能状態に対する耐性があるか
    bool m_resistanceImpossibleOperation[Player::PlayerNumberMax] = { false, false, false, false };

    int m_nowRank = m_INIT_RANK; //プレイヤーの順位データに渡すデータ
};