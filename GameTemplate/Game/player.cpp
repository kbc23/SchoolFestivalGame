#include "stdafx.h"
#include "player.h"

#include "main_processing.h"
#include "CPU_player_controller.h"
#include "stage.h"
#include "sudden_death_mode.h"
#include "pause.h"
#include "game_start_countdown.h"
#include "result.h"
#include "rank.h"

namespace //constant
{
	////////////////////////////////////////////////////////////
	// ������
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// SE�̉���
	//////////////////////////////

	const float INIT_SE_JUMP = 0.3f; //�W�����v��SE
	const float INIT_SE_FALL = 2.0f; //��������SE
	const float INIT_SE_SRIP = 0.7f; //�X���b�v����SE

	//////////////////////////////
	// �ʒu���
	//////////////////////////////

	const Vector3 PLAYER_INIT_POSITION[con::PlayerNumberMax] = {	//�v���C���[�̏������W
		{ 390.0f, 0.0f, -240.0f },											//�v���C���[�P
		{ 130.0f, 0.0f, -240.0f },											//�v���C���[�Q
		{ -130.0f, 0.0f, -240.0f },											//�v���C���[�R
		{ -390.0f, 0.0f, -240.0f }											//�v���C���[�S
	};
	
	const Vector2 PLAYER_RANK_SPRITE_INIT_POSITION[con::PlayerNumberMax] = {	//�v���C���[�̏��ʂ̏������W
		{ -490.0f, -70.0f},															//�v���C���[�P
		{ -180.0f, -70.0f },														//�v���C���[�Q
		{ 140.0f, -70.0f },															//�v���C���[�R
		{ 460.0f, -70.0f }															//�v���C���[�S
	};

	////////////////////////////////////////////////////////////
	// �^�C�}�[�֘A
	////////////////////////////////////////////////////////////

	const int TIME_ANIMATION = 30; //�W�����v�A�j���[�V�����̎���

	const int JUMP_UP_ANIMATION_START = 0; //�W�����v�A�j���[�V�����̏㏸�̊J�n
	const int JUMP_UP_ANIMATION_END = 15; //�W�����v�A�j���[�V�����̏㏸�̏I���
	const int JUMP_DOWN_ANIMATION_START = 16; //�W�����v�A�j���[�V�����̉��~�̊J�n
	const int JUMP_DOWN_ANIMATION_END = 30; //�W�����v�A�j���[�V�����̉��~�̏I���

	const int TIME_BLUE_BLOCK_ANIMATION = 30;   //�F�̃u���b�N�ɍs�����Ƃ��̃A�j���[�V�����̎���

	const int TIME_YELLOW_BLOCK_ANIMATION = 30; //���F�̃u���b�N�ɍs�����Ƃ��̃A�j���[�V�����̎���

	////////////////////////////////////////////////////////////
	// ���̑�
	////////////////////////////////////////////////////////////

	const float JUMP_MOVE = 2.0f; //�v���C���[�̃W�����v���̏㉺�̈ړ���

	const float BLUE_BLOCK_ANIMATION_DOWN_POSITION = 30.0f; //�u���b�N���̗�����
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

	////////////////////////////////////////////////////////////
	// NewGO
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// ���f����NewGO
	//////////////////////////////

	//�v���C���[���f���̏�����
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
		m_modelCharacter[playerNum]->Init(
			filePath::tkm::CHAEACTER_MODEL, modelUpAxis::enModelUpAxisY, m_animationPlayer, AnimationMax); //������
		m_modelCharacter[playerNum]->Deactivate(); //��\��
	}

	//////////////////////////////
	// SE��NewGO
	//////////////////////////////

	//�W�����v����SE�̏�����
	m_seJump = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seJump->Init(filePath::se::JUMP); //������
	m_seJump->SetVolume(INIT_SE_JUMP); //����

	//��������SE�̏�����
	m_seFall = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seFall->Init(filePath::se::FALL); //������
	m_seFall->SetVolume(INIT_SE_FALL); //����

	//�X���b�v����SE�̏�����
	m_seSrip = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seSrip->Init(filePath::se::SRIP); //������
	m_seSrip->SetVolume(INIT_SE_SRIP); //����
}

////////////////////////////////////////////////////////////
// �f�X�g���N�^�֘A
////////////////////////////////////////////////////////////

Player::~Player()
{
	//�v���C���[���Ƃɏ���
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		DeleteGO(m_modelCharacter[playerNum]);
	}

	DeleteGO(m_seJump);
	DeleteGO(m_seFall);
	DeleteGO(m_seSrip);
}

////////////////////////////////////////////////////////////
// Start�֐��֘A
////////////////////////////////////////////////////////////

bool Player::Start()
{
	////////////////////////////////////////////////////////////
	// FindGO
	////////////////////////////////////////////////////////////

	m_findCPUPlayerController = FindGO<CPUPlayerController>(igo::CLASS_NAME_CPU_PLAYER_CONTROLLER);
	m_findStage = FindGO<Stage>(igo::CLASS_NAME_STAGE);
	m_findMainProcessing = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);
	m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);
	m_findGameStartCountdown = FindGO<GameStartCountdown>(igo::CLASS_NAME_GAME_START_COUNTDOWN);
	m_findResult = FindGO<Result>(igo::CLASS_NAME_RESULT);
	m_findRank = FindGO<Rank>(igo::CLASS_NAME_RANK);

	return true;
}

void Player::Init()
{
	m_flagProcess = true;

	////////////////////////////////////////////////////////////
	// ���f���̏�����
	////////////////////////////////////////////////////////////

	//�v���C���[���f���̏�����
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->Activate(); //�\��
		m_modelCharacter[playerNum]->SetPosition(PLAYER_INIT_POSITION[playerNum]); //�ʒu
		m_modelCharacter[playerNum]->SetScale({ 0.03f,0.03f,0.03f }); //�傫��
		m_modelCharacter[playerNum]->PlayAnimation(idle); //�A�j���[�V����
	}

	////////////////////////////////////////////////////////////
	// �����o�ϐ��̏�����
	////////////////////////////////////////////////////////////

	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_activePlayer[playerNum] = true; //���̃v���C���[�͑��삵�Ă��邩
		m_goalRanking[playerNum] = con::rank_notClear; //�v���C���[�̃S�[������
		m_stopController[playerNum] = false; //����s�\��
		m_nowAnimationBlock[playerNum] = con::greenBlock; //�v���C���[�̌��݂̃A�j���[�V����
		m_flagStopAnimation[playerNum] = false; //�A�j���[�V�����̏������~�܂��Ă��邩
		m_flagAnimationJump[playerNum] = false;	//�W�����v�A�j���[�V��������
		m_timerAnimation[playerNum] = 0; //�A�j���[�V�����̃^�C�}�[
		m_roundPoint[playerNum] = 0; //�v���C���[�̃��E���h�̃|�C���g
	}

	m_maxPlayer = con::PlayerNumberMax; //�v���C���[�̍ő吔
	m_goalPlayer = 0;
	m_endTimer = 0; //�S�[�����Ă���̎���
	m_gameEnd = false; //�S�[�������v���C���[�̐l��
	m_finishSuddenDeath = false; //�T�h���f�X���[�h���I��������
}

void Player::Finish()
{
	m_flagProcess = false;

	//�v���C���[���f���̔�\��
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->Deactivate(); //��\��
	}
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
		//�S�[�������v���C���[�͑���ł��Ȃ�
		if (false == m_findRank->GetDoingGoal(playerNum)) {
			//����
			Controller(playerNum);
		}
		//�A�j���[�V����
		Animation(playerNum);
	}

	//�I����
	GameEnd();
}

void Player::GameEnd()
{
	//�S���S�[�������Ƃ�
	if (con::PlayerNumberMax == m_goalPlayer || m_finishSuddenDeath == true) {
		//�I��
		
		m_endTimer++;
		if (m_endTimer > 180) {
			//�T�h���f�X���[�h�̂Ƃ��������E���h�������ɉ����ď��ʂ��m��
			if (m_findSuddenDeathMode->GetFlagSuddenDeathMode() == true) {
				//�T�h���f�X���[�h���̍ŏI���ʂ̌���
				SuddenDeathRank();
			}
			
			//���U���g�V�[���ɏ��ʏ���n��
			for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
				//m_findResult->SetRank(playerNum, m_goalRanking[playerNum]);
				m_findRank->SetFlagGameEnd(true); //Rank�N���X�ɃQ�[�����I���������Ƃ�`����
			}

			//�Q�[���V�[���I��
			m_gameEnd = true;
			m_findMainProcessing->SetGameEnd(m_gameEnd);
		}
	}

}

//////////////////////////////
// �v���C���[�̑��쏈��
//////////////////////////////

void Player::Controller(const int playerNum)
{
	//�X�^�[�g���̃J�E���g�_�E�����I����Ă��邩
	if (m_findGameStartCountdown->GetFlagStopGameProcess() == true) {
		return;
	}

	//����ł����Ԃ�
	if (m_stopController[playerNum] == true) {
		return;
	}

	//�W�����v�A�j���[�V��������
	if (m_flagAnimationJump[playerNum] == true) {
		return;
	}

	//���쏈��

	//�v���C���[�̑���
	if (m_maxPlayer > playerNum) {
		ControllerPlayer(playerNum);
	}
	//CPU�̑���
	else {
		ControllerCPU(playerNum);
	}
}

void Player::ControllerPlayer(const int playerNum)
{
	//�Q�}�X�i��
	if (g_pad[playerNum]->IsTrigger(enButtonA) == true) {
		//�ړ�����
		if (m_findStage->MoveBlock(playerNum, con::MOVE_2) == false) {
			//�ړ��s��
			return;
		}

		//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
		m_seJump->Play(false); //�Đ�
		m_modelCharacter[playerNum]->PlayAnimation(jump); //�A�j���[�V����
		m_flagAnimationJump[playerNum] = true; //�W�����v�A�j���[�V����������
	}
	//�P�}�X�i��
	else if (g_pad[playerNum]->IsTrigger(enButtonB) == true) {
		//�ړ�����
		if (m_findStage->MoveBlock(playerNum, con::MOVE_1) == false) {
			//�ړ��s��
			return;
		}

		//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
		m_seJump->Play(false); //�Đ�
		m_modelCharacter[playerNum]->PlayAnimation(jump); //�A�j���[�V����
		m_flagAnimationJump[playerNum] = true; //�W�����v�A�j���[�V����������
	}
}

void Player::ControllerCPU(const int playerNum)
{
	//CPU�R���g���[���[�ɂ�鑀��̎擾
	const int CPUMove = m_findCPUPlayerController->CPUController(playerNum);

	//�Q�}�X�i��
	if (CPUMove == con::MOVE_2) {
		//�ړ�����
		if (m_findStage->MoveBlock(playerNum, con::MOVE_2) == false) {
			//�ړ��s��
			return;
		}

		//CPU�R���g���[���[�̑�����~����
		m_findCPUPlayerController->SetStopController(playerNum, true);

		//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
		m_seJump->Play(false); //�Đ�
		m_modelCharacter[playerNum]->PlayAnimation(jump); //�A�j���[�V����
		m_flagAnimationJump[playerNum] = true; //�W�����v�A�j���[�V����������
	}
	//�P�}�X�i��
	else if (CPUMove == con::MOVE_1) {
		//�ړ�����
		if (m_findStage->MoveBlock(playerNum, con::MOVE_1) == false) {
			//�ړ��s��
			return;
		}

		//CPU�R���g���[���[�̑�����~����
		m_findCPUPlayerController->SetStopController(playerNum, true);

		//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
		m_seJump->Play(false); //�Đ�
		m_modelCharacter[playerNum]->PlayAnimation(jump); //�A�j���[�V����
		m_flagAnimationJump[playerNum] = true; //�W�����v�A�j���[�V����������
	}
}

//////////////////////////////
// �v���C���[�̃A�j���[�V����
//////////////////////////////

void Player::Animation(const int playerNum)
{
	//�A�j���[�V�����̏������~�߂��Ă��邩
	if (m_flagStopAnimation[playerNum] == true) {
		return;
	}

	//�W�����v�A�j���[�V����
	JumpAnimation(playerNum);

	//�u���b�N���Ƃ̃A�j���[�V����
	switch (m_nowAnimationBlock[playerNum]) {
	//�΃u���b�N
	case con::greenBlock:
		// do nothing
		break;
	//�u���b�N
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
	//���F�u���b�N
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
	//�S�[���u���b�N
	case con::goalBlock:
		// do nothing
		break;
	}
}

void Player::JumpAnimation(const int playerNum)
{
	//�W�����v�A�j���[�V��������
	if (m_flagAnimationJump[playerNum] == false) {
		return;
	}

	++m_timerAnimation[playerNum]; //�^�C�}�[�̃J�E���g

	//�ʒu�̏㏸
	if (m_timerAnimation[playerNum] >= JUMP_UP_ANIMATION_START &&
		m_timerAnimation[playerNum] <= JUMP_UP_ANIMATION_END) {
		m_modelCharacter[playerNum]->UpPositionY(JUMP_MOVE); //���f���̏㏸
	}
	//�ʒu�̉��~
	else if (m_timerAnimation[playerNum] >= JUMP_DOWN_ANIMATION_START &&
		m_timerAnimation[playerNum] <= JUMP_DOWN_ANIMATION_END) {
		m_modelCharacter[playerNum]->DownPositionY(JUMP_MOVE); //���f���̉��~
	}

	//�^�C�}�[�𒴂���
	if (m_timerAnimation[playerNum] >= TIME_ANIMATION) {
		m_flagAnimationJump[playerNum] = false; //�W�����v�A�j���[�V���������Ȃ���Ԃɂ���
		m_timerAnimation[playerNum] = con::TIME_RESET_ZERO; //�^�C�}�[�����Z�b�g

		//���n�����u���b�N�̔���
		m_findStage->CheckBlock(playerNum);

		//�S�[�����̃A�j���[�V����
		GoalAnimation(playerNum);
	}
}

void Player::GoalAnimation(const int playerNum)
{
	//�S�[�����Ă��Ȃ��ꍇ
	if (m_findRank->GetDoingGoal(playerNum) == false) {
		m_modelCharacter[playerNum]->PlayAnimation(idle); //�A�j���[�V����
	}
	//�S�[�������ꍇ
	else {
		//�P��
		if (m_goalRanking[playerNum] == con::rank_1) {
			m_modelCharacter[playerNum]->PlayAnimation(win); //�A�j���[�V����
		}
		//�S��
		else if (m_goalRanking[playerNum] == con::rank_4) {
			m_modelCharacter[playerNum]->PlayAnimation(lose); //�A�j���[�V����
		}
		//�Q�A�R��
		else {
			m_modelCharacter[playerNum]->PlayAnimation(stand); //�A�j���[�V����
		}
	}
}

void Player::BlueBlockAnimation(const int playerNum)
{
	//�����A�j���[�V�������Đ�
	m_modelCharacter[playerNum]->PlayAnimation(fall);

	++m_timerAnimation[playerNum]; //�^�C�}�[�̃J�E���g

	//���f���̍~��
	m_modelCharacter[playerNum]->DownPositionY(BLUE_BLOCK_ANIMATION_DOWN_POSITION);

	//�^�C�}�[�𒴂���
	if (m_timerAnimation[playerNum] >= TIME_BLUE_BLOCK_ANIMATION) {
		m_timerAnimation[playerNum] = con::TIME_RESET_ZERO; //�^�C�}�[�����Z�b�g

		//���f�������ɖ߂�
		m_modelCharacter[playerNum]->ResetPositionY(); //�ʒu
		m_modelCharacter[playerNum]->PlayAnimation(idle); //�A�j���[�V����

		//�L�����N�^�[�̈ʒu��O�̈ʒu�ɖ߂�
		m_findStage->ReturnBlock(playerNum);

		//����\�ɂ���
		m_stopController[playerNum] = false;

		m_nowAnimationBlock[playerNum] = con::greenBlock; //�u���b�N�ɉ������A�j���[�V�����̏��
	}
}

void Player::SuddenDeathBlueBlockAnimation(const int playerNum)
{
	//�����A�j���[�V�������Đ�
	m_modelCharacter[playerNum]->PlayAnimation(fall);

	++m_timerAnimation[playerNum]; //�^�C�}�[�̃J�E���g

	//���f���̍~��
	m_modelCharacter[playerNum]->DownPositionY(BLUE_BLOCK_ANIMATION_DOWN_POSITION);

	//�^�C�}�[�𒴂���
	if (m_timerAnimation[playerNum] >= TIME_BLUE_BLOCK_ANIMATION) {
		m_modelCharacter[playerNum]->Deactivate(); //��\��
		//�A�j���[�V�������������Ȃ��悤�ɂ���
		m_flagStopAnimation[playerNum] = true;
	}
}

void Player::YellowBlockAnimation(const int playerNum)
{
	//�������A�j���[�V�������Đ�
	m_modelCharacter[playerNum]->PlayAnimation(srip);

	++m_timerAnimation[playerNum]; //�^�C�}�[�̃J�E���g

	//�^�C�}�[�𒴂���
	if (m_timerAnimation[playerNum] >= TIME_YELLOW_BLOCK_ANIMATION) {
		m_timerAnimation[playerNum] = con::TIME_RESET_ZERO;

		//���f�������ɖ߂�
		m_modelCharacter[playerNum]->PlayAnimation(idle); //�A�j���[�V����
		//����\�ɂ���
		m_stopController[playerNum] = false;

		m_nowAnimationBlock[playerNum] = con::greenBlock; //�u���b�N�ɉ������A�j���[�V�����̏��
	}
}

void Player::SuddenDeathYellowBlockAnimation(const int playerNum)
{
	//�������A�j���[�V�������Đ�
	m_modelCharacter[playerNum]->PlayAnimation(srip);

	++m_timerAnimation[playerNum]; //�^�C�}�[�̃J�E���g

	//�^�C�}�[�𒴂���
	if (m_timerAnimation[playerNum] >= TIME_YELLOW_BLOCK_ANIMATION) {
		//�A�j���[�V�������������Ȃ��悤�ɂ���
		m_flagStopAnimation[playerNum] = true;
	}
}

void Player::NextRound()
{
	//�v���C���[�̃��f�������ɖ߂�
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelCharacter[playerNum]->PlayAnimation(win); //�A�j���[�V����
		m_modelCharacter[playerNum]->PlayAnimation(idle); //�A�j���[�V����
		m_modelCharacter[playerNum]->ResetPositionY(); //Y���W�̈ʒu
		m_modelCharacter[playerNum]->Activate(); //�\��
	}

	//�S�[�������v���C���[���O�ɂ���
	m_goalPlayer = 0;

	//�ȉ��A�����o�ϐ������Z�b�g
	for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_activePlayer[playerNum] = true;
		m_goalRanking[playerNum] = con::rank_notClear;
		m_flagAnimationJump[playerNum] = false;
		m_timerAnimation[playerNum] = 0;
		m_flagStopAnimation[playerNum] = false;
		m_stopController[playerNum] = false;
		m_nowAnimationBlock[playerNum] = con::greenBlock;
	}
}

////////////////////////////////////////////////////////////
// �T�h���f�X���[�h�̊֐�
////////////////////////////////////////////////////////////

void Player::SuddenDeathRank()
{
	int Ranking = con::rank_1; //����
	bool checkAddRank = false; //���̎擾���E���h���̃v���C���[�̏��ʂ��ς�邩

	//�擾���E���h�����������ɍ������ʂɂ���
	for (int roundPointNum = 3; roundPointNum >= con::FIRST_ELEMENT_ARRAY; roundPointNum--) {
		for (int playerNum = con::FIRST_ELEMENT_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
			//���ʂ��m��
			if (m_roundPoint[playerNum] == roundPointNum) {
				m_goalRanking[playerNum] = Ranking;
				checkAddRank = true;
			}
		}

		//���̃v���C���[�̏��ʂ̂��߂ɏ��ʂ𑝉�����
		if (checkAddRank == true) {
			++Ranking; //���ʂ�ύX
			checkAddRank = false;
		}
	}
}