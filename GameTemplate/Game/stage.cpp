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

    ////////////////////////////////////////////////////////////
    // タイマー関連
    ////////////////////////////////////////////////////////////

    const int TIMER_RESET = 0;                  //タイマーのリセット
    const int TIME_RETURN_OPERATION = 30;       //操作復帰にかかる時間（1.5秒）

    const int TIME_BLUE_BLOCK_ANIMATION = 30;   //青色のブロックに行ったときのアニメーションの時間
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
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
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
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        DrawBlock(playerNum);
    }



    //int numberOfUsesGreenBlock = 0;
    //int numberOfUsesBlueBlock = 0;
    //int numberOfUsesYellowBlock = 0;

    //for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
    //    for (int blockNum = m_playerBlockPosition[playerNum]; blockNum < 14; blockNum++) {
    //        if (m_stageData[playerNum][blockNum] == greenBlock) {
    //            m_modelGreenBlock[playerNum][numberOfUsesGreenBlock]->SetPosition({
    //            BLOCK_POSITION_X[playerNum],
    //            BLOCK_POSITION_Y,
    //            BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
    //                });
    //            ++numberOfUsesGreenBlock;
    //        }
    //        else if (m_stageData[playerNum][blockNum] == blueBlock) {
    //            m_modelBlueBlock[playerNum][numberOfUsesBlueBlock]->SetPosition({
    //              BLOCK_POSITION_X[playerNum],
    //              BLOCK_POSITION_Y,
    //              BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
    //                });
    //            ++numberOfUsesBlueBlock;
    //        }
    //        else if (m_stageData[playerNum][blockNum] == yellowBlock) {
    //            m_modelYellowBlock[playerNum][numberOfUsesYellowBlock]->SetPosition({
    //               BLOCK_POSITION_X[playerNum],
    //               BLOCK_POSITION_Y,
    //               BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
    //                });
    //            ++numberOfUsesYellowBlock;
    //        }




            /*switch (m_stageData[playerNum][blockNum]) {
            case greenBlock:
                m_modelGreenBlock[playerNum][numberOfUsesGreenBlock]->SetPosition({
                    BLOCK_POSITION_X[playerNum],
                    BLOCK_POSITION_Y,
                    BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
                ++numberOfUsesGreenBlock;

                break;
            case blueBlock:
                m_modelBlueBlock[playerNum][numberOfUsesBlueBlock]->SetPosition({
                    BLOCK_POSITION_X[playerNum],
                    BLOCK_POSITION_Y,
                    BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
                ++numberOfUsesBlueBlock;

                break;
            case yellowBlock:
                m_modelYellowBlock[playerNum][numberOfUsesYellowBlock]->SetPosition({
                    BLOCK_POSITION_X[playerNum],
                    BLOCK_POSITION_Y,
                    BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
                ++numberOfUsesYellowBlock;

                break;
            default:

                break;
            }*/
    //    }
    //    numberOfUsesGreenBlock = 0;
    //    numberOfUsesBlueBlock = 0;
    //    numberOfUsesYellowBlock = 0;
    //}



    //old

    //for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
    //    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
    //        m_modelRender[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_FIRST);

    //        //ステージの情報にてモデルを変更
    //        if (m_stageData[playerNum][blockNum] == greenBlock) {
    //            m_modelRender[playerNum][blockNum]->Init(FILE_PATH_TKM_GREEN_BLOCK);
    //        }
    //        else if (m_stageData[playerNum][blockNum] == blueBlock) {
    //            m_modelRender[playerNum][blockNum]->Init(FILE_PATH_TKM_BLUE_BLOCK);
    //        }
    //        else if (m_stageData[playerNum][blockNum] == yellowBlock) {
    //            m_modelRender[playerNum][blockNum]->Init(FILE_PATH_TKM_YELLOW_BLOCK);
    //        }

    //        m_modelRender[playerNum][blockNum]->SetPosition({
    //            BLOCK_START_POSITION[playerNum].x,
    //            BLOCK_START_POSITION[playerNum].y,
    //            BLOCK_START_POSITION[playerNum].z + BLOCK_SIZE * blockNum
    //        });
    //    }
    //}

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
    std::uniform_int_distribution<int> dist(1, 100);

    int continuousGreenBlock = 0;   //緑のブロックが何回連続で出ているか。
    int randomGreenNum = 0;              //乱数を入れる変数
    int randomBlueOrYellowNum = 0;  //青色か黄色をセットするときに使用する乱数を入れる変数
    bool lastTimeBlockBlueOrYellow = false; //前回のブロックが青色か黄色だったか

    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        randomGreenNum = dist(mt); //乱数をセット

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

        //緑ブロックが６回連続で出ていた場合
        if (continuousGreenBlock >= 6) {
            //青ブロック、黄色ブロックのセット
            randomBlueOrYellowNum = dist(mt);
            CreateBlueOrYellow(blockNum, randomBlueOrYellowNum);
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

        //緑ブロックの確率
        if (CreateGreen(blockNum, randomGreenNum, continuousGreenBlock) == true) {
            m_stageData[con::player_1][blockNum] = greenBlock;
            ++continuousGreenBlock;
            continue;
        }
        else {
            //青ブロック、黄色ブロックのセット
            randomBlueOrYellowNum = dist(mt);
            CreateBlueOrYellow(blockNum, randomBlueOrYellowNum);
            continuousGreenBlock = 0;
            lastTimeBlockBlueOrYellow = true;
            continue;
        }
    }

    //プレイヤー１でセットしたものを他のプレイヤーのところにもセットする。
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
    //青ブロック（70%）
    if (randomBlueOrYellowNum <= 70) {
        m_stageData[con::player_1][blockNum] = blueBlock;
    }
    //黄色ブロック（30%）
    else {
        m_stageData[con::player_1][blockNum] = yellowBlock;
    }
}

////////////////////////////////////////////////////////////
// 毎フレームの処理
////////////////////////////////////////////////////////////

void Stage::Update()
{
    //スタート地点に戻る( Debug )
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


    //プレイヤーごとの処理
    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        CheckBlock(playerNum);

        ReturnOperationTimer(playerNum);
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

    for (int blockNum = -6; blockNum < 14; blockNum++) {
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

        /*switch (m_stageData[pNum][blockNum]) {
        case greenBlock:
            m_modelGreenBlock[pNum][numberOfUsesGreenBlock]->SetPosition({
                BLOCK_POSITION_X[pNum],
                BLOCK_POSITION_Y,
                BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
            });
            ++numberOfUsesGreenBlock;

            break;
        case blueBlock:
            m_modelBlueBlock[pNum][blockNum]->SetPosition({
                BLOCK_POSITION_X[pNum],
                BLOCK_POSITION_Y,
                BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
            });
            ++numberOfUsesBlueBlock;

            break;
        case yellowBlock:
            m_modelYellowBlock[pNum][blockNum]->SetPosition({
                BLOCK_POSITION_X[pNum],
                BLOCK_POSITION_Y,
                BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
            });
            ++numberOfUsesYellowBlock;

            break;
        default:

            break;
        }*/
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

    //操作不可状態に対する耐性を削除
    m_resistanceImpossibleOperation[pNum] = false;

    //プレイヤーの現在の位置を保存
    m_playerBeforeBlockPosition[pNum] = m_playerBlockPosition[pNum];

    //プレイヤーが何個目のブロックにいるかの情報を更新
    m_playerBlockPosition[pNum] += moveNum;

    //最大数を超えていたら、最大数 - 1に合わせる。
    if (m_playerBlockPosition[pNum] >= m_MAX_BLOCK) {
        m_playerBlockPosition[pNum] = m_MAX_BLOCK - 1;
    }


    DrawBlock(pNum);




    //old

    //モデルの位置を更新
    //for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
    //    m_modelRender[pNum][blockNum]->SetPosition({
    //        BLOCK_POSITION_X[pNum],
    //        BLOCK_POSITION_Y,
    //        BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - BLOCK_SIZE * (m_playerBlockPosition[pNum])
    //        //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
    //    });
    //}

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


 /*   int numberOfUsesGreenBlock = 0;
    int numberOfUsesBlueBlock = 0;
    int numberOfUsesYellowBlock = 0;

    for (int blockNum = 0; blockNum < 14; blockNum++) {
        if (m_stageData[pNum][m_playerBlockPosition[pNum] + blockNum] == greenBlock) {
            m_modelGreenBlock[pNum][numberOfUsesGreenBlock]->SetPosition({
            BLOCK_POSITION_X[pNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            ++numberOfUsesGreenBlock;
        }
        else if (m_stageData[pNum][m_playerBlockPosition[pNum] + blockNum] == blueBlock) {
            m_modelBlueBlock[pNum][numberOfUsesBlueBlock]->SetPosition({
              BLOCK_POSITION_X[pNum],
              BLOCK_POSITION_Y,
              BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            ++numberOfUsesBlueBlock;
        }
        else if (m_stageData[pNum][m_playerBlockPosition[pNum] + blockNum] == yellowBlock) {
            m_modelYellowBlock[pNum][numberOfUsesYellowBlock]->SetPosition({
               BLOCK_POSITION_X[pNum],
               BLOCK_POSITION_Y,
               BLOCK_POSITION_Z + BLOCK_SIZE * blockNum
                });
            ++numberOfUsesYellowBlock;
        }
    }*/


    //for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
    //    m_modelRender[pNum][blockNum]->SetPosition({
    //        BLOCK_POSITION_X[pNum],
    //        BLOCK_POSITION_Y,
    //        BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - BLOCK_SIZE * (m_playerBlockPosition[pNum])
    //    });
    //}
}

//////////////////////////////
// ゴール時の処理
//////////////////////////////

void Stage::GoalBlock()
{
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
        }
    }

    if (addNowRank == true) {
        //次の順位を設定
        m_nowRank += nextRank;
    }
}