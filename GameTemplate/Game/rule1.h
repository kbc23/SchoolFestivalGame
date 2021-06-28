#pragma once

class Stage;

class Rule1 : public IGameObject
{
public:
	Rule1();
	~Rule1();
	bool Start();

private:	// data member
	Stage* m_stage = nullptr;

};

