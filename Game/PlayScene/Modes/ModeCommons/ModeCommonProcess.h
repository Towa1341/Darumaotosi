#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../../../../Libraries/MyLibraries/Camera.h"
#include "Game/PlayScene/Object/BodyManager.h"
#include "../../../PlayScene/Object/Hammer.h"
#include "../../Object/Head.h"

class ModeCommonProcess
{
private:
	//	体マネージャ
	BodyManager* m_bodyManager;
	//	ハンマー
	Hammer* m_hammer;
	//	頭
	Head* m_head;

	/*	ゲージ用　*/
	//	ゲージの範囲の位置
	DirectX::SimpleMath::Vector2 m_scopePos;

	//	飛ばせるパワーの最大値最小値
	float m_hitPowerMin;
	float m_hitPowerMax;

	//	パワー
	float m_power;

	//	現在の体の番号
	int m_nowBodyNumber;

	int m_targetWeight;

public:
	//	コンストラクタ
	ModeCommonProcess();
	//	デストラクタ
	~ModeCommonProcess();

	//	初期化
	void Initialize();
	//	後始末
	void Finalize();

	//	必要なものの設定
	void Setting(Hammer* const hammer, Head* const head);
	void SetBodyManager(BodyManager* const bodyManager);

	//	共通の処理↓
	
	//	一定時間ごとに体を生成する
	void FallBodys(const DX::StepTimer& timer);

	//	ゲージを動かす
	void MovePower();

	//	ゲージ系の設定
	void SetGaugeState();

	//	範囲内にあるか？
	bool CheckGauge();

	//	上下キーで体の番号を変更
	void ChangeBodySelect();

	//	パワーリセット
	void ResetPower();

	float GetPower() { return m_power; }
	int GetTragetWeight() { return m_targetWeight; }

	void HeadPop();

	//	ゲージを表示
	void RenderGauge(DirectX::SpriteBatch* spriteBatch);
};