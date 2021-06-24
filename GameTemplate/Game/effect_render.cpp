#include "stdafx.h"
#include "effect_render.h"

EffectRender::EffectRender()
{
}

EffectRender::~EffectRender()
{

}

bool EffectRender::Start()
{
	return true;
}

void EffectRender::Update()
{
	EffectEngine::GetInstance()->UpdateEffectWorldMatrix(
		m_handle,
		m_position,
		m_rotation,
		m_scale
	);
}

void EffectRender::Init(const char16_t* filePath)
{
	m_effect = EffectEngine::GetInstance()->LoadEffect(filePath);
}

void EffectRender::Play()
{
	//�Đ����̃G�t�F�N�g���~����B
	EffectEngine::GetInstance()->Stop(m_handle);
	//�V�����Đ��B
	m_handle = EffectEngine::GetInstance()->Play(m_effect);
}