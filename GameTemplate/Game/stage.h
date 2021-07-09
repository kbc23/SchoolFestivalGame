#pragma once
#include "model_render.h"
#include "sound_BGM.h"
#include "effect_render.h"
#include "font_render.h"
#include "sprite_render.h"
#include "constant.h"

class Player;
class Score;
class MainProcessing;
class Rule1;

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start() override final;
    void Init() override final;
    void Finish() override final;
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

    /**
     * @brief �Đ�����BGM�̌���
    */
    void InitBGM();

    ////////////////////////////////////////////////////////////
    // ���t���[���̏���
    ////////////////////////////////////////////////////////////



    void DrawBlock(const int pNum);

    void DrawMoveBlock(const int pNum);

    void DrawFontPlayerBlockPosition(const int pNum);

    void DrawBackground();

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

    //////////////////////////////
    // ���E���h�ύX�̏���
    //////////////////////////////


    /**
     * @brief ���E���h�ύX�̏���
    */
    void NextRound();

    /**
     * @brief 20�}�X����Ă��邩�ǂ���
    */

    void Length();

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

    //////////////////////////////
    // �i�s�x
    //////////////////////////////

    void DegreeOfProgress(const int& pNum);



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
    /**
* @brief m_stageData��2���Get�֐�
* @param pNum �v���C���[�ԍ�
* @return m_stageData��2��̃u���b�N
*/
    int GetStageDatePuls2(const int pNum)
    {
        /* int plus = m_playerBlockPosition[pNum] + 2;
          if (plus >= 99) {
              plus = 99;
          }*/
        return m_stageData[pNum][m_playerBlockPosition[pNum] + 2];//����������
    }

    const int Getm(const int o)
    {
        return m;
    }

public: //enum
    //�u���b�N�̎��
    enum StageData
    {
        greenBlock,
        blueBlock,
        yellowBlock,
        goalBlock,
        stageDataMax
    };

    /**
     * @brief ���삷��v���C���[�̐l����ۑ�����ϐ���Set�֐�
     * @param i ���삷��v���C���[�̐l��
    */
    void SetMaxPlayer(const int i)
    {
        m_maxPlayer = i;
    }

    public:
        
        bool stop = false;  //���F�A�ɏ��������1�Q�[������s�\�t���O
        bool rule1NewGO = false;
        

private: //constant
    static const int m_MAX_BLOCK = 100;      //�P���[���̃u���b�N�̍ő吔
    static const int m_START_BLOCK = 0;     //�X�^�[�g�ʒu�̃u���b�N�̔ԍ�
    static const int m_INIT_RANK = 1;       //�v���C���[�ɓn�����ʃf�[�^�̏����l
    
    static const int m_MAX_GREEN_BLOCK = 20;            //�쐬����΃u���b�N�̃��f���̐�
    static const int m_MAX_YELLOW_BLOCK = 10;    //�쐬����A���F�u���b�N�̃��f���̐�
    static const int m_MAX_GOAL_BLOCK = 1;      //�쐬����S�[���u���b�N�̃��f���̐�

private: //data menber

    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    Player* m_player = nullptr;
    Score* m_score = nullptr;
    //ModelRender* m_modelRender[con::PlayerNumberMax][m_MAX_BLOCK] = { nullptr }; //[�v���C���[�ԍ�][�X�e�[�W�̃}�X�̐�]
    ModelRender* m_modelGreenBlock[con::PlayerNumberMax][m_MAX_GREEN_BLOCK] = { nullptr };
    //ModelRender* m_modelBlueBlock[con::PlayerNumberMax][m_MAX_YELLOW_BLOCK] = { nullptr };
    ModelRender* m_modelYellowBlock[con::PlayerNumberMax][m_MAX_YELLOW_BLOCK] = { nullptr };
    ModelRender* m_modelGoalBlock[con::PlayerNumberMax][m_MAX_GOAL_BLOCK] = { nullptr };
    SoundBGM* m_bgm = nullptr;
    FontRender* m_fontPlayerBlockPosition[con::PlayerNumberMax] = { nullptr }; //�v���C���[�����ڂ̃u���b�N�ɂ��邩�̕\��
    SpriteRender* m_spriteBackgroundSky = nullptr;
    SpriteRender* m_spriteBackgroundCloud_1 = nullptr;
    SpriteRender* m_spriteBackgroundCloud_2 = nullptr;
    
    SpriteRender* m_spriteDegreeOfProgress = nullptr;
    SpriteRender* m_spritePlayerMark[con::PlayerNumberMax] = { nullptr };

    MainProcessing* m_game = nullptr;

    

    Rule1* m_rule1 = nullptr;

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

    int m_amountOfMovement[con::PlayerNumberMax] = { 0, 0, 0, 0 };

    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//�W�����v�A�j���[�V��������
    int m_timerAnimation[con::PlayerNumberMax] = { 0, 0, 0, 0 };						//�A�j���[�V�����̃^�C�}�[

    int Playermember = 0;

    ///////////////////////////////////////////////////////////
    // NextRound
    ///////////////////////////////////////////////////////////
    int m_maxPlayer = con::PlayerNumberMax;	//�v���C���[�̍ő吔
    int n = 0;          //�S�[�������v���C���[�̐�
    int m = 0;          //���̃��E���h�Ɉڂ�̂Ɉ�u�ōs���Ȃ����߂̑҂�����
    bool m_allMiss = false;     //�v���C���[�S�����~�X�����Ă��邩

    ///////////////////////////////////////////////////////////
    // Length
    ///////////////////////////////////////////////////////////

    int j = 0;          //��Ԑi��ł���l�̃u���b�N��
    int t = 0;          //2�Ԗڂɐi��ł���l�̃u���b�N��
};