#pragma once
#include "Game/GameMain.h"

#include <GeometricPrimitive.h>
#include "CSVStage.h"

class Hole
{
private:

	std::unique_ptr<DirectX::GeometricPrimitive> m_hole;

	DirectX::SimpleMath::Vector3 m_position;
	float m_moveLength;

	float m_radius;
	float m_height;
	float m_moveVel;

	bool m_isChange;

	int m_score;

	CSVStage::MOVE_DIR m_modeDir;

public:
	Hole();
	~Hole() = default;


	void Initialize();

	void Update(const DX::StepTimer& timer);

	void MoveUp(const DX::StepTimer& timer);
	void MoveSide(const DX::StepTimer& timer);
	void MoveAround(const DX::StepTimer& timer);

	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);


	void SetState(DirectX::SimpleMath::Vector3 pos, CSVStage::MOVE_DIR modeDir,float moveVel,int score);


	DirectX::SimpleMath::Vector3 GetPositon() { return m_position; }

	float GetRadius() { return m_radius; }

	bool Hit(DirectX::SimpleMath::Vector3 bodyPosition);

	int GetScore() { return m_score; }
};