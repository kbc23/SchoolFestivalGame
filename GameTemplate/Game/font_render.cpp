#include "stdafx.h"
#include "font_render.h"

FontRender::FontRender()
{

}

FontRender::~FontRender()
{

}

bool FontRender::Start()
{


	return true;
}

void FontRender::Update()
{

}

////////////////////////////////////////////////////////////
// ������
////////////////////////////////////////////////////////////


void FontRender::Init(
	const wchar_t* text,
	const Vector2 position,
	const Vector4 color,
	const float rotation,
	const float scale,
	const Vector2 pivot
)
{
	//���ꂼ��������o�ϐ��ɕێ�������B
	swprintf_s(m_text, text);
	m_position = position;
	m_color = color;
	m_rotation = rotation;
	m_scale = scale;
	m_pivot = pivot;
}

////////////////////////////////////////////////////////////
// �`�揈��
////////////////////////////////////////////////////////////


void FontRender::Render(RenderContext& renderContext)
{
	//�`��J�n
	m_font.Begin(renderContext);

	//�`�揈��
	m_font.Draw(m_text,
		m_position,
		m_color,
		m_rotation,
		m_scale,
		m_pivot);

	//�`��I��
	m_font.End(renderContext);
}