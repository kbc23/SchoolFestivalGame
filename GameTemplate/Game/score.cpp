#include "stdafx.h"
#include "score.h"

#include <string>

#include "main_processing.h"
#include "player.h"
#include "pause.h"
#include "game_start_countdown.h"
#include "stage.h"
#include "rank.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // �ʒu���
    ////////////////////////////////////////////////////////////

    const Vector2 SCORE_TIME_FONT_POSITION[con::PlayerNumberMax] = {	//�X�R�A�^�C���̕\���ʒu
        { -570.0f, -300.0f },										        //�v���C���[�P
        { -250.0f, -300.0f },												//�v���C���[�Q
        { 70.0f, -300.0f },												    //�v���C���[�R
        { 400.0f, -300.0f }													//�v���C���[�S
    };

    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    const wchar_t* INIT_FONT_SCORE_TIME = L"0:00:00";   //�����\���̃X�R�A�^�C��
    const int ONE_MINUTES_FLAME = 3600;                 //�t���[���J�E���g�ł̂P��
    const int ONE_DIGIT_CHECK = 10;                     //�P�����̊m�F�Ɏg�p����萔
    const int SIXTY_FLAME = 60;                         //60�t���[��
    const int ADJUSTMENT_CONMA_SECONDS = 100;           //�R���}�b�̕\���̒����Ɏg�p
}



Score::Score()
{
    ////////////////////////////////////////////////////////////
    // NewGO
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // �t�H���g��NewGO
    //////////////////////////////

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontScoreTime[playerNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontScoreTime[playerNum]->Init(INIT_FONT_SCORE_TIME, SCORE_TIME_FONT_POSITION[playerNum]); //������
        m_fontScoreTime[playerNum]->Deactivate(); //��\��
    }

    //////////////////////////////
    // �X�v���C�g��NewGO
    //////////////////////////////

    m_spriteUI = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteUI->Init(filePath::dds::SCORE_TIME_UI); //������
    m_spriteUI->Deactivate(); //��\��
}

Score::~Score()
{
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        DeleteGO(m_fontScoreTime[playerNum]);
    }

    DeleteGO(m_spriteUI);
}

bool Score::Start()
{
    ////////////////////////////////////////////////////////////
    // FindGO
    ////////////////////////////////////////////////////////////

    m_findPlayer = FindGO<Player>(igo::CLASS_NAME_PLAYER);
    m_findMainProcessing = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);
    m_findPause = FindGO<Pause>(igo::CLASS_NAME_PAUSE);
    m_findGameStartCountdown = FindGO<GameStartCountdown>(igo::CLASS_NAME_GAME_START_COUNTDOWN);
    m_findStage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
    m_findRank = FindGO<Rank>(igo::CLASS_NAME_RANK);

    return true;
}

void Score::Init()
{
    m_flagProcess = true;

    ////////////////////////////////////////////////////////////
    // �t�H���g�̏�����
    ////////////////////////////////////////////////////////////

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontScoreTime[playerNum]->Activate(); //�\��
    }

    ////////////////////////////////////////////////////////////
    // �X�v���C�g�̏�����
    ////////////////////////////////////////////////////////////

    m_spriteUI->Activate(); //�\��

    ////////////////////////////////////////////////////////////
    // �����o�ϐ��̏�����
    ////////////////////////////////////////////////////////////

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_scoreTime[playerNum] = 0; //�X�R�A�^�C���̃J�E���g
        m_flagScoreTimeProcessing[playerNum] = true; //�X�R�A�^�C���̃J�E���g�̏����������Ȃ��Ă��邩
        m_scoreTimeMinutes[playerNum] = 0; //��
        m_scoreTimeSeconds[playerNum] = 0; //�b
        m_scoreTimeCommaSeconds[playerNum] = 0; //�R���}�b

        //�\�����Ԃ��O�b�ɃZ�b�g
        m_fontScoreTime[playerNum]->SetText(INIT_FONT_SCORE_TIME);
    }
}

void Score::Finish()
{
    m_flagProcess = false;

    //////////
    // �t�H���g�̔�\��
    //////////

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontScoreTime[playerNum]->Deactivate(); //��\��
    }

    //////////
    // �X�v���C�g�̔�\��
    //////////

    m_spriteUI->Deactivate(); //��\��
}

////////////////////////////////////////////////////////////
// ���t���[���̏���
////////////////////////////////////////////////////////////

void Score::Update()
{
    if (false == m_flagProcess) {
        return;
    }

    //�ŏ��̃J�E���g�_�E�����I����ĂȂ���
    if (m_findGameStartCountdown->GetFlagStopGameProcess() == true) {
        return;
    }

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        if (m_flagScoreTimeProcessing[playerNum] == true) {
            //�^�C���̃J�E���g
            AddTime(playerNum);
            //�^�C���̃J�E���g���~���邩
            FinishTime(playerNum);
            //���݂̃^�C���̕`��
            DrawTime(playerNum);
        }
    }

}

void Score::AddTime(const int playerNum)
{
    ++m_scoreTime[playerNum]; //�J�E���g
}

void Score::FinishTime(const int playerNum)
{
    //�S�[�����Ă��Ȃ��ꍇ
    if (false == m_findRank->GetDoingGoal(playerNum)) {
        return;
    }

    //�J�E���g�������~����
    m_flagScoreTimeProcessing[playerNum] = false;
}

void Score::DrawTime(const int playerNum)
{
    std::wstring scoreTimeString; //�`�悷�镶����

    //���̌v�Z
    if (m_scoreTime[playerNum] == ONE_MINUTES_FLAME) {
        m_scoreTime[playerNum] = con::TIME_RESET_ZERO;
        m_scoreTimeSeconds[playerNum] = con::TIME_RESET_ZERO;
        ++m_scoreTimeMinutes[playerNum];
    }

    //�b�̌v�Z
    m_scoreTimeSeconds[playerNum] = m_scoreTime[playerNum] / SIXTY_FLAME;

    //�R���}�b�̌v�Z
    double conmaSeconds = m_scoreTime[playerNum] % SIXTY_FLAME;
    conmaSeconds = conmaSeconds / SIXTY_FLAME * ADJUSTMENT_CONMA_SECONDS;
    m_scoreTimeCommaSeconds[playerNum] = static_cast<int>(conmaSeconds);

    //�\������^�C���̕�������쐬
    //�b���P��
    if (m_scoreTimeSeconds[playerNum] < ONE_DIGIT_CHECK) {
        //�R���}�b���P��
        if (m_scoreTimeCommaSeconds[playerNum] < ONE_DIGIT_CHECK) {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[playerNum]) + L":0" +
                std::to_wstring(m_scoreTimeSeconds[playerNum]) + L":0" +
                std::to_wstring(m_scoreTimeCommaSeconds[playerNum]);
        }
        //�R���}�b���Q��
        else {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[playerNum]) + L":0" +
                std::to_wstring(m_scoreTimeSeconds[playerNum]) + L":" +
                std::to_wstring(m_scoreTimeCommaSeconds[playerNum]);
        }
    }
    //�b���Q��
    else {
        //�R���}�b���P��
        if (m_scoreTimeCommaSeconds[playerNum] < ONE_DIGIT_CHECK) {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[playerNum]) + L":" +
                std::to_wstring(m_scoreTimeSeconds[playerNum]) + L":0" +
                std::to_wstring(m_scoreTimeCommaSeconds[playerNum]);
        }
        //�R���}�b���Q��
        else {
            scoreTimeString = std::to_wstring(m_scoreTimeMinutes[playerNum]) + L":" +
                std::to_wstring(m_scoreTimeSeconds[playerNum]) + L":" +
                std::to_wstring(m_scoreTimeCommaSeconds[playerNum]);
        }
    }

    //�\�����镶������X�V
    m_fontScoreTime[playerNum]->SetText(scoreTimeString.c_str());
}

void Score::NextRound()
{
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_scoreTime[playerNum] = con::TIME_RESET_ZERO;
        m_flagScoreTimeProcessing[playerNum] = true;
        m_scoreTimeMinutes[playerNum] = con::TIME_RESET_ZERO;
        m_scoreTimeSeconds[playerNum] = con::TIME_RESET_ZERO;
        m_scoreTimeCommaSeconds[playerNum] = con::TIME_RESET_ZERO;
        
        m_fontScoreTime[playerNum]->SetText(INIT_FONT_SCORE_TIME); //�e�L�X�g
    }
}