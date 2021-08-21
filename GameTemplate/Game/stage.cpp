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
    //※バナナとバナナが乗ってるブロックは１つのモデルでできている。

    ////////////////////////////////////////////////////////////
    // ブロックの情報
    ////////////////////////////////////////////////////////////

    const float BLOCK_SIZE = 80.0f;         //ブロックのサイズ

    ////////////////////////////////////////////////////////////
    // 位置情報
    ////////////////////////////////////////////////////////////

    const float BLOCK_POSITION_X[con::PlayerNumberMax] = {           //ブロックのX座標
        390.0f,                                                             //プレイヤー１
        130.0f,                                                             //プレイヤー２
        -130.0f,                                                            //プレイヤー３
        -390.0f                                                             //プレイヤー４
    };
    const float BLOCK_POSITION_Y = -80.0f;                              //ブロックのY座標
    const float BLOCK_POSITION_Z = -250.0f;                             //ブロックのZ座標
    const Vector3 BLOCK_START_POSITION[con::PlayerNumberMax] = {     //プレイヤーごとのスタート位置のブロックの座標
        { 390.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z }, 	                //プレイヤー１
        { 130.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z },	                    //プレイヤー２
        { -130.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z },	                //プレイヤー３
        { -390.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z }	                    //プレイヤー４
    };

    const Vector2 PLAYER_BLOCK_POSITION_FONT_POSITION[con::PlayerNumberMax] = {	//スコアタイムの表示位置
    { -520.0f, -215.0f },										        //プレイヤー１
    { -200.0f, -215.0f },												//プレイヤー２
    { 120.0f, -215.0f },												//プレイヤー３
    { 440.0f, -215.0f }													//プレイヤー４
    };

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    const int TIMER_RESET = 0;                  //タイマーのリセット
    const int TIME_RETURN_OPERATION = 30;       //操作復帰にかかる時間（1.5秒）

    const int TIME_BLUE_BLOCK_ANIMATION = 30;   //青色のブロックに行ったときのアニメーションの時間

    ////////////////////////////////////////////////////////////
    // 入力関連
    ////////////////////////////////////////////////////////////

    const int MOVE_BUTTON_A = 2;		//Aボタンを押したときの移動量
    const int MOVE_BUTTON_B = 1;		//Bボタンを押したときの移動量

    ////////////////////////////////////////////////////////////
    // 確率関連
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // ブロックの並びの生成
    //////////////////////////////

    const int UPPER_LIMIT_CONTINUOUS_GREEN_BLOCK = 6;   //連続で作成する緑ブロックの上限
    const int PROBABILITY_CREATE_GREEN_BLOCK[UPPER_LIMIT_CONTINUOUS_GREEN_BLOCK] = {    //緑ブロックを作成するかの確率
        80,                                                                                 //０個
        70,                                                                                 //１個
        65,                                                                                 //２個
        60,                                                                                 //３個
        55,                                                                                 //４個
        50                                                                                  //５個
    };
    const int PROBABILITY_CREATE_BLUE_BLOCK = 70;       //青ブロックを作成するかの確率
    //( 100 - PROBABILITY_CREATE_BLUE_BLOCK )が黄色ブロックを作成するかの確率

    //////////////////////////////
    // BGMの確率
    //////////////////////////////

    const int MAX_PROBABILITY_BGM = 5;
    const int PROBABILITY_BGM[MAX_PROBABILITY_BGM] = {
        20,20,20,20,20
    };

    ////////////////////////////////////////////////////////////
    // 進行度関連
    ////////////////////////////////////////////////////////////

    const float MAX_DEGREE_OF_PROGRESS_POSITION = 835.0f;
}



Stage::Stage()
{
    //////////////////////////////
    // NewGO
    //////////////////////////////

    //////////
    // モデルのNewGO
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
    // スプライトのNewGO
    //////////

    //背景のNewGO
    m_spriteBackgroundSky = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundSky->Init(filePath::dds::BACKGROUND_SKY);
    m_spriteBackgroundSky->Deactivate();
    m_spriteBackgroundCloud_1 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_1->Init(filePath::dds::BACKGROUND_CLOUD);
    m_spriteBackgroundCloud_1->Deactivate();
    m_spriteBackgroundCloud_2 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_2->Init(filePath::dds::BACKGROUND_CLOUD);
    m_spriteBackgroundCloud_2->Deactivate();

    //進行度のUIのNewGO
    m_spriteDegreeOfProgress = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteDegreeOfProgress->Init(filePath::dds::DEGREE_OF_PROGRESS);
    m_spriteDegreeOfProgress->Deactivate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spritePlayerMark[playerNum]->Init(filePath::dds::PLAYER_MARK[playerNum]);
        m_spritePlayerMark[playerNum]->Deactivate();
    }

    //ラウンドのUIのNewGO
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
            m_spriteRoundWin[playerNum][roundNum]->Init(filePath::dds::ROUND_WIN[playerNum][roundNum]);
            m_spriteRoundWin[playerNum][roundNum]->Deactivate();
        }
    }

    //////////
    // フォントのNewGO
    //////////

    //プレイヤーが現在何番目のブロックにいるかの表示のNewGO
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
// Start関数関連
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
    // モデルの初期化
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

    //ステージを作成
    StageCreate();

    //モデルの描画
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        DrawBlock(playerNum);
    }

    //////////
    // スプライトの初期化
    //////////

    //背景の初期化
    m_spriteBackgroundSky->Activate();
    m_spriteBackgroundCloud_1->Activate();
    m_spriteBackgroundCloud_1->SetPositionX(0.0f);
    m_spriteBackgroundCloud_2->Activate();
    m_spriteBackgroundCloud_2->SetPositionX(1280.0f);

    //進行度のUIの初期化
    m_spriteDegreeOfProgress->Activate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->Activate();
        m_spritePlayerMark[playerNum]->SetPositionX(0.0f);
    }

    //ラウンドのUIの初期化
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum]->Deactivate();
        }
    }

    //////////
    // フォントの初期化
    //////////

    //プレイヤーが現在何番目のブロックにいるかの表示の初期化
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum]->Activate();
    }

    //////////
    // BGMのNewGO、初期化
    //////////

    InitBGM();
    m_bgm->Play(true);

    //////////
    // メンバ変数の初期化
    //////////

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = m_START_BLOCK; //プレイヤーが何個目のブロックにいるか
        m_playerBeforeBlockPosition[playerNum] = m_START_BLOCK; //プレイヤーの前にいたブロックの番号
        m_amountOfMovement[playerNum] = 0;
        m_blueMiss[playerNum] = false;
        m_playerAnimation[playerNum] = greenBlock;
        m_flagAnimationJump[playerNum] = false;//ジャンプアニメーション中か
        m_timerAnimation[playerNum] = 0; //アニメーションのタイマー
    }

    m_nowRank = m_INIT_RANK; //プレイヤーの順位データに渡すデータ
    m_maxPlayer = con::PlayerNumberMax;	//プレイヤーの最大数
    m_goalPlayer = 0; //ゴールしたプレイヤーの数
    m_nextTime = 0; //次のラウンドに移るのに一瞬で行かないための待ち時間
    m_allMiss = false; //プレイヤー全員がミスをしているか
}

void Stage::Finish()
{
    m_flagProcess = false;

    //////////
    // モデルの非表示
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
    // スプライトの非表示
    //////////

    //背景の非表示
    m_spriteBackgroundSky->Deactivate();
    m_spriteBackgroundCloud_1->Deactivate();
    m_spriteBackgroundCloud_2->Deactivate();

    //進行度のUIの非表示
    m_spriteDegreeOfProgress->Deactivate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->Deactivate();
    }

    //ラウンドのUIの非表示
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum]->Deactivate();
        }
    }

    //////////
    // フォントの非表示
    //////////

    //プレイヤーが現在何番目のブロックにいるかの表示の非表示
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum]->Deactivate();
    }

    //////////
    // BGMのDeleteGO
    //////////

    DeleteGO(m_bgm);
}

void Stage::StageCreate()
{
    //緑は連続で６個まで
    //青の後は緑
    //黄色の後は緑
    //青:70% 黄色:30%

    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<int> randomNum(con::MINIMUM_RANDOM_NUMBER, con::MAXIMUM_RANDOM_NUMBER);

    int continuousGreenBlock = 0;   //緑のブロックが何回連続で出ているか。
    bool lastTimeBlockBlueOrYellow = false; //前回のブロックが青色か黄色だったか

    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        //最初のブロックは緑
        if (blockNum == con::FIRST_ELEMENT_ARRAY) {
            m_stageData[con::player_1][blockNum] = greenBlock;
            ++continuousGreenBlock;
            continue;
        }

        //ゴールの位置のブロックは緑
        if (blockNum == m_MAX_BLOCK - 1) {
            m_stageData[con::player_1][blockNum] = goalBlock;
            continue;
        }

        //緑ブロックが６回連続で作成されている場合
        if (continuousGreenBlock >= UPPER_LIMIT_CONTINUOUS_GREEN_BLOCK) {
            //青ブロック、黄色ブロックのセット
            CreateBlueOrYellow(blockNum, randomNum(mt));
            continuousGreenBlock = 0;
            lastTimeBlockBlueOrYellow = true;
            continue;
        }

        //前回が青色か黄色のブロックだった場合
        if (lastTimeBlockBlueOrYellow == true) {
            m_stageData[con::player_1][blockNum] = greenBlock;
            ++continuousGreenBlock;
            lastTimeBlockBlueOrYellow = false;
            continue;
        }

        //緑ブロックを作成するか
        if (CreateGreen(continuousGreenBlock, randomNum(mt)) == true) {
            //緑ブロックを作成
            m_stageData[con::player_1][blockNum] = greenBlock;
            ++continuousGreenBlock;
            continue;
        }
        else {
            //青ブロック、黄色ブロックを作成
            CreateBlueOrYellow(blockNum, randomNum(mt));
            continuousGreenBlock = 0;
            lastTimeBlockBlueOrYellow = true;
            continue;
        }
    }

    //プレイヤー１でセットしたものを他のプレイヤーのところにもセットする。
    for (int playerNum = con::player_2; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
            m_stageData[playerNum][blockNum] = m_stageData[con::player_1][blockNum];
        }
    }
}

bool Stage::CreateGreen(const int continuousGreenBlock, const int randomGreenNum)
{
    //緑ブロックを作成するかどうか
    if (randomGreenNum <= PROBABILITY_CREATE_GREEN_BLOCK[continuousGreenBlock]) {
        //緑ブロックを作成
        return true;
    }
    else {
        //緑ブロックを作成しない
        return false;
    }
}

void Stage::CreateBlueOrYellow(const int blockNum, const int randomBlueOrYellowNum)
{
    //青ブロックと黄色ブロックのどちらを作成するか
    if (randomBlueOrYellowNum <= PROBABILITY_CREATE_BLUE_BLOCK) {
        //青ブロックを作成
        m_stageData[con::player_1][blockNum] = blueBlock;
    }
    else {
        //黄色ブロックを作成
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
// 毎フレームの処理
////////////////////////////////////////////////////////////

void Stage::Update()
{
    if (false == m_flagProcess) {
        return;
    }

    //プレイヤーごとの処理
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        DrawMoveBlock(playerNum);

        DrawFontPlayerBlockPosition(playerNum);

        //進行度の描画
        DegreeOfProgress(playerNum);

        //サドンデスモード時のアニメーション
        if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
            if (m_findPlayer->GetStopController(playerNum) == false &&
                m_findPlayer->GetModelIsActive(playerNum) == true) {
            }
        }
    }

    //背景の描画
    DrawBackground();

    //ラウンド勝利数の描画
    DrawRoundWin();

    //ゴール時の処理
    GoalBlock();

    //20マス差の判定
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

    //モデルの描画
    int numberOfUsesGreenBlock = 0;
    int numberOfUsesBlueBlock = 0;
    int numberOfUsesYellowBlock = 0;
    const int numberOfUsesGoalBlock = 0;

    for (int blockNum = -6; blockNum < 15; blockNum++) {
        //スタート地点より前のブロックを描画しない
        if (m_playerBlockPosition[pNum] + blockNum < 0) {
            continue;
        }
        //ゴール地点より後のブロックを描画しない
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

    //モデルを全部非表示にする。
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
        m_modelYellowBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
        m_modelGoalBlock[pNum][blockNum]->Deactivate();
    }


    //モデルの描画
    int numberOfUsesGreenBlock = 0; //緑ブロックの描画数
    int numberOfUsesBlueBlock = 0; //穴ブロックの描画数
    int numberOfUsesYellowBlock = 0; //バナナブロックの描画数
    const int numberOfUsesGoalBlock = 0; //ゴールブロックの描画数

    double moveCorrection = 0;

    int playerBlockPosition = m_playerBlockPosition[pNum] - m_amountOfMovement[pNum];



    if (m_timerAnimation[pNum] < 30) {
        double test = BLOCK_SIZE * m_amountOfMovement[pNum] / 30;

        moveCorrection = test * m_timerAnimation[pNum];


        for (int blockNum = -6; blockNum < 15; blockNum++) {
            //スタート地点より前のブロックを描画しない
            if (playerBlockPosition + blockNum < 0) {
                continue;
            }
            //ゴール地点より後のブロックを描画しない
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

    //ラウンドのUI
    for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = 0; roundNum < 3; roundNum++) {
            if(roundNum < m_findPlayer->GetRoundPoint(playerNum))
            m_spriteRoundWin[playerNum][roundNum]->Activate();
        }
    }
}

//////////////////////////////
// ブロックの移動処理
//////////////////////////////

bool Stage::MoveBlock(const int pNum, const int moveNum)
{
    if (m_findPlayer->GetStopController(pNum) == true) {
        //操作不可能時
        return false;
    }

    //移動量を保存
    m_amountOfMovement[pNum] = moveNum;

    //プレイヤーの現在の位置を保存
    m_playerBeforeBlockPosition[pNum] = m_playerBlockPosition[pNum];

    //プレイヤーが何個目のブロックにいるかの情報を更新
    m_playerBlockPosition[pNum] += moveNum;

    //最大数を超えていたら、最大数 - 1に合わせる。
    if (m_playerBlockPosition[pNum] >= m_MAX_BLOCK) {
        m_playerBlockPosition[pNum] = m_MAX_BLOCK - 1;
        m_amountOfMovement[pNum] = MOVE_BUTTON_B;
    }

    //ブロックの描画
    m_flagAnimationJump[pNum] = true;

    return true;
}

//////////////////////////////
// ブロックごとの処理
//////////////////////////////

void Stage::CheckBlock(const int pNum)
{
    m_blueMiss[pNum] = false;
   
    //自キャラがいるブロックによって処理をおこなう。

    //ブロックごとに処理
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
// 青色のブロック
//////////

void Stage::BlueBlock(const int& pNum)
{
    //操作不能にする
    m_findPlayer->SetStopController(pNum, true);

    //アニメーションを設定
    m_findPlayer->SetNowAnimationBlock(pNum, con::blueBlock);

    //CPUに青ブロックでミスをしたことを伝える
    m_findCPUPlayerController->SetFlagBlueBlockMiss(pNum, true);
}

//////////
// 黄色のブロック
//////////

void Stage::YellowBlock(const int& playerNum)
{
    //黄色ブロックを緑ブロックに変更
    m_stageData[playerNum][m_playerBlockPosition[playerNum]] = greenBlock;
    DrawBlock(playerNum);

    //操作不能にする
    m_findPlayer->SetStopController(playerNum, true);

    //アニメーションを設定
    m_findPlayer->SetNowAnimationBlock(playerNum, con::yellowBlock);
}

//////////
// その他
//////////

void Stage::ReturnBlock(const int pNum)
{
    //プレイヤーの現在位置を前の位置に戻す。
    m_playerBlockPosition[pNum] = m_playerBeforeBlockPosition[pNum];

    //モデルの位置を更新
    DrawBlock(pNum);
}

//////////////////////////////
// ゴール時の処理
//////////////////////////////

void Stage::GoalBlock()
{
    //仮設置
    if (m_findPlayer->GetFinishSuddenDeath() == true) {
        return;
    }

    bool flagAddNowRank = false; //プレイヤーの順位に代入する数字が変わるかのフラグ
    int nextRank = 0; //次のプレイヤーの順位

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        //プレイヤーの順位を確定
        if (m_findPlayer->GetActivePlayer(playerNum) == true && m_playerBlockPosition[playerNum] == m_MAX_BLOCK - 1) {
            //プレイヤーの操作をできないようにする。
            m_findPlayer->SetActivePlayer(playerNum, false);

            //順位を確定
            m_findPlayer->SetGoalRanking(playerNum, m_nowRank);

            //ゴールした状態にする。
            m_findPlayer->SetFlagGoal(playerNum, true);

            ++nextRank;
            flagAddNowRank = true;

            if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
                m_goalPlayer += 1;
            }
        }
    }

    if (flagAddNowRank == true) {
        //次の順位を設定
        m_nowRank += nextRank;
    }
    
    if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
        if (1 <= m_goalPlayer) {
            //プレイヤーの操作をできないようにする。
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
        //他のプレイヤーが全員ミスをしているとき
        else {
            int count = 0;

            //ミスをしたプレイヤーをカウント
            for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
                if (m_findPlayer->GetStopController(playerNum) == true) {
                    ++count;
                }
            }

            //３人ミスのとき
            if (count == 3) {
                for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
                    if (m_findPlayer->GetStopController(playerNum) == false) {
                        //順位を確定
                        m_findPlayer->SetGoalRanking(playerNum, con::rank_1);
                        m_findPlayer->SetAnimationWin(playerNum);

                        //ゴールした人数を増加
                        ++m_goalPlayer;
                    }

                    m_findPlayer->SetActivePlayer(playerNum, false);
                    //ゴールした状態にする。
                    m_findPlayer->SetFlagGoal(playerNum, true);
                }
            }

            //４人ミスのとき
            if (count == 4) {
                //次のラウンドにいく
                if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
                    //ゴールした人数を増加
                    ++m_goalPlayer;
                    m_allMiss = true;
                }
            }
        }
    }
}

//////////////////////////////
// ラウンド変更の処理
//////////////////////////////

void Stage::NextRound()
{
    if (m_allMiss == false) {
        //プレイヤーのラウンド勝利ポイント
        for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
            if (m_findPlayer->GetGoalRanking(playerNum) == con::rank_1) {
                m_findPlayer->AddRoundPoint(playerNum);
            }
        }
    }
    
    if (m_findPlayer->GetFinishSuddenDeath() == true) {
        return;
    }



    //ステージを作成
    StageCreate();

    //緑１６個
    //青８個
    //黄色８個

    //モデルの非表示
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

    //モデルの描画
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = 0;
        DrawBlock(playerNum);
    }

    //変数の値のリセット
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
// 距離による勝利判定
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
            //勝利条件を満たしている
            WinPlayerDistance(playerNum);
        }
    }
}

bool Stage::CheckPlayerRank1(const int& playerNum)
{
    int otherPlayer[con::PlayerNumberMax - 1] = { 0,0,0 }; //他のプレイヤーの番号を保存

    //他のプレイヤーの番号を取得
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
            //勝利条件を満たしていない
            return false;
        }
    }

    return true;
}

bool Stage::CheckPlayerDistance20Block(const int& playerNum, const int& otherNum)
{
    //自分自身が操作不能の場合、
    if (m_findPlayer->GetStopController(playerNum) == true) {
        return false;
    }

    if (m_findPlayer->GetStopController(otherNum) == false) {
        if (m_playerBlockPosition[playerNum] > m_playerBlockPosition[otherNum]) {
            if (m_playerBlockPosition[playerNum] - m_playerBlockPosition[otherNum] >= 20)
            {
                //20ブロック以上間があいている
                return true;
            }
            else {
                //20ブロック以上間があいていない
                return false;
            }
        }
        else {
            //プレイヤーを越していない
            return false;
        }
    }

    //ミスをしたプレイヤーは無視
    return true;
}

void Stage::WinPlayerDistance(const int& playerNum)
{
    //プレイヤーの操作をできないようにする。
    for (int pNum = con::FIRST_ELEMENT_ARRAY; pNum < con::PlayerNumberMax; pNum++) {
        m_findPlayer->SetActivePlayer(pNum, false);
        //ゴールした状態にする。
        m_findPlayer->SetFlagGoal(pNum, true);
    }

    //順位を確定
    m_findPlayer->SetGoalRanking(playerNum, con::rank_1);
    m_findPlayer->SetAnimationWin(playerNum);

    m_goalPlayer += 1;
}

//////////////////////////////
// 進行度
//////////////////////////////

void Stage::DegreeOfProgress(const int& pNum)
{
    float test2 = static_cast<float>(m_playerBlockPosition[pNum]);

    test2 = test2 / m_MAX_BLOCK;

    m_spritePlayerMark[pNum]->SetPositionX(
        MAX_DEGREE_OF_PROGRESS_POSITION * test2
    );
}