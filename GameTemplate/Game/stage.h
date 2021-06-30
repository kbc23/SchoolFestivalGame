#pragma once
#include "model_render.h"
#include "sound_BGM.h"
#include "effect_render.h"
#include "constant.h"

class Player;
class Game;

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

    /**
     * @brief 緑ブロックを作成するかどうかの処理
     * @param continuousGreenBlock 連続で作成している緑ブロックの数
     * @param randomGreenNum 緑ブロックを作成するかの乱数
     * @return 緑ブロックを作成するか
    */
    bool CreateGreen(const int continuousGreenBlock, const int randomGreenNum);

    /**
     * @brief 青ブロックと黄色ブロックのどちらを作成するか
     * @param blockNum 現在作成しているブロックのブロック番号
     * @param randomBlueOrYellowNum 青ブロックと黄色ブロックのどちらを作成するかの乱数
    */
    void CreateBlueOrYellow(const int blockNum, const int randomBlueOrYellowNum);

    ////////////////////////////////////////////////////////////
    // 毎フレームの処理
    ////////////////////////////////////////////////////////////

    void Update() override final;

    void DrawBlock(const int pNum);

    void DrawMoveBlock(const int pNum);



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

    //////////////////////////////
    // ラウンド変更の処理
    //////////////////////////////

    /**
     * @brief ラウンド変更の処理
    */
    void NextRound();

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
        stageDataMax
    };

    public:
        bool stop = false;  //黄色、青に乗った時の1ゲーム操作不可能フラグ
        bool goal = false;  //ゴール時のNextRund生成フラグ

private: //constant
    static const int m_MAX_BLOCK = 100;      //１レーンのブロックの最大数
    static const int m_START_BLOCK = 0;     //スタート位置のブロックの番号
    static const int m_INIT_RANK = 1;       //プレイヤーに渡す順位データの初期値

    static const int m_MAX_GREEN_BLOCK = 20;            //作成する緑ブロックのモデルの数
    static const int m_MAX_BLUE_OR_YELLOW_BLOCK = 10;    //作成する青、黄色ブロックのモデルの数

private: //data menber

    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    Player* m_player = nullptr;
    ModelRender* m_modelRender[con::playerNumberMax][m_MAX_BLOCK] = { nullptr }; //[プレイヤー番号][ステージのマスの数]
    ModelRender* m_modelGreenBlock[con::playerNumberMax][m_MAX_GREEN_BLOCK] = { nullptr };
    ModelRender* m_modelBlueBlock[con::playerNumberMax][m_MAX_BLUE_OR_YELLOW_BLOCK] = { nullptr };
    ModelRender* m_modelYellowBlock[con::playerNumberMax][m_MAX_BLUE_OR_YELLOW_BLOCK] = { nullptr };
    SoundBGM* m_bgm = nullptr;
    EffectRender* m_testEffect = nullptr;
    Game* m_game = nullptr;

    ////////////////////////////////////////////////////////////
    // ブロックのデータ
    ////////////////////////////////////////////////////////////

    int m_stageData[con::playerNumberMax][m_MAX_BLOCK] = { greenBlock }; //ステージのデータを保存する配列
    int m_playerBlockPosition[con::playerNumberMax] = {                  //プレイヤーが何個目のブロックにいるか
        m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };
    int m_playerBeforeBlockPosition[con::playerNumberMax] = {            //プレイヤーの前にいたブロックの番号
        m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };

    ////////////////////////////////////////////////////////////
    // プレイヤーの操作状況
    ////////////////////////////////////////////////////////////

    bool m_activeOperation[con::playerNumberMax] = { true, true, true, true };   //プレイヤーが操作可能か
    int m_timerReturnOperation[con::playerNumberMax] = { 0, 0, 0, 0 };           //プレイヤーの操作復帰のタイマー
    bool m_resistanceImpossibleOperation[con::playerNumberMax] = {               //プレイヤーの操作不可状態に対する耐性があるか
        false, false, false, false };


    bool m_flagAnimationBlueBlock[con::playerNumberMax] = {                 //青いブロックに行ったときのアニメーションをおこなっているか
        false, false, false, false };
    int m_timerAnimationBlueBlock[con::playerNumberMax] = { 0, 0, 0, 0 };   //青いブロックに行ったときのアニメーションのタイマー
    bool m_activeOperationVersionBlue[con::playerNumberMax] = {             //プレイヤーが操作可能か（青色のブロックVer）
        true, true, true, true };

    int m_nowRank = m_INIT_RANK;            //プレイヤーの順位データに渡すデータ

    int m_amountOfMovement[con::playerNumberMax] = { 0, 0, 0, 0 };

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    bool m_flagAnimationJump[con::playerNumberMax] = { false, false, false, false };	//ジャンプアニメーション中か
    int m_timerAnimation[con::playerNumberMax] = { 0, 0, 0, 0 };						//アニメーションのタイマー

    int Playermember = 0;
};