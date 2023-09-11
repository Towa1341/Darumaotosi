#include "pch.h"

#include "Mode.h"

Mode::Mode()
{
	m_modeCommonProcess = std::make_unique<ModeCommonProcess>();
	m_modeCommonProcess->Initialize();

	m_pHammer = new Hammer();
	m_pHammer->Initialize();

	m_pHead = new Head();
	m_pHead->Initialize();

	//	設定
	m_modeCommonProcess->Setting(
		//m_pBodyManager,	//	体マネージャ
		m_pHammer,		//	ハンマー
		m_pHead			//	頭
	);
}

Mode::~Mode()
{
	if (m_pHammer != nullptr)
	{
		delete m_pHammer;
		m_pHammer = nullptr;
	}

	if (m_pHead != nullptr)
	{
		delete m_pHead;
		m_pHead = nullptr;
	}
}

void Mode::Initialize()
{
}
