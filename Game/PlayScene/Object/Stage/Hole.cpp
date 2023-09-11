#include "pch.h"
#include "DeviceResources.h"

#include <random>

#include "Hole.h"

Hole::Hole() :
	m_position(DirectX::SimpleMath::Vector3(3.0f, -0.5f, -7.0f)),
	m_moveLength(0.0f),
	m_radius(1.5f),
	m_height(1.0f),
	m_moveVel(5.0f),
	m_isChange(true),
	m_score(1)
{
	//	ïKóvÇ»ïœêîÇÃèÄîı
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	â~íåÇÃê∂ê¨
	m_hole = DirectX::GeometricPrimitive::CreateCylinder(context, m_height, m_radius * 2);
}

void Hole::Initialize()
{
}

void Hole::Update(const DX::StepTimer& timer)
{
	switch (m_modeDir)
	{
	case CSVStage::MOVE_DIR::UP:
		MoveUp(timer);
		break;
	case CSVStage::MOVE_DIR::SIDE:
		MoveSide(timer);
		break;
	case CSVStage::MOVE_DIR::AROUND:
		MoveAround(timer);
		break;
	}
}

void Hole::MoveUp(const DX::StepTimer& timer)
{
	std::mt19937 mt{ std::random_device{}() };

	if (m_isChange)
	{
		m_position.y -= static_cast<float>(timer.GetElapsedSeconds()) * 2;
		m_moveLength -= static_cast<float>(timer.GetElapsedSeconds()) * 2;

		if (m_moveLength <= -m_moveVel)
		{
			m_isChange = false;
		}
	}
	else
	{
		m_position.y += static_cast<float>(timer.GetElapsedSeconds()) * 2;
		m_moveLength += static_cast<float>(timer.GetElapsedSeconds()) * 2;

		if (m_moveLength >= m_moveVel)
		{
			m_isChange = true;
		}
	}
}

void Hole::MoveSide(const DX::StepTimer& timer)
{
	std::mt19937 mt{ std::random_device{}() };

	if (m_isChange)
	{
		m_position.x -= static_cast<float>(timer.GetElapsedSeconds()) * 2;
		m_moveLength -= static_cast<float>(timer.GetElapsedSeconds()) * 2;

		if (m_moveLength <= -m_moveVel)
		{
			m_isChange = false;
		}
	}
	else
	{
		m_position.x += static_cast<float>(timer.GetElapsedSeconds()) * 2;
		m_moveLength += static_cast<float>(timer.GetElapsedSeconds()) * 2;

		if (m_moveLength >= m_moveVel)
		{
			m_isChange = true;
		}
	}
}

void Hole::MoveAround(const DX::StepTimer& timer)
{
	std::mt19937 mt{ std::random_device{}() };

	if (m_isChange)
	{
		m_position.z -= static_cast<float>(timer.GetElapsedSeconds()) * 2;
		m_moveLength -= static_cast<float>(timer.GetElapsedSeconds()) * 2;

		if (m_moveLength <= -m_moveVel)
		{
			m_isChange = false;
		}
	}
	else
	{
		m_position.z += static_cast<float>(timer.GetElapsedSeconds()) * 2;
		m_moveLength += static_cast<float>(timer.GetElapsedSeconds()) * 2;

		if (m_moveLength >= m_moveVel)
		{
			m_isChange = true;
		}
	}
}

void Hole::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	m_hole->Draw(world, view, projection, DirectX::Colors::Black);
}

void Hole::SetState(DirectX::SimpleMath::Vector3 pos, CSVStage::MOVE_DIR modeDir,float moveVel,int score)
{
	m_position = pos;
	//	çÇÇ≥ï‚ê≥
	m_position.y -= 0.5f;
	m_modeDir = modeDir;
	m_moveVel = moveVel;
	m_score = score;
}

bool Hole::Hit(DirectX::SimpleMath::Vector3 bodyPosition)
{
	DirectX::SimpleMath::Vector3 bodyToHole = m_position - bodyPosition;
	//	0.2ÇÃï‚ê≥ñ≥Ç≠ÇƒÇ‡Ç¢Ç¢Ç©Ç‡
	float hitLength = m_radius + 0.2f;

	if (bodyToHole.Length() <= hitLength)
	{
		return true;
	}

	return false;
}
