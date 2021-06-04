#include "stdafx.h"
#include "system/system.h"


//ディレクションライト用の構造体を定義する
struct DirectionLight {
	Vector3 ligDirection; //ライトの方向
	//HLSL側の定数バッファのfloat3型の変数は16の倍数のアドレスに配置されるため、
	//C++側にはパディングを埋めておく。
	float pad;
	Vector3 ligColor;
	float pad1;

	//構造体に視点の位置を追加する
	Vector3 eyePos; //視点の位置
};




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
	DirectionLight directionLig;
	//ライトは斜め上から当たっている。
	directionLig.ligDirection.x = 1.0f;
	directionLig.ligDirection.y = -1.0f;
	directionLig.ligDirection.z = -1.0f;
	//正規化する。
	directionLig.ligDirection.Normalize();
	//ライトのカラーの設定
	directionLig.ligColor.x = 5.0f;
	directionLig.ligColor.y = 5.0f;
	directionLig.ligColor.z = 5.0f;

	//視点の位置を設定
	directionLig.eyePos = g_camera3D->GetPosition();

	//3Dモデルをロードするための情報を設定する
	//モデルの初期化するための情報を設定
	ModelInitData modelInitData;
	//tkmファイルのファイルパスを設定
	modelInitData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//使用するシェーダーファイルパスを設定
	modelInitData.m_fxFilePath = "Assets/shader/model.fx";
	//ディレクションライトの情報を定数バッファとしてディスクリプタヒープに
	//登録するためにモデルの初期化情報として渡す。
	modelInitData.m_expandConstantBuffer = &directionLig;
	modelInitData.m_expandConstantBufferSize = sizeof(directionLig);
	//初期化情報を使ってモデル表示処理を初期化する
	Model charaModel;
	charaModel.Init(modelInitData);















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

