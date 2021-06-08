#include "stdafx.h"
#include "stage.h"

namespace {
    const Vector3 BLOCK_SCALE = { 1.0f, 1.0f, 1.0f };
    const Vector3 BLOCK_START_POSITION[4] = {
        { 390.0f, -150.0f, -250.0f },	//プレイヤー１
        { 130.0f, -150.0f, -250.0f },	//プレイヤー２
        { -130.0f, -150.0f, -250.0f },	//プレイヤー３
        { -390.0f, -150.0f, -250.0f }	//プレイヤー４
    };
}

Stage::Stage()
{

}

Stage::~Stage()
{

}

bool Stage::Start()
{
    m_modelRender1 = NewGO<ModelRender>(0);
    m_modelRender1->Init("Assets/modelData/model_normal.tkm");
    m_modelRender1->SetPosition(BLOCK_START_POSITION[0]);
    m_modelRender1->SetScale(BLOCK_SCALE);
    m_modelRender2 = NewGO<ModelRender>(0);
    m_modelRender2->Init("Assets/modelData/model_flat.tkm");
    m_modelRender2->SetPosition(BLOCK_START_POSITION[1]);
    m_modelRender2->SetScale(BLOCK_SCALE);
    m_modelRender3 = NewGO<ModelRender>(0);
    m_modelRender3->Init("Assets/modelData/green.tkm");
    m_modelRender3->SetPosition(BLOCK_START_POSITION[2]);
    m_modelRender3->SetScale(BLOCK_SCALE);
    m_modelRender4 = NewGO<ModelRender>(0);
    m_modelRender4->Init("Assets/modelData/green.tkm");
    m_modelRender4->SetPosition(BLOCK_START_POSITION[3]);
    m_modelRender4->SetScale(BLOCK_SCALE);

    return true;
}

void Stage::Update()
{

}