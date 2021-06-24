#pragma once
#include "util.h"

class WavFile
{
public:
    WavFile();
    ~WavFile();

	/**
	 * @brief wav�t�@�C�����J��
	 * @param filePath �t�@�C����
	 * @return wav�t�@�C�����J������
	*/
	bool Open(const wchar_t* filePath);

	/**
	 * @brief �g�`�f�[�^��ǂݍ���
	 * @param pBuffer �g�`�f�[�^���������ރo�b�t�@
	 * @param sizeToRead �ǂݍ��ރf�[�^�T�C�Y
	 * @param currentReadSize 
	*/
	void Read(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);

	/**
	 * @brief �g�`�f�[�^��񓯊��ǂݍ��݁B
	 * @param pBuffer �g�`�f�[�^���������ރo�b�t�@�B
	 * @param sizeToRead �ǂݍ��ރf�[�^�T�C�Y�B
	 * @param currentReadSize 
	*/
	void ReadAsync(char* pBuffer, unsigned int sizeToRead, unsigned int* currentReadSize);


	/**
	 * @brief �g�`�f�[�^�̃V�[�N�ʒu�����Z�b�g
	*/
	void ResetFile();

	/**
	 * @brief �����[�X
	*/
	void Release();


public: //Get�֐�

	/**
	 * @brief �ǂݍ��ݏI������
	 * @return 
	*/
	bool IsReadEnd() const
	{
		return m_isReadEnd;
	}

	/**
	 * @brief �t�@�C���T�C�Y���擾
	 * @return 
	*/
	DWORD   GetSize() const
	{
		return m_dwSize;
	}

	/**
	 * @brief �t�@�C���t�H�[�}�b�g���擾
	 * @return 
	*/
	WAVEFORMATEX* GetFormat() const
	{
		return m_pwfx;
	};

	/**
	 * @brief �t�@�C���p�X�̃n�b�V���l���擾
	 * @return 
	*/
	unsigned int GetFilePathHash() const
	{
		return m_filePathHash;
	}

	/**
	 * @brief �ǂݍ��ݐ�̃o�b�t�@���m��
	 * @param size 
	*/
	void AllocReadBuffer(int size)
	{
		m_readBuffer.reset(new char[size]);
	}

	/**
	 * @brief �ǂݍ��ݐ�̃o�b�t�@���擾
	 * @return 
	*/
	char* GetReadBuffer()
	{
		return m_readBuffer.get();
	}

private:
	std::unique_ptr<char[]>	m_readBuffer = nullptr;	//�ǂݍ��ݐ�̃o�b�t�@�B
	HMMIO			m_hmmio = nullptr;	//Wave�t�@�C���n���h���B
	WAVEFORMATEX* m_pwfx = nullptr;  //wave�t�@�C���̃t�H�[�}�b�g��`�B
	MMCKINFO 		m_ckRiff;      // Use in opening a WAVE file
	DWORD			m_dwSize = 0;      // The size of the wave file
	MMCKINFO		m_ck;          // Multimedia RIFF chunk
	BYTE* m_pbData = nullptr;
	BYTE* m_pbDataCur = nullptr;
	ULONG			m_ulDataSize = 0;
	volatile bool	m_isReadEnd = true;	//�ǂݍ��ݏI���t���O�B
	std::thread		m_readAsyncThread;	//�񓯊��ǂݍ��݃X���b�h�B
	bool			m_isInvokeReadAsyncThread = false;
	std::wstring	m_filePath;			//�t�@�C���p�X�B
	unsigned int	m_filePathHash = 0;		//�t�@�C���p�X�̃n�b�V���R�[�h�B

};