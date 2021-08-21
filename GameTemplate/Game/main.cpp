#include "stdafx.h"
#include "system/system.h"
#include "sound_engine.h"
#include "../../MiniEngine/time/Stopwatch.h"

#include "main_processing.h"
#include "constant.h"

//作業が終わったら、Get関数、Set関数をconst参照に変更しておくこと

//８月２０日がリファクタリングの締め切り
//するべきこと
//・リファクタリング
//・「トゥーンシェーダー」の実装

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
	for (int controllerNum = con::FIRST_ELEMENT_ARRAY; controllerNum < NUMBER_OF_CONTROLLER; controllerNum++) {
		g_pad[controllerNum]->Init(controllerNum);
	}

	//ゲームオブジェクトマネージャーのインスタンスを作成する。
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	SoundEngine::CreateInstance(); //サウンドエンジン
	EffectEngine::CreateInstance(); //エフェクトエンジン

	//ストップウォッチを生成する
	Stopwatch stopWatch;

	MainProcessing* game = NewGO<MainProcessing>(igo::PRIORITY_CLASS, igo::CLASS_NAME_MAIN_PROCESSING);
	
	//////////////////////////////////////
	// 初期化を行うコードを書くのはここまで！！！
	//////////////////////////////////////
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		//ストップウォッチの計測開始
		stopWatch.Start();

		//レンダリング開始。
		g_engine->BeginFrame();
		
		//////////////////////////////////////
		// ここから絵を描くコードを記述する。
		//////////////////////////////////////
		
		GameObjectManager::GetInstance()->ExecuteUpdate();
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);
		//エフェクトエンジンのアップデート。
		EffectEngine::GetInstance()->Update(g_gameTime->GetFrameDeltaTime());

		//サウンドエンジンのアップデート
		SoundEngine::GetInstance()->Update();



		//エフェクトのドロー。
		EffectEngine::GetInstance()->Draw();

		//////////////////////////////////////
		// 絵を描くコードを書くのはここまで！！！
		//////////////////////////////////////
		g_engine->EndFrame();

		//FPS固定の処理
		//スピンロックを行う。
		int restTime = 0;
		do {
			stopWatch.Stop();
			restTime = STOP_WATCH_CONST_DATA::MILLISECOND_FOR_LOCK_60FPS -
				static_cast<int>(stopWatch.GetElapsedMillisecond());
		} while (restTime > 0);

		//ストップウォッチの計測終了
		stopWatch.Stop();

		//デルタタイムをストップウォッチの計測時間から、計算する
		g_gameTime->PushFrameDeltaTime((float)stopWatch.GetElapsed());
	}

	DeleteGO(game);


	//ゲームオブジェクトマネージャーを削除。
	PhysicsWorld::DeleteInstance();
	GameObjectManager::DeleteInstance();
	EffectEngine::DeleteInstance(); //エフェクトエンジン
	SoundEngine::DeleteInstance(); //サウンドエンジン

	return 0;
}