#pragma once

class EffectRender : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	EffectRender();
	~EffectRender();
	bool Start() override final;
	void Update() override final;
	//void UpdateWhenPaused() override final;	//アップデート関数

public:		//メンバ関数	
	/**
	 * @brief 初期化
	 * エフェクト再生のための準備
	 * @param filePath ファイルパス
	*/
	void Init(const char16_t* filePath);

	/**
	 * @brief 再生開始
	 * エフェクトの再生を開始する
	*/
	void Play();

	/**
	 * @brief 座標を設定
	 * @param pos 座標
	*/
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/**
	 * @brief 座標を取得
	 * @return 座標
	*/
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/**
	 * @brief 回転クォータニオンを設定
	 * @param rot 回転量
	*/
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/**
	 * @brief 回転クォータニオンを取得
	 * @return 回転量
	*/
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/**
	 * @brief 拡大率を設定
	 * @param scale 拡大率
	*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/**
	 * @brief 拡大率を取得
	 * @return 拡大率
	*/
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/**
	 * @brief エフェクトが再生中か判定
	 * @return エフェクトが再生中か
	*/
	bool IsPlay() const
	{
		return EffectEngine::GetInstance()->IsPlay(m_handle);
	}

	/**
	 * @brief エフェクトの再生を停止
	*/
	void Stop()
	{
		EffectEngine::GetInstance()->Stop(m_handle);
	}

private: //data menber
	Effekseer::EffectRef m_effect;	//エフェクト
	int	m_handle = -1;				//再生中のエフェクトのハンドル。
	Vector3 m_position;				//座標。
	Quaternion m_rotation;;			//回転。
	Vector3 m_scale = g_vec3One;	//拡大率。
};