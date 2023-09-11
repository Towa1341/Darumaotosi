#pragma once

#include "Mode.h"

#include "DeviceResources.h"
#include "../../../Libraries/MyLibraries/Camera.h"
#include "../../Libraries/MyLibraries/GridFloor.h"


#include "ModeCommons/ModeCommonProcess.h"

class GridFloor;
class DubugCamera;

class ChangeSameMode :public Mode
{
private:
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates>		m_commonState;

	std::unique_ptr<ModeCommonProcess> m_modeCommonProcess;

	std::unique_ptr<BodyManager> m_bodyManager;

	std::unique_ptr<Hammer> m_hammer;

	//	ビューとプロジェクション
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	//kari
	GridFloor* m_grid;
	Camera* m_camera;

	//	現在の体の番号
	int m_nowBodyNumber;


	//	光
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	bool m_endFlag;

public:
	ChangeSameMode();
	~ChangeSameMode();

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