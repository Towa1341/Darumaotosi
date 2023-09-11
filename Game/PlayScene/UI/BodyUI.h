//
//	BodyUI.h
//
#pragma once
#include "Game/GameMain.h"

#include <VertexTypes.h>

#include "TextureRender.h"
#include "../Object/BodyManager.h"
#include "../../TextureManager.h"

class BodyUI
{
private: 
	static const int TEXTURE_NUM = 4;

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	std::vector<std::unique_ptr<TextureRender>> m_textureRenders;

	int m_nowChangeBodyNum;
public:
	BodyUI();
	~BodyUI() = default;

	void Initialize();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void Finalize();

	void SetTexture(ID3D11ShaderResourceView* texture) { m_texture = texture; }
	void SetNowChangeBodyNum(int nowChangeBodyNum) { m_nowChangeBodyNum = nowChangeBodyNum; }
};