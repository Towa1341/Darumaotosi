/*
プレイシーン
*/
#pragma once

#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <Effects.h>

#include "../IScene.h"
#include "Game/GameMain.h"

//	音
#include "Libraries/MyLibraries/ADX2/Adx2.h"
#include "Resources/Sounds/Sounds.h"
#include "Resources/Sounds/daruma_acf.h"

class Camera;
class IMode;

class PlayScene : public IScene
{
private:
	//	音
	ADX2* m_pAdx2;
	int m_bgmID;

	//	コモンステート
	std::unique_ptr<DirectX::CommonStates>		m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch>		m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont>		m_spriteFont;

	//	次のゲームモード
	GAME_MODE m_nextGameMode;

	//	現在のゲームモード
	IMode* m_pMode;

	//	ビューとプロジェクション
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

public:

	// コンストラクタ
	PlayScene();

	// デストラクタ
	~PlayScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	//	追加

	//	モードの生成
	void CreateMode();

	//	モードの削除
	void DeleteMode();
};