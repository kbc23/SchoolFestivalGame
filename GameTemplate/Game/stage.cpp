#include "stdafx.h"
#include "stage.h"

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

    for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
            m_modelRender[playerNum][blockNum] = NewGO<ModelRender>(igo::PRIORITY_FIRST);

            //ステージの情報にてモデルを変更
            if (m_stageData[playerNum][blockNum] == greenBlock) {
                m_modelRender[playerNum][blockNum]->Init(FILE_PATH_TKM_GREEN_BLOCK);
            }
            else if (m_stageData[playerNum][blockNum] == blueBlock) {
                m_modelRender[playerNum][blockNum]->Init(FILE_PATH_TKM_BLUE_BLOCK);
            }
            else if (m_stageData[playerNum][blockNum] == yellowBlock) {
                m_modelRender[playerNum][blockNum]->Init(FILE_PATH_TKM_YELLOW_BLOCK);
            }

            m_modelRender[playerNum][blockNum]->SetPosition({
                BLOCK_START_POSITION[playerNum].x,
                BLOCK_START_POSITION[playerNum].y,
                BLOCK_START_POSITION[playerNum].z + BLOCK_SIZE * blockNum
            });
        }
    }

    m_player = FindGO<Player>(igo::CLASS_NAME_PLAYER);

    return true;
}

void Stage::StageCreate()
{
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        m_stageData[con::player_1][blockNum] = greenBlock;
    }

    m_stageData[con::player_1][6] = blueBlock;
    m_stageData[con::player_1][8] = blueBlock;
    m_stageData[con::player_1][12] = yellowBlock;
    m_stageData[con::player_1][15] = blueBlock;
    m_stageData[con::player_1][18] = blueBlock;
    m_stageData[con::player_1][25] = blueBlock;
    m_stageData[con::player_1][28] = yellowBlock;
    m_stageData[con::player_1][30] = blueBlock;
    m_stageData[con::player_1][32] = blueBlock;
    m_stageData[con::player_1][37] = yellowBlock;
    m_stageData[con::player_1][40] = blueBlock;
    m_stageData[con::player_1][43] = blueBlock;
    m_stageData[con::player_1][46] = blueBlock;

    for (int playerNum = con::player_2; playerNum < con::PlayerNumberMax; playerNum++) {
        for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
            m_stageData[playerNum][blockNum] = m_stageData[con::player_1][blockNum];
        }
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

    //モデルの位置を更新
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        m_modelRender[pNum][blockNum]->SetPosition({
            BLOCK_POSITION_X[pNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - BLOCK_SIZE * (m_playerBlockPosition[pNum])
            //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
        });
    }

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
            m_modelRender[pNum][m_playerBlockPosition[pNum]]->Init(FILE_PATH_TKM_GREEN_BLOCK);
            //※この方法でモデルを変更するよりかは、別の方法で変更するようにすること。
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
    for (int blockNum = con::FIRST_OF_THE_ARRAY; blockNum < m_MAX_BLOCK; blockNum++) {
        m_modelRender[pNum][blockNum]->SetPosition({
            BLOCK_POSITION_X[pNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * blockNum - BLOCK_SIZE * (m_playerBlockPosition[pNum])
        });
    }
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