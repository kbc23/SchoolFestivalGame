#pragma once
#include "model_render.h"
#include "sprite_render.h"
#include "font_render.h"
#include "sound_SE.h"
#include "constant.h"

class MainProcessing;
class Stage;
class CPUPlayerController;
class SuddenDeathMode;
class GameStartCountdown;
class Result;
class Rank;

class Player : public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override final;
	void Init();
	void Finish();

	////////////////////////////////////////////////////////////
	// ���t���[���̏���
	////////////////////////////////////////////////////////////

	void Update() override final;

	//////////////////////////////
    // �v���C���[�̑��쏈��
	//////////////////////////////

	/**
	 * @brief �L�����N�^�[�̑��쏈��
	 * @param pNum �v���C���[�ԍ�
	*/
	void Controller(const int playerNum);

	/**
	 * @brief �v���C���[�̑��쏈��
	 * @param playerNum �v���C���[�ԍ�
	*/
	void ControllerPlayer(const int playerNum);

	/**
	 * @brief CPU�̑��쏈��
	 * @param playerNum �v���C���[�ԍ�
	*/
	void ControllerCPU(const int playerNum);

	//////////////////////////////
	// �v���C���[�̃A�j���[�V����
	//////////////////////////////

	/**
	 * @brief �v���C���[�̃A�j���[�V�����̏���
	 * @param pNum �v���C���[�ԍ�
	*/
	void Animation(const int playerNum);

	/**
	 * @brief �W�����v���̃A�j���[�V����
	 * @param pNum �v���C���[�ԍ�
	*/
	void JumpAnimation(const int playerNum);

	/**
	 * @brief �F�̃u���b�N�̏�ɍs�����Ƃ��̃A�j���[�V����
	 * @param playerNum �v���C���[�ԍ�
	*/
	void BlueBlockAnimation(const int playerNum);

	/**
	 * @brief �F�̃u���b�N�̏�ɍs�����Ƃ��A�T�h���f�X���̃A�j���[�V����
	 * @param playerNum �v���C���[�ԍ�
	*/
	void SuddenDeathBlueBlockAnimation(const int playerNum);

	/**
	 * @brief ���F�̃u���b�N�̏�ɍs�����Ƃ��̃A�j���[�V����
	 * @param playerNum �v���C���[�ԍ�
	*/
	void YellowBlockAnimation(const int playerNum);

	/**
	 * @brief �F�̃u���b�N�̏�ɍs�����Ƃ��A�T�h���f�X���̃A�j���[�V����
	 * @param playerNum �v���C���[�ԍ�
	*/
	void SuddenDeathYellowBlockAnimation(const int playerNum);

	////////////////////////////////////////////////////////////
	// ���E���h�ύX�̏���
	////////////////////////////////////////////////////////////

	/**
	 * @brief ���E���h�ύX�̏���
	*/
	void NextRound();


private: //�ǉ�
	void GoalAnimation(const int playerNum);


public: //Get�֐�
	/**
	 * @brief m_activePlayer[pNum]��Get�֐�
	 * @param playerNum �v���C���[�ԍ�
	 * @return �v���C���[�ԍ�[pNum]���A����ł��邩�ǂ���
	*/
	const bool GetActivePlayer(const int playerNum) const
	{
		return m_activePlayer[playerNum];
	}

	/**
	 * @brief m_flagAnimationJump[pNum]��Get�֐�
	 * @param playerNum �v���C���[�ԍ�
	 * @return �v���C���[�ԍ�[pNum]���A�W�����v�A�j���[�V�������Đ����Ă��邩�ǂ���
	*/
	const bool GetmFlagAnimationJump(const int playerNum) const
	{
		return m_flagAnimationJump[playerNum];
	}

	/**
	 * @brief �T�h���f�X���[�h���I�����������擾
	 * @return �T�h���f�X���[�h���I��������
	*/
	const bool GetFinishSuddenDeath() const
	{
		return m_finishSuddenDeath;
	}

	/**
	 * @brief �L�����N�^�[�̃��f�����`�悳��Ă��邩���擾
	 * @param playerNum �v���C���[�ԍ�
	 * @return �L�����N�^�[�̃��f�����`�悳��Ă��邩
	*/
	const bool GetModelIsActive(const int playerNum) const
	{
		return m_modelCharacter[playerNum]->IsActive();
	}

	/**
	 * @brief �v���C���[������ł��Ȃ���Ԃ����擾
	 * @param playerNum �v���C���[�ԍ�
	 * @return �v���C���[������ł��Ȃ���Ԃ�
	*/
	const bool GetStopController(const int playerNum) const
	{
		return m_stopController[playerNum];
	}



public: //Set�֐�

	/**
	 * @brief �v���C���[���f����Y���W��������
	 * @param playerNum Y���W��������v���C���[�ԍ�
	 * @param downPosition �������
	*/
	void DownPositionY(const int playerNum, const float downPosition)
	{
		m_modelCharacter[playerNum]->DownPositionY(downPosition);
	}

	/**
	 * @brief �v���C���[���f����Y���W��0.0f�Ƀ��Z�b�g����
	 * @param playerNum Y���W��0.0f�Ƀ��Z�b�g����v���C���[�ԍ�
	*/
	void ResetPositionY(const int playerNum)
	{
		m_modelCharacter[playerNum]->ResetPositionY();
	}

	/**
	 * @brief �v���C���[���f����X���̉�]�ʂ�ύX����
	 * @param playerNum �v���C���[�ԍ�
	 * @param rotation ��]��
	*/
	void SetRotationX(const int playerNum, const float rotation)
	{
		m_modelCharacter[playerNum]->SetRotationX(rotation);
	}

	/**
	 * @brief m_activePlayer[pNum]��Set�֐�
	 * @param playerNum �v���C���[�ԍ�
	 * @param flagActive ����ł��邩�ǂ���
	*/
	void SetActivePlayer(const int playerNum, const bool flagActive)
	{
		m_activePlayer[playerNum] = flagActive;
	}

	/**
	 * @brief ���삷��v���C���[�̐l����ۑ�����ϐ���Set�֐�
	 * @param maxPlayer ���삷��v���C���[�̐l��
	*/
	void SetMaxPlayer(const int maxPlayer)
	{
		m_maxPlayer = maxPlayer;
	}

	/**
	 * @brief ���l�S�[�����������Z�b�g
	 * @param goalPlayer ���l�S�[��������
	*/
	void SetGoalPlayer(const int goalPlayer)
	{
		m_goalPlayer = goalPlayer;
	}

	/**
	 * @brief �v���C���[�̃��f���̃A�j���[�V������[idle]�ɐݒ肷��
	 * @param playerNum �v���C���[�ԍ�
	*/
	void SetAnimationIdle(const int playerNum)
	{
		m_modelCharacter[playerNum]->PlayAnimation(idle);
	}

	/**
	 * @brief �v���C���[�̃��f���̃A�j���[�V������[fall]�ɐݒ肵�ASE���Đ�����
	 * @param playerNum �v���C���[�ԍ�
	*/
	void SetAnimationFall(const int playerNum)
	{
		m_modelCharacter[playerNum]->PlayAnimation(fall);
		m_seFall->Play(false);
	}

	/**
	 * @brief �v���C���[�̃��f���̃A�j���[�V������[srip]�ɐݒ肵�ASE���Đ�����
	 * @param playerNum �v���C���[�ԍ�
	*/
	void SetAnimationSrip(const int playerNum)
	{
		m_modelCharacter[playerNum]->PlayAnimation(srip);
		m_seSrip->Play(false);
	}

	/**
	 * @brief �v���C���[�̃��f���̃A�j���[�V������[win]�ɐݒ肷��
	 * @param playerNum �v���C���[�ԍ�
	*/
	void SetAnimationWin(const int playerNum)
	{
		m_modelCharacter[playerNum]->PlayAnimation(win);
	}

	/**
	 * @brief �擾���E���h���𑝉�����B�R���E���h�擾�����ꍇ�A�T�h���f�X���[�h���I������B
	 * @param playerNum �v���C���[�ԍ�
	*/
	//void AddRoundPoint(const int playerNum)
	//{
	//	++m_roundPoint[playerNum];

	//	//�R���E���h�擾�����Ƃ�
	//	if (m_roundPoint[playerNum] == 3) {
	//		//�T�h���f�X���[�h�I��
	//		m_finishSuddenDeath = true;
	//	}
	//}

	/**
	 * @brief �v���C���[�̃R���g���[���[�̑���\��Ԃ�ύX
	 * @param playerNum �v���C���[�ԍ�
	 * @param stopController �R���g���[���[������\��
	*/
	void SetStopController(const int playerNum, const bool stopController)
	{
		m_stopController[playerNum] = stopController;
	}

	/**
	 * @brief �v���C���[�̃��f���̃u���b�N�ˑ��̃A�j���[�V�����Đ��󋵂�ύX����
	 * @param playerNum �v���C���[�ԍ�
	 * @param block �u���b�N�̎��
	*/
	void SetNowAnimationBlock(const int playerNum, const con::BlockData& block)
	{
		m_nowAnimationBlock[playerNum] = block;

		//SE�̍Đ�
		if (block == con::BlockData::blueBlock) {
			m_seFall->Play(false); //��������SE���Đ�
		}
		if (block == con::BlockData::yellowBlock) {
			m_seSrip->Play(false); //�X���b�v����SE���Đ�
		}
	}


private: //enum
	enum AnimationEnum
	{
		idle,
		jump,
		fall,
		srip,
		win,
		stand,
		lose,
		AnimationMax
	};

	AnimationClip m_animationPlayer[AnimationMax];


private: //data menber
	////////////////////////////////////////////////////////////
	// �N���X�̃I�u�W�F�N�g
	////////////////////////////////////////////////////////////

	//////////////////////////////
	// NewGO
	//////////////////////////////

	ModelRender* m_modelCharacter[con::PlayerNumberMax] = { nullptr };	//�v���C���[�L�����N�^�[�̃��f��
	SoundSE* m_seJump = nullptr; //�W�����v����SE
	SoundSE* m_seFall = nullptr; //��������SE
	SoundSE* m_seSrip = nullptr; //�X���b�v����SE

	//////////////////////////////
	// FindGO
	//////////////////////////////

	Stage* m_findStage = nullptr;
	MainProcessing* m_findMainProcessing = nullptr;
	CPUPlayerController* m_findCPUPlayerController = nullptr;
	SuddenDeathMode* m_findSuddenDeathMode = nullptr;
	GameStartCountdown* m_findGameStartCountdown = nullptr;
	Result* m_findResult = nullptr;
	Rank* m_findRank = nullptr;

	////////////////////////////////////////////////////////////
	// �v���C���[���
	////////////////////////////////////////////////////////////

	int m_activePlayer[con::PlayerNumberMax] = { true, true, true, true };	//���̃v���C���[�͑��삵�Ă��邩
	int m_maxPlayer = con::PlayerNumberMax;									//�v���C���[�̍ő吔

	int m_goalPlayer = 0;				
	
	int m_endTimer = 0; //�S�[�����Ă���̎���

	bool m_gameEnd = false; //�S�[�������v���C���[�̐l��

	bool m_stopController[con::PlayerNumberMax] = { false,false,false,false }; //����s�\��
	
	con::BlockData m_nowAnimationBlock[con::PlayerNumberMax] =					//�v���C���[�̌��݂̃A�j���[�V����
	{ con::greenBlock,con::greenBlock ,con::greenBlock ,con::greenBlock };

	bool m_flagStopAnimation[con::PlayerNumberMax] = { false,false,false,false }; //�A�j���[�V�����̏������~�܂��Ă��邩

	////////////////////////////////////////////////////////////
	// �^�C�}�[�֘A
	////////////////////////////////////////////////////////////

	bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//�W�����v�A�j���[�V��������
	int m_timerAnimation[con::PlayerNumberMax] = { 0, 0, 0, 0 };						//�A�j���[�V�����̃^�C�}�[

	////////////////////////////////////////////////////////////
	// �T�h���f�X���[�h�̃����o�ϐ�
	////////////////////////////////////////////////////////////

	bool m_finishSuddenDeath = false;							//�T�h���f�X���[�h���I��������
};