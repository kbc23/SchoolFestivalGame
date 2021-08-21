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

    /**
     * @brief �w�i�摜���ړ������鏈��
    */
    void DrawBackground();

    ////////////////////////////////////////////////////////////
    // �V�[�����Ƃ̏���
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // �Q�[���J�n���̃��[�h�̏���
    //////////////////////////////

    /**
     * @brief �Q�[���J�n���̃��[�h
    */
    void StartLoading();

    //////////////////////////////
    // �^�C�g���V�[���̏���
    //////////////////////////////

    /**
     * @brief �^�C�g���V�[���̏���
    */
    void TitleScene();

    /**
     * @brief ����I����
    */
    void DecisionTitleScene();

    //////////////////////////////
    // ���[�h�Z���N�g�V�[���̏���
    //////////////////////////////

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

    //////////////////////////////
    // �v���C���[�Z���N�g�V�[���̏���
    //////////////////////////////

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

    //////////////////////////////
    // CPU�̓�Փx�I���V�[���̏���
    //////////////////////////////

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

    //////////////////////////////
    // �Q�[���V�[���̂��߂̃��[�h
    //////////////////////////////

    /**
     * @brief ���[�h�̏���
    */
    void LoadingGameScene();

    /**
     * @brief ���[�h�̏���
    */
    void PreparingForLoading();

    /**
     * @brief ���[�h
    */
    void Loading();

    /**
     * @brief ���[�h�̏I���
    */
    void EndOfLoading();

    //////////////////////////////
    // �Q�[���V�[���̏���
    //////////////////////////////

    /**
     * @brief �Q�[���V�[���̏���
    */
    void GameScene();

    /**
     * @brief �Q�[���V�[���I����
    */
    void FinishGameScene();

    //////////////////////////////
    // ���U���g�V�[���̏���
    //////////////////////////////

    /**
     * @brief ���U���g�V�[���̏���
    */
    void ResultScene();



public: //Get�֐�
    /**
     * @brief m_StopOperation��Get�֐�
     * @return �v���C���[�̑��쏈�����ł��Ȃ����Ă��邩
    */
    const bool GetStopOperation()
    {
        return m_StopOperation;
    }

    const bool GetFlagSuddenDeathMode()
    {
        return m_flagSuddenDeathMode;
    }

public: //Set�֐�
    /**
     * @brief m_maxPlayer��Set�֐�
     * @param maxPlayer ���삵�Ă���v���C���[�̐�
    */
    void SetMaxPlayer(int maxPlayer)
    {
        m_maxPlayer = maxPlayer;
    }

    /**
     * @brief �v���C���[�̏��ʂ��Z�b�g
     * @param playerNum �v���C���[�̔ԍ�
     * @param rank ����
    */
    void SetRank(int playerNum, int rank)
    {
        m_rank[playerNum] = rank;
    }

    /**
     * @brief CPU�̋������Z�b�g
     * @param level CPU�̋���
    */
    void SetCPULevel(const con::CPULevel& level)
    {
        m_CPULevel = level;
    }

    /**
     * @brief ���U���g��ʂłǂ̑I������I�񂾂����Z�b�g
     * @param select ���U���g��ʂłǂ̑I������I�񂾂�
    */
    void SetResultSelect(int select)
    {
        m_resultselect = select;
    }

    /**
     * @brief �T�h���f�X���[�h��I�����������Z�b�g
     * @param flagSuddenDeathMode �T�h���f�X���[�h��I��������
    */
    void SetFlagSuddenDeathMode(bool flagSuddenDeathMode)
    {
        m_flagSuddenDeathMode = flagSuddenDeathMode;
    }

    /**
     * @brief �Q�[���̏������I����Ă��邩
     * @param flagGameEnd �Q�[���̏������I����Ă��邩
    */
    void SetGameEnd(bool flagGameEnd)
    {
        m_gameEnd = flagGameEnd;
    }

    /**
     * @brief �|�[�Y��ʂŁu���g���C�v��I�����������Z�b�g
     * @param flagPauseStage �|�[�Y��ʂŁu���g���C�v��I��������
    */
    void SetPause_Stage(bool flagPauseStage)
    {
        m_pause_stage = flagPauseStage;
    }

    /**
     * @brief �|�[�Y��ʂŁu�^�C�g���ɖ߂�v��I�����������Z�b�g
     * @param flagPauseTitle �|�[�Y��ʂŁu�^�C�g���ɖ߂�v��I��������
    */
    void SetPause_Title(bool flagPauseTitle)
    {
        m_pause_title = flagPauseTitle;
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

private: //constant


public: //constant
    static const int m_MAX_BACKGROUND = 7;                  //�w�i�摜�̍ő喇��



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
    
    
    SpriteRender* m_spriteBackground[m_MAX_BACKGROUND] = { nullptr };
    Fade* m_fade = nullptr;
    SoundBGM* m_bgmTitle = nullptr;
    SoundSE* m_seCancel = nullptr;

    
    

    ////////////////////////////////////////////////////////////
    // �^�C�}�[�֘A
    ////////////////////////////////////////////////////////////

    bool m_StopOperation = true; //�v���C���[�̑��삪�ł��邩
    bool m_gameSceneFlagFinish = false; //�Q�[���V�[���ł�邱�Ƃ��I����Ă��邩
    
    ////////////////////////////////////////////////////////////
    // �t���O�֘A
    ////////////////////////////////////////////////////////////

    GameStatus m_gameStatus = GameStatus::title;
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
    con::CPULevel m_CPULevel = con::easy;//��Փx�󂯎��󂯓n���p
    int m_resultselect = 0;//���U���g�ŉ��I�񂾂�

    bool m_flagSuddenDeathMode = false;

    bool m_pause_stage = false;
    
    bool m_pause_title = false;

};

