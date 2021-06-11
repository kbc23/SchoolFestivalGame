#include "stdafx.h"
#include "stage.h"



//モデルの読み込みで時間がかかっているので、
//読み込む必要があるモデルの数を減らすことができないか検証してみること。
//※現在は、プロトなのでこのままで続行



namespace //constant
{
    //////////////////////////////
    // ファイルパス
    //////////////////////////////

    const char* FILE_PATH_TKM_GREEN_BLOCK = "Assets/modelData/green.tkm";
    const char* FILE_PATH_TKM_BLUE_BLOCK = "Assets/modelData/blue.tkm";
    const char* FILE_PATH_TKM_YELLOW_BLOCK = "Assets/modelData/yellow.tkm";

    //////////////////////////////
    // ブロックの情報
    //////////////////////////////

    const float BLOCK_SIZE = 80.0f;         //ブロックのサイズ

    //////////////////////////////
    // 位置情報
    //////////////////////////////

    const float BLOCK_POSITION_X[Player::PlayerNumberMax] = {           //ブロックのX座標
        390.0f,                                                             //プレイヤー１
        130.0f,                                                             //プレイヤー２
        -130.0f,                                                            //プレイヤー３
        -390.0f                                                             //プレイヤー４
    };
    const float BLOCK_POSITION_Y = -80.0f;                              //ブロックのY座標
    const float BLOCK_POSITION_Z = -250.0f;                             //ブロックのZ座標
    const Vector3 BLOCK_START_POSITION[Player::PlayerNumberMax] = {     //プレイヤーごとのスタート位置のブロックの座標
        { 390.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z }, 	                //プレイヤー１
        { 130.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z },	                    //プレイヤー２
        { -130.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z },	                //プレイヤー３
        { -390.0f, BLOCK_POSITION_Y, BLOCK_POSITION_Z }	                    //プレイヤー４
    };

    //////////////////////////////
    // タイマー関連
    //////////////////////////////

    const int TIME_RETURN_OPERATION = 90;   //操作復帰にかかる時間（1.5秒）
}



Stage::Stage()
{

}

Stage::~Stage()
{
    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        for (int s = 0; s < m_MAX_BLOCK; s++) {
            DeleteGO(m_modelRender[i][s]);
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

    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        for (int s = 0; s < m_MAX_BLOCK; s++) {
            m_modelRender[i][s] = NewGO<ModelRender>(0);

            //ステージの情報にてモデルを変更
            if (m_stageData[s] == greenBlock) {
                m_modelRender[i][s]->Init(FILE_PATH_TKM_GREEN_BLOCK);
            }
            else if (m_stageData[s] == blueBlock) {
                m_modelRender[i][s]->Init(FILE_PATH_TKM_BLUE_BLOCK);
            }
            else if (m_stageData[s] == yellowBlock)
            {
                m_modelRender[i][s]->Init(FILE_PATH_TKM_YELLOW_BLOCK);
            }

           
            //m_modelRender[i][s]->SetScale(BLOCK_SCALE);
            m_modelRender[i][s]->SetPosition({
                BLOCK_START_POSITION[i].x,
                BLOCK_START_POSITION[i].y,
                BLOCK_START_POSITION[i].z + BLOCK_SIZE * s
            });
        }
    }

    m_player = FindGO<Player>("player");

    return true;
}

void Stage::StageCreate()
{
    for (int i = 0; i < m_MAX_BLOCK; i++) {
        m_stageData[i] = greenBlock;
    }

    m_stageData[5] = blueBlock;
    m_stageData[13] = yellowBlock;
    m_stageData[19] = yellowBlock;
    m_stageData[23] = blueBlock;
    m_stageData[27] = blueBlock;
}

////////////////////////////////////////////////////////////
// 毎フレームの処理
////////////////////////////////////////////////////////////

void Stage::Update()
{
    //スタート地点に戻る( Debug )
    if (g_pad[0]->IsTrigger(enButtonX)) {
        for (int i = 0; i < Player::PlayerNumberMax; i++) {
            for (int s = 0; s < m_MAX_BLOCK; s++) {
                m_modelRender[i][s]->SetPosition({
                    BLOCK_START_POSITION[i].x,
                    BLOCK_START_POSITION[i].y,
                    BLOCK_START_POSITION[i].z + BLOCK_SIZE * s
                    });
            }
            m_playerBlockPosition[i] = 0;
        }
    }


    //プレイヤーごとの処理
    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        CheckBlock(i);

        ReturnOperationTimer(i);
    }




    GoalBlock();
}

void Stage::MoveBlock(const int pNum, const int moveNum)
{
    if (m_activeOperation[pNum] == false) {
        return;
    }

    //操作不能状態に対する耐性を削除
    m_resistanceImpossibleOperation[pNum] = false;

    //プレイヤーの現在の位置を保存
    m_playerBeforeBlockPosition[pNum] = m_playerBlockPosition[pNum];

    //プレイヤーが何個目のブロックにいるかの情報を更新
    m_playerBlockPosition[pNum] += moveNum;
    if (m_playerBlockPosition[pNum] >= m_MAX_BLOCK) {
        m_playerBlockPosition[pNum] = m_MAX_BLOCK - 1;
    }

    //モデルの位置を更新
    for (int i = 0; i < m_MAX_BLOCK; i++) {
        m_modelRender[pNum][i]->SetPosition({
            BLOCK_POSITION_X[pNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * i - BLOCK_SIZE * (m_playerBlockPosition[pNum])
            //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
        });
    }
}

void Stage::ReturnBlock(const int pNum)
{
    //プレイヤーの現在位置を前の位置に戻す。
    m_playerBlockPosition[pNum] = m_playerBeforeBlockPosition[pNum];

    //モデルの位置を更新
    for (int i = 0; i < m_MAX_BLOCK; i++) {
        m_modelRender[pNum][i]->SetPosition({
            BLOCK_POSITION_X[pNum],
            BLOCK_POSITION_Y,
            BLOCK_POSITION_Z + BLOCK_SIZE * i - BLOCK_SIZE * (m_playerBlockPosition[pNum])
            //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
        });
    }
}

void Stage::ReturnOperationTimer(const int pNum)
{
    if (m_activeOperation[pNum] == true) {
        return;
    }

    ++m_timerReturnOperation[pNum];

    if (m_timerReturnOperation[pNum] >= TIME_RETURN_OPERATION) {
        m_activeOperation[pNum] = true;
        m_timerReturnOperation[pNum] = 0;
        m_resistanceImpossibleOperation[pNum] = true;
    }

}

void Stage::CheckBlock(const int pNum)
{
    if (m_stageData[m_playerBlockPosition[pNum]] == greenBlock) {

    }
    else if (m_stageData[m_playerBlockPosition[pNum]] == blueBlock) {
        ReturnBlock(pNum);
    }
    else if (m_stageData[m_playerBlockPosition[pNum]] == yellowBlock) {
        if (m_resistanceImpossibleOperation[pNum] == false) {
            m_activeOperation[pNum] = false;
        }
    }
}

void Stage::GoalBlock()
{
    bool addNowRank = false;
    int nextRank = 0;

    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        //プレイヤーの順位を確定
        if (m_player->GetActivePlayer(i) == true && m_playerBlockPosition[i] == m_MAX_BLOCK - 1) {
            //プレイヤーの操作をできないようにする。
            m_player->SetActivePlayer(i, false);

            //順位を確定
            m_player->SetGoalRanking(i, m_nowRank);

            ++nextRank;
            addNowRank = true;
        }
    }

    if (addNowRank == true) {
        //次の順位を設定
        m_nowRank += nextRank;
    }
}