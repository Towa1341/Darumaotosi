//
//	UIManager.cpp
//
#include "pch.h"

#include "UIManager.h"

UIManager::UIManager() :
	m_state(Hammer::eHammerState::None),
	m_changeBodyNum(0),
	m_texture(nullptr)
{
}

UIManager::~UIManager()
{
}

void UIManager::Initialize()
{
	m_hammerUI = std::make_unique<HammerUI>();
	m_hammerUI->Initialize();

	m_bodyUI = std::make_unique<BodyUI>();
	m_bodyUI->Initialize();

	m_informationUI = std::make_unique<InformationUI>();
	m_informationUI->Initialize();

	m_moveNumberUI = std::make_unique<MoveNumberUI>();
	m_moveNumberUI->Initialize();
}

void UIManager::Update()
{
}

void UIManager::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	m_bodyUI->SetTexture(m_texture.Get());
	m_bodyUI->SetNowChangeBodyNum(m_changeBodyNum);
	m_bodyUI->Render(view, projection);

	m_hammerUI->Render(view, projection, m_state);

	m_informationUI->Render(view, projection);
}

void UIManager::Finalize()
{
}
