//
//	MoveNumberUI.h
//
#pragma once
#include <VertexTypes.h>    //  頂点情報の形式
#include <PrimitiveBatch.h> //  三角形等の描画
#include <Effects.h>        //  頂点の彩色、ライティング、テクスチャなど
#include <CommonStates.h>
#include <vector>

#include "DeviceResources.h"

class MoveNumberUI
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
	//	回転
	float m_yrot;
	float m_xrot;

	int m_number;

	float m_minCutPos;
	float m_maxCutPos;

public:
	MoveNumberUI();
	~MoveNumberUI() = default;

	void Initialize();
	void SetRenderState(DirectX::SimpleMath::Vector3 position,
		DirectX::SimpleMath::Vector3 scale,
		const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture);

	void Render(DirectX::SimpleMath::Matrix view,
		DirectX::SimpleMath::Matrix projection);

	void Finalize();

	void SetNumber(int number) { m_number = number; }
	void SetYRot(float rot) { m_yrot = rot; }
	void SetXRot(float rot) { m_xrot = rot; }
};