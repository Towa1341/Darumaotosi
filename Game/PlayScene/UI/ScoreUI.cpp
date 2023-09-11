//
//	ScoreUI.cpp
//
#include "pch.h"
#include "DeviceResources.h"

#include "ScoreUI.h"
#include "../../TextureManager.h"

#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

ScoreUI::ScoreUI() :
	m_inScoreCheck(false),
	m_scale(10.0f),
	m_inScorePosition(0.0f, -0.9f, -12.0f),
	m_inScore(0)
{
	//	•K—v‚È•Ï”‚Ì€”õ
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	TextureManager* textureManager = TextureManager::GetInstance();
	m_texture = textureManager->GetTexture(TextureManager::eTextureName::SCORE_NUMBER);

	m_numberUIOnce = std::make_unique<MoveNumberUI>();
	m_numberUIOnce->SetYRot(270.0f);
	m_numberUIOnce->SetXRot(-90.0f);

	m_numberUITence = std::make_unique<MoveNumberUI>();
	m_numberUITence->SetYRot(270.0f);
	m_numberUITence->SetXRot(-90.0f);

	m_numberUIInScore = std::make_unique<MoveNumberUI>();
	m_numberUIInScore->SetYRot(270.0f);
	m_numberUIInScore->SetXRot(-90.0f);
}

void ScoreUI::Initialize()
{
}

void ScoreUI::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	m_numberUIOnce->Render(view,projection);
	m_numberUITence->Render(view, projection);

	if (m_inScoreCheck)
	{
		m_numberUIInScore->Render(view, projection);
	}
}

void ScoreUI::Update(const DX::StepTimer& timer, int score)
{
	int once = score % 10;
	int tence = score / 10;

	SimpleMath::Vector3 position = SimpleMath::Vector3(-1.5f, 0.0f, 2.0f);

	m_numberUIOnce->SetRenderState(position + SimpleMath::Vector3(3.0f, 0.0f, 0.0f), SimpleMath::Vector3(5, 5, 5), m_texture);
	m_numberUIOnce->SetNumber(once);

	m_numberUITence->SetRenderState(position, SimpleMath::Vector3(5, 5, 5), m_texture);
	m_numberUITence->SetNumber(tence);

	if (m_inScoreCheck)
	{
		//	¬‚³‚­‚µ‚Ä‰º‚ÉˆÚ“®‚³‚¹‚Ä‚¢‚­
		m_scale -= 0.2f;
		m_inScorePosition.z += 1.0f;

		//	‡‚í‚³‚é‚Æ‚±‚ë‚Åfalse‚É‚·‚é
		if (m_inScorePosition.z >= 0.0f)
		{
			m_inScoreCheck = false;
		}
	}
	else
	{
		// ‚Å‚©‚­^‚ñ’†‚ÉŒÅ’è‚Ä‚¨‚­
		m_scale = 10.0f;
		m_inScorePosition = SimpleMath::Vector3(0.0f, -0.9f, -12.0f);
	}

	m_numberUIInScore->SetRenderState(m_inScorePosition, SimpleMath::Vector3(m_scale, m_scale, m_scale), m_texture);
	m_numberUIInScore->SetNumber(m_inScore);
}

void ScoreUI::Finalize()
{
}
