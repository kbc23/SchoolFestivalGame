#pragma once
#include "Noncopyable.h"
#include "wav_file.h"


typedef std::shared_ptr<WavFile>	CWaveFilePtr;
typedef std::map<unsigned int, CWaveFilePtr>	CWaveFilePtrMap;

class WavFileBank : Noncopyable
{
public:
	WavFileBank();
	~WavFileBank();
	/*!
	 *@brief	�g�`�f�[�^��o�^�B
	 *@param[in]	groupID		�O���[�v�h�c�B�w��ł���h�c�̍ő�l��MAX_GROUP-1�B
	 *@param[in]	waveFile	�g�`�f�[�^�B
	 */
	void RegistWaveFile(int groupID, CWaveFilePtr waveFile);
	/*!
	*@brief	�����Ŏw�肵���t�@�C���p�X�̔g�`�f�[�^���o���N�ɓo�^����Ă��邩��������B
	*@param[in]	groupID		�O���[�v�h�c�B�w��ł���h�c�̍ő�l��MAX_GROUP-1�B
	*@param[in]	filePath	�t�@�C���p�X�B
	*@return �g�`�f�[�^�B�o�^����Ă��Ȃ��ꍇ��NULL���Ԃ�B
	*/
	CWaveFilePtr FindWaveFile(int groupID, const wchar_t* filePath);
	/*!
	*@brief	�����Ŏw�肵���t�@�C���p�X�̔g�`�f�[�^���o���N�ɓo�^����Ă��邩��������B
	*@details
	*  �������̂ق����������������B
	*@param[in]	groupID		�O���[�v�h�c�B�w��ł���h�c�̍ő�l��MAX_GROUP-1�B
	*@param[in]	nameyKey	���O�L�[�B�B
	*@return �g�`�f�[�^�B�o�^����Ă��Ȃ��ꍇ��NULL���Ԃ�B
	*/
	//CWaveFilePtr FindWaveFile(int groupID, const WNameKey& nameKey);
	/*!
	*@brief	�g�`�f�[�^���o���N����o�^�����B
	*@param[in]	groupID		�O���[�v�h�c�B�w��ł���h�c�̍ő�l��MAX_GROUP-1�B
	*@param[in]	waveFile	�g�`�f�[�^�B
	*/
	void UnregistWaveFile(int groupID, CWaveFilePtr waveFile);
	/*!
	*@brief	�O���[�v�P�ʂŉ���B
	*/
	void Release(int groupID);
	/*!
	*@brief	�S�ĉ���B
	*/
	void ReleaseAll()
	{
		for (int i = 0; i < MAX_GROUP; i++) {
			Release(i);
		}
	}
private:
	static const int MAX_GROUP = 256;
	CWaveFilePtrMap	m_waveFileMap[MAX_GROUP];		//!<wave�t�@�C���̃��X�g�B
};