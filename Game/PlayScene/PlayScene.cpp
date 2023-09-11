/*
プレイシーン
*/
#include "pch.h"

#include "DeviceResources.h"
#include "../GameMain.h"

#include "../InputManager.h"

#include "PlayScene.h"

#include "Modes/IMode.h"
#include "Modes/FlySpecificMode.h"
#include "Modes/FlyExceptingMode.h"
#include "Modes/ShootingMode.h"

#include <string>

// 名前空間の利用
using namespace DirectX::SimpleMath;
using namespace DirectX;

/*--------------------------------------------------
コンストラクタ
--------------------------------------------------*/
PlayScene::PlayScene() :
	m_nextGameMode(g_gameMode),
	m_pMode(nullptr)
{
}

/*--------------------------------------------------
デストラクタ
--------------------------------------------------*/
PlayScene::~PlayScene()
{
	m_pAdx2->Finalize();
	Finalize();
}

/*--------------------------------------------------
初期化
--------------------------------------------------*/
void PlayScene::Initialize()
{
	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	コモンステート：D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// スプライトバッチ::デバッグ情報の表示に必要
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	//	音
	m_pAdx2 = &ADX2::GetInstance();
	m_pAdx2->Initialize("Resources/Sounds/daruma.acf", "Resources/Sounds/Sounds.acb");

	//	BGM
	m_bgmID = m_pAdx2->Play(CRI_SOUNDS_おなじみの風景);
}

/*--------------------------------------------------
更新
戻り値	:次のシーン番号
--------------------------------------------------*/
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	InputManager* inputManager = InputManager::GetInstance();
	inputManager->Update();

	// 次のシーンが設定されていたらシーン切り替え
	if (m_nextGameMode != GAME_MODE::NONE)
	{
		// シーン削除
		DeleteMode();

		// シーン作成
		CreateMode();
	}

	// 実態があれば更新
	if (m_pMode != nullptr)
	{
		m_nextGameMode = m_pMode->Update(timer);

		//	終わりフラグが立ったらリザルトへ
		if (m_pMode->GetEndFlag())
		{
			return GAME_SCENE::RESULT;
		}
	}

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
描画
--------------------------------------------------*/
void PlayScene::Draw()
{
	///	2D系の描画ここから ///
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//	Debugの描画はここに
	//	m_spriteFont->DrawString(m_spriteBatch.get(), std::to_string(/* 変数名 */).c_str(), DirectX::XMFLOAT2(10, 30));

	///	ここまで ///
	m_spriteBatch->End();
	
	// 実態があれば描画
	if (m_pMode != nullptr)
	{
		m_pMode->Draw();
	}
}

/*--------------------------------------------------
終了処理
--------------------------------------------------*/
void PlayScene::Finalize()
{
	DeleteMode();
}

void PlayScene::CreateMode()
{
	// シーンが作成されているときは処理しない
	if (m_pMode != nullptr)
	{
		return;
	}

	// 次シーンの作成	シーンを増やしたい場合ここにも増やす
	switch (m_nextGameMode)
	{
	case GAME_MODE::FLY_SPECIFIC:
	{
		m_pMode = new FlySpecificMode();
		break;
	}
	/*case GAME_MODE::FLY_EXEPTING:
	{
		m_pMode = new FlyExceptingMode();
		break;
	}*/
	case GAME_MODE::SHOOTING:
	{
		m_pMode = new ShootingMode();
		break;
	}
	default:
	{
		// 例外なので処理を中断
		return;
	}

	}

	// 作成したシーンを初期化
	m_pMode->Initialize();
}

void PlayScene::DeleteMode()
{
	// シーンが作成されていなければ処理しない
	if (m_pMode == nullptr)
	{
		return;
	}

	// 現シーンの終了処理
	m_pMode->Finalize();

	// 現シーンの削除
	delete m_pMode;
	m_pMode = nullptr;
}