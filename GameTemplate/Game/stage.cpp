#include "stdafx.h"
#include "stage.h"

#include <random>

#include "game_data.h"
#include "player.h"
#include "score.h"
#include "main_processing.h"
#include "sudden_death_mode.h"
#include "CPU_player_controller.h"
#include "pause.h"
#include "sudden_death_mode.h"



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
    //////////////////////////////
    // NewGO
    //////////////////////////////

    //////////
    // ���f����NewGO
    //////////

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

    //////////
    // �X�v���C�g��NewGO
    //////////

    //�w�i��NewGO
    m_spriteBackgroundSky = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundSky->Init(filePath::dds::BACKGROUND_SKY);
    m_spriteBackgroundSky->Deactivate();
    m_spriteBackgroundCloud_1 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_1->Init(filePath::dds::BACKGROUND_CLOUD);
    m_spriteBackgroundCloud_1->Deactivate();
    m_spriteBackgroundCloud_2 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_2->Init(filePath::dds::BACKGROUND_CLOUD);
    m_spriteBackgroundCloud_2->Deactivate();

    //�i�s�x��UI��NewGO
    m_spriteDegreeOfProgress = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteDegreeOfProgress->Init(filePath::dds::DEGREE_OF_PROGRESS);
    m_spriteDegreeOfProgress->Deactivate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spritePlayerMark[playerNum]->Init(filePath::dds::PLAYER_MARK[playerNum]);
        m_spritePlayerMark[playerNum]->Deactivate();
    }

    //���E���h��UI��NewGO
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
            m_spriteRoundWin[playerNum][roundNum]->Init(filePath::dds::ROUND_WIN[playerNum][roundNum]);
            m_spriteRoundWin[playerNum][roundNum]->Deactivate();
        }
    }

    //////////
    // �t�H���g��NewGO
    //////////

    //�v���C���[�����݉��Ԗڂ̃u���b�N�ɂ��邩�̕\����NewGO
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontPlayerBlockPosition[playerNum]->Init(L"", PLAYER_BLOCK_POSITION_FONT_POSITION[playerNum]);
        m_fontPlayerBlockPosition[playerNum]->SetText(m_playerBlockPosition[playerNum] + 1);
        m_fontPlayerBlockPosition[playerNum]->Deactivate();
    }
}

Stage::~Stage()
{
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
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
    //////////////////////////////
    // FindGO
    //////////////////////////////

    m_findGameData = FindGO<GameData>(igo::CLASS_NAME_GAME_DATA);
    m_findPlayer = FindGO<Player>(igo::CLASS_NAME_PLAYER);
    m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);
    m_findMainProcessing = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);
    m_findCPUPlayerController = FindGO<CPUPlayerController>(igo::CLASS_NAME_CPU_PLAYER_CONTROLLER);
    m_findPause = FindGO<Pause>(igo::CLASS_NAME_PAUSE);
    m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);

    return true;
}

void Stage::Init()
{
    m_flagProcess = true;

    //////////
    // ���f���̏�����
    //////////

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

    //�X�e�[�W���쐬
    StageCreate();

    //���f���̕`��
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        DrawBlock(playerNum);
    }

    //////////
    // �X�v���C�g�̏�����
    //////////

    //�w�i�̏�����
    m_spriteBackgroundSky->Activate();
    m_spriteBackgroundCloud_1->Activate();
    m_spriteBackgroundCloud_1->SetPositionX(0.0f);
    m_spriteBackgroundCloud_2->Activate();
    m_spriteBackgroundCloud_2->SetPositionX(1280.0f);

    //�i�s�x��UI�̏�����
    m_spriteDegreeOfProgress->Activate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->Activate();
        m_spritePlayerMark[playerNum]->SetPositionX(0.0f);
    }

    //���E���h��UI�̏�����
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum]->Deactivate();
        }
    }

    //////////
    // �t�H���g�̏�����
    //////////

    //�v���C���[�����݉��Ԗڂ̃u���b�N�ɂ��邩�̕\���̏�����
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum]->Activate();
    }

    //////////
    // BGM��NewGO�A������
    //////////

    InitBGM();
    m_bgm->Play(true);

    //////////
    // �����o�ϐ��̏�����
    //////////

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = m_START_BLOCK; //�v���C���[�����ڂ̃u���b�N�ɂ��邩
        m_playerBeforeBlockPosition[playerNum] = m_START_BLOCK; //�v���C���[�̑O�ɂ����u���b�N�̔ԍ�
        m_amountOfMovement[playerNum] = 0;
        m_blueMiss[playerNum] = false;
        m_playerAnimation[playerNum] = greenBlock;
        m_flagAnimationJump[playerNum] = false;//�W�����v�A�j���[�V��������
        m_timerAnimation[playerNum] = 0; //�A�j���[�V�����̃^�C�}�[
    }

    m_nowRank = m_INIT_RANK; //�v���C���[�̏��ʃf�[�^�ɓn���f�[�^
    m_maxPlayer = con::PlayerNumberMax;	//�v���C���[�̍ő吔
    m_goalPlayer = 0; //�S�[�������v���C���[�̐�
    m_nextTime = 0; //���̃��E���h�Ɉڂ�̂Ɉ�u�ōs���Ȃ����߂̑҂�����
    m_allMiss = false; //�v���C���[�S�����~�X�����Ă��邩
}

void Stage::Finish()
{
    m_flagProcess = false;

    //////////
    // ���f���̔�\��
    //////////

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

    //////////
    // �X�v���C�g�̔�\��
    //////////

    //�w�i�̔�\��
    m_spriteBackgroundSky->Deactivate();
    m_spriteBackgroundCloud_1->Deactivate();
    m_spriteBackgroundCloud_2->Deactivate();

    //�i�s�x��UI�̔�\��
    m_spriteDegreeOfProgress->Deactivate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->Deactivate();
    }

    //���E���h��UI�̔�\��
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum]->Deactivate();
        }
    }

    //////////
    // �t�H���g�̔�\��
    //////////

    //�v���C���[�����݉��Ԗڂ̃u���b�N�ɂ��邩�̕\���̔�\��
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum]->Deactivate();
    }

    //////////
    // BGM��DeleteGO
    //////////

    DeleteGO(m_bgm);
}

void Stage::StageCreate()
{
    //�΂͘A���łU�܂�
    //�̌�͗�
    //���F�̌�͗�
    //��:70% ���F:30%

    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<int> randomNum(con::MINIMUM_RANDOM_NUMBER, con::MAXIMUM_RANDOM_NUMBER);

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
    std::uniform_int_distribution<int> random(con::MINIMUM_RANDOM_NUMBER, con::MAXIMUM_RANDOM_NUMBER);

    int randomNum = random(mt);
    int checkRandom = 0;

    for (int check = con::FIRST_ELEMENT_ARRAY; check < MAX_PROBABILITY_BGM; check++) {
        checkRandom += PROBABILITY_BGM[check];

        if (randomNum <= checkRandom) {
            m_bgm = NewGO<SoundBGM>(igo::PRIORITY_CLASS);
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
    if (false == m_flagProcess) {
        return;
    }

    //�v���C���[���Ƃ̏���
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        DrawMoveBlock(playerNum);

        DrawFontPlayerBlockPosition(playerNum);

        //�i�s�x�̕`��
        DegreeOfProgress(playerNum);

        //�T�h���f�X���[�h���̃A�j���[�V����
        if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
            if (m_findPlayer->GetStopController(playerNum) == false &&
                m_findPlayer->GetModelIsActive(playerNum) == true) {
            }
        }
    }

    //�w�i�̕`��
    DrawBackground();

    //���E���h�������̕`��
    DrawRoundWin();

    //�S�[�����̏���
    GoalBlock();

    //20�}�X���̔���
    CheckPlayerDistance();
}

void Stage::DrawBlock(const int pNum)
{
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
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
        DrawBlock(pNum);

        return;
    }

    ++m_timerAnimation[pNum];

    //���f����S����\���ɂ���B
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
        m_modelYellowBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
        m_modelGoalBlock[pNum][blockNum]->Deactivate();
    }


    //���f���̕`��
    int numberOfUsesGreenBlock = 0; //�΃u���b�N�̕`�搔
    int numberOfUsesBlueBlock = 0; //���u���b�N�̕`�搔
    int numberOfUsesYellowBlock = 0; //�o�i�i�u���b�N�̕`�搔
    const int numberOfUsesGoalBlock = 0; //�S�[���u���b�N�̕`�搔

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
    if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == false) {
        return;
    }

    //���E���h��UI
    for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = 0; roundNum < 3; roundNum++) {
            if(roundNum < m_findPlayer->GetRoundPoint(playerNum))
            m_spriteRoundWin[playerNum][roundNum]->Activate();
        }
    }
}

//////////////////////////////
// �u���b�N�̈ړ�����
//////////////////////////////

bool Stage::MoveBlock(const int pNum, const int moveNum)
{
    if (m_findPlayer->GetStopController(pNum) == true) {
        //����s�\��
        return false;
    }

    //�ړ��ʂ�ۑ�
    m_amountOfMovement[pNum] = moveNum;

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
// �u���b�N���Ƃ̏���
//////////////////////////////

void Stage::CheckBlock(const int pNum)
{
    m_blueMiss[pNum] = false;
   
    //���L����������u���b�N�ɂ���ď����������Ȃ��B

    //�u���b�N���Ƃɏ���
    if (m_stageData[pNum][m_playerBlockPosition[pNum]] == greenBlock) {

    }
    else if (m_stageData[pNum][m_playerBlockPosition[pNum]] == blueBlock) {
        BlueBlock(pNum);
    }
    else if (m_stageData[pNum][m_playerBlockPosition[pNum]] == yellowBlock) {
        YellowBlock(pNum);
    }
}

//////////
// �F�̃u���b�N
//////////

void Stage::BlueBlock(const int& pNum)
{
    //����s�\�ɂ���
    m_findPlayer->SetStopController(pNum, true);

    //�A�j���[�V������ݒ�
    m_findPlayer->SetNowAnimationBlock(pNum, con::blueBlock);

    //CPU�ɐu���b�N�Ń~�X���������Ƃ�`����
    m_findCPUPlayerController->SetFlagBlueBlockMiss(pNum, true);
}

//////////
// ���F�̃u���b�N
//////////

void Stage::YellowBlock(const int& playerNum)
{
    //���F�u���b�N��΃u���b�N�ɕύX
    m_stageData[playerNum][m_playerBlockPosition[playerNum]] = greenBlock;
    DrawBlock(playerNum);

    //����s�\�ɂ���
    m_findPlayer->SetStopController(playerNum, true);

    //�A�j���[�V������ݒ�
    m_findPlayer->SetNowAnimationBlock(playerNum, con::yellowBlock);
}

//////////
// ���̑�
//////////

void Stage::ReturnBlock(const int pNum)
{
    //�v���C���[�̌��݈ʒu��O�̈ʒu�ɖ߂��B
    m_playerBlockPosition[pNum] = m_playerBeforeBlockPosition[pNum];

    //���f���̈ʒu���X�V
    DrawBlock(pNum);
}

//////////////////////////////
// �S�[�����̏���
//////////////////////////////

void Stage::GoalBlock()
{
    //���ݒu
    if (m_findPlayer->GetFinishSuddenDeath() == true) {
        return;
    }

    bool flagAddNowRank = false; //�v���C���[�̏��ʂɑ�����鐔�����ς�邩�̃t���O
    int nextRank = 0; //���̃v���C���[�̏���

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        //�v���C���[�̏��ʂ��m��
        if (m_findPlayer->GetActivePlayer(playerNum) == true && m_playerBlockPosition[playerNum] == m_MAX_BLOCK - 1) {
            //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
            m_findPlayer->SetActivePlayer(playerNum, false);

            //���ʂ��m��
            m_findPlayer->SetGoalRanking(playerNum, m_nowRank);

            //�S�[��������Ԃɂ���B
            m_findPlayer->SetFlagGoal(playerNum, true);

            ++nextRank;
            flagAddNowRank = true;

            if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
                m_goalPlayer += 1;
            }
        }
    }

    if (flagAddNowRank == true) {
        //���̏��ʂ�ݒ�
        m_nowRank += nextRank;
    }
    
    if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
        if (1 <= m_goalPlayer) {
            //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                m_findPlayer->SetActivePlayer(playerNum, false);
            }
            ++m_nextTime;
            if (m_nextTime == 120) {
                m_findSuddenDeathMode->NextRound();
                m_goalPlayer = 0;
                m_nextTime = 0;
            }
        }
        //���̃v���C���[���S���~�X�����Ă���Ƃ�
        else {
            int count = 0;

            //�~�X�������v���C���[���J�E���g
            for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
                if (m_findPlayer->GetStopController(playerNum) == true) {
                    ++count;
                }
            }

            //�R�l�~�X�̂Ƃ�
            if (count == 3) {
                for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
                    if (m_findPlayer->GetStopController(playerNum) == false) {
                        //���ʂ��m��
                        m_findPlayer->SetGoalRanking(playerNum, con::rank_1);
                        m_findPlayer->SetAnimationWin(playerNum);

                        //�S�[�������l���𑝉�
                        ++m_goalPlayer;
                    }

                    m_findPlayer->SetActivePlayer(playerNum, false);
                    //�S�[��������Ԃɂ���B
                    m_findPlayer->SetFlagGoal(playerNum, true);
                }
            }

            //�S�l�~�X�̂Ƃ�
            if (count == 4) {
                //���̃��E���h�ɂ���
                if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
                    //�S�[�������l���𑝉�
                    ++m_goalPlayer;
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
        for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
            if (m_findPlayer->GetGoalRanking(playerNum) == con::rank_1) {
                m_findPlayer->AddRoundPoint(playerNum);
            }
        }
    }
    
    if (m_findPlayer->GetFinishSuddenDeath() == true) {
        return;
    }



    //�X�e�[�W���쐬
    StageCreate();

    //�΂P�U��
    //�W��
    //���F�W��

    //���f���̔�\��
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
        m_playerBlockPosition[playerNum] = 0;
        DrawBlock(playerNum);
    }

    //�ϐ��̒l�̃��Z�b�g
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = m_START_BLOCK;
        m_playerBeforeBlockPosition[playerNum] = m_START_BLOCK;
        m_amountOfMovement[playerNum] = 0;
        m_flagAnimationJump[playerNum] = false;
        m_timerAnimation[playerNum] = 0;
    }

    m_nowRank = m_INIT_RANK;
    
    m_goalPlayer = 0;
    m_nextTime = 0;

    m_allMiss = false;
}

//////////////////////////////
// �����ɂ�鏟������
//////////////////////////////

void Stage::CheckPlayerDistance()
{
    if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == false) {
        return;
    }

    if (1 <= m_goalPlayer) {
        return;
    }

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        bool check = true;

        if (CheckPlayerRank1(playerNum) == true) {
            //���������𖞂����Ă���
            WinPlayerDistance(playerNum);
        }
    }
}

bool Stage::CheckPlayerRank1(const int& playerNum)
{
    int otherPlayer[con::PlayerNumberMax - 1] = { 0,0,0 }; //���̃v���C���[�̔ԍ���ۑ�

    //���̃v���C���[�̔ԍ����擾
    switch (playerNum) {
    case con::player_1:
        otherPlayer[0] = con::player_2;
        otherPlayer[1] = con::player_3;
        otherPlayer[2] = con::player_4;
        break;
    case con::player_2:
        otherPlayer[0] = con::player_1;
        otherPlayer[1] = con::player_3;
        otherPlayer[2] = con::player_4;
        break;
    case con::player_3:
        otherPlayer[0] = con::player_1;
        otherPlayer[1] = con::player_2;
        otherPlayer[2] = con::player_4;
        break;
    case con::player_4:
        otherPlayer[0] = con::player_1;
        otherPlayer[1] = con::player_2;
        otherPlayer[2] = con::player_3;
        break;
    }

    for (int otherNum = 0; otherNum < con::PlayerNumberMax - 1; otherNum++) {
        if (CheckPlayerDistance20Block(playerNum, otherPlayer[otherNum]) == false) {
            //���������𖞂����Ă��Ȃ�
            return false;
        }
    }

    return true;
}

bool Stage::CheckPlayerDistance20Block(const int& playerNum, const int& otherNum)
{
    //�������g������s�\�̏ꍇ�A
    if (m_findPlayer->GetStopController(playerNum) == true) {
        return false;
    }

    if (m_findPlayer->GetStopController(otherNum) == false) {
        if (m_playerBlockPosition[playerNum] > m_playerBlockPosition[otherNum]) {
            if (m_playerBlockPosition[playerNum] - m_playerBlockPosition[otherNum] >= 20)
            {
                //20�u���b�N�ȏ�Ԃ������Ă���
                return true;
            }
            else {
                //20�u���b�N�ȏ�Ԃ������Ă��Ȃ�
                return false;
            }
        }
        else {
            //�v���C���[���z���Ă��Ȃ�
            return false;
        }
    }

    //�~�X�������v���C���[�͖���
    return true;
}

void Stage::WinPlayerDistance(const int& playerNum)
{
    //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
    for (int pNum = con::FIRST_ELEMENT_ARRAY; pNum < con::PlayerNumberMax; pNum++) {
        m_findPlayer->SetActivePlayer(pNum, false);
        //�S�[��������Ԃɂ���B
        m_findPlayer->SetFlagGoal(pNum, true);
    }

    //���ʂ��m��
    m_findPlayer->SetGoalRanking(playerNum, con::rank_1);
    m_findPlayer->SetAnimationWin(playerNum);

    m_goalPlayer += 1;
}

//////////////////////////////
// �i�s�x
//////////////////////////////

void Stage::DegreeOfProgress(const int& pNum)
{
    float test2 = static_cast<float>(m_playerBlockPosition[pNum]);

    test2 = test2 / m_MAX_BLOCK;

    m_spritePlayerMark[pNum]->SetPositionX(
        MAX_DEGREE_OF_PROGRESS_POSITION * test2
    );
}