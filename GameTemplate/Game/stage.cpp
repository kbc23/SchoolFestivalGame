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

    const int DRAW_FIRST_BLOCK = -6;
    const int DRAW_LAST_BLOCK = 15;
    
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

    const Vector2 PLAYER_BLOCK_POSITION_FONT_POSITION[con::PlayerNumberMax] = {	//スコアタイムの表示位置
    { -520.0f, -215.0f },										        //プレイヤー１
    { -200.0f, -215.0f },												//プレイヤー２
    { 120.0f, -215.0f },												//プレイヤー３
    { 440.0f, -215.0f }													//プレイヤー４
    };

    const float MAX_DEGREE_OF_PROGRESS_POSITION = 835.0f; //進行度

    const float INIT_SPRITE_BACKGROUND_CLOUD_1 = 0.0f;
    const float INIT_SPRITE_BACKGROUND_CLOUD_2 = 1280.0f;

    const float MOVE_BACKGROUND = 1.0f;

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
    // ( 100 - PROBABILITY_CREATE_BLUE_BLOCK )が黄色ブロックを作成するかの確率

    //////////////////////////////
    // BGMの確率
    //////////////////////////////

    const int MAX_PROBABILITY_BGM = 5; //BGMの数
    const int PROBABILITY_BGM[MAX_PROBABILITY_BGM] = { //BGMごとの確率
        20,20,20,20,20
    };
}



Stage::Stage()
{
    ////////////////////////////////////////////////////////////
    // NewGO
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // モデルのNewGO
    //////////////////////////////

    //ブロックの初期化
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        //緑ブロック
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
            m_modelGreenBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelGreenBlock[playerNum][blockNum]->Init(filePath::tkm::GREEN_BLOCK); //初期化
            m_modelGreenBlock[playerNum][blockNum]->Deactivate(); //非表示
        }
        //黄色ブロック
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
            m_modelYellowBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelYellowBlock[playerNum][blockNum]->Init(filePath::tkm::YELLOW_BLOCK); //初期化
            m_modelYellowBlock[playerNum][blockNum]->Deactivate(); //非表示
        }
        //ゴールブロック
        for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
            m_modelGoalBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
            m_modelGoalBlock[playerNum][blockNum]->Init(filePath::tkm::GOAL_BLOCK); //初期化
            m_modelGoalBlock[playerNum][blockNum]->Deactivate(); //非表示
        }
    }

    //////////////////////////////
    // スプライトのNewGO
    //////////////////////////////

    //背景のNewGO
    //青空
    m_spriteBackgroundSky = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundSky->Init(filePath::dds::BACKGROUND_SKY); //初期化
    m_spriteBackgroundSky->Deactivate(); //非表示
    //雲１
    m_spriteBackgroundCloud_1 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_1->Init(filePath::dds::BACKGROUND_CLOUD); //初期化
    m_spriteBackgroundCloud_1->Deactivate(); //非表示
    //雲２
    m_spriteBackgroundCloud_2 = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
    m_spriteBackgroundCloud_2->Init(filePath::dds::BACKGROUND_CLOUD); //初期化
    m_spriteBackgroundCloud_2->Deactivate(); //非表示


    //進行度のUIのNewGO
    //進行度の線
    m_spriteDegreeOfProgress = NewGO<SpriteRender>(igo::PRIORITY_UI);
    m_spriteDegreeOfProgress->Init(filePath::dds::DEGREE_OF_PROGRESS); //初期化
    m_spriteDegreeOfProgress->Deactivate(); //非表示
    //プレイヤーごとのマーカー
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spritePlayerMark[playerNum]->Init(filePath::dds::PLAYER_MARK[playerNum]); //初期化
        m_spritePlayerMark[playerNum]->Deactivate(); //非表示
    }

    //ラウンドのポイントのUIのNewGO
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
            m_spriteRoundWin[playerNum][roundNum]->Init(filePath::dds::ROUND_WIN[playerNum][roundNum]); //初期化
            m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //非表示
        }
    }

    //////////////////////////////
    // フォントのNewGO
    //////////////////////////////

    //プレイヤーが現在何番目のブロックにいるかの表示のNewGO
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontPlayerBlockPosition[playerNum]->Init(L"", PLAYER_BLOCK_POSITION_FONT_POSITION[playerNum]); //初期化
        m_fontPlayerBlockPosition[playerNum]->SetText(m_playerBlockPosition[playerNum] + 1); //テキスト
        m_fontPlayerBlockPosition[playerNum]->Deactivate(); //非表示
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
// Start関数関連
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
    // モデルの初期化
    ////////////////////////////////////////////////////////////

    //モデルの非表示
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        BlockDeactivate(playerNum);
    }

    //ステージを作成
    StageCreate();

    //モデルの描画
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        DrawBlock(playerNum);
    }

    ////////////////////////////////////////////////////////////
    // スプライトの初期化
    ////////////////////////////////////////////////////////////


    //背景の初期化
    m_spriteBackgroundSky->Activate(); //表示
    m_spriteBackgroundCloud_1->Activate(); //表示
    m_spriteBackgroundCloud_1->SetPositionX(INIT_SPRITE_BACKGROUND_CLOUD_1); //位置
    m_spriteBackgroundCloud_2->Activate(); //表示
    m_spriteBackgroundCloud_2->SetPositionX(INIT_SPRITE_BACKGROUND_CLOUD_2); //位置

    //進行度のUIの初期化
    m_spriteDegreeOfProgress->Activate(); //表示
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->Activate(); //表示
        m_spritePlayerMark[playerNum]->SetPositionX(0.0f); //位置
    }

    //ラウンドのUIの初期化
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //非表示
        }
    }

    ////////////////////////////////////////////////////////////
    // フォントの初期化
    ////////////////////////////////////////////////////////////

    //プレイヤーが現在何番目のブロックにいるかの表示の初期化
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum]->Activate(); //表示
    }

    ////////////////////////////////////////////////////////////
    // BGMのNewGO、初期化
    ////////////////////////////////////////////////////////////

    //再生するBGMの設定
    InitBGM();
    m_bgm->Play(true); ///再生

    ////////////////////////////////////////////////////////////
    // メンバ変数の初期化
    ////////////////////////////////////////////////////////////

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = m_START_BLOCK; //プレイヤーが何個目のブロックにいるか
        m_playerBeforeBlockPosition[playerNum] = m_START_BLOCK; //プレイヤーの前にいたブロックの番号
        m_amountOfMovement[playerNum] = 0;
        m_playerAnimation[playerNum] = greenBlock;
        m_flagAnimationJump[playerNum] = false;//ジャンプアニメーション中か
        m_timerAnimation[playerNum] = 0; //アニメーションのタイマー
    }

    m_nowRank = m_INIT_RANK; //プレイヤーの順位データに渡すデータ
    m_maxPlayer = con::PlayerNumberMax;	//プレイヤーの最大数
    m_goalPlayer = 0; //ゴールしたプレイヤーの数
    m_allMiss = false; //プレイヤー全員がミスをしているか
}

void Stage::Finish()
{
    m_flagProcess = false;

    //////////
    // モデルの非表示
    //////////

    //ブロックの非表示
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        BlockDeactivate(playerNum);
    }

    //////////
    // スプライトの非表示
    //////////

    //背景の非表示
    m_spriteBackgroundSky->Deactivate(); //非表示
    m_spriteBackgroundCloud_1->Deactivate(); //非表示
    m_spriteBackgroundCloud_2->Deactivate(); //非表示

    //進行度のUIの非表示
    m_spriteDegreeOfProgress->Deactivate(); //非表示
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_spritePlayerMark[playerNum]->Deactivate(); //非表示
    }

    //ラウンドのUIの非表示
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            m_spriteRoundWin[playerNum][roundNum]->Deactivate(); //非表示
        }
    }

    //////////
    // フォントの非表示
    //////////

    //プレイヤーが現在何番目のブロックにいるかの表示の非表示
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_fontPlayerBlockPosition[playerNum]->Deactivate(); //非表示
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

    int continuousGreenBlock = 0; //緑のブロックが何回連続で出ているか。
    bool lastTimeBlockBlueOrYellow = false; //前回のブロックが青色か黄色だったか

    //プレイヤー１のを設定
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        //最初のブロックは緑
        if (blockNum == con::FIRST_ELEMENT_ARRAY) {
            m_stageData[con::player_1][blockNum] = greenBlock; //緑ブロック
            ++continuousGreenBlock; //緑ブロックの連続回数を増加
            continue;
        }

        //ゴールの位置のブロックは緑
        if (blockNum == m_MAX_BLOCK - 1) {
            m_stageData[con::player_1][blockNum] = goalBlock; //ゴールブロック
            continue;
        }

        //緑ブロックが６回連続で作成されている場合
        if (continuousGreenBlock >= UPPER_LIMIT_CONTINUOUS_GREEN_BLOCK) {
            //青ブロック、黄色ブロックのセット
            CreateBlueOrYellow(blockNum, randomNum(mt));
            continuousGreenBlock = 0; //緑ブロックの連続回数をリセット
            lastTimeBlockBlueOrYellow = true; //前回のブロックが青色か黄色だった状態にする
            continue;
        }

        //前回が青色か黄色のブロックだった場合
        if (lastTimeBlockBlueOrYellow == true) {
            m_stageData[con::player_1][blockNum] = greenBlock; //緑ブロック
            ++continuousGreenBlock; //緑ブロックの連続回数を増加
            lastTimeBlockBlueOrYellow = false; //前回のブロックが青色か黄色ではなかった状態にする
            continue;
        }

        //緑ブロックを作成するか
        if (CreateGreen(continuousGreenBlock, randomNum(mt)) == true) {
            //緑ブロックを作成
            m_stageData[con::player_1][blockNum] = greenBlock; //緑ブロック
            ++continuousGreenBlock; //緑ブロックの連続回数を増加
            continue;
        }
        else {
            //青ブロック、黄色ブロックを作成
            CreateBlueOrYellow(blockNum, randomNum(mt));
            continuousGreenBlock = 0; //緑ブロックの連続回数をリセット
            lastTimeBlockBlueOrYellow = true; //前回のブロックが青色か黄色ではなかった状態にする
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

    int randomNum = random(mt); //乱数
    int checkRandom = 0; //BGMごとの確率の確認にしようする変数

    for (int check = con::FIRST_ELEMENT_ARRAY; check < MAX_PROBABILITY_BGM; check++) {
        checkRandom += PROBABILITY_BGM[check]; //BGMごとの確率を足す

        //このBGMにする
        if (randomNum <= checkRandom) {
            m_bgm = NewGO<SoundBGM>(igo::PRIORITY_CLASS);
            m_bgm->Init(filePath::bgm::STAGE[check]); //初期化
            return;
        }
    }
}

void Stage::BlockDeactivate(const int playerNum)
{
    //緑ブロック
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[playerNum][blockNum]->Deactivate(); //非表示
    }
    //黄色ブロック
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_YELLOW_BLOCK; blockNum++) {
        m_modelYellowBlock[playerNum][blockNum]->Deactivate(); //非表示
    }
    //ゴールブロック
    for (int blockNum = con::FIRST_ELEMENT_ARRAY; blockNum < m_MAX_GOAL_BLOCK; blockNum++) {
        m_modelGoalBlock[playerNum][blockNum]->Deactivate(); //非表示
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
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        //ブロックの描画処理
        DrawMoveBlock(playerNum);

        //プレイヤーの進んだブロック数の表示
        DrawFontPlayerBlockPosition(playerNum);

        //進行度の描画
        DegreeOfProgress(playerNum);
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

void Stage::DrawBlock(const int playerNum)
{
    //モデルの非表示
    BlockDeactivate(playerNum);

    //モデルの描画
    int numberOfUsesGreenBlock = 0;
    int numberOfUsesYellowBlock = 0;
    const int numberOfUsesGoalBlock = 0;

    for (int blockNum = DRAW_FIRST_BLOCK; blockNum < DRAW_LAST_BLOCK; blockNum++) {
        //スタート地点より前のブロックを描画しない
        if (m_playerBlockPosition[playerNum] + blockNum < 0) {
            continue;
        }
        //ゴール地点より後のブロックを描画しない
        if (m_playerBlockPosition[playerNum] + blockNum >= m_MAX_BLOCK) {
            break;
        }

        //緑ブロック
        if (m_stageData[playerNum][m_playerBlockPosition[playerNum] + blockNum] == greenBlock) {
            m_modelGreenBlock[playerNum][numberOfUsesGreenBlock]->SetPosition({ //位置
            BLOCK_POSITION_X[playerNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelGreenBlock[playerNum][numberOfUsesGreenBlock]->Activate(); //表示
            ++numberOfUsesGreenBlock;
        }
        //青ブロック
        else if (m_stageData[playerNum][m_playerBlockPosition[playerNum] + blockNum] == blueBlock) {
            // do nothing
        }
        //黄色ブロック
        else if (m_stageData[playerNum][m_playerBlockPosition[playerNum] + blockNum] == yellowBlock) {
            m_modelYellowBlock[playerNum][numberOfUsesYellowBlock]->SetPosition({ //位置
               BLOCK_POSITION_X[playerNum],
               BLOCK_POSITION_Y,
               BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelYellowBlock[playerNum][numberOfUsesYellowBlock]->Activate(); //表示
            ++numberOfUsesYellowBlock;
        }
        //ゴールブロック
        else if (m_stageData[playerNum][m_playerBlockPosition[playerNum] + blockNum] == goalBlock) {
            m_modelGoalBlock[playerNum][numberOfUsesGoalBlock]->SetPosition({ //位置
               BLOCK_POSITION_X[playerNum],
               BLOCK_POSITION_Y,
               BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelGoalBlock[playerNum][numberOfUsesGoalBlock]->Activate(); //表示
        }
    }
}

void Stage::DrawMoveBlock(const int playerNum)
{
    //プレイヤーがジャンプアニメーション中ではないとき
    if (m_flagAnimationJump[playerNum] == false) {
        DrawBlock(playerNum);

        return;
    }

    ++m_timerAnimation[playerNum];

    //モデルを全部非表示にする。
    BlockDeactivate(playerNum);


    //モデルの描画
    int numberOfUsesGreenBlock = 0; //緑ブロックの描画数
    int numberOfUsesYellowBlock = 0; //バナナブロックの描画数
    const int numberOfUsesGoalBlock = 0; //ゴールブロックの描画数

    double moveCorrection = 0; //モデルの移動量

    int playerBlockPosition = m_playerBlockPosition[playerNum] - m_amountOfMovement[playerNum]; //モデルの現在の位置

    if (m_timerAnimation[playerNum] < 30) {
        double test = BLOCK_SIZE * m_amountOfMovement[playerNum] / 30;

        moveCorrection = test * m_timerAnimation[playerNum];

        //ブロックの描画
        for (int blockNum = DRAW_FIRST_BLOCK; blockNum < DRAW_LAST_BLOCK; blockNum++) {
            //スタート地点より前のブロックを描画しない
            if (playerBlockPosition + blockNum < 0) {
                continue;
            }
            //ゴール地点より後のブロックを描画しない
            if (playerBlockPosition + blockNum >= m_MAX_BLOCK) {
                break;
            }
            
            //緑ブロック
            if (m_stageData[playerNum][playerBlockPosition + blockNum] == greenBlock) {
                m_modelGreenBlock[playerNum][numberOfUsesGreenBlock]->SetPosition({ //位置
                BLOCK_POSITION_X[playerNum],
                BLOCK_POSITION_Y,
                BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - static_cast<float>(moveCorrection)
                    });
                m_modelGreenBlock[playerNum][numberOfUsesGreenBlock]->Activate(); //表示
                ++numberOfUsesGreenBlock;
            }
            //青ブロック
            else if (m_stageData[playerNum][playerBlockPosition + blockNum] == blueBlock) {
                // do nothing
            }
            //黄色ブロック
            else if (m_stageData[playerNum][playerBlockPosition + blockNum] == yellowBlock) {
                m_modelYellowBlock[playerNum][numberOfUsesYellowBlock]->SetPosition({ //位置
                   BLOCK_POSITION_X[playerNum],
                   BLOCK_POSITION_Y,
                   BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - static_cast<float>(moveCorrection)
                    });
                m_modelYellowBlock[playerNum][numberOfUsesYellowBlock]->Activate(); //表示
                ++numberOfUsesYellowBlock;
            }
            //ゴールブロック
            else if (m_stageData[playerNum][playerBlockPosition + blockNum] == goalBlock) {
                m_modelGoalBlock[playerNum][numberOfUsesGoalBlock]->SetPosition({ //位置
                   BLOCK_POSITION_X[playerNum],
                   BLOCK_POSITION_Y,
                   BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - static_cast<float>(moveCorrection)
                    });
                m_modelGoalBlock[playerNum][numberOfUsesGoalBlock]->Activate(); //表示
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
    //プレイヤーのブロック位置の情報を更新
    m_fontPlayerBlockPosition[playerNum]->SetText(m_playerBlockPosition[playerNum] + 1);
}

void Stage::DrawBackground()
{
    //背景を動かす
    m_spriteBackgroundCloud_1->SetPositionX(m_spriteBackgroundCloud_1->GetPosition().x - MOVE_BACKGROUND);
    m_spriteBackgroundCloud_2->SetPositionX(m_spriteBackgroundCloud_2->GetPosition().x - MOVE_BACKGROUND);

    //一定量動いたら最初の状態に戻す
    if (m_spriteBackgroundCloud_1->GetPosition().x <= -INIT_SPRITE_BACKGROUND_CLOUD_2) {
        m_spriteBackgroundCloud_1->SetPositionX(INIT_SPRITE_BACKGROUND_CLOUD_1);
        m_spriteBackgroundCloud_2->SetPositionX(INIT_SPRITE_BACKGROUND_CLOUD_2);
    }
}

void Stage::DrawRoundWin()
{
    //サドンデスモードでないとき
    if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == false) {
        return;
    }

    //取得ラウンド数のUIの表示
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int roundNum = con::FIRST_ELEMENT_ARRAY; roundNum < m_MAX_RAUND_WIN; roundNum++) {
            if (roundNum < m_findPlayer->GetRoundPoint(playerNum)) {
                m_spriteRoundWin[playerNum][roundNum]->Activate(); //表示
            }
        }
    }
}

//////////////////////////////
// ブロックの移動処理
//////////////////////////////

bool Stage::MoveBlock(const int playerNum, const int moveNum)
{
    //操作不可能か
    if (m_findPlayer->GetStopController(playerNum) == true) {
        return false;
    }

    //移動量を保存
    m_amountOfMovement[playerNum] = moveNum;

    //プレイヤーの現在の位置を保存
    m_playerBeforeBlockPosition[playerNum] = m_playerBlockPosition[playerNum];

    //プレイヤーが何個目のブロックにいるかの情報を更新
    m_playerBlockPosition[playerNum] += moveNum;

    //最大数を超えていたら、最大数 - 1に合わせる。
    if (m_playerBlockPosition[playerNum] >= m_MAX_BLOCK) {
        m_playerBlockPosition[playerNum] = m_MAX_BLOCK - 1;
        m_amountOfMovement[playerNum] = con::MOVE_1;
    }

    //ブロックの描画
    m_flagAnimationJump[playerNum] = true;

    return true;
}

//////////////////////////////
// ブロックごとの処理
//////////////////////////////

void Stage::CheckBlock(const int playerNum)
{
    //ブロックごとに処理
    //緑ブロック
    if (m_stageData[playerNum][m_playerBlockPosition[playerNum]] == greenBlock) {
        // do nothing
    }
    //青ブロック
    else if (m_stageData[playerNum][m_playerBlockPosition[playerNum]] == blueBlock) {
        BlueBlock(playerNum);
    }
    //黄色ブロック
    else if (m_stageData[playerNum][m_playerBlockPosition[playerNum]] == yellowBlock) {
        YellowBlock(playerNum);
    }
}

//////////
// 青色のブロック
//////////

void Stage::BlueBlock(const int playerNum)
{
    //操作不能にする
    m_findPlayer->SetStopController(playerNum, true);

    //アニメーションを設定
    m_findPlayer->SetNowAnimationBlock(playerNum, con::blueBlock);

    //CPUコントローラーに青ブロックでミスをしたことを伝える
    m_findCPUPlayerController->SetFlagBlueBlockMiss(playerNum, true);
}

//////////
// 黄色のブロック
//////////

void Stage::YellowBlock(const int playerNum)
{
    //黄色ブロックを緑ブロックに変更
    m_stageData[playerNum][m_playerBlockPosition[playerNum]] = greenBlock;
    DrawBlock(playerNum); //描画

    //操作不能にする
    m_findPlayer->SetStopController(playerNum, true);

    //アニメーションを設定
    m_findPlayer->SetNowAnimationBlock(playerNum, con::yellowBlock);
}

//////////
// その他
//////////

void Stage::ReturnBlock(const int playerNum)
{
    //プレイヤーの現在位置を前の位置に戻す。
    m_playerBlockPosition[playerNum] = m_playerBeforeBlockPosition[playerNum];

    //モデルの位置を更新
    DrawBlock(playerNum);
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

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        //プレイヤーの順位を確定
        if (m_findPlayer->GetActivePlayer(playerNum) == true && m_playerBlockPosition[playerNum] == m_MAX_BLOCK - 1) {
            //プレイヤーの操作をできないようにする。
            m_findPlayer->SetActivePlayer(playerNum, false);

            //順位を確定
            m_findPlayer->SetGoalRanking(playerNum, m_nowRank);

            //ゴールした状態にする。
            m_findPlayer->SetFlagGoal(playerNum, true);

            //次のプレイヤーの順位のために増加
            ++nextRank;
            flagAddNowRank = true;

            //サドンデスモードか
            if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
                m_goalPlayer += 1;
            }
        }
    }

    //次のプレイヤーの順位が変わる場合
    if (flagAddNowRank == true) {
        //次の順位を設定
        m_nowRank += nextRank;
    }
    
    //ミスによる勝利条件の確認
    MissRoundWin();
}

void Stage::MissRoundWin()
{
    //サドンデスモードか
    if (false == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
        return;
    }

    //ゴールしたプレイヤーがいるか
    if (1 <= m_goalPlayer) {
        //プレイヤーの操作をできないようにする。
        for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
            m_findPlayer->SetActivePlayer(playerNum, false);
        }

        //次のラウンドに移行する
        m_findSuddenDeathMode->SetFlagNextRound(true);
        m_goalPlayer = 0;
    }
    //ゴールしたプレイヤーがいないとき
    else {
        int count = 0; //ミスしたプレイヤーの人数

        //ミスをしたプレイヤーをカウント
        for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
            if (m_findPlayer->GetStopController(playerNum) == true) {
                ++count;
            }
        }

        //３人ミスのとき
        if (count == 3) {
            for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
                if (m_findPlayer->GetStopController(playerNum) == false) {
                    //順位を確定
                    m_findPlayer->SetGoalRanking(playerNum, con::rank_1);
                    m_findPlayer->SetAnimationWin(playerNum); //アニメーション

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

//////////////////////////////
// ラウンド変更の処理
//////////////////////////////

void Stage::NextRound()
{
    if (m_allMiss == false) {
        //プレイヤーのラウンド勝利ポイント
        for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
            //１位のプレイヤー
            if (m_findPlayer->GetGoalRanking(playerNum) == con::rank_1) {
                //ラウンドを取得
                m_findPlayer->AddRoundPoint(playerNum);
            }
        }
    }
    
    //ステージを作成
    StageCreate();

    //モデルの非表示
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        BlockDeactivate(playerNum);
    }

    //モデルの描画
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_playerBlockPosition[playerNum] = 0; //プレイヤーの位置を最初のブロックの位置にする
        DrawBlock(playerNum);
    }

    //変数の値のリセット
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
// 距離による勝利判定
//////////////////////////////

void Stage::CheckPlayerDistance()
{
    //サドンデスモードか
    if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == false) {
        return;
    }

    //ゴールしたプレイヤーが１人以上
    if (1 <= m_goalPlayer) {
        return;
    }

    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        //距離による勝利条件を満たしているか
        if (CheckPlayerRank1(playerNum) == true) {
            //勝利条件を満たしている
            WinPlayerDistance(playerNum);
        }
    }
}

bool Stage::CheckPlayerRank1(const int playerNum)
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


    for (int otherNum = con::FIRST_ELEMENT_ARRAY; otherNum < con::PlayerNumberMax - 1; otherNum++) {
        //プレイヤーごとに距離を比べる
        if (CheckPlayerDistance20Block(playerNum, otherPlayer[otherNum]) == false) {
            //勝利条件を満たしていない
            return false;
        }
    }

    return true;
}

bool Stage::CheckPlayerDistance20Block(const int playerNum, const int otherNum)
{
    //自分自身が操作不能のとき
    if (m_findPlayer->GetStopController(playerNum) == true) {
        return false;
    }

    //自分自身が操作不能でないとき
    if (m_findPlayer->GetStopController(otherNum) == false) {
        //対象のプレイヤーとの距離を計算する
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

void Stage::WinPlayerDistance(const int playerNum)
{
    //プレイヤーの操作をできないようにする。
    for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
        m_findPlayer->SetActivePlayer(playerNum, false);
        //ゴールした状態にする。
        m_findPlayer->SetFlagGoal(playerNum, true);
    }

    //順位を確定
    m_findPlayer->SetGoalRanking(playerNum, con::rank_1);
    m_findPlayer->SetAnimationWin(playerNum); //アニメーション
    
    //ゴールしたプレイヤーの人数を増やす
    m_goalPlayer += 1;
}

//////////////////////////////
// 進行度
//////////////////////////////

void Stage::DegreeOfProgress(const int playerNum)
{
    float moveSprite = static_cast<float>(m_playerBlockPosition[playerNum]);

    moveSprite = moveSprite / m_MAX_BLOCK;

    m_spritePlayerMark[playerNum]->SetPositionX(
        MAX_DEGREE_OF_PROGRESS_POSITION * moveSprite
    );
}