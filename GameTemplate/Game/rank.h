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
    /**
     * @brief ゲームシーンが終了したかを確認（次のラウンドに移行するかも確認）
    */
    void CheckGameEnd();

    /**
     * @brief ゲームシーンが終了するときの処理
    */
    void GameEnd();


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
     * @brief ゲームが終了したかをセット
     * @param flag ゲームが終了したか
    */
    void SetFlagGameEnd(const bool flag)
    {
        m_flagGameEnd = flag;
    }



    void SpriteActivate(const int playerNum, const int rank)
    {
        m_spriteGoalRank[playerNum][rank]->Activate();
    }



private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    SpriteRender* m_spriteGoalRank[con::PlayerNumberMax][con::GoalRankMax] = { nullptr }; //ゴール順位の画像
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

    bool m_flagGameEnd = false; //ゲームが終了したか
};