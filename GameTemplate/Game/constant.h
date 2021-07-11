#pragma once

namespace con //constant
{
	////////////////////////////////////////////////////////////
	// 数字関連
	////////////////////////////////////////////////////////////

	//const int ZERO = 0;
	const int INIT_ZERO = 0;
	const float FLOAT_ZERO = 0.0f;
	const int FIRST_OF_THE_ARRAY = 0;
	const int TIME_RESET_ZERO = 0;


	//プレイヤーの番号
	const enum PlayerNumber
	{
		player_1,
		player_2,
		player_3,
		player_4,
		PlayerNumberMax
	};

	//モデルの上方向の軸
	//const enum ModelUpAxis {
	//	modelUpAxisY,		//モデルの上方向がY軸。
	//	modelUpAxisZ,		//モデルの上方向がZ軸。
	//};
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
		//const char* TKM_BLUE_BLOCK = "Assets/modelData/blue.tkm";
		constexpr const char* YELLOW_BLOCK = "Assets/modelData/yellow.tkm";
		constexpr const char* GOAL_BLOCK = "Assets/modelData/goal.tkm";
	}

	////////////////////////////////////////////////////////////
	// アニメーション
	////////////////////////////////////////////////////////////

	namespace tka
	{
		constexpr const char* IDLE = "Assets/animData/UnityChanIdle.tka";
		constexpr const char* JUMP = "Assets/animData/UnityChanJump.tka";
		constexpr const char* FALL = "Assets/animData/UnityChanFall.tka";
		constexpr const char* SRIP = "Assets/animData/UnityChanSrip2.tka";
		constexpr const char* WIN = "Assets/animData/UnityChanWin.tka";
		constexpr const char* STAND = "Assets/animData/UnityChanStand.tka";
		constexpr const char* LOSE = "Assets/animData/UnityChanLose.tka";
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
		constexpr const char* MODE_RACE = "Assets/Image/Command_race.dds";
		constexpr const char* MODE_SUDDEN_DEATH = "Assets/Image/Command_sudden_death.dds";
		//プレイヤー数選択画面のUI
		constexpr const char* NUMBER_OF_PLAYERS_1 = "Assets/Image/Number_of_players_1.dds";
		constexpr const char* NUMBER_OF_PLAYERS_2 = "Assets/Image/Number_of_players_2.dds";
		constexpr const char* NUMBER_OF_PLAYERS_3 = "Assets/Image/Number_of_players_3.dds";
		constexpr const char* NUMBER_OF_PLAYERS_4 = "Assets/Image/Number_of_players_4.dds";
		//CPUの強さを選択する画面のUI
		constexpr const char* CPU_WEAK = "Assets/Image/CPU_weak.dds";
		constexpr const char* CPU_USUALLY = "Assets/Image/CPU_usually.dds";
		constexpr const char* CPU_STRONG = "Assets/Image/CPU_strong.dds";
		//ポーズ画面のUI
		constexpr const char* PAUSE_RETURN_TO_GAME = "Assets/Image/Pause_return_to_game.dds";
		constexpr const char* PAUSE_RETRY = "Assets/Image/Pause_retry.dds";
		constexpr const char* PAUSE_QUIT_THE_GAME = "Assets/Image/Pause_quit_the_game.dds";
		//タイトル画面のUI
		constexpr const char* PRESS_A_BUTTON = "Assets/Image/Press_A_Button.dds";
		//カウントダウン
		constexpr const char* COUNT[4] = {
			"Assets/Image/Count_start.dds",
			"Assets/Image/Count_1.dds",
			"Assets/Image/Count_2.dds",
			"Assets/Image/Count_3.dds"
		};
		//ゲーム画面のUI
		constexpr const char* SCORE_TIME_UI = "Assets/Image/Score_time_UI.dds";
		constexpr const char* BACKGROUND_SKY = "Assets/Image/Background_sky.dds";
		constexpr const char* BACKGROUND_CLOUD = "Assets/Image/Background_cloud.dds";
		constexpr const char* DEGREE_OF_PROGRESS = "Assets/Image/Degree_of_progress.dds";
		constexpr const char* PLAYER_MARK[con::PlayerNumberMax] = {
			"Assets/Image/Player_mark_1.dds",
			"Assets/Image/Player_mark_2.dds",
			"Assets/Image/Player_mark_3.dds",
			"Assets/Image/Player_mark_4.dds"
		};
		constexpr const char* GAME_END = "Assets/Image/Game_end.dds";
		//ラウンド
		constexpr const char* ROUND_WIN[con::PlayerNumberMax][3] = {
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
		constexpr const char* COMMAND_PLAY_ONE_MORE_TIME = "Assets/Image/Command_play_one_more_time.dds";
		constexpr const char* COMMAND_CHOOSE_THE_NUMBER_OF_PLAYERS = "Assets/Image/Command_choose_the_number_of_players.dds";
		constexpr const char* COMMAND_CHOOSE_A_RULE = "Assets/Image/Command_choose_a_rule.dds";
		constexpr const char* COMMAND_EXIT_GAME = "Assets/Image/Command_exit_game.dds";
		//順位表示
		constexpr const char* RANKONE = "Assets/Image/juni1.dds";
		constexpr const char* RANKTWO = "Assets/Image/juni2.dds";
		constexpr const char* RANKTHREE = "Assets/Image/juni3.dds";
		constexpr const char* RANKFOUR = "Assets/Image/juni4.dds";
		constexpr const char* PRESS_A_NEXT = "Assets/Image/Press_A_Next.dds";
		//フェード
		constexpr const char* FADE = "Assets/Image/Fade.dds";
	}

	////////////////////////////////////////////////////////////
	// BGM
	////////////////////////////////////////////////////////////

	namespace bgm
	{
		constexpr const wchar_t* TITLE = L"Assets/Sound/BGM/Title.wav";
		//ゲーム画面
		constexpr const wchar_t* STAGE[6] =
		{
			L"Assets/Sound/BGM/Stage_1.wav",
			L"Assets/Sound/BGM/Stage_2.wav",
			L"Assets/Sound/BGM/Stage_3.wav",
			L"Assets/Sound/BGM/Stage_4.wav",
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
		constexpr const wchar_t* DECISION = L"Assets/Sound/SE/Decision.wav";
		constexpr const wchar_t* CANCEL = L"Assets/Sound/SE/Cancel.wav";
		constexpr const wchar_t* MOVE_CURSOR = L"Assets/Sound/SE/Move_cursor.wav";
		//カウントダウン
		constexpr const wchar_t* COUNT = L"Assets/Sound/SE/Count.wav";
		constexpr const wchar_t* GAME_START = L"Assets/Sound/SE/Game_start.wav";
		//ゲーム画面のSE
		constexpr const wchar_t* JUMP = L"Assets/Sound/SE/Jump.wav";
		constexpr const wchar_t* FALL = L"Assets/Sound/SE/Fall.wav";
		constexpr const wchar_t* SRIP = L"Assets/Sound/SE/Srip.wav";
	}


}