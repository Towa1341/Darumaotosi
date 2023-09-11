//
// Scene.cpp
//
#include "pch.h"

#include "DeviceResources.h"
#include "GameMain.h"

#include "InputManager.h"
#include "TextureManager.h"

#include "Libraries/MyLibraries/DebugCamera.h"
#include "Libraries/MyLibraries/GridFloor.h"

#include "Game/TitleScene/TitleScene.h"
#include "Game/PlayScene/PlayScene.h"
#include "Game/ResultScene/ResultScene.h"

//	グローバル変数の初期化
CLEAR_FLAG g_clearFlag = CLEAR_FLAG::SHOOTING;
GAME_MODE g_gameMode = GAME_MODE::FLY_SPECIFIC;
int g_score = 0;

// 名前空間の利用
using namespace DirectX;

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
GameMain::GameMain()
	: m_nextScene(GAME_SCENE::TITLE)		// 初期シーンの設定
	, m_pScene(nullptr)
{
}

//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
GameMain::~GameMain()
{
	Finalize();
}

//-------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------
void GameMain::Initialize()
{
	InputManager* inputManager = InputManager::GetInstance();
	inputManager->Initialize();

	TextureManager* textureManager = TextureManager::GetInstance();
	textureManager->CreateTexture();

	// シーン作成
	CreateScene();
}

//-------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------
void GameMain::Update(const DX::StepTimer& timer)
{
	InputManager* inputManager = InputManager::GetInstance();
	//inputManager->Update();

	// ESCキーで終了
	if (inputManager->GetKeyboardState().Escape)
	{
		PostQuitMessage(0);
	}

	// 次のシーンが設定されていたらシーン切り替え
	if (m_nextScene != GAME_SCENE::NONE)
	{
		// シーン削除
		DeleteScene();
		
		// シーン作成
		CreateScene();
	}

	// 実態があれば更新
	if (m_pScene != nullptr)
	{
		m_nextScene = m_pScene->Update(timer);
	}
}

//-------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------
void GameMain::Render()
{
	// 実態があれば描画
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}
}

//-------------------------------------------------------------------
// 後始末
//-------------------------------------------------------------------
void GameMain::Finalize()
{
	DeleteScene();
}

/*--------------------------------------------------
シーンの作成
--------------------------------------------------*/
void GameMain::CreateScene()
{
	// シーンが作成されているときは処理しない
	if (m_pScene != nullptr)
	{
		return;
	}

	// 次シーンの作成	シーンを増やしたい場合ここにも増やす
	switch (m_nextScene)
	{
	case GAME_SCENE::TITLE:
	{
		m_pScene = new TitleScene();
		break;
	}
	case GAME_SCENE::PLAY:
	{
		m_pScene = new PlayScene();
		break;
	}
	case GAME_SCENE::RESULT:
	{
		m_pScene = new ResultScene();
		break;
	}
	default:
	{
		// 例外なので処理を中断
		return;
	}
	}

	// 作成したシーンを初期化
	m_pScene->Initialize();
}

/*--------------------------------------------------
シーンの削除
--------------------------------------------------*/
void GameMain::DeleteScene()
{
	// シーンが作成されていなければ処理しない
	if (m_pScene == nullptr)
	{
		return;
	}

	// 現シーンの終了処理
	m_pScene->Finalize();

	// 現シーンの削除
	delete m_pScene;
	m_pScene = nullptr;
}
