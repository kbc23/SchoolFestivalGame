#pragma once
#include "model_render.h"
#include "sound_BGM.h"
#include "effect_render.h"
#include "font_render.h"
#include "constant.h"

class Player;

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start() override final;
    void Update() override final;


private:
    ////////////////////////////////////////////////////////////
    // �����ݒ�
    ////////////////////////////////////////////////////////////

    /**
     * @brief �X�e�[�W�̍쐬����
    */
    void StageCreate();

    /**
     * @brief �΃u���b�N���쐬���邩�ǂ����̏���
     * @param continuousGreenBlock �A���ō쐬���Ă���΃u���b�N�̐�
     * @param randomGreenNum �΃u���b�N���쐬���邩�̗���
     * @return �΃u���b�N���쐬���邩
    */
    bool CreateGreen(const int continuousGreenBlock, const int randomGreenNum);

    /**
     * @brief �u���b�N�Ɖ��F�u���b�N�̂ǂ�����쐬���邩
     * @param blockNum ���ݍ쐬���Ă���u���b�N�̃u���b�N�ԍ�
     * @param randomBlueOrYellowNum �u���b�N�Ɖ��F�u���b�N�̂ǂ�����쐬���邩�̗���
    */
    void CreateBlueOrYellow(const int blockNum, const int randomBlueOrYellowNum);

    ////////////////////////////////////////////////////////////
    // ���t���[���̏���
    ////////////////////////////////////////////////////////////



    void DrawBlock(const int pNum);

    void DrawMoveBlock(const int pNum);

    void DrawFontPlayerBlockPosition(const int pNum);

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


public:
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
        stageDataMax
    };

    public:
        bool stop = false;  //���F�A�ɏ��������1�Q�[������s�\�t���O

private: //constant
    static const int m_MAX_BLOCK = 100;      //�P���[���̃u���b�N�̍ő吔
    static const int m_START_BLOCK = 0;     //�X�^�[�g�ʒu�̃u���b�N�̔ԍ�
    static const int m_INIT_RANK = 1;       //�v���C���[�ɓn�����ʃf�[�^�̏����l

    static const int m_MAX_GREEN_BLOCK = 20;            //�쐬����΃u���b�N�̃��f���̐�
    static const int m_MAX_YELLOW_BLOCK = 10;    //�쐬����A���F�u���b�N�̃��f���̐�

private: //data menber

    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    Player* m_player = nullptr;
    //ModelRender* m_modelRender[con::playerNumberMax][m_MAX_BLOCK] = { nullptr }; //[�v���C���[�ԍ�][�X�e�[�W�̃}�X�̐�]
    ModelRender* m_modelGreenBlock[con::playerNumberMax][m_MAX_GREEN_BLOCK] = { nullptr };
    //ModelRender* m_modelBlueBlock[con::playerNumberMax][m_MAX_YELLOW_BLOCK] = { nullptr };
    ModelRender* m_modelYellowBlock[con::playerNumberMax][m_MAX_YELLOW_BLOCK] = { nullptr };
    SoundBGM* m_bgm = nullptr;
    EffectRender* m_testEffect = nullptr;
    FontRender* m_fontPlayerBlockPosition[con::playerNumberMax] = { nullptr }; //�v���C���[�����ڂ̃u���b�N�ɂ��邩�̕\��

    ////////////////////////////////////////////////////////////
    // �u���b�N�̃f�[�^
    ////////////////////////////////////////////////////////////

    int m_stageData[con::playerNumberMax][m_MAX_BLOCK] = { greenBlock }; //�X�e�[�W�̃f�[�^��ۑ�����z��
    int m_playerBlockPosition[con::playerNumberMax] = {                  //�v���C���[�����ڂ̃u���b�N�ɂ��邩
        m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };
    int m_playerBeforeBlockPosition[con::playerNumberMax] = {            //�v���C���[�̑O�ɂ����u���b�N�̔ԍ�
        m_START_BLOCK,m_START_BLOCK,m_START_BLOCK,m_START_BLOCK };

    ////////////////////////////////////////////////////////////
    // �v���C���[�̑����
    ////////////////////////////////////////////////////////////

    bool m_activeOperation[con::playerNumberMax] = { true, true, true, true };   //�v���C���[������\��
    int m_timerReturnOperation[con::playerNumberMax] = { 0, 0, 0, 0 };           //�v���C���[�̑��앜�A�̃^�C�}�[
    bool m_resistanceImpossibleOperation[con::playerNumberMax] = {               //�v���C���[�̑���s��Ԃɑ΂���ϐ������邩
        false, false, false, false };


    bool m_flagAnimationBlueBlock[con::playerNumberMax] = {                 //���u���b�N�ɍs�����Ƃ��̃A�j���[�V�����������Ȃ��Ă��邩
        false, false, false, false };
    int m_timerAnimationBlueBlock[con::playerNumberMax] = { 0, 0, 0, 0 };   //���u���b�N�ɍs�����Ƃ��̃A�j���[�V�����̃^�C�}�[
    bool m_activeOperationVersionBlue[con::playerNumberMax] = {             //�v���C���[������\���i�F�̃u���b�NVer�j
        true, true, true, true };

    int m_nowRank = m_INIT_RANK;            //�v���C���[�̏��ʃf�[�^�ɓn���f�[�^

    int m_amountOfMovement[con::playerNumberMax] = { 0, 0, 0, 0 };

    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    bool m_flagAnimationJump[con::playerNumberMax] = { false, false, false, false };	//�W�����v�A�j���[�V��������
    int m_timerAnimation[con::playerNumberMax] = { 0, 0, 0, 0 };						//�A�j���[�V�����̃^�C�}�[
};