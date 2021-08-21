#pragma once

namespace con //constant
{
	////////////////////////////////////////////////////////////
	// �����֘A
	////////////////////////////////////////////////////////////

	const int FIRST_ELEMENT_ARRAY = 0;		//�z��̍ŏ��̗v�f
	const int TIME_RESET_ZERO = 0;			//�^�C�}�[���O�Ƀ��Z�b�g

	const float GAME_SCREEN_W = 1280.0f;	//��ʂ̉���
	const float GAME_SCREEN_H = 720.0f;		//��ʂ̏c��

	////////////////////////////////////////////////////////////
	// �m���֘A
	////////////////////////////////////////////////////////////

	const int MINIMUM_RANDOM_NUMBER = 1;	//�����̍ŏ��l
	const int MAXIMUM_RANDOM_NUMBER = 100;	//�����̍ő�l

	////////////////////////////////////////////////////////////
	// �v���C���[�̈ړ���
	////////////////////////////////////////////////////////////

	const int MOVE_2 = 2;		//A�{�^�����������Ƃ��̈ړ��ʁi�Q�}�X�ړ��j
	const int MOVE_1 = 1;		//B�{�^�����������Ƃ��̈ړ��ʁi�P�}�X�ړ��j
	const int NOT_MOVE = 0;		//�ړ����Ȃ�

	////////////////////////////////////////////////////////////
	// enum�֘A
	////////////////////////////////////////////////////////////

	/**
	 * @brief �v���C���[�ԍ�
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
	 * @brief �S�[������
	*/
	const enum GoalRank
	{
		rank_1,			//�P��
		rank_2,			//�Q��
		rank_3,			//�R��
		rank_4,			//�S��
		GoalRankMax,	//���ʂ̔z��̑傫��
		rank_notClear	//���ʖ��m��
	};

	/**
	 * @brief �u���b�N�̎��
	*/
	const enum BlockData
	{
		greenBlock,		//��
		blueBlock,		//��
		yellowBlock,	//�o�i�i
		goalBlock,		//�S�[��
		BlockDataMax
	};

	/**
	 * @brief CPU�̋���
	*/
	const enum CPULevel
	{
		easy,			//��킢
		normal,			//�ӂ�
		hard,			//�悢
		DifficultyMax
	};
}

namespace choice
{
	/**
	 * @brief ���[�h�̑I����
	*/
	const enum Mode
	{
		race,			//���[�X
		sudden_death,	//�T�h���f�X
		ModeMax
	};

	/**
	 * @brief 
	*/
	const enum ResultCommand
	{
		playOneMoreTime,			//������x�v���C
		chooseTheNumberOfPlayers,	//�v���C���[���I��
		chooseARule,				//���[�h�I��
		exitGame,					//�Q�[������߂�
		ResultCommandMax
	};
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
		constexpr const char* YELLOW_BLOCK = "Assets/modelData/yellow.tkm";
		constexpr const char* GOAL_BLOCK = "Assets/modelData/goal.tkm";
	}

	////////////////////////////////////////////////////////////
	// �A�j���[�V����
	////////////////////////////////////////////////////////////

	namespace tka
	{
		constexpr const char* IDLE = "Assets/animData/UnityChanIdle.tka";	//�ҋ@��
		constexpr const char* JUMP = "Assets/animData/UnityChanJump.tka";	//�W�����v
		constexpr const char* FALL = "Assets/animData/UnityChanFall.tka";	//����
		constexpr const char* SRIP = "Assets/animData/UnityChanSrip2.tka";	//�X���b�v
		constexpr const char* WIN = "Assets/animData/UnityChanWin.tka";		//�P�ʎ�
		constexpr const char* STAND = "Assets/animData/UnityChanStand.tka";	//�Q�A�R�ʎ�
		constexpr const char* LOSE = "Assets/animData/UnityChanLose.tka";	//�S�ʎ�
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
		constexpr const char* MODE_UI[choice::Mode::ModeMax] =
		{
			"Assets/Image/Command_race.dds",			//���[�X
			"Assets/Image/Command_sudden_death.dds"		//�T�h���f�X
		};
		//�v���C���[���I����ʂ�UI
		constexpr const char* NUMBER_OF_PLAYERS[4] =
		{
			"Assets/Image/Number_of_players_1.dds",
			"Assets/Image/Number_of_players_2.dds",
			"Assets/Image/Number_of_players_3.dds",
			"Assets/Image/Number_of_players_4.dds"
		};
		//CPU�̋�����I�������ʂ�UI
		constexpr const char* CPU_STRENGTH[3] =
		{
			"Assets/Image/CPU_weak.dds",
			"Assets/Image/CPU_usually.dds",
			"Assets/Image/CPU_strong.dds"
		};
		//�|�[�Y��ʂ�UI
		constexpr const char* PAUSE_UI[3] =
		{
			"Assets/Image/Pause_return_to_game.dds",	//�Q�[���ɖ߂�
			"Assets/Image/Pause_retry.dds",				//���g���C
			"Assets/Image/Pause_quit_the_game.dds"		//�Q�[������߂�
		};
		//�^�C�g����ʂ�UI
		constexpr const char* PRESS_A_BUTTON = "Assets/Image/Press_A_Button.dds";
		//�J�E���g�_�E��
		constexpr const char* COUNT[4] =
		{
			"Assets/Image/Count_start.dds",
			"Assets/Image/Count_1.dds",
			"Assets/Image/Count_2.dds",
			"Assets/Image/Count_3.dds"
		};
		//�Q�[����ʂ�UI
		constexpr const char* SCORE_TIME_UI = "Assets/Image/Score_time_UI.dds";
		constexpr const char* BACKGROUND_SKY = "Assets/Image/Background_sky.dds";
		constexpr const char* BACKGROUND_CLOUD = "Assets/Image/Background_cloud.dds";
		//�v���C���[�̐M�x
		constexpr const char* DEGREE_OF_PROGRESS = "Assets/Image/Degree_of_progress.dds";
		constexpr const char* PLAYER_MARK[con::PlayerNumberMax] =
		{
			"Assets/Image/Player_mark_1.dds",
			"Assets/Image/Player_mark_2.dds",
			"Assets/Image/Player_mark_3.dds",
			"Assets/Image/Player_mark_4.dds"
		};
		constexpr const char* GAME_END = "Assets/Image/Game_end.dds";
		//���E���h
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
		//���ʉ�ʌ�̑I����ʂ�UI
		constexpr const char* RESULT_COMMAND[choice::ResultCommand::ResultCommandMax] =
		{
			"Assets/Image/Command_play_one_more_time.dds",
			"Assets/Image/Command_choose_the_number_of_players.dds",
			"Assets/Image/Command_choose_a_rule.dds",
			"Assets/Image/Command_exit_game.dds"
		};
		//���ʕ\��
		constexpr const char* RANK[con::GoalRankMax] =
		{
			"Assets/Image/Rank_1.dds",
			"Assets/Image/Rank_2.dds",
			"Assets/Image/Rank_3.dds",
			"Assets/Image/Rank_4.dds"
		};
		//A�{�^���������Ă˂�UI
		constexpr const char* PRESS_A_NEXT = "Assets/Image/Press_A_Next.dds";
		//�t�F�[�h
		constexpr const char* FADE = "Assets/Image/Fade.dds";
	}

	////////////////////////////////////////////////////////////
	// BGM
	////////////////////////////////////////////////////////////

	namespace bgm
	{
		//�^�C�g�����
		constexpr const wchar_t* TITLE = L"Assets/Sound/BGM/Title.wav";
		//�Q�[�����
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
		//�I����ʌn
		constexpr const wchar_t* DECISION = L"Assets/Sound/SE/Decision.wav";		//����
		constexpr const wchar_t* CANCEL = L"Assets/Sound/SE/Cancel.wav";			//�L�����Z��
		constexpr const wchar_t* MOVE_CURSOR = L"Assets/Sound/SE/Move_cursor.wav";	//�J�[�\���ړ�
		//�J�E���g�_�E��
		constexpr const wchar_t* COUNT = L"Assets/Sound/SE/Count.wav";
		constexpr const wchar_t* GAME_START = L"Assets/Sound/SE/Game_start.wav";
		//�Q�[����ʂ�SE
		constexpr const wchar_t* JUMP = L"Assets/Sound/SE/Jump.wav";	//�W�����v
		constexpr const wchar_t* FALL = L"Assets/Sound/SE/Fall.wav";	//����
		constexpr const wchar_t* SRIP = L"Assets/Sound/SE/Srip.wav";	//�X���b�v
	}
}