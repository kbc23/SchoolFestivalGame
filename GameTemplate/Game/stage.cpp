#include "stdafx.h"
#include "stage.h"

#include <random>

#include "player.h"



//モデルの読み込みで時間がかかっているので、
//読み込む必要があるモデルの数を減らすことができないか検証してみること。
//※現在は、プロトなのでこのままで続行



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // ファイルパス
    ////////////////////////////////////////////////////////////

    const char* FILE_PATH_TKM_GREEN_BLOCK = "Assets/modelData/green.tkm";
    const char* FILE_PATH_TKM_BLUE_BLOCK = "Assets/modelData/blue.tkm";
    const char* FILE_PATH_TKM_YELLOW_BLOCK = "Assets/modelData/yellow.tkm";

    ////////////////////////////////////////////////////////////
    // ブロックの情報
    ////////////////////////////////////////////////////////////

    const float BLOCK_SIZE = 80.0f;         //ブロックのサイズ

    ////////////////////////////////////////////////////////////
    // 位置情報
    ////////////////////////////////////////////////////////////

    const float BLOCK_POSITION_X[con::playerNumberMax] = {           //ブロックのX座標
        390.0f,                                                             //プレイヤー１
        130.0f,                                                             //プレイヤー２
        -130.0f,                                                            //プレイヤー３
        -390.0f                                                             //プレイヤー４
    };
    const float BLOCK_POSITION_Y = -80.0f;                              //ブロックのY座標
    const float BLOCK_POSITION_Z = -250.0f;                             //ブロックのZ座標
    const Vector3 BLOCK_START_POSITION[con::playerNumberMax] = {     //プレイヤーごとのスタート位置のブロックの座標
        { 390.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z }, 	                //プレイヤー１
        { 130.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z },	                    //プレイヤー２
        { -130.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z },	                //プレイヤー３
        { -390.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z }	                    //プレイヤー４
    };

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    const int TIMER_RESET = 0;                  //タイマーのリセット
    const int TIME_RETURN_OPERATION = 30;       //操作復帰にかかる時間（1.5秒）

    const int TIME_BLUE_BLOCK_ANIMATION = 30;   //青色のブロックに行ったときのアニメーションの時間

    ////////////////////////////////////////////////////////////
    // 入力関係
    ////////////////////////////////////////////////////////////

    const int MOVE_BUTTON_A = 2;		//Aボタンを押したときの移動量
    const int MOVE_BUTTON_B = 1;		//Bボタンを押したときの移動量



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
}



Stage::Stage()
{

}

Stage::~Stage()
{
    for (int playerNum = con::player_1; playerNum < con::playerNumberMax; playerNum++) {
        for (int blockNum = 0; blockNum < m_MAX_BLOCK; blockNum++) {
            DeleteGO(m_modelRender[playerNum][blockNum]);
        }
    }
}

////////////////////////////////////////////////////////////
// Start関数関連
////////////////////////////////////////////////////////////

bool Stage::Start()
{
    //ステージを作成
    StageCreate();

    //緑１６個
    //青８個
    //黄色８個

    //モデルの作成
    for (int playerNum = con::player_1; playerNum < con::playerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
            m_modelGreenBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_FIRST);
            m_modelGreenBlock[playerNum][blockNum]->Init(FILE_PATH_TKM_GREEN_BLOCK);
            m_modelGreenBlock[playerNum][blockNum]->Deactivate();
        }

        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLUE_OR_YELLOW_BLOCK; blockNum++) {
            m_modelBlueBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_FIRST);
            m_modelBlueBlock[playerNum][blockNum]->Init(FILE_PATH_TKM_BLUE_BLOCK);
            m_modelBlueBlock[playerNum][blockNum]->Deactivate();
            m_modelYellowBlock[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_FIRST);
            m_modelYellowBlock[playerNum][blockNum]->Init(FILE_PATH_TKM_YELLOW_BLOCK);
            m_modelYellowBlock[playerNum][blockNum]->Deactivate();
        }
    }

    //モデルの描画
    for (int playerNum = con::player_1; playerNum < con::playerNumberMax; playerNum++) {
        DrawBlock(playerNum);
    }

    m_player = FindGO<Player>(igo::CLASS_NAME_PLAYER);

    return true;
}

void Stage::StageCreate()
{
    //緑は連続で６個まで
    //青の後は緑
    //黄色の後は緑
    //青:70% 黄色:30%

    std::mt19937 mt{ std::random_device{}() };
    std::uniform_int_distribution<int> randomNum(1, 100);

    int continuousGreenBlock = 0;   //緑のブロックが何回連続で出ているか。
    int randomGreenNum = 0;              //乱数を入れる変数
    int randomBlueOrYellowNum = 0;  //青色か黄色をセットするときに使用する乱数を入れる変数
    bool lastTimeBlockBlueOrYellow = false; //前回のブロックが青色か黄色だったか

    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        //最初のブロックは緑
        if (blockNum == con::FIRST_OF_THE_ARRAY) {
            m_stageData[con::player_1][blockNum] = greenBlock;
            ++continuousGreenBlock;
            continue;
        }

        //ゴールの位置のブロックは緑
        if (blockNum == m_MAX_BLOCK - 1) {
            m_stageData[con::player_1][blockNum] = greenBlock;
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
    for (int playerNum = con::player_2; playerNum < con::playerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
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

////////////////////////////////////////////////////////////
// 毎フレームの処理
////////////////////////////////////////////////////////////

void Stage::Update()
{
    //スタート地点に戻る( Debug )
    //if (g_pad[0]->IsTrigger(enButtonX)) {
    //    for (int playerNum = con::player_1; playerNum < con::playerNumberMax; playerNum++) {
    //        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
    //            m_modelRender[playerNum][blockNum]->SetPosition({
    //                BLOCK_START_POSITION[playerNum].x,
    //                BLOCK_START_POSITION[playerNum].y,
    //                BLOCK_START_POSITION[playerNum].z + BLOCK_SIZE * blockNum
    //                });
    //        }
    //        m_playerBlockPosition[playerNum] = 0;
    //    }
    //}
    //to this point( Debug )


    //プレイヤーごとの処理
    for (int playerNum = con::player_1; playerNum < con::playerNumberMax; playerNum++) {
        CheckBlock(playerNum);

        ReturnOperationTimer(playerNum);

        DrawMoveBlock(playerNum);
    }

    //ゴール時の処理
    GoalBlock();
}

void Stage::DrawBlock(const int pNum)
{
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLUE_OR_YELLOW_BLOCK; blockNum++) {
        m_modelBlueBlock[pNum][blockNum]->Deactivate();
        m_modelYellowBlock[pNum][blockNum]->Deactivate();
    }

    //モデルの描画
    int numberOfUsesGreenBlock = 0;
    int numberOfUsesBlueBlock = 0;
    int numberOfUsesYellowBlock = 0;

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
            m_modelBlueBlock[pNum][numberOfUsesBlueBlock]->SetPosition({
              BLOCK_POSITION_X[pNum],
              BLOCK_POSITION_Y,
              BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            m_modelBlueBlock[pNum][numberOfUsesBlueBlock]->Activate();
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
    }

}

void Stage::DrawMoveBlock(const int pNum)
{
    if (m_flagAnimationJump[pNum] == false) {
        return;
    }

    ++m_timerAnimation[pNum];

    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_GREEN_BLOCK; blockNum++) {
        m_modelGreenBlock[pNum][blockNum]->Deactivate();
    }
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLUE_OR_YELLOW_BLOCK; blockNum++) {
        m_modelBlueBlock[pNum][blockNum]->Deactivate();
        m_modelYellowBlock[pNum][blockNum]->Deactivate();
    }


    //モデルの描画
    int numberOfUsesGreenBlock = 0;
    int numberOfUsesBlueBlock = 0;
    int numberOfUsesYellowBlock = 0;

    double moveCorrection = 0;

    int playerBlockPosition = m_playerBlockPosition[pNum] - m_amountOfMovement[pNum];

    //if (m_amountOfMovement[pNum] == MOVE_BUTTON_A) {
    //    moveCorrection = BLOCK_SIZE * MOVE_BUTTON_A / (30 - m_player->GetTimerAnimation(pNum));
    //}
    //else if (m_amountOfMovement[pNum] == MOVE_BUTTON_B) {
    //    moveCorrection = BLOCK_SIZE * MOVE_BUTTON_B / (30 - m_player->GetTimerAnimation(pNum));
    //}


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
                m_modelBlueBlock[pNum][numberOfUsesBlueBlock]->SetPosition({
                  BLOCK_POSITION_X[pNum],
                  BLOCK_POSITION_Y,
                  BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - float(moveCorrection)
                    });
                m_modelBlueBlock[pNum][numberOfUsesBlueBlock]->Activate();
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
        }
    }
    else {
        DrawBlock(pNum);
        m_timerAnimation[pNum] = 0;
        m_flagAnimationJump[pNum] = false;
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
        m_player->SetRotationX(pNum, con::FLOAT_ZERO);
    }
}

//////////////////////////////
// ブロックごとの処理
//////////////////////////////

void Stage::CheckBlock(const int pNum)
{
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
            //モデルを回転させる。
            m_player->SetRotationX(pNum, -90.0f);
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
        //モデルを回転させる。
        m_player->SetRotationX(pNum, -90.0f);
    }

    BlueBlockAnimation(pNum);

    ReturnBlock(pNum);
}

void Stage::BlueBlockAnimation(const int pNum)
{
    ++m_timerAnimationBlueBlock[pNum];

    //前の位置に戻すためにフラグをfalseにする。
    if (m_timerAnimationBlueBlock[pNum] >= TIME_BLUE_BLOCK_ANIMATION) {
        m_flagAnimationBlueBlock[pNum] = false;
        m_timerAnimationBlueBlock[pNum] = TIMER_RESET;
        //プレイヤーを操作できるようにする。
        m_activeOperationVersionBlue[pNum] = true;
        //モデルを元に戻す。
        m_player->SetRotationX(pNum, con::FLOAT_ZERO);
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
    bool addNowRank = false; //プレイヤーの順位に代入する数字が変わるかのフラグ
    int nextRank = con::INIT_ZERO; //次のプレイヤーの順位

    for (int playerNum = con::player_1; playerNum < con::playerNumberMax; playerNum++) {
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
        }
    }

    if (addNowRank == true) {
        //次の順位を設定
        m_nowRank += nextRank;
    }
}