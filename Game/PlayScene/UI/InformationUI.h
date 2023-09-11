//
//	InformationUI.h
//
#pragma once
#include "Game/GameMain.h"

#include <VertexTypes.h>

#include "TextureRender.h"
#include "../Object/BodyManager.h"
#include "../../TextureManager.h"


class InformationUI
{
private:
	std::vector<std::unique_ptr<TextureRender>> m_textureRenders;

public:
	InformationUI();
	~InformationUI() = default;

	void Initialize();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void Finalize();
};