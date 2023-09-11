//
//	HintUI.cpp
//
#include "pch.h"
#include "DeviceResources.h"

#include "HintUI.h"

#include <WICTextureLoader.h>

using namespace DirectX;
using namespace DirectX::SimpleMath;

HintUI::HintUI():
	m_popFlag(false),
	m_bodyAnsNum(0),
	m_ansNumber(0)
{
	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	TextureManager* textureManager = TextureManager::GetInstance();

	//	テクスチャの生成
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	texture;
	m_bodyTextures.push_back(textureManager->GetTexture(TextureManager::eTextureName::WOOD_BODY));
	texture.Reset();

	m_bodyTextures.push_back(textureManager->GetTexture(TextureManager::eTextureName::BRICK_BODY));
	texture.Reset();

	m_bodyTextures.push_back(textureManager->GetTexture(TextureManager::eTextureName::METAL_BODY));
	texture.Reset();

	//	キーボードトラッカーの生成
	m_keyboardTracker = std::make_unique<DirectX::Keyboard::KeyboardStateTracker>();
}

void HintUI::Initialize()
{
	
}

void HintUI::Update()
{
	//	キーボードトラッカー
	DirectX::Keyboard::State state = DirectX::Keyboard::Get().GetState();
	m_keyboardTracker->Update(state);

	if (m_keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::A))
	{
		if (m_popFlag)
			m_popFlag = false;
		else
			m_popFlag = true;
	}

}

void HintUI::Render()
{
	m_spriteBatch->Begin();

	Vector2 pos = Vector2(1280 - 130, 720 - 130);

	TextureManager* textureManager = TextureManager::GetInstance();

	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::BIKKURI).Get(), pos, nullptr, Colors::White, 0.0f, Vector2::Zero, 0.5f);

	if (m_popFlag)
	{
		m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::MIHONN).Get(), Vector2(640.0f - 150.0f, 370.0f - 150.0f));

		for (int i = 0; i < m_bodyAnsNum; i++)
		{
			m_spriteBatch->Draw(m_bodyTextures[m_ansNumber[i]].Get(), Vector2(640.0f - 130.0f, 370.0f + 90.0f - (60.0f * i)));
		}
	}

	m_spriteBatch->End();
}

void HintUI::Finalize()
{
}

void HintUI::SetAnsNum(int num)
{
	m_bodyAnsNum = num;
}

void HintUI::SetAndNumber(int number)
{
	m_ansNumber.push_back(number);
}
