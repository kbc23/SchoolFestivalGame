#include "stdafx.h"
#include "stage.h"

#include "player.h"

namespace {
	//////////////////////////////
	// �t�@�C���p�X
	//////////////////////////////

	const char* FILE_PATH_CHAEACTER_MODEL = "Assets/modelData/unityChan.tkm";

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
	m_modelRender[pNum]->Init(FILE_PATH_CHAEACTER_MODEL);
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
// Update�֐�
////////////////////////////////////////////////////////////

void Player::Update()
{
	//�v���C���[�̑���
	Controller();

	//�v���C���[�֘A�̕`�揈��
	Draw();
}

////////////////////////////////////////////////////////////
// Draw�֐��֘A
////////////////////////////////////////////////////////////

void Player::Draw()
{
	//�v���C���[���Ƃɕ`��
	for (int i = 0; i < m_maxPlayer; i++) {
		DrawIndividual(i);
	}
}

void Player::DrawIndividual(const int pNum)
{

}

////////////////////////////////////////////////////////////
// �v���C���[�̑��쏈��
////////////////////////////////////////////////////////////

void Player::Controller()
{
	//�v���C���[���Ƃɑ���
	for (int i = 0; i < m_maxPlayer; i++) {
		if (m_activePlayer[i] == true) {
			ControllerIndividual(i);
		}
	}
}

void Player::ControllerIndividual(const int pNum)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	//�Q�}�X�i��
	if (g_pad[pNum]->IsTrigger(enButtonA) == true) {
		m_stage->MoveBlock(pNum, MOVE_BUTTON_A);
	}
	//�P�}�X�i��
	else if (g_pad[pNum]->IsTrigger(enButtonB) == true) {
		m_stage->MoveBlock(pNum, MOVE_BUTTON_B);
	}
	


}