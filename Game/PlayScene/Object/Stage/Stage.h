//
//	Stage.h
//
#pragma once
#include "Game/GameMain.h"

#include <GeometricPrimitive.h>
class Stage
{
private:
	std::unique_ptr<DirectX::GeometricPrimitive> m_box;
	std::unique_ptr<DirectX::GeometricPrimitive> m_box2;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;


public:
	Stage();
	~Stage() = default;

	void Initialize();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void Finalize();
};