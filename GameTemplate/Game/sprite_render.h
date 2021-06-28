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
	/// �������p�֐�
	/// �ŏ��ɌĂ�łˁB
	/// </summary>
	/// <param name="texFilePath">dds�t�@�C���p�X</param>
	/// <param name="w">�X�v���C�g�̉���</param>
	/// <param name="h">�X�v���C�g�̏c��</param>
	/// <param name="pivot">�s�{�b�g�i��_�j</param>
	/// <param name="alphaBlendMode">�A���t�@�u�����f�B���O���[�h</param>


	/**
	 * @brief ������
	 * @param texFilePath dds�t�@�C���p�X
	 * @param w ����
	 * @param h �c��
	 * @param pivot �s�{�b�g�i��_�j
	 * @param alphaBlendMode �A���t�@�u�����f�B���O���[�h
	*/
	void Init(const char* texFilePath, float w, float h, const Vector2& pivot = { 0.5f,0.5f },
		AlphaBlendMode alphaBlendMode = AlphaBlendMode_None);

	/**
	 * @brief �ꏊ��ݒ�i{0.0f,0.0f}�����S�j
	 * @param pos �ꏊ
	*/
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/**
	 * @brief ��]�ʂ�ݒ�
	 * @param rot ��]��
	*/
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	
	/**
	 * @brief �g�嗦��ݒ�
	 * @param scale �g�嗦
	*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		//���W�n�̊֌W�ō��E���]���邩��
		//x�̊g��𔽓]������
		m_scale.x *= -1.0f;
	}

	/**
	 * @brief �s�{�b�g�i��_�j��ݒ�
	 * {0.0f,0.0f}������
	 * {0.5f,0.5f}������
	 * {1.0f,1.0f}���E��
	 * @param pivot �s�{�b�g�i��_�j
	*/
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
	}

	/**
	 * @brief m_position��Get�֐�
	 * @return 
	*/
	Vector3 GetPosition()
	{
		return m_position;
	}


private: //data menber
	Sprite m_sprite;							//�X�v���C�g�N���X
	SpriteInitData m_initData;					//�X�v���C�g�̏������f�[�^�B�\���́B

	Vector3 m_position = { 0.0f,0.0f,0.0f };	//�ꏊ
	Quaternion m_rotation = g_quatIdentity;		//��]
	//���W�n�̊֌W�ō��E���]���邩��
	//x�̊g��𔽓]������
	Vector3 m_scale = { -1.0f,1.0f,1.0f };		//�g��
	Vector2 m_pivot = { 0.5f,0.5f };			//�s�{�b�g�i��_�j
};