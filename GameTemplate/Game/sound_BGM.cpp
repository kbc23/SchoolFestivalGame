#include "stdafx.h"
#include "sound_BGM.h"

float SoundBGM::m_BGMVolume = 1.0f;

SoundBGM::SoundBGM()
{

}

SoundBGM::~SoundBGM()
{
	//CSoundCue�I�u�W�F�N�g��DeleteGO�����ہA�Đ����������ꍇ�A��~������B
	if (m_loopSoundSource) {
		DeleteGO(m_loopSoundSource);
	}
}

void SoundBGM::Init(const wchar_t* filePath, const bool is3DSound)
{
	swprintf_s(m_filePath, filePath);

	//��x�A����Ĕj�����邱�ƂŁA
	//Flyweight�p�^�[�����g�p���Ă���WaveFileBank��
	//�o�^������B
	SoundSource* ss = NewGO<SoundSource>(0);
	ss->Init(m_filePath, m_is3DSound);
	DeleteGO(ss);
}

void SoundBGM::Play(const bool isLoop)
{
	if (isLoop)
	{
		//���[�v�Đ�
		PlayLoop();
	}
	else
	{
		//�����V���b�g�Đ�
		PlayOneShot();
	}
}

void SoundBGM::PlayLoop()
{
	//���[�v�Đ��p�̃T�E���h�\�[�X��
//���łɍ���Ă�����
	if (m_loopSoundSource)
	{
		//�ꎞ��~�������̂��ĊJ����
		m_loopSoundSource->Play(true);
		//�t���O���ꎞ��~�������������
		m_isPaused = false;
	}
	else
	{
		//���[�v�Đ��p�̃T�E���h�\�[�X��
		//�܂����g�p�̏ꍇ��
		//�V��������čĐ�����B
		m_loopSoundSource = NewGO<SoundSource>(0);
		m_loopSoundSource->Init(m_filePath, m_is3DSound);
		m_loopSoundSource->Play(true);
		SetVolume(m_volume);
	}
}

void SoundBGM::PlayOneShot()
{
	//����A�T�E���h�\�[�X������čĐ�������
	//Flyweight�p�^�[�����g�p���Ă��邩��d���Ȃ�Ȃ��B
	SoundSource* ss = NewGO<SoundSource>(0);
	ss->Init(m_filePath, m_is3DSound);
	ss->SetVolume(m_volume * m_BGMVolume);	//OS�̏ꍇ�͂����Ń{�����[����ݒ肷��
	ss->Play(false);
}