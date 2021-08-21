#pragma once
#include "model_render.h"
#include "constant.h"

class Stage;
class SuddenDeathMode;

class CPUPlayerController : public IGameObject
{
public:
	CPUPlayerController();
	~CPUPlayerController();
	bool Start() override final;
	void Init();
	void Finish();
	void Update() override final;

	/**
	 * @brief CPU�̑��쏈��
	 * @param playerNum �v���C���[�ԍ�
	 * @return CPU�����{�^���̑����������
	*/
	int CPUController(const int playerNum);

	/**
	 * @brief CPU�̃R���g���[���[�̑�����~���鏈��
	 * @param playerNum �v���C���[�ԍ�
	 * @return CPU�̃R���g���[���[�̑�����~���邩
	*/
	bool StopController(const int playerNum);

	/**
	 * @brief CPU���~�X������Ƃ��̑���
	 * @param playerNum �v���C���[�ԍ�
	 * @return CPU�����{�^���̑����������
	*/
	const int& MissMove(const int playerNum);

	/**
	 * @brief CPU���~�X�����Ȃ��Ƃ��̑���
	 * @param playerNum �v���C���[�ԍ�
	 * @return CPU�����{�^���̑����������
	*/
	const int& OKMove(const int playerNum);



	void NextRound();



public: //Get�֐�

public://Set�֐�
	/**
	 * @brief CPU�̋������擾
	 * @param level CPU�̋���
	*/
	void SetCPULevel(const con::CPULevel& level)
	{
		m_CPULevel = level;
	}

	/**
	 * @brief �F�̃u���b�N�̂Ƃ���Ń~�X�����������擾
	 * @param playerNum �v���C���[�ԍ�
	 * @param flagMiss �F�̃u���b�N�̂Ƃ���Ń~�X��������
	*/
	void SetFlagBlueBlockMiss(int playerNum, bool flagMiss)
	{
		m_flagBlueBlockMiss[playerNum] = flagMiss;
	}

	/**
	 * @brief CPU�̃R���g���[���[�̏������~���邩���擾
	 * @param playerNum �v���C���[�ԍ�
	 * @param flagStop CPU�̃R���g���[���[�̏������~���邩
	*/
	void SetStopController(int playerNum, bool flagStop)
	{
		m_stopController[playerNum] = flagStop;
	}



private: //constant
	static const int m_MISS_INVALID = 3; //�T�h���f�X���[�h�̂Ƃ��Ƀ~�X�����Ȃ��悤�ɂ����


private: //data menber
	////////////////////////////////////////////////////////////
	// �N���X�̃I�u�W�F�N�g
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// FindGO
	//////////////////////////////

	Stage* m_findStage = nullptr;
	SuddenDeathMode* m_findSuddenDeathMode = nullptr;

	////////////////////////////////////////////////////////////
	// �t���O�֘A
	////////////////////////////////////////////////////////////

	bool m_flagBlueBlockMiss[con::PlayerNumberMax] = { false,false,false,false }; //�u���b�N�Ń~�X��������

	bool m_stopController[con::PlayerNumberMax] = { false,false,false,false }; //CPU�̃R���g���[���[�̏������~�߂邩

	int m_missInvalidCount[con::PlayerNumberMax] = 
	{ m_MISS_INVALID,m_MISS_INVALID,m_MISS_INVALID,m_MISS_INVALID }; //�T�h���f�X���̃~�X�𖳌��ɂ����

	////////////////////////////////////////////////////////////
	// ���̑�
	////////////////////////////////////////////////////////////

	con::CPULevel m_CPULevel = con::easy; //CPU�̋���
	int m_stopControllerTimer[con::PlayerNumberMax] = { 0,0,0,0 }; //CPU�̃R���g���[���[�̏������~�߂Ă�Ƃ��̃^�C�}�[

};