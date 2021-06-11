#include "stdafx.h"
#include "stage.h"



//���f���̓ǂݍ��݂Ŏ��Ԃ��������Ă���̂ŁA
//�ǂݍ��ޕK�v�����郂�f���̐������炷���Ƃ��ł��Ȃ������؂��Ă݂邱�ƁB
//�����݂́A�v���g�Ȃ̂ł��̂܂܂ő��s



namespace //constant
{
    //////////////////////////////
    // �t�@�C���p�X
    //////////////////////////////

    const char* FILE_PATH_TKM_GREEN_BLOCK = "Assets/modelData/green.tkm";
    const char* FILE_PATH_TKM_BLUE_BLOCK = "Assets/modelData/blue.tkm";
    const char* FILE_PATH_TKM_YELLOW_BLOCK = "Assets/modelData/yellow.tkm";

    //////////////////////////////
    // �u���b�N�̏��
    //////////////////////////////

    const float BLOCK_SIZE = 80.0f;         //�u���b�N�̃T�C�Y

    //////////////////////////////
    // �ʒu���
    //////////////////////////////

    const float BLOCK_POSITION_X[Player::PlayerNumberMax] = {           //�u���b�N��X���W
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

    //////////////////////////////
    // �^�C�}�[�֘A
    //////////////////////////////

    const int TIME_RETURN_OPERATION = 90;   //���앜�A�ɂ����鎞�ԁi1.5�b�j
}



Stage::Stage()
{

}

Stage::~Stage()
{
    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        for (int s = 0; s < m_MAX_BLOCK; s++) {
            DeleteGO(m_modelRender[i][s]);
        }
    }
}

////////////////////////////////////////////////////////////
// Start�֐��֘A
////////////////////////////////////////////////////////////

bool Stage::Start()
{
    //�X�e�[�W���쐬
    StageCreate();

    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        for (int s = 0; s < m_MAX_BLOCK; s++) {
            m_modelRender[i][s] = NewGO<ModelRender>(0);

            //�X�e�[�W�̏��ɂă��f����ύX
            if (m_stageData[s] == greenBlock) {
                m_modelRender[i][s]->Init(FILE_PATH_TKM_GREEN_BLOCK);
            }
            else if (m_stageData[s] == blueBlock) {
                m_modelRender[i][s]->Init(FILE_PATH_TKM_BLUE_BLOCK);
            }
            else if (m_stageData[s] == yellowBlock)
            {
                m_modelRender[i][s]->Init(FILE_PATH_TKM_YELLOW_BLOCK);
            }

           
            //m_modelRender[i][s]->SetScale(BLOCK_SCALE);
            m_modelRender[i][s]->SetPosition({
                BLOCK_START_POSITION[i].x,
                BLOCK_START_POSITION[i].y,
                BLOCK_START_POSITION[i].z + BLOCK_SIZE * s
            });
        }
    }

    m_player = FindGO<Player>("player");

    return true;
}

void Stage::StageCreate()
{
    for (int i = 0; i < m_MAX_BLOCK; i++) {
        m_stageData[i] = greenBlock;
    }

    m_stageData[5] = blueBlock;
    m_stageData[13] = yellowBlock;
    m_stageData[19] = yellowBlock;
    m_stageData[23] = blueBlock;
    m_stageData[27] = blueBlock;
}

////////////////////////////////////////////////////////////
// ���t���[���̏���
////////////////////////////////////////////////////////////

void Stage::Update()
{
    //�X�^�[�g�n�_�ɖ߂�( Debug )
    if (g_pad[0]->IsTrigger(enButtonX)) {
        for (int i = 0; i < Player::PlayerNumberMax; i++) {
            for (int s = 0; s < m_MAX_BLOCK; s++) {
                m_modelRender[i][s]->SetPosition({
                    BLOCK_START_POSITION[i].x,
                    BLOCK_START_POSITION[i].y,
                    BLOCK_START_POSITION[i].z + BLOCK_SIZE * s
                    });
            }
            m_playerBlockPosition[i] = 0;
        }
    }


    //�v���C���[���Ƃ̏���
    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        CheckBlock(i);

        ReturnOperationTimer(i);
    }




    GoalBlock();
}

void Stage::MoveBlock(const int pNum, const int moveNum)
{
    if (m_activeOperation[pNum] == false) {
        return;
    }

    //����s�\��Ԃɑ΂���ϐ����폜
    m_resistanceImpossibleOperation[pNum] = false;

    //�v���C���[�̌��݂̈ʒu��ۑ�
    m_playerBeforeBlockPosition[pNum] = m_playerBlockPosition[pNum];

    //�v���C���[�����ڂ̃u���b�N�ɂ��邩�̏����X�V
    m_playerBlockPosition[pNum] += moveNum;
    if (m_playerBlockPosition[pNum] >= m_MAX_BLOCK) {
        m_playerBlockPosition[pNum] = m_MAX_BLOCK - 1;
    }

    //���f���̈ʒu���X�V
    for (int i = 0; i < m_MAX_BLOCK; i++) {
        m_modelRender[pNum][i]->SetPosition({
            BLOCK_POSITION_X[pNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * i - BLOCK_SIZE * (m_playerBlockPosition[pNum])
            //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
        });
    }
}

void Stage::ReturnBlock(const int pNum)
{
    //�v���C���[�̌��݈ʒu��O�̈ʒu�ɖ߂��B
    m_playerBlockPosition[pNum] = m_playerBeforeBlockPosition[pNum];

    //���f���̈ʒu���X�V
    for (int i = 0; i < m_MAX_BLOCK; i++) {
        m_modelRender[pNum][i]->SetPosition({
            BLOCK_POSITION_X[pNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * i - BLOCK_SIZE * (m_playerBlockPosition[pNum])
            //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
        });
    }
}

void Stage::ReturnOperationTimer(const int pNum)
{
    if (m_activeOperation[pNum] == true) {
        return;
    }

    ++m_timerReturnOperation[pNum];

    if (m_timerReturnOperation[pNum] >= TIME_RETURN_OPERATION) {
        m_activeOperation[pNum] = true;
        m_timerReturnOperation[pNum] = 0;
        m_resistanceImpossibleOperation[pNum] = true;
    }

}

void Stage::CheckBlock(const int pNum)
{
    if (m_stageData[m_playerBlockPosition[pNum]] == greenBlock) {

    }
    else if (m_stageData[m_playerBlockPosition[pNum]] == blueBlock) {
        ReturnBlock(pNum);
    }
    else if (m_stageData[m_playerBlockPosition[pNum]] == yellowBlock) {
        if (m_resistanceImpossibleOperation[pNum] == false) {
            m_activeOperation[pNum] = false;
        }
    }
}

void Stage::GoalBlock()
{
    bool addNowRank = false;
    int nextRank = 0;

    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        //�v���C���[�̏��ʂ��m��
        if (m_player->GetActivePlayer(i) == true && m_playerBlockPosition[i] == m_MAX_BLOCK - 1) {
            //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
            m_player->SetActivePlayer(i, false);

            //���ʂ��m��
            m_player->SetGoalRanking(i, m_nowRank);

            ++nextRank;
            addNowRank = true;
        }
    }

    if (addNowRank == true) {
        //���̏��ʂ�ݒ�
        m_nowRank += nextRank;
    }
}