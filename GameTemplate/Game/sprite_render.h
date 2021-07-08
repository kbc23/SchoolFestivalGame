#pragma once



namespace srName //constant
{
	const Vector4 COLOR_NORMAL = { 1.0f,1.0f,1.0f,1.0f };
	const Vector4 COLOR_GRAY = { 0.5f,0.5f,0.5f,1.0f };
}



class SpriteRender : public IGameObject
{
public:
    SpriteRender();
    ~SpriteRender();
    bool Start() override final;
    void Update() override final;
    void Render(RenderContext& renderContext) override final;


public:
	/**
	 * @brief ������
	 * @param texFilePath dds�t�@�C���p�X
	 * @param w ����
	 * @param h �c��
	 * �c���Ɖ����́A���ׂē����T�C�Y�̉摜�ō���Ă��邽�߁A�f�t�H���g�����ɂ��Ă���B
	 * @param alphaBlendMode �A���t�@�u�����f�B���O���[�h
	 * ��{�I�ɓ��߉摜���g�p����̂�AlphaBlendMode_Trans���f�t�H���g�����ɂ��Ă���B
	 * @param pivot �s�{�b�g�i��_�j
	*/
	void Init(const char* texFilePath, float w = 1280.0f, float h = 720.0f,
		AlphaBlendMode alphaBlendMode = AlphaBlendMode_Trans, const Vector2& pivot = { 0.5f,0.5f });

	/**
	 * @brief �ꏊ��ݒ�i{0.0f,0.0f}�����S�j
	 * @param pos �ꏊ
	*/
	void SetPosition(const Vector2& pos)
	{
		m_position.x = pos.x;
		m_position.y = pos.y;
	}

	void SetPositionX(const float& f)
	{
		m_position.x = f;
	}

	void SetPositionY(const float& f)
	{
		m_position.y = f;
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
		//m_scale.x *= -1.0f;
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
	 * @brief �X�v���C�g�ɐF����Z����֐�
	 * �A���t�@�l���|���ē��߂���Ƃ��́AInit����Ƃ���
	 * �A���t�@�u�����f�B���O���[�h�𓧉ߍ������[�h�ɂ��Ă�������
	 * @param mulColor ��Z����F
	 * mulColor.x = R�J���[
	 * mulColor.y = G�J���[
	 * mulColor.z = B�J���[
	 * mulColor.w = ����
	*/
	void SetMulColor(const Vector4& mulColor)
	{
		m_sprite.SetMulColor(mulColor);
	}

	void SetMulColorW(const float& mulColorW)
	{
		m_sprite.SetMulColorW(mulColorW);
	}

	/**
	 * @brief m_position��Get�֐�
	 * @return 
	*/
	const Vector3& GetPosition()
	{
		return m_position;
	}

	const float& GetMulColorW()
	{
		return m_sprite.GetMulColorW();
	}


private: //data menber
	Sprite m_sprite;							//�X�v���C�g�N���X
	SpriteInitData m_initData;					//�X�v���C�g�̏������f�[�^�B�\���́B

	Vector3 m_position = { 0.0f,0.0f,0.0f };	//�ꏊ
	Quaternion m_rotation = g_quatIdentity;		//��]
	//���W�n�̊֌W�ō��E���]���邩��
	//x�̊g��𔽓]������
	Vector3 m_scale = { 1.0f,1.0f,1.0f };		//�g��
	Vector2 m_pivot = { 0.5f,0.5f };			//�s�{�b�g�i��_�j
};