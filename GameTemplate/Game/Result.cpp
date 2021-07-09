#include "stdafx.h"
#include "Result.h"
#include "main_processing.h"



namespace
{
	const Vector3 PLAYER_POSITION[con::PlayerNumberMax] = {	//�v���C���[�̏������W
		{ 1200.0f, -1300.0f, -240.0f },											//�v���C���[�P
		{ 400.0f, -1300.0f, -240.0f },											//�v���C���[�Q
		{ -400.0f, -1300.0f, -240.0f },											//�v���C���[�R
		{ -1200.0f, -1300.0f, -240.0f }											//�v���C���[�S
	};
	const Vector2 PLAYER_RANK_SPRITE[con::PlayerNumberMax] = {	//�v���C���[�̏������W
		{ -420.0f, -200.0f},											//�v���C���[�P
		{ -140.0f, -200.0f },											//�v���C���[�Q
		{ 140.0f, -200.0f },											//�v���C���[�R
		{ 420.0f,-200.0f }											//�v���C���[�S
	};

	const Vector2 MODE_SELECT_SPRITE[con::PlayerNumberMax] = {	//�v���C���[�̏������W
		{ 0.0f, 200.0f },											//�v���C���[�P
		{ 0.0f, 100.0f },											//�v���C���[�Q
		{ 0.0f, 0.0f },											//�v���C���[�R
		{ 0.0f, -100.0f }											//�v���C���[�S
	};

	const int UP_END = 0;     //��ԏ�
	const int UP = 1;    //�^�񒆏�
	const int DOWN = 2;    //�^�񒆉�
	const int DOWN_END = 3;    //��ԉ�
}



Result::Result() {
	/*for (int playerNum = 0; playerNum < con::PlayerNumberMax; playerNum++) {
		m_rank[playerNum]=m_game->GetRank(playerNum);
	}*/
}

Result::~Result() {
	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		DeleteIndividual(playerNum);
	}

	DeleteGO(m_spriteBackground);
	/*DeleteGO(m_spriteGoalRank[0]);
	DeleteGO(m_spriteGoalRank[1]);
	DeleteGO(m_spriteGoalRank[2]);
	DeleteGO(m_spriteGoalRank[3]);*/


}

void Result::DeleteIndividual(const int pNum)
{
	//p_num�̓v���C���[�̃R���g���[���[�ԍ�
	DeleteGO(m_modelRender[pNum]);
	DeleteGO(m_spriteGoalRank[pNum]);
	DeleteGO(m_spriteChoices[pNum]);

	//DeleteGO(m_skinModelRender[pNum]);
}
bool Result::Start() {
	//�w�i
	m_spriteBackground = NewGO<SpriteRender>(igo::PRIORITY_BACKGROUND);
	m_spriteBackground->Init(filePath::dds::BACKGROUND_SKY);
	m_spriteBackground->Deactivate();

	//�A�j���[�V�����̐ݒ�
	m_animationPlayer[win].Load(filePath::tka::WIN);
	m_animationPlayer[stand].Load(filePath::tka::STAND);
	m_animationPlayer[lose].Load(filePath::tka::LOSE);
	//���[�v�Đ�
	m_animationPlayer[stand].SetLoopFlag(false);
	m_animationPlayer[win].SetLoopFlag(false);
	m_animationPlayer[lose].SetLoopFlag(false);

	//�v���C���[���Ƃɏ���
	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		bool check = StartIndividual(playerNum);

		//StartIndividual�֐���false��Ԃ�����false��Ԃ��ď������I��������B
		if (check == false) {
			return false;
		}
	}

	m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seDecision->Init(filePath::se::DECISION);
	m_seMoveCursor = NewGO<SoundSE>(igo::PRIORITY_CLASS);
	m_seMoveCursor->Init(filePath::se::MOVE_CURSOR);

	//�I����
	m_spriteChoices[0] = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteChoices[0]->Init(filePath::dds::COMMAND_PLAY_ONE_MORE_TIME);
	m_spriteChoices[0]->SetPosition(MODE_SELECT_SPRITE[0]);
	m_spriteChoices[0]->Deactivate();

	m_spriteChoices[1] = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteChoices[1]->Init(filePath::dds::COMMAND_CHOOSE_THE_NUMBER_OF_PLAYERS);
	m_spriteChoices[1]->SetPosition(MODE_SELECT_SPRITE[1]);
	m_spriteChoices[1]->Deactivate();

	m_spriteChoices[2] = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteChoices[2]->Init(filePath::dds::COMMAND_CHOOSE_A_RULE);
	m_spriteChoices[2]->SetPosition(MODE_SELECT_SPRITE[2]);
	m_spriteChoices[2]->Deactivate();

	m_spriteChoices[3] = NewGO<SpriteRender>(igo::PRIORITY_UI);
	m_spriteChoices[3]->Init(filePath::dds::COMMAND_EXIT_GAME);
	m_spriteChoices[3]->SetPosition(MODE_SELECT_SPRITE[3]);
	m_spriteChoices[3]->Deactivate();

	m_game = FindGO<MainProcessing>(igo::CLASS_NAME_GAME);
	return true;
}

void Result::Init()
{
	m_flagProcessing = true;

	m_spriteBackground->Activate();

	m_spriteChoices[0]->SetMulColor(srName::COLOR_NORMAL);
	m_spriteChoices[0]->Deactivate();
	m_spriteChoices[1]->SetMulColor(srName::COLOR_GRAY);
	m_spriteChoices[1]->Deactivate();
	m_spriteChoices[2]->SetMulColor(srName::COLOR_GRAY);
	m_spriteChoices[2]->Deactivate();
	m_spriteChoices[3]->SetMulColor(srName::COLOR_GRAY);
	m_spriteChoices[3]->Deactivate();

	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		InitIndividual(playerNum);
	}



	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_rank[playerNum] = 0;
	}

	m_spriteChoicesNewGO = false;
	m_spriteChoicesNewGORE = false;

	m_flagDecision = false;    //���肵�����̃t���O
	m_cursorPosition = 0;//�J�[�\���̏ꏊ
	m_flagFinish = false;      //���̃N���X�ł���ׂ��������I�������
}

void Result::InitIndividual(const int& pNum)
{
	m_modelRender[pNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
	m_modelRender[pNum]->Init(filePath::tkm::CHAEACTER_MODEL, modelUpAxis::enModelUpAxisZ, m_animationPlayer, Animation_Max);
	m_modelRender[pNum]->SetPosition(PLAYER_POSITION[pNum]);
	m_modelRender[pNum]->SetScale({ 0.2f,0.2f,0.2f });
	m_modelRender[pNum]->Activate();

	switch (m_rank[pNum]) {
	case 1:
		m_spriteGoalRank[pNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteGoalRank[pNum]->Init(filePath::dds::RANKONE);
		m_spriteGoalRank[pNum]->SetScale({ 0.2f,0.2f,0.2f });
		m_spriteGoalRank[pNum]->SetPosition(PLAYER_RANK_SPRITE[pNum]);
		m_modelRender[pNum]->PlayAnimation(win);
		break;
	case 2:
		m_spriteGoalRank[pNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteGoalRank[pNum]->Init(filePath::dds::RANKTWO);
		m_spriteGoalRank[pNum]->SetScale({ 0.2f,0.2f,0.2f });
		m_spriteGoalRank[pNum]->SetPosition(PLAYER_RANK_SPRITE[pNum]);
		m_modelRender[pNum]->PlayAnimation(stand);
		break;
	case 3:
		m_spriteGoalRank[pNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteGoalRank[pNum]->Init(filePath::dds::RANKTHREE);
		m_spriteGoalRank[pNum]->SetScale({ 0.2f,0.2f,0.2f });
		m_spriteGoalRank[pNum]->SetPosition(PLAYER_RANK_SPRITE[pNum]);
		m_modelRender[pNum]->PlayAnimation(stand);
		break;
	case 4:
		m_spriteGoalRank[pNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteGoalRank[pNum]->Init(filePath::dds::RANKFOUR);
		m_spriteGoalRank[pNum]->SetScale({ 0.2f,0.2f,0.2f });
		m_spriteGoalRank[pNum]->SetPosition(PLAYER_RANK_SPRITE[pNum]);
		m_modelRender[pNum]->PlayAnimation(lose);
		break;
	}
}

void Result::Finish()
{
	m_flagProcessing = false;

	m_spriteChoices[0]->Deactivate();
	m_spriteChoices[1]->Deactivate();
	m_spriteChoices[2]->Deactivate();
	m_spriteChoices[3]->Deactivate();

	m_spriteBackground->Deactivate();

	for (int playerNum = con::FIRST_OF_THE_ARRAY; playerNum < con::PlayerNumberMax; playerNum++) {
		m_modelRender[playerNum]->Deactivate();
		DeleteGO(m_spriteGoalRank[playerNum]);
	}
}

void Result::Update()
{
	if (m_flagProcessing == false) {
		return;
	}

	m_spriteChoicesNewGORE = m_spriteChoicesNewGO;
	SelectDisplay();
	if (m_flagDecision == true && m_flagFinish == false) {
		FinishResult();
	}
}

bool Result::StartIndividual(const int pNum) {
	m_modelRender[pNum] = NewGO<ModelRender>(igo::PRIORITY_MODEL);
	m_modelRender[pNum]->Init(filePath::tkm::CHAEACTER_MODEL, modelUpAxis::enModelUpAxisZ, m_animationPlayer, Animation_Max);
	m_modelRender[pNum]->Deactivate();





	

	return true;
}

void Result::Display() {

}


void Result::AnimationDisplay() {

}

void Result::SelectDisplay() {
	if (g_pad[0]->IsTrigger(enButtonA) == true && m_spriteChoicesNewGO == false) {
		m_spriteChoicesNewGO = true;



		m_spriteChoices[0] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteChoices[0]->Init(filePath::dds::COMMAND_PLAY_ONE_MORE_TIME);
		m_spriteChoices[0]->SetPosition(MODE_SELECT_SPRITE[0]);

		m_spriteChoices[1] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteChoices[1]->Init(filePath::dds::COMMAND_CHOOSE_THE_NUMBER_OF_PLAYERS);
		m_spriteChoices[1]->SetPosition(MODE_SELECT_SPRITE[1]);
		m_spriteChoices[1]->SetMulColor(srName::COLOR_GRAY);

		m_spriteChoices[2] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteChoices[2]->Init(filePath::dds::COMMAND_CHOOSE_A_RULE);
		m_spriteChoices[2]->SetPosition(MODE_SELECT_SPRITE[2]);
		m_spriteChoices[2]->SetMulColor(srName::COLOR_GRAY);

		m_spriteChoices[3] = NewGO<SpriteRender>(igo::PRIORITY_UI);
		m_spriteChoices[3]->Init(filePath::dds::COMMAND_EXIT_GAME);
		m_spriteChoices[3]->SetPosition(MODE_SELECT_SPRITE[3]);
		m_spriteChoices[3]->SetMulColor(srName::COLOR_GRAY);
	
		
	}
	ResultSelect();

}

void Result::ResultSelect() {
	//����
	if (g_pad[con::player_1]->IsTrigger(enButtonA) && m_spriteChoicesNewGORE == true) {
		m_seDecision->Play(false);
		m_game->SetResultSelect(m_cursorPosition);

		m_flagDecision = true;
	}
	//��Ɉړ�
	else if (g_pad[con::player_1]->IsTrigger(enButtonUp) == true && m_spriteChoicesNewGORE == true) {
		m_seMoveCursor->Play(false);

		if (m_cursorPosition == UP_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_GRAY);

		--m_cursorPosition;

		m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_NORMAL);
	}
	//���Ɉړ�
	else if (g_pad[con::player_1]->IsTrigger(enButtonDown) == true && m_spriteChoicesNewGORE == true) {
		m_seMoveCursor->Play(false);

		if (m_cursorPosition == DOWN_END) {
			return;
		}

		m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_GRAY);

		++m_cursorPosition;

		m_spriteChoices[m_cursorPosition]->SetMulColor(srName::COLOR_NORMAL);
	}

}

void Result::FinishResult()
{
	m_flagFinish = true;
}