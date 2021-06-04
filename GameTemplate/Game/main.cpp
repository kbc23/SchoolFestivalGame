#include "stdafx.h"
#include "system/system.h"


//ディレクションライト用の構造体を定義する
struct Light {
	Vector3 ligDirection; //ライトの方向
	//HLSL側の定数バッファのfloat3型の変数は16の倍数のアドレスに配置されるため、
	//C++側にはパディングを埋めておく。
	float pad;
	Vector3 ligColor;
	float pad1;

	//ライト構造体にポイントライト用のメンバ変数を追加する
	Vector3 ptPosition; //位置
	float pad2;			//パディング
	Vector3 ptColor;	//カラー
	float ptRange;		//影響範囲

	//ライト構造体にスポットライト用のメンバ変数を追加
	Vector3 spPosition;		//位置
	float pad3;				//パディング
	Vector3 spColor;		//カラー
	float spRange;			//影響範囲
	Vector3 spDirection;	//射出方向
	float spAngle;			//射出角度

	//構造体に視点の位置を追加する
	Vector3 eyePos; //視点の位置
	float pad4;

	Vector3 ambientLight;   // アンビエントライト
};

//////////////////////////////////////
//関数宣言
//////////////////////////////////////
void InitDirectionLight(Light& light);
void InitPointLight(Light& light);
void InitAmbientLight(Light& light);



///////////////////////////////////////////////////////////////////
// ウィンドウプログラムのメイン関数。
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// ここから初期化を行うコードを記述する。
	//////////////////////////////////////

	//モデル・ライトの実装

	//ディレクションライトのデータを作成する
	Light light;
	// ディレクションライトを初期化する
	InitDirectionLight(light);
	// ポイントライトを初期化する
	//InitPointLight(light);
	// アンビエントライトを初期化する
	InitAmbientLight(light);

	//スポットライトのデータを初期化する
	//初期座標
	light.spPosition.x = 0.0f;
	light.spPosition.y = 0.0f;
	light.spPosition.z = -50.0f;
	//ライトのカラー
	light.spColor.x = 200.0f;
	light.spColor.y = 10.0f;
	light.spColor.z = 10.0f;
	//初期方向は斜め下にする。
	light.spDirection.x = 1.0f;
	light.spDirection.y = -1.0f;
	light.spDirection.z = 1.0f;
	//正規化
	light.spDirection.Normalize();
	//射出範囲は300
	light.spRange = 300.0f;
	//射出角度は25度
	light.spAngle = Math::DegToRad(25.0f);




	//3Dモデルをロードするための情報を設定する
	//モデルの初期化するための情報を設定
	ModelInitData modelInitData;
	//tkmファイルのファイルパスを設定
	modelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//使用するシェーダーファイルパスを設定
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//ディレクションライトの情報を定数バッファとしてディスクリプタヒープに
	//登録するためにモデルの初期化情報として渡す。
	modelInitData.m_expandConstantBuffer = &light;
	modelInitData.m_expandConstantBufferSize = sizeof(light);
	//初期化情報を使ってモデル表示処理を初期化する
	Model charaModel;
	charaModel.Init(modelInitData);




	//ModelInitData lightModelInitData;
	//lightModelInitData.m_tkmFilePath = "Assets/modelData/light.tkm";

	//// 使用するシェーダーファイルパスを設定する
	//lightModelInitData.m_fxFilePath = "Assets/shader/light.fx";
	//lightModelInitData.m_expandConstantBuffer = &light;
	//lightModelInitData.m_expandConstantBufferSize = sizeof(light);

	//// 初期化情報を使ってモデルを初期化する
	//Model lightModel;
	//lightModel.Init(lightModelInitData);










	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//レンダリング開始。
		g_engine->BeginFrame();
		

		//////////////////////////////////////
		//ここから絵を描くコードを記述する。
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		
		//light.ptPosition.x -= g_pad[0]->GetLStickXF();
		//if (g_pad[0]->IsPress(enButtonB)) {
		//	light.ptPosition.y += g_pad[0]->GetLStickXF();
		//}
		//else {
		//	light.ptPosition.z -= g_pad[0]->GetLStickYF();
		//}
		light.spPosition.x -= g_pad[0]->GetLStickXF();
		if (g_pad[0]->IsPress(enButtonB)) {
			light.spPosition.y += g_pad[0]->GetLStickYF();
		}
		else {
			light.spPosition.z -= g_pad[0]->GetLStickYF();
		}

		Quaternion qRotY;
		qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);
		qRotY.Apply(light.spDirection);
		Vector3 rotAxis;
		rotAxis.Cross(g_vec3AxisY, light.spDirection);
		Quaternion qRotX;
		qRotX.SetRotation(rotAxis, g_pad[0]->GetLStickYF() * 0.01f);
		qRotX.Apply(light.spDirection);



		//モデル・ライトの実装					  
							  
		//モデルのドローコールを実行する
		charaModel.Draw(renderContext);





		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}
	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}

void InitDirectionLight(Light& light)
{
	//ライトは斜め上から当たっている。
	light.ligDirection.x = 1.0f;
	light.ligDirection.y = -1.0f;
	light.ligDirection.z = -1.0f;
	//正規化する。
	light.ligDirection.Normalize();
	//ライトのカラーの設定
	light.ligColor.x = 5.0f;
	light.ligColor.y = 5.0f;
	light.ligColor.z = 5.0f;

	//視点の位置を設定
	light.eyePos = g_camera3D->GetPosition();
}

void InitPointLight(Light& light)
{
	//ポイントライトの初期座標を設定する
	light.ptPosition.x = 0.0f;
	light.ptPosition.y = 50.0f;
	light.ptPosition.z = 50.0f;

	//ポイントライトの初期カラーを設定する
	light.ptColor.x = 15.0f;
	light.ptColor.y = 0.0f;
	light.ptColor.z = 0.0f;

	//ポイントライトの影響範囲を設定する
	light.ptRange = 100.0f;
}

void InitAmbientLight(Light& light)
{
	//環境光
	light.ambientLight.x = 0.8f;
	light.ambientLight.y = 0.8f;
	light.ambientLight.z = 0.8f;
}