#pragma once
#include "constant.h"

/**
 * @brief ゲームシーン全体で使用するデータを扱うクラス
*/
class GameData : public IGameObject
{
public:
    GameData();
    ~GameData();
    bool Start() override final;
    void Update() override final;


public: //Get関数
    /**
     * @brief プレイするプレイヤーの最大数
     * @return プレイするプレイヤーの最大数
    */
    const int& GetMaxPlayer() const
    {
        return m_maxPlayer;
    }

    /**
     * @brief 選択したCPUの強さ
     * @return 選択したCPUの強さ
    */
    const con::CPULevel& GetCPULevel() const
    {
        return m_CPULevel;
    }


public: //Set関数
    /**
     * @brief プレイするプレイヤーの最大数をセット
     * @param maxPlayer プレイするプレイヤーの最大数
    */
    void SetMaxPlayer(int maxPlayer)
    {
        m_maxPlayer = maxPlayer;
    }

    /**
     * @brief 選択したCPUの強さをセット
     * @param level 選択したCPUの強さ
    */
    void SetCPULevel(const con::CPULevel& level)
    {
        m_CPULevel = level;
    }


private: //data menber
    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_maxPlayer = 0;                    //操作しているプレイヤーの数
    con::CPULevel m_CPULevel = con::easy;   //難易度受け取り受け渡し用
};