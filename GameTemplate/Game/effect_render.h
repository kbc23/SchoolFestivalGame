#pragma once

class EffectRender : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	EffectRender();
	~EffectRender();
	bool Start() override final;
	void Update() override final;
	//void UpdateWhenPaused() override final;	//�A�b�v�f�[�g�֐�

public:		//�����o�֐�	
	/**
	 * @brief ������
	 * �G�t�F�N�g�Đ��̂��߂̏���
	 * @param filePath �t�@�C���p�X
	*/
	void Init(const char16_t* filePath);

	/**
	 * @brief �Đ��J�n
	 * �G�t�F�N�g�̍Đ����J�n����
	*/
	void Play();

	/**
	 * @brief ���W��ݒ�
	 * @param pos ���W
	*/
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/**
	 * @brief ���W���擾
	 * @return ���W
	*/
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/**
	 * @brief ��]�N�H�[�^�j�I����ݒ�
	 * @param rot ��]��
	*/
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/**
	 * @brief ��]�N�H�[�^�j�I�����擾
	 * @return ��]��
	*/
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/**
	 * @brief �g�嗦��ݒ�
	 * @param scale �g�嗦
	*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/**
	 * @brief �g�嗦���擾
	 * @return �g�嗦
	*/
	const Vector3& GetScale() const
	{
		return m_scale;
	}

	/**
	 * @brief �G�t�F�N�g���Đ���������
	 * @return �G�t�F�N�g���Đ�����
	*/
	bool IsPlay() const
	{
		return EffectEngine::GetInstance()->IsPlay(m_handle);
	}

	/**
	 * @brief �G�t�F�N�g�̍Đ����~
	*/
	void Stop()
	{
		EffectEngine::GetInstance()->Stop(m_handle);
	}

private: //data menber
	Effekseer::EffectRef m_effect;	//�G�t�F�N�g
	int	m_handle = -1;				//�Đ����̃G�t�F�N�g�̃n���h���B
	Vector3 m_position;				//���W�B
	Quaternion m_rotation;;			//��]�B
	Vector3 m_scale = g_vec3One;	//�g�嗦�B
};