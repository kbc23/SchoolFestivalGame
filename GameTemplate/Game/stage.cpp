#include "stdafx.h"
#include "stage.h"

#include <random>

#include "player.h"
#include "score.h"
#include "main_processing.h"
#include "rule1.h"
#include "EnemyAI.h"
#include "pause.h"


//モデルの読み込みで時間がかかっているので、
//読み込む必要があるモデルの数を減らすことができないか検証してみること。
//※現在は、プロトなのでこのままで続行

//メモ
//バナナでこけたときに、バナナが飛ぶようにした方がいいとのこと（重要度：中）



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

    const int MINIMUM_RANDOM_NUMBER = 1;  
    const int MAXIMUM_RANDOM_NUMBER = 100;

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
// Start関数関連
////////////////////////////////////////////////////////////

bool Stage::Start()
{
    //緑１６個
    //青８個
    //黄色８個

    const char* kari1 = "green";
    const char* kari2 = "yellow";
    const char* kari3 = "goal";



    //モデルの作成
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


    //背景の描画
    m_spriteBackgroundSky = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundSky->Init(filePath::dds::BACKGROUND_SKY);
    m_spriteBackgroundSky->Deactivate();
    m_spriteBackgroundCloud_1 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_1->Init(filePath::dds::BACKGROUND_CLOUD);
    m_spriteBackgroundCloud_1->Deactivate();
    m_spriteBackgroundCloud_2 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_2->Init(filePath::dds::BACKGROUND_CLOUD);
    m_spriteBackgroundCloud_2->Deactivate();

    //進行度のUIを作成
    m_spriteDegreeOfProgress = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteDegreeOfProgress->Init(filePath::dds::DEGREE_OF_PROGRESS);
    m_spriteDegreeOfProgress->Deactivate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spritePlayerMark[playerNum]->Init(filePath::dds::PLAYER_MARK[playerNum]);
        m_spritePlayerMark[playerNum]->Deactivate();
    }

    //BGMの再生
    //m_bgm = NewGO<SoundBGM>(0);

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontPlayerBlockPosition[playerNum]->Init(L"", PLAYER_BLOCK_POSITION_FONT_POSITION[playerNum]);
        m_fontPlayerBlockPosition[playerNum]->Deactivate();
    }

    //ラウンドのUI
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

    m_nowRank = m_INIT_RANK;            //プレイヤーの順位データに渡すデータ

    Playermember = 0;

    m_maxPlayer = con::PlayerNumberMax;	//プレイヤーの最大数
    n = 0;          //ゴールしたプレイヤーの数
    m = 0;          //次のラウンドに移るのに一瞬で行かないための待ち時間
    m_allMiss = false;     //プレイヤー全員がミスをしているか

    j = 0;          //一番進んでいる人のブロック数
    t = 0;          //2番目に進んでいる人のブロック数



    //ステージを作成
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

    //モデルの描画
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        DrawBlock(playerNum);
    }

    //背景の描画
    m_spriteBackgroundSky->Activate();
    m_spriteBackgroundCloud_1->Activate();
    m_spriteBackgroundCloud_2->Activate();
    m_spriteBackgroundCloud_1->SetPositionX(0.0f);
    m_spriteBackgroundCloud_2->SetPositionX(1280.0f);

    //進行度のUIを作成
    m_spriteDegreeOfProgress->Activate();
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->SetPositionX(0.0f);
        m_spritePlayerMark[playerNum]->Activate();
    }

    //BGMの再生
    InitBGM();
    m_bgm->Play(true);

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum]->SetText(m_playerBlockPosition[playerNum] + 1);
        m_fontPlayerBlockPosition[playerNum]->Activate();
    }

    //ラウンドのUI
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

    //ラウンドのUI
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
    //緑は連続で６個まで
    //青の後は緑
    //黄色の後は緑
    //青:70% 黄色:30%

    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<int> randomNum(MINIMUM_RANDOM_NUMBER, MAXIMUM_RANDOM_NUMBER);

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
// 毎フレームの処理
////////////////////////////////////////////////////////////

void Stage::Update()
{
    if (m_flagProcessing == false) {
        return;
    }

    //プレイヤーごとの処理
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {

        CheckBlock(playerNum);


        if (stop == false) {    //黄色に乗った時にずっと操作不能にするかどうか
            ReturnOperationTimer(playerNum);
        }

        DrawMoveBlock(playerNum);

        DrawFontPlayerBlockPosition(playerNum);

        //進行度の描画
        DegreeOfProgress(playerNum);

        if (rule1NewGO == true) {
            if (m_activeOperationVersionBlue[playerNum] == false && m_player->GetModelIsActive(playerNum) == true) {
                SuddenDeathBlueBlockAnimation(playerNum);
            }
        }
    }

    //背景の描画
    DrawBackground();

    //ラウンド勝利数の描画
    DrawRoundWin();

    //ゴール時の処理
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

    //モデルを全部非表示にする。
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


    //モデルの描画
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

    //ラウンドのUI
    for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = 0; roundNum < 3; roundNum++) {
            if(roundNum < m_player->GetRoundPoint(playerNum))
            m_spriteRoundWin[playerNum][roundNum]->Activate();
        }
    }
}

//////////////////////////////
// ブロックの移動処理
//////////////////////////////

bool Stage::MoveBlock(const int pNum, const int moveNum)
{
  
    if (m_activeOperation[pNum] == false || m_activeOperationVersionBlue[pNum] == false) {
        return false;
    }

    //移動量を保存
    m_amountOfMovement[pNum] = moveNum;

    //操作不可状態に対する耐性を削除
    m_resistanceImpossibleOperation[pNum] = false;

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
// タイマー
//////////////////////////////

void Stage::ReturnOperationTimer(const int pNum)
{
    if (m_activeOperation[pNum] == true) {
        return;
    }

    ++m_timerReturnOperation[pNum];

    //操作可能にする。
    if (m_timerReturnOperation[pNum] >= TIME_RETURN_OPERATION) {
        m_activeOperation[pNum] = true;
        m_timerReturnOperation[pNum] = TIMER_RESET;
        m_resistanceImpossibleOperation[pNum] = true;

        //モデルの回転を元に戻す。
        m_player->SetAnimationIdle(pNum);
    }
}

//////////////////////////////
// ブロックごとの処理
//////////////////////////////

void Stage::CheckBlock(const int pNum)
{
    m_blueMiss[pNum] = false;
   
   
    //自キャラがいるブロックによって処理をおこなう。

    //ジャンプアニメーション中は処理をおこなわない。
    if (m_player->GetmFlagAnimationJump(pNum) == true) {
        return;
    }

    //ブロックごとに処理
    if (m_stageData[pNum][m_playerBlockPosition[pNum]] == greenBlock) {

    }
    else if (m_stageData[pNum][m_playerBlockPosition[pNum]] == blueBlock) {
        BlueBlock(pNum);
    }
    else if (m_stageData[pNum][m_playerBlockPosition[pNum]] == yellowBlock) {
        if (m_resistanceImpossibleOperation[pNum] == false) {
            m_activeOperation[pNum] = false;
            //黄色ブロックを緑ブロックに変更
            m_stageData[pNum][m_playerBlockPosition[pNum]] = greenBlock;
            DrawBlock(pNum);
            //こけたアニメーションを再生
            m_player->SetAnimationSrip(pNum);
        }
    }
}

//////////
// 青色のブロック
//////////

void Stage::BlueBlock(const int pNum)
{
    if (m_flagAnimationBlueBlock[pNum] == false) {
        m_flagAnimationBlueBlock[pNum] = true;
        m_activeOperationVersionBlue[pNum] = false;
        //溺れているアニメーションを再生
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

    //前の位置に戻すためにフラグをfalseにする。
    if (m_timerAnimationBlueBlock[pNum] >= TIME_BLUE_BLOCK_ANIMATION) {
        m_flagAnimationBlueBlock[pNum] = false;
        m_timerAnimationBlueBlock[pNum] = TIMER_RESET;
        //プレイヤーを操作できるようにする。
        m_activeOperationVersionBlue[pNum] = true;
        //モデルを元に戻す。
        m_player->ResetPositionY(pNum);
        m_player->SetAnimationIdle(pNum);
    }
}

void Stage::SuddenDeathBlueBlockAnimation(const int& pNum)
{
    ++m_timerAnimationBlueBlock[pNum];
    m_player->DownPositionY(pNum, 30.0f);

    //プレイヤーを非表示にする。
    if (m_timerAnimationBlueBlock[pNum] >= TIME_BLUE_BLOCK_ANIMATION) {
        m_player->PlayerModelDeactivate(pNum);
    }
}

void Stage::ReturnBlock(const int pNum)
{
    if (m_flagAnimationBlueBlock[pNum] == true) {
        return;
    }

    //プレイヤーの現在位置を前の位置に戻す。
    m_playerBlockPosition[pNum] = m_playerBeforeBlockPosition[pNum];

    //モデルの位置を更新
    //モデルの描画
    DrawBlock(pNum);
}

//////////////////////////////
// ゴール時の処理
//////////////////////////////

void Stage::GoalBlock()
{
    //仮設置
    if (m_player->GetFinishSuddenDeath() == true) {
        return;
    }

    bool addNowRank = false; //プレイヤーの順位に代入する数字が変わるかのフラグ
    int nextRank = con::INIT_ZERO; //次のプレイヤーの順位

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        //プレイヤーの順位を確定
        if (m_player->GetActivePlayer(playerNum) == true && m_playerBlockPosition[playerNum] == m_MAX_BLOCK - 1) {
            //プレイヤーの操作をできないようにする。
            m_player->SetActivePlayer(playerNum, false);

            //順位を確定
            m_player->SetGoalRanking(playerNum, m_nowRank);

            //ゴールした状態にする。
            m_player->SetFlagGoal(playerNum, true);

            ++nextRank;
            addNowRank = true;

            if (rule1NewGO == true) {
                n += 1;
            }
        }
    }

    if (addNowRank == true) {
        //次の順位を設定
        m_nowRank += nextRank;
    }
    
    if (rule1NewGO == true) {
        if (1 <= n) {
            //プレイヤーの操作をできないようにする。
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
        //他のプレイヤーが全員ミスをしているとき
        else {
            int count = 0;

            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                if (m_activeOperation[playerNum] == false || m_activeOperationVersionBlue[playerNum] == false) {
                    ++count;
                }
            }

            //３人ミスのとき
            if (count == 3) {
                for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                    if (m_activeOperation[playerNum] == true && m_activeOperationVersionBlue[playerNum] == true) {
                        //順位を確定
                        m_player->SetGoalRanking(playerNum, 1);
                        m_player->SetAnimationWin(playerNum);

                        if (rule1NewGO == true) {
                            n += 1;
                        }
                    }

                    m_player->SetActivePlayer(playerNum, false);
                    //ゴールした状態にする。
                    m_player->SetFlagGoal(playerNum, true);
                }
            }

            //４人ミスのとき
            if (count == 4) {
                //次のラウンドにいく
                if (rule1NewGO == true) {
                    n += 1;
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
        for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
            if (m_player->GetGoalRanking(playerNum) == 1) {
                m_player->AddRoundPoint(playerNum);
            }
        }
    }
    
    if (m_player->GetFinishSuddenDeath() == true) {
        return;
    }



    //ステージを作成
    StageCreate();

    //緑１６個
    //青８個
    //黄色８個

    //モデルの作成
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

    //モデルの描画
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = 0;
        DrawBlock(playerNum);
    }

    //if (m_bgm->IsPlaying()) {
    //    m_bgm->Stop();
    //}

    //BGMの再生
    //m_bgm->Play(true);

    /*for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        m_player->SetFlagGoal(playerNum, false);
    }*/

    //変数の値のリセット
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

    //別のクラスのNextRound()を呼び出す。
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
        //player_1の判定
        if ((m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_2] >= 20 ||
            m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_2] <= -20) &&
            (m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_3] >= 20 ||
                m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_3] <= -20) &&
            (m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_4] >= 20 ||
                m_playerBlockPosition[con::player_1] - m_playerBlockPosition[con::player_4] <= -20))
        {
            //プレイヤーの操作をできないようにする。
            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                m_player->SetActivePlayer(playerNum, false);
                //ゴールした状態にする。
                m_player->SetFlagGoal(playerNum, true);
            }

            //順位を確定
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
        //player_2の判定
        if ((m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_1] >= 20 ||
            m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_1] <= -20) &&
            (m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_3] >= 20 ||
                m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_3] <= -20) &&
            (m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_4] >= 20 ||
                m_playerBlockPosition[con::player_2] - m_playerBlockPosition[con::player_4] <= -20))
        {
            //プレイヤーの操作をできないようにする。
            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                m_player->SetActivePlayer(playerNum, false);
                //ゴールした状態にする。
                m_player->SetFlagGoal(playerNum, true);
            }

            //順位を確定
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
        //player_3の判定
        if ((m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_1] >= 20 ||
            m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_1] <= -20) &&
            (m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_2] >= 20 ||
                m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_2] <= -20) &&
            (m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_4] >= 20 ||
                m_playerBlockPosition[con::player_3] - m_playerBlockPosition[con::player_4] <= -20))
        {
            //プレイヤーの操作をできないようにする。
            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                m_player->SetActivePlayer(playerNum, false);
                //ゴールした状態にする。
                m_player->SetFlagGoal(playerNum, true);
            }

            //順位を確定
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
        //player_4の判定
        if ((m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_1] >= 20 ||
            m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_1] <= -20) &&
            (m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_2] >= 20 ||
                m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_2] <= -20) &&
            (m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_3] >= 20 ||
                m_playerBlockPosition[con::player_4] - m_playerBlockPosition[con::player_3] <= -20))
        {
            //プレイヤーの操作をできないようにする。
            for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
                m_player->SetActivePlayer(playerNum, false);
                //ゴールした状態にする。
                m_player->SetFlagGoal(playerNum, true);
            }

            //順位を確定
            m_player->SetGoalRanking(con::player_4, 1);
            m_player->SetAnimationWin(con::player_4);

            if (rule1NewGO == true) {
                n += 1;
            }
        }
    }
}

//////////////////////////////
// 進行度
//////////////////////////////

void Stage::DegreeOfProgress(const int& pNum)
{
    float test2 = m_playerBlockPosition[pNum];

    test2 = test2 / m_MAX_BLOCK;

    m_spritePlayerMark[pNum]->SetPositionX(
        MAX_DEGREE_OF_PROGRESS_POSITION * test2
    );
}