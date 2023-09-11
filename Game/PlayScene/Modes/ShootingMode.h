#pragma once

#include <PrimitiveBatch.h>
#include <Effects.h>
#include <SpriteBatch.h>
#include <CommonStates.h>
#include <GeometricPrimitive.h>

#include "Mode.h"

#include "DeviceResources.h"
#include "../../../Libraries/MyLibraries/Camera.h"
#include "../../Libraries/MyLibraries/GridFloor.h"
#include "../Object/Stage/GimmickManager.h"
#include "../UI/ScoreUI.h"
#include "ModeCommons/ModeCommonProcess.h"

class GridFloor;
class DubugCamera;

class ShootingMode :public Mode
{
private:
	static const int BODYMANAGER_NUM;

	static const float ROT_MAX;	//	�p�x�̍ő�l
	static const float MOVE_ROT_VALUE;	//	�p�x���������l

private:
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>		m_commonState;

	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	//	�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;


	//	�r���[�ƃv���W�F�N�V����
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	//�J����
	Camera* m_camera;

	//	���݂̑̂̔ԍ�
	int m_nowBodyNumber;


	//	��
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	bool m_endFlag;

	//	����
	std::unique_ptr<GimmickManager> m_gimmickManager;

	std::vector<std::unique_ptr<BodyManager>> m_bodyManagers;
	BodyManager* m_pBodyManager;

	//	��΂��p�x
	float m_fryRot;
	float m_fryRotLength;

	//	��΂������H
	bool m_isFry;
	//	��΂�����
	Body* m_fryBody;

	//	�Ǐ]�t���O
	bool m_followFlag;

	//	��
	std::unique_ptr<DirectX::GeometricPrimitive> m_ground;
	//	����
	std::unique_ptr<DirectX::GeometricPrimitive> m_wall;

	//	�X�R�A��\������N���X
	std::unique_ptr<ScoreUI> m_scoreUI;

	//	���X�R�A
	int m_score;

public:
	ShootingMode();
	~ShootingMode();

	// ������
	void Initialize() override;

	// �X�V
	GAME_MODE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	bool GetEndFlag()override;

	void ChangeBpdyManagers();

	void DrawLine();

	//	�̂��΂��p�x�𓮂���
	void MoveFryRot();
	float GetFryRot() { return m_fryRot; }

	//	�\���͈͂ɃM�~�b�N��a���邩
	bool IfHitGimmick();
	//	�J������Ǐ]������
	void FollowCamera(DirectX::SimpleMath::Vector3 position);

};