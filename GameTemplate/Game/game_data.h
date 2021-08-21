#pragma once
#include "constant.h"

/**
 * @brief �Q�[���V�[���S�̂Ŏg�p����f�[�^�������N���X
*/
class GameData : public IGameObject
{
public:
    GameData();
    ~GameData();
    bool Start() override final;
    void Update() override final;


public: //Get�֐�
    /**
     * @brief �v���C����v���C���[�̍ő吔
     * @return �v���C����v���C���[�̍ő吔
    */
    const int& GetMaxPlayer() const
    {
        return m_maxPlayer;
    }

    /**
     * @brief �I������CPU�̋���
     * @return �I������CPU�̋���
    */
    const con::CPULevel& GetCPULevel() const
    {
        return m_CPULevel;
    }


public: //Set�֐�
    /**
     * @brief �v���C����v���C���[�̍ő吔���Z�b�g
     * @param maxPlayer �v���C����v���C���[�̍ő吔
    */
    void SetMaxPlayer(int maxPlayer)
    {
        m_maxPlayer = maxPlayer;
    }

    /**
     * @brief �I������CPU�̋������Z�b�g
     * @param level �I������CPU�̋���
    */
    void SetCPULevel(const con::CPULevel& level)
    {
        m_CPULevel = level;
    }


private: //data menber
    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_maxPlayer = 0;                    //���삵�Ă���v���C���[�̐�
    con::CPULevel m_CPULevel = con::easy;   //��Փx�󂯎��󂯓n���p
};