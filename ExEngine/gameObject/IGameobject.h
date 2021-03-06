/*!
 *@brief	ゲームオブジェクトの基底クラス。
 */

#pragma once

#include <list>
#include <string>
class RenderContext;

/*!
	*@brief	ゲームオブジェクト。
	*/
class IGameObject {
public:
	/*!
		*@brief	デストラクタ
		*/
	virtual ~IGameObject()
	{
	}
public:


	/*!
	*@brief	Updateの直前で呼ばれる開始処理。
	*@details
	*
	* 本関数がtrueを返すとゲームオブジェクトの準備が完了したと判断されて</br>
	* Update関数が呼ばれ出します。trueを返して以降はStart関数は呼ばれなくなります。</br>
	* ゲームオブジェクトの初期化に複数フレームかかる場合などはfalseを返して、初期化ステップなどを使って</br>
	* 適切に初期化を行ってください。
	*/
	virtual bool Start() { return true; }
	/*!
		*@brief	更新
		*/
	virtual void Update()
	{

	}
	/*!
	 *@brief	描画
	*/
	virtual void Render(RenderContext& renderContext)
	{
		(void)renderContext;
	}
	/// <summary>
	/// ポーズ中でも呼ばれるUpdate（更新）
	/// </summary>
	virtual void AlwaysUpdate()
	{

	};
	/// <summary>
	/// ポーズ中にだけ呼ばれるUpdate（更新）
	/// </summary>
	virtual void UpdateOnlyPaused()
	{

	};

public:
	/*!
	*@brief Start関数が完了した？
	*/
	bool IsStart() const
	{
		return m_isStart;
	}
	/*!
	*@brief アクティブかどうかを判定。
	*/
	bool IsActive() const
	{
		return m_isActive;
	}
	/// <summary>
	/// ゲームオブジェクトをアクティブにする。
	/// </summary>
	void Activate()
	{
		m_isActive = true;
	}
	/// <summary>
	/// ゲームオブジェクトを非アクティブにする。
	/// </summary>
	void Deactivate()
	{
		m_isActive = false;
	}

	/// <summary>
	/// 死亡させる。
	/// </summary>
	void Dead()
	{
		m_isDead = true;
	}
	/// <summary>
	/// 死亡している？
	/// </summary>
	/// <returns>trueが返ってきたら死亡している</returns>
	bool IsDead() const
	{
		return m_isDead;
	}
	/// <summary>
	/// ゲームオブジェクトの名前を設定。
	/// </summary>
	/// <param name="name">名前</param>
	void SetName(const char* name)
	{
		if (name != nullptr) {
			m_name = name;
		}
	}
	/// <summary>
	/// 引数で渡された名前が、このゲームオブジェクトの名前とマッチするか判定。
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	bool IsMatchName(const char* name) const
	{
		if (strcmp(m_name.c_str(), name) == 0) {
			return true;
		}
		return false;
	}

	static void SetGamePaused(const bool paused)
	{
		m_gamePaused = paused;
	}
	static const bool GetGamePaused()
	{
		return m_gamePaused;
	}


public:

	void RenderWrapper(RenderContext& renderContext)
	{
		if (m_isActive && m_isStart && !m_isDead) {
			Render(renderContext);
		}
	}

	void UpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			Update();
		}
	}
	void StartWrapper()
	{
		if (m_isActive && !m_isStart && !m_isDead) {
			if (Start()) {
				//初期化処理完了。
				m_isStart = true;
			}
		}
	}
	void AlwaysUpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			AlwaysUpdate();
		}
	}
	void UpdateOnlyPausedWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead) {
			UpdateOnlyPaused();
		}
	}


	friend class CGameObjectManager;


private: //static
	static bool m_gamePaused;				//ゲームがポーズ中。Updateが呼ばれない。



protected:
	std::string m_name;								//ゲームオブジェクトの名前
	bool m_isStart = false;							//Startの開始フラグ。
	bool m_isDead = false;							//死亡フラグ。
	bool m_isRegistDeadList = false;				//死亡リストに積まれている。
	bool m_isNewFromGameObjectManager;	//GameObjectManagerでnewされた。
	bool m_isRegist = false;							//GameObjectManagerに登録されている？
	bool m_isActive = true;							//Activeフラグ。

	bool m_flagProcess = false;		//処理をおこなうかどうか
};



////////////////////////////////////////////////////////////
// IGameObjectクラスを継承したクラス関連の定数
////////////////////////////////////////////////////////////

namespace igo //constant
{
	////////////////////////////////////////////////////////////
	// クラスのオブジェクトにつけた名前
	////////////////////////////////////////////////////////////

	//constexpr const char*型で宣言すること
	//しなかった場合は、コンパイルエラーが発生する。

	constexpr const char* CLASS_NAME_MAIN_PROCESSING = "mainProcessing";
	constexpr const char* CLASS_NAME_PLAYER = "player";
	constexpr const char* CLASS_NAME_GAME = "game";
	constexpr const char* CLASS_NAME_STAGE = "stage";
	constexpr const char* CLASS_NAME_SUDDEN_DEATH = "suddenDeath";
	constexpr const char* CLASS_NAME_CPU_PLAYER_CONTROLLER = "CPUPlayerController";
	constexpr const char* CLASS_NAME_SCORE = "score";
	constexpr const char* CLASS_NAME_PAUSE = "pause";
	constexpr const char* CLASS_NAME_GAME_DATA = "gameData";
	constexpr const char* CLASS_NAME_GAME_START_COUNTDOWN = "gameStartCountdown";
	constexpr const char* CLASS_NAME_GOAL = "goal";
	constexpr const char* CLASS_NAME_RANK = "rank";
	constexpr const char* CLASS_NAME_RESULT = "result";

	////////////////////////////////////////////////////////////
	// 優先度
	////////////////////////////////////////////////////////////

	const int PRIORITY_CLASS = 0;
	const int PRIORITY_BACKGROUND = 0;
	const int PRIORITY_MODEL = 1;
	const int PRIORITY_UI = 2;
	const int PRIORITY_UI_2 = 3;
	const int PRIORITY_FONT = 4;
	const int PRIORITY_FADE = 5;
}