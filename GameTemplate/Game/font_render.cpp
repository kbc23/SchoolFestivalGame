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
// 初期化
////////////////////////////////////////////////////////////


void FontRender::Init(
	const wchar_t* text,
	const Vector2 position,
	const float scale,
	const Vector4 color,
	const float rotation,
	const Vector2 pivot
)
{
	//それぞれをメンバ変数に保持させる。
	swprintf_s(m_text, text);
	m_position = position;
	m_color = color;
	m_rotation = rotation;
	m_scale = scale;
	m_pivot = pivot;
}

////////////////////////////////////////////////////////////
// 描画処理
////////////////////////////////////////////////////////////


void FontRender::Render(RenderContext& renderContext)
{
	//描画開始
	m_font.Begin(renderContext);

	//描画処理
	m_font.Draw(m_text,
		m_position,
		m_color,
		m_rotation,
		m_scale,
		m_pivot);

	//描画終了
	m_font.End(renderContext);
}