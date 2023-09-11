//
//	Hammer.cpp
//
#include"pch.h"
#include "Hammer.h"

#include "DeviceResources.h"
#include "../../GameMain.h"

Hammer::Hammer():
	m_position(7.0f,0.0f,0.9f),
	m_yRot(0.0f),
	m_xRot(0.0f),
	m_hammerState(eHammerState::FRY)
{
}

Hammer::~Hammer()
{
}

void Hammer::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);


	// エフェクトファクトリの作成
	DirectX::EffectFactory* factory = new DirectX::EffectFactory(pDR->GetD3DDevice());
	// テクスチャの読み込みパス指定
	factory->SetDirectory(L"Resources/Models");
	// ファイルを指定してモデルデータ読み込み
	m_pModel = DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/Hammer.cmo",
		*factory
	);
	delete factory;
}

void Hammer::Finalize()
{
}

void Hammer::SetXPos(float pos)
{
	const float BASE_POSITION = -2.0f;
	const float SPASE_POSITION = 6.0f;

	m_position.x = BASE_POSITION + SPASE_POSITION * pos;
}


void Hammer::Update()
{
	switch (m_hammerState)
	{
	case eHammerState::FRY:
		if (m_xRot > 0.0f)
		{
			m_xRot -= 10;
		}
		else
		{
			m_xRot = 0.0f;
		}
		break;

	case eHammerState::CHANGE:
		if (m_xRot < 180.0f)
		{
			m_xRot +=10;
		}
		else
		{
			m_xRot = 180.0f;
		}
		break;
	}
}

void Hammer::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	world = DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(-4.0f, 0.0f, 0.0f));
	world *= DirectX::SimpleMath::Matrix::CreateRotationY(m_yRot / 180 * DirectX::XM_PI);
	world *= DirectX::SimpleMath::Matrix::CreateRotationX(m_xRot / 180 * DirectX::XM_PI);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

	// モデル描画
	m_pModel->Draw(context, *m_commonState.get(), world, view, projection);
}

void Hammer::SetRot(float rot)
{
	switch (m_hammerState)
	{
	case eHammerState::FRY:
		m_yRot = rot;
		break;

	case eHammerState::CHANGE:
		m_yRot = -rot;
		break;
	}
}

void Hammer::ChangeState()
{
	if (m_hammerState == eHammerState::FRY)
	{
		m_hammerState = eHammerState::CHANGE;
	}
	else if(m_hammerState == eHammerState::CHANGE)
	{
		m_hammerState = eHammerState::FRY;
	}
}

Hammer::eHammerState Hammer::GetHammerState()
{
	return m_hammerState;
}
