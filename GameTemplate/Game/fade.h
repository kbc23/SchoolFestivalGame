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
    void FadeOut();

    /**
     * @brief フェードの量を減少
    */
    void FadeIn();

public: //Get関数


public: //Set関数
    /**
     * @brief フェードアウトの量を変更
     * @param f フェードアウトの量
    */
    void SetAmountOfChangeFadeOut(const float& f)
    {
        m_amountOfChangeFadeOut = f;
    }

    /**
     * @brief フェードインの量を変更
     * @param f フェードインの量
    */
    void SetAmountOfChangeFadeIn(const float& f)
    {
        m_amountOfChangeFadeIn = f;
    }


public: //その他
    /**
     * @brief フェードを始める
    */
    void StartFadeOut()
    {
        m_statusFade = CurrentFadeState::fadeOut;
    }

    void StartFadeIn()
    {
        m_statusFade = CurrentFadeState::fadeIn;
    }

    /**
     * @brief 現在フェードをしているか
     * @return 現在フェードをしているか
    */
    const bool& IsFadeProgress()
    {
        if (m_statusFade == CurrentFadeState::wait) {
            return false;
        }
        else {
            return true;
        }
    }

    /**
     * @brief 現在フェードアウトをしているか
     * @return 現在フェードアウトをしているか
    */
    const bool& IsFadeOutProgress()
    {
        if (m_statusFade == CurrentFadeState::fadeOut) {
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @brief 現在フェードインをしているか
     * @return 現在フェードインをしているか
    */
    const bool& IsFadeInProgress()
    {
        if (m_statusFade == CurrentFadeState::fadeIn) {
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @brief フェードの緊急停止
    */
    void EmergencyStop()
    {
        m_statusFade = CurrentFadeState::wait;
        m_amountOfFade = 0.0f;
        m_spriteFade->SetMulColorW(m_amountOfFade);
    }


private: //enum
    /**
     * @brief 現在のフェードの状態
    */
    enum class CurrentFadeState
    {
        wait,                   //待機中
        fadeOut,                //フェードアウト中
        fadeIn,                 //フェードイン中
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

    float m_amountOfFade = 0.0f;                //現在のフェードの量
    float m_amountOfChangeFadeOut = 0.1f;      //フェードの増加量（1フレーム）
    float m_amountOfChangeFadeIn = 0.1f;      //フェードの減少量（1フレーム）
    CurrentFadeState m_statusFade = CurrentFadeState::wait;    //現在のフェードの状態

};