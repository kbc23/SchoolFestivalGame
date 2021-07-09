#pragma once
#include "model_render.h"
#include "sprite_render.h"
#include "font_render.h"
#include "sound_SE.h"
#include "constant.h"
class MainProcessing;
class Result : public IGameObject
{
public:
	Result();
	~Result();
	bool Start() override final;
	bool StartIndividual(const int pNum);
	void DeleteIndividual(const int pNum);
	void Init() override final;
	void InitIndividual(const int& pNum);
	void Finish() override final;
	void Update() override final;


	void Display();
	void AnimationDisplay();
	void SelectDisplay();

	void ResultSelect();

	void FinishResult();


public://Get�֐�
	bool GetFlagFinish() {
		return m_flagFinish;
	}
	bool GetSelect() {
		return m_cursorPosition;
	}
public://Set�֐�
	void SetRank(int pNum, int rank) {
		m_rank[pNum] = rank;
	}
private:
	static const int m_NUMBER_OF_CHOICES = 4;     //�I�����̐�
	enum AnimationEnum
	{
		
		win,
		stand,
		lose,
		Animation_Max
	};

	AnimationClip m_animationPlayer[Animation_Max];
	MainProcessing* m_game = nullptr;
	ModelRender* m_modelRender[con::PlayerNumberMax] = { nullptr };	//�v���C���[�L�����N�^�[�̃��f��
	SpriteRender* m_spriteGoalRank[con::PlayerNumberMax] = { nullptr };	//�S�[�����ʂ�\������
	SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };	//�I����UI�\��
	SpriteRender* m_spriteBackground =  nullptr ;	//�I����UI�\��
	SoundSE* m_seDecision = nullptr;
	SoundSE* m_seMoveCursor = nullptr;

	int m_rank[con::PlayerNumberMax] = { 0,0,0,0 };
	bool m_spriteChoicesNewGO = false;
	bool m_spriteChoicesNewGORE = false;

	bool m_flagDecision = false;    //���肵�����̃t���O
	int m_cursorPosition = 0;//�J�[�\���̏ꏊ
	bool m_flagFinish = false;      //���̃N���X�ł���ׂ��������I�������

};

