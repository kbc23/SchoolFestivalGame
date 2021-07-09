#pragma once
#include "model_render.h"
#include "sound_BGM.h"
#include "effect_render.h"
#include "font_render.h"
#include "sprite_render.h"
#include "constant.h"

class Player;
class Score;
class MainProcessing;
class Rule1;

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start() override final;
    void Init() override final;
    void Finish() override final;
    void Update() override final;


private:
    ////////////////////////////////////////////////////////////
    // 初期設定
    ////////////////////////////////////////////////////////////

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

    /**
     * @brief 再生するBGMの決定
    */
    void InitBGM();

    ////////////////////////////////////////////////////////////
    // 毎フレームの処理
    ////////////////////////////////////////////////////////////



    void DrawBlock(const int pNum);

    void DrawMoveBlock(const int pNum);

    void DrawFontPlayerBlockPosition(const int pNum);

    void DrawBackground();

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

    /**
     * @brief 20マス離れているかどうか
    */

    void Length();

public:
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
    // 進行度
    //////////////////////////////

    void DegreeOfProgress(const int& pNum);



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
    /**
* @brief m_stageDataの2個先のGet関数
* @param pNum プレイヤー番号
* @return m_stageDataの2個先のブロック
*/
    int GetStageDatePuls2(const int pNum)
    {
        /* int plus = m_playerBlockPosition[pNum] + 2;
          if (plus >= 99) {
              plus = 99;
          }*/
        return m_stageData[pNum][m_playerBlockPosition[pNum] + 2];//ここ怪しい
    }

    const int Getm(const int o)
    {
        return m;
    }

public: //enum
    //ブロックの種類
    enum StageData
    {
        greenBlock,
        blueBlock,
        yellowBlock,
        goalBlock,
        stageDataMax
    };

    /**
     * @brief 操作するプレイヤーの人数を保存する変数のSet関数
     * @param i 操作するプレイヤーの人数
    */
    void SetMaxPlayer(const int i)
    {
        m_maxPlayer = i;
    }

    public:
        
        bool stop = false;  //黄色、青に乗った時の1ゲーム操作不可能フラグ
        bool rule1NewGO = false;
        

private: //constant
    static const int m_MAX_BLOCK = 100;      //１レーンのブロックの最大数
    static const int m_START_BLOCK = 0;     //スタート位置のブロックの番号
    static const int m_INIT_RANK = 1;       //プレイヤーに渡す順位データの初期値
    
    static const int m_MAX_GREEN_BLOCK = 20;            //作成する緑ブロックのモデルの数
    static const int m_MAX_YELLOW_BLOCK = 10;    //作成する青、黄色ブロックのモデルの数
    static const int m_MAX_GOAL_BLOCK = 1;      //作成するゴールブロックのモデルの数

private: //data menber

    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    Player* m_player = nullptr;
    Score* m_score = nullptr;
    //ModelRender* m_modelRender[con::PlayerNumberMax][m_MAX_BLOCK] = { nullptr }; //[プレイヤー番号][ステージのマスの数]
    ModelRender* m_modelGreenBlock[con::PlayerNumberMax][m_MAX_GREEN_BLOCK] = { nullptr };
    //ModelRender* m_modelBlueBlock[con::PlayerNumberMax][m_MAX_YELLOW_BLOCK] = { nullptr };
    ModelRender* m_modelYellowBlock[con::PlayerNumberMax][m_MAX_YELLOW_BLOCK] = { nullptr };
    ModelRender* m_modelGoalBlock[con::PlayerNumberMax][m_MAX_GOAL_BLOCK] = { nullptr };
    SoundBGM* m_bgm = nullptr;
    FontRender* m_fontPlayerBlockPosition[con::PlayerNumberMax] = { nullptr }; //プレイヤーが何個目のブロックにいるかの表示
    SpriteRender* m_spriteBackgroundSky = nullptr;
    SpriteRender* m_spriteBackgroundCloud_1 = nullptr;
    SpriteRender* m_spriteBackgroundCloud_2 = nullptr;
    
    SpriteRender* m_spriteDegreeOfProgress = nullptr;
    SpriteRender* m_spritePlayerMark[con::PlayerNumberMax] = { nullptr };

    MainProcessing* m_game = nullptr;

    

    Rule1* m_rule1 = nullptr;

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

    int m_amountOfMovement[con::PlayerNumberMax] = { 0, 0, 0, 0 };

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//ジャンプアニメーション中か
    int m_timerAnimation[con::PlayerNumberMax] = { 0, 0, 0, 0 };						//アニメーションのタイマー

    int Playermember = 0;

    ///////////////////////////////////////////////////////////
    // NextRound
    ///////////////////////////////////////////////////////////
    int m_maxPlayer = con::PlayerNumberMax;	//プレイヤーの最大数
    int n = 0;          //ゴールしたプレイヤーの数
    int m = 0;          //次のラウンドに移るのに一瞬で行かないための待ち時間
    bool m_allMiss = false;     //プレイヤー全員がミスをしているか

    ///////////////////////////////////////////////////////////
    // Length
    ///////////////////////////////////////////////////////////

    int j = 0;          //一番進んでいる人のブロック数
    int t = 0;          //2番目に進んでいる人のブロック数
};