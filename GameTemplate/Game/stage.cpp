#include "stdafx.h"
#include "stage.h"

#include <random>

#include "player.h"



//���f���̓ǂݍ��݂Ŏ��Ԃ��������Ă���̂ŁA
//�ǂݍ��ޕK�v�����郂�f���̐������炷���Ƃ��ł��Ȃ������؂��Ă݂邱�ƁB
//�����݂́A�v���g�Ȃ̂ł��̂܂܂ő��s



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // �t�@�C���p�X
    ////////////////////////////////////////////////////////////

    const char* FILE_PATH_TKM_GREEN_BLOCK = "Assets/modelData/green.tkm";
    const char* FILE_PATH_TKM_BLUE_BLOCK = "Assets/modelData/blue.tkm";
    const char* FILE_PATH_TKM_YELLOW_BLOCK = "Assets/modelData/yellow.tkm";

    ////////////////////////////////////////////////////////////
    // �u���b�N�̏��
    ////////////////////////////////////////////////////////////

    const float BLOCK_SIZE = 80.0f;         //�u���b�N�̃T�C�Y

    ////////////////////////////////////////////////////////////
    // �ʒu���
    ////////////////////////////////////////////////////////////

    const float BLOCK_POSITION_X[con::PlayerNumberMax] = {           //�u���b�N��X���W
        390.0f,                                                             //�v���C���[�P
        130.0f,                                                             //�v���C���[�Q
        -130.0f,                                                            //�v���C���[�R
        -390.0f                                                             //�v���C���[�S
    };
    const float BLOCK_POSITION_Y = -80.0f;                              //�u���b�N��Y���W
    const float BLOCK_POSITION_Z = -250.0f;                             //�u���b�N��Z���W
    const Vector3 BLOCK_START_POSITION[con::PlayerNumberMax] = {     //�v���C���[���Ƃ̃X�^�[�g�ʒu�̃u���b�N�̍��W
        { 390.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z }, 	                //�v���C���[�P
        { 130.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z },	                    //�v���C���[�Q
        { -130.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z },	                //�v���C���[�R
        { -390.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z }	                    //�v���C���[�S
    };

    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    const int TIMER_RESET = 0;                  //�^�C�}�[�̃��Z�b�g
    const int TIME_RETURN_OPERATION = 30;       //���앜�A�ɂ����鎞�ԁi1.5�b�j

    const int TIME_BLUE_BLOCK_ANIMATION = 30;   //�F�̃u���b�N�ɍs�����Ƃ��̃A�j���[�V�����̎���
}



Stage::Stage()
{

}

Stage::~Stage()
{
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = 0; blockNum < m_MAX_BLOCK; blockNum++) {
            DeleteGO(m_modelRender[playerNum][blockNum]);
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

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
            m_modelRender[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_FIRST);

            //�X�e�[�W�̏��ɂă��f����ύX
            if (m_stageData[playerNum][blockNum] == greenBlock) {
                m_modelRender[playerNum][blockNum]->Init(FILE_PATH_TKM_GREEN_BLOCK);
            }
            else if (m_stageData[playerNum][blockNum] == blueBlock) {
                m_modelRender[playerNum][blockNum]->Init(FILE_PATH_TKM_BLUE_BLOCK);
            }
            else if (m_stageData[playerNum][blockNum] == yellowBlock) {
                m_modelRender[playerNum][blockNum]->Init(FILE_PATH_TKM_YELLOW_BLOCK);
            }

            m_modelRender[playerNum][blockNum]->SetPosition({
                BLOCK_START_POSITION[playerNum].x,
                BLOCK_START_POSITION[playerNum].y,
                BLOCK_START_POSITION[playerNum].z + BLOCK_SIZE * blockNum
            });
        }
    }

    m_player = FindGO<Player>(igo::CLASS_NAME_PLAYER);

    return true;
}

void Stage::StageCreate()
{
    //�΂͘A���łU�܂�
    //�̌�͗�
    //���F�̌�͗�
    //��:70% ���F:30%

    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<int> dist(1, 100);

    int continuousGreenBlock = 0;   //�΂̃u���b�N������A���ŏo�Ă��邩�B
    int randomGreenNum = 0;              //����������ϐ�
    int randomBlueOrYellowNum = 0;  //�F�����F���Z�b�g����Ƃ��Ɏg�p���闐��������ϐ�
    bool lastTimeBlockBlueOrYellow = false; //�O��̃u���b�N���F�����F��������

    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        randomGreenNum = dist(mt); //�������Z�b�g

        //�ŏ��̃u���b�N�͗�
        if (blockNum == con::FIRST_OF_THE_ARRAY) {
            m_stageData[con::player_1][blockNum] = greenBlock;
            ++continuousGreenBlock;
            continue;
        }

        //�S�[���̈ʒu�̃u���b�N�͗�
        if (blockNum == m_MAX_BLOCK - 1) {
            m_stageData[con::player_1][blockNum] = greenBlock;
            continue;
        }

        //�΃u���b�N���U��A���ŏo�Ă����ꍇ
        if (continuousGreenBlock >= 6) {
            //�u���b�N�A���F�u���b�N�̃Z�b�g
            randomBlueOrYellowNum = dist(mt);
            CreateBlueOrYellow(blockNum, randomBlueOrYellowNum);
            continuousGreenBlock = 0;
            lastTimeBlockBlueOrYellow = true;
            continue;
        }

        //�O�񂪐F�����F�̃u���b�N�������ꍇ
        if (lastTimeBlockBlueOrYellow == true) {
            m_stageData[con::player_1][blockNum] = greenBlock;
            ++continuousGreenBlock;
            lastTimeBlockBlueOrYellow = false;
            continue;
        }

        //�΃u���b�N�̊m��
        if (CreateGreen(blockNum, randomGreenNum, continuousGreenBlock) == true) {
            m_stageData[con::player_1][blockNum] = greenBlock;
            ++continuousGreenBlock;
            continue;
        }
        else {
            //�u���b�N�A���F�u���b�N�̃Z�b�g
            randomBlueOrYellowNum = dist(mt);
            CreateBlueOrYellow(blockNum, randomBlueOrYellowNum);
            continuousGreenBlock = 0;
            lastTimeBlockBlueOrYellow = true;
            continue;
        }





 


    }

    //�v���C���[�P�ŃZ�b�g�������̂𑼂̃v���C���[�̂Ƃ���ɂ��Z�b�g����B
    for (int playerNum = con::player_2; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
            m_stageData[playerNum][blockNum] = m_stageData[con::player_1][blockNum];
        }
    }






    //old
    
    //for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
    //    m_stageData[con::player_1][blockNum] = greenBlock;
    //}

    //m_stageData[con::player_1][6] = blueBlock;
    //m_stageData[con::player_1][8] = blueBlock;
    //m_stageData[con::player_1][12] = yellowBlock;
    //m_stageData[con::player_1][15] = blueBlock;
    //m_stageData[con::player_1][18] = blueBlock;
    //m_stageData[con::player_1][25] = blueBlock;
    //m_stageData[con::player_1][28] = yellowBlock;
    //m_stageData[con::player_1][30] = blueBlock;
    //m_stageData[con::player_1][32] = blueBlock;
    //m_stageData[con::player_1][37] = yellowBlock;
    //m_stageData[con::player_1][40] = blueBlock;
    //m_stageData[con::player_1][43] = blueBlock;
    //m_stageData[con::player_1][46] = blueBlock;

    //for (int playerNum = con::player_2; playerNum < con::PlayerNumberMax; playerNum++) {
    //    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
    //        m_stageData[playerNum][blockNum] = m_stageData[con::player_1][blockNum];
    //    }
    //}
}

bool Stage::CreateGreen(const int blockNum, const int randomGreenNum, const int continuousGreenBlock)
{
    bool createGreenBlock = false;

    switch (continuousGreenBlock) {
    case 0:
        if (randomGreenNum <= 80) {
            createGreenBlock = true;
        }
        else {
            createGreenBlock = false;
        }

        break;
    case 1:
        if (randomGreenNum <= 70) {
            createGreenBlock = true;
        }
        else {
            createGreenBlock = false;
        }

        break;
        
    case 2:
        if (randomGreenNum <= 65) {
            createGreenBlock = true;
        }
        else {
            createGreenBlock = false;
        }

        break;
    case 3:
        if (randomGreenNum <= 60) {
            createGreenBlock = true;
        }
        else {
            createGreenBlock = false;
        }

        break;
    case 4:
        if (randomGreenNum <= 55) {
            createGreenBlock = true;
        }
        else {
            createGreenBlock = false;
        }

        break;
    case 5:
        if (randomGreenNum <= 50) {
            createGreenBlock = true;
        }
        else {
            createGreenBlock = false;
        }

        break;
    default:

        break;
    }

    return createGreenBlock;
}

void Stage::CreateBlueOrYellow(const int blockNum, const int randomBlueOrYellowNum)
{
    //�u���b�N�i70%�j
    if (randomBlueOrYellowNum <= 70) {
        m_stageData[con::player_1][blockNum] = blueBlock;
    }
    //���F�u���b�N�i30%�j
    else {
        m_stageData[con::player_1][blockNum] = yellowBlock;
    }
}

////////////////////////////////////////////////////////////
// ���t���[���̏���
////////////////////////////////////////////////////////////

void Stage::Update()
{
    //�X�^�[�g�n�_�ɖ߂�( Debug )
    if (g_pad[0]->IsTrigger(enButtonX)) {
        for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
            for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
                m_modelRender[playerNum][blockNum]->SetPosition({
                    BLOCK_START_POSITION[playerNum].x,
                    BLOCK_START_POSITION[playerNum].y,
                    BLOCK_START_POSITION[playerNum].z + BLOCK_SIZE * blockNum
                    });
            }
            m_playerBlockPosition[playerNum] = 0;
        }
    }
    //to this point( Debug )


    //�v���C���[���Ƃ̏���
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        CheckBlock(playerNum);

        ReturnOperationTimer(playerNum);
    }



    //�S�[�����̏���
    GoalBlock();
}

//////////////////////////////
// �u���b�N�̈ړ�����
//////////////////////////////

bool Stage::MoveBlock(const int pNum, const int moveNum)
{
    if (m_activeOperation[pNum] == false || m_activeOperationVersionBlue[pNum] == false) {
        return false;
    }

    //����s��Ԃɑ΂���ϐ����폜
    m_resistanceImpossibleOperation[pNum] = false;

    //�v���C���[�̌��݂̈ʒu��ۑ�
    m_playerBeforeBlockPosition[pNum] = m_playerBlockPosition[pNum];

    //�v���C���[�����ڂ̃u���b�N�ɂ��邩�̏����X�V
    m_playerBlockPosition[pNum] += moveNum;

    //�ő吔�𒴂��Ă�����A�ő吔 - 1�ɍ��킹��B
    if (m_playerBlockPosition[pNum] >= m_MAX_BLOCK) {
        m_playerBlockPosition[pNum] = m_MAX_BLOCK - 1;
    }

    //���f���̈ʒu���X�V
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        m_modelRender[pNum][blockNum]->SetPosition({
            BLOCK_POSITION_X[pNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - BLOCK_SIZE * (m_playerBlockPosition[pNum])
            //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
        });
    }

    return true;
}

//////////////////////////////
// �^�C�}�[
//////////////////////////////

void Stage::ReturnOperationTimer(const int pNum)
{
    if (m_activeOperation[pNum] == true) {
        return;
    }

    ++m_timerReturnOperation[pNum];

    //����\�ɂ���B
    if (m_timerReturnOperation[pNum] >= TIME_RETURN_OPERATION) {
        m_activeOperation[pNum] = true;
        m_timerReturnOperation[pNum] = TIMER_RESET;
        m_resistanceImpossibleOperation[pNum] = true;

        //���f���̉�]�����ɖ߂��B
        m_player->SetRotationX(pNum, con::FLOAT_ZERO);
    }
}

//////////////////////////////
// �u���b�N���Ƃ̏���
//////////////////////////////

void Stage::CheckBlock(const int pNum)
{
    //���L����������u���b�N�ɂ���ď����������Ȃ��B

    //�W�����v�A�j���[�V�������͏����������Ȃ�Ȃ��B
    if (m_player->GetmFlagAnimationJump(pNum) == true) {
        return;
    }

    //�u���b�N���Ƃɏ���
    if (m_stageData[pNum][m_playerBlockPosition[pNum]] == greenBlock) {

    }
    else if (m_stageData[pNum][m_playerBlockPosition[pNum]] == blueBlock) {
        BlueBlock(pNum);
    }
    else if (m_stageData[pNum][m_playerBlockPosition[pNum]] == yellowBlock) {
        if (m_resistanceImpossibleOperation[pNum] == false) {
            m_activeOperation[pNum] = false;
            //���F�u���b�N��΃u���b�N�ɕύX
            m_stageData[pNum][m_playerBlockPosition[pNum]] = greenBlock;
            m_modelRender[pNum][m_playerBlockPosition[pNum]]->Init(FILE_PATH_TKM_GREEN_BLOCK);
            //�����̕��@�Ń��f����ύX�����肩�́A�ʂ̕��@�ŕύX����悤�ɂ��邱�ƁB
            //���f������]������B
            m_player->SetRotationX(pNum, -90.0f);
        }
    }
}

//////////
// �F�̃u���b�N
//////////

void Stage::BlueBlock(const int pNum)
{
    if (m_flagAnimationBlueBlock[pNum] == false) {
        m_flagAnimationBlueBlock[pNum] = true;
        m_activeOperationVersionBlue[pNum] = false;
        //���f������]������B
        m_player->SetRotationX(pNum, -90.0f);
    }

    BlueBlockAnimation(pNum);

    ReturnBlock(pNum);
}

void Stage::BlueBlockAnimation(const int pNum)
{
    ++m_timerAnimationBlueBlock[pNum];

    //�O�̈ʒu�ɖ߂����߂Ƀt���O��false�ɂ���B
    if (m_timerAnimationBlueBlock[pNum] >= TIME_BLUE_BLOCK_ANIMATION) {
        m_flagAnimationBlueBlock[pNum] = false;
        m_timerAnimationBlueBlock[pNum] = TIMER_RESET;
        //�v���C���[�𑀍�ł���悤�ɂ���B
        m_activeOperationVersionBlue[pNum] = true;
        //���f�������ɖ߂��B
        m_player->SetRotationX(pNum, con::FLOAT_ZERO);
    }
}

void Stage::ReturnBlock(const int pNum)
{
    if (m_flagAnimationBlueBlock[pNum] == true) {
        return;
    }

    //�v���C���[�̌��݈ʒu��O�̈ʒu�ɖ߂��B
    m_playerBlockPosition[pNum] = m_playerBeforeBlockPosition[pNum];

    //���f���̈ʒu���X�V
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        m_modelRender[pNum][blockNum]->SetPosition({
            BLOCK_POSITION_X[pNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - BLOCK_SIZE * (m_playerBlockPosition[pNum])
        });
    }
}

//////////////////////////////
// �S�[�����̏���
//////////////////////////////

void Stage::GoalBlock()
{
    bool addNowRank = false; //�v���C���[�̏��ʂɑ�����鐔�����ς�邩�̃t���O
    int nextRank = con::INIT_ZERO; //���̃v���C���[�̏���

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        //�v���C���[�̏��ʂ��m��
        if (m_player->GetActivePlayer(playerNum) == true && m_playerBlockPosition[playerNum] == m_MAX_BLOCK - 1) {
            //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
            m_player->SetActivePlayer(playerNum, false);

            //���ʂ��m��
            m_player->SetGoalRanking(playerNum, m_nowRank);

            //�S�[��������Ԃɂ���B
            m_player->SetFlagGoal(playerNum, true);

            ++nextRank;
            addNowRank = true;
        }
    }

    if (addNowRank == true) {
        //���̏��ʂ�ݒ�
        m_nowRank += nextRank;
    }
}