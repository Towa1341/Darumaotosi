#include "pch.h"

#include "ChangeSameMode.h"
#include "../PlayScene.h"

#include "../../InputManager.h"
#include "../../../Libraries/MyLibraries/DebugCamera.h"


using namespace DirectX;

ChangeSameMode::ChangeSameMode() :
	Mode(),
	m_camera(nullptr),
	m_nowBodyNumber(0)
{
}

ChangeSameMode::~ChangeSameMode()
{
}

void ChangeSameMode::Initialize()
{
	m_bodyManager = std::make_unique<BodyManager>();
	m_bodyManager->Initialize(DirectX::SimpleMath::Vector2(3.0f,-2.0f));

	m_hammer = std::make_unique<Hammer>();
	m_hammer->Initialize();

	m_modeCommonProcess = std::make_unique<ModeCommonProcess>();
	m_modeCommonProcess->Initialize();
	//m_modeCommonProcess->Setting(m_bodyManager.get(), m_hammer.get());

	//	��ŃJ�����ɂ���
	m_camera = new Camera(DirectX::SimpleMath::Vector3(21.0f, 8.0f, -8.0f), DirectX::SimpleMath::Vector3(0.0f, 4.0f, -8.0f));

	m_view = m_camera->GetViewMatrix();
	m_projection = m_camera->GetProjectionMatrix();


	//	�K�v�ȕϐ��̏���
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�R�����X�e�[�g�FD3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	m_grid = new GridFloor(device, context, 10, 10);


	//	basicEffect�̐���
	m_basicEffect = std::make_unique<BasicEffect>(device);

	// ���C�g�̐ݒ�ɕK�v�ȏ��
	m_basicEffect->SetLightingEnabled(true); // ���C�g(on)
	m_basicEffect->SetTextureEnabled(true); // �e�N�X�`��(on)
	m_basicEffect->SetVertexColorEnabled(false); // ���_�J���[(off)

	// ���̓��C�A�E�g���쐬���邽�߂̏���
	const void* shaderByteCode;
	size_t byteCodeLength;
	// �g�p���钸�_�V�F�[�_�[���̎擾
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	// ���̓��C�A�E�g�̍쐬
	device->CreateInputLayout(
		VertexPositionNormalTexture::InputElements,
		VertexPositionNormalTexture::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);
}

GAME_MODE ChangeSameMode::Update(const DX::StepTimer& timer)
{
	//	�L�[�{�[�h�g���b�J�[
	InputManager* inputMabager = InputManager::GetInstance();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputMabager->GetKeyboardTracker();

	//	�̂𐶐�����
	m_modeCommonProcess->FallBodys(timer);

	//	�G���^�[�������Ă���ԃp���[�𑝌�����
	if (inputMabager->GetKeyboardState().Enter)
	{
		m_modeCommonProcess->MovePower();
	}

	if (keyboardTracker->IsKeyReleased(DirectX::Keyboard::Keys::Enter) &&
		m_modeCommonProcess->CheckGauge()&&
		m_bodyManager->GetBodysSize() != 0 &&
		(m_bodyManager->GetSelectBody()->GetEndMoveFlag()))
	{
		//	��΂�
		m_bodyManager->FryBody();

		//	�p���[�����Z�b�g����
		m_modeCommonProcess->ResetPower();
	}

	//	�㉺�L�[�ő̂�I������֐�
	m_modeCommonProcess->ChangeBodySelect();

	//	BodyManager�X�V
	m_bodyManager->Update(timer);

	//	��ŏ���
	m_view = m_camera->GetViewMatrix();
	m_projection = m_camera->GetProjectionMatrix();

	return GAME_MODE::NONE;
}

void ChangeSameMode::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	//	�F�ƌ���
	SimpleMath::Color lightColor = SimpleMath::Color(0.98f, 0.98f, 0.98f);
	SimpleMath::Vector3 lightDirection = SimpleMath::Vector3(-1.0f, -1.0f, 0.2f);

	// �G�t�F�N�g�Ƀ��C�g�̌��ʂ�ݒ肷��
	m_basicEffect->SetLightEnabled(0, true); // ���C�g 0 �Ԃ��g�p

	m_basicEffect->SetLightDiffuseColor(0, lightColor); // ���C�g�̐F���w��

	m_basicEffect->SetLightDirection(0, lightDirection); // ���C�g�̌������w��

	// �G�t�F�N�g�Ɋe��ϊ��s��̐ݒ� -> �ݒ肪�}�Y���ƌ����ڂ��������Ȃ�Ȃ�
	m_basicEffect->SetWorld(world); // ���[���h�s���ݒ�
	m_basicEffect->SetView(m_view); // �r���[�s��̐ݒ�
	m_basicEffect->SetProjection(m_projection); // �ˉe�s��̐ݒ�
	m_basicEffect->Apply(context); // �G�t�F�N�g�̔��f


	//	�}�l�[�W���̕`��
	m_bodyManager->Render(m_view, m_projection, m_basicEffect.get(), m_inputLayout);

	//	�n���}�[�̕`��
	m_hammer->Render(m_view, m_projection);

	//	��ŏ���
	m_grid->Draw(context, m_commonState.get(), m_view, m_projection);

}

void ChangeSameMode::Finalize()
{
	if (m_camera != nullptr)
	{
		delete(m_camera);
		m_camera = nullptr;
	}
}

bool ChangeSameMode::GetEndFlag()
{
	return m_endFlag;
}
