#pragma once

namespace con //constant
{
	////////////////////////////////////////////////////////////
	// 数字関連
	////////////////////////////////////////////////////////////

	const int FIRST_ELEMENT_ARRAY = 0;		//配列の最初の要素
	const int TIME_RESET_ZERO = 0;			//タイマーを０にリセット

	const float GAME_SCREEN_W = 1280.0f;	//画面の横幅
	const float GAME_SCREEN_H = 720.0f;		//画面の縦幅

	////////////////////////////////////////////////////////////
	// 確率関連
	////////////////////////////////////////////////////////////

	const int MINIMUM_RANDOM_NUMBER = 1;	//乱数の最小値
	const int MAXIMUM_RANDOM_NUMBER = 100;	//乱数の最大値

	////////////////////////////////////////////////////////////
	// プレイヤーの移動量
	////////////////////////////////////////////////////////////

	const int MOVE_2 = 2;		//Aボタンを押したときの移動量（２マス移動）
	const int MOVE_1 = 1;		//Bボタンを押したときの移動量（１マス移動）
	const int NOT_MOVE = 0;		//移動しない

	////////////////////////////////////////////////////////////
	// enum関連
	////////////////////////////////////////////////////////////

	/**
	 * @brief プレイヤー番号
	*/
	const enum PlayerNumber
	{
		player_1,		//P1
		player_2,		//P2
		player_3,		//P3
		player_4,		//P4
		PlayerNumberMax
	};

	/**
	 * @brief ゴール順位
	*/
	const enum GoalRank
	{
		rank_1,			//１位
		rank_2,			//２位
		rank_3,			//３位
		rank_4,			//４位
		GoalRankMax,	//順位の配列の大きさ
		rank_notClear	//順位未確定
	};

	/**
	 * @brief ブロックの種類
	*/
	const enum BlockData
	{
		greenBlock,		//緑
		blueBlock,		//穴
		yellowBlock,	//バナナ
		goalBlock,		//ゴール
		BlockDataMax
	};

	/**
	 * @brief CPUの強さ
	*/
	const enum CPULevel
	{
		easy,			//よわい
		normal,			//ふつう
		hard,			//つよい
		DifficultyMax
	};
}

namespace choice
{
	/**
	 * @brief モードの選択肢
	*/
	const enum Mode
	{
		race,			//レース
		sudden_death,	//サドンデス
		ModeMax
	};

	/**
	 * @brief 
	*/
	const enum ResultCommand
	{
		playOneMoreTime,			//もう一度プレイ
		chooseTheNumberOfPlayers,	//プレイヤー数選択
		chooseARule,				//モード選択
		exitGame,					//ゲームをやめる
		ResultCommandMax
	};
}

//ファイルパス
namespace filePath
{
	////////////////////////////////////////////////////////////
	// モデル
	////////////////////////////////////////////////////////////

	namespace tkm
	{
		constexpr const char* CHAEACTER_MODEL = "Assets/modelData/unityChan.tkm";
		constexpr const char* GREEN_BLOCK = "Assets/modelData/green.tkm";
		constexpr const char* YELLOW_BLOCK = "Assets/modelData/yellow.tkm";
		constexpr const char* GOAL_BLOCK = "Assets/modelData/goal.tkm";
	}

	////////////////////////////////////////////////////////////
	// アニメーション
	////////////////////////////////////////////////////////////

	namespace tka
	{
		constexpr const char* IDLE = "Assets/animData/UnityChanIdle.tka";	//待機中
		constexpr const char* JUMP = "Assets/animData/UnityChanJump.tka";	//ジャンプ
		constexpr const char* FALL = "Assets/animData/UnityChanFall.tka";	//落下
		constexpr const char* SRIP = "Assets/animData/UnityChanSrip2.tka";	//スリップ
		constexpr const char* WIN = "Assets/animData/UnityChanWin.tka";		//１位時
		constexpr const char* STAND = "Assets/animData/UnityChanStand.tka";	//２、３位時
		constexpr const char* LOSE = "Assets/animData/UnityChanLose.tka";	//４位時
	}

	////////////////////////////////////////////////////////////
	// 2D画像
	////////////////////////////////////////////////////////////

	namespace dds
	{
		//ローディング画面
		constexpr const char* START_LOADING = "Assets/Image/Loading.dds";
		//タイトル
		constexpr const char* TITLE = "Assets/Image/Title.dds";
		//選択画面系の背景
		constexpr const char* BACKGROUND = "Assets/Image/Background.dds";
		constexpr const char* BACKGROUND_2 = "Assets/Image/Background_2.dds";
		//モード選択画面のUI
		constexpr const char* MODE_UI[choice::Mode::ModeMax] =
		{
			"Assets/Image/Command_race.dds",			//レース
			"Assets/Image/Command_sudden_death.dds"		//サドンデス
		};
		//プレイヤー数選択画面のUI
		constexpr const char* NUMBER_OF_PLAYERS[4] =
		{
			"Assets/Image/Number_of_players_1.dds",
			"Assets/Image/Number_of_players_2.dds",
			"Assets/Image/Number_of_players_3.dds",
			"Assets/Image/Number_of_players_4.dds"
		};
		//CPUの強さを選択する画面のUI
		constexpr const char* CPU_STRENGTH[3] =
		{
			"Assets/Image/CPU_weak.dds",
			"Assets/Image/CPU_usually.dds",
			"Assets/Image/CPU_strong.dds"
		};
		//ポーズ画面のUI
		constexpr const char* PAUSE_UI[3] =
		{
			"Assets/Image/Pause_return_to_game.dds",	//ゲームに戻る
			"Assets/Image/Pause_retry.dds",				//リトライ
			"Assets/Image/Pause_quit_the_game.dds"		//ゲームをやめる
		};
		//タイトル画面のUI
		constexpr const char* PRESS_A_BUTTON = "Assets/Image/Press_A_Button.dds";
		//カウントダウン
		constexpr const char* COUNT[4] =
		{
			"Assets/Image/Count_start.dds",
			"Assets/Image/Count_1.dds",
			"Assets/Image/Count_2.dds",
			"Assets/Image/Count_3.dds"
		};
		//ゲーム画面のUI
		constexpr const char* SCORE_TIME_UI = "Assets/Image/Score_time_UI.dds";
		constexpr const char* BACKGROUND_SKY = "Assets/Image/Background_sky.dds";
		constexpr const char* BACKGROUND_CLOUD = "Assets/Image/Background_cloud.dds";
		//プレイヤーの信仰度
		constexpr const char* DEGREE_OF_PROGRESS = "Assets/Image/Degree_of_progress.dds";
		constexpr const char* PLAYER_MARK[con::PlayerNumberMax] =
		{
			"Assets/Image/Player_mark_1.dds",
			"Assets/Image/Player_mark_2.dds",
			"Assets/Image/Player_mark_3.dds",
			"Assets/Image/Player_mark_4.dds"
		};
		constexpr const char* GAME_END = "Assets/Image/Game_end.dds";
		//ラウンド
		constexpr const char* ROUND_WIN[con::PlayerNumberMax][3] =
		{
			{
			"Assets/Image/round_p1_1.dds",
			"Assets/Image/round_p1_2.dds",
			"Assets/Image/round_p1_3.dds",
			},
			{
			"Assets/Image/round_p2_1.dds",
			"Assets/Image/round_p2_2.dds",
			"Assets/Image/round_p2_3.dds",
			},
			{
			"Assets/Image/round_p3_1.dds",
			"Assets/Image/round_p3_2.dds",
			"Assets/Image/round_p3_3.dds",
			},
			{
			"Assets/Image/round_p4_1.dds",
			"Assets/Image/round_p4_2.dds",
			"Assets/Image/round_p4_3.dds",
			}
		};
		//結果画面後の選択画面のUI
		constexpr const char* RESULT_COMMAND[choice::ResultCommand::ResultCommandMax] =
		{
			"Assets/Image/Command_play_one_more_time.dds",
			"Assets/Image/Command_choose_the_number_of_players.dds",
			"Assets/Image/Command_choose_a_rule.dds",
			"Assets/Image/Command_exit_game.dds"
		};
		//順位表示
		constexpr const char* RANK[con::GoalRankMax] =
		{
			"Assets/Image/Rank_1.dds",
			"Assets/Image/Rank_2.dds",
			"Assets/Image/Rank_3.dds",
			"Assets/Image/Rank_4.dds"
		};
		//Aボタンを押してねのUI
		constexpr const char* PRESS_A_NEXT = "Assets/Image/Press_A_Next.dds";
		//フェード
		constexpr const char* FADE = "Assets/Image/Fade.dds";
	}

	////////////////////////////////////////////////////////////
	// BGM
	////////////////////////////////////////////////////////////

	namespace bgm
	{
		//タイトル画面
		constexpr const wchar_t* TITLE = L"Assets/Sound/BGM/Title.wav";
		//ゲーム画面
		constexpr const wchar_t* STAGE[5] =
		{
			L"Assets/Sound/BGM/Stage_1.wav",
			L"Assets/Sound/BGM/Stage_2.wav",
			L"Assets/Sound/BGM/Stage_3.wav",
			//L"Assets/Sound/BGM/Stage_4.wav",
			L"Assets/Sound/BGM/Stage_5.wav",
			L"Assets/Sound/BGM/Stage_6.wav"
		};
	}

	////////////////////////////////////////////////////////////
	// SE
	////////////////////////////////////////////////////////////
	
	namespace se
	{
		//選択画面系
		constexpr const wchar_t* DECISION = L"Assets/Sound/SE/Decision.wav";		//決定
		constexpr const wchar_t* CANCEL = L"Assets/Sound/SE/Cancel.wav";			//キャンセル
		constexpr const wchar_t* MOVE_CURSOR = L"Assets/Sound/SE/Move_cursor.wav";	//カーソル移動
		//カウントダウン
		constexpr const wchar_t* COUNT = L"Assets/Sound/SE/Count.wav";
		constexpr const wchar_t* GAME_START = L"Assets/Sound/SE/Game_start.wav";
		//ゲーム画面のSE
		constexpr const wchar_t* JUMP = L"Assets/Sound/SE/Jump.wav";	//ジャンプ
		constexpr const wchar_t* FALL = L"Assets/Sound/SE/Fall.wav";	//落下
		constexpr const wchar_t* SRIP = L"Assets/Sound/SE/Srip.wav";	//スリップ
	}
}