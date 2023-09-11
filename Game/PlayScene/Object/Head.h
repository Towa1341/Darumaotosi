#pragma once

#include <Model.h>
#include <Effects.h>
#include <CommonStates.h>

class Head
{
public: 
	enum class eHeadState
	{
		PALA,
		SYOBONN,
		NUNN,
		NONE
	};

private:
	std::unique_ptr<DirectX::CommonStates> m_commonState;

	std::vector<std::unique_ptr<DirectX::Model>> m_pModels;

	float m_maxPos;
	float m_pos;
	float m_rot;

	eHeadState m_state;

	bool m_moveEnd;


public:
	Head();
	~Head() = default;

	void Initialize();
	void Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	void SetMaxPos(float pos) { m_maxPos = pos; }
	void SetState(eHeadState state) { m_state = state; }

	bool GetEndMove() { return m_moveEnd; }
};