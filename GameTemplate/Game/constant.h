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
		playerNumberMax
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

	const char* TKM_CHAEACTER_MODEL = "Assets/modelData/unityChan.tkm";
	const char* TKM_GREEN_BLOCK = "Assets/modelData/green.tkm";
	//const char* TKM_BLUE_BLOCK = "Assets/modelData/blue.tkm";
	const char* TKM_YELLOW_BLOCK = "Assets/modelData/yellow.tkm";

	////////////////////////////////////////////////////////////
	// アニメーション
	////////////////////////////////////////////////////////////

	const char* TKA_IDLE = "Assets/animData/UnityChanIdle.tka";

	////////////////////////////////////////////////////////////
	// 2D画像
	////////////////////////////////////////////////////////////

	const char* DDS_BACKGROUND = "Assets/Image/Background.dds";
	const char* DDS_CPU_WEAK = "Assets/Image/CPU_weak.dds";
	const char* DDS_CPU_USUALLY = "Assets/Image/CPU_usually.dds";
	const char* DDS_CPU_STRONG = "Assets/Image/CPU_strong.dds";
	const char* DDS_NUMBER_OF_PLAYERS_1 = "Assets/Image/Number_of_players_1.dds";
	const char* DDS_NUMBER_OF_PLAYERS_2 = "Assets/Image/Number_of_players_2.dds";
	const char* DDS_NUMBER_OF_PLAYERS_3 = "Assets/Image/Number_of_players_3.dds";
	const char* DDS_NUMBER_OF_PLAYERS_4 = "Assets/Image/Number_of_players_4.dds";
	const char* DDS_PAUSE_RETURN_TO_GAME = "Assets/Image/Pause_return_to_game.dds";
	const char* DDS_PAUSE_RETRY = "Assets/Image/Pause_retry.dds";
	const char* DDS_PAUSE_QUIT_THE_GAME = "Assets/Image/Pause_quit_the_game";
	const char* DDS_PLESS_A_BUTTON = "Assets/Image/Pless_A_Button.dds";
	const char* DDS_SCORE_TIME_UI = "Assets/Image/Score_time_UI.dds";
	const char* DDS_BACKGROUND_SKY = "Assets/Image/Background_sky.dds";
	const char* DDS_COMMAND_RACE = "Assets/Image/Command_race.dds";
	const char* DDS_COMMAND_PLAY_ONE_MORE_TIME = "Assets/Image/Command_play_one_more_time.dds";
	const char* DDS_COMMAND_CHOOSE_THE_NUMBER_OF_PLAYERS = "Assets/Image/Command_choose_the_number_of_players";
	const char* DDS_COMMAND_CHOOSE_A_RULE = "Assets/Image/Command_choose_a_rule.dds";
	const char* DDS_COMMAND_EXIT_GAME = "Assets/Image/Command_exit_game.dds";

	////////////////////////////////////////////////////////////
	// BGM
	////////////////////////////////////////////////////////////

	const wchar_t* WAV_STAGE1 = L"Assets/Sound/Stage1.wav";

	////////////////////////////////////////////////////////////
	// SE
	////////////////////////////////////////////////////////////
	



}