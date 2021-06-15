#pragma once
#include "font_render.h"
#include "game.h"

class PlayerSelect : public IGameObject
{
public:
    PlayerSelect();
    ~PlayerSelect();
    bool Start() override final;
    void Update() override final;



    void SelectTheNumberOfPlayers();


    void Kari();



public: //Get関数

    bool GetmFlagFinish()
    {
        return m_flagFinish;
    }




private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    Game* m_game = nullptr;
    FontRender* m_fontNumberOfPlayer[3] = { nullptr };
    FontRender* m_fontCursor = nullptr;

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