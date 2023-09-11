#include "pch.h"

#include "FlySpecificMode.h"
#include "../PlayScene.h"

#include "../../InputManager.h"
#include "../../../Libraries/MyLibraries/DebugCamera.h"


using namespace DirectX;

FlySpecificMode::FlySpecificMode() :
	Mode(),
	m_camera(nullptr),
	m_nowBodyNumber(0), 
	m_deltaTime(25.0f),
	m_endFlag(false)
{
}

FlySpecificMode::~FlySpecificMode()
{
}

void FlySpecificMode::Initialize()
{
	Mode::Initialize();

	//	�X�e�[�W
	m_stage = std::make_unique<Stage>();

	//	�J����
	m_camera = new Camera(SimpleMath::Vector3(21.0f, 8.0f, -8.0f), SimpleMath::Vector3(0.0f, 4.0f, -8.0f));

	m_view = m_camera->GetViewMatrix();
	m_projection = m_camera->GetProjectionMatrix();

	//	�K�v�ȕϐ��̏���
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�R�����X�e�[�g�FD3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);
	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	//	basicEffect�̐���
	m_basicEffect = std::make_unique<BasicEffect>(device);

	// ���C�g�̐ݒ�ɕK�v�ȏ��
	m_basicEffect->SetLightingEnabled(true);		// ���C�g(on)
	m_basicEffect->SetTextureEnabled(true);			// �e�N�X�`��(on)
	m_basicEffect->SetVertexColorEnabled(false);	// ���_�J���[(off)

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

	m_uiManager = std::make_unique<UIManager>();
	m_uiManager->Initialize();
	m_moveNumberUI = std::make_unique<MoveNumberUI>();

	m_bodyManager = std::make_unique<BodyManager>();
	m_bodyManager->Initialize(DirectX::SimpleMath::Vector2(3.0f, -2.0f));
	m_bodyManager->SetBodyRot(-90.0f);

	ModeCommonProcess* modeCommonProcess = GetModeCommonProcess();
	modeCommonProcess->SetBodyManager(m_bodyManager.get());
}

GAME_MODE FlySpecificMode::Update(const DX::StepTimer& timer)
{
	//	�L�[�{�[�h�g���b�J�[
	InputManager* inputMabager = InputManager::GetInstance();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputMabager->GetKeyboardTracker();

	ModeCommonProcess* modeCommonProcess = GetModeCommonProcess();

	//	BodyManager�X�V
	m_bodyManager->Update(timer);

	m_uiManager->Update();

	//	�������Ԃ̍X�V
	m_deltaTime -= static_cast<float>(timer.GetElapsedSeconds());
	//	�O�ȉ��ł͂O�ɂ��Ċ�ʂ��o��
	if (m_deltaTime <= 0.0f)
	{
		m_deltaTime = 0.0f;
		modeCommonProcess->HeadPop();
	}
	else
	{
		//	�̂𐶐�����
		modeCommonProcess->FallBodys(timer);

		//	�G���^�[�������Ă���ԃp���[�𑝌�����
		if (inputMabager->GetKeyboardState().Enter)
		{
			modeCommonProcess->MovePower();
		}

		//	�㉺�L�[�ő̂�I������֐�
		modeCommonProcess->ChangeBodySelect();

		if (keyboardTracker->IsKeyReleased(DirectX::Keyboard::Keys::Enter) &&
			modeCommonProcess->CheckGauge() &&
			m_bodyManager->GetBodysSize() != 0 &&
			(m_bodyManager->GetSelectBody()->GetEndMoveFlag()))
		{
			//	��΂�
			m_bodyManager->FryBody();

			//	�p���[�����Z�b�g����
			modeCommonProcess->ResetPower();
		}

		//	�Q�[�W�̐ݒ�
		modeCommonProcess->SetGaugeState();
	}

	//	���������I�������
	if (GetHead()->GetEndMove())
	{
		//	�N���A����ݒ�
		if (m_bodyManager->GetGrossWeight() == modeCommonProcess->GetTragetWeight())
		{
			g_clearFlag = CLEAR_FLAG::GOOD;
		}
		else if (m_bodyManager->GetGrossWeight() > modeCommonProcess->GetTragetWeight() - 5 &&
			m_bodyManager->GetGrossWeight() < modeCommonProcess->GetTragetWeight() + 5)
		{
			g_clearFlag = CLEAR_FLAG::NORMAL;
		}
		else
		{
			g_clearFlag = CLEAR_FLAG::BAD;
		}

		//	���[���؂�ւ���
		m_endFlag = true;
	}

	return GAME_MODE::NONE;
}

void FlySpecificMode::Draw()
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
	GetHammer()->Render(m_view, m_projection);

	//	�X�e�[�W�̕`��
	m_stage->Render(m_view, m_projection);

	GetHead()->Render(m_view, m_projection);

	///		�����̕`��		///
	float numberSize = 2.0;

	const float Pos = -4.0f;

	TextureManager* textureManager = TextureManager::GetInstance();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numberTexture = textureManager->GetTexture(TextureManager::eTextureName::NUMBER);

	//	��������
	int time = m_deltaTime;
	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 11.5f, -1.5f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(time % 10);
	m_moveNumberUI->Render(m_view, m_projection);

	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 11.5f, 0.0f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(time / 10);
	m_moveNumberUI->Render(m_view, m_projection);

	//	���d��
	int num = m_bodyManager->GetGrossWeight();
	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 9.0f, -1.5f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(num % 10);
	m_moveNumberUI->Render(m_view, m_projection);

	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 9.0f, 0.0f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(num / 10);
	m_moveNumberUI->Render(m_view, m_projection);


	int targetWeight = GetModeCommonProcess()->GetTragetWeight();

	//	�ڕW�d��
	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 6.5f, -1.5f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(targetWeight % 10);
	m_moveNumberUI->Render(m_view, m_projection);

	m_moveNumberUI->SetRenderState(SimpleMath::Vector3(Pos, 6.5f, 0.0f), SimpleMath::Vector3(numberSize, numberSize, numberSize), numberTexture);
	m_moveNumberUI->SetNumber(targetWeight / 10);
	m_moveNumberUI->Render(m_view, m_projection);

	m_uiManager->SetChangeBodyNum(0);
	m_uiManager->SetNextBodyTexture(m_bodyManager->GetNextBodyTexture());
	m_uiManager->Render(m_view, m_projection);

	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	ModeCommonProcess* modeCommonProcess = GetModeCommonProcess();
	modeCommonProcess->RenderGauge(m_spriteBatch.get());

	///	�����܂� ///
	m_spriteBatch->End();

}

void FlySpecificMode::Finalize()
{
	if (m_camera != nullptr)
	{
		delete(m_camera);
		m_camera = nullptr;
	}
}

bool FlySpecificMode::GetEndFlag()
{
	return m_endFlag;
}
