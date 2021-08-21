#pragma once
#include <Model.h>

#include "constant.h"

////////////////////////////////////////////////////////////
// ����
// �g�D�[�������_�����O�Ɏg�p����摜�Ȃǂ̃p�X��ǂݍ���ł���Ƃ���
// NullTextureMaps.cpp
////////////////////////////////////////////////////////////

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
	 * @brief ������
	 * @param filePath tkm�t�@�C���̃t�@�C���p�X
	 * @param modelUpAxis ���f���̏����
	 * @param animationClip �A�j���[�V�����N���b�v
	 * @param maxAnimationClipNum �A�j���[�V�����N���b�v�̍ő吔
	*/
	void Init(const char* filePath,
		modelUpAxis::EnModelUpAxis modelUpAxis = modelUpAxis::enModelUpAxisZ,
		AnimationClip* animationClip = nullptr,
		int maxAnimationClipNum = 0
	);



private:
	/**
	 * @brief ���f���̏�����
	 * @param filePath tkm�t�@�C���̃t�@�C���p�X
	 * @param  ���f���̏����
	*/
	void InitModel(const char* filePath,
		modelUpAxis::EnModelUpAxis = modelUpAxis::enModelUpAxisZ
	);

	/**
	 * @brief �f�B���N�V�������C�g�̏�����
	*/
	void InitDirectionLight();

	/**
	 * @brief �|�C���g���C�g�̏�����
	*/
	void InitPointLight();

	/**
	 * @brief �X�|�b�g���C�g�̏�����
	*/
	void InitSpotLight();

	/**
	 * @brief �����̏�����
	*/
	void InitAmbientLight();

	/**
	 * @brief �������C�g�̏�����
	*/
	void InitHemiLight();

	/**
	 * @brief �X�P���g���̏�����
	 * @param filePath �t�@�C���p�X
	 * @return 
	*/
	bool InitSkeleton(const char* filePath);

	/**
	 * @brief �A�j���[�V�����̏�����
	 * @param animationClip �A�j���[�V�����N���b�v
	 * @param maxAnimationClipNum �A�j���[�V�����N���b�v�̑���
	*/
	void InitAnimation(AnimationClip* animationClip, int maxAnimationClipNum);


	void InitToonShader();




public:
	/**
	 * @brief �A�j���[�V�������Đ�����
	 * @param animationNumber �o�^���Ă���A�j���[�V�����N���b�v�̔ԍ�
	 * @param interpolateTime �⊮���ԁi�P�ʁF�b�j
	*/
	void PlayAnimation(int animationNumber, float interpolateTime = 0.2f)
	{
		m_animationPointer->Play(animationNumber, interpolateTime);
	}

	/**
	 * @brief �A�j���[�V�������Đ�����
	 * @return �Đ�����
	*/
	const bool IsPlayingAnimation() const
	{
		return m_animationPointer->IsPlaying();
	}

	/**
	 * @brief �A�j���[�V�����̏��������������Ă��邩
	 * @return �������Ă��邩
	*/
	const bool IsInited() const
	{
		return m_animationPointer->IsInited();
	}



public: //Get�֐�
	/**
	 * @brief �ʒu���擾
	 * @return �ʒu
	*/
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/**
	 * @brief ��]�ʂ��擾
	 * @return ��]��
	*/
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/**
	 * @brief �g�嗦���擾
	 * @return �g�嗦
	*/
	const Vector3& GetScale() const
	{
		return m_scale;
	}



public: //Set�֐�
	/**
	 * @brief �ʒu��ݒ�
	 * @param vec �ʒu
	*/
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}

	/**
	 * @brief Y���W���w�肵���ʂ����㏸
	 * @param position �㏸�����
	*/
	void UpPositionY(const float position)
	{
		m_position.y += position;
	}

	/**
	 * @brief Y���W���w�肵���ʂ������~
	 * @param position ���~�����
	*/
	void DownPositionY(const float position)
	{
		m_position.y -= position;
	}

	/**
	 * @brief Y���W��0.0f�ɐݒ肷��
	*/
	void ResetPositionY()
	{
		m_position.y = 0.0f;
	}

	/**
	 * @brief ��]�ʂ�ݒ�
	 * @param rotation ��]��
	*/
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	/**
	 * @brief X���̉�]�ʂ�ݒ�
	 * @param rotation X���̉�]��
	*/
	void SetRotationX(const float rotation) {
		m_rotation.SetRotationX(rotation);
	}

	/**
	 * @brief �g�嗦��ݒ�
	 * @param scale �g�嗦
	*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}





private: //data menber
    Model m_model;
	std::unique_ptr<Skeleton> m_skeletonPointer; //�X�P���g���̃|�C���^�[
	std::unique_ptr<Animation> m_animationPointer; //�A�j���[�V�����̃|�C���^�[


    const char* m_tkmFilePath = nullptr; //tkm�t�@�C���̃t�@�C���p�X
	Light m_light;

	Vector3 m_position = g_vec3Zero;			//�ʒu
	Quaternion m_rotation = g_quatIdentity;		//��]
	Vector3 m_scale = g_vec3One;				//�g��

	bool m_finishInit = false; //���������I�������
};