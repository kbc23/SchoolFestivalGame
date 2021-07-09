#pragma once
#include "model_render.h"
#include "constant.h"
class Player;
class Game;
class Stage;
class EnemyAI : public IGameObject
{
public:
	EnemyAI();
	~EnemyAI();
	bool Start();
	void Update();
	void Move(const int pNum);
	void Moverule1(const int pNum);

	void DifficultyMove(const int eNum);
	void DifficultyMoverule1(const int eNum);
	void AutoController1(const int pNum);
	void AutoController11(const int pNum);
	//void AutoController2(const int pNum); 
	void AutoController3(const int pNum);



public: //Get�֐�
	/*bool GetJampFlag(int pNum) {
		return m_JumpFlag[pNum];
	}*/
public://Set�֐�
	//��Փx�󂯎��
	void SetDifficultyLevel(int i) {
		m_difficultyLevel = i;
	}
	void SetmissInvalidCount(int pNum,int i) {
		m_missInvalidCount[pNum] = i;
	}
	//�u���b�N�Ń~�X�������󂯎��
	/*void SetBlueMiss(int pNum, bool i) {
		m_bluemiss[pNum] = i;
	}*/
private:
	Player* m_player = nullptr;
	Game* m_game = nullptr;
	Stage* m_stage = nullptr;
	int m_difficultyLevel = 0;//��Փx1�ȒP2����3���
	int m_missInvalidCount[con::PlayerNumberMax] = { 0,0,0,0 };//stop�̃J�E���g����stop��true

	bool m_activePlayer[con::PlayerNumberMax] = { false,false,false,false };	//CPU���蓖�Ă邩
	int m_maxPlayer = 0;//�v���C���[�ő吔
	int m_addEnemyNumber = 2;//CPU�p�ԍ�=2
	bool m_activeOperation[con::PlayerNumberMax] = { true, true, true, true };   //CPU������\��
	bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//�W�����v�A�j���[�V��������
	bool m_flagGoal[con::PlayerNumberMax] = { false, false, false, false };	//�S�[��������
	bool m_bluemiss[con::PlayerNumberMax] = { false };//�u���b�N�Ń~�X������
	//bool m_bluemissRe = false;//�u���b�N�Ń~�X������

	bool m_JumpFlag[con::PlayerNumberMax] = { false };//�W�����v�t���O
	
};