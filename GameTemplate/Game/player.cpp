#include "stdafx.h"
#include "player.h"

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

	return true;
}

bool Player::StartIndividual(const int p_num)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�

	m_modelRender[p_num] = NewGO<ModelRender>(0);
	m_modelRender[p_num]->Init("Assets/modelData/unityChan.tkm");
	m_modelRender[p_num]->SetPosition({ 0.0f, 10.0f * p_num, 0.0f });

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

	//m_skinModelRender[p_num]->SetPosition(m_position[p_num]);
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