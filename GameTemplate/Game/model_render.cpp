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

////////////////////////////////////////////////////////////
// 初期化
////////////////////////////////////////////////////////////

//void ModelRender::Init(const char* filePath)
//{
//  //tkmファイルのファイルパスを保持
//  m_tkmFilePath = filePath;
//
//	//モデルの初期化
//	InitModel(filePath);
//
//	//初期化完了
//	m_finishInit = true;
//}

void ModelRender::Init(const char* filePath,
	modelUpAxis::EnModelUpAxis modelUpAxis,
	AnimationClip* animationClip,
	int maxAnimationClipNum
)
{
	//tkmファイルのファイルパスを保持
	m_tkmFilePath = filePath;
	//スケルトンのデータの読み込み
	InitSkeleton(filePath);
	//モデルの初期化
	InitModel(filePath, modelUpAxis);
	//アニメーションを初期化
	InitAnimation(animationClip, maxAnimationClipNum);

	//初期化完了
	m_finishInit = true;
}

bool ModelRender::InitSkeleton(const char* filePath)
{
	//tkmファイルをtksファイルに変換
	std::string skeletonFilePath = filePath;
	int pos = (int)skeletonFilePath.find(".tkm");
	skeletonFilePath.replace(pos, 4, ".tks");

	//スケルトンのリソースを確保
	m_skeletonPointer.reset(new Skeleton);
	//スケルトンのデータの読み込み
	bool isInited = m_skeletonPointer->Init(skeletonFilePath.c_str());

	//初期化に成功したか
	//成功
	if (isInited) {
		return true;
	}
	//失敗
	else {
		//スケルトンのリソースを解放
		m_skeletonPointer.reset();
		return false;
	}
}

void ModelRender::InitAnimation(AnimationClip* animationClip, int maxAnimationClipNum)
{
	//アニメ―ションクリップを登録しているか
	if (animationClip == nullptr) {
		//していたらreturn
		return;
	}

	//アニメーションのリソースを確保
	m_animationPointer.reset(new Animation);
	//アニメーションを初期化
	m_animationPointer->Init(
		*m_skeletonPointer,
		animationClip,
		maxAnimationClipNum
	);
}

void InitToonShader()
{

}

void ModelRender::InitModel(const char* filePath,
	modelUpAxis::EnModelUpAxis modelUpAxis
)
{
	//ライトの設定をする。
	//ディレクションライトを初期化する
	InitDirectionLight();

	//ポイントライトを初期化する
	//InitPointLight();

	//スポットライトを初期化する
	//InitSpotLight();

	//環境光を初期化する
	InitAmbientLight();

	//半球ライトを初期化する
	//InitHemiLight();

	//3Dモデルをロードするための情報を設定する
	//モデルの初期化するための情報を設定
	ModelInitData modelInitData;
	//tkmファイルのファイルパスを設定
	modelInitData.m_tkmFilePath = filePath;
	//使用するシェーダーファイルパスを設定
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//スケルトンを指定する。
	if (m_skeletonPointer) {	//スケルトンが初期化されていたら
		modelInitData.m_skeleton = m_skeletonPointer.get();
	}
	//モデルの上方向を指定
	modelInitData.m_modelUpAxis = modelUpAxis;
	//ライトの情報を定数バッファとしてディスクリプタヒープに
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
	m_light.ptColor.z = 240.0f;

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
	m_light.spColor.x = 200.0f;
	m_light.spColor.y = 10.0f;
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
	m_light.ambientLight.x = 0.05f;
	m_light.ambientLight.y = 0.05f;
	m_light.ambientLight.z = 0.05f;
}

void ModelRender::InitHemiLight()
{
	//地面色、天球色、地面の法線のデータを設定する
	m_light.groundColor.x = 0.7f;
	m_light.groundColor.y = 0.5f;
	m_light.groundColor.z = 0.3f;

	//天球色を設定
	m_light.skyColor.x = 0.15f;
	m_light.skyColor.y = 0.7f;
	m_light.skyColor.z = 0.95f;

	//地面の法線を設定
	m_light.groundNormal.x = 0.0f;
	m_light.groundNormal.y = 1.0f;
	m_light.groundNormal.z = 0.0f;
}

////////////////////////////////////////////////////////////
// 描画処理
////////////////////////////////////////////////////////////

void ModelRender::Render(RenderContext& renderContext)
{
	//未初期化時
	if (m_finishInit == false) {
		return;
	}

	//モデルの描画
	m_model.Draw(renderContext);
}

void ModelRender::Update()
{
	//未初期化時
	if (m_finishInit == false) {
		return;
	}

	//スケルトンを更新。
	if (m_skeletonPointer) {	//スケルトンが初期化されていたら
		m_skeletonPointer->Update(m_model.GetWorldMatrix());
	}
	//アニメーションを進める。
	if (m_animationPointer) {	//アニメーションが初期化されていたら
		m_animationPointer->Progress(g_gameTime->GetFrameDeltaTime());
	}
	//モデルの座標更新
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

}