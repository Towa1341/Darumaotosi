/*
タイトルシーン
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "../InputManager.h"
#include "../TextureManager.h"

#include "TitleScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
TitleScene::TitleScene() :
	m_rot(0.0f),
	m_renderNum(0),
	m_modeNum(0)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();

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

	//	スコアの初期化
	g_score = 0;
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
TitleScene::~TitleScene()
{
	m_pAdx2->Finalize();
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void TitleScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3Dデバイスとデバイスコンテキストの取得
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// コモンステート::D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	//	音
	m_pAdx2 = &ADX2::GetInstance();
	m_pAdx2->Initialize("Resources/Sounds/daruma.acf", "Resources/Sounds/Sounds.acb");

	//	BGM
	m_bgmID = m_pAdx2->Play(CRI_SOUNDS_リコーダービート);



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

	TextureManager* textureManager = TextureManager::GetInstance();
	//	モードごとの画像設定
	m_modeTextures[GAME_MODE::SHOOTING] = textureManager->GetTexture(TextureManager::eTextureName::MODE_SHOOTING);
	m_modeTextures[GAME_MODE::FLY_SPECIFIC] = textureManager->GetTexture(TextureManager::eTextureName::MODE_FLY);
	//m_modeTextures[GAME_MODE::FLY_EXEPTING] = textureManager->GetTexture(TextureManager::eTextureName::MODE_CHANGE);

	m_leftYazirusiTexture = textureManager->GetTexture(TextureManager::eTextureName::YAZIRUSI2);
	m_rightYazirusiTexture = textureManager->GetTexture(TextureManager::eTextureName::YAZIRUSI2);
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
GAME_SCENE TitleScene::Update(const DX::StepTimer& timer)
{
	InputManager* inputManager = InputManager::GetInstance();
	inputManager->Update();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputManager->GetKeyboardTracker();
	TextureManager* texturemanager = TextureManager::GetInstance();

	//	回転してる頭の回転と切替
	m_rot++;
	if (m_rot >= 360.0f)
	{
		if (m_renderNum == 2)
		{
			m_renderNum = 0;
		}
		else
		{
			m_renderNum++;
		}

		m_rot = 0.0f;
	}

	//	モード切替（左）
	if (keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		int num = m_modeNum - 1;

		if (num < 0)
		{
			m_modeNum = static_cast<int>(GAME_MODE::NONE) - 2;
		}
		else
		{
			m_modeNum = num;
		}
		
		//	矢印の色を変える
		m_leftYazirusiTexture = texturemanager->GetTexture(TextureManager::eTextureName::YAZIRUSI1);
	}

	if (keyboardTracker->IsKeyReleased(DirectX::Keyboard::Keys::Left))
	{
		//	矢印の色を戻す
		m_leftYazirusiTexture = texturemanager->GetTexture(TextureManager::eTextureName::YAZIRUSI2);
	}

	//	モード切替（右）
	if (keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		int num = m_modeNum + 1;

		if (num >= static_cast<int>(GAME_MODE::NONE))
		{
			m_modeNum = 0;
		}
		else
		{
			m_modeNum = num;
		}
		//	矢印の色を変える
		m_rightYazirusiTexture = texturemanager->GetTexture(TextureManager::eTextureName::YAZIRUSI1);
	}
	
	if (keyboardTracker->IsKeyReleased(DirectX::Keyboard::Keys::Right))
	{
		//	矢印の色を戻す
		m_rightYazirusiTexture = texturemanager->GetTexture(TextureManager::eTextureName::YAZIRUSI2);
	}

	//	シーン切り替え
	if (inputManager->GetKeyboardTracker()->IsKeyPressed(Keyboard::Keys::Enter))
	{
		m_pAdx2->Play(CRI_SOUNDS_MOUSE);

		g_gameMode = static_cast<GAME_MODE>(m_modeNum);

		return GAME_SCENE::PLAY;
	}

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void TitleScene::Draw()
{
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	TextureManager* textureManager = TextureManager::GetInstance();

	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::TITLE).Get(), DirectX::SimpleMath::Vector2(0.0f, -20.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, 1.5f);

	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::CRI).Get(), DirectX::SimpleMath::Vector2(0.0f, 0.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, 0.25f);

	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::OTO_LOGIC).Get(), DirectX::SimpleMath::Vector2(0.0f, 650.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	m_spriteBatch->Draw(m_modeTextures[static_cast<GAME_MODE>(m_modeNum)].Get(), DirectX::SimpleMath::Vector2(540.0f, 270.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, 2.0f);

	m_spriteBatch->Draw(m_leftYazirusiTexture.Get(), DirectX::SimpleMath::Vector2(540.0f, 430.0f), nullptr, DirectX::Colors::White, 180.0f / 180.0f * DirectX::XM_PI, DirectX::SimpleMath::Vector2::Zero);
	m_spriteBatch->Draw(m_rightYazirusiTexture.Get(), DirectX::SimpleMath::Vector2(740.0f, 330.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	m_spriteBatch->End();

	ModelDraw();
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void TitleScene::Finalize()
{
}

void TitleScene::ModelDraw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	Matrix world, view, projection, lightWorld;
	world = view = projection = lightWorld = Matrix::Identity;

	world = Matrix::CreateScale(0.25f, 0.5f, 0.25f);
	world *= Matrix::CreateRotationY((m_rot - 90.0f) / 180.0f * XM_PI);
	world *= Matrix::CreateTranslation(0.8f, -1.0f, 0.0f);

	//	iro
	DirectX::SimpleMath::Color lightColor = DirectX::SimpleMath::Color(0.25f, 0.25f, 0.25f);
	DirectX::SimpleMath::Vector3 lightDirection = DirectX::SimpleMath::Vector3(0.f, 0.0f, -4.0f);

	m_pModels[m_renderNum]->UpdateEffects([&](IEffect* effect)
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
				basicEffect->SetWorld(lightWorld); // ワールド行列を設定
				basicEffect->SetView(view); // ビュー行列の設定
				basicEffect->SetProjection(projection); // 射影行列の設定
				basicEffect->Apply(context); // エフェクトの反映
			}
		});

	m_pModels[m_renderNum]->Draw(context, *m_commonState.get(), world, view, projection);

}
