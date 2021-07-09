#pragma once

class Player;
class Stage;
class Score;
class EnemyAI;
class Rule1;
class Pause;

class Game : public IGameObject
{
public:
    Game();
    ~Game();
    bool Start() override final;
    void Init() override final;
    void Finish() override final;
    void Update() override final;



public: //Get�֐�



public: //Set�֐�
    void SetRuleSuddenDeath(const bool& b)
    {
        m_ruleSuddenDeath = b;
    }





private: //data menber
    ////////////////////////////////////////////////////////////
    // �N���X�̃I�u�W�F�N�g
    ////////////////////////////////////////////////////////////

    Player* m_player = nullptr;
    Stage* m_stage = nullptr;
    Score* m_score = nullptr;
    EnemyAI* m_enemyAI = nullptr;
    Rule1* m_rule1 = nullptr;
    Pause* m_pause = nullptr;


    ////////////////////////////////////////////////////////////
    // �t���O
    ////////////////////////////////////////////////////////////

    bool m_ruleSuddenDeath = false;


    ////////////////////////////////////////////////////////////
    // ���̑�
    ////////////////////////////////////////////////////////////

    int m_maxPlayer = 0;                        //���삵�Ă���v���C���[�̐�
    int m_dilevel = 0;//��Փx�󂯎��󂯓n���p

public:
    void SetMaxPlayer(const int& i)
    {
        m_maxPlayer = i;
    }

    void SetDilevel(const int& i)
    {
        m_dilevel = i;
    }
};