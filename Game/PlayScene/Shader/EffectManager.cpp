#include "pch.h"
#include "DeviceResources.h"
#include "EffectManager.h"

#include <random>

using namespace DirectX;
using namespace DirectX::SimpleMath;

void EffectManager::Create()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	ID3D11Device1* device = pDR->GetD3DDevice();

	//バッチエフェクトの作成
	m_batchEffect = std::make_unique<DirectX::AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL);
	m_batchEffect->SetReferenceAlpha(255);
	// 入力レイアウト生成
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(
		VertexPositionTexture::InputElements,
		VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf()
	);

	//プリミティブバッチの作成
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(pDR->GetD3DDeviceContext());
	
	//テクスチャのロード
	DirectX::CreateWICTextureFromFile(
		device,
		L"Resources/Textures/shadow.png",
		nullptr,
		m_texture.GetAddressOf()
	);

	int count = 1;
	for (int i = 0; i < count; i++) {
		std::unique_ptr<ShaderSample> effect = std::make_unique <ShaderSample>();
		effect->Create();
		m_shockEffectList.push_back(std::move(effect));
	}



	m_shockEffectFlag = false;
	m_chargeEffectFlag = false;
	m_chargeYPos = 0.0f;
	m_timer = 0.0f;
	m_chargeEffectNum = 0;
	m_rot = 0.0f;
}

void EffectManager::Initialize(Vector3 pos)
{
	int range = 100;

	//life,pos,vel の値でm_effectを初期化する
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_shockEffectList.begin();
		ite != m_shockEffectList.end(); ++ite)
	{
		Vector3 vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		while (vel.Length() < 0.03f)
		{
			vel = Vector3(((rand() % (range * 2)) - range) / (float)range * 0.1f, ((rand() % (range * 2)) - range) / (float)range * 0.1f, 0);
		}

		(*ite)->Initialize(pos, vel, ShaderSample::eEffectType::SHOCK);
	}
}


void EffectManager::Update(const DX::StepTimer& timer)
{
	if (m_shockEffectFlag)
	{
		//timerを渡してShaderSampleの更新処理を行う
		for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_shockEffectList.begin();
			ite != m_shockEffectList.end(); ++ite)
		{
			(*ite)->Update(timer);
		}
	}

	float elapsedTime = float(timer.GetElapsedSeconds());
	m_timer += elapsedTime;

	if (m_chargeEffectFlag)
	{	
		// 0.1秒ごとに生成
		if (m_timer >= 0.1f && m_chargeEffectNum <= 100)
		{
			std::unique_ptr<ShaderSample> effect = std::make_unique <ShaderSample>();
			effect->Create();

			const float RADIUS = 4.0f;
			m_rot += m_timer * 10.0f;

			Vector3 position = Vector3::Zero;

			if (m_chargeEffectNum <= 50)
				 position = Vector3(RADIUS * cos(m_rot), RADIUS * sin(m_rot), 8.0f);
			else if(m_chargeEffectNum<=100)
				position = Vector3(0.0f, RADIUS * sin(m_rot), RADIUS * cos(m_rot) + 8.0f);

			effect->Initialize(position, Vector3::Zero, ShaderSample::eEffectType::CHARGE);
			effect->SetRenderState(m_camera, m_view, m_proj);

			m_chargeEffectList.push_back(std::move(effect));

			m_timer = 0.0f;
			m_chargeEffectNum++;
		}
		

		//timerを渡してShaderSampleの更新処理を行う
		for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_chargeEffectList.begin();
			ite != m_chargeEffectList.end(); ++ite)
		{
			(*ite)->Update(timer);
		}
	}
}

void EffectManager::Render(float yPos)
{
	m_chargeYPos = yPos;

	if (m_shockEffectFlag)
	{
		//m_shockEffectList描画する
		for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_shockEffectList.begin(); ite != m_shockEffectList.end(); ++ite)
		{
			(*ite)->Render(yPos);
		}
	}

	if (m_chargeEffectFlag)
	{
		// リストをソート
		m_chargeEffectList.sort([&](const std::unique_ptr<ShaderSample>& a, const std::unique_ptr<ShaderSample>& b)
			{ return (m_camera - a->GetPosition()).LengthSquared() > (m_camera - b->GetPosition()).LengthSquared(); });

		//m_chargeEffectListを描画する
		for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_chargeEffectList.begin(); ite != m_chargeEffectList.end(); ++ite)
		{
			(*ite)->Render(yPos);
		}
	}

	Reset();
}

void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 camera, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_camera = camera;
	m_view = view;
	m_proj = proj;

	//camera,view,proj,の値をm_effectに渡す
	for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_shockEffectList.begin();ite != m_shockEffectList.end(); ++ite)
	{
		(*ite)->SetRenderState(camera, view, proj);
	}
}

void EffectManager::Reset()
{
	if (!m_shockEffectFlag)
	{
		//m_shockEffectLIstをリセットする
		for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_shockEffectList.begin(); ite != m_shockEffectList.end(); ++ite)
		{
			(*ite)->Reset();
		}
	}

	if (!m_chargeEffectFlag)
	{
		//m_chargeEffectListをリセットする
		for (std::list<std::unique_ptr<ShaderSample>>::iterator ite = m_chargeEffectList.begin(); ite != m_chargeEffectList.end(); ++ite)
		{
			(*ite)->Reset();
			m_chargeEffectNum = 0;
		}
	}
}

