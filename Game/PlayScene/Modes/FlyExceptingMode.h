#pragma once

#include "Mode.h"

#include "ModeCommons/ModeCommonProcess.h"

class FlyExceptingMode :public Mode
{
private:
	//	キーボードトラッカー
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;

	std::unique_ptr<ModeCommonProcess> m_modeCommonProcess;

	bool m_endFlag;

public:
	FlyExceptingMode();
	~FlyExceptingMode();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_MODE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	bool GetEndFlag()override;
};