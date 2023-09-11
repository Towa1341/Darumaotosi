//
//	BodyUI.cpp
//
#include "pch.h"
#include "DeviceResources.h"

#include "BodyUI.h"

#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

BodyUI::BodyUI()
{
	//	ïKóvÇ»ïœêîÇÃèÄîı
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	ê›íË
	for (int i = 0; i < TEXTURE_NUM; i++)
	{
		m_textureRenders.push_back(std::make_unique<TextureRender>());
	}

	TextureManager* textureManager = TextureManager::GetInstance();

	m_textureRenders[0]->SetRenderState(Vector3(-4.5f, 9.0f, -19.0f), Vector3(5.0f, 7.0f, 5.0f), textureManager->GetTexture(TextureManager::eTextureName::CHANGE_BODY));
	m_textureRenders[1]->SetRenderState(Vector3(-4.0f, 8.0f, -13.0f), Vector3(7.0f, 7.0f, 7.0f), textureManager->GetTexture(TextureManager::eTextureName::NEXT_BODY));

	m_nowChangeBodyNum = 0;

	m_texture = nullptr;
}

void BodyUI::Initialize()
{
}

void BodyUI::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	//m_textureRenders[0]->Render(view, projection);

	m_textureRenders[2]->SetRenderState(Vector3(-4.5f, 8.0f, -13.1f), Vector3(3.5f, 3.5f, 3.5f), m_texture);
	m_textureRenders[2]->Render(view, projection);

	m_textureRenders[1]->Render(view, projection);

	TextureManager* textureManager = TextureManager::GetInstance();

	//m_textureRenders[3]->SetRenderState(Vector3(-4.0f, 10.8f - 2.1f * m_nowChangeBodyNum, -18.5f), Vector3(2.0f, 2.0f, 2.0f), textureManager->GetTexture(TextureManager::eTextureName::NOW));
	//m_textureRenders[3]->Render(view, projection);
}

void BodyUI::Finalize()
{
}
