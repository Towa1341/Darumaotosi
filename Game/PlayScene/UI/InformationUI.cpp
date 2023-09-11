//
//	InformationUI.cpp
//
#include "pch.h"
#include "DeviceResources.h"

#include "InformationUI.h"

#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

InformationUI::InformationUI()
{
	//	ïKóvÇ»ïœêîÇÃèÄîı
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	TextureManager* textureManager = TextureManager::GetInstance();

	//	ê›íË
	m_textureRenders.push_back(std::make_unique<TextureRender>());

	m_textureRenders[0]->SetRenderState(Vector3(-4.5f, 9.0f, 3.0f), Vector3(5.0f, 7.0f, 5.0f), textureManager->GetTexture(TextureManager::eTextureName::UI_INTERFACE));
}

void InformationUI::Initialize()
{
}

void InformationUI::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	m_textureRenders[0]->Render(view, projection);
}

void InformationUI::Finalize()
{
}
