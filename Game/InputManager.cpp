/*
InooutManager.cpp
*/

#include "pch.h"

#include "InputManager.h"

InputManager::InputManager()
{
}

void InputManager::Initialize()
{	
	// �L�[�{�[�h
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_keyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//	�}�E�X
	m_mouse = std::make_unique<DirectX::Mouse>();
}

void InputManager::Update()
{
	// �L�[�{�[�h�̏�ԍX�V
	m_keyboardState = m_keyboard->GetState();
	// �L�[�{�[�h�g���b�J�[�̍X�V
	m_keyboardStateTracker->Update(m_keyboardState);

	//	�}�E�X�̏�ԍX�V
	m_mouseState = m_mouse->GetState();
}
