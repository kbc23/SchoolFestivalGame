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



public: //Get関数



public: //Set関数
    void SetRuleSuddenDeath(const bool& b)
    {
        m_ruleSuddenDeath = b;
    }





private: //data menber
    ////////////////////////////////////////////////////////////
    // クラスのオブジェクト
    ////////////////////////////////////////////////////////////

    Player* m_player = nullptr;
    Stage* m_stage = nullptr;
    Score* m_score = nullptr;
    EnemyAI* m_enemyAI = nullptr;
    Rule1* m_rule1 = nullptr;
    Pause* m_pause = nullptr;


    ////////////////////////////////////////////////////////////
    // フラグ
    ////////////////////////////////////////////////////////////

    bool m_ruleSuddenDeath = false;


    ////////////////////////////////////////////////////////////
    // その他
    ////////////////////////////////////////////////////////////

    int m_maxPlayer = 0;                        //操作しているプレイヤーの数
    int m_dilevel = 0;//難易度受け取り受け渡し用

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