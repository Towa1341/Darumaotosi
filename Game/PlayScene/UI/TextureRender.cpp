//
//	TextureRender.cpp
//
#include "pch.h"
#include "TextureRender.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

TextureRender::TextureRender()
{
	//	�K�v�ȕϐ��̏���
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);


	//	�x�[�V�b�N�G�t�F�N�g�̍쐬
	m_basicEffect = std::make_unique<BasicEffect>(device);

	//	���̓��C�A�E�g���쐬���邽�߂̏���
	const void* shaderByteCode;
	size_t byteCodeLength;

	//	���_�������肷�邽�߂ɕK�v�ȏ��
	m_basicEffect->SetLightingEnabled(false);
	m_basicEffect->SetTextureEnabled(true);
	m_basicEffect->SetVertexColorEnabled(false);

	//	�g�p���钸�_�V�F�[�_�[���̎擾
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	//	���̓��C�A�E�g�̍쐬
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);

	//	�v���~�e�B�u�o�b�`�̍쐬
	m_primitivBatch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);
}

void TextureRender::Initialize()
{
}

void TextureRender::SetRenderState(DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Vector3 scale, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& texture)
{
	m_texture = texture;
	m_position = position;
	m_scale = scale;
}

void TextureRender::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	//	�K�v�ȕϐ��̏���
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	// �ϊ��s��̐錾(���[���h�A�r���[�A�ˉe(�v���W�F�N�V����))
	Matrix world, r_view, r_projection;

	// ���[���h�s��̏�����(�P�ʍs��)
	world = Matrix::Identity;

	world = Matrix::CreateScale(m_scale);
	world *= Matrix::CreateTranslation(m_position);

	r_view = view;
	r_projection = projection;

	//	�G�t�F�N�g�Ɋe��ϊ��s��̐ݒ�[���ݒ肪�܂����ƌ����ڂ��������Ȃ�Ȃ�
	m_basicEffect->SetWorld(world);
	m_basicEffect->SetView(r_view);
	m_basicEffect->SetProjection(r_projection);
	m_basicEffect->SetTexture(m_texture.Get());
	m_basicEffect->Apply(context);

	//	���̓A�Z���u���X�e�[�W�ɓ��̓��C�A�E�g��ݒ�
	context->IASetInputLayout(m_inputLayout.Get());

	//	�����������̐ݒ�
	context->OMSetBlendState(m_commonState->NonPremultiplied(), nullptr, 0xffffff);

	//	�s�N�Z���V�F�[�_�ɃT���v��(�e�N�X�`���̈�����)���w�肷��
	ID3D11SamplerState* sampler = m_commonState->LinearWrap();
	context->PSSetSamplers(0, 1, &sampler);

	static const VertexPositionTexture vertices[] =
	{
		VertexPositionTexture(Vector3(0.0f,0.5f,0.5f),Vector2(0.0f,0.0f)),
		VertexPositionTexture(Vector3(0.0f,0.5f,-0.5f),Vector2(1.0f,0.0f)),
		VertexPositionTexture(Vector3(0.0f,-0.5f,0.5f),Vector2(0.0f,1.0f)),
		VertexPositionTexture(Vector3(0.0f,-0.5f,-0.5f),Vector2(1.0f,1.0f))
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

void TextureRender::Finalize()
{
}
