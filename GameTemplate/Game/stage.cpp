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

    const int DRAW_FIRST_BLOCK = -6;
    const int DRAW_LAST_BLOCK = 15;
    
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

    const Vector2 PLAYER_BLOCK_POSITION_FONT_POSITION[con::PlayerNumberMax] = {	//�X�R�A�^�C���̕\���ʒu
    { -520.0f, -215.0f },										        //�v���C���[�P
    { -200.0f, -215.0f },												//�v���C���[�Q
    { 120.0f, -215.0f },												//�v���C���[�R
    { 440.0f, -215.0f }													//�v���C���[�S
    };

    const float MAX_DEGREE_OF_PROGRESS_POSITION = 835.0f; //�i�s�x

    const float INIT_SPRITE_BACKGROUND_CLOUD_1 = 0.0f;
    const float INIT_SPRITE_BACKGROUND_CLOUD_2 = 1280.0f;

    const float MOVE_BACKGROUND = 1.0f;

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
    // ( 100 - PROBABILITY_CREATE_BLUE_BLOCK )�����F�u���b�N���쐬���邩�̊m��

    //////////////////////////////
    // BGM�̊m��
    //////////////////////////////

    const int MAX_PROBABILITY_BGM = 5; //BGM�̐�
    const int PROBABILITY_BGM[MAX_PROBABILITY_BGM] = { //BGM���Ƃ̊m��
        20,20,20,20,20
    };
}



Stage::Stage()
{
    ////////////////////////////////////////////////////////////
    // NewGO
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // ���f����NewGO
    //////////////////////////////

    //�u���b�N�̏�����
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        //�΃u���b�N
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
            m_modelGreenBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelGreenBlock[playerNum][blockNum]->Init(filePath::tkm::GREEN_BLOCK); //������
            m_modelGreenBlock[playerNum][blockNum]->Deactivate(); //��\��
        }
        //���F�u���b�N
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
            m_modelYellowBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelYellowBlock[playerNum][blockNum]->Init(filePath::tkm::YELLOW_BLOCK); //������
            m_modelYellowBlock[playerNum][blockNum]->Deactivate(); //��\��
        }
        //�S�[���u���b�N
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
            m_modelGoalBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelGoalBlock[playerNum][blockNum]->Init(filePath::tkm::GOAL_BLOCK); //������
            m_modelGoalBlock[playerNum][blockNum]->Deactivate(); //��\��
        }
    }

    //////////////////////////////
    // �X�v���C�g��NewGO
    //////////////////////////////

    //�w�i��NewGO
    //��
    m_spriteBackgroundSky = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundSky->Init(filePath::dds::BACKGROUND_SKY); //������
    m_spriteBackgroundSky->Deactivate(); //��\��
    //�_�P
    m_spriteBackgroundCloud_1 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_1->Init(filePath::dds::BACKGROUND_CLOUD); //������
    m_spriteBackgroundCloud_1->Deactivate(); //��\��
    //�_�Q
    m_spriteBackgroundCloud_2 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_2->Init(filePath::dds::BACKGROUND_CLOUD); //������
    m_spriteBackgroundCloud_2->Deactivate(); //��\��


    //�i�s�x��UI��NewGO
    //�i�s�x�̐�
    m_spriteDegreeOfProgress = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteDegreeOfProgress->Init(filePath::dds::DEGREE_OF_PROGRESS); //������
    m_spriteDegreeOfProgress->Deactivate(); //��\��
    //�v���C���[���Ƃ̃}�[�J�[
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spritePlayerMark[playerNum]->Init(filePath::dds::PLAYER_MARK[playerNum]); //������
        m_spritePlayerMark[playerNum]->Deactivate(); //��\��
    }

    //���E���h�̃|�C���g��UI��NewGO
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
            m_spriteRoundWin[playerNum][roundNum]->Init(filePath::dds::ROUND_WIN[playerNum][roundNum]); //������
            m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //��\��
        }
    }

    //////////////////////////////
    // �t�H���g��NewGO
    //////////////////////////////

    //�v���C���[�����݉��Ԗڂ̃u���b�N�ɂ��邩�̕\����NewGO
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontPlayerBlockPosition[playerNum]->Init(L"", PLAYER_BLOCK_POSITION_FONT_POSITION[playerNum]); //������
        m_fontPlayerBlockPosition[playerNum]->SetText(m_playerBlockPosition[playerNum] + 1); //�e�L�X�g
        m_fontPlayerBlockPosition[playerNum]->Deactivate(); //��\��
    }
}

Stage::~Stage()
{
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
            DeleteGO(m_modelGreenBlock[playerNum][blockNum]);
        }
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
            DeleteGO(m_modelYellowBlock[playerNum][blockNum]);
        }
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
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
    ////////////////////////////////////////////////////////////
    // FindGO
    ////////////////////////////////////////////////////////////

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

    ////////////////////////////////////////////////////////////
    // ���f���̏�����
    ////////////////////////////////////////////////////////////

    //���f���̔�\��
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        BlockDeactivate(playerNum);
    }

    //�X�e�[�W���쐬
    StageCreate();

    //���f���̕`��
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        DrawBlock(playerNum);
    }

    ////////////////////////////////////////////////////////////
    // �X�v���C�g�̏�����
    ////////////////////////////////////////////////////////////


    //�w�i�̏�����
    m_spriteBackgroundSky->Activate(); //�\��
    m_spriteBackgroundCloud_1->Activate(); //�\��
    m_spriteBackgroundCloud_1->SetPositionX(INIT_SPRITE_BACKGROUND_CLOUD_1); //�ʒu
    m_spriteBackgroundCloud_2->Activate(); //�\��
    m_spriteBackgroundCloud_2->SetPositionX(INIT_SPRITE_BACKGROUND_CLOUD_2); //�ʒu

    //�i�s�x��UI�̏�����
    m_spriteDegreeOfProgress->Activate(); //�\��
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->Activate(); //�\��
        m_spritePlayerMark[playerNum]->SetPositionX(0.0f); //�ʒu
    }

    //���E���h��UI�̏�����
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //��\��
        }
    }

    ////////////////////////////////////////////////////////////
    // �t�H���g�̏�����
    ////////////////////////////////////////////////////////////

    //�v���C���[�����݉��Ԗڂ̃u���b�N�ɂ��邩�̕\���̏�����
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum]->Activate(); //�\��
    }

    ////////////////////////////////////////////////////////////
    // BGM��NewGO�A������
    ////////////////////////////////////////////////////////////

    //�Đ�����BGM�̐ݒ�
    InitBGM();
    m_bgm->Play(true); ///�Đ�

    ////////////////////////////////////////////////////////////
    // �����o�ϐ��̏�����
    ////////////////////////////////////////////////////////////

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = m_START_BLOCK; //�v���C���[�����ڂ̃u���b�N�ɂ��邩
        m_playerBeforeBlockPosition[playerNum] = m_START_BLOCK; //�v���C���[�̑O�ɂ����u���b�N�̔ԍ�
        m_amountOfMovement[playerNum] = 0;
        m_playerAnimation[playerNum] = greenBlock;
        m_flagAnimationJump[playerNum] = false;//�W�����v�A�j���[�V��������
        m_timerAnimation[playerNum] = 0; //�A�j���[�V�����̃^�C�}�[
    }

    m_nowRank = m_INIT_RANK; //�v���C���[�̏��ʃf�[�^�ɓn���f�[�^
    m_maxPlayer = con::PlayerNumberMax;	//�v���C���[�̍ő吔
    m_goalPlayer = 0; //�S�[�������v���C���[�̐�
    m_allMiss = false; //�v���C���[�S�����~�X�����Ă��邩
}

void Stage::Finish()
{
    m_flagProcess = false;

    //////////
    // ���f���̔�\��
    //////////

    //�u���b�N�̔�\��
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        BlockDeactivate(playerNum);
    }

    //////////
    // �X�v���C�g�̔�\��
    //////////

    //�w�i�̔�\��
    m_spriteBackgroundSky->Deactivate(); //��\��
    m_spriteBackgroundCloud_1->Deactivate(); //��\��
    m_spriteBackgroundCloud_2->Deactivate(); //��\��

    //�i�s�x��UI�̔�\��
    m_spriteDegreeOfProgress->Deactivate(); //��\��
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->Deactivate(); //��\��
    }

    //���E���h��UI�̔�\��
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //��\��
        }
    }

    //////////
    // �t�H���g�̔�\��
    //////////

    //�v���C���[�����݉��Ԗڂ̃u���b�N�ɂ��邩�̕\���̔�\��
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum]->Deactivate(); //��\��
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

    int continuousGreenBlock = 0; //�΂̃u���b�N������A���ŏo�Ă��邩�B
    bool lastTimeBlockBlueOrYellow = false; //�O��̃u���b�N���F�����F��������

    //�v���C���[�P�̂�ݒ�
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        //�ŏ��̃u���b�N�͗�
        if (blockNum == con::FIRST_ELEMENT_ARRAY) {
            m_stageData[con::player_1][blockNum] = greenBlock; //�΃u���b�N
            ++continuousGreenBlock; //�΃u���b�N�̘A���񐔂𑝉�
            continue;
        }

        //�S�[���̈ʒu�̃u���b�N�͗�
        if (blockNum == m_MAX_BLOCK - 1) {
            m_stageData[con::player_1][blockNum] = goalBlock; //�S�[���u���b�N
            continue;
        }

        //�΃u���b�N���U��A���ō쐬����Ă���ꍇ
        if (continuousGreenBlock >= UPPER_LIMIT_CONTINUOUS_GREEN_BLOCK) {
            //�u���b�N�A���F�u���b�N�̃Z�b�g
            CreateBlueOrYellow(blockNum, randomNum(mt));
            continuousGreenBlock = 0; //�΃u���b�N�̘A���񐔂����Z�b�g
            lastTimeBlockBlueOrYellow = true; //�O��̃u���b�N���F�����F��������Ԃɂ���
            continue;
        }

        //�O�񂪐F�����F�̃u���b�N�������ꍇ
        if (lastTimeBlockBlueOrYellow == true) {
            m_stageData[con::player_1][blockNum] = greenBlock; //�΃u���b�N
            ++continuousGreenBlock; //�΃u���b�N�̘A���񐔂𑝉�
            lastTimeBlockBlueOrYellow = false; //�O��̃u���b�N���F�����F�ł͂Ȃ�������Ԃɂ���
            continue;
        }

        //�΃u���b�N���쐬���邩
        if (CreateGreen(continuousGreenBlock, randomNum(mt)) == true) {
            //�΃u���b�N���쐬
            m_stageData[con::player_1][blockNum] = greenBlock; //�΃u���b�N
            ++continuousGreenBlock; //�΃u���b�N�̘A���񐔂𑝉�
            continue;
        }
        else {
            //�u���b�N�A���F�u���b�N���쐬
            CreateBlueOrYellow(blockNum, randomNum(mt));
            continuousGreenBlock = 0; //�΃u���b�N�̘A���񐔂����Z�b�g
            lastTimeBlockBlueOrYellow = true; //�O��̃u���b�N���F�����F�ł͂Ȃ�������Ԃɂ���
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

    int randomNum = random(mt); //����
    int checkRandom = 0; //BGM���Ƃ̊m���̊m�F�ɂ��悤����ϐ�

    for (int check = con::FIRST_ELEMENT_ARRAY; check < MAX_PROBABILITY_BGM; check++) {
        checkRandom += PROBABILITY_BGM[check]; //BGM���Ƃ̊m���𑫂�

        //����BGM�ɂ���
        if (randomNum <= checkRandom) {
            m_bgm = NewGO<SoundBGM>(igo::PRIORITY_CLASS);
            m_bgm->Init(filePath::bgm::STAGE[check]); //������
            return;
        }
    }
}

void Stage::BlockDeactivate(const int playerNum)
{
    //�΃u���b�N
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[playerNum][blockNum]->Deactivate(); //��\��
    }
    //���F�u���b�N
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
        m_modelYellowBlock[playerNum][blockNum]->Deactivate(); //��\��
    }
    //�S�[���u���b�N
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
        m_modelGoalBlock[playerNum][blockNum]->Deactivate(); //��\��
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
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        //�u���b�N�̕`�揈��
        DrawMoveBlock(playerNum);

        //�v���C���[�̐i�񂾃u���b�N���̕\��
        DrawFontPlayerBlockPosition(playerNum);

        //�i�s�x�̕`��
        DegreeOfProgress(playerNum);
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

void Stage::DrawBlock(const int playerNum)
{
    //���f���̔�\��
    BlockDeactivate(playerNum);

    //���f���̕`��
    int numberOfUsesGreenBlock = 0;
    int numberOfUsesYellowBlock = 0;
    const int numberOfUsesGoalBlock = 0;

    for (int blockNum = DRAW_FIRST_BLOCK; blockNum < DRAW_LAST_BLOCK; blockNum++) {
        //�X�^�[�g�n�_���O�̃u���b�N��`�悵�Ȃ�
        if (m_playerBlockPosition[playerNum] + blockNum < 0) {
            continue;
        }
        //�S�[���n�_����̃u���b�N��`�悵�Ȃ�
        if (m_playerBlockPosition[playerNum] + blockNum >= m_MAX_BLOCK) {
            break;
        }

        //�΃u���b�N
        if (m_stageData[playerNum][m_playerBlockPosition[playerNum] + blockNum] == greenBlock) {
            m_modelGreenBlock[playerNum][numberOfUsesGreenBlock]->SetPosition({ //�ʒu
            BLOCK_POSITION_X[playerNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelGreenBlock[playerNum][numberOfUsesGreenBlock]->Activate(); //�\��
            ++numberOfUsesGreenBlock;
        }
        //�u���b�N
        else if (m_stageData[playerNum][m_playerBlockPosition[playerNum] + blockNum] == blueBlock) {
            // do nothing
        }
        //���F�u���b�N
        else if (m_stageData[playerNum][m_playerBlockPosition[playerNum] + blockNum] == yellowBlock) {
            m_modelYellowBlock[playerNum][numberOfUsesYellowBlock]->SetPosition({ //�ʒu
               BLOCK_POSITION_X[playerNum],
               BLOCK_POSITION_Y,
               BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelYellowBlock[playerNum][numberOfUsesYellowBlock]->Activate(); //�\��
            ++numberOfUsesYellowBlock;
        }
        //�S�[���u���b�N
        else if (m_stageData[playerNum][m_playerBlockPosition[playerNum] + blockNum] == goalBlock) {
            m_modelGoalBlock[playerNum][numberOfUsesGoalBlock]->SetPosition({ //�ʒu
               BLOCK_POSITION_X[playerNum],
               BLOCK_POSITION_Y,
               BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelGoalBlock[playerNum][numberOfUsesGoalBlock]->Activate(); //�\��
        }
    }
}

void Stage::DrawMoveBlock(const int playerNum)
{
    //�v���C���[���W�����v�A�j���[�V�������ł͂Ȃ��Ƃ�
    if (m_flagAnimationJump[playerNum] == false) {
        DrawBlock(playerNum);

        return;
    }

    ++m_timerAnimation[playerNum];

    //���f����S����\���ɂ���B
    BlockDeactivate(playerNum);


    //���f���̕`��
    int numberOfUsesGreenBlock = 0; //�΃u���b�N�̕`�搔
    int numberOfUsesYellowBlock = 0; //�o�i�i�u���b�N�̕`�搔
    const int numberOfUsesGoalBlock = 0; //�S�[���u���b�N�̕`�搔

    double moveCorrection = 0; //���f���̈ړ���

    int playerBlockPosition = m_playerBlockPosition[playerNum] - m_amountOfMovement[playerNum]; //���f���̌��݂̈ʒu

    if (m_timerAnimation[playerNum] < 30) {
        double test = BLOCK_SIZE * m_amountOfMovement[playerNum] / 30;

        moveCorrection = test * m_timerAnimation[playerNum];

        //�u���b�N�̕`��
        for (int blockNum = DRAW_FIRST_BLOCK; blockNum < DRAW_LAST_BLOCK; blockNum++) {
            //�X�^�[�g�n�_���O�̃u���b�N��`�悵�Ȃ�
            if (playerBlockPosition + blockNum < 0) {
                continue;
            }
            //�S�[���n�_����̃u���b�N��`�悵�Ȃ�
            if (playerBlockPosition + blockNum >= m_MAX_BLOCK) {
                break;
            }
            
            //�΃u���b�N
            if (m_stageData[playerNum][playerBlockPosition + blockNum] == greenBlock) {
                m_modelGreenBlock[playerNum][numberOfUsesGreenBlock]->SetPosition({ //�ʒu
                BLOCK_POSITION_X[playerNum],
                BLOCK_POSITION_Y,
                BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - static_cast<float>(moveCorrection)
                    });
                m_modelGreenBlock[playerNum][numberOfUsesGreenBlock]->Activate(); //�\��
                ++numberOfUsesGreenBlock;
            }
            //�u���b�N
            else if (m_stageData[playerNum][playerBlockPosition + blockNum] == blueBlock) {
                // do nothing
            }
            //���F�u���b�N
            else if (m_stageData[playerNum][playerBlockPosition + blockNum] == yellowBlock) {
                m_modelYellowBlock[playerNum][numberOfUsesYellowBlock]->SetPosition({ //�ʒu
                   BLOCK_POSITION_X[playerNum],
                   BLOCK_POSITION_Y,
                   BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - static_cast<float>(moveCorrection)
                    });
                m_modelYellowBlock[playerNum][numberOfUsesYellowBlock]->Activate(); //�\��
                ++numberOfUsesYellowBlock;
            }
            //�S�[���u���b�N
            else if (m_stageData[playerNum][playerBlockPosition + blockNum] == goalBlock) {
                m_modelGoalBlock[playerNum][numberOfUsesGoalBlock]->SetPosition({ //�ʒu
                   BLOCK_POSITION_X[playerNum],
                   BLOCK_POSITION_Y,
                   BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - static_cast<float>(moveCorrection)
                    });
                m_modelGoalBlock[playerNum][numberOfUsesGoalBlock]->Activate(); //�\��
            }
        }
    }
    else {
        DrawBlock(playerNum);
        m_timerAnimation[playerNum] = 0;
        m_flagAnimationJump[playerNum] = false;
    }
}

void Stage::DrawFontPlayerBlockPosition(const int playerNum)
{
    //�v���C���[�̃u���b�N�ʒu�̏����X�V
    m_fontPlayerBlockPosition[playerNum]->SetText(m_playerBlockPosition[playerNum] + 1);
}

void Stage::DrawBackground()
{
    //�w�i�𓮂���
    m_spriteBackgroundCloud_1->SetPositionX(m_spriteBackgroundCloud_1->GetPosition().x - MOVE_BACKGROUND);
    m_spriteBackgroundCloud_2->SetPositionX(m_spriteBackgroundCloud_2->GetPosition().x - MOVE_BACKGROUND);

    //���ʓ�������ŏ��̏�Ԃɖ߂�
    if (m_spriteBackgroundCloud_1->GetPosition().x <= -INIT_SPRITE_BACKGROUND_CLOUD_2) {
        m_spriteBackgroundCloud_1->SetPositionX(INIT_SPRITE_BACKGROUND_CLOUD_1);
        m_spriteBackgroundCloud_2->SetPositionX(INIT_SPRITE_BACKGROUND_CLOUD_2);
    }
}

void Stage::DrawRoundWin()
{
    //�T�h���f�X���[�h�łȂ��Ƃ�
    if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == false) {
        return;
    }

    //�擾���E���h����UI�̕\��
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            if (roundNum < m_findPlayer->GetRoundPoint(playerNum)) {
                m_spriteRoundWin[playerNum][roundNum]->Activate(); //�\��
            }
        }
    }
}

//////////////////////////////
// �u���b�N�̈ړ�����
//////////////////////////////

bool Stage::MoveBlock(const int playerNum, const int moveNum)
{
    //����s�\��
    if (m_findPlayer->GetStopController(playerNum) == true) {
        return false;
    }

    //�ړ��ʂ�ۑ�
    m_amountOfMovement[playerNum] = moveNum;

    //�v���C���[�̌��݂̈ʒu��ۑ�
    m_playerBeforeBlockPosition[playerNum] = m_playerBlockPosition[playerNum];

    //�v���C���[�����ڂ̃u���b�N�ɂ��邩�̏����X�V
    m_playerBlockPosition[playerNum] += moveNum;

    //�ő吔�𒴂��Ă�����A�ő吔 - 1�ɍ��킹��B
    if (m_playerBlockPosition[playerNum] >= m_MAX_BLOCK) {
        m_playerBlockPosition[playerNum] = m_MAX_BLOCK - 1;
        m_amountOfMovement[playerNum] = con::MOVE_1;
    }

    //�u���b�N�̕`��
    m_flagAnimationJump[playerNum] = true;

    return true;
}

//////////////////////////////
// �u���b�N���Ƃ̏���
//////////////////////////////

void Stage::CheckBlock(const int playerNum)
{
    //�u���b�N���Ƃɏ���
    //�΃u���b�N
    if (m_stageData[playerNum][m_playerBlockPosition[playerNum]] == greenBlock) {
        // do nothing
    }
    //�u���b�N
    else if (m_stageData[playerNum][m_playerBlockPosition[playerNum]] == blueBlock) {
        BlueBlock(playerNum);
    }
    //���F�u���b�N
    else if (m_stageData[playerNum][m_playerBlockPosition[playerNum]] == yellowBlock) {
        YellowBlock(playerNum);
    }
}

//////////
// �F�̃u���b�N
//////////

void Stage::BlueBlock(const int playerNum)
{
    //����s�\�ɂ���
    m_findPlayer->SetStopController(playerNum, true);

    //�A�j���[�V������ݒ�
    m_findPlayer->SetNowAnimationBlock(playerNum, con::blueBlock);

    //CPU�R���g���[���[�ɐu���b�N�Ń~�X���������Ƃ�`����
    m_findCPUPlayerController->SetFlagBlueBlockMiss(playerNum, true);
}

//////////
// ���F�̃u���b�N
//////////

void Stage::YellowBlock(const int playerNum)
{
    //���F�u���b�N��΃u���b�N�ɕύX
    m_stageData[playerNum][m_playerBlockPosition[playerNum]] = greenBlock;
    DrawBlock(playerNum); //�`��

    //����s�\�ɂ���
    m_findPlayer->SetStopController(playerNum, true);

    //�A�j���[�V������ݒ�
    m_findPlayer->SetNowAnimationBlock(playerNum, con::yellowBlock);
}

//////////
// ���̑�
//////////

void Stage::ReturnBlock(const int playerNum)
{
    //�v���C���[�̌��݈ʒu��O�̈ʒu�ɖ߂��B
    m_playerBlockPosition[playerNum] = m_playerBeforeBlockPosition[playerNum];

    //���f���̈ʒu���X�V
    DrawBlock(playerNum);
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

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        //�v���C���[�̏��ʂ��m��
        if (m_findPlayer->GetActivePlayer(playerNum) == true && m_playerBlockPosition[playerNum] == m_MAX_BLOCK - 1) {
            //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
            m_findPlayer->SetActivePlayer(playerNum, false);

            //���ʂ��m��
            m_findPlayer->SetGoalRanking(playerNum, m_nowRank);

            //�S�[��������Ԃɂ���B
            m_findPlayer->SetFlagGoal(playerNum, true);

            //���̃v���C���[�̏��ʂ̂��߂ɑ���
            ++nextRank;
            flagAddNowRank = true;

            //�T�h���f�X���[�h��
            if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
                m_goalPlayer += 1;
            }
        }
    }

    //���̃v���C���[�̏��ʂ��ς��ꍇ
    if (flagAddNowRank == true) {
        //���̏��ʂ�ݒ�
        m_nowRank += nextRank;
    }
    
    //�~�X�ɂ�鏟�������̊m�F
    MissRoundWin();
}

void Stage::MissRoundWin()
{
    //�T�h���f�X���[�h��
    if (false == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
        return;
    }

    //�S�[�������v���C���[�����邩
    if (1 <= m_goalPlayer) {
        //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
        for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
            m_findPlayer->SetActivePlayer(playerNum, false);
        }

        //���̃��E���h�Ɉڍs����
        m_findSuddenDeathMode->SetFlagNextRound(true);
        m_goalPlayer = 0;
    }
    //�S�[�������v���C���[�����Ȃ��Ƃ�
    else {
        int count = 0; //�~�X�����v���C���[�̐l��

        //�~�X�������v���C���[���J�E���g
        for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
            if (m_findPlayer->GetStopController(playerNum) == true) {
                ++count;
            }
        }

        //�R�l�~�X�̂Ƃ�
        if (count == 3) {
            for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
                if (m_findPlayer->GetStopController(playerNum) == false) {
                    //���ʂ��m��
                    m_findPlayer->SetGoalRanking(playerNum, con::rank_1);
                    m_findPlayer->SetAnimationWin(playerNum); //�A�j���[�V����

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

//////////////////////////////
// ���E���h�ύX�̏���
//////////////////////////////

void Stage::NextRound()
{
    if (m_allMiss == false) {
        //�v���C���[�̃��E���h�����|�C���g
        for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
            //�P�ʂ̃v���C���[
            if (m_findPlayer->GetGoalRanking(playerNum) == con::rank_1) {
                //���E���h���擾
                m_findPlayer->AddRoundPoint(playerNum);
            }
        }
    }
    
    //�X�e�[�W���쐬
    StageCreate();

    //���f���̔�\��
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        BlockDeactivate(playerNum);
    }

    //���f���̕`��
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = 0; //�v���C���[�̈ʒu���ŏ��̃u���b�N�̈ʒu�ɂ���
        DrawBlock(playerNum);
    }

    //�ϐ��̒l�̃��Z�b�g
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = m_START_BLOCK;
        m_playerBeforeBlockPosition[playerNum] = m_START_BLOCK;
        m_amountOfMovement[playerNum] = 0;
        m_flagAnimationJump[playerNum] = false;
        m_timerAnimation[playerNum] = 0;
    }

    m_nowRank = m_INIT_RANK;
    
    m_goalPlayer = 0;

    m_allMiss = false;
}

//////////////////////////////
// �����ɂ�鏟������
//////////////////////////////

void Stage::CheckPlayerDistance()
{
    //�T�h���f�X���[�h��
    if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == false) {
        return;
    }

    //�S�[�������v���C���[���P�l�ȏ�
    if (1 <= m_goalPlayer) {
        return;
    }

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        //�����ɂ�鏟�������𖞂����Ă��邩
        if (CheckPlayerRank1(playerNum) == true) {
            //���������𖞂����Ă���
            WinPlayerDistance(playerNum);
        }
    }
}

bool Stage::CheckPlayerRank1(const int playerNum)
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


    for (int otherNum = con::FIRST_ELEMENT_ARRAY; otherNum < con::PlayerNumberMax - 1; otherNum++) {
        //�v���C���[���Ƃɋ������ׂ�
        if (CheckPlayerDistance20Block(playerNum, otherPlayer[otherNum]) == false) {
            //���������𖞂����Ă��Ȃ�
            return false;
        }
    }

    return true;
}

bool Stage::CheckPlayerDistance20Block(const int playerNum, const int otherNum)
{
    //�������g������s�\�̂Ƃ�
    if (m_findPlayer->GetStopController(playerNum) == true) {
        return false;
    }

    //�������g������s�\�łȂ��Ƃ�
    if (m_findPlayer->GetStopController(otherNum) == false) {
        //�Ώۂ̃v���C���[�Ƃ̋������v�Z����
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

void Stage::WinPlayerDistance(const int playerNum)
{
    //�v���C���[�̑�����ł��Ȃ��悤�ɂ���B
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_findPlayer->SetActivePlayer(playerNum, false);
        //�S�[��������Ԃɂ���B
        m_findPlayer->SetFlagGoal(playerNum, true);
    }

    //���ʂ��m��
    m_findPlayer->SetGoalRanking(playerNum, con::rank_1);
    m_findPlayer->SetAnimationWin(playerNum); //�A�j���[�V����
    
    //�S�[�������v���C���[�̐l���𑝂₷
    m_goalPlayer += 1;
}

//////////////////////////////
// �i�s�x
//////////////////////////////

void Stage::DegreeOfProgress(const int playerNum)
{
    float moveSprite = static_cast<float>(m_playerBlockPosition[playerNum]);

    moveSprite = moveSprite / m_MAX_BLOCK;

    m_spritePlayerMark[playerNum]->SetPositionX(
        MAX_DEGREE_OF_PROGRESS_POSITION * moveSprite
    );
}