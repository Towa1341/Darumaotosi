/*
InooutManager.h

シングルトンクラス
*/

#include <Keyboard.h>
#include <Mouse.h>

class InputManager final
{
public:
	// インスタンスを返す
	static InputManager* GetInstance()
	{
		static InputManager instance;
		return &instance;
	}

//	変数
public:
	//	キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;
	DirectX::Keyboard::State m_keyboardState;

	//	マウス
	std::unique_ptr<DirectX::Mouse> m_mouse;
	DirectX::Mouse::State m_mouseState;

//	関数
private:
	//	コンストラクタ、デストラクタはprivate
	InputManager();
	~InputManager() = default;

	// コピーコンストラクタ、ムーブコンストラクタ、関係する代入演算子の削除
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager(const InputManager&&) = delete;
	InputManager&& operator=(const InputManager&&) = delete;

public:
	void Initialize();
	void Update();

	//	Geter
	DirectX::Keyboard::State& GetKeyboardState() { return m_keyboardState; }
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardTracker() const { return m_keyboardStateTracker.get(); }

	DirectX::Mouse::State& GetMouseState() { return m_mouseState; }
};