#pragma once
#include "sprite_render.h"

class Goal : public IGameObject
{
public:
    Goal();
    ~Goal();
    bool Start() override final;
    void Init();
    void Finish();
    void Update() override final;


private:
    /**
     * @brief ゲームが終了したときの処理
    */
    void GameEnd();


public: //Get関数
    /**
     * @brief ゲームが終了したかを取得
     * @return ゲームが終了したか
    */
    const bool GetFlagGameEnd()
    {
        return m_flagGameEnd;
    }



public: //Set関数
    /**
     * @brief ゲームが終了したかをセット
     * @param flag ゲームが終了したか
    */
    void SetFlagGameEnd(const bool flag)
    {
        m_flagGameEnd = flag;
    }



private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    SpriteRender* m_spriteGameEnd = nullptr; //ゲーム終了時の画像

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    bool m_flagGameEnd = false; //ゲームが終了したか
};