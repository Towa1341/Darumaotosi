#pragma once

#include "StepTimer.h"
#include "../PlayScene.h"

#include "../Object/Head.h"
#include "../Object/BodyManager.h"
#include "../Object/Hammer.h"

class IMode
{	
private:
	
public:

	virtual ~IMode() = default;

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual GAME_MODE Update(const DX::StepTimer & timer) = 0;

	// 描画
	virtual void Draw() = 0;

	// 終了処理
	virtual void Finalize() = 0;

	virtual bool GetEndFlag() = 0;

	virtual Hammer* const GetHammer() = 0;
	virtual Head* const GetHead() = 0;
};