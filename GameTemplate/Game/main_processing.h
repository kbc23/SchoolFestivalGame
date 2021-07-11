#pragma once
#include "font_render.h"
#include "sprite_render.h"
#include "fade.h"
#include "sound_BGM.h"
#include "sound_SE.h"
#include "constant.h"


class Title;
class ModeSelect;
class PlayerSelect;
class CPUStrengthSelect;
class Game;
class Result;

class GameCamera;




class MainProcessing : public IGameObject
{
public:
    MainProcessing();
    ~MainProcessing();
    bool Start() override final;
    void Update() override final;


private:
    ////////////////////////////////////////////////////////////
    // ���t���[�����鏈��
    ////////////////////////////////////////////////////////////

    void DrawBackground();

    ////////////////////////////////////////////////////////////
    // �ŏ��̓ǂݍ���
    ////////////////////////////////////////////////////////////

    void StartLoadingScene();

    /**
     * @brief ���[�h�̏���
    */
    void StartLoadingPreparation();

    void StartLoading();

    void StartLoadingFinish();

    ////////////////////////////////////////////////////////////
    // �^�C�g���V�[���̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief �^�C�g���V�[���̏���
    */
    void TitleScene();

    /**
     * @brief ����I����
    */
    void DecisionTitleScene();

    ////////////////////////////////////////////////////////////
    // ���[�h�Z���N�g�V�[���̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief ���[�h�Z���N�g�V�[���̏���
    */
    void ModeSelectScene();

    /**
     * @brief ����I����
    */
    void DecisionModeSelectScene();

    /**
     * @brief �L�����Z���I����
    */
    void CancelModeSelectScene();

    ////////////////////////////////////////////////////////////
    // �v���C���[�Z���N�g�V�[���̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief �v���C���[�Z���N�g�V�[���̏���
    */
    void PlayerSelectScene();

    /**
     * @brief ����I����
    */
    void DecisionPlayerSelectScene();

    /**
     * @brief �L�����Z���I����
    */
    void CancelPlayerSelectScene();

    ////////////////////////////////////////////////////////////
    // CPU�̓�Փx�I���V�[���̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief �^�C�g���V�[���̏���
    */
    void CPUStrengthSelectScene();

    /**
     * @brief ����I����
    */
    void DecisionCPUStrengthSelectScene();

    /**
     * @brief �L�����Z���I����
    */
    void CancelCPUStrengthSelectScene();

    ////////////////////////////////////////////////////////////
    // �Q�[���V�[���̂��߂̃��[�h
    ////////////////////////////////////////////////////////////

    void LoadingGameScene();

    void PreparingForLoading();

    void Loading();

    void EndOfLoading();

    ////////////////////////////////////////////////////////////
    // �Q�[���V�[���̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief �Q�[���V�[���̏���
    */
    void GameScene();

    /**
     * @brief �Q�[���J�n���̃J�E���g�_�E��
    */
    void StartCountdown();

    /**
     * @brief �Q�[���V�[���I����
    */
    void FinishGameScene();

    ////////////////////////////////////////////////////////////
    // ���U���g�V�[���̏���
    ////////////////////////////////////////////////////////////
    /**
     * @brief ���U���g�V�[���̏���
    */
    void ResultScene();

public: //Get�֐�
    /**
     * @brief m_StopOperation��Get�֐�
     * @return �v���C���[�̑��쏈�����ł��Ȃ����Ă��邩
    */
    bool GetStopOperation()
    {
        return m_StopOperation;
    }

    const bool& GetRuleSelect()
    {
        return m_ruleSelect;
    }

    /*const bool& GetRuleSelect()
    {
        return m_ruleSelect;
    }*/

public: //Set�֐�
    ///**
    // * @brief m_flagPlayerSelectScene��Set�֐�
    // * @param b �v���C���[�Z���N�g�V�[���̏��������Ă��邩
    //*/
    //void SetFlagPlayerSelectScene(const bool b)
    //{
    //    m_flagPlayerSelectScene = b;
    //}

    ///**
    // * @brief m_flagGameScene��Set�֐�
    // * @param b �Q�[���V�[���̏��������Ă��邩
    //*/
    //void SetFlagGameScene(const bool b)
    //{
    //    m_flagGameScene = b;
    //}

    /**
     * @brief m_maxPlayer��Set�֐�
     * @param i ���삵�Ă���v���C���[�̐�
    */
    void SetMaxPlayer(const int i)
    {
        m_maxPlayer = i;
    }

    void SetRank(int pNum, int rank) {//���ʎ󂯎��tuika
        m_rank[pNum] = rank;
    }
    void SetDiLevel(const int& i)//��Փx�󂯎��tuika
    {
        m_dilevel = i;
    }
    void SetResultSelect(const int i)//���U���g�I���󂯎��tuika
    {
        m_resultselect = i;
    }

    void SetRuleSelect(const bool& b)
    {
        m_ruleSelect = b;
    }

    void SetGameEnd(const bool& b)
    {
        m_gameEnd = b;
    }

    void SetPause_Stage(const bool& b)
    {
        m_pause_stage = b;
    }

    void SetPause_Title(const bool& b)
    {
        m_pause_title = b;
    }


private: //enum
    enum class GameStatus
    {
        startLoading,
        title,
        modeSelect,
        playerSelect,
        CPUStrengthSelect,
        loadingGame,
        game,
        result,
        GameStatusMax
    };

    enum class StartLoadingStatus
    {
        preparation,    //���[�h�̏���
        loading,        //���[�h��
        finish,         //���[�h�I��
        StartLoadingStatusMax
    };

    enum class LoadingStatus
    {
        doNothing,              //�������Ă��Ȃ�
        preparingForLoading,    //���[�h�̏���
        loading,                //���[�h��
        endOfLoading,           //���[�h�I��
        LoadingStatusMax
    };


public:
    //bool goal1 = false;

    void NextRound();

private: //constant
    static const int m_INIT_COUNT_START_COUNTDOWN = 240;        //m_countStartCountdown�̏����l

private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    Title* m_title = nullptr;
    ModeSelect* m_modeSelect = nullptr;
    PlayerSelect* m_playerSelect = nullptr;
    CPUStrengthSelect* m_CPUStrengthSelect = nullptr;
    Game* m_game = nullptr;
    Result* m_result = nullptr;

    GameCamera* m_gameCamera = nullptr;
    
    
    FontRender* m_fontStartCountdown = nullptr;     //�J�E���g�_�E���̃t�H���g
    SpriteRender* m_spriteBackground[7] = { nullptr };
    SpriteRender* m_spriteCountdown[4] = { nullptr };
    SpriteRender* m_spriteLoading = nullptr;
    Fade* m_fade = nullptr;
    SoundBGM* m_bgmTitle = nullptr;
    SoundSE* m_seCancel = nullptr;
    SoundSE* m_seCount = nullptr;
    SoundSE* m_seGameStart = nullptr;
    
    

    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    int m_countStartCountdown = m_INIT_COUNT_START_COUNTDOWN;       //�J�E���g�_�E���Ŏg�p�����^�C�}�[
    bool m_flagStartCountdown = true;                               //�J�E���g�_�E���������Ȃ��Ă��邩�̃t���O
    bool m_StopOperation = true;                                    //�v���C���[�̑��삪�ł��邩
    bool m_gameSceneFlagFinish = false;//�Q�[���V�[���ł�邱�Ƃ��I����Ă��邩tuika
    
    ////////////////////////////////////////////////////////////
    // �t���O�֘A
    ////////////////////////////////////////////////////////////

    GameStatus m_gameStatus = GameStatus::startLoading;
    StartLoadingStatus m_startLoadingStatus = StartLoadingStatus::preparation;
    LoadingStatus m_loadStatus = LoadingStatus::doNothing;
    bool m_startPreparingForLoading = false;
    bool m_startEndOfLoading = false;
    bool m_flagGameStart = false;

    bool m_gameEnd = false;

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_maxPlayer = 0;                        //���삵�Ă���v���C���[�̐�
    int m_rank[con::PlayerNumberMax] = { 0,0,0,0 };
    int m_dilevel = 0;//��Փx�󂯎��󂯓n���p
    int m_resultselect = 0;//���U���g�ŉ��I�񂾂�
    //bool m_ruleSelect = false;

    bool m_ruleSelect = false;

    bool m_pause_stage = false;
    
    bool m_pause_title = false;

};

