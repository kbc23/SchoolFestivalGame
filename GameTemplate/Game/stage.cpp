#include "stdafx.h"
#include "stage.h"

#include <random>

#include "player.h"



//���f���̓ǂݍ��݂Ŏ��Ԃ��������Ă���̂ŁA
//�ǂݍ��ޕK�v�����郂�f���̐������炷���Ƃ��ł��Ȃ������؂��Ă݂邱�ƁB
//�����݂́A�v���g�Ȃ̂ł��̂܂܂ő��s

//����
//�o�i�i�ł������Ƃ��ɁA�o�i�i����Ԃ悤�ɂ������������Ƃ̂��Ɓi�d�v�x�F���j



namespace //constant
{
    //���o�i�i�ƃo�i�i������Ă�u���b�N�͂P�̃��f���łł��Ă���B

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

    const Vector2 PLAYER_BLOCK_POSITION_FONT_POSITION[con::PlayerNumberMax] = {	//�X�R�A�^�C���̕\���ʒu
    { -520.0f, -215.0f },										        //�v���C���[�P
    { -200.0f, -215.0f },												//�v���C���[�Q
    { 120.0f, -215.0f },												//�v���C���[�R
    { 440.0f, -215.0f }													//�v���C���[�S
    };

    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    const int TIMER_RESET = 0;                  //�^�C�}�[�̃��Z�b�g
    const int TIME_RETURN_OPERATION = 30;       //���앜�A�ɂ����鎞�ԁi1.5�b�j

    const int TIME_BLUE_BLOCK_ANIMATION = 30;   //�F�̃u���b�N�ɍs�����Ƃ��̃A�j���[�V�����̎���

    ////////////////////////////////////////////////////////////
    // ���͊֘A
    ////////////////////////////////////////////////////////////

    const int MOVE_BUTTON_A = 2;		//A�{�^�����������Ƃ��̈ړ���
    const int MOVE_BUTTON_B = 1;		//B�{�^�����������Ƃ��̈ړ���

    ////////////////////////////////////////////////////////////
    // �m���֘A
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // �u���b�N�̕��т̐���
    //////////////////////////////

    const int UPPER_LIMIT_CONTINUOUS_GREEN_BLOCK = 6;   //�A���ō쐬����΃u���b�N�̏��
    const int PROBABILITY_CREATE_GREEN_BLOCK[UPPER_LIMIT_CONTINUOUS_GREEN_BLOCK] = {    //�΃u���b�N���쐬���邩�̊m��
        80,                                                                                 //�O��
        70,                                                                                 //�P��
        65,                                                                                 //�Q��
        60,                                                                                 //�R��
        55,                                                                                 //�S��
        50                                                                                  //�T��
    };
    const int PROBABILITY_CREATE_BLUE_BLOCK = 70;       //�u���b�N���쐬���邩�̊m��
    //( 100 - PROBABILITY_CREATE_BLUE_BLOCK )�����F�u���b�N���쐬���邩�̊m��

    const int MINIMUM_RANDOM_NUMBER = 1;  
    const int MAXIMUM_RANDOM_NUMBER = 100;

    //////////////////////////////
    // BGM�̊m��
    //////////////////////////////

    const int MAX_PROBABILITY_BGM = 6;
    const int PROBABILITY_BGM[MAX_PROBABILITY_BGM] = {
        19,19,19,19,19,5
    };
}



Stage::Stage()
{

}

Stage::~Stage()
{
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        //for (int blockNum = 0; blockNum < m_MAX_BLOCK; blockNum++) {
        //    DeleteGO(m_modelRender[playerNum][blockNum]);
        //}
        for (int blockNum = 0; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
            DeleteGO(m_modelGreenBlock[playerNum][blockNum]);
        }
        for (int blockNum = 0; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
            DeleteGO(m_modelYellowBlock[playerNum][blockNum]);
        }
        for (int blockNum = 0; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
            DeleteGO(m_modelGoalBlock[playerNum][blockNum]);
        }
        DeleteGO(m_fontPlayerBlockPosition[playerNum]);
    }

    DeleteGO(m_spriteBackground);

    DeleteGO(m_bgm);
    DeleteGO(m_testEffect);
}

////////////////////////////////////////////////////////////
// Start�֐��֘A
////////////////////////////////////////////////////////////

bool Stage::Start()
{
    m_spriteBackground = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackground->Init(filePath::dds::BACKGROUND_SKY);

    //�X�e�[�W���쐬
    StageCreate();

    //�΂P�U��
    //�W��
    //���F�W��

    //���f���̍쐬
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
            m_modelGreenBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelGreenBlock[playerNum][blockNum]->Init(filePath::tkm::GREEN_BLOCK);
            m_modelGreenBlock[playerNum][blockNum]->Deactivate();
        }

        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
            //m_modelBlueBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_FIRST);
            //m_modelBlueBlock[playerNum][blockNum]->Init(FILE_PATH_TKM_BLUE_BLOCK);
            //m_modelBlueBlock[playerNum][blockNum]->Deactivate();
            m_modelYellowBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelYellowBlock[playerNum][blockNum]->Init(filePath::tkm::YELLOW_BLOCK);
            m_modelYellowBlock[playerNum][blockNum]->Deactivate();
        }

        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
            m_modelGoalBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelGoalBlock[playerNum][blockNum]->Init(filePath::tkm::GOAL_BLOCK);
            m_modelGoalBlock[playerNum][blockNum]->Deactivate();
        }
    }

    //���f���̕`��
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        DrawBlock(playerNum);
    }

    //BGM�̍Đ�
    m_bgm = NewGO<SoundBGM>(0);
    InitBGM();
    m_bgm->Play(true);

    //�G�t�F�N�g�̍Đ�
    m_testEffect = NewGO<EffectRender>(0);
    m_testEffect->Init(u"Assets/Effect/goal.efk");
    m_testEffect->SetScale({ 20.0f,20.0f,20.0f });
    m_testEffect->Play();

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontPlayerBlockPosition[playerNum]->Init(L"", PLAYER_BLOCK_POSITION_FONT_POSITION[playerNum]);
        m_fontPlayerBlockPosition[playerNum]->SetText(m_playerBlockPosition[playerNum] + 1);
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
    std::uniform_int_distribution<int> randomNum(MINIMUM_RANDOM_NUMBER, MAXIMUM_RANDOM_NUMBER);

    int continuousGreenBlock = 0;   //�΂̃u���b�N������A���ŏo�Ă��邩�B
    bool lastTimeBlockBlueOrYellow = false; //�O��̃u���b�N���F�����F��������

    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        //�ŏ��̃u���b�N�͗�
        if (blockNum == con::FIRST_OF_THE_ARRAY) {
            m_stageData[con::player_1][blockNum] = greenBlock;
            ++continuousGreenBlock;
            continue;
        }

        //�S�[���̈ʒu�̃u���b�N�͗�
        if (blockNum == m_MAX_BLOCK - 1) {
            m_stageData[con::player_1][blockNum] = goalBlock;
            continue;
        }

        //�΃u���b�N���U��A���ō쐬����Ă���ꍇ
        if (continuousGreenBlock >= UPPER_LIMIT_CONTINUOUS_GREEN_BLOCK) {
            //�u���b�N�A���F�u���b�N�̃Z�b�g
            CreateBlueOrYellow(blockNum, randomNum(mt));
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

        //�΃u���b�N���쐬���邩
        if (CreateGreen(continuousGreenBlock, randomNum(mt)) == true) {
            //�΃u���b�N���쐬
            m_stageData[con::player_1][blockNum] = greenBlock;
            ++continuousGreenBlock;
            continue;
        }
        else {
            //�u���b�N�A���F�u���b�N���쐬
            CreateBlueOrYellow(blockNum, randomNum(mt));
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
}

bool Stage::CreateGreen(const int continuousGreenBlock, const int randomGreenNum)
{
    //�΃u���b�N���쐬���邩�ǂ���
    if (randomGreenNum <= PROBABILITY_CREATE_GREEN_BLOCK[continuousGreenBlock]) {
        //�΃u���b�N���쐬
        return true;
    }
    else {
        //�΃u���b�N���쐬���Ȃ�
        return false;
    }
}

void Stage::CreateBlueOrYellow(const int blockNum, const int randomBlueOrYellowNum)
{
    //�u���b�N�Ɖ��F�u���b�N�̂ǂ�����쐬���邩
    if (randomBlueOrYellowNum <= PROBABILITY_CREATE_BLUE_BLOCK) {
        //�u���b�N���쐬
        m_stageData[con::player_1][blockNum] = blueBlock;
    }
    else {
        //���F�u���b�N���쐬
        m_stageData[con::player_1][blockNum] = yellowBlock;
    }
}

void Stage::InitBGM()
{
    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<int> random(MINIMUM_RANDOM_NUMBER, MAXIMUM_RANDOM_NUMBER);

    int randomNum = random(mt);
    int checkRandom = 0;

    for (int check = 0; check < MAX_PROBABILITY_BGM; check++) {
        checkRandom += PROBABILITY_BGM[check];

        if (randomNum <= checkRandom) {
            m_bgm->Init(filePath::bgm::STAGE[check]);
            return;
        }
    }
}

////////////////////////////////////////////////////////////
// ���t���[���̏���
////////////////////////////////////////////////////////////

void Stage::Update()
{
    //�v���C���[���Ƃ̏���
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        CheckBlock(playerNum);

        if (stop == false) {    //���F�ɏ�������ɂ����Ƒ���s�\�ɂ��邩�ǂ���
            ReturnOperationTimer(playerNum);
        }

        DrawMoveBlock(playerNum);

        DrawFontPlayerBlockPosition(playerNum);
    }

    //�S�[�����̏���
    GoalBlock();
}

void Stage::DrawBlock(const int pNum)
{
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
        //m_modelBlueBlock[pNum][blockNum]->Deactivate();
        m_modelYellowBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
        m_modelGoalBlock[pNum][blockNum]->Deactivate();
    }

    //���f���̕`��
    int numberOfUsesGreenBlock = 0;
    int numberOfUsesBlueBlock = 0;
    int numberOfUsesYellowBlock = 0;
    const int numberOfUsesGoalBlock = 0;

    for (int blockNum = -6; blockNum < 15; blockNum++) {
        //�X�^�[�g�n�_���O�̃u���b�N��`�悵�Ȃ�
        if (m_playerBlockPosition[pNum] + blockNum < 0) {
            continue;
        }
        //�S�[���n�_����̃u���b�N��`�悵�Ȃ�
        if (m_playerBlockPosition[pNum] + blockNum >= m_MAX_BLOCK) {
            break;
        }

        if (m_stageData[pNum][m_playerBlockPosition[pNum] + blockNum] == greenBlock) {
            m_modelGreenBlock[pNum][numberOfUsesGreenBlock]->SetPosition({
            BLOCK_POSITION_X[pNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelGreenBlock[pNum][numberOfUsesGreenBlock]->Activate();
            ++numberOfUsesGreenBlock;
        }
        else if (m_stageData[pNum][m_playerBlockPosition[pNum] + blockNum] == blueBlock) {
            /*m_modelBlueBlock[pNum][numberOfUsesBlueBlock]->SetPosition({
              BLOCK_POSITION_X[pNum],
              BLOCK_POSITION_Y,
              BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelBlueBlock[pNum][numberOfUsesBlueBlock]->Activate();*/
            ++numberOfUsesBlueBlock;
        }
        else if (m_stageData[pNum][m_playerBlockPosition[pNum] + blockNum] == yellowBlock) {
            m_modelYellowBlock[pNum][numberOfUsesYellowBlock]->SetPosition({
               BLOCK_POSITION_X[pNum],
               BLOCK_POSITION_Y,
               BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelYellowBlock[pNum][numberOfUsesYellowBlock]->Activate();
            ++numberOfUsesYellowBlock;
        }
        else if (m_stageData[pNum][m_playerBlockPosition[pNum] + blockNum] == goalBlock) {
            m_modelGoalBlock[pNum][numberOfUsesGoalBlock]->SetPosition({
               BLOCK_POSITION_X[pNum],
               BLOCK_POSITION_Y,
               BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelGoalBlock[pNum][numberOfUsesGoalBlock]->Activate();
        }
    }

}

void Stage::DrawMoveBlock(const int pNum)
{
    if (m_flagAnimationJump[pNum] == false) {
        return;
    }

    ++m_timerAnimation[pNum];

    //���f����S����\���ɂ���B
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
        //m_modelBlueBlock[pNum][blockNum]->Deactivate();
        m_modelYellowBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
        m_modelGoalBlock[pNum][blockNum]->Deactivate();
    }


    //���f���̕`��
    int numberOfUsesGreenBlock = 0;
    int numberOfUsesBlueBlock = 0;
    int numberOfUsesYellowBlock = 0;
    const int numberOfUsesGoalBlock = 0;

    double moveCorrection = 0;

    int playerBlockPosition = m_playerBlockPosition[pNum] - m_amountOfMovement[pNum];



    if (m_timerAnimation[pNum] < 30) {
        double test = BLOCK_SIZE * m_amountOfMovement[pNum] / 30;

        moveCorrection = test * m_timerAnimation[pNum];


        for (int blockNum = -6; blockNum < 15; blockNum++) {
            //�X�^�[�g�n�_���O�̃u���b�N��`�悵�Ȃ�
            if (playerBlockPosition + blockNum < 0) {
                continue;
            }
            //�S�[���n�_����̃u���b�N��`�悵�Ȃ�
            if (playerBlockPosition + blockNum >= m_MAX_BLOCK) {
                break;
            }

            if (m_stageData[pNum][playerBlockPosition + blockNum] == greenBlock) {
                m_modelGreenBlock[pNum][numberOfUsesGreenBlock]->SetPosition({
                BLOCK_POSITION_X[pNum],
                BLOCK_POSITION_Y,
                BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - float(moveCorrection)
                    });
                m_modelGreenBlock[pNum][numberOfUsesGreenBlock]->Activate();
                ++numberOfUsesGreenBlock;
            }
            else if (m_stageData[pNum][playerBlockPosition + blockNum] == blueBlock) {
                /*m_modelBlueBlock[pNum][numberOfUsesBlueBlock]->SetPosition({
                  BLOCK_POSITION_X[pNum],
                  BLOCK_POSITION_Y,
                  BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - float(moveCorrection)
                    });
                m_modelBlueBlock[pNum][numberOfUsesBlueBlock]->Activate();*/
                ++numberOfUsesBlueBlock;
            }
            else if (m_stageData[pNum][playerBlockPosition + blockNum] == yellowBlock) {
                m_modelYellowBlock[pNum][numberOfUsesYellowBlock]->SetPosition({
                   BLOCK_POSITION_X[pNum],
                   BLOCK_POSITION_Y,
                   BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - float(moveCorrection)
                    });
                m_modelYellowBlock[pNum][numberOfUsesYellowBlock]->Activate();
                ++numberOfUsesYellowBlock;
            }
            else if (m_stageData[pNum][playerBlockPosition + blockNum] == goalBlock) {
                m_modelGoalBlock[pNum][numberOfUsesGoalBlock]->SetPosition({
                   BLOCK_POSITION_X[pNum],
                   BLOCK_POSITION_Y,
                   BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - float(moveCorrection)
                    });
                m_modelGoalBlock[pNum][numberOfUsesGoalBlock]->Activate();
            }
        }
    }
    else {
        DrawBlock(pNum);
        m_timerAnimation[pNum] = 0;
        m_flagAnimationJump[pNum] = false;
    }
}

void Stage::DrawFontPlayerBlockPosition(const int pNum)
{
    m_fontPlayerBlockPosition[pNum]->SetText(m_playerBlockPosition[pNum] + 1);
}

//////////////////////////////
// �u���b�N�̈ړ�����
//////////////////////////////

bool Stage::MoveBlock(const int pNum, const int moveNum)
{
    if (m_activeOperation[pNum] == false || m_activeOperationVersionBlue[pNum] == false) {
        return false;
    }

    //�ړ��ʂ�ۑ�
    m_amountOfMovement[pNum] = moveNum;

    //����s��Ԃɑ΂���ϐ����폜
    m_resistanceImpossibleOperation[pNum] = false;

    //�v���C���[�̌��݂̈ʒu��ۑ�
    m_playerBeforeBlockPosition[pNum] = m_playerBlockPosition[pNum];

    //�v���C���[�����ڂ̃u���b�N�ɂ��邩�̏����X�V
    m_playerBlockPosition[pNum] += moveNum;

    //�ő吔�𒴂��Ă�����A�ő吔 - 1�ɍ��킹��B
    if (m_playerBlockPosition[pNum] >= m_MAX_BLOCK) {
        m_playerBlockPosition[pNum] = m_MAX_BLOCK - 1;
        m_amountOfMovement[pNum] = MOVE_BUTTON_B;
    }

    //�u���b�N�̕`��
    m_flagAnimationJump[pNum] = true;

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
        m_player->SetAnimationIdle(pNum);
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
            DrawBlock(pNum);
            //�������A�j���[�V�������Đ�
            m_player->SetAnimationSrip(pNum);
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
        //�M��Ă���A�j���[�V�������Đ�
        m_player->SetAnimationDorwn(pNum);
    }

    if (stop == false) {
    BlueBlockAnimation(pNum);

    ReturnBlock(pNum);
    }
}

void Stage::BlueBlockAnimation(const int pNum)
{
    ++m_timerAnimationBlueBlock[pNum];
    m_player->DownPositionY(pNum, 30.0f);

    //�O�̈ʒu�ɖ߂����߂Ƀt���O��false�ɂ���B
    if (m_timerAnimationBlueBlock[pNum] >= TIME_BLUE_BLOCK_ANIMATION) {
        m_flagAnimationBlueBlock[pNum] = false;
        m_timerAnimationBlueBlock[pNum] = TIMER_RESET;
        //�v���C���[�𑀍�ł���悤�ɂ���B
        m_activeOperationVersionBlue[pNum] = true;
        //���f�������ɖ߂��B
        m_player->ResetPositionY(pNum);
        m_player->SetAnimationIdle(pNum);
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
    //���f���̕`��
    DrawBlock(pNum);
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