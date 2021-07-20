#pragma once
#include "font_render.h"
#include "sprite_render.h"
#include "fade.h"
#include "sound_BGM.h"
#include "sound_SE.h"
#include "constant.h"


class Title;
class ModeSelect;
class PlayerSelect;
class CPUStrengthSelect;
class Game;
class EnemyAI;
class Player;
class Result;

class GameCamera;




class MainProcessing : public IGameObject
{
public:
    MainProcessing();
    ~MainProcessing();
    bool Start() override final;
    void Update() override final;


private:
    ////////////////////////////////////////////////////////////
    // 毎フレームする処理
    ////////////////////////////////////////////////////////////

    void DrawBackground();

    ////////////////////////////////////////////////////////////
    // 最初の読み込み
    ////////////////////////////////////////////////////////////

    void StartLoadingScene();

    /**
     * @brief ロードの準備
    */
    void StartLoadingPreparation();

    void StartLoading();

    void StartLoadingFinish();

    ////////////////////////////////////////////////////////////
    // タイトルシーンの処理
    ////////////////////////////////////////////////////////////

    /**
     * @brief タイトルシーンの処理
    */
    void TitleScene();

    /**
     * @brief 決定選択時
    */
    void DecisionTitleScene();

    ////////////////////////////////////////////////////////////
    // モードセレクトシーンの処理
    ////////////////////////////////////////////////////////////

    /**
     * @brief モードセレクトシーンの処理
    */
    void ModeSelectScene();

    /**
     * @brief 決定選択時
    */
    void DecisionModeSelectScene();

    /**
     * @brief キャンセル選択時
    */
    void CancelModeSelectScene();

    ////////////////////////////////////////////////////////////
    // プレイヤーセレクトシーンの処理
    ////////////////////////////////////////////////////////////

    /**
     * @brief プレイヤーセレクトシーンの処理
    */
    void PlayerSelectScene();

    /**
     * @brief 決定選択時
    */
    void DecisionPlayerSelectScene();

    /**
     * @brief キャンセル選択時
    */
    void CancelPlayerSelectScene();

    ////////////////////////////////////////////////////////////
    // CPUの難易度選択シーンの処理
    ////////////////////////////////////////////////////////////

    /**
     * @brief タイトルシーンの処理
    */
    void CPUStrengthSelectScene();

    /**
     * @brief 決定選択時
    */
    void DecisionCPUStrengthSelectScene();

    /**
     * @brief キャンセル選択時
    */
    void CancelCPUStrengthSelectScene();

    ////////////////////////////////////////////////////////////
    // ゲームシーンのためのロード
    ////////////////////////////////////////////////////////////

    void LoadingGameScene();

    void PreparingForLoading();

    void Loading();

    void EndOfLoading();

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

    /**
     * @brief ゲームシーン終了時
    */
    void FinishGameScene();

    ////////////////////////////////////////////////////////////
    // リザルトシーンの処理
    ////////////////////////////////////////////////////////////
    /**
     * @brief リザルトシーンの処理
    */
    void ResultScene();

public: //Get関数
    /**
     * @brief m_StopOperationのGet関数
     * @return プレイヤーの操作処理をできなくしているか
    */
    bool GetStopOperation()
    {
        return m_StopOperation;
    }

    const bool& GetRuleSelect()
    {
        return m_ruleSelect;
    }

    /*const bool& GetRuleSelect()
    {
        return m_ruleSelect;
    }*/

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

    void SetRank(int pNum, int rank) {//順位受け取りtuika
        m_rank[pNum] = rank;
    }
    void SetDiLevel(const int& i)//難易度受け取りtuika
    {
        m_dilevel = i;
    }
    void SetResultSelect(const int i)//リザルト選択受け取りtuika
    {
        m_resultselect = i;
    }

    void SetRuleSelect(const bool& b)
    {
        m_ruleSelect = b;
    }

    void SetGameEnd(const bool& b)
    {
        m_gameEnd = b;
    }

    void SetPause_Stage(const bool& b)
    {
        m_pause_stage = b;
    }

    void SetPause_Title(const bool& b)
    {
        m_pause_title = b;
    }


private: //enum
    enum class GameStatus
    {
        startLoading,
        title,
        modeSelect,
        playerSelect,
        CPUStrengthSelect,
        loadingGame,
        game,
        result,
        GameStatusMax
    };

    enum class StartLoadingStatus
    {
        preparation,    //ロードの準備
        loading,        //ロード中
        finish,         //ロード終了
        StartLoadingStatusMax
    };

    enum class LoadingStatus
    {
        doNothing,              //何もしていない
        preparingForLoading,    //ロードの準備
        loading,                //ロード中
        endOfLoading,           //ロード終了
        LoadingStatusMax
    };


public:
    //bool goal1 = false;

    void NextRound();

private: //constant
    static const int m_INIT_COUNT_START_COUNTDOWN = 240;        //m_countStartCountdownの初期値
    static const int m_MAX_COUNTDOWN = 4;               //カウントダウンに使用する画像の最大枚数


public: //constant
    static const int m_MAX_BACKGROUND = 7;                  //背景画像の最大枚数



private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    Title* m_title = nullptr;
    ModeSelect* m_modeSelect = nullptr;
    PlayerSelect* m_playerSelect = nullptr;
    CPUStrengthSelect* m_CPUStrengthSelect = nullptr;
    Game* m_game = nullptr;
    Result* m_result = nullptr;
    Player* m_player = nullptr;
    EnemyAI* m_enemyAI = nullptr;

    GameCamera* m_gameCamera = nullptr;
    
    
    SpriteRender* m_spriteBackground[m_MAX_BACKGROUND] = { nullptr };
    SpriteRender* m_spriteCountdown[m_MAX_COUNTDOWN] = { nullptr };
    SpriteRender* m_spriteLoading = nullptr;
    Fade* m_fade = nullptr;
    SoundBGM* m_bgmTitle = nullptr;
    SoundSE* m_seCancel = nullptr;
    SoundSE* m_seCount = nullptr;
    SoundSE* m_seGameStart = nullptr;
    
    

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    float m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;       //カウントダウンで使用されるタイマー
    bool m_flagStartCountdown = true;                               //カウントダウンをおこなっているかのフラグ
    bool m_StopOperation = true;                                    //プレイヤーの操作ができるか
    bool m_gameSceneFlagFinish = false;//ゲームシーンでやることが終わっているかtuika
    
    ////////////////////////////////////////////////////////////
    // フラグ関連
    ////////////////////////////////////////////////////////////

    GameStatus m_gameStatus = GameStatus::startLoading;
    StartLoadingStatus m_startLoadingStatus = StartLoadingStatus::preparation;
    LoadingStatus m_loadStatus = LoadingStatus::doNothing;
    bool m_startPreparingForLoading = false;
    bool m_startEndOfLoading = false;
    bool m_flagGameStart = false;

    bool m_gameEnd = false;

    bool m_flagCountSE3 = false;
    bool m_flagCountSE2 = false;
    bool m_flagCountSE1 = false;
    bool m_flagStartCountdownSE = false;

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_maxPlayer = 0;                        //操作しているプレイヤーの数
    int m_rank[con::PlayerNumberMax] = { 0,0,0,0 };
    int m_dilevel = 0;//難易度受け取り受け渡し用
    int m_resultselect = 0;//リザルトで何選んだか
    //bool m_ruleSelect = false;

    bool m_ruleSelect = false;

    bool m_pause_stage = false;
    
    bool m_pause_title = false;

};

