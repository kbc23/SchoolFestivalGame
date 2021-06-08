#include "stdafx.h"
#include "player.h"

namespace {
	const Vector3 PLAYER_START_POSITION[Player::PlayerNumberMax] = {
		{ 390.0f, 0.0f, -250.0f },	//�v���C���[�P
		{ 130.0f, 0.0f, -250.0f },	//�v���C���[�Q
		{ -130.0f, 0.0f, -250.0f },	//�v���C���[�R
		{ -390.0f, 0.0f, -250.0f }	//�v���C���[�S
	};
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

void Player::DeleteIndividual(const int p_num)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	DeleteGO(m_modelRender[p_num]);

	//DeleteGO(m_skinModelRender[p_num]);
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
	
	m_testRender = NewGO<ModelRender>(0);
	m_testRender->Init("Assets/modelData/unityChan.tkm");
	m_testRender->SetPosition({ 0.0f,-50.0f,0.0f });
	m_testRender->SetScale({ 0.5f,0.5f,0.5f });
	//m_testRender->SetPosition(PLAYER_START_POSITION[p_num]);

	return true;
}

bool Player::StartIndividual(const int p_num)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	m_modelRender[p_num] = NewGO<ModelRender>(0);
	m_modelRender[p_num]->Init("Assets/modelData/unityChan.tkm");
	m_modelRender[p_num]->SetPosition(PLAYER_START_POSITION[p_num]);
	//Quaternion q = g_quatIdentity;
	//q.Quaternion::SetRotationDegY(0);
	//m_modelRender[p_num]->SetRotation(q);

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
		if (m_activePlayer[i] == true) {
			DrawIndividual(i);
		}
	}
}

void Player::DrawIndividual(const int p_num)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	//m_modelRender[p_num]->SetPosition({ 0.0f, 0.0f, 0.0f });
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

void Player::ControllerIndividual(const int p_num)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	


}