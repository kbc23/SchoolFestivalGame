#include "stdafx.h"
#include "player.h"

#include "stage.h"

namespace //constant
{
	//////////////////////////////
	// �t�@�C���p�X
	//////////////////////////////

	const char* FILE_PATH_TKM_CHAEACTER_MODEL = "Assets/modelData/unityChan.tkm";

	//////////////////////////////
	// �ʒu���
	//////////////////////////////

	const Vector3 PLAYER_START_POSITION[Player::PlayerNumberMax] = {	//�v���C���[�̏������W
		{ 390.0f, 0.0f, -250.0f },											//�v���C���[�P
		{ 130.0f, 0.0f, -250.0f },											//�v���C���[�Q
		{ -130.0f, 0.0f, -250.0f },											//�v���C���[�R
		{ -390.0f, 0.0f, -250.0f }											//�v���C���[�S
	};
	
	const Vector2 GOAL_RANK_FONT_POSITION[Player::PlayerNumberMax] = {	//�S�[�����ʂ̕\�����W
		{ -390.0f, 0.0f },													//�v���C���[�P
		{ -130.0f, 0.0f },													//�v���C���[�Q
		{ 130.0f, 0.0f },													//�v���C���[�R
		{ 390.0f, 0.0f }													//�v���C���[�S
	};

	//���f���ƃt�H���g��X���W�̌������t���ۂ�

	//////////////////////////////
	// ���͊֌W
	//////////////////////////////

	const int MOVE_BUTTON_A = 2;		//A�{�^�����������Ƃ��̈ړ���
	const int MOVE_BUTTON_B = 1;		//B�{�^�����������Ƃ��̈ړ���

	//////////////////////////////
	// �^�C�}�[�֘A
	//////////////////////////////

	const int TIMER_RESET = 0; //�^�C�}�[�̃��Z�b�g
	const int TIME_ANIMATION = 30; //�W�����v�A�j���[�V�����̎��ԁi0.5�b�j
}



Player::Player()
{

}

////////////////////////////////////////////////////////////
// �f�X�g���N�^�֘A
////////////////////////////////////////////////////////////

Player::~Player()
{
	//�v���C���[���Ƃɏ���
	for (int i = 0; i < m_maxPlayer; i++) {
		DeleteIndividual(i);
	}
}



void Player::DeleteIndividual(const int pNum)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	DeleteGO(m_modelRender[pNum]);
	DeleteGO(m_goalRankFont[pNum]);

	//DeleteGO(m_skinModelRender[pNum]);
}

////////////////////////////////////////////////////////////
// Start�֐��֘A
////////////////////////////////////////////////////////////

bool Player::Start()
{
	//�v���C���[���Ƃɏ���
	for (int i = 0; i < m_maxPlayer; i++) {
		bool check = StartIndividual(i);

		//StartIndividual�֐���false��Ԃ�����false��Ԃ��ď������I��������B
		if (check == false) {
			return false;
		}
	}

	m_stage = FindGO<Stage>("stage");
	m_game = FindGO<Game>("game");

	return true;
}

bool Player::StartIndividual(const int pNum)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	m_modelRender[pNum] = NewGO<ModelRender>(0);
	m_modelRender[pNum]->Init(FILE_PATH_TKM_CHAEACTER_MODEL);
	m_modelRender[pNum]->SetPosition(PLAYER_START_POSITION[pNum]);

	m_goalRankFont[pNum] = NewGO<FontRender>(0);
	m_goalRankFont[pNum]->Init(L"", GOAL_RANK_FONT_POSITION[pNum]);
	m_goalRankFont[pNum]->Deactivate();

	return true;
}

////////////////////////////////////////////////////////////
// ���t���[���̏���
////////////////////////////////////////////////////////////

void Player::Update()
{
	//�v���C���[���Ƃɑ���
	for (int i = 0; i < m_maxPlayer; i++) {
		if (m_flagGoal[i] == false) {
			Controller(i);
			Animation(i);
		}
		else {
			Animation(i);
		}
	}
}

//////////////////////////////
// �v���C���[�̑��쏈��
//////////////////////////////

void Player::Controller(const int pNum)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	if (m_game->GetStopOperation() == true) {
		return;
	}

	if (m_flagAnimationJump[pNum] == true ||
		//m_flagCheckBlock[pNum] == true ||
		m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}

	//�Q�}�X�i��
	if (g_pad[pNum]->IsTrigger(enButtonA) == true) {
		if (m_stage->MoveBlock(pNum, MOVE_BUTTON_A) == false) {
			return;
		}
		//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
		m_flagAnimationJump[pNum] = true;
	}
	//�P�}�X�i��
	else if (g_pad[pNum]->IsTrigger(enButtonB) == true) {
		if (m_stage->MoveBlock(pNum, MOVE_BUTTON_B) == false) {
			return;
		}
		//�L�����N�^�[���ړ�������A�j���[�V�������W�����v�A�j���[�V�������Đ�
		m_flagAnimationJump[pNum] = true;
	}
}

//////////////////////////////
// �v���C���[�̃A�j���[�V����
//////////////////////////////

void Player::Animation(const int pNum)
{
	JumpAnimation(pNum);

	ImpossibleOperationAnimation(pNum);
}

void Player::JumpAnimation(const int pNum)
{
	if (m_flagAnimationJump[pNum] == false) {
		return;
	}

	++m_timerAnimation[pNum];

	//�����̃}�W�b�N�i���o�[����ŉ�������B
	if (m_timerAnimation[pNum] >= 0 && m_timerAnimation[pNum] <= 15) {
		m_modelRender[pNum]->UpPositionY(1.0f);
	}
	else if (m_timerAnimation[pNum] >= 16 && m_timerAnimation[pNum] <= 30) {
		m_modelRender[pNum]->DownPositionY(1.0f);
	}


	if (m_timerAnimation[pNum] >= TIME_ANIMATION) {
		m_flagAnimationJump[pNum] = false;
		m_timerAnimation[pNum] = TIMER_RESET;
	}
}

void Player::ImpossibleOperationAnimation(const int pNum)
{

}