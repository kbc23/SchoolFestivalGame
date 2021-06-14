#pragma once



//����model_render.h��model_render.cpp�ɑ��݂���}�W�b�N�i���o�[��萔�ȂǂɕύX���Ă������ƁB



//���C�g�p�̍\���̂��`����
struct Light {
	//�f�B���N�V�������C�g
	Vector3 dirDirection; //���C�g�̕���
	//HLSL���̒萔�o�b�t�@��float3�^�̕ϐ���16�̔{���̃A�h���X�ɔz�u����邽�߁A
	//C++���ɂ̓p�f�B���O�𖄂߂Ă����B
	float pad;
	Vector3 dirColor;
	float pad1;

	//�|�C���g���C�g
	Vector3 ptPosition; //�ʒu
	float pad2;			//�p�f�B���O
	Vector3 ptColor;	//�J���[
	float ptRange;		//�e���͈�

	//�X�|�b�g���C�g
	Vector3 spPosition;		//�ʒu
	float pad3;				//�p�f�B���O
	Vector3 spColor;		//�J���[
	float spRange;			//�e���͈�
	Vector3 spDirection;	//�ˏo����
	float spAngle;			//�ˏo�p�x

	//�\���̂Ɏ��_�̈ʒu��ǉ�����
	Vector3 eyePos; //���_�̈ʒu
	float pad4;

	//����
	Vector3 ambientLight;   // �A���r�G���g���C�g
	float pad5;

	//�������C�g
	Vector3 groundColor;	//�n�ʐF
	float pad6;
	Vector3 skyColor;		//�V���F
	float pad7;
	Vector3 groundNormal;	//�n�ʂ̖@��
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
	 * @brief �������֐�
	 * @param filePath tkm�t�@�C���̃t�@�C���p�X
	*/
	void Init(const char* filePath);



private:
	/**
	 * @brief ���f���̏�����
	 * @param filePath tkm�t�@�C���̃t�@�C���p�X
	*/
	void InitModel(const char* filePath);

	void InitDirectionLight(); //�f�B���N�V�������C�g
	void InitPointLight(); //�|�C���g���C�g
	void InitSpotLight(); //�X�|�b�g���C�g
	void InitAmbientLight(); //����
	void InitHemiLight(); //�������C�g



public: //Set�֐�

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


public: //Get�֐�

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
    const char* m_tkmFilePath = nullptr; //tkm�t�@�C���̃t�@�C���p�X
	Light m_light;

	Vector3 m_position = g_vec3Zero;			//�ʒu
	Quaternion m_rotation = g_quatIdentity;		//��]
	Vector3 m_scale = g_vec3One;				//�g��

	bool m_finishInit = false; //���������I�������
};