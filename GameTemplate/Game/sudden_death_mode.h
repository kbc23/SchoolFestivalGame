#pragma once

class Stage;
class Score;
class Player;
class GameStartCountdown;
class CPUPlayerController;

class SuddenDeathMode : public IGameObject
{
public:
	SuddenDeathMode();
	~SuddenDeathMode();
	bool Start() override final;
	void Init();
	void Finish();
	void Update() override final;



public:
	/**
	 * @brief ���̃��E���h�Ɉړ����鏈��
	*/
	void NextRound();


public: //Get�֐�
	/**
	 * @brief �T�h���f�X���[�h���ǂ���
	 * @return �T�h���f�X���[�h���ǂ���
	*/
	const bool GetFlagSuddenDeathMode() const
	{
		return m_flagSuddenDeathMode;
	}


public: //Set�֐�
	/**
	 * @brief �T�h���f�X���[�h���ǂ������Z�b�g
	 * @param flagSuddenDeathMode �T�h���f�X���[�h���ǂ���
	*/
	void SetFlagSuddenDeathMode(const bool flagSuddenDeathMode)
	{
		m_flagSuddenDeathMode = flagSuddenDeathMode;
	}


private: //data member
	////////////////////////////////////////////////////////////
	// �N���X�̃I�u�W�F�N�g
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// FindGO
	//////////////////////////////

	Stage* m_findStage = nullptr;
	Score* m_findScore = nullptr;
	Player* m_findPlayer = nullptr;
	GameStartCountdown* m_findGameStartCountdown = nullptr;
	CPUPlayerController* m_findCPUPlayerController = nullptr;

	////////////////////////////////////////////////////////////
	// �t���O
	////////////////////////////////////////////////////////////

	bool m_flagSuddenDeathMode = false; //�T�h���f�X���[�h��

	bool m_finishSuddenDeath = false; //�T�h���f�X���[�h���I��������

	////////////////////////////////////////////////////////////
	// ���̑�
	////////////////////////////////////////////////////////////
};

