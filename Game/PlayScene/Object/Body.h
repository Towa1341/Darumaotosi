#pragma once
#include "Game/GameMain.h"

#include <GeometricPrimitive.h>
#include <Effects.h>

class Body
{
private:
	static const int POP_POS = 7;
	static const float GROUND;
	static const float GRAVITY;

public:

	enum class eBodyType
	{
		METAL = 0,	//	‹à‘®
		BRICK,	//	ƒŒƒ“ƒK
		WOOD,	//	–Ø
		HEAD,	//	“ª
		NONE,
	};

private:
	std::unique_ptr<DirectX::GeometricPrimitive> m_cylinder;

	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	bool m_moveFlag;
	bool m_deleteFlag;
	bool m_endMoveFlag;

	DirectX::SimpleMath::Vector3 m_pos;
	float m_vel;
	float m_jump;
	;
	int m_weight;

	eBodyType m_bodyType;

	float m_weitTime;
	float m_time;

	float m_radius;

	bool m_isHit;

	float m_rot;

public:
	Body(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture, int weight, eBodyType bodyType, DirectX::SimpleMath::Vector2 position);
	~Body();

	void Update(float yPos, const DX::StepTimer& timer);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection, DirectX::BasicEffect* basicEffect, Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout);
	void Initialize();
	void Finalize();

	//	Set
	void SetTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture) { m_texture = texture; }
	void SetWeight(int weight)			{ m_weight = weight; }
	void SetBodyType(eBodyType type)	{ m_bodyType = type; }
	void SetMoveFlag(bool flag)			{ m_moveFlag = flag; }
	void SetVel(float vel)				{ m_vel = vel; }
	void SetJump(float jump)			{ m_jump = jump; }
	void SetIsHit(bool isHit)			{ m_isHit = isHit; }
	void SetRot(float rot)				{ m_rot = rot; }

	//	Get
	bool GetDeleteFlag()	{ return m_deleteFlag; }
	bool GetEndMoveFlag()	{ return m_endMoveFlag; }
	int GetWeight()			{ return m_weight; }
	int GetMoveFlag()		{ return m_moveFlag; }
	eBodyType GetBodyType() { return m_bodyType; }
	bool GetIsHit()			{ return m_isHit; }

	DirectX::SimpleMath::Vector3 GetPosition() { return m_pos; }
	float GetRadius() { return m_radius; }
};