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

	// ‰Šú‰»
	virtual void Initialize() = 0;

	// XV
	virtual GAME_MODE Update(const DX::StepTimer & timer) = 0;

	// •`‰æ
	virtual void Draw() = 0;

	// I—¹ˆ—
	virtual void Finalize() = 0;

	virtual bool GetEndFlag() = 0;

	virtual Hammer* const GetHammer() = 0;
	virtual Head* const GetHead() = 0;
};