#pragma once
#include "model_render.h"
#include "sprite_render.h"
#include "font_render.h"
#include "sound_SE.h"
#include "constant.h"

class MainProcessing;
class Stage;
class EnemyAI;
class Rule1;


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

	void Init() override final;
	void Finish() override final;

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

	//////////////////////////////
	// ���E���h�ύX�̏���
	//////////////////////////////

	/**
	 * @brief ���E���h�ύX�̏���
	*/
	void NextRound();

	////////////////////////////////////////////////////////////
	// �T�h���f�X���[�h�̊֐�
	////////////////////////////////////////////////////////////
	void SuddenDeathRank();


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



	const int& GetGoalRanking(const int& pNum)
	{
		return m_goalRanking[pNum];
	}

	const int& GetRoundPoint(const int& pNum)
	{
		return m_roundPoint[pNum];
	}

	const bool& GetFinishSuddenDeath()
	{
		return m_finishSuddenDeath;
	}
	int GetBlueMiss(const int pNum)
	{
		return m_bluemiss[pNum];
	}

	bool GetModelIsActive(const int& pNum)
	{
		return m_modelRender[pNum]->IsActive();
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
		m_spriteGoalRank[pNum][rank - 1]->Activate();
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
	void SetGoalPlayer(const int o)
	{
		m_goalPlayer = o;
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

	void SetAnimationWin(const int& pNum)
	{
		m_modelRender[pNum]->PlayAnimation(win);
	}

	/**
	 * @brief cpu���W�����v�������肷��ϐ���Set�֐�
	 * @param i �W�����v�����^�U����
	*/
	void SetFlagAnimationJump(const int& pNum, const bool& i)
	{
		m_flagAnimationJump[pNum] = i;
		m_seJump->Play(false);
	}//tuika


	void SetCPUJumpFlag(const int& pNum, const bool& i) {
		m_EJumpFlag[pNum] = i;
	}

	void AddRoundPoint(const int& pNum)
	{
		++m_roundPoint[pNum];

		if (m_roundPoint[pNum] == 3) {
			m_finishSuddenDeath = true;
		}
	}
	void SetDifficultyLevel(const int& i) {//��Փx
		m_difficultyLevel = i;
	}

	void SetBlueMiss(const int& pNum, const bool& i) {
		m_bluemiss[pNum] = i;
	}

	void PlayerModelDeactivate(const int& pNum)
	{
		m_modelRender[pNum]->Deactivate();
	}

	void SetRule1NewGO(const bool b)
	{
		rule1NewGO = b;
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
		Animation_Max
	};

	AnimationClip m_animationPlayer[Animation_Max];



private: //data menber
	////////////////////////////////////////////////////////////
	// �N���X�̃I�u�W�F�N�g
	////////////////////////////////////////////////////////////

	Stage* m_stage = nullptr;
	MainProcessing* m_game = nullptr;
	EnemyAI* m_enemyAI = nullptr;

	ModelRender* m_modelRender[con::PlayerNumberMax] = { nullptr };	//�v���C���[�L�����N�^�[�̃��f��
	SpriteRender* m_spriteGameEnd = nullptr;
	SpriteRender* m_spriteGoalRank[con::PlayerNumberMax][4] = { nullptr };
	//FontRender* m_fontGoalRank[con::PlayerNumberMax] = { nullptr };	//�S�[�����ʂ�\������t�H���g
	SoundSE* m_seJump = nullptr;
	SoundSE* m_seFall = nullptr;
	SoundSE* m_seSrip = nullptr;

	Rule1* m_rule1 = nullptr;

	////////////////////////////////////////////////////////////
	// �v���C���[���
	////////////////////////////////////////////////////////////
	int m_difficultyLevel = 0;//��Փx
	int m_moveStop[con::PlayerNumberMax] = { 0, 0, 0, 0 };//m_moveStop���m_moveStopCount���傫��������m_moveStopBool��true
	int m_moveStopCount[con::PlayerNumberMax]= { 0, 0, 0, 0 };//m_moveStop���m_moveStopCount���傫��������m_moveStopBool��true
	bool m_moveStopBool[con::PlayerNumberMax] = { false, false, false, false };//true�ňړ�����
	bool m_bluemiss[con::PlayerNumberMax]= { false, false, false, false };//�O�̈ړ��Őu���b�N�ɗ�������


	int m_activePlayer[con::PlayerNumberMax] = { true, true, true, true };	//���̃v���C���[�͑��삵�Ă��邩
	int m_maxPlayer = con::PlayerNumberMax;									//�v���C���[�̍ő吔

	int m_goalRanking[con::PlayerNumberMax] = { 0, 0, 0, 0 };				//�v���C���[�̃S�[������
	bool m_flagGoal[con::PlayerNumberMax] = { false, false, false, false };	//�S�[��������
	int m_goalPlayer = 0;					
	
	int m_endTimer = 0;//�S�[�����Ă���̎���tuika
	int fontDeavtive = 0;

	bool m_gameEnd = false;//�S�[�������v���C���[�̐l��tuika

	bool m_EJumpFlag[con::PlayerNumberMax] = { false };//cpu�W�����v�t���O
	
	////////////////////////////////////////////////////////////
	// �^�C�}�[�֘A
	////////////////////////////////////////////////////////////

	bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//�W�����v�A�j���[�V��������
	int m_timerAnimation[con::PlayerNumberMax] = { 0, 0, 0, 0 };						//�A�j���[�V�����̃^�C�}�[


	int m_goalPlayerZero = 0;



	////////////////////////////////////////////////////////////
	// �T�h���f�X���[�h�̃����o�ϐ�
	////////////////////////////////////////////////////////////

	int m_roundPoint[con::PlayerNumberMax] = { 0,0,0,0 };		//�v���C���[�̃��E���h�̃|�C���g
	bool m_finishSuddenDeath = false;							//�T�h���f�X���[�h���I��������

	bool rule1NewGO = false;
};