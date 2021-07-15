#pragma once
#include "model_render.h"
#include "constant.h"
class Player;
class MainProcessing;
class Stage;
class EnemyAI : public IGameObject
{
public:
	EnemyAI();
	~EnemyAI();
	bool Start() override final;
	void Init() override final;
	void Finish() override final;
	void Update() override final;

	void Move(const int pNum);
	void Moverule1(const int pNum);

	void DifficultyMove(const int pNum);
	void DifficultyMoverule1(const int pNum);
	void AutoController1(const int pNum);
	void AutoControllerRule1(const int pNum); 
	void AutoController3(const int pNum);

public: //Get�֐�

public://Set�֐�
	//��Փx�󂯎��
	void SetDifficultyLevel(const int& i) {
		m_difficultyLevel = i;
	}
	void SetmissInvalidCount(const int& pNum, const int& i) {
		m_missInvalidCount[pNum] = i;
	}
private:
	Player* m_player = nullptr;
	MainProcessing* m_game = nullptr;
	Stage* m_stage = nullptr;

	//�萔
	static const int m_MOVE_2;		//�ړ���2
	static const int m_MOVE_1;		//�ړ���1

	static const int m_MISS_INVALID;//�~�X�𖳌��ɂ����
	static const int m_RANDOM_ZERO_TO_NINE;//���������Ŋ��邩
	static const int m_RAND_TIMES;//rand�̎��s��

	static const int m_MISSPLAY_EASY;//��Փx��킢�̃~�X�v���C�ɂȂ�\���̂���s�������銄��
	static const int m_MISSPLAY_NORMAL;//��Փx�ӂ��̃~�X�v���C�ɂȂ�\���̂���s�������銄��
	static const int m_MISSPLAY_DIFFICULT;//��Փx�悢�̃~�X�v���C�ɂȂ�\���̂���s�������銄��


	//�ϐ�
	int m_difficultyLevel = 0;//��Փx0��킢1�ӂ�2�悢
	int m_randomNumber = 0;//��������0�`9�A���l�ɂ����CPU�̍s�������܂�

	//cpu�֌W
	int m_missInvalidCount[con::PlayerNumberMax] = { 0,0,0,0 };//stop�̃J�E���g����stop��true
	bool m_activePlayer[con::PlayerNumberMax] = { false,false,false,false };	//CPU���蓖�Ă邩
	bool m_activeOperation[con::PlayerNumberMax] = { true, true, true, true };   //CPU������\��
	bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//�W�����v�A�j���[�V��������
	bool m_JumpFlag[con::PlayerNumberMax] = { false };//�W�����v�t���O	
	bool m_flagGoal[con::PlayerNumberMax] = { false, false, false, false };	//�S�[��������
	bool m_bluemiss[con::PlayerNumberMax] = { false };//�u���b�N�Ń~�X������	
};