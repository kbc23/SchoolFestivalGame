#pragma once
#include "model_render.h"
#include "constant.h"

class Player;

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();

    ////////////////////////////////////////////////////////////
    // 初期設定
    ////////////////////////////////////////////////////////////

    bool Start() override final;

    /**
     * @brief ステージの作成処理
    */
    void StageCreate();

    ////////////////////////////////////////////////////////////
    // 毎フレームの処理
    ////////////////////////////////////////////////////////////

    void Update() override final;



    //////////////////////////////
    // ブロックの移動
    //////////////////////////////

    /**
     * @brief プレイヤーの移動に応じてブロックを動かす。
     * @param pNum プレイヤーの番号
     * @param moveNum ブロックの動く量
     * @return ブロックを動かしたかどうか
    */
    bool MoveBlock(const int pNum, const int moveNum);

    //////////////////////////////
    // タイマー
    //////////////////////////////

    /**
     * @brief 操作不能状態のタイマーのカウント
     * @param pNum プレイヤーの番号
    */
    void ReturnOperationTimer(const int pNum);

    //////////////////////////////
    // ブロックごとの処理
    //////////////////////////////

    /**
     * @brief プレイヤーが乗っているブロックを判別
     * @param pNum プレイヤー番号
    */
    void CheckBlock(const int pNum);

    //////////
    // 青色のブロック
    //////////

    /**
     * @brief 青色のブロックの上に行ったときの処理    
     * @param pNum プレイヤー番号
    */
    void BlueBlock(const int pNum);

    /**
     * @brief 青色のブロックの上に行ったときのアニメーション
     * @param pNum プレイヤー番号
    */
    void BlueBlockAnimation(const int pNum);

    /**
     * @brief 前にいた位置のブロックに戻る処理
     * @param pNum プレイヤーの番号
    */
    void ReturnBlock(const int pNum);

    //////////////////////////////
    // ゴール時の処理
    //////////////////////////////

    /**
     * @brief ゴール時の処理
    */
    void GoalBlock();



public: //Get関数
    /**
     * @brief m_activeOperation[pNum]のGet関数
     * @param pNum プレイヤー番号
     * @return プレイヤー番号[pNum]が、操作可能か
    */
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
    static const int m_MAX_BLOCK = 50;      //１レーンのブロックの最大数
    static const int m_START_BLOCK = 0;     //スタート位置のブロックの番号
    static const int m_INIT_RANK = 1;       //プレイヤーに渡す順位データの初期値

private: //data menber

    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    Player* m_player = nullptr;
    ModelRender* m_modelRender[con::PlayerNumberMax][m_MAX_BLOCK] = { nullptr }; //[プレイヤー番号][ステージのマスの数]


    ////////////////////////////////////////////////////////////
    // ブロックのデータ
    ////////////////////////////////////////////////////////////

    int m_stageData[con::PlayerNumberMax][m_MAX_BLOCK] = { greenBlock }; //ステージのデータを保存する配列
    int m_playerBlockPosition[con::PlayerNumberMax] = {                  //プレイヤーが何個目のブロックにいるか
        m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };
    int m_playerBeforeBlockPosition[con::PlayerNumberMax] = {            //プレイヤーの前にいたブロックの番号
        m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };

    ////////////////////////////////////////////////////////////
    // プレイヤーの操作状況
    ////////////////////////////////////////////////////////////

    bool m_activeOperation[con::PlayerNumberMax] = { true, true, true, true };   //プレイヤーが操作可能か
    int m_timerReturnOperation[con::PlayerNumberMax] = { 0, 0, 0, 0 };           //プレイヤーの操作復帰のタイマー
    bool m_resistanceImpossibleOperation[con::PlayerNumberMax] = {               //プレイヤーの操作不可状態に対する耐性があるか
        false, false, false, false };


    bool m_flagAnimationBlueBlock[con::PlayerNumberMax] = {                 //青いブロックに行ったときのアニメーションをおこなっているか
        false, false, false, false };
    int m_timerAnimationBlueBlock[con::PlayerNumberMax] = { 0, 0, 0, 0 };   //青いブロックに行ったときのアニメーションのタイマー
    bool m_activeOperationVersionBlue[con::PlayerNumberMax] = {             //プレイヤーが操作可能か（青色のブロックVer）
        true, true, true, true };

    int m_nowRank = m_INIT_RANK;            //プレイヤーの順位データに渡すデータ
};