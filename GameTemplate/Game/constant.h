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
		constexpr const char* FALL = "Assets/animData/UnityChanFall.tka";
		constexpr const char* SRIP = "Assets/animData/UnityChanSrip2.tka";
		constexpr const char* WIN = "Assets/animData/UnityChanWin.tka";
		constexpr const char* STAND = "Assets/animData/UnityChanStand.tka";
		constexpr const char* LOSE = "Assets/animData/UnityChanLose.tka";
	}

	////////////////////////////////////////////////////////////
	// 2D�摜
	////////////////////////////////////////////////////////////

	namespace dds
	{
		//���[�f�B���O���
		constexpr const char* START_LOADING = "Assets/Image/Loading.dds";
		//�^�C�g��
		constexpr const char* TITLE = "Assets/Image/Title.dds";
		//�I����ʌn�̔w�i
		constexpr const char* BACKGROUND = "Assets/Image/Background.dds";
		constexpr const char* BACKGROUND_2 = "Assets/Image/Background_2.dds";
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
		constexpr const char* PAUSE_QUIT_THE_GAME = "Assets/Image/Pause_quit_the_game.dds";
		//�^�C�g����ʂ�UI
		constexpr const char* PRESS_A_BUTTON = "Assets/Image/Press_A_Button.dds";
		//�J�E���g�_�E��
		constexpr const char* COUNT[4] = {
			"Assets/Image/Count_start.dds",
			"Assets/Image/Count_1.dds",
			"Assets/Image/Count_2.dds",
			"Assets/Image/Count_3.dds"
		};
		//�Q�[����ʂ�UI
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
		//���E���h
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
		//���ʉ�ʌ�̑I����ʂ�UI
		constexpr const char* COMMAND_PLAY_ONE_MORE_TIME = "Assets/Image/Command_play_one_more_time.dds";
		constexpr const char* COMMAND_CHOOSE_THE_NUMBER_OF_PLAYERS = "Assets/Image/Command_choose_the_number_of_players.dds";
		constexpr const char* COMMAND_CHOOSE_A_RULE = "Assets/Image/Command_choose_a_rule.dds";
		constexpr const char* COMMAND_EXIT_GAME = "Assets/Image/Command_exit_game.dds";
		//���ʕ\��
		constexpr const char* RANKONE = "Assets/Image/juni1.dds";
		constexpr const char* RANKTWO = "Assets/Image/juni2.dds";
		constexpr const char* RANKTHREE = "Assets/Image/juni3.dds";
		constexpr const char* RANKFOUR = "Assets/Image/juni4.dds";
		constexpr const char* PRESS_A_NEXT = "Assets/Image/Press_A_Next.dds";
		//�t�F�[�h
		constexpr const char* FADE = "Assets/Image/Fade.dds";
	}

	////////////////////////////////////////////////////////////
	// BGM
	////////////////////////////////////////////////////////////

	namespace bgm
	{
		constexpr const wchar_t* TITLE = L"Assets/Sound/BGM/Title.wav";
		//�Q�[�����
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
		//�I����ʌn
		constexpr const wchar_t* DECISION = L"Assets/Sound/SE/Decision.wav";
		constexpr const wchar_t* CANCEL = L"Assets/Sound/SE/Cancel.wav";
		constexpr const wchar_t* MOVE_CURSOR = L"Assets/Sound/SE/Move_cursor.wav";
		//�J�E���g�_�E��
		constexpr const wchar_t* COUNT = L"Assets/Sound/SE/Count.wav";
		constexpr const wchar_t* GAME_START = L"Assets/Sound/SE/Game_start.wav";
		//�Q�[����ʂ�SE
		constexpr const wchar_t* JUMP = L"Assets/Sound/SE/Jump.wav";
		constexpr const wchar_t* FALL = L"Assets/Sound/SE/Fall.wav";
		constexpr const wchar_t* SRIP = L"Assets/Sound/SE/Srip.wav";
	}


}