#include "stdafx.h"
#include "stage.h"

#include "player.h"

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

	//////////////////////////////
	// ���͊֌W
	//////////////////////////////

	const int MOVE_BUTTON_A = 2;		//A�{�^�����������Ƃ��̈ړ���
	const int MOVE_BUTTON_B = 1;		//B�{�^�����������Ƃ��̈ړ���

	//////////////////////////////
	// �^�C�}�[�֘A
	//////////////////////////////

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


	//�t�H���g�̏���������
	for (int i = 0; i < m_maxPlayer; i++) {
		//m_fontRank[i] = NewGO<Font>(0);

	}

	m_stage = FindGO<Stage>("stage");



	return true;
}

bool Player::StartIndividual(const int pNum)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	m_modelRender[pNum] = NewGO<ModelRender>(0);
	m_modelRender[pNum]->Init(FILE_PATH_TKM_CHAEACTER_MODEL);
	m_modelRender[pNum]->SetPosition(PLAYER_START_POSITION[pNum]);

	m_goalRankFont[pNum] = NewGO<FontRender>(0);
	m_goalRankFont[pNum]->Init(L"", { PLAYER_START_POSITION[pNum].x,PLAYER_START_POSITION[pNum].y });
	m_goalRankFont[pNum]->Deactivate();

	//Quaternion q = g_quatIdentity;
	//q.Quaternion::SetRotationDegY(0);
	//m_modelRender[pNum]->SetRotation(q);

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

	if (m_flagDoingAnimation[pNum] == true || m_stage->GetmActiveOperation(pNum) == false) {
		return;
	}

	//�Q�}�X�i��
	if (g_pad[pNum]->IsTrigger(enButtonA) == true) {
		m_stage->MoveBlock(pNum, MOVE_BUTTON_A);
		m_flagDoingAnimation[pNum] = true;
	}
	//�P�}�X�i��
	else if (g_pad[pNum]->IsTrigger(enButtonB) == true) {
		m_stage->MoveBlock(pNum, MOVE_BUTTON_B);
		m_flagDoingAnimation[pNum] = true;
	}
}

void Player::Animation(const int pNum)
{
	if (m_flagDoingAnimation[pNum] == false) {
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
		m_flagDoingAnimation[pNum] = false;
		m_timerAnimation[pNum] = 0;
	}
}