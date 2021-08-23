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
class SuddenDeathMode;
class CPUPlayerController;
class Pause;
class GameData;

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start() override final;
    void Init();
    void Finish();
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

    void DrawBlock(const int playerNum);

    void DrawMoveBlock(const int playerNum);

    void DrawFontPlayerBlockPosition(const int playerNum);

    void DrawBackground();

    void DrawRoundWin();


public:

    //////////////////////////////
    // ブロックごとの処理
    //////////////////////////////

    /**
     * @brief プレイヤーが乗っているブロックを判別
     * @param playerNum プレイヤー番号
    */
    void CheckBlock(const int playerNum);

    //////////
    // 青色のブロック
    //////////

    /**
     * @brief 青色のブロックの上に行ったときの処理    
     * @param playerNum プレイヤー番号
    */
    void BlueBlock(const int playerNum);

    //////////
    // 黄色のブロック
    //////////

    /**
     * @brief 黄色のブロックの上に行ったときの処理
     * @param playerNum プレイヤー番号
    */
    void YellowBlock(const int playerNum);

    //////////
    // その他
    //////////

    /**
     * @brief 前にいた位置のブロックに戻る処理
     * @param playerNum プレイヤーの番号
    */
    void ReturnBlock(const int playerNum);

private:

    //////////////////////////////
    // ゴール時の処理
    //////////////////////////////

    /**
     * @brief ゴール時の処理
    */
    void GoalBlock();


public:
    //////////////////////////////
    // ラウンド変更の処理
    //////////////////////////////

    /**
     * @brief ラウンド変更の処理
    */
    void NextRound();


private:
    //////////////////////////////
    // 距離による勝利判定
    //////////////////////////////

    /**
     * @brief 20マス離れているかどうか
    */
    void CheckPlayerDistance();

    /**
     * @brief プレイヤーが１位の条件を満たしているかどうか
     * @param playerNum プレイヤーの番号
     * @return 条件を満たしているか
    */
    bool CheckPlayerRank1(const int playerNum);

    /**
     * @brief プレイヤーが20ブロック以上距離を離しているか
     * @param playerNum プレイヤーの番号
     * @param otherNum 他のプレイヤーの番号
     * @return 距離を離しているか
    */
    bool CheckPlayerDistance20Block(const int playerNum, const int otherNum);

    /**
     * @brief 距離の条件でプレイヤーが勝利したときの処理
     * @param playerNum プレイヤーの番号
    */
    void WinPlayerDistance(const int playerNum);

public:
    //////////////////////////////
    // ブロックの移動
    //////////////////////////////

    /**
     * @brief プレイヤーの移動に応じてブロックを動かす。
     * @param playerNum プレイヤーの番号
     * @param moveNum ブロックの動く量
     * @return ブロックを動かしたかどうか
    */
    bool MoveBlock(const int playerNum, const int moveNum);

    //////////////////////////////
    // 進行度
    //////////////////////////////

    void DegreeOfProgress(const int playerNum);



private: //追加
    void MissRoundWin();

    void BlockDeactivate(const int playerNum);


public: //Get関数

    /**
     * @brief m_stageDataの2個先を取得
     * @param playerNum プレイヤー番号
     * @return 現在、プレイヤーがいるブロックの２個先のブロックの情報
    */
    int& GetStageDateNext2Block(const int playerNum)
    {
        return m_stageData[playerNum][m_playerBlockPosition[playerNum] + 2];//ここ怪しい
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

public: //Set関数
    /**
     * @brief 操作するプレイヤーの人数を保存する変数のSet関数
     * @param playerNum 操作するプレイヤーの人数
    */
    void SetMaxPlayer(const int playerNum)
    {
        m_maxPlayer = playerNum;
    }   


private: //constant
    static const int m_MAX_BLOCK = 100;      //１レーンのブロックの最大数
    static const int m_START_BLOCK = 0;     //スタート位置のブロックの番号
    static const int m_INIT_RANK = con::rank_1;       //プレイヤーに渡す順位データの初期値
    
    static const int m_MAX_GREEN_BLOCK = 20;            //作成する緑ブロックのモデルの数
    static const int m_MAX_YELLOW_BLOCK = 10;    //作成する青、黄色ブロックのモデルの数
    static const int m_MAX_GOAL_BLOCK = 1;      //作成するゴールブロックのモデルの数

    static const int m_MAX_RAUND_WIN = 3;

private: //data menber

    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    ModelRender* m_modelGreenBlock[con::PlayerNumberMax][m_MAX_GREEN_BLOCK] = { nullptr };
    ModelRender* m_modelYellowBlock[con::PlayerNumberMax][m_MAX_YELLOW_BLOCK] = { nullptr };
    ModelRender* m_modelGoalBlock[con::PlayerNumberMax][m_MAX_GOAL_BLOCK] = { nullptr };
    SoundBGM* m_bgm = nullptr;
    FontRender* m_fontPlayerBlockPosition[con::PlayerNumberMax] = { nullptr }; //プレイヤーが何個目のブロックにいるかの表示
    SpriteRender* m_spriteBackgroundSky = nullptr;
    SpriteRender* m_spriteBackgroundCloud_1 = nullptr;
    SpriteRender* m_spriteBackgroundCloud_2 = nullptr;

    SpriteRender* m_spriteDegreeOfProgress = nullptr;
    SpriteRender* m_spritePlayerMark[con::PlayerNumberMax] = { nullptr };

    SpriteRender* m_spriteRoundWin[con::PlayerNumberMax][m_MAX_RAUND_WIN] = { nullptr };

    //////////////////////////////
    // FindGO
    //////////////////////////////

    GameData* m_findGameData = nullptr;
    Player* m_findPlayer = nullptr;
    CPUPlayerController* m_findCPUPlayerController = nullptr;
    MainProcessing* m_findMainProcessing = nullptr;
    SuddenDeathMode* m_findSuddenDeathMode = nullptr;
    Pause* m_findPause = nullptr;

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

    int m_nowRank = m_INIT_RANK;            //プレイヤーの順位データに渡すデータ

    int m_amountOfMovement[con::PlayerNumberMax] = { 0, 0, 0, 0 };

    StageData m_playerAnimation[con::PlayerNumberMax] = { greenBlock ,greenBlock ,greenBlock ,greenBlock };

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//ジャンプアニメーション中か
    int m_timerAnimation[con::PlayerNumberMax] = { 0, 0, 0, 0 };						//アニメーションのタイマー

    ///////////////////////////////////////////////////////////
    // NextRound
    ///////////////////////////////////////////////////////////

    int m_maxPlayer = con::PlayerNumberMax;	//プレイヤーの最大数
    int m_goalPlayer = 0;          //ゴールしたプレイヤーの数
    //int m_nextTime = 0;          //次のラウンドに移るのに一瞬で行かないための待ち時間
    bool m_allMiss = false;     //プレイヤー全員がミスをしているか
};