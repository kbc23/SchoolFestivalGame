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

private: //data menber
	bool pauseActive = false;	//ポーズが有効か?

	int keytouch = 0;			//ポーズ入力を受け付けるか

};