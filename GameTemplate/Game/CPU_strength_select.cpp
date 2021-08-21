#include "stdafx.h"
#include "CPU_strength_select.h"

#include "main_processing.h"
#include "constant.h"



namespace //constant
{
    ////////////////////////////////////////////////////////////
    // ������
    ////////////////////////////////////////////////////////////

    constexpr const wchar_t* FONT = L"CPU�̋�����I�����Ă�������";
    const Vector2 FONT_POSITION = { -400.0f,300.0f };
    const float FONT_SCALE = 1.3f;

    ////////////////////////////////////////////////////////////
    // �ʒu���
    ////////////////////////////////////////////////////////////

    const Vector2 CHOICES_POSITION[3] = {
        {-300.0f,0.0f},
        {0.0f,0.0f},
        {300.0f,0.0f}
    };

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    const int LEFT_END = 0;     //���[
    const int RIGHT_END = 2;    //�E�[
}



CPUStrengthSelect::CPUStrengthSelect()
{
    //////////////////////////////
    // NewGO
    //////////////////////////////

    //////////
    // �I������UI��NewGO
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteChoices[spriteNum]->Init(filePath::dds::CPU_STRENGTH[spriteNum]);
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // �t�H���g��NewGO
    //////////

    m_font = NewGO<FontRender>(igo::PRIORITY_CLASS);
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

CPUStrengthSelect::~CPUStrengthSelect()
{
    //////////
    // �I������UI��DeleteGO
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        DeleteGO(m_spriteChoices[spriteNum]);
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

bool CPUStrengthSelect::Start()
{
    //////////////////////////////
    // FindGO
    //////////////////////////////

    m_game = FindGO<MainProcessing>(igo::CLASS_NAME_MAIN_PROCESSING);

    return true;
}

void CPUStrengthSelect::Init()
{
    m_flagProcess = true;

    //////////
    // �I������UI��������
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
    // �t�H���g��������
    //////////

    m_font->Activate();

    //////////
    // �����o�ϐ���������
    //////////

    m_cursorPosition = 0;       //�J�[�\���̏ꏊ
    m_numberOfPlayer = 0;       //�v���C���[�̐l��
    m_flagDecision = false;     //�l�������肵�����̃t���O
    m_flagFinish = false;       //���̃N���X�ł���ׂ��������I�������
    m_flagDrawMove = true;      //UI���㉺�Ɉړ����鏈���Ɏg�p����t���O
}

void CPUStrengthSelect::Finish()
{
    m_flagProcess = false;

    //////////
    // �I������UI���\��
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // �t�H���g���\��
    //////////

    m_font->Deactivate();
}

void CPUStrengthSelect::Update()
{
    if (false == m_flagProcess) {
        return;
    }

    SelectTheNumberOfCPUStrength();

    if (m_flagDecision == true && m_flagFinish == false) {
        m_game->SetCPULevel(static_cast<con::CPULevel>(m_cursorPosition));
        FinishCPUStrengthSelect();
    }

    DrawMove();
}

void CPUStrengthSelect::SelectTheNumberOfCPUStrength()
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

        --m_cursorPosition; //�J�[�\�����ړ�

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

        ++m_cursorPosition; //�J�[�\�����ړ�

        //�V�����I�����Ă������̂�I�����Ă����Ԃɂ���
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_flagDrawMove = true;
    }
}

void CPUStrengthSelect::FinishCPUStrengthSelect()
{
    m_flagFinish = true;
}

void CPUStrengthSelect::DrawMove()
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