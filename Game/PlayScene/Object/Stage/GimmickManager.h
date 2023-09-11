#pragma once

#include "CSVStage.h"
#include "Hole.h"
#include "ScoreBoard.h"

class GimmickManager
{
private:
	std::unique_ptr<CSVStage> m_csvStage;

	std::vector<std::unique_ptr<Hole>> m_holes;

	std::vector<std::unique_ptr<ScoreBoard>> m_scoreBoards;

	DirectX::SimpleMath::Vector3 m_position;
	float m_radius;
	float m_height;

	int m_nowScore;

public:
	GimmickManager();
	~GimmickManager() = default;

	void Update(const DX::StepTimer& timer);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	void SetHitStates(DirectX::SimpleMath::Vector3 position, float radius, float height);

	bool HitHole();
	bool HitScoreBaord();
	//	あたる？？？
	bool ifHit(const DirectX::SimpleMath::Vector3 landPosition);

	int GetScore() { return m_nowScore; }
private:

	//	近くの穴を取得
	Hole* GetNearHole(const DirectX::SimpleMath::Vector3 position);
	//	近くのスコアボードを取得
	ScoreBoard* GetNearScoreBoard(const DirectX::SimpleMath::Vector3 position);
};