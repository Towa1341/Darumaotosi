//
//	UIManager.h
//
#pragma once

#include "BodyUI.h"
#include "HammerUI.h"
#include "InformationUI.h"
#include "MoveNumberUI.h"

#include "../Object/Hammer.h"

class UIManager
{
private:

	std::unique_ptr<HammerUI>		 m_hammerUI;
	std::unique_ptr<BodyUI>			 m_bodyUI;
	std::unique_ptr<InformationUI>	 m_informationUI;
	std::unique_ptr<MoveNumberUI>	 m_moveNumberUI;

	Hammer::eHammerState m_state;
	int m_changeBodyNum;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

public:
	UIManager();
	~UIManager();

	void Initialize();
	void Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	void Finalize();

	void SetHammerState(Hammer::eHammerState state) { m_state = state; }
	void SetChangeBodyNum(int num) { m_changeBodyNum = num; }
	void SetNextBodyTexture(ID3D11ShaderResourceView* texture) { m_texture = texture; }
};
