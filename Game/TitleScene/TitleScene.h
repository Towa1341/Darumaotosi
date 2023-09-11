/*
タイトルシーン
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include <Model.h>
#include <Effects.h>
#include <VertexTypes.h>
#include <map>

#include "../IScene.h"
#include "Game/GameMain.h"	

//	音
#include "Libraries/MyLibraries/ADX2/Adx2.h"
#include "Resources/Sounds/Sounds.h"
#include "Resources/Sounds/daruma_acf.h"

class TitleScene : public IScene
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

	//	光
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	モデル
	std::vector<std::unique_ptr<DirectX::Model>> m_pModels;

	float m_rot;
	int m_renderNum;

	int m_modeNum; 
	std::map<GAME_MODE, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_modeTextures;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_leftYazirusiTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rightYazirusiTexture;


public:

	// コンストラクタ
	TitleScene();

	// デストラクタ
	~TitleScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	void ModelDraw();
};