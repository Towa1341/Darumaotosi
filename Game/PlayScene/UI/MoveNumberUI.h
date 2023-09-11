//
//	MoveNumberUI.h
//
#pragma once
#include <VertexTypes.h>    //  ���_���̌`��
#include <PrimitiveBatch.h> //  �O�p�`���̕`��
#include <Effects.h>        //  ���_�̍ʐF�A���C�e�B���O�A�e�N�X�`���Ȃ�
#include <CommonStates.h>
#include <vector>

#include "DeviceResources.h"

class MoveNumberUI
{
private:
	//	�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;


	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>   m_commonState;

	//  �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//  �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitivBatch;

	//  �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	//	�ړ�
	DirectX::SimpleMath::Vector3 m_position;
	//	�g�k
	DirectX::SimpleMath::Vector3 m_scale;
	//	��]
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