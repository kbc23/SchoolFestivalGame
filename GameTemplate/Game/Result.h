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
	void Init();
	void Finish();
	void Update() override final;
	
	/**
	 * @brief ���ʂ̕\��
	*/
	void ResultDisplay();

	/**
	 * @brief ���ʏ��ʂ̕\��
	*/
	void SelectDisplay();

	/**
	 * @brief ���U���g��ʂł̑I�����̑I������
	*/
	void ResultSelect();

	/**
	 * @brief ���U���g�V�[���ł̏������I������ۂɂ��鏈��
	*/
	void FinishResult();


public://Get�֐�
	/**
	 * @brief �������I����������擾
	 * @return �������I�������
	*/
	const bool GetFlagFinish()
	{
		return m_flagFinish;
	}

	/**
	 * @brief �I�����Ă����I�����̔ԍ����擾
	 * @return �I�����Ă����I�����̔ԍ�
	*/
	const bool GetSelect()
	{
		return m_cursorPosition;
	}

	const int GetCursorPosition()
	{
		return m_cursorPosition;
	}




public://Set�֐�
	/**
	 * @brief �v���C���[�̏��ʂ��Z�b�g
	 * @param playNum �v���C���[�ԍ�
	 * @param rank ����
	*/
	void SetRank(const int playNum, const int rank)
	{
		m_rank[playNum] = rank;
	}


private: //enum
	enum class DisplayStatus
	{
		result,
		commandSelect,
		finish,
		DisplayStatusMax
	};

	DisplayStatus m_displayStatus = DisplayStatus::result;


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
	ModelRender* m_modelCharacter[con::PlayerNumberMax] = { nullptr };	//�v���C���[�L�����N�^�[�̃��f��
	SpriteRender* m_spriteGoalRank[con::PlayerNumberMax] = { nullptr };	//�S�[�����ʂ�\������
	SpriteRender* m_spriteChoices[m_NUMBER_OF_CHOICES] = { nullptr };	//�I����UI�\��
	SpriteRender* m_spriteBackground =  nullptr ;
	SpriteRender* m_spritePressANext = nullptr;
	SoundSE* m_seDecision = nullptr;
	SoundSE* m_seMoveCursor = nullptr;

	int m_rank[con::PlayerNumberMax] = { con::GoalRankMax,con::GoalRankMax,con::GoalRankMax,con::GoalRankMax };

	bool m_flagDecision = false;    //���肵�����̃t���O
	int m_cursorPosition = 0; //�J�[�\���̏ꏊ
	bool m_flagFinish = false;      //���̃N���X�ł���ׂ��������I�������
	bool m_flagBlinking = true; //m_pressAButton�̓��ߏ����Ɏg�p
};

