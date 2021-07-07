#pragma once

class Stage;
class Player;

class Rule1 : public IGameObject
{
public:
	Rule1();
	~Rule1();
	bool Start();
	void Update() override final;

private:	// data member
	Stage* m_stage = nullptr;
	Player* m_player = nullptr;
	
};

