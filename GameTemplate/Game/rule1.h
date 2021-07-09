#pragma once

class Stage;
class Player;

class Rule1 : public IGameObject
{
public:
	Rule1();
	~Rule1();
	bool Start() override final;
	void Init() override final;
	void Finish() override final;
	void Update() override final;

private:	// data member
	Stage* m_stage = nullptr;
	Player* m_player = nullptr;
	
};

