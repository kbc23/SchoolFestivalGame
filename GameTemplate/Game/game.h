#pragma once
#include "font_render.h"

class PlayerSelect;
class Player;
class GameCamera;
class Stage;
class Score;
class Rule1;

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;

    ////////////////////////////////////////////////////////////
    // 毎フレームの処理
    ////////////////////////////////////////////////////////////

    void Update() override final;

    //////////////////////////////
    // プレイヤーセレクトシーンの処理
    //////////////////////////////

    /**
     * @brief プレイヤーセレクトシーンの処理
    */
    void PlayerSelectScene();

    /**
     * @brief ゲームシーンで使用するオブジェクトのNewGO
    */
    void NewGOGame();

    //////////////////////////////
    // ゲームシーンの処理
    //////////////////////////////

    /**
     * @brief ゲームシーンの処理
    */
    void GameScene();

    /**
     * @brief ゲーム開始時のカウントダウン
    */
    void StartCountdown();


public: //Get関数
    /**
     * @brief m_StopOperationのGet関数
     * @return プレイヤーの操作処理をできなくしているか
    */
    bool GetStopOperation()
    {
        return m_StopOperation;
    }



public: //Set関数
    /**
     * @brief m_flagPlayerSelectSceneのSet関数
     * @param b プレイヤーセレクトシーンの処理をしているか
    */
    void SetFlagPlayerSelectScene(const bool b)
    {
        m_flagPlayerSelectScene = b;
    }

    /**
     * @brief m_flagGameSceneのSet関数
     * @param b ゲームシーンの処理をしているか
    */
    void SetFlagGameScene(const bool b)
    {
        m_flagGameScene = b;
    }

    /**
     * @brief m_maxPlayerのSet関数
     * @param i 操作しているプレイヤーの数
    */
    void SetMaxPlayer(const int i)
    {
        m_maxPlayer = i;
    }



private: //constant
    static const int m_INIT_COUNT_START_COUNTDOWN = 180;        //m_countStartCountdownの初期値

private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    PlayerSelect* m_playerSelect = nullptr;
    Player* m_player = nullptr;
    GameCamera* m_gameCamera = nullptr;
    Stage* m_stage = nullptr;
    Rule1* m_rule1 = nullptr;
    Score* m_score = nullptr;
    FontRender* m_fontStartCountdown = nullptr;     //カウントダウンのフォント

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    int m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;       //カウントダウンで使用されるタイマー
    bool m_flagStartCountdown = true;                               //カウントダウンをおこなっているかのフラグ
    bool m_StopOperation = true;                                    //プレイヤーの操作ができるか

    ////////////////////////////////////////////////////////////
    // フラグ関連
    ////////////////////////////////////////////////////////////

    bool m_flagPlayerSelectScene = true;        //プレイヤーセレクトシーンの処理をしているか
    bool m_flagGameScene = false;               //ゲームシーンの処理をしているか

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_maxPlayer = 0;                        //操作しているプレイヤーの数
};

