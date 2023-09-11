//
//	TextureRender.h
//
#pragma once
#include <VertexTypes.h>    //  頂点情報の形式
#include <PrimitiveBatch.h> //  三角形等の描画
#include <Effects.h>        //  頂点の彩色、ライティング、テクスチャなど
#include <CommonStates.h>
#include <vector>

#include "DeviceResources.h"

class TextureRender
{
private:
	//	テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;


	// コモンステート
	std::unique_ptr<DirectX::CommonStates>   m_commonState;

	//  インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//  プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitivBatch;

	//  ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	//	移動
	DirectX::SimpleMath::Vector3 m_position;
	//	拡縮
	DirectX::SimpleMath::Vector3 m_scale;

public:
	TextureRender();
	~TextureRender() = default;

	void Initialize();
	void SetRenderState(DirectX::SimpleMath::Vector3 position,
						DirectX::SimpleMath::Vector3 scale,
						const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);

	void Render(DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection);

	void Finalize();
};