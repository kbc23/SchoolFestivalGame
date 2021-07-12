#include "stdafx.h"
#include "stage.h"

#include <random>

#include "player.h"
#include "score.h"
#include "main_processing.h"
#include "rule1.h"
#include "EnemyAI.h"
#include "pause.h"


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

    const int MAX_PROBABILITY_BGM = 5;
    const int PROBABILITY_BGM[MAX_PROBABILITY_BGM] = {
        20,20,20,20,20
    };

    ////////////////////////////////////////////////////////////
    // �i�s�x�֘A
    ////////////////////////////////////////////////////////////

    const float MAX_DEGREE_OF_PROGRESS_POSITION = 835.0f;
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
        DeleteGO(m_spritePlayerMark[playerNum]);
    }

    DeleteGO(m_spriteBackgroundSky);
    DeleteGO(m_spriteBackgroundCloud_1);
    DeleteGO(m_spriteBackgroundCloud_2);

    DeleteGO(m_spriteDegreeOfProgress);

    if (m_bgm != nullptr) {
        DeleteGO(m_bgm);
    }
}

////////////////////////////////////////////////////////////
// Start�֐��֘A
////////////////////////////////////////////////////////////

bool Stage::Start()
{
    //�΂P�U��
    //�W��
    //���F�W��

    const char* kari1 = "green";
    const char* kari2 = "yellow";
    const char* kari3 = "goal";



    //���f���̍쐬
    m_modelGreenBlock[con::player_1][con::FIRST_ELEMENT_ARRAY] = NewGO<ModelRender>(igo::PRIORITY_MODEL, kari1);
    m_modelGreenBlock[con::player_1][con::FIRST_ELEMENT_ARRAY]->Init(filePath::tkm::GREEN_BLOCK);
    m_modelGreenBlock[con::player_1][con::FIRST_ELEMENT_ARRAY]->Deactivate();

    m_modelYellowBlock[con::player_1][con::FIRST_ELEMENT_ARRAY] = NewGO<ModelRender>(igo::PRIORITY_MODEL, kari2);
    m_modelYellowBlock[con::player_1][con::FIRST_ELEMENT_ARRAY]->Init(filePath::tkm::YELLOW_BLOCK);
    m_modelYellowBlock[con::player_1][con::FIRST_ELEMENT_ARRAY]->Deactivate();

    m_modelGoalBlock[con::player_1][con::FIRST_ELEMENT_ARRAY] = NewGO<ModelRender>(igo::PRIORITY_MODEL, kari3);
    m_modelGoalBlock[con::player_1][con::FIRST_ELEMENT_ARRAY]->Init(filePath::tkm::GOAL_BLOCK);
    m_modelGoalBlock[con::player_1][con::FIRST_ELEMENT_ARRAY]->Deactivate();

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
            m_modelGreenBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelGreenBlock[playerNum][blockNum]->Init(filePath::tkm::GREEN_BLOCK);
            m_modelGreenBlock[playerNum][blockNum]->Deactivate();
        }

        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
            m_modelYellowBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelYellowBlock[playerNum][blockNum]->Init(filePath::tkm::YELLOW_BLOCK);
            m_modelYellowBlock[playerNum][blockNum]->Deactivate();
        }

        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
            m_modelGoalBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelGoalBlock[playerNum][blockNum]->Init(filePath::tkm::GOAL_BLOCK);
            m_modelGoalBlock[playerNum][blockNum]->Deactivate();
        }
    }


    //�w�i�̕`��
    m_spriteBackgroundSky = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundSky->Init(filePath::dds::BACKGROUND_SKY);
    m_spriteBackgroundSky->Deactivate();
    m_spriteBackgroundCloud_1 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_1->Init(filePath::dds::BACKGROUND_CLOUD);
    m_spriteBackgroundCloud_1->Deactivate();
    m_spriteBackgroundCloud_2 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_2->Init(filePath::dds::BACKGROUND_CLOUD);
    m_spriteBackgroundCloud_2->Deactivate();

    //�i�s�x��UI���쐬
    m_spriteDegreeOfProgress = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteDegreeOfProgress->Init(filePath::dds::DEGREE_OF_PROGRESS);
    m_spriteDegreeOfProgress->Deactivate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spritePlayerMark[playerNum]->Init(filePath::dds::PLAYER_MARK[playerNum]);
        m_spritePlayerMark[playerNum]->Deactivate();
    }

    //BGM�̍Đ�
    //m_bgm = NewGO<SoundBGM>(0);

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontPlayerBlockPosition[playerNum]->Init(L"", PLAYER_BLOCK_POSITION_FONT_POSITION[playerNum]);
        m_fontPlayerBlockPosition[playerNum]->Deactivate();
    }

    //���E���h��UI
    for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = 0; roundNum < 3; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
            m_spriteRoundWin[playerNum][roundNum]->Init(filePath::dds::ROUND_WIN[playerNum][roundNum]);
            m_spriteRoundWin[playerNum][roundNum]->Deactivate();
        }
    }
    

    m_player = FindGO<Player>(igo::CLASS_NAME_PLAYER);

    m_rule1 = FindGO<Rule1>(igo::CLASS_NAME_RULE1);

    m_game = FindGO<MainProcessing>(igo::CLASS_NAME_GAME);

    m_enemyAI = FindGO<EnemyAI>(igo::CLASS_NAME_ENEMYAI);

    m_pause = FindGO<Pause>(igo::CLASS_NAME_PAUSE);

    return true;
}

void Stage::Init()
{
    m_flagProcessing = true;



    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = m_START_BLOCK;
        m_playerBeforeBlockPosition[playerNum] = m_START_BLOCK;
        m_activeOperation[playerNum] = true;
        m_timerReturnOperation[playerNum] = 0;
        m_resistanceImpossibleOperation[playerNum] = false;
        m_flagAnimationBlueBlock[playerNum] = false;
        m_timerAnimationBlueBlock[playerNum] = 0;
        m_activeOperationVersionBlue[playerNum] = true;
        m_amountOfMovement[playerNum] = 0;
        m_flagAnimationJump[playerNum] = false;
        m_timerAnimation[playerNum] = 0;
    }

    m_nowRank = m_INIT_RANK;            //�v���C���[�̏��ʃf�[�^�ɓn���f�[�^

    Playermember = 0;

    m_maxPlayer = con::PlayerNumberMax;	//�v���C���[�̍ő吔
    n = 0;          //�S�[�������v���C���[�̐�
    m = 0;          //���̃��E���h�Ɉڂ�̂Ɉ�u�ōs���Ȃ����߂̑҂�����
    m_allMiss = false;     //�v���C���[�S�����~�X�����Ă��邩

    j = 0;          //��Ԑi��ł���l�̃u���b�N��
    t = 0;          //2�Ԗڂɐi��ł���l�̃u���b�N��



    //�X�e�[�W���쐬
    StageCreate();

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
            m_modelGreenBlock[playerNum][blockNum]->Deactivate();
        }
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
            m_modelYellowBlock[playerNum][blockNum]->Deactivate();
        }
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
            m_modelGoalBlock[playerNum][blockNum]->Deactivate();
        }
    }

    //���f���̕`��
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        DrawBlock(playerNum);
    }

    //�w�i�̕`��
    m_spriteBackgroundSky->Activate();
    m_spriteBackgroundCloud_1->Activate();
    m_spriteBackgroundCloud_2->Activate();
    m_spriteBackgroundCloud_1->SetPositionX(0.0f);
    m_spriteBackgroundCloud_2->SetPositionX(1280.0f);

    //�i�s�x��UI���쐬
    m_spriteDegreeOfProgress->Activate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->SetPositionX(0.0f);
        m_spritePlayerMark[playerNum]->Activate();
    }

    //BGM�̍Đ�
    InitBGM();
    m_bgm->Play(true);

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum]->SetText(m_playerBlockPosition[playerNum] + 1);
        m_fontPlayerBlockPosition[playerNum]->Activate();
    }

    //���E���h��UI
    for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = 0; roundNum < 3; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum]->Deactivate();
        }
    }

}

void Stage::Finish()
{
    m_flagProcessing = false;

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
            m_modelGreenBlock[playerNum][blockNum]->Deactivate();
        }
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
            m_modelYellowBlock[playerNum][blockNum]->Deactivate();
        }
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
            m_modelGoalBlock[playerNum][blockNum]->Deactivate();
        }
    }

    m_spriteDegreeOfProgress->Deactivate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->Deactivate();
        m_fontPlayerBlockPosition[playerNum]->Deactivate();
    }

    //���E���h��UI
    for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = 0; roundNum < 3; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum]->Deactivate();
        }
    }


    m_spriteBackgroundSky->Deactivate();
    m_spriteBackgroundCloud_1->Deactivate();
    m_spriteBackgroundCloud_2->Deactivate();

    m_bgm->Stop();
    DeleteGO(m_bgm);
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

    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        //�ŏ��̃u���b�N�͗�
        if (blockNum == con::FIRST_ELEMENT_ARRAY) {
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
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
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
            m_bgm = NewGO<SoundBGM>(0);
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
    if (m_flagProcessing == false) {
        return;
    }

    //�v���C���[���Ƃ̏���
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {

        CheckBlock(playerNum);


        if (stop == false) {    //���F�ɏ�������ɂ����Ƒ���s�\�ɂ��邩�ǂ���
            ReturnOperationTimer(playerNum);
        }

        DrawMoveBlock(playerNum);

        DrawFontPlayerBlockPosition(playerNum);

        //�i�s�x�̕`��
        DegreeOfProgress(playerNum);

        if (rule1NewGO == true) {
            if (m_activeOperationVersionBlue[playerNum] == false && m_player->GetModelIsActive(playerNum) == true) {
                SuddenDeathBlueBlockAnimation(playerNum);
            }
        }
    }

    //�w�i�̕`��
    DrawBackground();

    //���E���h�������̕`��
    DrawRoundWin();

    //�S�[�����̏���
    GoalBlock();

    Length();

    /*NextRound();*/

}

void Stage::DrawBlock(const int pNum)
{
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
        //m_modelBlueBlock[pNum][blockNum]->Deactivate();
        m_modelYellowBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
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
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
        //m_modelBlueBlock[pNum][blockNum]->Deactivate();
        m_modelYellowBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
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

void Stage::DrawBackground()
{
    m_spriteBackgroundCloud_1->SetPositionX(m_spriteBackgroundCloud_1->GetPosition().x - 1.0f);
    m_spriteBackgroundCloud_2->SetPositionX(m_spriteBackgroundCloud_2->GetPosition().x - 1.0f);

    if (m_spriteBackgroundCloud_1->GetPosition().x <= -1280.0f) {
        m_spriteBackgroundCloud_1->SetPositionX(0.0f);
        m_spriteBackgroundCloud_2->SetPositionX(1280.0f);
    }
}

void Stage::DrawRoundWin()
{
    if (rule1NewGO == false) {
        return;
    }

    //���E���h��UI
    for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = 0; roundNum < 3; roundNum++) {
            if(roundNum < m_player->GetRoundPoint(playerNum))
            m_spriteRoundWin[playerNum][roundNum]->Activate();
        }
    }
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
    m_blueMiss[pNum] = false;
   
   
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
        m_player->SetAnimationFall(pNum);
    }

    if (stop == false) {
    BlueBlockAnimation(pNum);

    ReturnBlock(pNum);
    m_blueMiss [pNum]= true;
    m_player->SetBlueMiss(pNum, m_blueMiss);
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

void Stage::SuddenDeathBlueBlockAnimation(const int& pNum)
{
    ++m_timerAnimationBlueBlock[pNum];
    m_player->DownPositionY(pNum, 30.0f);

    //�v���C���[���\���ɂ���B
    if (m_timerAnimationBlueBlock[pNum] >= TIME_BLUE_BLOCK_ANIMATION) {
        m_player->PlayerModelDeactivate(pNum);
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
    //���ݒu
    if (m_player->GetFinishSuddenDeath() == true) {
        return;
    }

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

            if (rule1NewGO == true) {
                n += 1;
            }
        }
    }

    if (addNowRank == true) {
        //���̏��ʂ�ݒ�
        m_nowRank += nextRank;
    }
    
    if (rule1NewGO == true) {
        if (1 <= n) {
            //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                m_player->SetActivePlayer(playerNum, false);
            }
            ++m;
            if (m == 120) {
                NextRound();
                n = 0;
                m = 0;
            }
        }
        //���̃v���C���[���S���~�X�����Ă���Ƃ�
        else {
            int count = 0;

            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                if (m_activeOperation[playerNum] == false || m_activeOperationVersionBlue[playerNum] == false) {
                    ++count;
                }
            }

            //�R�l�~�X�̂Ƃ�
            if (count == 3) {
                for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                    if (m_activeOperation[playerNum] == true && m_activeOperationVersionBlue[playerNum] == true) {
                        //���ʂ��m��
                        m_player->SetGoalRanking(playerNum, 1);
                        m_player->SetAnimationWin(playerNum);

                        if (rule1NewGO == true) {
                            n += 1;
                        }
                    }

                    m_player->SetActivePlayer(playerNum, false);
                    //�S�[��������Ԃɂ���B
                    m_player->SetFlagGoal(playerNum, true);
                }
            }

            //�S�l�~�X�̂Ƃ�
            if (count == 4) {
                //���̃��E���h�ɂ���
                if (rule1NewGO == true) {
                    n += 1;
                    m_allMiss = true;
                }
            }
        }
    }
}

//////////////////////////////
// ���E���h�ύX�̏���
//////////////////////////////

void Stage::NextRound()
{  

    if (m_allMiss == false) {
        //�v���C���[�̃��E���h�����|�C���g
        for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
            if (m_player->GetGoalRanking(playerNum) == 1) {
                m_player->AddRoundPoint(playerNum);
            }
        }
    }
    
    if (m_player->GetFinishSuddenDeath() == true) {
        return;
    }



    //�X�e�[�W���쐬
    StageCreate();

    //�΂P�U��
    //�W��
    //���F�W��

    //���f���̍쐬
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
            //m_modelGreenBlock[playerNum][blockNum]->Init(FILE_PATH_TKM_GREEN_BLOCK);
            m_modelGreenBlock[playerNum][blockNum]->Deactivate();
        }

        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
            //m_modelBlueBlock[playerNum][blockNum]->Init(FILE_PATH_TKM_BLUE_BLOCK);
            //m_modelBlueBlock[playerNum][blockNum]->Deactivate();
            //m_modelYellowBlock[playerNum][blockNum]->Init(FILE_PATH_TKM_YELLOW_BLOCK);
            m_modelYellowBlock[playerNum][blockNum]->Deactivate();
        }
    }

    //���f���̕`��
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = 0;
        DrawBlock(playerNum);
    }

    //if (m_bgm->IsPlaying()) {
    //    m_bgm->Stop();
    //}

    //BGM�̍Đ�
    //m_bgm->Play(true);

    /*for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_player->SetFlagGoal(playerNum, false);
    }*/

    //�ϐ��̒l�̃��Z�b�g
    for (int i = 0; i < con::PlayerNumberMax; i++) {
        m_playerBlockPosition[i] = m_START_BLOCK;
        m_playerBeforeBlockPosition[i] = m_START_BLOCK;
        //m_stageData[i][m_MAX_BLOCK] = greenBlock; 
        m_activeOperation[i] = true;  
        m_timerReturnOperation[i] = 0;          
        m_resistanceImpossibleOperation[i] = false;
        m_flagAnimationBlueBlock[i] = false;
        m_timerAnimationBlueBlock[i] = 0;
        m_activeOperationVersionBlue[i] = true;
        m_amountOfMovement[i] = 0;
        m_flagAnimationJump[i] = false;
        m_timerAnimation[i] = 0;

        m_enemyAI->SetmissInvalidCount(i, 0);
        //m_maxPlayer = i;	
    }

    //stop = false;  
    //rule1NewGO = false;

    m_nowRank = m_INIT_RANK;

    Playermember = 0;

    
    n = 0;
    m = 0;
    j = 0;
    t = 0;

    m_allMiss = false;

    //�ʂ̃N���X��NextRound()���Ăяo���B
    m_score = FindGO<Score>(igo::CLASS_NAME_SCORE);
    m_game->NextRound();
    m_score->NextRound();
    m_player->NextRound();
 
}

void Stage::Length()
{
    if (rule1NewGO == false) {
        return;
    }

    if (1 <= n) {
        return;
    }

    if (m_playerBlockPosition[con::player_1] > m_playerBlockPosition[con::player_2] &&
        m_playerBlockPosition[con::player_1] > m_playerBlockPosition[con::player_3] &&
        m_playerBlockPosition[con::player_1] > m_playerBlockPosition[con::player_4])
    {
        //player_1�̔���
        if ((m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_2] >= 20 ||
            m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_2] <= -20) &&
            (m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_3] >= 20 ||
                m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_3] <= -20) &&
            (m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_4] >= 20 ||
                m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_4] <= -20))
        {
            //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                m_player->SetActivePlayer(playerNum, false);
                //�S�[��������Ԃɂ���B
                m_player->SetFlagGoal(playerNum, true);
            }

            //���ʂ��m��
            m_player->SetGoalRanking(con::player_1, 1);
            m_player->SetAnimationWin(con::player_1);

            if (rule1NewGO == true) {
                n += 1;
            }
        }
    }
    else if (m_playerBlockPosition[con::player_2] > m_playerBlockPosition[con::player_1] &&
                m_playerBlockPosition[con::player_2] > m_playerBlockPosition[con::player_3] &&
                m_playerBlockPosition[con::player_2] > m_playerBlockPosition[con::player_4])
    {
        //player_2�̔���
        if ((m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_1] >= 20 ||
            m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_1] <= -20) &&
            (m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_3] >= 20 ||
                m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_3] <= -20) &&
            (m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_4] >= 20 ||
                m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_4] <= -20))
        {
            //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                m_player->SetActivePlayer(playerNum, false);
                //�S�[��������Ԃɂ���B
                m_player->SetFlagGoal(playerNum, true);
            }

            //���ʂ��m��
            m_player->SetGoalRanking(con::player_2, 1);
            m_player->SetAnimationWin(con::player_2);

            if (rule1NewGO == true) {
                n += 1;
            }
        }
    }
    else if (m_playerBlockPosition[con::player_3] > m_playerBlockPosition[con::player_1] &&
                m_playerBlockPosition[con::player_3] > m_playerBlockPosition[con::player_2] &&
                m_playerBlockPosition[con::player_3] > m_playerBlockPosition[con::player_4])
    {
        //player_3�̔���
        if ((m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_1] >= 20 ||
            m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_1] <= -20) &&
            (m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_2] >= 20 ||
                m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_2] <= -20) &&
            (m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_4] >= 20 ||
                m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_4] <= -20))
        {
            //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                m_player->SetActivePlayer(playerNum, false);
                //�S�[��������Ԃɂ���B
                m_player->SetFlagGoal(playerNum, true);
            }

            //���ʂ��m��
            m_player->SetGoalRanking(con::player_3, 1);
            m_player->SetAnimationWin(con::player_3);

            if (rule1NewGO == true) {
                n += 1;
            }
        }
    }
    else if (m_playerBlockPosition[con::player_4] > m_playerBlockPosition[con::player_1] &&
                m_playerBlockPosition[con::player_4] > m_playerBlockPosition[con::player_2] &&
                m_playerBlockPosition[con::player_4] > m_playerBlockPosition[con::player_3])
    {
        //player_4�̔���
        if ((m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_1] >= 20 ||
            m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_1] <= -20) &&
            (m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_2] >= 20 ||
                m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_2] <= -20) &&
            (m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_3] >= 20 ||
                m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_3] <= -20))
        {
            //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                m_player->SetActivePlayer(playerNum, false);
                //�S�[��������Ԃɂ���B
                m_player->SetFlagGoal(playerNum, true);
            }

            //���ʂ��m��
            m_player->SetGoalRanking(con::player_4, 1);
            m_player->SetAnimationWin(con::player_4);

            if (rule1NewGO == true) {
                n += 1;
            }
        }
    }
}

//////////////////////////////
// �i�s�x
//////////////////////////////

void Stage::DegreeOfProgress(const int& pNum)
{
    float test2 = m_playerBlockPosition[pNum];

    test2 = test2 / m_MAX_BLOCK;

    m_spritePlayerMark[pNum]->SetPositionX(
        MAX_DEGREE_OF_PROGRESS_POSITION * test2
    );
}