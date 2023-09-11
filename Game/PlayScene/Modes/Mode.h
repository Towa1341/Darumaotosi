#pragma once

#include "IMode.h"

#include "DeviceResources.h"

#include "ModeCommons/ModeCommonProcess.h"

class Mode :public IMode
{
private:

	std::unique_ptr<ModeCommonProcess> m_modeCommonProcess;

	//	ÉnÉìÉ}Å[
	Hammer* m_pHammer;
	//	ì™
	Head* m_pHead;

public:

	Mode();
	~Mode();

	virtual void Initialize();

	Hammer* const GetHammer() { return m_pHammer; }
	Head* const GetHead() { return m_pHead; }

	ModeCommonProcess* const GetModeCommonProcess() { return m_modeCommonProcess.get(); }
};