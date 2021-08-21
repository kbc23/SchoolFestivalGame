#pragma once
#include "sprite_render.h"

class Goal : public IGameObject
{
public:
    Goal();
    ~Goal();
    bool Start() override final;
    void Init();
    void Finish();
    void Update() override final;


private:
    /**
     * @brief �Q�[�����I�������Ƃ��̏���
    */
    void GameEnd();


public: //Get�֐�
    /**
     * @brief �Q�[�����I�����������擾
     * @return �Q�[�����I��������
    */
    const bool GetFlagGameEnd()
    {
        return m_flagGameEnd;
    }



public: //Set�֐�
    /**
     * @brief �Q�[�����I�����������Z�b�g
     * @param flag �Q�[�����I��������
    */
    void SetFlagGameEnd(const bool flag)
    {
        m_flagGameEnd = flag;
    }



private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    SpriteRender* m_spriteGameEnd = nullptr; //�Q�[���I�����̉摜

    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    bool m_flagGameEnd = false; //�Q�[�����I��������
};