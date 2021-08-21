#pragma once
#include "constant.h"
#include "sprite_render.h"

class Rank : public IGameObject
{
public:
    Rank();
    ~Rank();
    bool Start() override final;
    void Init();
    void Finish();
    void Update() override final;


public:
    void NextRound();


private:
    void SetGoalRanking(const int playerNum, const int rank)
    {
        m_goalRanking[playerNum] = rank;
        m_spriteGoalRank[playerNum][rank]->Activate();
    }



    void SpriteActivate(const int playerNum, const int rank)
    {
        m_spriteGoalRank[playerNum][rank]->Activate();
    }



private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    SpriteRender* m_spriteGoalRank[con::PlayerNumberMax][con::GoalRankMax] = { nullptr }; //ゴール順位の画像

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_goalRanking[con::PlayerNumberMax] =
    { con::rank_notClear, con::rank_notClear, con::rank_notClear, con::rank_notClear };	//プレイヤーのゴール順位


};