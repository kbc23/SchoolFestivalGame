#pragma once

namespace con //constant
{
	////////////////////////////////////////////////////////////
	// �����֘A
	////////////////////////////////////////////////////////////

	//const int ZERO = 0;
	const int INIT_ZERO = 0;
	const float FLOAT_ZERO = 0.0f;
	const int FIRST_OF_THE_ARRAY = 0;
	const int TIME_RESET_ZERO = 0;


	//�v���C���[�̔ԍ�
	const enum PlayerNumber
	{
		player_1,
		player_2,
		player_3,
		player_4,
		PlayerNumberMax
	};

	//���f���̏�����̎�
	//const enum ModelUpAxis {
	//	modelUpAxisY,		//���f���̏������Y���B
	//	modelUpAxisZ,		//���f���̏������Z���B
	//};
}

//�t�@�C���p�X
namespace filePath
{
	////////////////////////////////////////////////////////////
	// ���f��
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
	// �A�j���[�V����
	////////////////////////////////////////////////////////////

	namespace tka
	{
		constexpr const char* IDLE = "Assets/animData/UnityChanIdle.tka";
		constexpr const char* JUMP = "Assets/animData/UnityChanJump.tka";
		constexpr const char* DROWN = "Assets/animData/UnityChanOboreru.tka";
		constexpr const char* SRIP = "Assets/animData/UnityChanSrip.tka";
		constexpr const char* WIN = "Assets/animData/UnityChanWin.tka";
		constexpr const char* LOSE = "Assets/animData/UnityChanLose.tka";
	}

	////////////////////////////////////////////////////////////
	// 2D�摜
	////////////////////////////////////////////////////////////

	namespace dds
	{
		//�^�C�g��
		constexpr const char* TITLE = "Assets/Image/Title.dds";
		//�I����ʌn�̔w�i
		constexpr const char* BACKGROUND = "Assets/Image/Background.dds";
		//���[�h�I����ʂ�UI
		constexpr const char* MODE_RACE = "Assets/Image/Command_race.dds";
		constexpr const char* MODE_SUDDEN_DEATH = "Assets/Image/Command_sudden_death.dds";
		//�v���C���[���I����ʂ�UI
		constexpr const char* NUMBER_OF_PLAYERS_1 = "Assets/Image/Number_of_players_1.dds";
		constexpr const char* NUMBER_OF_PLAYERS_2 = "Assets/Image/Number_of_players_2.dds";
		constexpr const char* NUMBER_OF_PLAYERS_3 = "Assets/Image/Number_of_players_3.dds";
		constexpr const char* NUMBER_OF_PLAYERS_4 = "Assets/Image/Number_of_players_4.dds";
		//CPU�̋�����I�������ʂ�UI
		constexpr const char* CPU_WEAK = "Assets/Image/CPU_weak.dds";
		constexpr const char* CPU_USUALLY = "Assets/Image/CPU_usually.dds";
		constexpr const char* CPU_STRONG = "Assets/Image/CPU_strong.dds";
		//�|�[�Y��ʂ�UI
		constexpr const char* PAUSE_RETURN_TO_GAME = "Assets/Image/Pause_return_to_game.dds";
		constexpr const char* PAUSE_RETRY = "Assets/Image/Pause_retry.dds";
		constexpr const char* PAUSE_QUIT_THE_GAME = "Assets/Image/Pause_quit_the_game";
		//�^�C�g����ʂ�UI
		constexpr const char* PRESS_A_BUTTON = "Assets/Image/Press_A_Button.dds";
		//�Q�[����ʂ�UI
		constexpr const char* SCORE_TIME_UI = "Assets/Image/Score_time_UI.dds";
		constexpr const char* BACKGROUND_SKY = "Assets/Image/Background_sky.dds";
		//���ʉ�ʌ�̑I����ʂ�UI
		constexpr const char* COMMAND_PLAY_ONE_MORE_TIME = "Assets/Image/Command_play_one_more_time.dds";
		constexpr const char* COMMAND_CHOOSE_THE_NUMBER_OF_PLAYERS = "Assets/Image/Command_choose_the_number_of_players";
		constexpr const char* COMMAND_CHOOSE_A_RULE = "Assets/Image/Command_choose_a_rule.dds";
		constexpr const char* COMMAND_EXIT_GAME = "Assets/Image/Command_exit_game.dds";
		//�t�F�[�h
		constexpr const char* FADE = "Assets/Image/Fade.dds";
	}

	////////////////////////////////////////////////////////////
	// BGM
	////////////////////////////////////////////////////////////

	namespace bgm
	{
		constexpr const wchar_t* STAGE1 = L"Assets/Sound/BGM/Stage1.wav";
		constexpr const wchar_t* TITLE = L"Assets/Sound/BGM/Title.wav";
	}

	////////////////////////////////////////////////////////////
	// SE
	////////////////////////////////////////////////////////////
	
	namespace se
	{
		constexpr const wchar_t* DECISION = L"Assets/Sound/SE/Decision.wav";
		constexpr const wchar_t* CANCEL = L"Assets/Sound/SE/Cancel.wav";
		constexpr const wchar_t* MOVE_CURSOR = L"Assets/Sound/SE/Move_cursor.wav";
	}


}