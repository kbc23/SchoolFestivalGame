#include "stdafx.h"
#include "stage.h"



//モデルの読み込みで時間がかかっているので、
//読み込む必要があるモデルの数を減らすことができないか検証してみること。
//※現在は、プロトなのでこのままで続行



namespace {
    //const Vector3 BLOCK_SCALE = { 1.0f, 1.0f, 1.0f };
    const float BLOCK_SIZE = 80.0f;                                     //ブロックのサイズ
    const float BLOCK_POSITION_X[Player::PlayerNumberMax] = {           //ブロックX座標
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
}



Stage::Stage()
{

}

Stage::~Stage()
{
    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        for (int s = 0; s < MAX_BLOCK; s++) {
            DeleteGO(m_modelRender[i][s]);
        }
    }
}

bool Stage::Start()
{
    for (int i = 0; i < Player::PlayerNumberMax; i++) {
        for (int s = 0; s < MAX_BLOCK; s++) {
            m_modelRender[i][s] = NewGO<ModelRender>(0);
            m_modelRender[i][s]->Init("Assets/modelData/green.tkm");
            //m_modelRender[i][s]->SetScale(BLOCK_SCALE);
            m_modelRender[i][s]->SetPosition({
                BLOCK_START_POSITION[i].x,
                BLOCK_START_POSITION[i].y,
                BLOCK_START_POSITION[i].z + BLOCK_SIZE * s
            });
        }
    }

    return true;
}

void Stage::Update()
{
    //スタート地点に戻る( Debug )
    if (g_pad[0]->IsTrigger(enButtonX)) {
        for (int i = 0; i < Player::PlayerNumberMax; i++) {
            for (int s = 0; s < MAX_BLOCK; s++) {
                m_modelRender[i][s]->SetPosition({
                    BLOCK_START_POSITION[i].x,
                    BLOCK_START_POSITION[i].y,
                    BLOCK_START_POSITION[i].z + BLOCK_SIZE * s
                    });
            }
            m_playerBlockPosition[i] = 0;
        }
    }

    GoalBlock();
}

void Stage::MoveBlock(const int pNum, const int moveNum)
{
    //プレイヤーが何個目のブロックにいるかの情報を更新
    m_playerBlockPosition[pNum] += moveNum;
    if (m_playerBlockPosition[pNum] >= MAX_BLOCK) {
        m_playerBlockPosition[pNum] = MAX_BLOCK - 1;
    }

    for (int i = 0; i < MAX_BLOCK; i++) {
        //最後のブロックか
        if (m_playerBlockPosition[pNum] >= MAX_BLOCK) {
            m_modelRender[pNum][i]->SetPosition({
                BLOCK_POSITION_X[pNum],
                BLOCK_POSITION_Y,
                BLOCK_POSITION_Z - BLOCK_SIZE * (MAX_BLOCK - 1) + 80.0f * i
                //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
            });
        }
        else {
            m_modelRender[pNum][i]->SetPosition({
                BLOCK_POSITION_X[pNum],
                BLOCK_POSITION_Y,
                BLOCK_POSITION_Z + BLOCK_SIZE * i - BLOCK_SIZE * (m_playerBlockPosition[pNum])
                //m_modelRender[pNum][i]->GetPosition().z - BLOCK_SIZE * moveNum
                });
        }
    }
}

void Stage::GoalBlock()
{
    for (int i = 0; i < Player::PlayerNumberMax; i++) {

    }



}