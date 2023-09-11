#pragma once

#include "Mode.h"

#include "DeviceResources.h"
#include "../../../Libraries/MyLibraries/Camera.h"

#include "../UI/MoveNumberUI.h"
#include "../UI/UIManager.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Head.h"

#include "ModeCommons/ModeCommonProcess.h"

class DubugCamera;

class FlySpecificMode :public Mode
{
private:
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch>		m_spriteBatch;

	//	�r���[�ƃv���W�F�N�V����
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	//	�X�e�[�W�����
	std::unique_ptr<Stage> m_stage;
	
	Camera* m_camera;

	//	���݂̑̂̔ԍ�
	int m_nowBodyNumber;

	//	��
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	std::unique_ptr<UIManager> m_uiManager;
	std::unique_ptr<MoveNumberUI> m_moveNumberUI;

	float m_deltaTime;
	int m_targetWeight;

	bool m_endFlag;

	std::unique_ptr<BodyManager> m_bodyManager;

public:
	FlySpecificMode();
	~FlySpecificMode();

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