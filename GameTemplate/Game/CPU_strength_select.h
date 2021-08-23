#pragma once
#include "sprite_render.h"
#include "font_render.h"
#include "sound_SE.h"

class MainProcessing;
class SuddenDeathMode;

class CPUStrengthSelect : public IGameObject
{
public:
    CPUStrengthSelect();
    ~CPUStrengthSelect();
    bool Start() override final;
    void Init();
    void Finish();
    void Update() override final;



private:
    /**
     * @brief 操作するプレイヤーの人数の選択
    */
    void SelectTheNumberOfCPUStrength();

    /**
     * @brief このクラスの処理の終了処理
    */
    void FinishCPUStrengthSelect();

    /**
     * @brief 選択しているUIが上下に移動する処理
    */
    void DrawMove();


public: //Get関数
    /**
     * @brief m_flagFinishのGet関数
     * @return このクラスの処理が終了したか
    */
    const bool GetFlagFinish() const
    {
        return m_flagFinish;
    }


private: //constant
    static const int m_NUMBER_OF_CHOICES = 3;     //選択肢の数



private: //data menber

    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    MainProcessing* m_game = nullptr;

    SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };
    FontRender* m_font = nullptr;
    SoundSE* m_seDecision = nullptr;
    SoundSE* m_seMoveCursor = nullptr;

    //////////////////////////////
    // FindGO
    //////////////////////////////



    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_cursorPosition = 0;       //カーソルの場所

    int m_numberOfPlayer = 0;       //プレイヤーの人数

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagDecision = false;    //人数を決定したかのフラグ

    bool m_flagFinish = false;      //このクラスでするべき処理が終わったか

    bool m_flagDrawMove = true;     //UIが上下に移動する処理に使用するフラグ
};