//
//	Stage.cpp
//
#include "pch.h"
#include "DeviceResources.h"

#include "Stage.h"

#include <WICTextureLoader.h>

using namespace DirectX::SimpleMath;

//	コンストラクタ
Stage::Stage()
{
	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	Boxの生成
	m_box = DirectX::GeometricPrimitive::CreateBox(context, Vector3(20.0f, 0.0f, 30.0f));
	m_box2 = DirectX::GeometricPrimitive::CreateBox(context, Vector3(30.0f, 0.0f, 20.0f));

	//	テクスチャの生成
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/tatami.jpg", nullptr, m_texture.ReleaseAndGetAddressOf());
	DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/husuma.jpg", nullptr, m_texture2.ReleaseAndGetAddressOf());
}

//	初期化
void Stage::Initialize()
{
}

//	描画
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

//	後始末
void Stage::Finalize()
{
}
