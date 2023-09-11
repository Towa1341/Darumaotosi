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

	// ������
	virtual void Initialize() = 0;

	// �X�V
	virtual GAME_MODE Update(const DX::StepTimer & timer) = 0;

	// �`��
	virtual void Draw() = 0;

	// �I������
	virtual void Finalize() = 0;

	virtual bool GetEndFlag() = 0;

	virtual Hammer* const GetHammer() = 0;
	virtual Head* const GetHead() = 0;
};