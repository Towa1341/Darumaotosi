//
//	MoveNumberUI.cpp
//
#include "pch.h"
#include "MoveNumberUI.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

MoveNumberUI::MoveNumberUI():
	m_number(0),
	m_minCutPos(0.0f),
	m_maxCutPos(0.1f),
	m_yrot(0.0f),
	m_xrot(0.0f)
{
	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	// コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);


	//	ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<BasicEffect>(device);

	//	入力レイアウトを作成するための準備
	const void* shaderByteCode;
	size_t byteCodeLength;

	//	頂点情報を決定するために必要な情報
	m_basicEffect->SetLightingEnabled(false);
	m_basicEffect->SetTextureEnabled(true);
	m_basicEffect->SetVertexColorEnabled(false);

	//	使用する頂点シェーダー情報の取得
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	//	入力レイアウトの作成
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);

	//	プリミティブバッチの作成
	m_primitivBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
}

void MoveNumberUI::Initialize()
{
}

void MoveNumberUI::SetRenderState(DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Vector3 scale, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
{
	m_texture = texture;
	m_position = position;
	m_scale = scale;
}

void MoveNumberUI::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	// 変換行列の宣言(ワールド、ビュー、射影(プロジェクション))
	Matrix world;

	// ワールド行列の初期化(単位行列)
	world = Matrix::Identity;

	world = Matrix::CreateScale(m_scale);
	world *= Matrix::CreateRotationY(m_yrot / 180.0f * DirectX::XM_PI);
	world *= Matrix::CreateRotationX(m_xrot / 180.0f * DirectX::XM_PI);
	world *= Matrix::CreateTranslation(m_position);

	//	エフェクトに各種変換行列の設定ー＞設定がまずいと見た目が正しくならない
	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(projection);
	m_basicEffect->SetTexture(m_texture.Get());
	m_basicEffect->Apply(context);

	//	入力アセンブラステージに入力レイアウトを設定
	context->IASetInputLayout(m_inputLayout.Get());

	//	半透明合成の設定
	context->OMSetBlendState(m_commonState->NonPremultiplied(), nullptr, 0xffffff);

	//	ピクセルシェーダにサンプラ(テクスチャの扱い方)を指定する
	ID3D11SamplerState* sampler = m_commonState->LinearWrap();
	context->PSSetSamplers(0, 1, &sampler);
	
	m_minCutPos = 0.1f * m_number;
	m_maxCutPos = 0.1f * (m_number + 1);

	const VertexPositionTexture vertices[] =
	{
		VertexPositionTexture(Vector3(0.0f,0.5f,0.5f),Vector2(0.0f,m_minCutPos)),
		VertexPositionTexture(Vector3(0.0f,0.5f,-0.5f),Vector2(1.0f,m_minCutPos)),
		VertexPositionTexture(Vector3(0.0f,-0.5f,0.5f),Vector2(0.0f,m_maxCutPos)),
		VertexPositionTexture(Vector3(0.0f,-0.5f,-0.5f),Vector2(1.0f,m_maxCutPos))
	};

	static const uint16_t indices[] =
	{
		0,1,2,
		1,3,2
	};

	m_primitivBatch->Begin();

	m_primitivBatch->DrawIndexed(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		indices,
		(sizeof(indices) / sizeof(indices[0])),
		vertices,
		(sizeof(vertices) / sizeof(vertices[0]))
	);

	m_primitivBatch->End();
}

void MoveNumberUI::Finalize()
{
}

