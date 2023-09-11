#pragma once

#include "Body.h"
#include "Game/GameMain.h"

#include "../UI/HintUI.h"

#include <SpriteBatch.h>
#include <list>


class BodyManager
{
private:
	static const int BODY_NUM = 5;
	static const int MAX_BODY_NUM = 15;

public:

private:
	//	�̂̔z��
	std::list<std::unique_ptr<Body>> m_bodys;

	//	�e��e�N�X�`���̔z��
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

	//	���d��
	int m_grossWeight;

	//	���ɗ����Ă����
	Body::eBodyType m_nextBody;

	//	�ς܂�Ă���̂̑���
	int m_bodyNum;

	//	�����Ă���̂̔z��
	std::vector<Body::eBodyType> m_fallBodys;

	//	���̗����Ă���t���O
	bool m_headPop;

	//	���̈ʒu
	float m_headPos;

	//	�q���g��UI  TODO�F�ϐ�����đ���Ȃ肵�Ė���������
	HintUI* m_hintUI;

	//	���ݑI��ł���u���b�N�̔z��ԍ�
	int m_nowBodyNumber;

	//	�̂�velosity
	float m_bodyVel;
	//	�p�x
	float m_bodyFryRot;

	DirectX::SimpleMath::Vector2 m_position;

public:
	BodyManager();
	~BodyManager();

	void Update(const DX::StepTimer& timer);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection,
		DirectX::BasicEffect* basicEffect, Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout);
	void Initialize(DirectX::SimpleMath::Vector2 position);
	void Finalize();

	//	�̂�ǉ�
	void AddBody();

	//	�ݒ� //
	int SetWeight(Body::eBodyType bodyType);
	Body::eBodyType SetBodyType();
	int SetTargetWeight();

	//	��΂��֐�
	void FryBody();
	//	�ς���֐�
	void ChangeBody(Body::eBodyType changeBodyType);

	//	�̂�����
	void DeleteBody();

public:
	/// 	�Q�b�^�[�A�Z�b�^�[	 ///

	//	���d�ʂ��擾
	int GetGrossWeight()							{ return m_grossWeight; }
	//	���݂̑̂̔ԍ����擾
	int GetNowBodyNum()								{ return m_nowBodyNumber; }
	int GetBodysSize()								{ return m_bodys.size(); }

	//	���̑̂̃e�N�X�`�����擾
	ID3D11ShaderResourceView* GetNextBodyTexture()	{ return m_textures[static_cast<int>(m_nextBody)].Get(); }


	bool GetHeadPop()								{ return m_headPop; }
	bool GetBodysPop()								{ return !m_bodys.empty(); }

	float GetHeadPos()								{ return m_headPos; }

	const DirectX::SimpleMath::Vector2& GetPosition() { return m_position; }

	void SetHintUI(HintUI* hintUI)					{ m_hintUI = hintUI; }
	void SetBodyNum(int num)						{ m_nowBodyNumber = num; }
	void SetBodyVel(float vel)						{ m_bodyVel = vel; }
	void SetBodyRot(float rot)						{ m_bodyFryRot = rot; }

	Body* GetSelectBody();
};