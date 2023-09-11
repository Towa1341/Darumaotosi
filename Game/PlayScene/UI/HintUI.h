//
//	HintUI.h
//
#pragma once
#include <SpriteBatch.h>
//#include <Mouse.h>
#include <vector>

#include "Game/GameMain.h"
#include "../../TextureManager.h"

class HintUI
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>			  m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>			  m_mihonnTexture;

	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_bodyTextures;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch>		m_spriteBatch;

	bool m_popFlag;

	//	キーボードトラッカー
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;

	//	正解の個数
	int m_bodyAnsNum;
	//	正解の番号
	std::vector<int> m_ansNumber;

public:
	HintUI();
	~HintUI() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	bool GetPopFlag() { return m_popFlag; }

	void SetAnsNum(int num);
	void SetAndNumber(int number);
};