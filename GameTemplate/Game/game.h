#pragma once
#include "font_render.h"
#include "sprite_render.h"
#include "fade.h"
#include "sound_BGM.h"
#include "sound_SE.h"

class Title;
class ModeSelect;
class PlayerSelect;
class CPUStrengthSelect;
class Player;
class GameCamera;
class Stage;
class Score;
class Rule1;

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;
    void Update() override final;


private:
    ////////////////////////////////////////////////////////////
    // �^�C�g���V�[���̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief �^�C�g���V�[���̏���
    */
    void TitleScene();

    /**
     * @brief �v���C���[�Z���N�g�V�[���Ŏg�p����I�u�W�F�N�g��NewGO
    */
    void NewGOModeSelectScene();

    ////////////////////////////////////////////////////////////
    // ���[�h�Z���N�g�V�[���̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief ���[�h�Z���N�g�V�[���̏���
    */
    void ModeSelectScene();

    /**
     * @brief �v���C���[�Z���N�g�V�[���Ŏg�p����I�u�W�F�N�g��NewGO
    */
    void NewGOPlayerSelectScene();

    /**
     * @brief �^�C�g���V�[���ɖ߂鏈��
    */
    void ReturnTitleScene();

    ////////////////////////////////////////////////////////////
    // �v���C���[�Z���N�g�V�[���̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief �v���C���[�Z���N�g�V�[���̏���
    */
    void PlayerSelectScene();

    /**
     * @brief �Q�[���V�[���Ŏg�p����I�u�W�F�N�g��NewGO
    */
    void NewGOCPUStrengthSelectScene();

    /**
     * @brief ���[�h�Z���N�g�V�[���ɖ߂鏈��
    */
    void ReturnModeSelectScene();

    ////////////////////////////////////////////////////////////
    // CPU�̓�Փx�I���V�[���̏���
    ////////////////////////////////////////////////////////////

    /**
     * @brief �^�C�g���V�[���̏���
    */
    void CPUStrengthSelectScene();

    /**
     * @brief �v���C���[�Z���N�g�V�[���Ŏg�p����I�u�W�F�N�g��NewGO
    */
    void NewGOGameScene();

    /**
     * @brief �v���C���[�Z���N�g�V�[���ɖ߂鏈��
    */
    void ReturnPlayerSelectScene();

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


public: //Get�֐�
    /**
     * @brief m_StopOperation��Get�֐�
     * @return �v���C���[�̑��쏈�����ł��Ȃ����Ă��邩
    */
    bool GetStopOperation()
    {
        return m_StopOperation;
    }



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


private: //enum
    enum class GameStatus
    {
        title,
        modeSelect,
        playerSelect,
        CPUStrengthSelect,
        loadingGame,
        game,
        GameStatusMax
    };

    enum class LoadingStatus
    {
        doNothing,              //�������Ă��Ȃ�
        preparingForLoading,    //���[�h�̏���
        loading,                //���[�h��
        endOfLoading,           //���[�h�I��
        LoadStatusMax
    };


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
    Player* m_player = nullptr;
    GameCamera* m_gameCamera = nullptr;
    Stage* m_stage = nullptr;
    Rule1* m_rule1 = nullptr;
    Score* m_score = nullptr;
    FontRender* m_fontStartCountdown = nullptr;     //�J�E���g�_�E���̃t�H���g
    SpriteRender* m_spriteBackground = nullptr;
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

    ////////////////////////////////////////////////////////////
    // �t���O�֘A
    ////////////////////////////////////////////////////////////

    GameStatus m_gameStatus = GameStatus::title;
    LoadingStatus m_loadStatus = LoadingStatus::doNothing;
    bool m_startPreparingForLoading = false;
    bool m_startEndOfLoading = false;

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_maxPlayer = 0;                        //���삵�Ă���v���C���[�̐�
};

