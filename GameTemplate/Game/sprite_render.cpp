#include "stdafx.h"
#include "sprite_render.h"

SpriteRender::SpriteRender()
{

}

SpriteRender::~SpriteRender()
{

}

bool SpriteRender::Start()
{
    return true;
}

void SpriteRender::Init(const char* texFilePath, float w, float h, const Vector2& pivot,
	AlphaBlendMode alphaBlendMode)
{
	//�X�v���C�g�̏������p�f�[�^
	SpriteInitData initData;
	initData.m_ddsFilePath[0] = texFilePath;
	initData.m_width = w;
	initData.m_height = h;
	initData.m_fxFilePath = "Assets/shader/sprite.fx";
	initData.m_psEntryPoinFunc = "PSMain";
	initData.m_vsEntryPointFunc = "VSMain";
	initData.m_alphaBlendMode = alphaBlendMode;

	m_pivot = pivot;

	//�X�v���C�g�̏�����
	m_sprite.Init(initData);
}

void SpriteRender::Update()
{
	m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
}

void SpriteRender::Render(RenderContext& renderContext)
{
	m_sprite.Draw(renderContext);
}