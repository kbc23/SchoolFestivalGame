#pragma once
#include "model_render.h"
#include "font_render.h"
#include "sound_SE.h"
#include "constant.h"

class Game;
class Stage;
class EnemyAI;//tuika



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

	////////////////////////////////////////////////////////////
	// ���t���[���̏���
	////////////////////////////////////////////////////////////

	void Update() override final;

	//////////////////////////////
    // �v���C���[�̑��쏈��
	//////////////////////////////

	/**
	 * @brief �v���C���[�̑��쏈���̓y��
	 * @param pNum �v���C���[�ԍ�
	*/
	void Controller(const int pNum);

	//////////////////////////////
	// �v���C���[�̃A�j���[�V����
	//////////////////////////////

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

	/**
	 * @brief m_flagGoal[pNum]��Get�֐�
	 * @param pNum �v���C���[�ԍ�
	 * @return �v���C���[���S�[���������ǂ���
	*/
	bool GetFlagGoal(const int pNum) {
		return m_flagGoal[pNum];
	}

	int GetTimerAnimation(const int pNum)
	{
		return m_timerAnimation[pNum];
	}

	bool GetFlagAnimationJump(const int pNum)
	{
		return m_flagAnimationJump[pNum];
	}

	bool GetGameEnd()//�Q�[���I��
	{
		return m_gameEnd;
	}

	bool GetModel(int pNum)//�Q�[���I��
	{
		return m_modelRender[pNum];
	}

public: //Set�֐�

	void DownPositionY(const int& pNum, const float& f)
	{
		m_modelRender[pNum]->DownPositionY(f);
	}

	void ResetPositionY(const int& pNum)
	{
		m_modelRender[pNum]->ResetPositionY();
	}

	/**
	 * @brief m_modelRender[pNum]��X���̉�]�ʂ�ݒ肷��Set�֐�
	 * @param pNum �v���C���[�ԍ�
	 * @param f ��]��
	*/
	void SetRotationX(const int& pNum, const float& f) {
		m_modelRender[pNum]->SetRotationX(f);
	}

	/**
	 * @brief m_activePlayer[pNum]��Set�֐�
	 * @param pNum �v���C���[�ԍ�
	 * @param b ����ł��邩�ǂ���
	*/
	void SetActivePlayer(const int& pNum, const bool& b) {
		m_activePlayer[pNum] = b;
	}

	/**
	 * @brief �S�[�����̏��ʂ��m��
	 * @param pNum �v���C���[�ԍ�
	 * @param rank ����
	*/
	void SetGoalRanking(const int& pNum, const int& rank) {
		m_goalRanking[pNum] = rank;

		SetAndActivateGoalRankFont(pNum, rank);
	}

	/**
	 * @brief ���ʂ̕`�揈�����߂̒l�̐ݒ�ƕ`���Ԃɂ���B
	 * @param pNum �v���C���[�ԍ�
	 * @param rank ����
	*/
	void SetAndActivateGoalRankFont(const int& pNum, const int& rank)
	{
		m_fontGoalRank[pNum]->SetText(rank);
		m_fontGoalRank[pNum]->Activate();
		++m_goalPlayer;
	}

	/**
	 * @brief �S�[���������ǂ�����Set�֐�
	 * @param pNum �v���C���[�ԍ�
	 * @param b �S�[���������ǂ���
	*/
	void SetFlagGoal(const int& pNum, const bool& b)
	{
		m_flagGoal[pNum] = b;
	}

	/**
	 * @brief ���삷��v���C���[�̐l����ۑ�����ϐ���Set�֐�
	 * @param i ���삷��v���C���[�̐l��
	*/
	void SetMaxPlayer(const int& i)
	{
		m_maxPlayer = i;
	}

	void SetAnimationIdle(const int& pNum)
	{
		m_modelRender[pNum]->PlayAnimation(idle);
	}

	void SetAnimationFall(const int& pNum)
	{
		m_modelRender[pNum]->PlayAnimation(fall);
		m_seFall->Play(false);
	}

	void SetAnimationSrip(const int& pNum)
	{
		m_modelRender[pNum]->PlayAnimation(srip);
		m_seSrip->Play(false);
	}

	/**
* @brief cpu���W�����v�������肷��ϐ���Set�֐�
* @param i �W�����v�����^�U����
*/
	void SetFlagAnimationJump(int pNum, const bool i)
	{
		m_flagAnimationJump[pNum] = i;
	}//tuika

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
		Animation_Max
	};

	AnimationClip m_animationPlayer[Animation_Max];



private: //data menber
	////////////////////////////////////////////////////////////
	// �N���X�̃I�u�W�F�N�g
	////////////////////////////////////////////////////////////

	Stage* m_stage = nullptr;
	Game* m_game = nullptr;
	EnemyAI* m_enemyAI = nullptr;//tuika

	ModelRender* m_modelRender[con::PlayerNumberMax] = { nullptr };	//�v���C���[�L�����N�^�[�̃��f��
	FontRender* m_fontGoalRank[con::PlayerNumberMax] = { nullptr };	//�S�[�����ʂ�\������t�H���g
	FontRender* m_fontEnd = nullptr;							//�u�I���I�v��\������t�H���g
	SoundSE* m_seFall = nullptr;
	SoundSE* m_seSrip = nullptr;

	////////////////////////////////////////////////////////////
	// �v���C���[���
	////////////////////////////////////////////////////////////

	int m_activePlayer[con::PlayerNumberMax] = { true, true, true, true };	//���̃v���C���[�͑��삵�Ă��邩
	int m_maxPlayer = con::PlayerNumberMax;									//�v���C���[�̍ő吔

	int m_goalRanking[con::PlayerNumberMax] = { 0, 0, 0, 0 };				//�v���C���[�̃S�[������
	bool m_flagGoal[con::PlayerNumberMax] = { false, false, false, false };	//�S�[��������
	int m_goalPlayer = 0;					
	
	int m_endTimer = 0;//�S�[�����Ă���̎���tuika

	bool m_gameEnd = false;//�S�[�������v���C���[�̐l��tuika

	////////////////////////////////////////////////////////////
	// �^�C�}�[�֘A
	////////////////////////////////////////////////////////////

	bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//�W�����v�A�j���[�V��������
	int m_timerAnimation[con::PlayerNumberMax] = { 0, 0, 0, 0 };						//�A�j���[�V�����̃^�C�}�[
};