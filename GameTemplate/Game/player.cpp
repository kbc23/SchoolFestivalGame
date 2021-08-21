#include "stdafx.h"
#include "player.h"

#include "game_data.h"
#include "main_processing.h"
#include "CPU_player_controller.h"
#include "stage.h"
#include "sudden_death_mode.h"
#include "pause.h"
#include "game_start_countdown.h"
#include "Result.h"

namespace //constant
{
	////////////////////////////////////////////////////////////
	// ������
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// SE�̉���
	//////////////////////////////

	const float INIT_SE_JUMP = 0.3f;
	const float INIT_SE_FALL = 2.0f;
	const float INIT_SE_SRIP = 0.7f;

	////////////////////////////////////////////////////////////
	// �ʒu���
	////////////////////////////////////////////////////////////

	const Vector3 PLAYER_START_POSITION[con::PlayerNumberMax] = {	//�v���C���[�̏������W
		{ 390.0f, 0.0f, -240.0f },											//�v���C���[�P
		{ 130.0f, 0.0f, -240.0f },											//�v���C���[�Q
		{ -130.0f, 0.0f, -240.0f },											//�v���C���[�R
		{ -390.0f, 0.0f, -240.0f }											//�v���C���[�S
	};
	
	const Vector2 GOAL_RANK_FONT_POSITION[con::PlayerNumberMax] = {	//�S�[�����ʂ̕\�����W
		{ -390.0f, 0.0f },													//�v���C���[�P
		{ -130.0f, 0.0f },													//�v���C���[�Q
		{ 130.0f, 0.0f },													//�v���C���[�R
		{ 390.0f, 0.0f }													//�v���C���[�S
	};

	//���f���ƃt�H���g��X���W�̌������t���ۂ�

	////////////////////////////////////////////////////////////
	// ���͊֌W
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// �^�C�}�[�֘A
	////////////////////////////////////////////////////////////

	const int TIMER_RESET = 0; //�^�C�}�[�̃��Z�b�g
	const int TIME_ANIMATION = 30; //�W�����v�A�j���[�V�����̎���

	const int TIME_BLUE_BLOCK_ANIMATION = 30;   //�F�̃u���b�N�ɍs�����Ƃ��̃A�j���[�V�����̎���

	const int TIME_YELLOW_BLOCK_ANIMATION = 30; //���F�̃u���b�N�ɍs�����Ƃ��̃A�j���[�V�����̎���


	////////////////////////////////////////////////////////////
	// ���̑�
	////////////////////////////////////////////////////////////

	const float JUMP_MOVE = 2.0f;

	const Vector2 PLAYER_RANK_SPRITE[con::PlayerNumberMax] = {	//�v���C���[�̏��ʂ̏������W
		{ -490.0f, -70.0f},											//�v���C���[�P
		{ -180.0f, -70.0f },										//�v���C���[�Q
		{ 140.0f, -70.0f },											//�v���C���[�R
		{ 460.0f, -70.0f }											//�v���C���[�S
	};
}



Player::Player()
{
	//�A�j���[�V�����̐ݒ�
	m_animationPlayer[idle].Load(filePath::tka::IDLE);
	m_animationPlayer[jump].Load(filePath::tka::JUMP);
	m_animationPlayer[fall].Load(filePath::tka::FALL);
	m_animationPlayer[srip].Load(filePath::tka::SRIP);
	m_animationPlayer[win].Load(filePath::tka::WIN);
	m_animationPlayer[stand].Load(filePath::tka::STAND);
	m_animationPlayer[lose].Load(filePath::tka::LOSE);
	//���[�v�Đ���true�ɂ���
	m_animationPlayer[idle].SetLoopFlag(true);
	m_animationPlayer[stand].SetLoopFlag(true);

	//////////////////////////////
	// NewGO
	//////////////////////////////

	//////////
	// ���f����NewGO
	//////////

	//�v���C���[���Ƃɏ���
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
		m_modelCharacter[playerNum]->Init(
			filePath::tkm::CHAEACTER_MODEL, modelUpAxis::enModelUpAxisY, m_animationPlayer, AnimationMax);
		m_modelCharacter[playerNum]->Deactivate();
	}

	//////////
	// �X�v���C�g��NewGO
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
			m_spriteGoalRank[playerNum][rankNum]->Init(filePath::dds::RANK[rankNum]);
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}

	m_spriteGameEnd = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteGameEnd->Init(filePath::dds::GAME_END);
	m_spriteGameEnd->Deactivate();

	//////////
	// SE��NewGO
	//////////

	m_seJump = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seJump->Init(filePath::se::JUMP);
	m_seJump->SetVolume(INIT_SE_JUMP);

	m_seFall = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seFall->Init(filePath::se::FALL);
	m_seFall->SetVolume(INIT_SE_FALL);

	m_seSrip = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seSrip->Init(filePath::se::SRIP);
	m_seSrip->SetVolume(INIT_SE_SRIP);
}

////////////////////////////////////////////////////////////
// �f�X�g���N�^�֘A
////////////////////////////////////////////////////////////

Player::~Player()
{
	//�v���C���[���Ƃɏ���
	for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
		DeleteGO(m_modelCharacter[playerNum]);
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			DeleteGO(m_spriteGoalRank[playerNum][rankNum]);
		}
	}

	DeleteGO(m_spriteGameEnd);
	DeleteGO(m_seJump);
	DeleteGO(m_seFall);
	DeleteGO(m_seSrip);
}

////////////////////////////////////////////////////////////
// Start�֐��֘A
////////////////////////////////////////////////////////////

bool Player::Start()
{
	//////////////////////////////
	// FindGO
	//////////////////////////////

	m_findCPUPlayerController = FindGO<CPUPlayerController>(igo::CLASS_NAME_CPU_PLAYER_CONTROLLER);
	m_findGameData = FindGO<GameData>(igo::CLASS_NAME_GAME_DATA);
	m_findStage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
	m_findMainProcessing = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);
	m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);
	m_findGameStartCountdown = FindGO<GameStartCountdown>(igo::CLASS_NAME_GAME_START_COUNTDOWN);
	m_findResult = FindGO<Result>(igo::CLASS_NAME_RESULT);

	return true;
}

void Player::Init()
{
	m_flagProcess = true;

	//////////
	// ���f���̏�����
	//////////

	//�v���C���[���Ƃɏ���
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->Activate();
		m_modelCharacter[playerNum]->SetPosition(PLAYER_START_POSITION[playerNum]);
		m_modelCharacter[playerNum]->SetScale({ 0.03f,0.03f,0.03f });
		m_modelCharacter[playerNum]->PlayAnimation(idle);
	}

	//////////
	// �X�v���C�g�̏�����
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->SetPosition(PLAYER_RANK_SPRITE[playerNum]);
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}

	m_spriteGameEnd->Deactivate();

	//////////
	// �����o�ϐ��̏�����
	//////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_activePlayer[playerNum] = true; //���̃v���C���[�͑��삵�Ă��邩
		m_goalRanking[playerNum] = con::rank_notClear;	//�v���C���[�̃S�[������
		m_flagGoal[playerNum] = false;	//�S�[��������
		m_stopController[playerNum] = false; //����s�\��
		m_nowAnimationBlock[playerNum] = con::greenBlock; //�v���C���[�̌��݂̃A�j���[�V����
		m_flagStopAnimation[playerNum] = false; //�A�j���[�V�����̏������~�܂��Ă��邩
		m_flagAnimationJump[playerNum] = false;	//�W�����v�A�j���[�V��������
		m_timerAnimation[playerNum] = 0;						//�A�j���[�V�����̃^�C�}�[
		m_roundPoint[playerNum] = 0;		//�v���C���[�̃��E���h�̃|�C���g
	}

	m_maxPlayer = con::PlayerNumberMax; //�v���C���[�̍ő吔
	m_goalPlayer = 0;
	m_endTimer = 0; //�S�[�����Ă���̎���
	fontDeavtive = 0;
	m_gameEnd = false; //�S�[�������v���C���[�̐l��
	m_finishSuddenDeath = false; //�T�h���f�X���[�h���I��������
}

void Player::Finish()
{
	m_flagProcess = false;

	//�v���C���[���Ƃɏ���
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->Deactivate();
	}

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		for (int rankNum = con::FIRST_ELEMENT_ARRAY; rankNum < con::GoalRankMax; rankNum++) {
			m_spriteGoalRank[playerNum][rankNum]->Deactivate();
		}
	}

	m_spriteGameEnd->Deactivate();
}

////////////////////////////////////////////////////////////
// ���t���[���̏���
////////////////////////////////////////////////////////////

void Player::Update()
{
	if (false == m_flagProcess) {
		return;
	}

	//�v���C���[���Ƃɑ���
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		if (m_flagGoal[playerNum] == false) {
			Controller(playerNum);
		}

		Animation(playerNum);
	}

	if (con::PlayerNumberMax == m_goalPlayer || m_finishSuddenDeath == true) {
		m_spriteGameEnd->Activate();
		m_endTimer++;
		if (m_endTimer > 180) {
			//�T�h���f�X���[�h�̂Ƃ��������E���h�������ɉ����ď��ʂ��m��
			if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
				SuddenDeathRank();
			}

			//���U���g�V�[���ɏ��ʏ���n��
			for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
				m_findResult->SetRank(playerNum, m_goalRanking[playerNum]);
			}

			m_gameEnd = true;
			m_findMainProcessing->SetGameEnd(m_gameEnd);
		}
	}
}

//////////////////////////////
// �v���C���[�̑��쏈��
//////////////////////////////

void Player::Controller(const int& playerNum)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	if (m_findGameStartCountdown->GetFlagStopGameProcess() == true) {
		return;
	}

	if (m_stopController[playerNum] == true) {
		return;
	}

	//if (m_findMainProcessing->GetStopOperation() == true) {
	//	return;
	//}

	if (m_flagAnimationJump[playerNum] == true) {
		return;
	}

	//�v���C���[�̑���
	if (m_maxPlayer > playerNum) {
		//�Q�}�X�i��
		if (g_pad[playerNum]->IsTrigger(enButtonA) == true) {
			if (m_findStage->MoveBlock(playerNum, con::MOVE_2) == false) {
				return;
			}
			//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
			m_seJump->Play(false);
			m_modelCharacter[playerNum]->PlayAnimation(jump);
			m_flagAnimationJump[playerNum] = true;
		}
		//�P�}�X�i��
		else if (g_pad[playerNum]->IsTrigger(enButtonB) == true) {
			if (m_findStage->MoveBlock(playerNum, con::MOVE_1) == false) {
				return;
			}
			//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
			m_seJump->Play(false);
			m_modelCharacter[playerNum]->PlayAnimation(jump);
			m_flagAnimationJump[playerNum] = true;
		}
	}
	//CPU�̑���
	else {
		const int CPUMove = m_findCPUPlayerController->CPUController(playerNum);

		//�Q�}�X�i��
		if (CPUMove == con::MOVE_2) {
			if (m_findStage->MoveBlock(playerNum, con::MOVE_2) == false) {
				return;
			}

			m_findCPUPlayerController->SetStopController(playerNum, true);

			//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
			m_seJump->Play(false);
			m_modelCharacter[playerNum]->PlayAnimation(jump);
			m_flagAnimationJump[playerNum] = true;
		}
		//�P�}�X�i��
		else if (CPUMove == con::MOVE_1) {
			if (m_findStage->MoveBlock(playerNum, con::MOVE_1) == false) {
				return;
			}

			m_findCPUPlayerController->SetStopController(playerNum, true);

			//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
			m_seJump->Play(false);
			m_modelCharacter[playerNum]->PlayAnimation(jump);
			m_flagAnimationJump[playerNum] = true;
		}
	}
}

//////////////////////////////
// �v���C���[�̃A�j���[�V����
//////////////////////////////

void Player::Animation(const int& playerNum)
{
	if (m_flagStopAnimation[playerNum] == true) {
		return;
	}

	JumpAnimation(playerNum);

	switch (m_nowAnimationBlock[playerNum]) {
	case con::greenBlock:

		break;
	case con::blueBlock:
		//�T�h���f�X��
		if (true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
			SuddenDeathBlueBlockAnimation(playerNum);
		}
		//���[�X��
		else {
			BlueBlockAnimation(playerNum);
		}
		break;
	case con::yellowBlock:
		//�T�h���f�X��
		if (true == m_findSuddenDeathMode->GetFlagSuddenDeathMode()) {
			SuddenDeathYellowBlockAnimation(playerNum);
		}
		//���[�X��
		else {
			YellowBlockAnimation(playerNum);
		}
		break;
	case con::goalBlock:

		break;
	}
}

void Player::JumpAnimation(const int& playerNum)
{
	if (m_flagAnimationJump[playerNum] == false) {
		return;
	}

	++m_timerAnimation[playerNum];

	//�����̃}�W�b�N�i���o�[����ŉ�������B
	if (m_timerAnimation[playerNum] >= 0 && m_timerAnimation[playerNum] <= 15) {
		m_modelCharacter[playerNum]->UpPositionY(JUMP_MOVE);
	}
	else if (m_timerAnimation[playerNum] >= 16 && m_timerAnimation[playerNum] <= 30) {
		m_modelCharacter[playerNum]->DownPositionY(JUMP_MOVE);
	}


	if (m_timerAnimation[playerNum] >= TIME_ANIMATION) {
		m_flagAnimationJump[playerNum] = false;
		m_timerAnimation[playerNum] = TIMER_RESET;

		//���n�����u���b�N�̔���
		m_findStage->CheckBlock(playerNum);



		if (m_flagGoal[playerNum] == false) {
			m_modelCharacter[playerNum]->PlayAnimation(idle);
		}
		else {
			if (m_goalRanking[playerNum] == con::rank_1) {
				m_modelCharacter[playerNum]->PlayAnimation(win);
			}
			else if (m_goalRanking[playerNum] == con::rank_4) {
				m_modelCharacter[playerNum]->PlayAnimation(lose);
			}
			else {
				m_modelCharacter[playerNum]->PlayAnimation(stand);
			}
		}
	}
}

void Player::BlueBlockAnimation(const int& playerNum)
{
	//�����A�j���[�V�������Đ�
	m_modelCharacter[playerNum]->PlayAnimation(fall);

	++m_timerAnimation[playerNum];
	m_modelCharacter[playerNum]->DownPositionY(30.0f);

	//�O�̈ʒu�ɖ߂����߂Ƀt���O��false�ɂ���B
	if (m_timerAnimation[playerNum] >= TIME_BLUE_BLOCK_ANIMATION) {
		m_timerAnimation[playerNum] = TIMER_RESET;
		//���f�������ɖ߂��B
		m_modelCharacter[playerNum]->ResetPositionY();
		m_modelCharacter[playerNum]->PlayAnimation(idle);
		//�L�����N�^�[�̈ʒu��O�̈ʒu�ɖ߂�
		m_findStage->ReturnBlock(playerNum);

		//����\�ɂ���
		m_stopController[playerNum] = false;

		m_nowAnimationBlock[playerNum] = con::greenBlock;
	}
}

void Player::SuddenDeathBlueBlockAnimation(const int& playerNum)
{
	//�����A�j���[�V�������Đ�
	m_modelCharacter[playerNum]->PlayAnimation(fall);

	++m_timerAnimation[playerNum];
	m_modelCharacter[playerNum]->DownPositionY(30.0f);

	//�v���C���[���\���ɂ���B
	if (m_timerAnimation[playerNum] >= TIME_BLUE_BLOCK_ANIMATION) {
		m_modelCharacter[playerNum]->Deactivate();
		//�A�j���[�V�������������Ȃ��悤�ɂ���
		m_flagStopAnimation[playerNum] = true;
	}
}

void Player::YellowBlockAnimation(const int& playerNum)
{
	//�������A�j���[�V�������Đ�
	m_modelCharacter[playerNum]->PlayAnimation(srip);

	++m_timerAnimation[playerNum];

	//����\�ɂ���B
	if (m_timerAnimation[playerNum] >= TIME_YELLOW_BLOCK_ANIMATION) {
		m_timerAnimation[playerNum] = TIMER_RESET;

		//���f�������ɖ߂�
		m_modelCharacter[playerNum]->PlayAnimation(idle);
		//����\�ɂ���
		m_stopController[playerNum] = false;

		m_nowAnimationBlock[playerNum] = con::greenBlock;
	}
}

void Player::SuddenDeathYellowBlockAnimation(const int& playerNum)
{
	//�������A�j���[�V�������Đ�
	m_modelCharacter[playerNum]->PlayAnimation(srip);

	++m_timerAnimation[playerNum];

	//����\�ɂ���B
	if (m_timerAnimation[playerNum] >= TIME_YELLOW_BLOCK_ANIMATION) {
		//�A�j���[�V�������������Ȃ��悤�ɂ���
		m_flagStopAnimation[playerNum] = true;
	}
}

void Player::NextRound()
{
	while (fontDeavtive < 120)
	{
		fontDeavtive += 1;
	}

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->PlayAnimation(idle);
		m_modelCharacter[playerNum]->ResetPositionY();
		m_modelCharacter[playerNum]->Activate();
	}
	
	if (fontDeavtive >= 120) {
		m_spriteGameEnd->Deactivate();
		m_goalPlayer = 0;

		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < m_maxPlayer; playerNum++) {
			m_flagGoal[playerNum] = false;
		}

		fontDeavtive = 0;

		for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
			for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < con::GoalRankMax; spriteNum++) {
				m_spriteGoalRank[playerNum][spriteNum]->Deactivate();
			}
		}
	}

	for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
		m_activePlayer[playerNum] = true;
		m_goalRanking[playerNum] = con::rank_notClear;
		m_flagGoal[playerNum] = false;
		m_flagAnimationJump[playerNum] = false;
		m_timerAnimation[playerNum] = 0;
		m_flagStopAnimation[playerNum] = false;
		m_stopController[playerNum] = false;
		m_nowAnimationBlock[playerNum] = con::greenBlock;
	}

	m_goalPlayer = 0;												
	fontDeavtive = 0;
}

////////////////////////////////////////////////////////////
// �T�h���f�X���[�h�̊֐�
////////////////////////////////////////////////////////////

void Player::SuddenDeathRank()
{
	int Ranking = con::rank_1;
	bool checkAddRank = false;

	//�擾���E���h�����������ɍ������ʂɂ���
	for (int roundPointNum = 3; roundPointNum >= 0; roundPointNum--) {
		for (int playerNum = con::player_1; playerNum < con::PlayerNumberMax; playerNum++) {
			if (m_roundPoint[playerNum] == roundPointNum) {
				m_goalRanking[playerNum] = Ranking;
				checkAddRank = true;
			}
		}

		if (checkAddRank == true) {
			++Ranking;
			checkAddRank = false;
		}
	}
}