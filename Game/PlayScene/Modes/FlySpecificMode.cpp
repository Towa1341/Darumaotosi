#include "pch.h"

#include "FlySpecificMode.h"
#include "../PlayScene.h"

#include "../../InputManager.h"
#include "../../../Libraries/MyLibraries/DebugCamera.h"


using namespace DirectX;

FlySpecificMode::FlySpecificMode() :
	Mode(),
	m_camera(nullptr),
	m_nowBodyNumber(0), 
	m_deltaTime(25.0f),
	m_endFlag(false)
{
}

FlySpecificMode::~FlySpecificMode()
{
}

void FlySpecificMode::Initialize()
{
	Mode::Initialize();

	//	ステージ
	m_stage = std::make_unique<Stage>();

	//	カメラ
	m_camera = new Camera(SimpleMath::Vector3(21.0f, 8.0f, -8.0f), SimpleMath::Vector3(0.0f, 4.0f, -8.0f));

	m_view = m_camera->GetViewMatrix();
	m_projection = m_camera->GetProjectionMatrix();

	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	コモンステート：D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);
	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	//	basicEffectの生成
	m_basicEffect = std::make_unique<BasicEffect>(device);

	// ライトの設定に必要な情報
	m_basicEffect->SetLightingEnabled(true);		// ライト(on)
	m_basicEffect->SetTextureEnabled(true);			// テクスチャ(on)
	m_basicEffect->SetVertexColorEnabled(false);	// 頂点カラー(off)

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

	m_uiManager = std::make_unique<UIManager>();
	m_uiManager->Initialize();
	m_moveNumberUI = std::make_unique<MoveNumberUI>();

	m_bodyManager = std::make_unique<BodyManager>();
	m_bodyManager->Initialize(DirectX::SimpleMath::Vector2(3.0f, -2.0f));
	m_bodyManager->SetBodyRot(-90.0f);

	ModeCommonProcess* modeCommonProcess = GetModeCommonProcess();
	modeCommonProcess->SetBodyManager(m_bodyManager.get());
}

GAME_MODE FlySpecificMode::Update(const DX::StepTimer& timer)
{
	//	キーボードトラッカー
	InputManager* inputMabager = InputManager::GetInstance();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputMabager->GetKeyboardTracker();

	ModeCommonProcess* modeCommonProcess = GetModeCommonProcess();

	//	BodyManager更新
	m_bodyManager->Update(timer);

	m_uiManager->Update();

	//	制限時間の更新
	m_deltaTime -= static_cast<float>(timer.GetElapsedSeconds());
	//	０以下では０にして顔面を出す
	if (m_deltaTime <= 0.0f)
	{
		m_deltaTime = 0.0f;
		modeCommonProcess->HeadPop();
	}
	else
	{
		//	体を生成する
		modeCommonProcess->FallBodys(timer);

		//	エンターを押している間パワーを増減する
		if (inputMabager->GetKeyboardState().Enter)
		{
			modeCommonProcess->MovePower();
		}

		//	上下キーで体を選択する関数
		modeCommonProcess->ChangeBodySelect();

		if (keyboardTracker->IsKeyReleased(DirectX::Keyboard::Keys::Enter) &&
			modeCommonProcess->CheckGauge() &&
			m_bodyManager->GetBodysSize() != 0 &&
			(m_bodyManager->GetSelectBody()->GetEndMoveFlag()))
		{
			//	飛ばす
			m_bodyManager->FryBody();

			//	パワーをリセットする
			modeCommonProcess->ResetPower();
		}

		//	ゲージの設定
		modeCommonProcess->SetGaugeState();
	}

	//	頭が動き終わったら
	if (GetHead()->GetEndMove())
	{
		//	クリア判定設定
		if (m_bodyManager->GetGrossWeight() == modeCommonProcess->GetTragetWeight())
		{
			g_clearFlag = CLEAR_FLAG::GOOD;
		}
		else if (m_bodyManager->GetGrossWeight() > modeCommonProcess->GetTragetWeight() - 5 &&
			m_bodyManager->GetGrossWeight() < modeCommonProcess->GetTragetWeight() + 5)
		{
			g_clearFlag = CLEAR_FLAG::NORMAL;
		}
		else
		{
			g_clearFlag = CLEAR_FLAG::BAD;
		}

		//	しーんを切り替える
		m_endFlag = true;
	}

	return GAME_MODE::NONE;
}

void FlySpecificMode::Draw()
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
	GetHammer()->Render(m_view, m_projection);

	//	ステージの描画
	m_stage->Render(m_view, m_projection);

	GetHead()->Render(m_view, m_projection);

	///		数字の描画		///
	float numberSize = 2.0;

	const float Pos = -4.0f;

	TextureManager* textureManager = TextureManager::GetInstance();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numberTexture = textureManager->GetTexture(TextureManager::eTextureName::NUMBER);

	//	制限時間
	int time = m_deltaTime;
	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 11.5f, -1.5f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(time % 10);
	m_moveNumberUI->Render(m_view, m_projection);

	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 11.5f, 0.0f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(time / 10);
	m_moveNumberUI->Render(m_view, m_projection);

	//	総重量
	int num = m_bodyManager->GetGrossWeight();
	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 9.0f, -1.5f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(num % 10);
	m_moveNumberUI->Render(m_view, m_projection);

	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 9.0f, 0.0f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(num / 10);
	m_moveNumberUI->Render(m_view, m_projection);


	int targetWeight = GetModeCommonProcess()->GetTragetWeight();

	//	目標重量
	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 6.5f, -1.5f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(targetWeight % 10);
	m_moveNumberUI->Render(m_view, m_projection);

	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 6.5f, 0.0f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(targetWeight / 10);
	m_moveNumberUI->Render(m_view, m_projection);

	m_uiManager->SetChangeBodyNum(0);
	m_uiManager->SetNextBodyTexture(m_bodyManager->GetNextBodyTexture());
	m_uiManager->Render(m_view, m_projection);

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	ModeCommonProcess* modeCommonProcess = GetModeCommonProcess();
	modeCommonProcess->RenderGauge(m_spriteBatch.get());

	///	ここまで ///
	m_spriteBatch->End();

}

void FlySpecificMode::Finalize()
{
	if (m_camera != nullptr)
	{
		delete(m_camera);
		m_camera = nullptr;
	}
}

bool FlySpecificMode::GetEndFlag()
{
	return m_endFlag;
}
