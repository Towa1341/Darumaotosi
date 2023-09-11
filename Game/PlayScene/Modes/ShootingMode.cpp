#include "pch.h"

#include "ShootingMode.h"
#include "../PlayScene.h"

#include "../../InputManager.h"
#include "../../../Libraries/MyLibraries/DebugCamera.h"

#include "Libraries/MyLibraries/DebugNumRender.h"

using namespace DirectX;

const int ShootingMode::BODYMANAGER_NUM = 3;
const float ShootingMode::ROT_MAX = 30.0f;
const float ShootingMode::MOVE_ROT_VALUE = 5.0f;

ShootingMode::ShootingMode() :
	Mode(),
	m_camera(nullptr),
	m_nowBodyNumber(0),
	m_endFlag(false), 
	m_fryRot(-90.0f),
	m_fryRotLength(0.0f),
	m_isFry(false),
	m_fryBody(nullptr),
	m_followFlag(false),
	m_score(0)
{
}

ShootingMode::~ShootingMode()
{
	//	�Ō�ɃO���[�o���ϐ��ɔ��f����
	g_score = m_score;
}

void ShootingMode::Initialize()
{
	//	�J����
	m_camera = new Camera(SimpleMath::Vector3(0.0f, 20.0f, 20.0f), SimpleMath::Vector3(0.0f, 4.0f, -8.0f));

	m_view = m_camera->GetViewMatrix();
	m_projection = m_camera->GetProjectionMatrix();


	//	�K�v�ȕϐ��̏���
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�R�����X�e�[�g�FD3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	//	�v���~�e�B�u�o�b�`
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
	//	�G�t�F�N�g
	m_effect = std::make_unique<BasicEffect>(device);
	//	�X�v���C�g�o�b�`
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

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

	//	�M�~�b�N�}�l�[�W��
	m_gimmickManager = std::make_unique<GimmickManager>();

	//	�ς܂��̂̍ő吔
	const int STACK_BODY_NUM = 3;

	//	BodyManager�̐����Ə�����
	for (int i = 0; i < BODYMANAGER_NUM; i++)
	{
		m_bodyManagers.push_back(std::make_unique<BodyManager>());
		m_bodyManagers[i]->Initialize(DirectX::SimpleMath::Vector2((i * 6.0f) - 6.0f, -2.0f));

		//	Body�𑝂₷
		for (int j = 0; j < STACK_BODY_NUM; j++)
		{
			m_bodyManagers[i]->AddBody();
		}
	}

	//	�ŏ��ɑI������Ă��镨���O�Ɏw��
	m_pBodyManager = m_bodyManagers[0].get();

	//	�N���A�������V���[�e�B���O���[�h�p�ɕύX���Ă���
	g_clearFlag = CLEAR_FLAG::SHOOTING;

	//	�n���}�[�̈ʒu��ݒ肵�Ă���
	GetHammer()->SetXPos(0);

	//	�䂩
	m_ground = DirectX::GeometricPrimitive::CreateBox(context, SimpleMath::Vector3(60.0f, 0.0f, 50.0f));
	m_wall = DirectX::GeometricPrimitive::CreateBox(context, SimpleMath::Vector3(55.0f, 0.0f, 30.0f));

	//	�X�R�A��\������N���X�̏�����
	m_scoreUI = std::make_unique<ScoreUI>();
}

GAME_MODE ShootingMode::Update(const DX::StepTimer& timer)
{
	//	�L�[�{�[�h�g���b�J�[
	InputManager* inputMabager = InputManager::GetInstance();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputMabager->GetKeyboardTracker();

	ModeCommonProcess* modeCommonProcess = GetModeCommonProcess();

	modeCommonProcess->SetBodyManager(m_pBodyManager);

	//	�̂𐶐�����
	//modeCommonProcess->FallBodys(timer);

	if (!m_isFry)
	{
		//	�G���^�[�������Ă���ԃp���[�𑝌�����
		if (inputMabager->GetKeyboardState().Enter)
		{
			modeCommonProcess->MovePower();
		}

		//	��΂�����I�����違�ł��؂��Ă�����I��s��
		ChangeBpdyManagers();
		//	�㉺�L�[�ő̂�I������֐�
		modeCommonProcess->ChangeBodySelect();
		//	���E�L�[�Ō��������߂Đݒ肷��
		MoveFryRot();
	}
	else
	{
		m_isFry = m_pBodyManager->GetSelectBody()->GetDeleteFlag();
	}

	//	��΂�������
	if (keyboardTracker->IsKeyReleased(DirectX::Keyboard::Keys::Enter) &&
		m_pBodyManager->GetBodysSize() != 0 &&
		m_pBodyManager->GetSelectBody()->GetEndMoveFlag())
	{
		//	�p���[�ɂ���Ĕ�΂�������ς���
		m_pBodyManager->SetBodyVel((modeCommonProcess->GetPower() + 0.5f) * 0.9f);

		//	��΂�
		m_pBodyManager->FryBody();

		//	��΂����t���O���I���ɂ���
		m_isFry = true;

		//	���݂̑̂̃^�C�v��ݒ肷��
		m_fryBody = m_pBodyManager->GetSelectBody();

		//	�M�~�b�N�Ɠ�����\�������邩�𔻒肵�Ă���΃J������Ǐ]
		if (IfHitGimmick())
		{
			int a = 0;
			//	�Ǐ]�t���O���I���ɂ���
			m_followFlag = true;

			//	�\���͈͂�����
		}

		//	�p���[�����Z�b�g����
		modeCommonProcess->ResetPower();
	}

	if (m_pBodyManager->GetBodysPop() && m_pBodyManager->GetSelectBody()->GetEndMoveFlag())
	{
		//	�����蔻����s�����߂̐ݒ�
		m_gimmickManager->SetHitStates(m_pBodyManager->GetSelectBody()->GetPosition(), m_pBodyManager->GetSelectBody()->GetRadius(), 1.0f);

		//	���ȂƂ̓����蔻��
		if (m_gimmickManager->HitHole() &&
			!m_pBodyManager->GetSelectBody()->GetIsHit())
		{
			//	�X�R�A�����Z���� �ς���
			m_score += m_gimmickManager->GetScore();
			//	���Z���ꂽ���Ƃ�`����
			m_scoreUI->SetScoreCheck(true);
			//	�������X�R�A��ݒ肷��
			m_scoreUI->SetInScore(m_gimmickManager->GetScore());

			//	�����������Ƃ�Body�ɒm�点��
			m_pBodyManager->GetSelectBody()->SetIsHit(true);
		}

		//	�X�R�A�{�[�h�Ƃ̓����蔻��
		if (m_gimmickManager->HitScoreBaord())
		{
			//	���Ƃ�
			m_pBodyManager->GetSelectBody()->SetVel(0.0f);
		}
	}

	
	/*if (m_pBodyManager->GetSelectBody()->GetEndMoveFlag())
	{
		m_camera->ResetCamera();
	}*/
	

	//	TODO�F�Ǐ]�t���O�ɂ���@���F���ł邩�t���O�@�m�F�F�t���O�Ȃ�
	if (m_followFlag)
	{
		//	�J������Ǐ]
		FollowCamera(m_fryBody->GetPosition());

		if (m_fryBody->GetDeleteFlag())
		{
			m_camera->ResetCamera();

			m_fryBody = nullptr;

			m_followFlag = false;
		}
	}

	//	BodyManager�X�V
	for (int i = 0; i < BODYMANAGER_NUM; i++)
	{
		m_bodyManagers[i]->Update(timer);
	}

	////	�J����
	//m_camera->Update();

	m_camera->CalculateMatrix();
	m_view = m_camera->GetViewMatrix();
	m_projection = m_camera->GetProjectionMatrix();

	//	���ȂƃX�R�A�{�[�h�̍X�V
	m_gimmickManager->Update(timer);

	//	�S���ł��؂�����ʂ̂Ɉړ�����
	if (!m_pBodyManager->GetBodysPop())
	{
		//	���̃��[�h���I������H�H
		bool isEnd = true;

		for (int i = 0; i < 3; i++)
		{
			//	�ł��؂��Ă��Ȃ����̂�T��
			if (m_bodyManagers[i]->GetBodysPop())
			{
				//	�ݒ肷��
				m_pBodyManager = m_bodyManagers[i].get();
				//	�n���}�[�̈ʒu���X�V
				GetHammer()->SetXPos(static_cast<float>(i));
				//	�܂��I���Ȃ�
				isEnd = false;
				break;
			}
		}

		//	�ł��؂��Ă��Ȃ����̂���������False�ɂȂ�̂ŃV�[���̑J�ڂ��s���Ȃ�
		m_endFlag = isEnd;
	}


	//	�X�R�A�̕\���̍X�V
	m_scoreUI->Update(timer, m_score);

	return GAME_MODE::NONE;
}

void ShootingMode::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	���̕`��
	TextureManager* textureManager = TextureManager::GetInstance();
	SimpleMath::Matrix groundWorld = SimpleMath::Matrix::Identity;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = textureManager->GetTexture(TextureManager::eTextureName::TATAMI);
	groundWorld = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -20.0f);
	m_ground->Draw(groundWorld, m_view, m_projection, DirectX::Colors::White, texture.Get());
	//	�ǂ̕`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> wallTexture = textureManager->GetTexture(TextureManager::eTextureName::WALL);
	SimpleMath::Matrix wallWorld, wallWorld2, wallWorld3 = SimpleMath::Matrix::Identity;
	//	����
	wallWorld = wallWorld2 = wallWorld3 = SimpleMath::Matrix::CreateRotationX(-90.0f / 180.0f * DirectX::XM_PI);
	wallWorld *= SimpleMath::Matrix::CreateTranslation(0.0f, 15.0f, -40.0f);
	m_wall->Draw(wallWorld, m_view, m_projection, DirectX::Colors::White, wallTexture.Get());
	//	�E
	wallWorld2 *= SimpleMath::Matrix::CreateRotationY(90.0f / 180.0f * DirectX::XM_PI);
	wallWorld2 *= SimpleMath::Matrix::CreateTranslation(25.0f, 15.0f, -20.0f);
	m_wall->Draw(wallWorld2, m_view, m_projection, DirectX::Colors::White, wallTexture.Get());
	//	��
	wallWorld3 *= SimpleMath::Matrix::CreateRotationY(-90.0f / 180.0f * DirectX::XM_PI);
	wallWorld3 *= SimpleMath::Matrix::CreateTranslation(-25.0f, 15.0f, -20.0f);
	m_wall->Draw(wallWorld3, m_view, m_projection, DirectX::Colors::White, wallTexture.Get());

	//	��������̕`��
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());
	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::OPERATION1).Get(), DirectX::SimpleMath::Vector2(0.0f, 0.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);
	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::OPERATION2).Get(), DirectX::SimpleMath::Vector2(0.0f, 50.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);
	m_spriteBatch->End();


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
	for (int i = 0; i < BODYMANAGER_NUM; i++)
	{
		m_bodyManagers[i]->Render(m_view, m_projection, m_basicEffect.get(), m_inputLayout);
	}

	//	�n���}�[�̕`��
	GetHammer()->Render(m_view, m_projection);

	//	���ȂƃX�R�A�{�[�h��`��
	m_gimmickManager->Render(m_view, m_projection);

	//	��ԕ������������̕`��
	DrawLine();

	//	�X�R�A�̕`��
	m_scoreUI->Render(m_view, m_projection);
}

void ShootingMode::Finalize()
{
	if (m_camera != nullptr)
	{
		delete(m_camera);
		m_camera = nullptr;
	}
}

bool ShootingMode::GetEndFlag()
{
	return m_endFlag;
}

//	��΂����ύX����
void ShootingMode::ChangeBpdyManagers()
{
	//	�L�[�{�[�h�g���b�J�[
	InputManager* inputMabager = InputManager::GetInstance();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputMabager->GetKeyboardTracker();

	//	�P�A�Q�A�R�L�[�Ŕ�΂����̂�ύX���違�n���}�[�̈ʒu��ύX����
	//	��΂����̂�������ΕύX�s��
	if (keyboardTracker->IsKeyPressed(Keyboard::Keys::D1) &&
		m_bodyManagers[0]->GetBodysPop())
	{
		m_pBodyManager = m_bodyManagers[0].get();
		GetHammer()->SetXPos(0);
	}
	else if (keyboardTracker->IsKeyPressed(Keyboard::Keys::D2) &&
		m_bodyManagers[1]->GetBodysPop())
	{
		m_pBodyManager = m_bodyManagers[1].get();
		GetHammer()->SetXPos(1);
	}
	else if (keyboardTracker->IsKeyPressed(Keyboard::Keys::D3) &&
		m_bodyManagers[2]->GetBodysPop())
	{
		m_pBodyManager = m_bodyManagers[2].get();
		GetHammer()->SetXPos(2);
	}
}

//	���ł����p�x�̐���`�悷��
void ShootingMode::DrawLine()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�G�t�F�N�g�̐ݒ�
	m_effect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
	m_effect->SetProjection(m_projection);
	m_effect->SetView(m_view);
	m_effect->Apply(context);

	//	�F
	DirectX::FXMVECTOR color = Colors::Red;
	
	//	�ʒu
	SimpleMath::Vector2 basicPosition = m_pBodyManager->GetPosition();
	SimpleMath::Vector3 startPos = SimpleMath::Vector3(basicPosition.x, 0.0f, basicPosition.y);
	float radian = DirectX::XMConvertToRadians(m_fryRot);
	const float length = 7.0f;
	SimpleMath::Vector3 endPos = SimpleMath::Vector3(basicPosition.x + cos(radian) * length, 0.0f, basicPosition.y + sin(radian) * length);

	//	���_
	VertexPositionColor startVerTex = VertexPositionColor{ startPos,color };
	VertexPositionColor endVerTex = VertexPositionColor{ endPos,color };
	VertexPositionColor startVerTex2 = VertexPositionColor{ startPos + SimpleMath::Vector3(1.0f,0.0f,0.0f),color };
	VertexPositionColor endVerTex2 = VertexPositionColor{ endPos + SimpleMath::Vector3(1.0f,0.0f,0.0f),color };

	//	�`��
	m_primitiveBatch->Begin();

	m_primitiveBatch->DrawLine(startVerTex, endVerTex);

	m_primitiveBatch->DrawQuad(startVerTex, endVerTex, startVerTex2, endVerTex2);

	m_primitiveBatch->End();
}


//	���ł����p�x�𑀍삷��
void ShootingMode::MoveFryRot()
{
	InputManager* inputManager = InputManager::GetInstance();
	DirectX::Keyboard::State keyboardState = inputManager->GetKeyboardState();

	//	���E�L�[�ň��l�̊p�x�ύX
	if (keyboardState.Right && m_fryRotLength < ROT_MAX)
	{
		m_fryRot += MOVE_ROT_VALUE;
		m_fryRotLength += MOVE_ROT_VALUE;
	}
	else if (keyboardState.Left && m_fryRotLength > -ROT_MAX)
	{
		m_fryRot -= MOVE_ROT_VALUE;
		m_fryRotLength -= MOVE_ROT_VALUE;
	}

	//	�p�x��ݒ肷��
	m_pBodyManager->SetBodyRot(m_fryRot);
}

bool ShootingMode::IfHitGimmick()
{
	ModeCommonProcess* modeCommonProcess = GetModeCommonProcess();

	//	�\���ʒu
	const float GRAVITY = 0.08f;
	const float JUMP = static_cast<float>(static_cast<int>(m_fryBody->GetBodyType()) + 1) * 0.3f;

	//	�ǂꂭ�炢�Ŕ��ł�����
	float time = (JUMP / GRAVITY) * 2;
	float vel = (modeCommonProcess->GetPower() + 0.5f) * 0.9f;

	//	���ł����p�x
	float radian = DirectX::XMConvertToRadians(m_fryRot);

	SimpleMath::Vector3 basePosition{ m_pBodyManager->GetPosition().x, 0.0f, m_pBodyManager->GetPosition().y };

	//	�\���ʒu
	SimpleMath::Vector3 position = SimpleMath::Vector3(cos(radian) * (vel * time), 0.0f, sin(radian) * (vel * time))
		+ basePosition;
	
	//	�\���ʒu�ƃM�~�b�N�����Ƃ̓����蔻���Ԃ�
	return m_gimmickManager->ifHit(position);
}

void ShootingMode::FollowCamera(DirectX::SimpleMath::Vector3 position)
{
	//	�J������Ǐ]������@eye��targat��ς���H�H�H
	SimpleMath::Vector3 spacePos{ 0.0f,5.0f,10.0f };
	m_camera->SetEyePosition(position + spacePos);
	m_camera->SetTargetPosition(position);
} 