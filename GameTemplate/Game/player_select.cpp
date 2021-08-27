#include "stdafx.h"
#include "player_select.h"

#include "constant.h"
#include "player.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // ������
    ////////////////////////////////////////////////////////////

    constexpr const wchar_t* FONT = L"�v���C����l����I�����Ă�������";
    const Vector2 FONT_POSITION = { -450.0f,300.0f };
    const float FONT_SCALE = 1.3f;


    ////////////////////////////////////////////////////////////
    // �ʒu���
    ////////////////////////////////////////////////////////////

    const Vector2 CHOICES_POSITION[4] = {
        {-300.0f,0.0f},
        {-100.0f,0.0f},
        {100.0f,0.0f},
        {300.0f,0.0f}
    };

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    const int ADD_ONE = 1;      //�v���C�l���̒l��n���Ƃ��̐����킹

    const int LEFT_END = 0;     //���[
    const int RIGHT_END = 3;    //�E�[
}



PlayerSelect::PlayerSelect()
{
    //////////////////////////////
    // NewGO
    //////////////////////////////

    //////////
    // UI��NewGO
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteChoices[spriteNum]->Init(filePath::dds::NUMBER_OF_PLAYERS[spriteNum]);
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // �t�H���g��NewGO
    //////////

    m_font = NewGO<FontRender>(igo::PRIORITY_FONT);
    m_font->Init(FONT, FONT_POSITION, FONT_SCALE);
    m_font->Deactivate();

    //////////
    // SE��NewGO
    //////////

    m_seDecision = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seDecision->Init(filePath::se::DECISION);
    m_seMoveCursor = NewGO<SoundSE>(igo::PRIORITY_CLASS);
    m_seMoveCursor->Init(filePath::se::MOVE_CURSOR);
}

PlayerSelect::~PlayerSelect()
{
    //////////
    // UI��DeleteGO
    //////////

    for (int choicesNum = con::FIRST_ELEMENT_ARRAY; choicesNum < m_NUMBER_OF_CHOICES; choicesNum++) {
        DeleteGO(m_spriteChoices[choicesNum]);
    }

    //////////
    // �t�H���g��DeleteGO
    //////////

    DeleteGO(m_font);

    //////////
    // SE��DeleteGO
    //////////

    DeleteGO(m_seDecision);
    DeleteGO(m_seMoveCursor);
}

bool PlayerSelect::Start()
{
    //////////////////////////////
    // FindGO
    //////////////////////////////

    m_findPlayer = FindGO<Player>(igo::CLASS_NAME_PLAYER);

    return true;
}

void PlayerSelect::Init()
{
    m_flagProcess = true;

    //////////
    // UI�̏�����
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum]->Activate();
        m_spriteChoices[spriteNum]->SetPosition(CHOICES_POSITION[spriteNum]);

        if (con::FIRST_ELEMENT_ARRAY == spriteNum) {
            m_spriteChoices[spriteNum]->SetMulColor(SRns::COLOR_NORMAL);
        }
        else {
            m_spriteChoices[spriteNum]->SetMulColor(SRns::COLOR_GRAY);
        }
    }

    //////////
    // �t�H���g�̏�����
    //////////

    m_font->Activate();

    //////////
    // �����o�ϐ��̏�����
    //////////

    m_cursorPosition = 0;       //�J�[�\���̏ꏊ
    m_numberOfPlayer = 0;       //�v���C���[�̐l��
    m_flagDecision = false;     //�l�������肵�����̃t���O
    m_flagFinish = false;       //���̃N���X�ł���ׂ��������I�������
    m_flagDrawMove = true;      //UI���㉺�Ɉړ����鏈���Ɏg�p����t���O
}

void PlayerSelect::Finish()
{
    m_flagProcess = false;

    //////////
    // UI�̔�\��
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // �t�H���g�̏�����
    //////////

    m_font->Deactivate();
}

void PlayerSelect::Update()
{
    if (false == m_flagProcess) {
        return;
    }

    SelectTheNumberOfPlayers();

    if (m_flagDecision == true && m_flagFinish == false) {
        FinishPlayerSelect();
    }

    DrawMove();
}

void PlayerSelect::SelectTheNumberOfPlayers()
{
    //����
    if (g_pad[con::player_1]->IsTrigger(enButtonA)) {
        m_seDecision->Play(false);

        m_flagDecision = true;
    }
    //���Ɉړ�
    else if (g_pad[con::player_1]->IsTrigger(enButtonLeft) == true) {
        m_seMoveCursor->Play(false);

        if (m_cursorPosition == LEFT_END) { //���[�Ȃ�J�[�\���ړ����Ȃ�
            return;
        }

        //�O�ɑI�����Ă������̂�I�����Ă��Ȃ���Ԃɂ���
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);
        m_spriteChoices[m_cursorPosition]->SetPositionY(0.0f);

        --m_cursorPosition; //�J�[�\���̈ړ�

        //�V�����I�����Ă������̂�I�����Ă����Ԃɂ���
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_flagDrawMove = true;
    }
    //�E�Ɉړ�
    else if (g_pad[con::player_1]->IsTrigger(enButtonRight) == true) {
        m_seMoveCursor->Play(false);

        if (m_cursorPosition == RIGHT_END) { //�E�[�Ȃ�J�[�\���ړ����Ȃ�
            return;
        }

        //�O�ɑI�����Ă������̂�I�����Ă��Ȃ���Ԃɂ���
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_GRAY);
        m_spriteChoices[m_cursorPosition]->SetPositionY(0.0f);

        ++m_cursorPosition; //�J�[�\���̈ړ�

        //�V�����I�����Ă������̂�I�����Ă����Ԃɂ���
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_flagDrawMove = true;
    }
}

void PlayerSelect::FinishPlayerSelect()
{
    m_findPlayer->SetMaxPlayer(m_cursorPosition + ADD_ONE);

    m_flagFinish = true;
}

void PlayerSelect::DrawMove()
{
    if (m_flagDrawMove == true) { //�I�����Ă���UI����Ɉړ����鏈��
        m_spriteChoices[m_cursorPosition]->SetPositionY(m_spriteChoices[m_cursorPosition]->GetPosition().y + 0.5f);
        if (m_spriteChoices[m_cursorPosition]->GetPosition().y >= 10.0f) {
            m_spriteChoices[m_cursorPosition]->SetPositionY(10.0f);
            m_flagDrawMove = false;
        }
    }
    else { //�I�����Ă���UI�����Ɉړ����鏈��
        m_spriteChoices[m_cursorPosition]->SetPositionY(m_spriteChoices[m_cursorPosition]->GetPosition().y - 0.5f);
        if (m_spriteChoices[m_cursorPosition]->GetPosition().y <= -10.0f) {
            m_spriteChoices[m_cursorPosition]->SetPositionY(-10.0f);
            m_flagDrawMove = true;
        }
    }
}