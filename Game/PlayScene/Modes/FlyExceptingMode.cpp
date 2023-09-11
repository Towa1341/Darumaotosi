#include "pch.h"

#include "FlyExceptingMode.h"
#include "../PlayScene.h"

using namespace DirectX;

FlyExceptingMode::FlyExceptingMode():
	Mode()
{
}

FlyExceptingMode::~FlyExceptingMode()
{
}

void FlyExceptingMode::Initialize()
{
	//	�L�[�{�[�h�g���b�J�[�̐���
	m_keyboardTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();

	m_modeCommonProcess = std::make_unique<ModeCommonProcess>();
	m_modeCommonProcess->Initialize();
}

GAME_MODE FlyExceptingMode::Update(const DX::StepTimer& timer)
{
	//	�L�[�{�[�h�g���b�J�[
	Keyboard::State state = Keyboard::Get().GetState();
	m_keyboardTracker->Update(state);

	return GAME_MODE::NONE;
}

void FlyExceptingMode::Draw()
{
}

void FlyExceptingMode::Finalize()
{
}

bool FlyExceptingMode::GetEndFlag()
{
	return m_endFlag;
}