//
//	Hammer.h
//
#pragma once

#include <CommonStates.h>
#include <Model.h>
#include <SimpleMath.h>
#include <Effects.h>

class Hammer
{
public:
	enum class eHammerState
	{
		FRY,
		CHANGE,
		None,
	};

private: 
	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_commonState;

	std::unique_ptr<DirectX::Model> m_pModel;

	DirectX::SimpleMath::Vector3 m_position;

	float m_yRot;
	float m_xRot;

	eHammerState m_hammerState;

public:
	Hammer();
	~Hammer();

	void Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void Initialize();
	void Finalize();

	void SetYPos(float pos) { m_position.y = pos; }
	void SetXPos(float pos);
	float GetYPos() { return m_position.y; }

	void SetRot(float rot);

	void ChangeState();
	eHammerState GetHammerState();
};