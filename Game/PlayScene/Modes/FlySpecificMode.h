#pragma once

#include "Mode.h"

#include "DeviceResources.h"
#include "../../../Libraries/MyLibraries/Camera.h"

#include "../UI/MoveNumberUI.h"
#include "../UI/UIManager.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Head.h"

#include "ModeCommons/ModeCommonProcess.h"

class DubugCamera;

class FlySpecificMode :public Mode
{
private:
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch>		m_spriteBatch;

	//	ビューとプロジェクション
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	//	ステージを作る
	std::unique_ptr<Stage> m_stage;
	
	Camera* m_camera;

	//	現在の体の番号
	int m_nowBodyNumber;

	//	光
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	std::unique_ptr<UIManager> m_uiManager;
	std::unique_ptr<MoveNumberUI> m_moveNumberUI;

	float m_deltaTime;
	int m_targetWeight;

	bool m_endFlag;

	std::unique_ptr<BodyManager> m_bodyManager;

public:
	FlySpecificMode();
	~FlySpecificMode();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_MODE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	bool GetEndFlag()override;
};