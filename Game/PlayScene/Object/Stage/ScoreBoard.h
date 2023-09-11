#pragma once
#include "Game/GameMain.h"

#include <GeometricPrimitive.h>

#include "../../UI/MoveNumberUI.h"

class ScoreBoard
{
private:
	static const int SIZE = 5;

	int m_score;

	std::unique_ptr<DirectX::GeometricPrimitive> m_scoreBoard;
	std::unique_ptr<MoveNumberUI> m_moveNumberUI;

	DirectX::SimpleMath::Matrix m_world;

	DirectX::SimpleMath::Vector3 m_position;

public:
	ScoreBoard();
	~ScoreBoard() = default;

	void Update(DirectX::SimpleMath::Vector3 holePosition);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	bool Hit(DirectX::SimpleMath::Vector3 position, float radius, float height);

	void SetScore(int score) { m_score = score; }

	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }
	float GetSize() { return SIZE; }
};