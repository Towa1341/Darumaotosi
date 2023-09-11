/*
リザルトシーン
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "../InputManager.h"
#include "../TextureManager.h"

#include "ResultScene.h"


using namespace DirectX;

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
ResultScene::ResultScene():
	m_texture(nullptr),
	m_headPos(-1.1f)
{
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
ResultScene::~ResultScene()
{
	m_pAdx2->Finalize();
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void ResultScene::Initialize()
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

	TextureManager* textureManaegr = TextureManager::GetInstance();

	//	クリアフラグによって表示する背景画像を変える
	switch (g_clearFlag)
	{
	case CLEAR_FLAG::GOOD:
		m_texture = textureManaegr->GetTexture(TextureManager::eTextureName::GOOD_END);
		break;

	case CLEAR_FLAG::NORMAL:
		m_texture = textureManaegr->GetTexture(TextureManager::eTextureName::NORMAL_END);
		break;

	case CLEAR_FLAG::BAD:
		m_texture = textureManaegr->GetTexture(TextureManager::eTextureName::BAD_END);
		break;
		
	case CLEAR_FLAG::SHOOTING:
		m_texture = textureManaegr->GetTexture(TextureManager::eTextureName::SHOOTING_END);
		break;
	}

	//	音
	m_pAdx2 = &ADX2::GetInstance();
	m_pAdx2->Initialize("Resources/Sounds/daruma.acf", "Resources/Sounds/Sounds.acb");

	//	BGM
	m_bgmID = m_pAdx2->Play(CRI_SOUNDS_リコーダー初級);


	//	各モデルの設定・（TODO：画像と同じようにクリアフラグで読み込むの変える）
	//	毎回全部読み込むの良くないからクリアフラグに合わせて１つだけ読み込む

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
		L"Resources/Models/nunnHead.cmo",
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
		L"Resources/Models/palaHead.cmo",
		*factory
	));
	delete factory;
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
GAME_SCENE ResultScene::Update(const DX::StepTimer& timer)
{
	InputManager* inputManager = InputManager::GetInstance();
	inputManager->Update();

	//	エンターキーでタイトルに戻る
	if (inputManager->GetKeyboardTracker()->IsKeyPressed(Keyboard::Keys::Enter))
	{
		m_pAdx2->Play(CRI_SOUNDS_MOUSE);

		return GAME_SCENE::TITLE;
	}

	//	頭を移動させる
	m_headPos += 0.01f;
	//	画面右端に行ったら左端に移動
	if (m_headPos >= 1.5f)
	{
		m_headPos = -1.5f;
	}

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void ResultScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	DirectX::SimpleMath::Matrix world, view, projection, lightWorld;
	world = view = projection = lightWorld = DirectX::SimpleMath::Matrix::Identity;


	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//	背景画像の描画
	m_spriteBatch->Draw(m_texture.Get(), SimpleMath::Vector2::Zero);

	//	もしシューティングだったらスコアを表示する
	if (g_clearFlag == CLEAR_FLAG::SHOOTING)
	{
		//	テクスチャの取得
		TextureManager* textureManager = TextureManager::GetInstance();
		const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numberTexture = textureManager->GetTexture(TextureManager::eTextureName::SCORE_NUMBER);

		//	表示する場所
		const SimpleMath::Vector2 position(570.0f, 370.0f);
		const SimpleMath::Vector2 spacePosition(60.0f, 0.0f);

		//	画像の切り取り位置
		//	10の位
		float catPosition1 = 32.0f * (g_score / 10);
		RECT rect1 = { 0.0f,catPosition1,32.0f,catPosition1 + 32.0f };
		//	1の位
		float catPosition2 = 32.0f * (g_score % 10);
		RECT rect2 = { 0.0f,catPosition2,32.0f,catPosition2 + 32.0f };

		//	描画
		m_spriteBatch->Draw(numberTexture.Get(), position, &rect1, Colors::White, 0.0f, SimpleMath::Vector2::Zero, 3.0f);
		m_spriteBatch->Draw(numberTexture.Get(), position + spacePosition, &rect2, Colors::White, 0.0f, SimpleMath::Vector2::Zero, 3.0f);
	}

	m_spriteBatch->End();


	//	動いてる頭の描画
	world = SimpleMath::Matrix::CreateScale(0.25f, 0.5f, 0.25f);
	world *= SimpleMath::Matrix::CreateRotationY(95.5f) / 180.0f * XM_PI;
	world *= SimpleMath::Matrix::CreateTranslation(m_headPos, -1.0f, 0.0f);

	int modelNumber = static_cast<int>(g_clearFlag) - 1;
	m_pModels[modelNumber]->Draw(context, *m_commonState.get(), world, view, projection);
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void ResultScene::Finalize()
{
}
