#include "pch.h"

#include "ChangeSameMode.h"
#include "../PlayScene.h"

#include "../../InputManager.h"
#include "../../../Libraries/MyLibraries/DebugCamera.h"


using namespace DirectX;

ChangeSameMode::ChangeSameMode() :
	Mode(),
	m_camera(nullptr),
	m_nowBodyNumber(0)
{
}

ChangeSameMode::~ChangeSameMode()
{
}

void ChangeSameMode::Initialize()
{
	m_bodyManager = std::make_unique<BodyManager>();
	m_bodyManager->Initialize(DirectX::SimpleMath::Vector2(3.0f,-2.0f));

	m_hammer = std::make_unique<Hammer>();
	m_hammer->Initialize();

	m_modeCommonProcess = std::make_unique<ModeCommonProcess>();
	m_modeCommonProcess->Initialize();
	//m_modeCommonProcess->Setting(m_bodyManager.get(), m_hammer.get());

	//	後でカメラにする
	m_camera = new Camera(DirectX::SimpleMath::Vector3(21.0f, 8.0f, -8.0f), DirectX::SimpleMath::Vector3(0.0f, 4.0f, -8.0f));

	m_view = m_camera->GetViewMatrix();
	m_projection = m_camera->GetProjectionMatrix();


	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	コモンステート：D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	m_grid = new GridFloor(device, context, 10, 10);


	//	basicEffectの生成
	m_basicEffect = std::make_unique<BasicEffect>(device);

	// ライトの設定に必要な情報
	m_basicEffect->SetLightingEnabled(true); // ライト(on)
	m_basicEffect->SetTextureEnabled(true); // テクスチャ(on)
	m_basicEffect->SetVertexColorEnabled(false); // 頂点カラー(off)

	// 入力レイアウトを作成するための準備
	const void* shaderByteCode;
	size_t byteCodeLength;
	// 使用する頂点シェーダー情報の取得
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	// 入力レイアウトの作成
	device->CreateInputLayout(
		VertexPositionNormalTexture::InputElements,
		VertexPositionNormalTexture::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);
}

GAME_MODE ChangeSameMode::Update(const DX::StepTimer& timer)
{
	//	キーボードトラッカー
	InputManager* inputMabager = InputManager::GetInstance();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputMabager->GetKeyboardTracker();

	//	体を生成する
	m_modeCommonProcess->FallBodys(timer);

	//	エンターを押している間パワーを増減する
	if (inputMabager->GetKeyboardState().Enter)
	{
		m_modeCommonProcess->MovePower();
	}

	if (keyboardTracker->IsKeyReleased(DirectX::Keyboard::Keys::Enter) &&
		m_modeCommonProcess->CheckGauge()&&
		m_bodyManager->GetBodysSize() != 0 &&
		(m_bodyManager->GetSelectBody()->GetEndMoveFlag()))
	{
		//	飛ばす
		m_bodyManager->FryBody();

		//	パワーをリセットする
		m_modeCommonProcess->ResetPower();
	}

	//	上下キーで体を選択する関数
	m_modeCommonProcess->ChangeBodySelect();

	//	BodyManager更新
	m_bodyManager->Update(timer);

	//	後で消す
	m_view = m_camera->GetViewMatrix();
	m_projection = m_camera->GetProjectionMatrix();

	return GAME_MODE::NONE;
}

void ChangeSameMode::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	//	色と向き
	SimpleMath::Color lightColor = SimpleMath::Color(0.98f, 0.98f, 0.98f);
	SimpleMath::Vector3 lightDirection = SimpleMath::Vector3(-1.0f, -1.0f, 0.2f);

	// エフェクトにライトの効果を設定する
	m_basicEffect->SetLightEnabled(0, true); // ライト 0 番を使用

	m_basicEffect->SetLightDiffuseColor(0, lightColor); // ライトの色を指定

	m_basicEffect->SetLightDirection(0, lightDirection); // ライトの向きを指定

	// エフェクトに各種変換行列の設定 -> 設定がマズいと見た目が正しくならない
	m_basicEffect->SetWorld(world); // ワールド行列を設定
	m_basicEffect->SetView(m_view); // ビュー行列の設定
	m_basicEffect->SetProjection(m_projection); // 射影行列の設定
	m_basicEffect->Apply(context); // エフェクトの反映


	//	マネージャの描画
	m_bodyManager->Render(m_view, m_projection, m_basicEffect.get(), m_inputLayout);

	//	ハンマーの描画
	m_hammer->Render(m_view, m_projection);

	//	後で消す
	m_grid->Draw(context, m_commonState.get(), m_view, m_projection);

}

void ChangeSameMode::Finalize()
{
	if (m_camera != nullptr)
	{
		delete(m_camera);
		m_camera = nullptr;
	}
}

bool ChangeSameMode::GetEndFlag()
{
	return m_endFlag;
}
