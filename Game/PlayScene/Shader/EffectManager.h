//-----------------------------------
//	�G�t�F�N�g�Ǘ��N���X	
//	EffectManager.h
//-----------------------------------
#pragma once

#include "StepTimer.h"

#include <SimpleMath.h>
#include <list>
#include <WICTextureLoader.h>

#include"ShaderSample.h"

class EffectManager
{
private:

	//�o�b�`�G�t�F�N�g
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect;

	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;

	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//�G�t�F�N�g�̃|�C���^
	std::list<std::unique_ptr<ShaderSample>>	m_shockEffectList;
	std::list<std::unique_ptr<ShaderSample>>	m_chargeEffectList;


	//�e�N�X�`���f�[�^
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;

	DirectX::SimpleMath::Vector3 m_camera;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;


	bool m_shockEffectFlag;
	bool m_chargeEffectFlag;

	float m_chargeYPos;
	int m_chargeEffectNum;

	float m_timer;

	float m_rot;
public:

	void Create();

	//Create()�őS�������͒����̂ŕ���
	void Initialize(DirectX::SimpleMath::Vector3 pos);

	void Update(const DX::StepTimer& timer);

	void Render(float yPos);

	void SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	void Reset();

	void SetChargeEffectFlag(bool flag) { m_chargeEffectFlag = flag; }
	void SetShockEffectFlag(bool flag) { m_shockEffectFlag = flag; }
};