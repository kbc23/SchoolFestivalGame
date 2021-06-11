#pragma once
#include "model_render.h"

#include "player.h"

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start() override final;
    void Update() override final;


public:
    /**
     * @brief �v���C���[�̈ړ��ɉ����ău���b�N�𓮂����B
     * @param pNum �v���C���[�̔ԍ�
     * @param moveNum �u���b�N�̓�����
    */
    void MoveBlock(const int pNum, const int moveNum);

    /**
     * @brief �S�[�����̏���
    */
    void GoalBlock();



private: //constant
    static const int m_MAX_BLOCK = 20;       //�P���[���̃u���b�N�̍ő吔
    static const int m_START_BLOCK = 0;       //�X�^�[�g�ʒu�̃u���b�N�̔ԍ�
    static const int m_INIT_RANK = 1;

private: //data menber

    Player* m_player = nullptr;
    //[�v���C���[�ԍ�][�X�e�[�W�̃}�X�̐�]
    ModelRender* m_modelRender[Player::PlayerNumberMax][m_MAX_BLOCK] = { nullptr };
 
    //�v���C���[�̈ʒu
    int m_playerBlockPosition[Player::PlayerNumberMax] = { m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };

    int m_nowRank = m_INIT_RANK; //�v���C���[�̏��ʃf�[�^�ɓn���f�[�^
};