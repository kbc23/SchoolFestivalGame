#pragma once
#include "model_render.h"

class Stage;

class Player : public IGameObject
{
public:
	Player();

	~Player();
	void DeleteIndividual(const int pNum); //デストラクタの処理をプレイヤーごとに個別でおこなう。

	bool Start();
	bool StartIndividual(const int pNum); //Start関数の処理をプレイヤーごとに個別でおこなう。

	void Update();

	
	void Draw();
	void DrawIndividual(const int pNum); //Draw関数の処理をプレイヤーごとに個別でおこなう。

	////////////////////////////////////////////////////////////
    // プレイヤーの操作処理
    ////////////////////////////////////////////////////////////

	/// <summary>
	/// プレイヤーの操作処理の土台
	/// </summary>
	void Controller();
	
	/// <summary>
	/// プレイヤーの操作処理
	/// </summary>
	/// <param name="pNum">プレイヤー番号</param>
	void ControllerIndividual(const int pNum);

public: //Set関数
	//Set関数の引数にある[int i]はプレイヤー番号である。

	//////////////////////////////
	// m_positionのSet関数
	//////////////////////////////

	void SetPosition(int i, const Vector3 v) {
		//m_position[i] = v;
	}


	
	void SetActivePlayer(int i, bool b) {
		m_activePlayer[i] = b;
	}







public: //Get関数
	//Get関数の引数にある[int i]はプレイヤー番号である。

	//////////////////////////////
	// m_positionのGet関数
	//////////////////////////////

	Vector3 GetPosition(int i) {
		//return m_position[i];
	}



	float GetActivePlayer(int i) {
		return m_activePlayer[i];
	}



public: //enum
	//プレイヤーの番号
	enum PlayerNumber
	{
		player_1,
		player_2,
		player_3,
		player_4,
		PlayerNumberMax
	};



private: //メンバ変数
	Stage* m_stage = nullptr;

	//プレイヤーが最大４人だから、メンバ変数は要素数４の配列で管理する。
	//ModelRender* m_modelRender = nullptr;
	ModelRender* m_modelRender[PlayerNumberMax] = { nullptr }; //スキンモデルレンダラー。

	Vector3 m_position[PlayerNumberMax]; //キャラクターの座標
	Quaternion m_rotation[PlayerNumberMax] = { Quaternion::Identity }; //キャラクターの回転情報

	bool m_activePlayer[PlayerNumberMax] = { true, true, true, true }; //プレイヤーが操作できるかどうか
	//※今はテストのためm_activePlayer[player_1]だけtrue

	int m_maxPlayer = PlayerNumberMax; //プレイヤーの最大数
	//※プレイヤー数選択画面を作成したら、そこからこの変数にプレイヤーの最大数を代入する。

};