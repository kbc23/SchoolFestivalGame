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
    /**
     * @brief �Q�[���V�[�����I�����������m�F�i���̃��E���h�Ɉڍs���邩���m�F�j
    */
    void CheckGameEnd();

    /**
     * @brief �Q�[���V�[�����I������Ƃ��̏���
    */
    void GameEnd();


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
     * @brief �Q�[�����I�����������Z�b�g
     * @param flag �Q�[�����I��������
    */
    void SetFlagGameEnd(const bool flag)
    {
        m_flagGameEnd = flag;
    }



    void SpriteActivate(const int playerNum, const int rank)
    {
        m_spriteGoalRank[playerNum][rank]->Activate();
    }



private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    //////////////////////////////
    // NewGO
    //////////////////////////////

    SpriteRender* m_spriteGoalRank[con::PlayerNumberMax][con::GoalRankMax] = { nullptr }; //�S�[�����ʂ̉摜
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

    bool m_flagGameEnd = false; //�Q�[�����I��������
};