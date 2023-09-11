#include "pch.h"
#include "DeviceResources.h"

#include "DebugNumRender.h"

#include <string>

DebugNumRender::DebugNumRender()
{
	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	コモンステート：D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");
}

void DebugNumRender::Render(float num, int xpos, int ypos)
{
	///	2D系の描画ここから ///
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//	Debugの描画はここに
	m_spriteFont->DrawString(m_spriteBatch.get(), std::to_string(num).c_str(), DirectX::XMFLOAT2(20 + xpos, 30 + ypos));

	///	ここまで ///
	m_spriteBatch->End();
}
