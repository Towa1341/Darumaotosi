#pragma once

#include <PrimitiveBatch.h>
#include <Effects.h>
#include <SpriteBatch.h>
#include <CommonStates.h>
#include <GeometricPrimitive.h>

#include "Mode.h"

#include "DeviceResources.h"
#include "../../../Libraries/MyLibraries/Camera.h"
#include "../../Libraries/MyLibraries/GridFloor.h"
#include "../Object/Stage/GimmickManager.h"
#include "../UI/ScoreUI.h"
#include "ModeCommons/ModeCommonProcess.h"

class GridFloor;
class DubugCamera;

class ShootingMode :public Mode
{
private:
	static const int BODYMANAGER_NUM;

	static const float ROT_MAX;	//	角度の最大値
	static const float MOVE_ROT_VALUE;	//	角度が動く数値

private:
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates>		m_commonState;

	//	プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	//	エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;


	//	ビューとプロジェクション
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	//カメラ
	Camera* m_camera;

	//	現在の体の番号
	int m_nowBodyNumber;


	//	光
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// インプットレイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	bool m_endFlag;

	//	あな
	std::unique_ptr<GimmickManager> m_gimmickManager;

	std::vector<std::unique_ptr<BodyManager>> m_bodyManagers;
	BodyManager* m_pBodyManager;

	//	飛ばす角度
	float m_fryRot;
	float m_fryRotLength;

	//	飛ばしたか？
	bool m_isFry;
	//	飛ばした体
	Body* m_fryBody;

	//	追従フラグ
	bool m_followFlag;

	//	床
	std::unique_ptr<DirectX::GeometricPrimitive> m_ground;
	//	かべ
	std::unique_ptr<DirectX::GeometricPrimitive> m_wall;

	//	スコアを表示するクラス
	std::unique_ptr<ScoreUI> m_scoreUI;

	//	総スコア
	int m_score;

public:
	ShootingMode();
	~ShootingMode();

	// 初期化
	void Initialize() override;

	// 更新
	GAME_MODE Update(const DX::StepTimer& timer) override;

	// 描画
	void Draw() override;

	// 終了処理
	void Finalize() override;

	bool GetEndFlag()override;

	void ChangeBpdyManagers();

	void DrawLine();

	//	体を飛ばす角度を動かす
	void MoveFryRot();
	float GetFryRot() { return m_fryRot; }

	//	予測範囲にギミックがaあるか
	bool IfHitGimmick();
	//	カメラを追従させる
	void FollowCamera(DirectX::SimpleMath::Vector3 position);

};