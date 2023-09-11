//
//	Stage.cpp
//
#include "pch.h"
#include "DeviceResources.h"

#include "Stage.h"

#include <WICTextureLoader.h>

using namespace DirectX::SimpleMath;

//	�R���X�g���N�^
Stage::Stage()
{
	//	�K�v�ȕϐ��̏���
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	Box�̐���
	m_box = DirectX::GeometricPrimitive::CreateBox(context, Vector3(20.0f, 0.0f, 30.0f));
	m_box2 = DirectX::GeometricPrimitive::CreateBox(context, Vector3(30.0f, 0.0f, 20.0f));

	//	�e�N�X�`���̐���
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/tatami.jpg", nullptr, m_texture.ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/husuma.jpg", nullptr, m_texture2.ReleaseAndGetAddressOf());
}

//	������
void Stage::Initialize()
{
}

//	�`��
void Stage::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	Matrix world = Matrix::Identity;
	Matrix world2 = Matrix::Identity;

	world = Matrix::CreateTranslation(5.0f, -1.0f, -8.0f);

	world2 = Matrix::CreateRotationZ(90.0f / 180.0f * DirectX::XM_PI);
	world2 *= Matrix::CreateRotationX(270.0f / 180.0f * DirectX::XM_PI);
	world2 *= Matrix::CreateTranslation(-5.0f, 9.0f, -8.0f);

	m_box->Draw(world, view, projection, DirectX::Colors::White, m_texture.Get());
	m_box2->Draw(world2, view, projection, DirectX::Colors::White, m_texture2.Get());
}

//	��n��
void Stage::Finalize()
{
}
