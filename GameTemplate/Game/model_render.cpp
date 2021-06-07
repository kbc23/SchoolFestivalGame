#include "stdafx.h"
#include "model_render.h"

ModelRender::ModelRender()
{

}

ModelRender::~ModelRender()
{

}

bool ModelRender::Start()
{


	return true;
}

void ModelRender::Update()
{
	//未初期化時
	if (m_finishInit == false) {
		return;
	}

	//モデルの座標更新
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

}

////////////////////////////////////////////////////////////
// 初期化
////////////////////////////////////////////////////////////

void ModelRender::Init(const char* filePath)
{
    //tkmファイルのファイルパスを保持
    m_tkmFilePath = filePath;

	//モデルの初期化
	InitModel(filePath);

	//初期化完了
	m_finishInit = true;
}

void ModelRender::InitModel(const char* filePath)
{
	//ライトの設定をする。
	//ディレクションライトを初期化する
	InitDirectionLight();

	//ポイントライトを初期化する
	//InitPointLight(light);

	//スポットライトを初期化する
	//InitSpotLight(light);

	//アンビエントライトを初期化する
	InitAmbientLight();

	//3Dモデルをロードするための情報を設定する
	//モデルの初期化するための情報を設定
	ModelInitData modelInitData;
	//tkmファイルのファイルパスを設定
	modelInitData.m_tkmFilePath = filePath;
	//使用するシェーダーファイルパスを設定
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//ディレクションライトの情報を定数バッファとしてディスクリプタヒープに
	//登録するためにモデルの初期化情報として渡す。
	modelInitData.m_expandConstantBuffer = &m_light;
	modelInitData.m_expandConstantBufferSize = sizeof(m_light);
	//初期化情報を使ってモデル表示処理を初期化する
	m_model.Init(modelInitData);
}

//////////////////////////////
// ライトごとの初期化
//////////////////////////////

void ModelRender::InitDirectionLight()
{
	//ライトは斜め上から当たっている。
	m_light.dirDirection.x = 1.0f;
	m_light.dirDirection.y = -1.0f;
	m_light.dirDirection.z = -1.0f;
	//正規化する。
	m_light.dirDirection.Normalize();
	//ライトのカラーの設定（ライトの強さ）
	m_light.dirColor.x = 1.5f;
	m_light.dirColor.y = 1.5f;
	m_light.dirColor.z = 1.5f;

	//視点の位置を設定
	m_light.eyePos = g_camera3D->GetPosition();
}


void ModelRender::InitPointLight()
{
	//ポイントライトの初期座標を設定する
	m_light.ptPosition.x = 0.0f;
	m_light.ptPosition.y = 50.0f;
	m_light.ptPosition.z = 50.0f;

	//ポイントライトの初期カラーを設定する
	m_light.ptColor.x = 15.0f;
	m_light.ptColor.y = 0.0f;
	m_light.ptColor.z = 0.0f;

	//ポイントライトの影響範囲を設定する
	m_light.ptRange = 100.0f;
}

void ModelRender::InitSpotLight()
{
	//初期座標
	m_light.spPosition.x = 0.0f;
	m_light.spPosition.y = 50.0f;
	m_light.spPosition.z = -150.0f;
	//ライトのカラー
	m_light.spColor.x = 10.0f;
	m_light.spColor.y = 200.0f;
	m_light.spColor.z = 10.0f;
	//初期方向は斜め下にする。
	m_light.spDirection.x = 1.0f;
	m_light.spDirection.y = -1.0f;
	m_light.spDirection.z = 1.0f;
	//正規化
	m_light.spDirection.Normalize();
	//射出範囲は300
	m_light.spRange = 300.0f;
	//射出角度は25度
	m_light.spAngle = Math::DegToRad(25.0f);
}

void ModelRender::InitAmbientLight()
{
	//環境光
	m_light.ambientLight.x = 0.1f;
	m_light.ambientLight.y = 0.1f;
	m_light.ambientLight.z = 0.1f;
}





void ModelRender::Render(RenderContext& renderContext)
{
	//未初期化時
	if (m_finishInit == false) {
		return;
	}

	//モデルの描画
	m_model.Draw(renderContext);
}