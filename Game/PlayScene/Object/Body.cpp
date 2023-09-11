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
	//	�K�v�ȕϐ��̏���
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�~���̐���
	m_cylinder = DirectX::GeometricPrimitive::CreateCylinder(context, 1.0f, m_radius * 2);
}

Body::~Body()
{
}

void Body::Update(float yPos, const DX::StepTimer& timer)
{
	//	���Ɉړ�����
	if (m_moveFlag)
	{
		bool z = false;
		bool y = false;

		//	���ړ�  ������rot�ɍ��킹�Ĉړ�����(x,z)sin.cos vel�͊|���Z����

		float radian = DirectX::XMConvertToRadians(m_rot);

		//	�T�C���R�T�C���ňʒu���o��
		//	vel���|���Z���Ĉړ�����
		m_pos.z += sin(radian) * m_vel;
		m_pos.x += cos(radian) * m_vel;

		//	�d��
		m_pos.y += m_jump;
		m_jump -= GRAVITY;

		//	�n�ʂɒ�������
		if (m_pos.y <= GROUND)
		{
			//	�~�߂�
			m_vel = 0.0f;
			z = true;

			m_pos.y = GROUND;
			y = true;

			m_jump = 0.0f;
		}

		if (z && y)
		{
			//	������Ƒ҂��ď���
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
		//	������
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
