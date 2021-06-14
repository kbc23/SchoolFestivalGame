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
	void Init(const char* filePath);



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





private: //data menber
    Model m_model;
    const char* m_tkmFilePath = nullptr; //tkmファイルのファイルパス
	Light m_light;

	Vector3 m_position = g_vec3Zero;			//位置
	Quaternion m_rotation = g_quatIdentity;		//回転
	Vector3 m_scale = g_vec3One;				//拡大

	bool m_finishInit = false; //初期化が終わったか
};