#include "stdafx.h"
#include "mode_select.h"

#include "sudden_death_mode.h"
#include "constant.h"


namespace //constant
{
    ////////////////////////////////////////////////////////////
    // ������
    ////////////////////////////////////////////////////////////

    const Vector2 CHOICES_POSITION[2] = //UI�摜�̈ʒu
    {
        {-150.0f,0.0f},
        {150.0f,0.0f}
    };

    const Vector2 FONT_CHOICES_POSITION[2] = //�t�H���g�̈ʒu
    {
        { -500.0f,-200.0f },
        { -600.0f,-200.0f }
    };

    constexpr const wchar_t* FONT_CHOICES[2] =
    {
        L"�N���ŏ��ɃS�[���ɂ��ǂ蒅���邩���������[�X!",
        L"���~�X�������炻���ŏI��!�n���n���h�L�h�L�̃T�h���f�X!"
    };

    const Vector2 FONT_POSITION = { -500.0f, 300.0f };

    const float FONT_SCALE = 1.3f;

    constexpr const wchar_t* FONT = L"�v���C���郂�[�h��I�����Ă�������";

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    const int LEFT_END = 0;     //���[
    const int RIGHT_END = 1;    //�E�[
}



ModeSelect::ModeSelect()
{
    //////////////////////////////
    // NewGO
    //////////////////////////////

    //////////
    // UI��NewGO
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum] = NewGO<SpriteRender>(igo::PRIORITY_UI);
        m_spriteChoices[spriteNum]->Init(filePath::dds::MODE_UI[spriteNum]);
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // �t�H���g��NewGO
    //////////

    //�I�������Ƃ̃t�H���g
    for (int fontNum = con::FIRST_ELEMENT_ARRAY; fontNum < m_NUMBER_OF_CHOICES; fontNum++) {
        m_fontChoices[fontNum] = NewGO<FontRender>(igo::PRIORITY_FONT);
        m_fontChoices[fontNum]->Init(FONT_CHOICES[fontNum], FONT_CHOICES_POSITION[fontNum]);
        m_fontChoices[fontNum]->Deactivate();
    }

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

ModeSelect::~ModeSelect()
{
    //////////
    // UI��DeleteGO
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        DeleteGO(m_spriteChoices[spriteNum]);
    }

    //////////
    // �t�H���g��DeleteGO
    //////////

    //�I�������Ƃ̃t�H���g
    for (int fontNum = con::FIRST_ELEMENT_ARRAY; fontNum < m_NUMBER_OF_CHOICES; fontNum++) {
        DeleteGO(m_fontChoices[fontNum]);
    }

    DeleteGO(m_font);

    //////////
    // SE��DeleteGO
    //////////

    DeleteGO(m_seDecision);
    DeleteGO(m_seMoveCursor);
}

bool ModeSelect::Start()
{
    //////////////////////////////
    // FindGO
    //////////////////////////////

    m_findSuddenDeathMode = FindGO<SuddenDeathMode>(igo::CLASS_NAME_SUDDEN_DEATH);

    return true;
}

void ModeSelect::Init()
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

    //�I�������Ƃ̃t�H���g
    for (int fontNum = con::FIRST_ELEMENT_ARRAY; fontNum < m_NUMBER_OF_CHOICES; fontNum++) {
        if (con::FIRST_ELEMENT_ARRAY == fontNum) { //�P�ԍŏ��̂����A�N�e�B�u�ɂ���
            m_fontChoices[fontNum]->Activate();
        }
        else {
            m_fontChoices[fontNum]->Deactivate();
        }
    }

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

void ModeSelect::Finish()
{
    m_flagProcess = false;

    //////////
    // UI�̔�\��
    //////////

    for (int spriteNum = con::FIRST_ELEMENT_ARRAY; spriteNum < m_NUMBER_OF_CHOICES; spriteNum++) {
        m_spriteChoices[spriteNum]->Deactivate();
    }

    //////////
    // �t�H���g�̔�\��
    //////////

    //�I�������Ƃ̃t�H���g
    for (int fontNum = con::FIRST_ELEMENT_ARRAY; fontNum < m_NUMBER_OF_CHOICES; fontNum++) {
        m_fontChoices[fontNum]->Deactivate();
    }

    m_font->Deactivate();
}

void ModeSelect::Update()
{
    if (false == m_flagProcess) {
        return;
    }

    SelectTheNumberOfCPUStrength();

    if (m_flagDecision == true && m_flagFinish == false) {
        FinishCPUStrengthSelect();
    }

    DrawMove();
}

void ModeSelect::SelectTheNumberOfCPUStrength()
{
    //����
    if (g_pad[con::player_1]->IsTrigger(enButtonA) == true) {
        m_seDecision->Play(false);

        //���[�X���[�h
        if (m_cursorPosition == raceMode) {
            m_findSuddenDeathMode->SetFlagSuddenDeathMode(false);
        }
        //�T�h���f�X���[�h
        else if (m_cursorPosition == suddenDeathMode) {
            m_findSuddenDeathMode->SetFlagSuddenDeathMode(true);
        }

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
        m_fontChoices[m_cursorPosition]->Deactivate();

        --m_cursorPosition; //�J�[�\�����ړ�

        //�V�����I�����Ă������̂�I�����Ă����Ԃɂ���
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_fontChoices[m_cursorPosition]->Activate();
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
        m_fontChoices[m_cursorPosition]->Deactivate();

        ++m_cursorPosition; //�J�[�\�����ړ�

        //�V�����I�����Ă������̂�I�����Ă����Ԃɂ���
        m_spriteChoices[m_cursorPosition]->SetMulColor(SRns::COLOR_NORMAL);
        m_fontChoices[m_cursorPosition]->Activate();
        m_flagDrawMove = true;
    }
}

void ModeSelect::FinishCPUStrengthSelect()
{
    m_flagFinish = true;
}

void ModeSelect::DrawMove()
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