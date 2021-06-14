#pragma once
#include "model_render.h"

#include "player.h"

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start() override final;
    void Update() override final;


public:
    ////////////////////////////////////////////////////////////
    // �����ݒ�
    ////////////////////////////////////////////////////////////

    /**
     * @brief �X�e�[�W�̍쐬����
    */
    void StageCreate();

    ////////////////////////////////////////////////////////////
    // �u���b�N�̈ړ�
    ////////////////////////////////////////////////////////////

    /**
     * @brief �v���C���[�̈ړ��ɉ����ău���b�N�𓮂����B
     * @param pNum �v���C���[�̔ԍ�
     * @param moveNum �u���b�N�̓�����
     * @return �u���b�N�𓮂��������ǂ���
    */
    bool MoveBlock(const int pNum, const int moveNum);

    ////////////////////////////////////////////////////////////
    // �^�C�}�[
    ////////////////////////////////////////////////////////////

    /**
     * @brief ����s�\��Ԃ̃^�C�}�[�̃J�E���g
     * @param pNum �v���C���[�̔ԍ�
    */
    void ReturnOperationTimer(const int pNum);

    ////////////////////////////////////////////////////////////
    // �u���b�N���Ƃ̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief �v���C���[������Ă���u���b�N�𔻕�
     * @param pNum �v���C���[�ԍ�
    */
    void CheckBlock(const int pNum);

    //////////////////////////////
    // �F�̃u���b�N
    //////////////////////////////

    /**
     * @brief �F�̃u���b�N�̏�ɍs�����Ƃ��̏���    
     * @param pNum �v���C���[�ԍ�
    */
    void BlueBlock(const int pNum);

    /**
     * @brief �F�̃u���b�N�̏�ɍs�����Ƃ��̃A�j���[�V����
     * @param pNum �v���C���[�ԍ�
    */
    void BlueBlockAnimation(const int pNum);

    /**
     * @brief �O�ɂ����ʒu�̃u���b�N�ɖ߂鏈��
     * @param pNum �v���C���[�̔ԍ�
    */
    void ReturnBlock(const int pNum);

    ////////////////////////////////////////////////////////////
    // �S�[�����̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief �S�[�����̏���
    */
    void GoalBlock();



public: //Get�֐�
    /**
     * @brief m_activeOperation[pNum]��Get�֐�
     * @param pNum �v���C���[�ԍ�
     * @return �v���C���[�ԍ�[pNum]���A����\��
    */
    const bool GetmActiveOperation(const int pNum)
    {
        return m_activeOperation[pNum];
    }


public: //enum
    //�u���b�N�̎��
    enum StageData
    {
        greenBlock,
        blueBlock,
        yellowBlock,
        StageDataMax
    };



private: //constant
    static const int m_MAX_BLOCK = 50;      //�P���[���̃u���b�N�̍ő吔
    static const int m_START_BLOCK = 0;     //�X�^�[�g�ʒu�̃u���b�N�̔ԍ�
    static const int m_INIT_RANK = 1;       //�v���C���[�ɓn�����ʃf�[�^�̏����l

private: //data menber

    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    Player* m_player = nullptr;

    ////////////////////////////////////////////////////////////
    // �u���b�N�̃f�[�^
    ////////////////////////////////////////////////////////////

    int m_stageData[Player::PlayerNumberMax][m_MAX_BLOCK] = { greenBlock }; //�X�e�[�W�̃f�[�^��ۑ�����z��
    //[�v���C���[�ԍ�][�X�e�[�W�̃}�X�̐�]
    ModelRender* m_modelRender[Player::PlayerNumberMax][m_MAX_BLOCK] = { nullptr };
 
    //�v���C���[�����ڂ̃u���b�N�ɂ��邩
    int m_playerBlockPosition[Player::PlayerNumberMax] = { m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };
    //�v���C���[�̑O�ɂ����u���b�N�̔ԍ�
    int m_playerBeforeBlockPosition[Player::PlayerNumberMax] = { m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };

    ////////////////////////////////////////////////////////////
    // �v���C���[�̑����
    ////////////////////////////////////////////////////////////

    //�v���C���[������\��
    bool m_activeOperation[Player::PlayerNumberMax] = { true, true, true, true };
    //�v���C���[�̑��앜�A�̃^�C�}�[
    int m_timerReturnOperation[Player::PlayerNumberMax] = { 0, 0, 0, 0 };
    //�v���C���[�̑���s��Ԃɑ΂���ϐ������邩
    bool m_resistanceImpossibleOperation[Player::PlayerNumberMax] = { false, false, false, false };

    //���u���b�N�ɍs�����Ƃ��̃A�j���[�V�����������Ȃ��Ă��邩
    bool m_flagAnimationBlueBlock[Player::PlayerNumberMax] = { false, false, false, false };
    //���u���b�N�ɍs�����Ƃ��̃A�j���[�V�����̃^�C�}�[
    int m_timerAnimationBlueBlock[Player::PlayerNumberMax] = { 0, 0, 0, 0 };
    //�v���C���[������\���i�F�̃u���b�NVer�j
    bool m_activeOperationVersionBlue[Player::PlayerNumberMax] = { true, true, true, true };

    int m_nowRank = m_INIT_RANK; //�v���C���[�̏��ʃf�[�^�ɓn���f�[�^
};