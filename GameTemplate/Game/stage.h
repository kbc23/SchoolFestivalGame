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
class SuddenDeathMode;
class CPUPlayerController;
class Pause;
class GameData;

class Stage : public IGameObject
{
public:
    Stage();
    ~Stage();
    bool Start() override final;
    void Init();
    void Finish();
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

    void DrawBlock(const int playerNum);

    void DrawMoveBlock(const int playerNum);

    void DrawFontPlayerBlockPosition(const int playerNum);

    void DrawBackground();

    void DrawRoundWin();


public:

    //////////////////////////////
    // �u���b�N���Ƃ̏���
    //////////////////////////////

    /**
     * @brief �v���C���[������Ă���u���b�N�𔻕�
     * @param playerNum �v���C���[�ԍ�
    */
    void CheckBlock(const int playerNum);

    //////////
    // �F�̃u���b�N
    //////////

    /**
     * @brief �F�̃u���b�N�̏�ɍs�����Ƃ��̏���    
     * @param playerNum �v���C���[�ԍ�
    */
    void BlueBlock(const int playerNum);

    //////////
    // ���F�̃u���b�N
    //////////

    /**
     * @brief ���F�̃u���b�N�̏�ɍs�����Ƃ��̏���
     * @param playerNum �v���C���[�ԍ�
    */
    void YellowBlock(const int playerNum);

    //////////
    // ���̑�
    //////////

    /**
     * @brief �O�ɂ����ʒu�̃u���b�N�ɖ߂鏈��
     * @param playerNum �v���C���[�̔ԍ�
    */
    void ReturnBlock(const int playerNum);

private:

    //////////////////////////////
    // �S�[�����̏���
    //////////////////////////////

    /**
     * @brief �S�[�����̏���
    */
    void GoalBlock();


public:
    //////////////////////////////
    // ���E���h�ύX�̏���
    //////////////////////////////

    /**
     * @brief ���E���h�ύX�̏���
    */
    void NextRound();


private:
    //////////////////////////////
    // �����ɂ�鏟������
    //////////////////////////////

    /**
     * @brief 20�}�X����Ă��邩�ǂ���
    */
    void CheckPlayerDistance();

    /**
     * @brief �v���C���[���P�ʂ̏����𖞂����Ă��邩�ǂ���
     * @param playerNum �v���C���[�̔ԍ�
     * @return �����𖞂����Ă��邩
    */
    bool CheckPlayerRank1(const int playerNum);

    /**
     * @brief �v���C���[��20�u���b�N�ȏ㋗���𗣂��Ă��邩
     * @param playerNum �v���C���[�̔ԍ�
     * @param otherNum ���̃v���C���[�̔ԍ�
     * @return �����𗣂��Ă��邩
    */
    bool CheckPlayerDistance20Block(const int playerNum, const int otherNum);

    /**
     * @brief �����̏����Ńv���C���[�����������Ƃ��̏���
     * @param playerNum �v���C���[�̔ԍ�
    */
    void WinPlayerDistance(const int playerNum);

public:
    //////////////////////////////
    // �u���b�N�̈ړ�
    //////////////////////////////

    /**
     * @brief �v���C���[�̈ړ��ɉ����ău���b�N�𓮂����B
     * @param playerNum �v���C���[�̔ԍ�
     * @param moveNum �u���b�N�̓�����
     * @return �u���b�N�𓮂��������ǂ���
    */
    bool MoveBlock(const int playerNum, const int moveNum);

    //////////////////////////////
    // �i�s�x
    //////////////////////////////

    void DegreeOfProgress(const int playerNum);



private: //�ǉ�
    void MissRoundWin();

    void BlockDeactivate(const int playerNum);


public: //Get�֐�

    /**
     * @brief m_stageData��2����擾
     * @param playerNum �v���C���[�ԍ�
     * @return ���݁A�v���C���[������u���b�N�̂Q��̃u���b�N�̏��
    */
    int& GetStageDateNext2Block(const int playerNum)
    {
        return m_stageData[playerNum][m_playerBlockPosition[playerNum] + 2];//����������
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

public: //Set�֐�
    /**
     * @brief ���삷��v���C���[�̐l����ۑ�����ϐ���Set�֐�
     * @param playerNum ���삷��v���C���[�̐l��
    */
    void SetMaxPlayer(const int playerNum)
    {
        m_maxPlayer = playerNum;
    }   


private: //constant
    static const int m_MAX_BLOCK = 100;      //�P���[���̃u���b�N�̍ő吔
    static const int m_START_BLOCK = 0;     //�X�^�[�g�ʒu�̃u���b�N�̔ԍ�
    static const int m_INIT_RANK = con::rank_1;       //�v���C���[�ɓn�����ʃf�[�^�̏����l
    
    static const int m_MAX_GREEN_BLOCK = 20;            //�쐬����΃u���b�N�̃��f���̐�
    static const int m_MAX_YELLOW_BLOCK = 10;    //�쐬����A���F�u���b�N�̃��f���̐�
    static const int m_MAX_GOAL_BLOCK = 1;      //�쐬����S�[���u���b�N�̃��f���̐�

    static const int m_MAX_RAUND_WIN = 3;

private: //data menber

    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    ModelRender* m_modelGreenBlock[con::PlayerNumberMax][m_MAX_GREEN_BLOCK] = { nullptr };
    ModelRender* m_modelYellowBlock[con::PlayerNumberMax][m_MAX_YELLOW_BLOCK] = { nullptr };
    ModelRender* m_modelGoalBlock[con::PlayerNumberMax][m_MAX_GOAL_BLOCK] = { nullptr };
    SoundBGM* m_bgm = nullptr;
    FontRender* m_fontPlayerBlockPosition[con::PlayerNumberMax] = { nullptr }; //�v���C���[�����ڂ̃u���b�N�ɂ��邩�̕\��
    SpriteRender* m_spriteBackgroundSky = nullptr;
    SpriteRender* m_spriteBackgroundCloud_1 = nullptr;
    SpriteRender* m_spriteBackgroundCloud_2 = nullptr;

    SpriteRender* m_spriteDegreeOfProgress = nullptr;
    SpriteRender* m_spritePlayerMark[con::PlayerNumberMax] = { nullptr };

    SpriteRender* m_spriteRoundWin[con::PlayerNumberMax][m_MAX_RAUND_WIN] = { nullptr };

    //////////////////////////////
    // FindGO
    //////////////////////////////

    GameData* m_findGameData = nullptr;
    Player* m_findPlayer = nullptr;
    CPUPlayerController* m_findCPUPlayerController = nullptr;
    MainProcessing* m_findMainProcessing = nullptr;
    SuddenDeathMode* m_findSuddenDeathMode = nullptr;
    Pause* m_findPause = nullptr;

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

    int m_nowRank = m_INIT_RANK;            //�v���C���[�̏��ʃf�[�^�ɓn���f�[�^

    int m_amountOfMovement[con::PlayerNumberMax] = { 0, 0, 0, 0 };

    StageData m_playerAnimation[con::PlayerNumberMax] = { greenBlock ,greenBlock ,greenBlock ,greenBlock };

    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    bool m_flagAnimationJump[con::PlayerNumberMax] = { false, false, false, false };	//�W�����v�A�j���[�V��������
    int m_timerAnimation[con::PlayerNumberMax] = { 0, 0, 0, 0 };						//�A�j���[�V�����̃^�C�}�[

    ///////////////////////////////////////////////////////////
    // NextRound
    ///////////////////////////////////////////////////////////

    int m_maxPlayer = con::PlayerNumberMax;	//�v���C���[�̍ő吔
    int m_goalPlayer = 0;          //�S�[�������v���C���[�̐�
    //int m_nextTime = 0;          //���̃��E���h�Ɉڂ�̂Ɉ�u�ōs���Ȃ����߂̑҂�����
    bool m_allMiss = false;     //�v���C���[�S�����~�X�����Ă��邩
};