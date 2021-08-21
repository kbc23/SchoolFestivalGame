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

    /**
     * @brief 背景画像を移動させる処理
    */
    void DrawBackground();

    ////////////////////////////////////////////////////////////
    // シーンごとの処理
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // ゲーム開始時のロードの処理
    //////////////////////////////

    /**
     * @brief ゲーム開始時のロード
    */
    void StartLoading();

    //////////////////////////////
    // タイトルシーンの処理
    //////////////////////////////

    /**
     * @brief タイトルシーンの処理
    */
    void TitleScene();

    /**
     * @brief 決定選択時
    */
    void DecisionTitleScene();

    //////////////////////////////
    // モードセレクトシーンの処理
    //////////////////////////////

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

    //////////////////////////////
    // プレイヤーセレクトシーンの処理
    //////////////////////////////

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

    //////////////////////////////
    // CPUの難易度選択シーンの処理
    //////////////////////////////

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

    //////////////////////////////
    // ゲームシーンのためのロード
    //////////////////////////////

    /**
     * @brief ロードの処理
    */
    void LoadingGameScene();

    /**
     * @brief ロードの準備
    */
    void PreparingForLoading();

    /**
     * @brief ロード
    */
    void Loading();

    /**
     * @brief ロードの終わり
    */
    void EndOfLoading();

    //////////////////////////////
    // ゲームシーンの処理
    //////////////////////////////

    /**
     * @brief ゲームシーンの処理
    */
    void GameScene();

    /**
     * @brief ゲームシーン終了時
    */
    void FinishGameScene();

    //////////////////////////////
    // リザルトシーンの処理
    //////////////////////////////

    /**
     * @brief リザルトシーンの処理
    */
    void ResultScene();



public: //Get関数
    /**
     * @brief m_StopOperationのGet関数
     * @return プレイヤーの操作処理をできなくしているか
    */
    const bool GetStopOperation()
    {
        return m_StopOperation;
    }

    const bool GetFlagSuddenDeathMode()
    {
        return m_flagSuddenDeathMode;
    }

public: //Set関数
    /**
     * @brief m_maxPlayerのSet関数
     * @param maxPlayer 操作しているプレイヤーの数
    */
    void SetMaxPlayer(int maxPlayer)
    {
        m_maxPlayer = maxPlayer;
    }

    /**
     * @brief プレイヤーの順位をセット
     * @param playerNum プレイヤーの番号
     * @param rank 順位
    */
    void SetRank(int playerNum, int rank)
    {
        m_rank[playerNum] = rank;
    }

    /**
     * @brief CPUの強さをセット
     * @param level CPUの強さ
    */
    void SetCPULevel(const con::CPULevel& level)
    {
        m_CPULevel = level;
    }

    /**
     * @brief リザルト画面でどの選択肢を選んだかをセット
     * @param select リザルト画面でどの選択肢を選んだか
    */
    void SetResultSelect(int select)
    {
        m_resultselect = select;
    }

    /**
     * @brief サドンデスモードを選択したかをセット
     * @param flagSuddenDeathMode サドンデスモードを選択したか
    */
    void SetFlagSuddenDeathMode(bool flagSuddenDeathMode)
    {
        m_flagSuddenDeathMode = flagSuddenDeathMode;
    }

    /**
     * @brief ゲームの処理が終わっているか
     * @param flagGameEnd ゲームの処理が終わっているか
    */
    void SetGameEnd(bool flagGameEnd)
    {
        m_gameEnd = flagGameEnd;
    }

    /**
     * @brief ポーズ画面で「リトライ」を選択したかをセット
     * @param flagPauseStage ポーズ画面で「リトライ」を選択したか
    */
    void SetPause_Stage(bool flagPauseStage)
    {
        m_pause_stage = flagPauseStage;
    }

    /**
     * @brief ポーズ画面で「タイトルに戻る」を選択したかをセット
     * @param flagPauseTitle ポーズ画面で「タイトルに戻る」を選択したか
    */
    void SetPause_Title(bool flagPauseTitle)
    {
        m_pause_title = flagPauseTitle;
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

private: //constant


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

    GameCamera* m_gameCamera = nullptr;
    
    
    SpriteRender* m_spriteBackground[m_MAX_BACKGROUND] = { nullptr };
    Fade* m_fade = nullptr;
    SoundBGM* m_bgmTitle = nullptr;
    SoundSE* m_seCancel = nullptr;

    
    

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    bool m_StopOperation = true; //プレイヤーの操作ができるか
    bool m_gameSceneFlagFinish = false; //ゲームシーンでやることが終わっているか
    
    ////////////////////////////////////////////////////////////
    // フラグ関連
    ////////////////////////////////////////////////////////////

    GameStatus m_gameStatus = GameStatus::title;
    StartLoadingStatus m_startLoadingStatus = StartLoadingStatus::preparation;
    LoadingStatus m_loadStatus = LoadingStatus::doNothing;
    bool m_startPreparingForLoading = false;
    bool m_startEndOfLoading = false;
    bool m_flagGameStart = false;

    bool m_gameEnd = false;


    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_maxPlayer = 0;                        //操作しているプレイヤーの数
    int m_rank[con::PlayerNumberMax] = { 0,0,0,0 };
    con::CPULevel m_CPULevel = con::easy;//難易度受け取り受け渡し用
    int m_resultselect = 0;//リザルトで何選んだか

    bool m_flagSuddenDeathMode = false;

    bool m_pause_stage = false;
    
    bool m_pause_title = false;

};

