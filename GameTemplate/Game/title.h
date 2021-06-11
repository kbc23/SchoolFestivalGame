#pragma once

class Title : public IGameObject
{
public:
	Title();
	~Title();
	bool Start() override final;
	void Update() override final;
	void Draw();

private: //data menber


};