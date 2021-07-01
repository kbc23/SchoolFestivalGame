#pragma once
#include "font_render.h"

class Title;
class ModeSelect;
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
    void Update() override final;


private:
    ////////////////////////////////////////////////////////////
    // タイトルシーンの処理
    ////////////////////////////////////////////////////////////

    /**
     * @brief タイトルシーンの処理
    */
    void TitleScene();

    /**
     * @brief プレイヤーセレクトシーンで使用するオブジェクトのNewGO
    */
    void NewGOModeSelectScene();

    ////////////////////////////////////////////////////////////
    // モードセレクトシーンの処理
    ////////////////////////////////////////////////////////////

    /**
     * @brief モードセレクトシーンの処理
    */
    void ModeSelectScene();

    /**
     * @brief プレイヤーセレクトシーンで使用するオブジェクトのNewGO
    */
    void NewGOPlayerSelectScene();

    ////////////////////////////////////////////////////////////
    // プレイヤーセレクトシーンの処理
    ////////////////////////////////////////////////////////////

    /**
     * @brief プレイヤーセレクトシーンの処理
    */
    void PlayerSelectScene();

    /**
     * @brief ゲームシーンで使用するオブジェクトのNewGO
    */
    void NewGOGameScene();

    ////////////////////////////////////////////////////////////
    // ゲームシーンの処理
    ////////////////////////////////////////////////////////////

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
    ///**
    // * @brief m_flagPlayerSelectSceneのSet関数
    // * @param b プレイヤーセレクトシーンの処理をしているか
    //*/
    //void SetFlagPlayerSelectScene(const bool b)
    //{
    //    m_flagPlayerSelectScene = b;
    //}

    ///**
    // * @brief m_flagGameSceneのSet関数
    // * @param b ゲームシーンの処理をしているか
    //*/
    //void SetFlagGameScene(const bool b)
    //{
    //    m_flagGameScene = b;
    //}

    /**
     * @brief m_maxPlayerのSet関数
     * @param i 操作しているプレイヤーの数
    */
    void SetMaxPlayer(const int i)
    {
        m_maxPlayer = i;
    }


private: //enum
    enum class GameStatus
    {
        title,
        modeSelect,
        playerSelect,
        game,
        GameStatusMax
    };

private: //constant
    static const int m_INIT_COUNT_START_COUNTDOWN = 180;        //m_countStartCountdownの初期値

private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    Title* m_title = nullptr;
    ModeSelect* m_modeSelect = nullptr;
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

    GameStatus m_gameStatus = GameStatus::title;

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_maxPlayer = 0;                        //操作しているプレイヤーの数
};

