#pragma once
#include "font_render.h"

#include "player.h"
#include "game.h"

class Score : public IGameObject
{
public:
    Score();
    ~Score();
    bool Start() override final;
    void Update() override final;

    /**
     * @brief �X�R�A���Ԃ𑝉�����֐�
     * @param pNum �v���C���[�ԍ�
    */
    void AddTime(const int pNum);

    /**
     * @brief �X�R�A���Ԃ̑������~����֐�
     * @param pNum �v���C���[�ԍ�
    */
    void FinishTime(const int pNum);

    /**
     * @brief �X�R�A���Ԃ̕`�揈��
     * @param pNum �v���C���[�ԍ�
    */
    void DrawTime(const int pNum);



private: //data menber
    Player* m_player = nullptr;
    Game* m_game = nullptr;
    FontRender* m_fontScoreTime[Player::PlayerNumberMax] = { nullptr };

    int m_scoreTime[Player::PlayerNumberMax] = { 0,0,0,0 };

    bool m_flagProcessing[Player::PlayerNumberMax] = { true,true,true,true };


    int m_scoreTimeMinutes[Player::PlayerNumberMax] = { 0,0,0,0 };            //��
    int m_scoreTimeSeconds[Player::PlayerNumberMax] = { 0,0,0,0 };            //�b
    int m_scoreTimeCommaSeconds[Player::PlayerNumberMax] = { 0,0,0,0 };       //�R���}�b

};

