#include "pch.h"
#include "DeviceResources.h"

#include "Head.h"

using namespace DirectX;

Head::Head():
	m_maxPos(0.0f),
	m_pos(10.0f),
	m_rot(0.0f),
	m_state(eHeadState::NONE),
	m_moveEnd(false)
{
}

void Head::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// エフェクトファクトリの作成
	EffectFactory* factory = new EffectFactory(pDR->GetD3DDevice());
	// テクスチャの読み込みパス指定
	factory->SetDirectory(L"Resources/Models");
	// ファイルを指定してモデルデータ読み込み
	m_pModels.push_back(DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/palaHead.cmo",
		*factory
	));
	delete factory;

	factory = new EffectFactory(pDR->GetD3DDevice());
	// テクスチャの読み込みパス指定
	factory->SetDirectory(L"Resources/Models");
	// ファイルを指定してモデルデータ読み込み
	m_pModels.push_back(DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/syobonnHead.cmo",
		*factory
	));
	delete factory;

	factory = new EffectFactory(pDR->GetD3DDevice());
	// テクスチャの読み込みパス指定
	factory->SetDirectory(L"Resources/Models");
	// ファイルを指定してモデルデータ読み込み
	m_pModels.push_back(DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/nunnHead.cmo",
		*factory
	));
	delete factory;

}

void Head::Update()
{
	if (m_pos > m_maxPos)
	{
		m_pos -= 0.1f;
		m_rot += 36.0f;
	}
	else
	{
		m_pos = m_maxPos;

		m_moveEnd = true;
	}
}

void Head::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();


	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::Identity;

	//	iro
	DirectX::SimpleMath::Color lightColor = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f);
	DirectX::SimpleMath::Vector3 lightDirection = DirectX::SimpleMath::Vector3(-1.0f, -1.0f, 0.2f);


	for (std::vector<std::unique_ptr<Model>>::iterator it = m_pModels.begin(); it != m_pModels.end(); ++it)
	{
		(*it)->UpdateEffects([&](IEffect* effect)
			{
				IEffectLights* lights = dynamic_cast<IEffectLights*>(effect);
				if (lights)
				{
					// エフェクトにライトの効果を設定する
					lights->SetLightEnabled(0, true); // ライト 0 番を使用

					lights->SetLightDiffuseColor(0, lightColor); // ライトの色を指定

					lights->SetLightDirection(0, lightDirection); // ライトの向きを指定
				}
				BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
				if (basicEffect)
				{
					// エフェクトに各種変換行列の設定 -> 設定がマズいと見た目が正しくならない
					basicEffect->SetWorld(world); // ワールド行列を設定
					basicEffect->SetView(view); // ビュー行列の設定
					basicEffect->SetProjection(projection); // 射影行列の設定
					basicEffect->Apply(context); // エフェクトの反映
				}
			});
	}



	world = DirectX::SimpleMath::Matrix::CreateScale(1.5f, 2.0f, 1.5f);
	//world *= DirectX::SimpleMath::Matrix::CreateRotationY(-180.0f / 180.0f * XM_PI);
	world *= DirectX::SimpleMath::Matrix::CreateRotationY(m_rot / 180.0f * XM_PI);
	world *= DirectX::SimpleMath::Matrix::CreateTranslation(3.0f, m_pos, -2.0f);

	switch (m_state)
	{
	case eHeadState::PALA:
		// モデル描画
		m_pModels[0]->Draw(context, *m_commonState.get(), world, view, projection);
		break;

	case eHeadState::SYOBONN:
		m_pModels[1]->Draw(context, *m_commonState.get(), world, view, projection);
		break;

	case eHeadState::NUNN:
		m_pModels[2]->Draw(context, *m_commonState.get(), world, view, projection);
		break;
	}
}
