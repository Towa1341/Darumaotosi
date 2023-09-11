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

	//	�ݒ�
	m_modeCommonProcess->Setting(
		//m_pBodyManager,	//	�̃}�l�[�W��
		m_pHammer,		//	�n���}�[
		m_pHead			//	��
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
