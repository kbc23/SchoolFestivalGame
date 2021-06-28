#pragma once
#include "font_render.h"
#include "sprite_render.h"

class Game;

class PlayerSelect : public IGameObject
{
public:
    PlayerSelect();
    ~PlayerSelect();
    bool Start() override final;
    
    ////////////////////////////////////////////////////////////
    // 毎フレームの処理
    ////////////////////////////////////////////////////////////

    void Update() override final;

    /**
     * @brief 操作するプレイヤーの人数の選択
    */
    void SelectTheNumberOfPlayers();

    /**
     * @brief このクラスの処理の終了処理
    */
    void FinishPlayerSelect();



public: //Get関数
    /**
     * @brief m_flagFinishのGet関数
     * @return このクラスの処理が終了したか
    */
    bool GetmFlagFinish()
    {
        return m_flagFinish;
    }


private: //constant
    static const int m_NUMBER_OF_FONTS = 3;     //フォントの数


private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    Game* m_game = nullptr;
    FontRender* m_fontNumberOfPlayer[m_NUMBER_OF_FONTS] = { nullptr };      //操作するプレイヤーの人数を選択するフォント
    FontRender* m_fontCursor = nullptr;                     //カーソルのフォント
    SpriteRender* m_spriteBackground = nullptr;

    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_cursorPosition = 0;       //カーソルの場所

    int m_numberOfPlayer = 0;       //プレイヤーの人数

    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_flagDecision = false;    //人数を決定したかのフラグ

    bool m_flagFinish = false;      //人数を決定した後の処理を終了したかのフラグ

    bool m_flagInput = false;       //前のフレームで入力しているかどうかのフラグ
};