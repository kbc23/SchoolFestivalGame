#pragma once
#include "model_render.h"
#include "font_render.h"

class Stage;

class Player : public IGameObject
{
public:
	Player();

	~Player();
	/**
	 * @brief �f�X�g���N�^�̏������v���C���[���ƂɌʂł����Ȃ��B
	 * @param pNum �v���C���[�ԍ�
	*/
	void DeleteIndividual(const int pNum);

	bool Start() override final;
	/**
	 * @brief Start�֐��̏������v���C���[���ƂɌʂł����Ȃ��B
	 * @param pNum �v���C���[�ԍ�
	 * @return ����������ɏI���������ǂ���
	*/
	bool StartIndividual(const int pNum);

	void Update() override final;

	////////////////////////////////////////////////////////////
    // �v���C���[�̑��쏈��
    ////////////////////////////////////////////////////////////

	/**
	 * @brief �v���C���[�̑��쏈���̓y��
	 * @param pNum �v���C���[�ԍ�
	*/
	void Controller(const int pNum);

	////////////////////////////////////////////////////////////
	// �v���C���[�̃A�j���[�V����
	////////////////////////////////////////////////////////////

	/**
	 * @brief �v���C���[�̃A�j���[�V�����̏���
	 * @param pNum �v���C���[�ԍ�
	*/
	void Animation(const int pNum);

	/**
	 * @brief �W�����v���̃A�j���[�V����
	 * @param pNum �v���C���[�ԍ�
	*/
	void JumpAnimation(const int pNum);

	/**
	 * @brief ����s���̃A�j���[�V����
	 * @param pNum �v���C���[�ԍ�
	*/
	void ImpossibleOperationAnimation(const int pNum);


public: //Set�֐�
	/**
	 * @brief �v���C���[�̃��f����X���W�����ɉ�]������B
	 * @param pNum �v���C���[�ԍ�
	 * @param f ��]��
	*/
	void SetRotationX(const int pNum, float f) {
		m_modelRender[pNum]->SetRotationX(f);
	}

	/**
	 * @brief �v���C���[������ł��邩�ǂ�����Set�֐�
	 * @param pNum �v���C���[�ԍ�
	 * @param b ����ł��邩�ǂ���
	*/
	void SetActivePlayer(const int pNum, const bool b) {
		m_activePlayer[pNum] = b;
	}

	/**
	 * @brief �S�[�����̏��ʂ��m��
	 * @param pNum �v���C���[�ԍ�
	 * @param rank ����
	*/
	void SetGoalRanking(const int pNum, const int rank) {
		m_goalRanking[pNum] = rank;

		SetAndActivateGoalRankFont(pNum, rank);
	}

	/**
	 * @brief ���ʂ̕`�揈�����߂̒l�̐ݒ�ƕ`���Ԃɂ���B
	 * @param pNum �v���C���[�ԍ�
	 * @param rank ����
	*/
	void SetAndActivateGoalRankFont(const int pNum, const int rank)
	{
		m_goalRankFont[pNum]->SetText(rank);
		m_goalRankFont[pNum]->Activate();
	}

	/**
	 * @brief �S�[���������ǂ�����Set�֐�
	 * @param pNum �v���C���[�ԍ�
	 * @param b �S�[���������ǂ���
	*/
	void SetFlagGoal(const int pNum, const bool b)
	{
		m_flagGoal[pNum] = b;
	}


public: //Get�֐�
	/**
	 * @brief m_activePlayer[pNum]��Get�֐�
	 * @param pNum �v���C���[�ԍ�
	 * @return �v���C���[�ԍ�[pNum]���A����ł��邩�ǂ���
	*/
	bool GetActivePlayer(const int pNum) {
		return m_activePlayer[pNum];
	}

	/**
	 * @brief m_flagAnimationJump[pNum]��Get�֐�
	 * @param pNum �v���C���[�ԍ�
	 * @return �v���C���[�ԍ�[pNum]���A�W�����v�A�j���[�V�������Đ����Ă��邩�ǂ���
	*/
	bool GetmFlagAnimationJump(const int pNum) {
		return m_flagAnimationJump[pNum];
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



private: //data menber
	Stage* m_stage = nullptr;

	//�v���C���[���ő�S�l������A�����o�ϐ��͗v�f���S�̔z��ŊǗ�����B
	//ModelRender* m_modelRender = nullptr;
	ModelRender* m_modelRender[PlayerNumberMax] = { nullptr }; //���f�������_�[
	FontRender* m_goalRankFont[PlayerNumberMax] = { nullptr }; //�t�H���g�����_�[

	//Vector3 m_position[PlayerNumberMax]; //�L�����N�^�[�̍��W
	//Quaternion m_rotation[PlayerNumberMax] = { Quaternion::Identity }; //�L�����N�^�[�̉�]���

	bool m_activePlayer[PlayerNumberMax] = { true, true, true, true }; //�v���C���[������ł��邩�ǂ���

	int m_maxPlayer = PlayerNumberMax; //�v���C���[�̍ő吔
	//���v���C���[���I����ʂ��쐬������A�������炱�̕ϐ��Ƀv���C���[�̍ő吔��������B

	int m_goalRanking[PlayerNumberMax] = { 0, 0, 0, 0 }; //�v���C���[�̃S�[������
	bool m_flagGoal[PlayerNumberMax] = { false, false, false, false }; //�S�[��������

	bool m_flagAnimationJump[PlayerNumberMax] = { false, false, false, false }; //�W�����v�A�j���[�V��������
	//bool m_flagDoingAnimation[PlayerNumberMax] = { false, false, false, false }; //�A�j���[�V��������
	int m_timerAnimation[PlayerNumberMax] = { 0, 0, 0, 0 }; //�A�j���[�V�����̃^�C�}�[
};