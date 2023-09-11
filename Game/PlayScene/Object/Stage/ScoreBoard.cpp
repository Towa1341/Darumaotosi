#include "pch.h"
#include "DeviceResources.h"

#include "ScoreBoard.h"
#include "../../../TextureManager.h"

using namespace DirectX;

ScoreBoard::ScoreBoard():
	m_score(0),
	m_position(SimpleMath::Vector3::Zero)
{
	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_scoreBoard = DirectX::GeometricPrimitive::CreateBox(context, DirectX::XMFLOAT3(SIZE, SIZE, 0.0f));

	m_moveNumberUI = std::make_unique<MoveNumberUI>();
	m_moveNumberUI->SetYRot(270.0f);

	m_world = DirectX::SimpleMath::Matrix::Identity;
}

void ScoreBoard::Update(DirectX::SimpleMath::Vector3 holePosition)
{
	m_position = holePosition + DirectX::SimpleMath::Vector3(0.0f, 3.0f, -2.0f);

	TextureManager* textureManager = TextureManager::GetInstance();
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numberTexture = textureManager->GetTexture(TextureManager::eTextureName::NUMBER);

	const float numberSize = 2.0f;

	SimpleMath::Vector3 pos = m_position + SimpleMath::Vector3(0.0f, 0.0f, 0.3f);

	m_moveNumberUI->SetRenderState(pos, SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(m_score);

	
	m_world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
}

void ScoreBoard::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	DirectX::CXMVECTOR color = XMVECTORF32{ {{1.0f,1.0f,1.0f,0.5f}} };

	m_scoreBoard->Draw(m_world, view, projection, color);

	m_moveNumberUI->Render(view, projection);
}

//　AABB当たり判定（円柱と）
bool ScoreBoard::Hit(DirectX::SimpleMath::Vector3 position, float radius, float height)
{
	SimpleMath::Vector3 bodyMin = SimpleMath::Vector3(
		position.x - radius,
		position.y - height/2,
		position.z - radius
	);

	SimpleMath::Vector3 bodyMax = SimpleMath::Vector3(
		position.x + radius,
		position.y + height/2,
		position.z + radius
	);

	//	見た目よりちょっと分厚くしておく
	const float zSize = 0.3f;

	SimpleMath::Vector3 scoreBoradMin = SimpleMath::Vector3(
		m_position.x - SIZE / 2,
		m_position.y - SIZE / 2,
		m_position.z - zSize
	);

	SimpleMath::Vector3 scoreBoradMax = SimpleMath::Vector3(
		m_position.x + SIZE / 2,
		m_position.y + SIZE / 2,
		m_position.z + zSize
	);

	return (scoreBoradMin.x <= bodyMax.x && scoreBoradMax.x >= bodyMin.x) &&
		   (scoreBoradMin.y <= bodyMax.y && scoreBoradMax.y >= bodyMin.y) &&
		   (scoreBoradMin.z <= bodyMax.z && scoreBoradMax.z >= bodyMin.z);
}
