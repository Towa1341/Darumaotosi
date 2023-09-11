#include "pch.h"

#include "GimmickManager.h"

using namespace DirectX;

GimmickManager::GimmickManager()
{
	m_csvStage = std::make_unique<CSVStage>();
	m_csvStage->LoadData("Resources/Csv/Stage1.csv");

	const std::vector<CSVStage::DATA> csvdata = m_csvStage->GetDatas();

	for (int i = 0; i < m_csvStage->GetGimmickNum(); i++)
	{
		m_holes.push_back(std::make_unique<Hole>());
		m_holes[i]->Initialize();

		m_holes[i]->SetState(csvdata[i].stratPosition, csvdata[i].moveDir, csvdata[i].moveVel, csvdata[i].score);

		m_scoreBoards.push_back(std::make_unique<ScoreBoard>());
	}

	m_nowScore = 0;
}

void GimmickManager::Update(const DX::StepTimer& timer)
{
	for (int i = 0; i < m_csvStage->GetGimmickNum(); i++)
	{
		m_holes[i]->Update(timer);

		m_scoreBoards[i]->Update(m_holes[i]->GetPositon());
		m_scoreBoards[i]->SetScore(m_holes[i]->GetScore());
	}
}

void GimmickManager::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	for (int i = 0; i < m_csvStage->GetGimmickNum(); i++)
	{
		m_holes[i]->Render(view, projection);

		m_scoreBoards[i]->Render(view, projection);
	}
}

void GimmickManager::SetHitStates(DirectX::SimpleMath::Vector3 position, float radius, float height)
{
	m_position = position;
	m_radius = radius;
	m_height = height;
}

bool GimmickManager::HitHole()
{
	Hole* nearHole = GetNearHole(m_position);

	DirectX::SimpleMath::Vector3 bodyToHole = nearHole->GetPositon() - m_position;
	//	0.2の補正無くてもいいかも
	float hitLength = nearHole->GetRadius() + 0.5f;

	if (bodyToHole.Length() <= hitLength)
	{
		m_nowScore = nearHole->GetScore();

		return true;
	}

	return false;
}

bool GimmickManager::HitScoreBaord()
{
	SimpleMath::Vector3 bodyMin = SimpleMath::Vector3(
		m_position.x - m_radius,
		m_position.y - m_height / 2,
		m_position.z - m_radius
	);

	SimpleMath::Vector3 bodyMax = SimpleMath::Vector3(
		m_position.x + m_radius,
		m_position.y + m_height / 2,
		m_position.z + m_radius
	);


	ScoreBoard* nearScoreBoard = GetNearScoreBoard(m_position);

	//	見た目よりちょっと分厚くしておく
	const float zSize = 0.3f;

	SimpleMath::Vector3 scorePosition = nearScoreBoard->GetPosition();
	float size = nearScoreBoard->GetSize();

	SimpleMath::Vector3 scoreBoradMin = SimpleMath::Vector3(
		scorePosition.x - size / 2,
		scorePosition.y - size / 2,
		scorePosition.z - zSize
	);

	SimpleMath::Vector3 scoreBoradMax = SimpleMath::Vector3(
		scorePosition.x + size / 2,
		scorePosition.y + size / 2,
		scorePosition.z + zSize
	);

	return (scoreBoradMin.x <= bodyMax.x && scoreBoradMax.x >= bodyMin.x) &&
		(scoreBoradMin.y <= bodyMax.y && scoreBoradMax.y >= bodyMin.y) &&
		(scoreBoradMin.z <= bodyMax.z && scoreBoradMax.z >= bodyMin.z);
}

//	近くの穴を取得
Hole* GimmickManager::GetNearHole(const DirectX::SimpleMath::Vector3 position)
{
	float length = std::numeric_limits<float>::max();
	Hole* hole = nullptr;

	for (int i = 0; i < m_csvStage->GetGimmickNum(); i++)
	{
		float holeToBodyLength = DirectX::SimpleMath::Vector3::DistanceSquared(m_holes[i]->GetPositon(), position);
		if (holeToBodyLength < length)
		{
			hole = m_holes[i].get();

			length = holeToBodyLength;
		}
	}

	return hole;
}

//	近くのスコアボードを取得
ScoreBoard* GimmickManager::GetNearScoreBoard(const DirectX::SimpleMath::Vector3 position)
{
	float length = std::numeric_limits<float>::max();
	ScoreBoard* scoreBoard = nullptr;

	for (int i = 0; i < m_csvStage->GetGimmickNum(); i++)
	{
		float holeToBodyLength = DirectX::SimpleMath::Vector3::DistanceSquared(m_holes[i]->GetPositon(), position);
		if (holeToBodyLength < length)
		{
			scoreBoard = m_scoreBoards[i].get();

			length = holeToBodyLength;
		}
	}

	return scoreBoard;
}

bool GimmickManager::ifHit(const DirectX::SimpleMath::Vector3 landPosition)
{
	const float hitRadius = 5.0f;
	//	↑の範囲内に入ってるギミックを判定する
	for (int i = 0; i < m_csvStage->GetGimmickNum(); i++)
	{
		//	範囲の中に入っている　&&　向かってきている
		SimpleMath::Vector3 bodyToHole = m_holes[i]->GetPositon() - landPosition;

		if (bodyToHole.Length() <= hitRadius/*&& 向かってきている*/)
		{
			return true;
		}
	}

	return false;
}
