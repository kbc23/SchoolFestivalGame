#include "stdafx.h"
#include "system/system.h"
#include "sound_engine.h"
#include "effect_render.h"

#include "game.h"



namespace //constant
{
	const int NUMBER_OF_CONTROLLER = 4; //コントローラーの最大の数
}



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
	for (int i = 0; i < NUMBER_OF_CONTROLLER; i++) {
		g_pad[i]->Init(i);
	}

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	SoundEngine::CreateInstance(); //サウンドエンジン
	EffectEngine::CreateInstance(); //エフェクトエンジン

	Game* game = NewGO<Game>(0, "game");
	
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
		// ここから絵を描くコードを記述する。
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		//エフェクトエンジンのアップデート。
		EffectEngine::GetInstance()->Update(1.0f / 60.0f);

		//サウンドエンジンのアップデート
		SoundEngine::GetInstance()->Update();



		//エフェクトのドロー。
		EffectEngine::GetInstance()->Draw();

		//////////////////////////////////////
		// 絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();
	}

	DeleteGO(game);


	//ゲームオブジェクトマネージャーを削除。
	PhysicsWorld::DeleteInstance();
	GameObjectManager::DeleteInstance();
	EffectEngine::DeleteInstance(); //エフェクトエンジン
	SoundEngine::DeleteInstance(); //サウンドエンジン

	return 0;
}