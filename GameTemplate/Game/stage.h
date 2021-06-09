#pragma once
#include "model_render.h"

#include "player.h"



class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start();
    void Update();


public:
    void MoveBlock(const int pNum, const int moveNum);
    void GoalBlock();



private: //�萔
 
    static const int MAX_BLOCK = 20;       //�P���[���̃u���b�N�̍ő吔
    static const int START_BLOCK = 0;       //�X�^�[�g�ʒu�̃u���b�N�̔ԍ�

private: //data menber
    //[�v���C���[�ԍ�][�X�e�[�W�̃}�X�̐�]
    ModelRender* m_modelRender[Player::PlayerNumberMax][MAX_BLOCK] = { nullptr };
    //�v���C���[�̈ʒu
    int m_playerBlockPosition[Player::PlayerNumberMax] = { START_BLOCK, START_BLOCK, START_BLOCK, START_BLOCK };

};

