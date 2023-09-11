#pragma once

#include "Mode.h"

#include "ModeCommons/ModeCommonProcess.h"

class FlyExceptingMode :public Mode
{
private:
	//	�L�[�{�[�h�g���b�J�[
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;

	std::unique_ptr<ModeCommonProcess> m_modeCommonProcess;

	bool m_endFlag;

public:
	FlyExceptingMode();
	~FlyExceptingMode();

	// ������
	void Initialize() override;

	// �X�V
	GAME_MODE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	bool GetEndFlag()override;
};