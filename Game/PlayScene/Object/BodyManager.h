#pragma once

#include "Body.h"
#include "Game/GameMain.h"

#include "../UI/HintUI.h"

#include <SpriteBatch.h>
#include <list>


class BodyManager
{
private:
	static const int BODY_NUM = 5;
	static const int MAX_BODY_NUM = 15;

public:

private:
	//	体の配列
	std::list<std::unique_ptr<Body>> m_bodys;

	//	各種テクスチャの配列
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

	//	総重量
	int m_grossWeight;

	//	次に落ちてくる体
	Body::eBodyType m_nextBody;

	//	積まれている体の総数
	int m_bodyNum;

	//	落ちてくる体の配列
	std::vector<Body::eBodyType> m_fallBodys;

	//	頭の落ちてくるフラグ
	bool m_headPop;

	//	頭の位置
	float m_headPos;

	//	ヒントのUI  TODO：変数作って送るなりして無くしたい
	HintUI* m_hintUI;

	//	現在選んでいるブロックの配列番号
	int m_nowBodyNumber;

	//	体のvelosity
	float m_bodyVel;
	//	角度
	float m_bodyFryRot;

	DirectX::SimpleMath::Vector2 m_position;

public:
	BodyManager();
	~BodyManager();

	void Update(const DX::StepTimer& timer);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection,
		DirectX::BasicEffect* basicEffect, Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout);
	void Initialize(DirectX::SimpleMath::Vector2 position);
	void Finalize();

	//	体を追加
	void AddBody();

	//	設定 //
	int SetWeight(Body::eBodyType bodyType);
	Body::eBodyType SetBodyType();
	int SetTargetWeight();

	//	飛ばす関数
	void FryBody();
	//	変える関数
	void ChangeBody(Body::eBodyType changeBodyType);

	//	体を消す
	void DeleteBody();

public:
	/// 	ゲッター、セッター	 ///

	//	総重量を取得
	int GetGrossWeight()							{ return m_grossWeight; }
	//	現在の体の番号を取得
	int GetNowBodyNum()								{ return m_nowBodyNumber; }
	int GetBodysSize()								{ return m_bodys.size(); }

	//	次の体のテクスチャを取得
	ID3D11ShaderResourceView* GetNextBodyTexture()	{ return m_textures[static_cast<int>(m_nextBody)].Get(); }


	bool GetHeadPop()								{ return m_headPop; }
	bool GetBodysPop()								{ return !m_bodys.empty(); }

	float GetHeadPos()								{ return m_headPos; }

	const DirectX::SimpleMath::Vector2& GetPosition() { return m_position; }

	void SetHintUI(HintUI* hintUI)					{ m_hintUI = hintUI; }
	void SetBodyNum(int num)						{ m_nowBodyNumber = num; }
	void SetBodyVel(float vel)						{ m_bodyVel = vel; }
	void SetBodyRot(float rot)						{ m_bodyFryRot = rot; }

	Body* GetSelectBody();
};