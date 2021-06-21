#pragma once



//このmodel_render.hとmodel_render.cppに存在するマジックナンバーを定数などに変更しておくこと。



//ライト用の構造体を定義する
struct Light {
	//ディレクションライト
	Vector3 dirDirection; //ライトの方向
	//HLSL側の定数バッファのfloat3型の変数は16の倍数のアドレスに配置されるため、
	//C++側にはパディングを埋めておく。
	float pad;
	Vector3 dirColor;
	float pad1;

	//ポイントライト
	Vector3 ptPosition; //位置
	float pad2;			//パディング
	Vector3 ptColor;	//カラー
	float ptRange;		//影響範囲

	//スポットライト
	Vector3 spPosition;		//位置
	float pad3;				//パディング
	Vector3 spColor;		//カラー
	float spRange;			//影響範囲
	Vector3 spDirection;	//射出方向
	float spAngle;			//射出角度

	//構造体に視点の位置を追加する
	Vector3 eyePos; //視点の位置
	float pad4;

	//環境光
	Vector3 ambientLight;   // アンビエントライト
	float pad5;

	//半球ライト
	Vector3 groundColor;	//地面色
	float pad6;
	Vector3 skyColor;		//天球色
	float pad7;
	Vector3 groundNormal;	//地面の法線
};



class ModelRender : public IGameObject
{
public:
    ModelRender();
    ~ModelRender();
	bool Start() override final;
    void Update() override final;
	void Render(RenderContext& renderContext) override final;



public:
	/**
	 * @brief 初期化関数
	 * @param filePath tkmファイルのファイルパス
	*/
	//void Init(const char* filePath);


	void Init(const char* filePath,
		AnimationClip* animationClip = nullptr,
		int maxAnimationClipNum = 0
	);



private:
	/**
	 * @brief モデルの初期化
	 * @param filePath tkmファイルのファイルパス
	*/
	void InitModel(const char* filePath);

	void InitDirectionLight(); //ディレクションライト
	void InitPointLight(); //ポイントライト
	void InitSpotLight(); //スポットライト
	void InitAmbientLight(); //環境光
	void InitHemiLight(); //半球ライト


	bool InitSkeleton(const char* filePath);

	/**
	 * @brief アニメーションの初期化
	 * @param animationClip アニメーションクリップ
	 * @param maxAnimationClipNum アニメーションクリップの総数
	*/
	void InitAnimation(AnimationClip* animationClip, int maxAnimationClipNum);


public:
	/**
	 * @brief アニメーションを再生する
	 * @param animationNumber 登録しているアニメーションクリップの番号
	 * @param interpolateTime 補完時間（単位：秒）
	*/
	void PlayAnimation(int animationNumber, float interpolateTime = 0.2f)
	{
		m_animationPointer->Play(animationNumber, interpolateTime);
	}

	/**
	 * @brief アニメーションを再生中か
	 * @return 再生中
	*/
	const bool IsPlayingAnimation() const
	{
		return m_animationPointer->IsPlaying();
	}

	bool IsInited() const
	{
		return m_animationPointer->IsInited();
	}



public: //Get関数

	const Vector3 GetPosition()
	{
		return m_position;
	}

	const Quaternion GetRotation()
	{
		return m_rotation;
	}

	const Vector3 GetScale()
	{
		return m_scale;
	}



public: //Set関数

	void SetPosition(const Vector3 v)
	{
		m_position = v;
	}

	void UpPositionY(const float f)
	{
		m_position.y += f;
	}

	void DownPositionY(const float f)
	{
		m_position.y -= f;
	}

	void SetRotation(const Quaternion q)
	{
		m_rotation = q;
	}

	void SetRotationX(const float f) {
		m_rotation.SetRotationX(f);
	}

	void SetScale(const Vector3 v)
	{
		m_scale = v;
	}





private: //data menber
    Model m_model;
	std::unique_ptr<Skeleton> m_skeletonPointer; //スケルトンのポインター
	std::unique_ptr<Animation> m_animationPointer; //アニメーションのポインター


    const char* m_tkmFilePath = nullptr; //tkmファイルのファイルパス
	Light m_light;

	Vector3 m_position = g_vec3Zero;			//位置
	Quaternion m_rotation = g_quatIdentity;		//回転
	Vector3 m_scale = g_vec3One;				//拡大

	bool m_finishInit = false; //初期化が終わったか
};