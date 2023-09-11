//
//	HammerUI.h
//
#pragma once
#include "Game/GameMain.h"
#include "../Object/Hammer.h"
#include "../../TextureManager.h"

#include <GeometricPrimitive.h>

class HammerUI
{
private:
	std::unique_ptr<DirectX::GeometricPrimitive> m_box;
	std::unique_ptr<DirectX::GeometricPrimitive> m_box2;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

public:
	HammerUI();
	~HammerUI() = default;

	void Initialize();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection, Hammer::eHammerState state);
	void Finalize();
};