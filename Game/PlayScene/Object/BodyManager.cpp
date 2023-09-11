#include "pch.h"
#include "DeviceResources.h"

#include "BodyManager.h"
#include "../../TextureManager.h"

#include <WICTextureLoader.h>
#include <random>
#include <iterator>

BodyManager::BodyManager() :
	m_bodys{},
	m_textures{},
	m_nowBodyNumber(0),
	m_grossWeight(0),
	m_nextBody(Body::eBodyType::NONE),
	m_bodyNum(0),
	m_fallBodys{},
	m_headPop(false),
	m_headPos(0.0f),
	m_bodyVel(0.5f),
	m_bodyFryRot(0.0f)
{
}

BodyManager::~BodyManager()
{
}


void BodyManager::Initialize(DirectX::SimpleMath::Vector2 position)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();

	TextureManager* textureManager = TextureManager::GetInstance();

	//	テクスチャの登録
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	texture = textureManager->GetTexture(TextureManager::eTextureName::METAL);
	m_textures.push_back(texture);
	texture.Reset();

	texture = textureManager->GetTexture(TextureManager::eTextureName::BRICK);
	m_textures.push_back(texture);
	texture.Reset();

	texture = textureManager->GetTexture(TextureManager::eTextureName::WOOD);
	m_textures.push_back(texture);
	texture.Reset();

	m_position = position;

	//	一番最初に落ちてくる体をランダムに決定する
	for (int i = 0; i < MAX_BODY_NUM; i++)
	{
		std::mt19937 mt{ std::random_device{}() };
		std::uniform_int_distribution<int> dist(0, 2);
		int randNum = dist(mt);

		m_fallBodys.push_back(static_cast<Body::eBodyType>(randNum));
	}

	//	次の体の設定
	m_nextBody = SetBodyType();
}

void BodyManager::Finalize()
{
}

void BodyManager::Update(const DX::StepTimer& timer)
{
	int i = 0;

	//	体のアップデート
	for (std::unique_ptr<Body>& body : m_bodys)
	{
		i++;
		body->Update(i, timer);
		m_headPos = i + 1 - 0.55f;
	}

	DeleteBody();
}

//	描画
void BodyManager::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection,
	DirectX::BasicEffect* basicEffect, Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout)
{
	int bodyNum = 0;
	//	体の描画
	for (std::unique_ptr<Body>& body : m_bodys)
	{
		//	選択されている体は色を変える
		if (bodyNum == m_nowBodyNumber)
		{
			basicEffect->SetDiffuseColor(DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
		}
		else
		{
			basicEffect->SetDiffuseColor(DirectX::SimpleMath::Vector3(0.6f, 0.6f, 0.6f));
		}

		//	描画
		body->Render(view, projection, basicEffect, inputLayout);

		bodyNum++;
	}
}

void BodyManager::AddBody()
{
	//	体を追加する
	//	五個以下だったら
	if (m_bodys.size() < BODY_NUM)
	{
		//	追加と総重量加算
		m_bodys.push_back(std::make_unique<Body>(m_textures[static_cast<int>(m_nextBody)], SetWeight(m_nextBody), m_nextBody, m_position));
		m_grossWeight += SetWeight(m_nextBody);

		m_bodyNum++;

		//	次を再設定
		m_nextBody = SetBodyType();
	}
}

//	各重量の設定
int BodyManager::SetWeight(Body::eBodyType bodyType)
{
	switch (bodyType)
	{
	case Body::eBodyType::METAL:
		return 9;
		break;
	case Body::eBodyType::BRICK:
		return 6;
		break;
	case Body::eBodyType::WOOD:
		return 2;
		break;
	case Body::eBodyType::HEAD:
		return 0;
		break;
	}

	return 0;
}

Body::eBodyType BodyManager::SetBodyType()
{
	//	ランダムにする
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<int> dist(0, MAX_BODY_NUM - 1);
	int randNum = dist(mt);
	Body::eBodyType body = m_fallBodys[randNum];

	return body;
}


void BodyManager::FryBody()
{
	//	イテレータ
	std::list<std::unique_ptr<Body>>::iterator it = m_bodys.begin();

	// イテレータを動かす
	std::list<std::unique_ptr<Body>>::iterator ite = std::next(it, m_nowBodyNumber);

	//	落ち切っていて動いていなかったら動かす
	if ((*ite)->GetEndMoveFlag() && (*ite)->GetMoveFlag() == false)
	{
		//	設定する
		(*ite)->SetVel(m_bodyVel);
		(*ite)->SetRot(m_bodyFryRot);

		//	重力の設定をする
		(*ite)->SetJump((static_cast<int>((*ite)->GetBodyType()) + 1) * 0.3f);

		//	体を動かしてるよー
		(*ite)->SetMoveFlag(true);

		//	総重量を減らす
		m_grossWeight -= (*ite)->GetWeight();

		m_bodyNum--;
	}
}

void BodyManager::ChangeBody(Body::eBodyType changeBodyType)
{
	//	イテレータ
	std::list<std::unique_ptr<Body>>::iterator it = m_bodys.begin();

	// イテレータを動かす
	std::list<std::unique_ptr<Body>>::iterator ite = std::next(it, m_nowBodyNumber);

	//	 変更	//
	//	テクスチャ
	(*ite)->SetTexture(m_textures[static_cast<int>(changeBodyType)]);
	//	総重量を調整
	m_grossWeight += SetWeight(changeBodyType) - (*ite)->GetWeight();
	//	重量
	(*ite)->SetWeight(this->SetWeight(changeBodyType));
	//	タイプ
	(*ite)->SetBodyType(changeBodyType);
}

void BodyManager::DeleteBody()
{
	//	イテレータ
	std::list<std::unique_ptr<Body>>::iterator it = m_bodys.begin();

	//	消す
	while (it != m_bodys.end())
	{
		if ((*it)->GetDeleteFlag())
		{
			it = m_bodys.erase(it);
		}
		else
		{
			++it;
		}
	}
}

Body* BodyManager::GetSelectBody()
{	
	if (m_nowBodyNumber > m_bodys.size() - 1)
	{
		m_nowBodyNumber = m_bodys.size() - 1;
	}

	//	イテレータ
	std::list<std::unique_ptr<Body>>::iterator it = m_bodys.begin();

	// イテレータを動かす
	std::list<std::unique_ptr<Body>>::iterator ite = std::next(it, m_nowBodyNumber);

	return (*ite).get();
}

int BodyManager::SetTargetWeight()
{
	//	総数を決める(4～BODY_NUM)
	std::mt19937 mt{ std::random_device{}() };
	std::uniform_int_distribution<int> dist(4, BODY_NUM);
	int randNum = dist(mt);

	int weight = 0;
	//	落ちてくる体の中から決めた数分をランダムに出して目標重量に重量を足す
	for (int i = 0; i < randNum; i++)
	{
		std::uniform_int_distribution<int> dist1(0, MAX_BODY_NUM - 1);
		int randNum1 = dist1(mt);

		weight += SetWeight(m_fallBodys[randNum1]);
	}

	return weight;
}
