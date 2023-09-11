#include "pch.h"
#include "DeviceResources.h"

#include "TextureManager.h"

#include <WICTextureLoader.h>

TextureManager::TextureManager() :
	m_textures{}
{
}

void TextureManager::CreateTexture()
{
	//	テクスチャファイルリスト
	std::wstring fileList[] =
	{
		L"Resources/Textures/metal.jpg",
		L"Resources/Textures/brick.jpg",
		L"Resources/Textures/wood.jpg",
		L"Resources/Textures/Number.png",
		L"Resources/Textures/scoreNumber.png",
		L"Resources/Textures/end.png",
		L"Resources/Textures/GaugeBase.jpg",
		L"Resources/Textures/Gauge.jpg",
		L"Resources/Textures/GaugeScope.png",
		L"Resources/Textures/Title.png",
		L"Resources/Textures/cri.png",
		L"Resources/Textures/otologic.png",
		L"Resources/Textures/changeBodys.png",
		L"Resources/Textures/NextBody.png",
		L"Resources/Textures/now.png",
		L"Resources/Textures/Rred.png",
		L"Resources/Textures/Rbule.png",
		L"Resources/Textures/interface.png",
		L"Resources/Textures/Bikkuri.jpg",
		L"Resources/Textures/mihonn.png",
		L"Resources/Textures/woodBody.PNG",
		L"Resources/Textures/brickBody.PNG",
		L"Resources/Textures/metalBody.PNG",
		L"Resources/Textures/tobasuTutorial.png",
		L"Resources/Textures/kaeruTutorial.png",
		L"Resources/Textures/space.png",
		L"Resources/Textures/NormalEnd.png",
		L"Resources/Textures/GoodEnd.png",
		L"Resources/Textures/BadEnd.png",
		L"Resources/Textures/ShootingEnd.png",
		L"Resources/Textures/ModeShooting.jpg",
		L"Resources/Textures/ModeChange.jpg",
		L"Resources/Textures/ModeFly.jpg",
		L"Resources/Textures/tatami.jpg",
		L"Resources/Textures/husuma.jpg",
		L"Resources/Textures/yazirusi2.png",
		L"Resources/Textures/yazirusi3.png",
		L"Resources/Textures/operation1.jpg",
		L"Resources/Textures/operation2.jpg",
	};

	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();

	//	使いまわし
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

	for (int i = 0; i < static_cast<int>(eTextureName::NONE); i++)
	{
		DirectX::CreateWICTextureFromFile(device, fileList[i].c_str(), nullptr, texture.ReleaseAndGetAddressOf());
		eTextureName name = static_cast<eTextureName>(i);
		m_textures[name] = texture;

		texture.Reset();
	}
}
