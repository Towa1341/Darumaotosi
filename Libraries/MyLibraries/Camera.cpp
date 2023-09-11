//
// Camera.cpp
//
#include "pch.h"
#include "Camera.h"

#include "DeviceResources.h"


using namespace DirectX;

// 定数の初期化
const float Camera::DEFAULT_CAMERA_DISTANCE = 5.0f;

//-------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------
Camera::Camera(DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 target)
	: m_view(DirectX::SimpleMath::Matrix::Identity)
	, m_eye(eye)
	, m_baseEye(eye)
	, m_target(target)
	, m_baseTarget(target)
	, m_up(0.0f, 1.0f, 0.0f)
{
	CalculateViewMatrix();
	CalculateProjectionMatrix();
}


//-------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------
Camera::~Camera()
{
}

DirectX::SimpleMath::Matrix Camera::GetViewMatrix()
{
	return m_view;
}

DirectX::SimpleMath::Matrix Camera::GetProjectionMatrix()
{
	return m_projection;
}

void Camera::ResetCamera()
{
	m_eye = m_baseEye;
	m_target = m_baseTarget;
}

void Camera::CalculateMatrix()
{
	CalculateProjectionMatrix();
	CalculateViewMatrix();
}

//-------------------------------------------------------------------
// ビュー行列の算出
//-------------------------------------------------------------------
void Camera::CalculateViewMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, m_up);
}

//-------------------------------------------------------------------
// プロジェクション行列の算出
//-------------------------------------------------------------------
void Camera::CalculateProjectionMatrix()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// ウィンドウサイズの取得
	float width = static_cast<float>(pDR->GetOutputSize().right);
	float height = static_cast<float>(pDR->GetOutputSize().bottom);

	constexpr float fieldOfView = XMConvertToRadians(45.0f);    // 画角
	float aspectRatio = width / height;							// 画面縦横比
	float nearPlane = 1.0f;                                     // カメラから一番近い投影面
	float farPlane = 100.0f;                                    // カメラから一番遠い投影面
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(fieldOfView, aspectRatio, nearPlane, farPlane);
}
