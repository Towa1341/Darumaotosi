/*
InooutManager.h

�V���O���g���N���X
*/

#include <Keyboard.h>
#include <Mouse.h>

class InputManager final
{
public:
	// �C���X�^���X��Ԃ�
	static InputManager* GetInstance()
	{
		static InputManager instance;
		return &instance;
	}

//	�ϐ�
public:
	//	�L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardStateTracker;
	DirectX::Keyboard::State m_keyboardState;

	//	�}�E�X
	std::unique_ptr<DirectX::Mouse> m_mouse;
	DirectX::Mouse::State m_mouseState;

//	�֐�
private:
	//	�R���X�g���N�^�A�f�X�g���N�^��private
	InputManager();
	~InputManager() = default;

	// �R�s�[�R���X�g���N�^�A���[�u�R���X�g���N�^�A�֌W���������Z�q�̍폜
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