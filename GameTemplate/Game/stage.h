#pragma once
#include "model_render.h"
#include "constant.h"

class Player;

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();

    ////////////////////////////////////////////////////////////
    // �����ݒ�
    ////////////////////////////////////////////////////////////

    bool Start() override final;

    /**
     * @brief �X�e�[�W�̍쐬����
    */
    void StageCreate();

    ////////////////////////////////////////////////////////////
    // ���t���[���̏���
    ////////////////////////////////////////////////////////////

    void Update() override final;



    //////////////////////////////
    // �u���b�N�̈ړ�
    //////////////////////////////

    /**
     * @brief �v���C���[�̈ړ��ɉ����ău���b�N�𓮂����B
     * @param pNum �v���C���[�̔ԍ�
     * @param moveNum �u���b�N�̓�����
     * @return �u���b�N�𓮂��������ǂ���
    */
    bool MoveBlock(const int pNum, const int moveNum);

    //////////////////////////////
    // �^�C�}�[
    //////////////////////////////

    /**
     * @brief ����s�\��Ԃ̃^�C�}�[�̃J�E���g
     * @param pNum �v���C���[�̔ԍ�
    */
    void ReturnOperationTimer(const int pNum);

    //////////////////////////////
    // �u���b�N���Ƃ̏���
    //////////////////////////////

    /**
     * @brief �v���C���[������Ă���u���b�N�𔻕�
     * @param pNum �v���C���[�ԍ�
    */
    void CheckBlock(const int pNum);

    //////////
    // �F�̃u���b�N
    //////////

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

    //////////////////////////////
    // �S�[�����̏���
    //////////////////////////////

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
    ModelRender* m_modelRender[con::PlayerNumberMax][m_MAX_BLOCK] = { nullptr }; //[�v���C���[�ԍ�][�X�e�[�W�̃}�X�̐�]


    ////////////////////////////////////////////////////////////
    // �u���b�N�̃f�[�^
    ////////////////////////////////////////////////////////////

    int m_stageData[con::PlayerNumberMax][m_MAX_BLOCK] = { greenBlock }; //�X�e�[�W�̃f�[�^��ۑ�����z��
    int m_playerBlockPosition[con::PlayerNumberMax] = {                  //�v���C���[�����ڂ̃u���b�N�ɂ��邩
        m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };
    int m_playerBeforeBlockPosition[con::PlayerNumberMax] = {            //�v���C���[�̑O�ɂ����u���b�N�̔ԍ�
        m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };

    ////////////////////////////////////////////////////////////
    // �v���C���[�̑����
    ////////////////////////////////////////////////////////////

    bool m_activeOperation[con::PlayerNumberMax] = { true, true, true, true };   //�v���C���[������\��
    int m_timerReturnOperation[con::PlayerNumberMax] = { 0, 0, 0, 0 };           //�v���C���[�̑��앜�A�̃^�C�}�[
    bool m_resistanceImpossibleOperation[con::PlayerNumberMax] = {               //�v���C���[�̑���s��Ԃɑ΂���ϐ������邩
        false, false, false, false };


    bool m_flagAnimationBlueBlock[con::PlayerNumberMax] = {                 //���u���b�N�ɍs�����Ƃ��̃A�j���[�V�����������Ȃ��Ă��邩
        false, false, false, false };
    int m_timerAnimationBlueBlock[con::PlayerNumberMax] = { 0, 0, 0, 0 };   //���u���b�N�ɍs�����Ƃ��̃A�j���[�V�����̃^�C�}�[
    bool m_activeOperationVersionBlue[con::PlayerNumberMax] = {             //�v���C���[������\���i�F�̃u���b�NVer�j
        true, true, true, true };

    int m_nowRank = m_INIT_RANK;            //�v���C���[�̏��ʃf�[�^�ɓn���f�[�^
};