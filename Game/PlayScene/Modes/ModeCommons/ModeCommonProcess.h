#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>

#include "../../../../Libraries/MyLibraries/Camera.h"
#include "Game/PlayScene/Object/BodyManager.h"
#include "../../../PlayScene/Object/Hammer.h"
#include "../../Object/Head.h"

class ModeCommonProcess
{
private:
	//	�̃}�l�[�W��
	BodyManager* m_bodyManager;
	//	�n���}�[
	Hammer* m_hammer;
	//	��
	Head* m_head;

	/*	�Q�[�W�p�@*/
	//	�Q�[�W�͈̔͂̈ʒu
	DirectX::SimpleMath::Vector2 m_scopePos;

	//	��΂���p���[�̍ő�l�ŏ��l
	float m_hitPowerMin;
	float m_hitPowerMax;

	//	�p���[
	float m_power;

	//	���݂̑̂̔ԍ�
	int m_nowBodyNumber;

	int m_targetWeight;

public:
	//	�R���X�g���N�^
	ModeCommonProcess();
	//	�f�X�g���N�^
	~ModeCommonProcess();

	//	������
	void Initialize();
	//	��n��
	void Finalize();

	//	�K�v�Ȃ��̂̐ݒ�
	void Setting(Hammer* const hammer, Head* const head);
	void SetBodyManager(BodyManager* const bodyManager);

	//	���ʂ̏�����
	
	//	��莞�Ԃ��Ƃɑ̂𐶐�����
	void FallBodys(const DX::StepTimer& timer);

	//	�Q�[�W�𓮂���
	void MovePower();

	//	�Q�[�W�n�̐ݒ�
	void SetGaugeState();

	//	�͈͓��ɂ��邩�H
	bool CheckGauge();

	//	�㉺�L�[�ő̂̔ԍ���ύX
	void ChangeBodySelect();

	//	�p���[���Z�b�g
	void ResetPower();

	float GetPower() { return m_power; }
	int GetTragetWeight() { return m_targetWeight; }

	void HeadPop();

	//	�Q�[�W��\��
	void RenderGauge(DirectX::SpriteBatch* spriteBatch);
};