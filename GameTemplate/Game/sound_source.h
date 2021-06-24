#pragma once
#include "wav_file.h"
#include "sound_engine.h"

class SoundSource : public IGameObject
{
public:
	SoundSource();
	~SoundSource();

	/**
	 * @brief ������
	 * @param filePath �t�@�C���p�X�B�Ή����Ă���t�@�C���t�H�[�}�b�g(*.wav)
	 * @param is3DSound 3D�T�E���h�H
	*/
	void Init(const wchar_t* filePath, bool is3DSound = false);

	/**
	 * @brief �X�g���[�~���O�Đ������̏�����
	 * @param filePath �t�@�C���p�X�B�Ή����Ă���t�@�C���t�H�[�}�b�g(*.wav)
	 * @param is3DSound 3D�T�E���h�H
	 * @param ringBufferSize �����O�o�b�t�@�̃T�C�Y�B(bufferSize�̔{���ɂȂ��Ă���Ɩ��ʂȂ����p�ł��܂�)
	 * @param bufferingSize �X�g���[�~���O�̍ő�o�b�t�@�����O�T�C�Y
	*/
	void InitStreaming(wchar_t* filePath, bool is3DSound = false, unsigned int ringBufferSize = 3 * 1024 * 1024, unsigned int bufferingSize = 1024 * 512);

	/**
	 * @brief �J��
	*/
	void Release();

	/**
	 * @brief �Đ�
	 * @param isLoop ���[�v�Đ��t���O
	*/
	void Play(bool isLoop);

	/**
	 * @brief ��~
	*/
	void Stop()
	{
		m_sourceVoice->Stop();
		m_isPlaying = false;
	}
	
	/**
	 * @brief �ꎞ��~
	*/
	void Pause()
	{
		m_sourceVoice->Stop(/*XAUDIO2_PLAY_TAILS*/);
	}

	/**
	 * @brief �Đ�����
	 * @return �Đ�����
	*/
	bool IsPlaying() const
	{
		return m_isPlaying;
	}

	/**
	 * @brief �{�����[����ݒ�
	 * @param vol �{�����[��
	*/
	void SetVolume(float vol)
	{
		m_sourceVoice->SetVolume(vol);
	}

	/**
	 * @brief �{�����[�����擾
	 * @return ���݂̃{�����[��
	*/
	float GetVolume() const
	{
		float vol;
		m_sourceVoice->GetVolume(&vol);
		return vol;
	}

	/**
	 * @brief �����̍��W��ݒ�
	 * @details 3D�T�E���h�̎��ɕK�v�ɂȂ�܂��B2D�T�E���h�ł͖�������܂��B
	 * @param pos �����̍��W
	*/
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		if (m_isSetPositionFirst) {
			m_lastFramePosition = m_position;
			m_isSetPositionFirst = false;
		}
	}

	/**
	 * @brief �����̍��W���擾
	 * @return �����̍��W
	*/
	Vector3 GetPosition() const
	{
		return m_position;
	}

	/**
	 * @brief �����̈ړ����x���擾
	 * @return �����̈ړ����x
	*/
	Vector3 GetVelocity() const
	{
		return m_velocity;
	}

	/**
	 * @brief ���[�v�t���O���擾
	 * @return ���[�v�t���O
	*/
	bool GetLoopFlag() const
	{
		return m_isLoop;
	}

	/**
	 * @brief �{�C�X�̎��g��������
	 * @details �ڍׂ�IXAudio2SourceVoice��SetFrequencyRatio���Q�Ƃ��Ă��������B
	 * @param ratio ���g����
	*/
	void SetFrequencyRatio(float ratio)
	{
		if (m_sourceVoice != nullptr) {
			m_sourceVoice->SetFrequencyRatio(ratio);
		}
	}
	IXAudio2SourceVoice* GetXAudio2SourceVoice()
	{
		return m_sourceVoice;
	}
	//���̓`�����l�������擾�B
	int GetNumInputChannel()const
	{
		return m_waveFile->GetFormat()->nChannels;
	}
	FLOAT32* GetEmitterAzimuths()
	{
		return m_emitterAzimuths;
	}
	FLOAT32* GetMatrixCoefficients()
	{
		return m_matrixCoefficients;
	}
	X3DAUDIO_DSP_SETTINGS* GetDspSettings()
	{
		return &m_dspSettings;
	}
private:
	void InitCommon();
	//�X�g���[�~���O�Đ����̍X�V�����B
	void UpdateStreaming();
	//�I���������Đ����̍X�V�����B
	void UpdateOnMemory();

	void Play(char* buff, unsigned int bufferSize);

	/**
	 * @brief �X�g���[�~���O�o�b�t�@�����O�̊J�n
	*/
	void StartStreamingBuffring();

	void Remove3DSound();

	/**
	 * @brief �X�V
	*/
	void Update() override;


public: //Get�֐�
	/**
	 * @brief �Ȃ̏I���ɂ�����
	 * @return �Ȃ̏I���ɂ�����
	*/
	const bool GetEndOfSound() const
	{
		return m_endOfSound;
	}


private:
	enum EnStreamingStatus {
		enStreamingBuffering,	//�o�b�t�@�����O���B
		enStreamingQueueing,	//�L���[�C���O���B
	};
	std::shared_ptr<WavFile>	m_waveFile;					//!<�g�`�f�[�^�B
	IXAudio2SourceVoice* m_sourceVoice = nullptr;	//!<�\�[�X�{�C�X�B
	bool						m_isLoop = false;			//!<���[�v�t���O�B
	bool						m_isPlaying = false;		//!<�Đ����t���O�B
	bool					m_isStreaming = false;		//!<�X�g���[�~���O�Đ��H
	unsigned int			m_streamingBufferSize = 0;	//!<�X�g���[�~���O�p�̃o�b�t�@�����O�T�C�Y�B
	unsigned int			m_currentBufferingSize = 0;	//!<���݂̃o�b�t�@�����O�̃T�C�Y�B
	unsigned int			m_readStartPos = 0;			//!<�ǂݍ��݊J�n�ʒu�B
	unsigned int			m_ringBufferSize = 0;		//!<�����O�o�b�t�@�̃T�C�Y�B
	EnStreamingStatus		m_streamingState = enStreamingBuffering;	//!<�X�g���[�~���O�X�e�[�^�X�B
	bool					m_is3DSound = false;		//!<3D�T�E���h�H
	Vector3				m_position = Vector3::Zero;	//!<�����̍��W�B3D�T�E���h�̎��ɕK�v�B
	Vector3				m_lastFramePosition = Vector3::Zero;//!<������1�t���[���O�̍��W�B3D�T�E���h�̎��ɕK�v�B
	Vector3				m_velocity = Vector3::Zero;	//!<���x�B3D�T�E���h�̎��ɕK�v�E
	FLOAT32 m_emitterAzimuths[INPUTCHANNELS];
	FLOAT32 m_matrixCoefficients[INPUTCHANNELS * OUTPUTCHANNELS];
	X3DAUDIO_DSP_SETTINGS m_dspSettings;
	bool m_isSetPositionFirst = true;	//!<��ԍŏ���setPosition?
	bool m_isAvailable = false;			//!<�C���X�^���X�����p�\�H
	bool m_endOfSound = false; //�Ȃ̏I���ɂ�����
};