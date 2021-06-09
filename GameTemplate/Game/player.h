#pragma once
#include "model_render.h"

class Stage;

class Player : public IGameObject
{
public:
	Player();

	~Player();
	void DeleteIndividual(const int pNum); //�f�X�g���N�^�̏������v���C���[���ƂɌʂł����Ȃ��B

	bool Start();
	bool StartIndividual(const int pNum); //Start�֐��̏������v���C���[���ƂɌʂł����Ȃ��B

	void Update();

	
	void Draw();
	void DrawIndividual(const int pNum); //Draw�֐��̏������v���C���[���ƂɌʂł����Ȃ��B

	////////////////////////////////////////////////////////////
    // �v���C���[�̑��쏈��
    ////////////////////////////////////////////////////////////

	/// <summary>
	/// �v���C���[�̑��쏈���̓y��
	/// </summary>
	void Controller();
	
	/// <summary>
	/// �v���C���[�̑��쏈��
	/// </summary>
	/// <param name="pNum">�v���C���[�ԍ�</param>
	void ControllerIndividual(const int pNum);

public: //Set�֐�
	//Set�֐��̈����ɂ���[int i]�̓v���C���[�ԍ��ł���B

	//////////////////////////////
	// m_position��Set�֐�
	//////////////////////////////

	void SetPosition(int i, const Vector3 v) {
		//m_position[i] = v;
	}


	
	void SetActivePlayer(int i, bool b) {
		m_activePlayer[i] = b;
	}







public: //Get�֐�
	//Get�֐��̈����ɂ���[int i]�̓v���C���[�ԍ��ł���B

	//////////////////////////////
	// m_position��Get�֐�
	//////////////////////////////

	Vector3 GetPosition(int i) {
		//return m_position[i];
	}



	float GetActivePlayer(int i) {
		return m_activePlayer[i];
	}



public: //enum
	//�v���C���[�̔ԍ�
	enum PlayerNumber
	{
		player_1,
		player_2,
		player_3,
		player_4,
		PlayerNumberMax
	};



private: //�����o�ϐ�
	Stage* m_stage = nullptr;

	//�v���C���[���ő�S�l������A�����o�ϐ��͗v�f���S�̔z��ŊǗ�����B
	//ModelRender* m_modelRender = nullptr;
	ModelRender* m_modelRender[PlayerNumberMax] = { nullptr }; //�X�L�����f�������_���[�B

	Vector3 m_position[PlayerNumberMax]; //�L�����N�^�[�̍��W
	Quaternion m_rotation[PlayerNumberMax] = { Quaternion::Identity }; //�L�����N�^�[�̉�]���

	bool m_activePlayer[PlayerNumberMax] = { true, true, true, true }; //�v���C���[������ł��邩�ǂ���
	//�����̓e�X�g�̂���m_activePlayer[player_1]����true

	int m_maxPlayer = PlayerNumberMax; //�v���C���[�̍ő吔
	//���v���C���[���I����ʂ��쐬������A�������炱�̕ϐ��Ƀv���C���[�̍ő吔��������B

};