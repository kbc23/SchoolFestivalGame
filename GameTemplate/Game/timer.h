#pragma once

class Timer : public IGameObject
{
public:
    Timer();
    ~Timer();
    bool Start() override final;
    void Update() override final;

    
    void CountTimer();

    //���Z
    void CountAdditionTimer();
    //���Z
    void CountSubtractionTimer();


public:
    void SetTime(const int setTime)
    {
        m_time = setTime;
    }

    void SetCheckTime(const int setTime)
    {
        m_checkTime = setTime;
    }


private: //data menber
    int m_time = 0;

    int m_checkTime = 0;

    //true:���Z false:���Z
    bool m_countTimerAdditionOrSubtraction = true;



};