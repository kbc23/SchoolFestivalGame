#pragma once
#include "sprite_render.h"

class Fade : public IGameObject
{
public:
    Fade();
    ~Fade();
    bool Start() override final;
    void Update() override final;



private:
    /**
     * @brief フェードの処理
    */
    void ChangeAmountOfFade();

    /**
     * @brief フェードの量を増加
    */
    void IncreaseFade();

    /**
     * @brief フェードで画面を隠した状態で待機
    */
    void WaitFade();

    /**
     * @brief フェードの量を減少
    */
    void DecreaseFade();

public: //Get関数


public: //Set関数
    /**
     * @brief フェードの増加量を変更
     * @param f フェードの増加量
    */
    void SetIncreaseAmountOfChange(const float& f)
    {
        m_increaseAmountOfChange = f;
    }

    /**
     * @brief フェードの減少量を変更
     * @param f フェードの減少量
    */
    void SetmDecreaseAmountOfChange(const float& f)
    {
        m_decreaseAmountOfChange = f;
    }

    /**
     * @brief フェードの待機時間を変更
     * @param i フェードの待機時間（60で１秒）
    */
    void SetWaitTime(const int& i)
    {
        m_waitTime = i;
    }


public: //その他
    /**
     * @brief フェードを始める
    */
    void StartFade()
    {
        m_flagFade = true;
    }

    /**
     * @brief 現在フェードをしているか
     * @return 現在フェードをしているか
    */
    const bool& IsFadeInProgress()
    {
        return m_flagFade;
    }

    /**
     * @brief フェードの緊急停止
    */
    void EmergencyStop()
    {
        m_flagFade = false;
        m_stateFade = CurrentFadeState::increase;
        m_amountOfFade = 0.0f;
        m_waitTimer = 0;
        m_spriteFade->SetMulColorW(m_amountOfFade);
    }


private: //enum
    /**
     * @brief 現在のフェードの状態
    */
    enum class CurrentFadeState
    {
        increase,               //増加中
        wait,                   //待機中
        decrease,               //減少中
        CurrentFadeStateMax
    };


private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    SpriteRender* m_spriteFade = nullptr;

    ////////////////////////////////////////////////////////////
    // フェード処理の変数
    ////////////////////////////////////////////////////////////

    bool m_flagFade = false;                    //フェードをしているかのフラグ
    float m_amountOfFade = 0.0f;                //現在のフェードの量
    float m_increaseAmountOfChange = 0.1f;      //フェードの増加量（1フレーム）
    float m_decreaseAmountOfChange = 0.1f;      //フェードの減少量（1フレーム）
    int m_waitTime = 60;                        //フェードの待機時間（60で１秒）
    int m_waitTimer = 0;                        //フェードの待機時間のタイマー
    CurrentFadeState m_stateFade = CurrentFadeState::increase;    //現在のフェードの状態

};