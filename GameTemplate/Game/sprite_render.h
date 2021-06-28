#pragma once

class SpriteRender : public IGameObject
{
public:
    SpriteRender();
    ~SpriteRender();
    bool Start() override final;
    void Update() override final;
    void Render(RenderContext& renderContext) override final;


public:
	/// <summary>
	/// 初期化用関数
	/// 最初に呼んでね。
	/// </summary>
	/// <param name="texFilePath">ddsファイルパス</param>
	/// <param name="w">スプライトの横幅</param>
	/// <param name="h">スプライトの縦幅</param>
	/// <param name="pivot">ピボット（基点）</param>
	/// <param name="alphaBlendMode">アルファブレンディングモード</param>


	/**
	 * @brief 初期化
	 * @param texFilePath ddsファイルパス
	 * @param w 横幅
	 * @param h 縦幅
	 * @param pivot ピボット（基点）
	 * @param alphaBlendMode アルファブレンディングモード
	*/
	void Init(const char* texFilePath, float w, float h, const Vector2& pivot = { 0.5f,0.5f },
		AlphaBlendMode alphaBlendMode = AlphaBlendMode_None);

	/**
	 * @brief 場所を設定（{0.0f,0.0f}が中心）
	 * @param pos 場所
	*/
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/**
	 * @brief 回転量を設定
	 * @param rot 回転量
	*/
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	
	/**
	 * @brief 拡大率を設定
	 * @param scale 拡大率
	*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		//座標系の関係で左右反転するから
		//xの拡大を反転させる
		m_scale.x *= -1.0f;
	}

	/**
	 * @brief ピボット（基点）を設定
	 * {0.0f,0.0f}が左下
	 * {0.5f,0.5f}が中央
	 * {1.0f,1.0f}が右上
	 * @param pivot ピボット（基点）
	*/
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
	}

	/**
	 * @brief m_positionのGet関数
	 * @return 
	*/
	Vector3 GetPosition()
	{
		return m_position;
	}


private: //data menber
	Sprite m_sprite;							//スプライトクラス
	SpriteInitData m_initData;					//スプライトの初期化データ。構造体。

	Vector3 m_position = { 0.0f,0.0f,0.0f };	//場所
	Quaternion m_rotation = g_quatIdentity;		//回転
	//座標系の関係で左右反転するから
	//xの拡大を反転させる
	Vector3 m_scale = { -1.0f,1.0f,1.0f };		//拡大
	Vector2 m_pivot = { 0.5f,0.5f };			//ピボット（基点）
};