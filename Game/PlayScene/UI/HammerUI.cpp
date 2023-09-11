//
//	HammerUI.cpp
//
#include "pch.h"
#include "DeviceResources.h"

#include "HammerUI.h"
#include "../Object/Hammer.h"

#include <WICTextureLoader.h>

using namespace DirectX::SimpleMath;

HammerUI::HammerUI()
{
	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	Boxの生成
	m_box = DirectX::GeometricPrimitive::CreateBox(context, Vector3(5.0f, 0.0f, 2.0f));
	m_box2 = DirectX::GeometricPrimitive::CreateBox(context, Vector3(5.0f, 0.0f, 2.0f));

	TextureManager* textureManager = TextureManager::GetInstance();
	m_texture = textureManager->GetTexture(TextureManager::eTextureName::FLY);
}

void HammerUI::Initialize()
{
}

void HammerUI::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection,Hammer::eHammerState state)
{
	Matrix world = Matrix::Identity;

	world = Matrix::CreateRotationY(-90.0f / 180.0f * DirectX::XM_PI);
	world *= Matrix::CreateRotationZ(-20.0f / 180.0f * DirectX::XM_PI);
	world *= Matrix::CreateTranslation(7.0f, 0.0f, -3.0f);
	
	TextureManager* textureManager = TextureManager::GetInstance();

	//	ハンマーの状態
	if (state == Hammer::eHammerState::CHANGE)
		m_texture = textureManager->GetTexture(TextureManager::eTextureName::CHANGE);
	else if (state == Hammer::eHammerState::FRY)
		m_texture = textureManager->GetTexture(TextureManager::eTextureName::FLY);

	m_box->Draw(world, view, projection, DirectX::Colors::White, m_texture.Get());
}

void HammerUI::Finalize()
{
}
