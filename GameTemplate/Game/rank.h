#pragma once
#include "constant.h"
#include "sprite_render.h"

class SuddenDeathMode;
class Result;
class MainProcessing;

class Rank : public IGameObject
{
public:
    Rank();
    ~Rank();
    bool Start() override final;
    void Init();
    void Finish();
    void Update() override final;


private:
    void DrawRoundPoint();
    /**
     * @brief ゲームシーンが終了したかを確認（次のラウンドに移行するかも確認）
    */
    void CheckGameEnd();


    void CheckNextRound();

    /**
     * @brief ゲームシーンが終了するときの処理
    */
    void GameEnd();

    /**
     * @brief サドンデスモード時の最終順位を確定
    */
    void SuddenDeathRank();


public:
    /**
     * @brief 次のラウンドに移行する処理
    */
    void NextRound();


public: //Get関数
    /**
     * @brief プレイヤーの順位が確定しているかを確認
     * @param playerNum プレイヤー番号
     * @return プレイヤーの順位が確定しているか
    */
    const bool GetDoingGoal(const int playerNum) const
    {
        //プレイヤーの順位が未確定
        if (con::rank_notClear == m_goalRanking[playerNum]) {
            return false;
        }
        //プレイヤーの順位が確定
        else {
            return true;
        }
    }

    /**
     * @brief プレイヤーのゴール順位を取得
     * @param playerNum プレイヤー番号
     * @return プレイヤーのゴール順位
    */
    const int GetGoalRanking(const int playerNum) const
    {
        return m_goalRanking[playerNum];
    }

    const bool GetFlagNextRound() const
    {
        return m_flagNextRound;
    }


public: //Set関数
    /**
     * @brief 順位を保存
     * @param playerNum プレイヤー番号
     * @param rank 順位
    */
    void SetGoalRanking(const int playerNum, const int rank)
    {
        m_goalRanking[playerNum] = rank; //順位をセット
        m_spriteGoalRank[playerNum][rank]->Activate(); //表示

        //プレイヤー全員の順位が確定したかを確認
        for (int playerNum2 = con::FIRST_ELEMENT_ARRAY; playerNum2 < con::PlayerNumberMax; playerNum2++) {
            //順位がまだ確定していない
            if (con::rank_notClear == m_goalRanking[playerNum2]) {
                return;
            }
        }

        //プレイヤー全員の順位が確定している
        m_flagGameEnd = true; //ゲームが終了した判定にする
    }

    /**
     * @brief 取得ラウンド数を増加する。３ラウンド取得した場合、サドンデスモードを終了する。
     * @param playerNum プレイヤー番号
    */
    void AddRoundPoint(const int playerNum)
    {
        ++m_roundPoint[playerNum];
        m_flagNextRound = true; //次のラウンドに移行する

        //３ラウンド取得したとき
        if (m_roundPoint[playerNum] == 3) {
            m_flagGameEnd = true; //ゲームが終了した判定にする

            //サドンデスモード終了
            //m_finishSuddenDeath = true;
        }
    }

    /**
     * @brief ゲームが終了したかをセット
     * @param flag ゲームが終了したか
    */
    void SetFlagGameEnd(const bool flag)
    {
        m_flagGameEnd = flag;
    }

private: //constant
    static const int m_MAX_RAUND_WIN = 3;



private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    SpriteRender* m_spriteGoalRank[con::PlayerNumberMax][con::GoalRankMax] = { nullptr }; //ゴール順位の画像
    SpriteRender* m_spriteRoundWin[con::PlayerNumberMax][m_MAX_RAUND_WIN] = { nullptr };
    SpriteRender* m_spriteGameEnd = nullptr; //ゲーム終了時の画像

    //////////////////////////////
    // FindGO
    //////////////////////////////

    SuddenDeathMode* m_findSuddenDeathMode = nullptr;
    Result* m_findResult = nullptr;
    MainProcessing* m_findMainProcessing = nullptr;

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_goalRanking[con::PlayerNumberMax] =
    { con::rank_notClear, con::rank_notClear, con::rank_notClear, con::rank_notClear };	//プレイヤーのゴール順位

    int m_roundPoint[con::PlayerNumberMax] = { 0,0,0,0 }; //プレイヤーのラウンドのポイント

    bool m_flagGameEnd = false; //ゲームが終了したか
    bool m_flagNextRound = false; //次のラウンドに移行するか

    int m_timerGameEnd = con::TIME_RESET_ZERO; //ゲーム終了時のタイマー
    int m_timerNextRound = con::TIME_RESET_ZERO; //次のラウンドに移行するタイマー
};