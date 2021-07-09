#pragma once

class Pause : public IGameObject
{
public:
    Pause();
    ~Pause();
    bool Start() override final;
    void Update() override final;

	/**
	 * @brief �|�[�Y���Ă��邩�֌W�Ȃ��ɌĂ΂��A�b�v�f�[�g�֐�
	*/
	void AlwaysUpdate()override final;

	/**
	 * @brief �|�[�Y���ɂ����Ă΂��A�b�v�f�[�g�֐�
	*/
	void UpdateOnlyPaused()override final;

public:
	//Get�֐�

	bool GetPauseFlag()
	{
		return pauseFlag;
	}

public:
	//Set�֐�
	
	void SetPauseFlag(const bool b)
	{
		pauseFlag = b;
	}

private: //data menber
	bool pauseActive = false;	//�|�[�Y���L����?
	bool pauseFlag = false;		//�|�[�Y����?

	int keytouch = 0;			//�|�[�Y���͂��󂯕t���邩

};