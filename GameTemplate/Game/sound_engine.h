#pragma once
#include "wav_file_bank.h"
#include "wav_file.h"

#define INPUTCHANNELS 2  // number of source channels
#define OUTPUTCHANNELS 8	//�ő�o�̓`�����l�����B

class SoundSource;

class SoundEngine
{
public:
	/*!
	 * @brief �R���X�g���N�^
	 */
	SoundEngine();

	/*!
	 * @brief �f�X�g���N�^
	 */
	~SoundEngine();

	/*!
	 * @brief ������
	 */
	void Init();

	/*!
	 * @brief �J��
	 */
	void Release();

	/*!
	* @brief �X�V
	*/
	void Update();
	
	/**
	 * @brief 3D�T�E���h�\�[�X��ǉ�
	 * @param source �ǉ�����3D�T�E���h�\�[�X
	*/
	void Add3DSoundSource(SoundSource* source)
	{
		m_3dSoundSource.push_back(source);
	}

	/**
	 * @brief 3D�T�E���h�\�[�X���폜
	 * @param source �폜����3D�T�E���h�\�[�X
	*/
	void Remove3DSoundSource(SoundSource* source)
	{
		auto it = std::find(m_3dSoundSource.begin(), m_3dSoundSource.end(), source);
		if (it != m_3dSoundSource.end()) {
			m_3dSoundSource.erase(it);
		}
	}

	/**
	 * @brief �T�E���h���X�i�[�̍��W��ݒ�
	 * @details 3D�������g���Ă��鎞�̌v�Z�Ɏg�p
	 * @param pos �T�E���h���X�i�[�̍��W
	*/
	void SetListenerPosition(const Vector3& pos)
	{
		m_listenerPosition = pos;
	}

	/**
	 * @brief �T�E���h���X�i�[�̑O������ݒ�
	 * @details 3D�������g���Ă��鎞�̌v�Z�Ɏg�p
	 * @param front �T�E���h���X�i�[�̑O����
	*/
	void SetListenerFront(const Vector3& front)
	{
		m_listener.OrientFront.x = front.x;
		m_listener.OrientFront.y = front.y;
		m_listener.OrientFront.z = front.z;
	}

	/**
	 * @brief �T�E���h���X�i�[�̏������ݒ�
	 * 	 * @details 3D�������g���Ă��鎞�̌v�Z�Ɏg�p
	 * @param up �T�E���h���X�i�[�̏����
	*/
	void SetListenerUp(const Vector3& up)
	{
		m_listener.OrientTop.x = up.x;
		m_listener.OrientTop.y = up.y;
		m_listener.OrientTop.z = up.z;
	}
	/*!
	* @brief	XAudio2�̃\�[�X�{�C�X���쐬�B
	*/
	IXAudio2SourceVoice* CreateXAudio2SourceVoice(WavFile* waveFile, bool is3DSound);
	IXAudio2MasteringVoice* GetMasteringVoice()
	{
		return m_masteringVoice;
	}
	IXAudio2SubmixVoice* GetSubmixVoice()
	{
		return m_submixVoice;
	}
	/*!
	* @brief	�o�̓`�����l���̐����擾�B
	*/
	const int GetNumChannel() const
	{
		return m_nChannels;
	}
	/*!
	* @brief	�g�`�f�[�^�o���N���擾�B
	*/
	WavFileBank& GetWaveFileBank()
	{
		return m_waveFileBank;
	}
	/// <summary>
	/// �T�E���h�G���W�������p�\���ǂ�������B
	/// </summary>
	/// <returns>true���Ԃ��Ă����痘�p�\�ł��B</returns>
	const bool IsAvailable() const
	{
		return m_isInited;
	}


private:	//static�ȃf�[�^�����o
		static SoundEngine* m_instance;


public:		//static�ȃ����o�֐�
		/// <summary>
	/// �V���O���g���p�^�[��
	/// �B��̃C���X�^���X�����֐�
	/// �ŏ��ɌĂ�łˁI
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new SoundEngine;
	}

	/// <summary>
	/// �C���X�^���X�������֐�
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}

	/// <summary>
	/// �C���X�^���X�̎Q�Ƃ�߂��֐�
	/// </summary>
	/// <returns>�C���X�^���X�̎Q��</returns>
	static SoundEngine* GetInstance()
	{
		return m_instance;
	}


private:
	IXAudio2* m_xAudio2 = nullptr;
	IXAudio2MasteringVoice* m_masteringVoice = nullptr;
	X3DAUDIO_HANDLE m_hx3DAudio;					//!<3D�I�[�f�B�I�̃n���h���B
	IUnknown* m_reverbEffect = nullptr;				//!<���o�[�v�G�t�F�N�g�B
	IXAudio2SubmixVoice* m_submixVoice = nullptr;	//!<�T�u�~�b�N�X�{�C�X�B
	DWORD m_channelMask = 0;						//!<�I�[�f�B�I�`�����l���}�X�N�B
	DWORD m_nChannels = 0;							//!<�`�����l�����B
	Vector3 m_listenerPosition = Vector3::Zero;	//!<���X�i�[�̍��W�B
	float m_fListenerAngle = 0.0f;
	bool m_UseListenerCone = false;
	bool m_fUseInnerRadius = false;
	bool m_useRedirectToLFE = false;
	bool m_isInited = false;
	X3DAUDIO_LISTENER m_listener;					//!<�T�E���h���X�i�[�B
	std::list<SoundSource*>	m_3dSoundSource;	//!<3D�T�E���h�̃T�E���h�\�[�X�B
	X3DAUDIO_CONE	m_emitterCone;
	WavFileBank m_waveFileBank;					//!<�g�`�f�[�^�̃o���N�B
};