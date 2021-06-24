#pragma once
#include "sound_source.h"

class SoundBGM : public IGameObject
{
public:
    SoundBGM();
    ~SoundBGM();

    /**
     * @brief ������
     * @param filePath wav�t�@�C���p�X
     * @param is3DSound 3D�Đ����邩
    */
    void Init(const wchar_t* filePath, const bool is3DSound = false);

    /**
     * @brief �Đ�
     * @param isLoop ���[�v�Đ����邩
    */
    void Play(const bool isLoop);

    /**
     * @brief �{�����[����ݒ�i1.0f���ʏ�j
     * ���̒l������Ɛ�Βl�ɂȂ�B
     * @param volume �{�����[��
    */
    void SetVolume(const float volume)
    {
        //�{�����[����ێ�
        m_volume = volume;

        //���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă��Ȃ��Ȃ�
        if (m_loopSoundSource == nullptr) {
            return;
        }

        //���[�v�Đ��p�̃T�E���h�\�[�X�̃{�����[����ݒ肷��
        m_loopSoundSource->SetVolume(m_volume * m_BGMVolume);
    }

    void SetBGMVolume(const float BGMVolume)
    {
        m_BGMVolume = BGMVolume;

        //���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă��Ȃ��Ȃ�
        if (m_loopSoundSource == nullptr) {
            return;
        }

        //���[�v�Đ��p�̃T�E���h�\�[�X�̃{�����[����ݒ肷��
        m_loopSoundSource->SetVolume(m_volume * m_BGMVolume);

    }

    /**
     * @brief �{�����[�����擾
     * @return �{�����[��
    */
    const float GetVolume() const
    {
        return m_volume;
    }

    /**
     * @brief �Đ����~
     * ���ɍĐ�����Ƃ��͍ŏ�����
    */
    void Stop()
    {
        //���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă�����
        if (!m_loopSoundSource) {
            return;
        }

        //�T�E���h���~����
        m_loopSoundSource->Stop();
    }

    /**
     * @brief �Đ����ꎞ��~
     * ���ɍĐ�����Ƃ��͈ꎞ��~�����n�_����
    */
    void Pause()
    {
        //���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă�����
        if (!m_loopSoundSource) {
            return;
        }

        //�T�E���h���ꎞ��~����
        m_loopSoundSource->Pause();
        //�t���O���ꎞ��~���ɂ���
        m_isPaused = true;
    }

    /**
     * @brief �Đ�����
     * @return �Đ�����
    */
    const bool IsPlaying() const
    {
        //���[�v�Đ��p�̃T�E���h�\�[�X���g���Ă�����
        if (!m_loopSoundSource) {
            return false;
        }

        //�Đ������ǂ������ׂ�
        return m_loopSoundSource->IsPlaying();
    }

    /**
     * @brief �ꎞ��~����
     * @return �ꎞ��~����
    */
    const bool IsPaused() const
    {
        return m_isPaused;
    }

    /**
     * @brief �Ȃ̏I���ɂ�����
     * @return �Ȃ̏I���ɂ�����
    */
    const bool EndOfSound() const
    {
        return m_loopSoundSource->GetEndOfSound();
    }

private:	//private�ȃ����o�֐�
    void PlayLoop();		//���[�v�Đ��̏���
    void PlayOneShot();		//�����V���b�g�Đ��̏���


private: //data menber
    wchar_t m_filePath[256];					//wav�t�@�C���p�X
    bool m_is3DSound = false;					//3D�T�E���h�ōĐ����邩
    float m_volume = 1.0f;						//�S�̂̃{�����[��
    bool m_isPaused = false;					//�ꎞ��~����
    SoundSource* m_loopSoundSource = nullptr;	//���[�v�Đ��p�̃T�E���h�\�[�X


private: //static data menber
    static float m_BGMVolume;
};