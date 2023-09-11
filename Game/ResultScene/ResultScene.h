/*
リザルトシーン
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include <Effects.h>

#include <Model.h>

#include "../IScene.h"
#include "Game/GameMain.h"	

//	音
#include "Libraries/MyLibraries/ADX2/Adx2.h"
#include "Resources/Sounds/Sounds.h"
#include "Resources/Sounds/daruma_acf.h"



class ResultScene : public IScene
{
private:
	ADX2* m_pAdx2;
	int m_bgmID;


	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;


	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	

	//	モデル
	std::vector<std::unique_ptr<DirectX::Model>> m_pModels;

	float m_headPos;

public:

	// コンストラクタ
	ResultScene();

	// デストラクタ
	~ResultScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;


	////	モードごとに変わるよう
	//GAME_SCENE NormalUpdate(const DX::StepTimer& timer);
	//void NormalDraw();

	//GAME_SCENE ShootingUpdate(const DX::StepTimer& timer);
	//void ShootingNormalDraw();
};