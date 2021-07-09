#pragma once
#include "model_render.h"
#include "sprite_render.h"
#include "sound_SE.h"
#include "constant.h"

class Stage;
class MainProcessing;

class Pause : public IGameObject
{
public:
    Pause();
    ~Pause();
    bool Start() override final;
	void Init() override final;
	void Finish() override final;
    void Update() override final;

	/**
	 * @brief ポーズしているか関係なしに呼ばれるアップデート関数
	*/
	void AlwaysUpdate()override final;

	/**
	 * @brief ポーズ中にだけ呼ばれるアップデート関数
	*/
	void UpdateOnlyPaused()override final;

    /**
     * @brief このクラスの処理の終了処理
    */
    void FinishPause();

private:
    //ポーズ画面のときに呼ぶやつ
    void PauseInit();

    //ポーズ画面じゃなくなるときに呼ぶやつ
    void PauseFinish();

private: //constant
	static const int m_NUMBER_OF_CHOICES = 3;     //選択肢の数

private: //data menber
	SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };
    Stage* m_stage = nullptr;
    MainProcessing* m_game = nullptr;

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

    bool m_flagMove = true;
};