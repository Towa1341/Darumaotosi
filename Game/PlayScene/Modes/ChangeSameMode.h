#pragma once

#include "Mode.h"

#include "DeviceResources.h"
#include "../../../Libraries/MyLibraries/Camera.h"
#include "../../Libraries/MyLibraries/GridFloor.h"


#include "ModeCommons/ModeCommonProcess.h"

class GridFloor;
class DubugCamera;

class ChangeSameMode :public Mode
{
private:
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>		m_commonState;

	std::unique_ptr<ModeCommonProcess> m_modeCommonProcess;

	std::unique_ptr<BodyManager> m_bodyManager;

	std::unique_ptr<Hammer> m_hammer;

	//	�r���[�ƃv���W�F�N�V����
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	//kari
	GridFloor* m_grid;
	Camera* m_camera;

	//	���݂̑̂̔ԍ�
	int m_nowBodyNumber;


	//	��
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	bool m_endFlag;

public:
	ChangeSameMode();
	~ChangeSameMode();

	// ������
	void Initialize() override;

	// �X�V
	GAME_MODE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	bool GetEndFlag()override;
};