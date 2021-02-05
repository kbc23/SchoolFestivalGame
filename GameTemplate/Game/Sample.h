#pragma once
#include "../../ExEngine/level2D/Level2D.h"
class Sample : public IGameObject
{
	bool Start();
	void Update();
	void Render(RenderContext& renderContext)
	{
		m_sprite[0].Draw(renderContext);
		//m_sprite[1].Draw(renderContext);
		//m_sprite[2].Draw(renderContext);
		m_level.Draw(renderContext);
	} 
private:
	Vector3 m_position[3];
	Vector3 m_scale[3];
	Sprite m_sprite[3];
	Level2D m_level;

};

