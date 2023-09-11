#include "pch.h"
#include "DeviceResources.h"

#include "Body.h"

const float Body::GROUND = 0.5f;
const float Body::GRAVITY = 0.08f;

Body::Body(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, int weight, eBodyType bodyType, DirectX::SimpleMath::Vector2 position) :
	m_moveFlag(false),
	m_deleteFlag(false),
	m_pos(position.x, POP_POS, position.y),
	m_texture(texture),
	m_weight(weight),
	m_endMoveFlag(false),
	m_bodyType(bodyType),
	m_weitTime(0.0f),
	m_time(0.1f),
	m_radius(1.5f),
	m_isHit(false)
{
	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	円柱の生成
	m_cylinder = DirectX::GeometricPrimitive::CreateCylinder(context, 1.0f, m_radius * 2);
}

Body::~Body()
{
}

void Body::Update(float yPos, const DX::StepTimer& timer)
{
	//	横に移動する
	if (m_moveFlag)
	{
		bool z = false;
		bool y = false;

		//	横移動  ここをrotに合わせて移動する(x,z)sin.cos velは掛け算する

		float radian = DirectX::XMConvertToRadians(m_rot);

		//	サインコサインで位置を出す
		//	velを掛け算して移動する
		m_pos.z += sin(radian) * m_vel;
		m_pos.x += cos(radian) * m_vel;

		//	重力
		m_pos.y += m_jump;
		m_jump -= GRAVITY;

		//	地面に着いたら
		if (m_pos.y <= GROUND)
		{
			//	止める
			m_vel = 0.0f;
			z = true;

			m_pos.y = GROUND;
			y = true;

			m_jump = 0.0f;
		}

		if (z && y)
		{
			//	ちょっと待って消す
			if (m_time >= 0.0f)
			{
				m_time -= static_cast<float>(timer.GetElapsedSeconds());
			}
			else
			{
				m_deleteFlag = true;
			} 
		}
	}
	else
	{
		//	落ちる
		m_pos.y -= 0.1f;
		if (m_pos.y <= yPos - GROUND)
		{
			m_endMoveFlag = true;
			m_pos.y = yPos - GROUND;
		}
		else
		{
			m_endMoveFlag = false;
		}
	}
}

void Body::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection ,
	DirectX::BasicEffect* basicEffect,Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout)
{
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);

	basicEffect->SetWorld(world);

	basicEffect->SetTexture(m_texture.Get());

	m_cylinder->Draw(basicEffect, inputLayout.Get());
}

void Body::Initialize()
{
}

void Body::Finalize()
{
}
