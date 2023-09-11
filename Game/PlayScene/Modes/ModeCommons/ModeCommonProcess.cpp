#include "pch.h"

#include "ModeCommonProcess.h"

#include "../../../InputManager.h"

#include "Libraries/MyLibraries/DebugNumRender.h"

using namespace DirectX;

ModeCommonProcess::ModeCommonProcess():
	m_scopePos(DirectX::SimpleMath::Vector2::Zero),
	m_hitPowerMin(0.0f),
	m_hitPowerMax(0.0f),
	m_power(0.001f) ,
	m_nowBodyNumber(0)
{
}

ModeCommonProcess::~ModeCommonProcess()
{
	
}

void ModeCommonProcess::Initialize()
{

}

void ModeCommonProcess::Finalize()
{

}

void ModeCommonProcess::Setting(Hammer* const hammer, Head* const head)
{
	m_hammer = hammer;
	m_head = head;
}

void ModeCommonProcess::SetBodyManager(BodyManager* const bodyManager)
{
	m_bodyManager = bodyManager;

	m_targetWeight = m_bodyManager->SetTargetWeight();
}

void ModeCommonProcess::FallBodys(const DX::StepTimer& timer)
{
	if (timer.GetFrameCount() % 100 == 0 && !m_bodyManager->GetHeadPop())
	{
		m_bodyManager->AddBody();
	}
}

void ModeCommonProcess::MovePower()
{
	if (InputManager::GetInstance()->GetKeyboardState().Enter)
	{
		m_power += 0.03f;

		if (m_power >= 0.9f)
		{
			m_power = 0.001f;
		}
	}
}

void ModeCommonProcess::SetGaugeState()
{
	//	体が生成されていたら
	if (m_bodyManager->GetBodysPop())
	{
		Body::eBodyType bodyType = m_bodyManager->GetSelectBody()->GetBodyType();

		switch (bodyType)
		{
		case Body::eBodyType::WOOD:
			m_scopePos = DirectX::SimpleMath::Vector2(40.0f, 480.0f);
			m_hitPowerMax = 0.4f;
			m_hitPowerMin = 0.1f;
			break;
		case Body::eBodyType::BRICK:
			m_scopePos = DirectX::SimpleMath::Vector2(40.0f, 340.0f);
			m_hitPowerMax = 0.7f;
			m_hitPowerMin = 0.4f;
			break;
		case Body::eBodyType::METAL:
			m_scopePos = DirectX::SimpleMath::Vector2(40.0f, 200.0f);
			m_hitPowerMax = 1.0f;
			m_hitPowerMin = 0.7f;
			break;
		case Body::eBodyType::HEAD:
			m_scopePos = DirectX::SimpleMath::Vector2(40.0f, 0.0f);
			break;
		}
	}
}

bool ModeCommonProcess::CheckGauge()
{
	return m_hitPowerMax >= m_power && m_hitPowerMin <= m_power;
}

void ModeCommonProcess::ChangeBodySelect()
{
	InputManager* inputManager = InputManager::GetInstance();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputManager->GetKeyboardTracker();

	//	体が生成されていたら
	if (m_bodyManager->GetBodysPop())
	{
		if (m_nowBodyNumber > m_bodyManager->GetBodysSize() - 1)
		{
			m_nowBodyNumber = m_bodyManager->GetBodysSize() - 1;
		}
		//	飛ばす体を変更する
		else if (keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::Up) && m_nowBodyNumber < m_bodyManager->GetBodysSize() - 1)
		{
			m_nowBodyNumber++;
		}
		else if (keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::Down) && m_nowBodyNumber >= 1)
		{
			m_nowBodyNumber--;
		}

		m_bodyManager->SetBodyNum(m_nowBodyNumber);

		//	ハンマーのpositionとrotを設定
		m_hammer->SetYPos(m_nowBodyNumber + 0.5f);
		m_hammer->SetRot(m_power * 40);
	}
}

void ModeCommonProcess::ResetPower()
{
	m_power = 0.0f;
	m_hammer->SetRot(0.0f);
}

void ModeCommonProcess::HeadPop()
{
	//	場所設定
	m_head->SetMaxPos(m_bodyManager->GetBodysSize() + 1);

	//	顔面決める
	Head::eHeadState headState = Head::eHeadState::NONE;
	if (m_bodyManager->GetGrossWeight() == m_targetWeight)
	{
		headState = Head::eHeadState::PALA;
	}
	else if (m_bodyManager->GetGrossWeight() >= m_targetWeight - 5 &&
		m_bodyManager->GetGrossWeight() <= m_targetWeight + 5)
	{
		headState = Head::eHeadState::NUNN;
	}
	else
	{
		headState = Head::eHeadState::SYOBONN;
	}

	//	セットする
	m_head->SetState(headState);

	//	更新
	m_head->Update();
}

void ModeCommonProcess::RenderGauge(DirectX::SpriteBatch* spriteBatch)
{
	TextureManager* textureManaegr = TextureManager::GetInstance();

	//	ゲージ
	spriteBatch->Draw(textureManaegr->GetTexture(TextureManager::eTextureName::GAUGE_BASE).Get(), DirectX::SimpleMath::Vector2(40.0f, 200.0f), nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	//	伸縮するゲージ
	float pos = m_power;
	spriteBatch->Draw(textureManaegr->GetTexture(TextureManager::eTextureName::GAUGE).Get(), DirectX::SimpleMath::Vector2(70.0f, 200.0f + 464.0f), nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 464.0f), DirectX::SimpleMath::Vector2(0.5f, pos));

	//	ゲージの範囲
	spriteBatch->Draw(textureManaegr->GetTexture(TextureManager::eTextureName::GAUAGE_SCOPE).Get(), m_scopePos, nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f), DirectX::SimpleMath::Vector2(0.5f, 1.0f));

}