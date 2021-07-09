#pragma once

class Pause : public IGameObject
{
public:
    Pause();
    ~Pause();
    bool Start() override final;
    void Update() override final;

	/**
	 * @brief ポーズしているか関係なしに呼ばれるアップデート関数
	*/
	void AlwaysUpdate()override final;

	/**
	 * @brief ポーズ中にだけ呼ばれるアップデート関数
	*/
	void UpdateOnlyPaused()override final;

public:
	//Get関数

	bool GetPauseFlag()
	{
		return pauseFlag;
	}

public:
	//Set関数
	
	void SetPauseFlag(const bool b)
	{
		pauseFlag = b;
	}

private: //data menber
	bool pauseActive = false;	//ポーズが有効か?
	bool pauseFlag = false;		//ポーズ中か?

	int keytouch = 0;			//ポーズ入力を受け付けるか

};