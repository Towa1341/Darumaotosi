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
	// キーボード
	m_keyboard = std::make_unique<DirectX::Keyboard>();
	m_keyboardStateTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	//	マウス
	m_mouse = std::make_unique<DirectX::Mouse>();
}

void InputManager::Update()
{
	// キーボードの状態更新
	m_keyboardState = m_keyboard->GetState();
	// キーボードトラッカーの更新
	m_keyboardStateTracker->Update(m_keyboardState);

	//	マウスの状態更新
	m_mouseState = m_mouse->GetState();
}
