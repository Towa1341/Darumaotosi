//
//	ScoreUI.h
//
#pragma once
#include "Game/GameMain.h"

#include <VertexTypes.h>

#include "MoveNumberUI.h"

class ScoreUI
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	std::unique_ptr<MoveNumberUI> m_numberUIOnce;
	std::unique_ptr<MoveNumberUI> m_numberUITence;

	std::unique_ptr<MoveNumberUI> m_numberUIInScore;

	bool m_inScoreCheck;
	float m_scale;
	DirectX::SimpleMath::Vector3 m_inScorePosition;
	int m_inScore;

public:
	ScoreUI();
	~ScoreUI() = default;

	void Initialize();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void Update(const DX::StepTimer& timer, int score);
	void Finalize();

	bool GetScoreCheck() { return m_inScoreCheck; }
	void SetScoreCheck(bool ret) { m_inScoreCheck = ret; }
	void SetInScore(int score) { m_inScore = score; }
};