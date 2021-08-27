#pragma once
#include "constant.h"
#include "sprite_render.h"

class SuddenDeathMode;
class Result;
class MainProcessing;

class Rank : public IGameObject
{
public:
    Rank();
    ~Rank();
    bool Start() override final;
    void Init();
    void Finish();
    void Update() override final;


private:
    void DrawRoundPoint();
    /**
     * @brief �Q�[���V�[�����I�����������m�F�i���̃��E���h�Ɉڍs���邩���m�F�j
    */
    void CheckGameEnd();


    void CheckNextRound();

    /**
     * @brief �Q�[���V�[�����I������Ƃ��̏���
    */
    void GameEnd();

    /**
     * @brief �T�h���f�X���[�h���̍ŏI���ʂ��m��
    */
    void SuddenDeathRank();


public:
    /**
     * @brief ���̃��E���h�Ɉڍs���鏈��
    */
    void NextRound();


public: //Get�֐�
    /**
     * @brief �v���C���[�̏��ʂ��m�肵�Ă��邩���m�F
     * @param playerNum �v���C���[�ԍ�
     * @return �v���C���[�̏��ʂ��m�肵�Ă��邩
    */
    const bool GetDoingGoal(const int playerNum) const
    {
        //�v���C���[�̏��ʂ����m��
        if (con::rank_notClear == m_goalRanking[playerNum]) {
            return false;
        }
        //�v���C���[�̏��ʂ��m��
        else {
            return true;
        }
    }

    /**
     * @brief �v���C���[�̃S�[�����ʂ��擾
     * @param playerNum �v���C���[�ԍ�
     * @return �v���C���[�̃S�[������
    */
    const int GetGoalRanking(const int playerNum) const
    {
        return m_goalRanking[playerNum];
    }

    const bool GetFlagNextRound() const
    {
        return m_flagNextRound;
    }


public: //Set�֐�
    /**
     * @brief ���ʂ�ۑ�
     * @param playerNum �v���C���[�ԍ�
     * @param rank ����
    */
    void SetGoalRanking(const int playerNum, const int rank)
    {
        m_goalRanking[playerNum] = rank; //���ʂ��Z�b�g
        m_spriteGoalRank[playerNum][rank]->Activate(); //�\��

        //�v���C���[�S���̏��ʂ��m�肵�������m�F
        for (int playerNum2 = con::FIRST_ELEMENT_ARRAY; playerNum2 < con::PlayerNumberMax; playerNum2++) {
            //���ʂ��܂��m�肵�Ă��Ȃ�
            if (con::rank_notClear == m_goalRanking[playerNum2]) {
                return;
            }
        }

        //�v���C���[�S���̏��ʂ��m�肵�Ă���
        m_flagGameEnd = true; //�Q�[�����I����������ɂ���
    }

    /**
     * @brief �擾���E���h���𑝉�����B�R���E���h�擾�����ꍇ�A�T�h���f�X���[�h���I������B
     * @param playerNum �v���C���[�ԍ�
    */
    void AddRoundPoint(const int playerNum)
    {
        ++m_roundPoint[playerNum];
        m_flagNextRound = true; //���̃��E���h�Ɉڍs����

        //�R���E���h�擾�����Ƃ�
        if (m_roundPoint[playerNum] == 3) {
            m_flagGameEnd = true; //�Q�[�����I����������ɂ���

            //�T�h���f�X���[�h�I��
            //m_finishSuddenDeath = true;
        }
    }

    /**
     * @brief �Q�[�����I�����������Z�b�g
     * @param flag �Q�[�����I��������
    */
    void SetFlagGameEnd(const bool flag)
    {
        m_flagGameEnd = flag;
    }

private: //constant
    static const int m_MAX_RAUND_WIN = 3;



private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    SpriteRender* m_spriteGoalRank[con::PlayerNumberMax][con::GoalRankMax] = { nullptr }; //�S�[�����ʂ̉摜
    SpriteRender* m_spriteRoundWin[con::PlayerNumberMax][m_MAX_RAUND_WIN] = { nullptr };
    SpriteRender* m_spriteGameEnd = nullptr; //�Q�[���I�����̉摜

    //////////////////////////////
    // FindGO
    //////////////////////////////

    SuddenDeathMode* m_findSuddenDeathMode = nullptr;
    Result* m_findResult = nullptr;
    MainProcessing* m_findMainProcessing = nullptr;

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_goalRanking[con::PlayerNumberMax] =
    { con::rank_notClear, con::rank_notClear, con::rank_notClear, con::rank_notClear };	//�v���C���[�̃S�[������

    int m_roundPoint[con::PlayerNumberMax] = { 0,0,0,0 }; //�v���C���[�̃��E���h�̃|�C���g

    bool m_flagGameEnd = false; //�Q�[�����I��������
    bool m_flagNextRound = false; //���̃��E���h�Ɉڍs���邩

    int m_timerGameEnd = con::TIME_RESET_ZERO; //�Q�[���I�����̃^�C�}�[
    int m_timerNextRound = con::TIME_RESET_ZERO; //���̃��E���h�Ɉڍs����^�C�}�[
};