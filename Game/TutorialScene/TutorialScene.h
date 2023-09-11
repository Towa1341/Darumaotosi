/*
チュートリアルシーン

*/
#pragma once

#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <Effects.h>

#include "../IScene.h"
#include "Game/GameMain.h"

//	オブジェクト
#include "../PlayScene/Object/BodyManager.h"		//	体マネージャ
#include "../PlayScene/Object/Hammer.h"			//	ハンマー

#include "../PlayScene/Object/Head.h"			//	頭

#include "../PlayScene/Object/Stage/Stage.h"		//	ステージ

//	UI系
#include "../PlayScene/UI/MoveNumberUI.h"
#include "../PlayScene/UI/UIManager.h"
#include "../PlayScene/UI/HintUI.h"

//	音
#include "Libraries/MyLibraries/ADX2/Adx2.h"
#include "Resources/Sounds/Sounds.h"
#include "Resources/Sounds/daruma_acf.h"

//	テクスチャマネージャ
#include "../TextureManager.h"

class Camera;
class GridFloor;

class TutorialScene : public IScene
{
private:
	ADX2* m_pAdx2;

	int m_bgmID;

	//	コモンステート
	std::unique_ptr<DirectX::CommonStates>		m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch>		m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont>		m_spriteFont;

	float m_weit;

	//	デバッグカメラ
	DebugCamera* m_pDebugCamera;
	GridFloor* m_pGridFloor;

	//	ビューとプロジェクション
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tobasuTetxture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_kaeruTetxture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tetxture;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_fryTetxture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_changeTetxture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_hammerTetxture;


	//	ゲージの範囲の位置
	DirectX::SimpleMath::Vector2 m_scopePos;
	float m_hitPowerMin;
	float m_hitPowerMax;

	float m_vel;

	//	ハンマー
	std::unique_ptr<Hammer> m_hammer;

	//	キーボードトラッカー
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;

	//	UI系
	std::unique_ptr<UIManager> m_uiManager;

	//	光
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;


	std::list<std::unique_ptr<Body>> m_bodys;

	float m_rot;

	bool m_changeHammer;
	bool m_hammerFlag;

	int m_bodyNumber;

	Body::eBodyType m_changeType;

	std::map<Body::eBodyType, int> m_bodyNum;
public:

	// コンストラクタ
	TutorialScene();

	// デストラクタ
	~TutorialScene();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	void SetState(Body::eBodyType type);
};