#include "stdafx.h"
#include "stage.h"



//���f���̓ǂݍ��݂Ŏ��Ԃ��������Ă���̂ŁA
//�ǂݍ��ޕK�v�����郂�f���̐������炷���Ƃ��ł��Ȃ������؂��Ă݂邱�ƁB
//�����݂́A�v���g�Ȃ̂ł��̂܂܂ő��s



namespace {
    //const Vector3 BLOCK_SCALE = { 1.0f, 1.0f, 1.0f };
    const float BLOCK_SIZE = 80.0f;                                     //�u���b�N�̃T�C�Y
    const float BLOCK_POSITION_X[Player::PlayerNumberMax] = {           //�u���b�NX���W
        390.0f,                                                             //�v���C���[�P
        130.0f,                                                             //�v���C���[�Q
        -130.0f,                                                            //�v���C���[�R
        -390.0f                                                             //�v���C���[�S
    };
    const float BLOCK_POSITION_Y = -80.0f;                              //�u���b�N��Y���W
    const float BLOCK_POSITION_Z = -250.0f;                             //�u���b�N��Z���W
    const Vector3 BLOCK_START_POSITION[Player::PlayerNumberMax] = {     //�v���C���[���Ƃ̃X�^�[�g�ʒu�̃u���b�N�̍��W
        { 390.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z }, 	                //�v���C���[�P
        { 130.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z },	                    //�v���C���[�Q
        { -130.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z },	                //�v���C���[�R
        { -390.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z }	                    //�v���C���[�S
    };
}



Stage::Stage()
{

}

Stage::~Stage()
{
    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        for (int s = 0; s < MAX_BLOCK; s++) {
            DeleteGO(m_modelRender[i][s]);
        }
    }
}

bool Stage::Start()
{
    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        for (int s = 0; s < MAX_BLOCK; s++) {
            m_modelRender[i][s] = NewGO<ModelRender>(0);
            m_modelRender[i][s]->Init("Assets/modelData/green.tkm");
            //m_modelRender[i][s]->SetScale(BLOCK_SCALE);
            m_modelRender[i][s]->SetPosition({
                BLOCK_START_POSITION[i].x,
                BLOCK_START_POSITION[i].y,
                BLOCK_START_POSITION[i].z + BLOCK_SIZE * s
            });
        }
    }

    return true;
}

void Stage::Update()
{
    //�X�^�[�g�n�_�ɖ߂�( Debug )
    if (g_pad[0]->IsTrigger(enButtonX)) {
        for (int i = 0; i < Player::PlayerNumberMax; i++) {
            for (int s = 0; s < MAX_BLOCK; s++) {
                m_modelRender[i][s]->SetPosition({
                    BLOCK_START_POSITION[i].x,
                    BLOCK_START_POSITION[i].y,
                    BLOCK_START_POSITION[i].z + BLOCK_SIZE * s
                    });
            }
            m_playerBlockPosition[i] = 0;
        }
    }

    GoalBlock();
}

void Stage::MoveBlock(const int pNum, const int moveNum)
{
    //�v���C���[�����ڂ̃u���b�N�ɂ��邩�̏����X�V
    m_playerBlockPosition[pNum] += moveNum;
    if (m_playerBlockPosition[pNum] >= MAX_BLOCK) {
        m_playerBlockPosition[pNum] = MAX_BLOCK - 1;
    }

    for (int i = 0; i < MAX_BLOCK; i++) {
        //�Ō�̃u���b�N��
        if (m_playerBlockPosition[pNum] >= MAX_BLOCK) {
            m_modelRender[pNum][i]->SetPosition({
                BLOCK_POSITION_X[pNum],
                BLOCK_POSITION_Y,
                BLOCK_POSITION_Z - BLOCK_SIZE * (MAX_BLOCK - 1) + 80.0f * i
                //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
            });
        }
        else {
            m_modelRender[pNum][i]->SetPosition({
                BLOCK_POSITION_X[pNum],
                BLOCK_POSITION_Y,
                BLOCK_POSITION_Z + BLOCK_SIZE * i - BLOCK_SIZE * (m_playerBlockPosition[pNum])
                //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
                });
        }
    }
}

void Stage::GoalBlock()
{
    for (int i = 0; i < Player::PlayerNumberMax; i++) {

    }



}