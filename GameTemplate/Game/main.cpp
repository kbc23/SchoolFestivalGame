#include "stdafx.h"
#include "system/system.h"

#include "game.h"


//ディレクションライト用の構造体を定義する
//struct Light {
//	//ディレクションライト
//	Vector3 dirDirection; //ライトの方向
//	//HLSL側の定数バッファのfloat3型の変数は16の倍数のアドレスに配置されるため、
//	//C++側にはパディングを埋めておく。
//	float pad;
//	Vector3 dirColor;
//	float pad1;
//
//	//ライト構造体にポイントライト用のメンバ変数を追加する
//	Vector3 ptPosition; //位置
//	float pad2;			//パディング
//	Vector3 ptColor;	//カラー
//	float ptRange;		//影響範囲
//
//	//ライト構造体にスポットライト用のメンバ変数を追加
//	Vector3 spPosition;		//位置
//	float pad3;				//パディング
//	Vector3 spColor;		//カラー
//	float spRange;			//影響範囲
//	Vector3 spDirection;	//射出方向
//	float spAngle;			//射出角度
//
//	//構造体に視点の位置を追加する
//	Vector3 eyePos; //視点の位置
//	float pad4;
//
//	//環境光
//	Vector3 ambientLight;   // アンビエントライト
//	float pad5;
//
//	//地面色と天球色、地面の法線を追加する
//	Vector3 groundColor;	//地面色
//	float pad6;
//	Vector3 skyColor;		//天球色
//	float pad7;
//	Vector3 groundNormal;	//地面の法線
//};

//////////////////////////////////////
//関数宣言
//////////////////////////////////////
//void InitDirectionLight(Light& light);
//void InitPointLight(Light& light);
//void InitSpotLight(Light& light);
//void InitAmbientLight(Light& light);



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

	//コントローラーの初期化
	for (int i = 0; i < 4; i++) {
		g_pad[i]->Init(i);
	}



	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	Game* game = NewGO<Game>(0);
	
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




		//////////////////////////////////////
		//絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}

	DeleteGO(game);


	//ゲームオブジェクトマネージャーを削除。
	GameObjectManager::DeleteInstance();
	return 0;
}