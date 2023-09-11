/*
�`���[�g���A���V�[��
*/
#include "pch.h"

#include "DeviceResources.h"
#include "../GameMain.h"
#include "../../Libraries/MyLibraries/DebugCamera.h"
#include "../../Libraries/MyLibraries/GridFloor.h"
#include "../TextureManager.h"

#include <WICTextureLoader.h>
#include <string>
#include <random>

#include "TutorialScene.h"

// ���O��Ԃ̗��p
using namespace DirectX::SimpleMath;
using namespace DirectX;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
TutorialScene::TutorialScene() :
	m_rot(0.0f),
	m_changeHammer(true),
	m_bodyNumber(0),
	m_hammerFlag(true),
	m_changeType(Body::eBodyType::WOOD)
{
	//	�K�v�ȕϐ��̏���
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�f�o�b�O�J�����̏���
	m_pDebugCamera = new DebugCamera();
	m_pGridFloor = new GridFloor(device, context, 10, 10);

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

	//	�n���}�[�̐���
	m_hammer = std::make_unique<Hammer>();
	m_hammer->Initialize();
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
TutorialScene::~TutorialScene()
{
	//	�|�C���^�̉��
	if (m_pDebugCamera != nullptr)
	{
		delete m_pDebugCamera;
		m_pDebugCamera = nullptr;
	}

	if (m_pGridFloor != nullptr)
	{
		delete m_pGridFloor;
		m_pGridFloor = nullptr;
	}

	m_pAdx2->Finalize();
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void TutorialScene::Initialize()
{
	//	��
	m_pAdx2 = &ADX2::GetInstance();
	m_pAdx2->Initialize("Resources/Sounds/daruma.acf", "Resources/Sounds/Sounds.acb");

	//	BGM
	m_bgmID = m_pAdx2->Play(CRI_SOUNDS_���Ȃ��݂̕��i);

	//	�K�v�ȕϐ��̏���
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�R�����X�e�[�g�FD3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	//	�e�N�X�`���}�l�[�W���̐���
	TextureManager* textureManager = TextureManager::GetInstance();

	m_tobasuTetxture = textureManager->GetTexture(TextureManager::eTextureName::TOBASU);
	m_kaeruTetxture = textureManager->GetTexture(TextureManager::eTextureName::KAERU);

	m_tetxture = m_tobasuTetxture;


	m_fryTetxture = textureManager->GetTexture(TextureManager::eTextureName::FLY);
	m_changeTetxture = textureManager->GetTexture(TextureManager::eTextureName::CHANGE);

	m_hammerTetxture = m_fryTetxture;

	//	�J�����̐ݒ�
	DirectX::SimpleMath::Vector3 eye = DirectX::SimpleMath::Vector3(21.0f, 8.0f, -8.0f);
	DirectX::SimpleMath::Vector3 target = DirectX::SimpleMath::Vector3(0.0f, -4.0f, -8.0f);
	DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f);

	//	�r���[�s��̏�����
	m_view = DirectX::SimpleMath::Matrix::CreateLookAt(eye, target, up);
	//	�v���W�F�N�V����
	m_projection = m_pDebugCamera->GetProjectionMatrix();

	for (int i = 0; i < 3; i++)
	{
		m_bodys.push_back(std::make_unique<Body>(textureManager->GetTexture(static_cast<TextureManager::eTextureName>(i)),
			2,
			static_cast<Body::eBodyType>(i),
			DirectX::SimpleMath::Vector2(3.0f,-2.0f)));

		m_bodyNum[static_cast<Body::eBodyType>(i)] = 1;
	}

	m_keyboardTracker = std::make_unique<Keyboard::KeyboardStateTracker>();
}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
GAME_SCENE TutorialScene::Update(const DX::StepTimer& timer)
{
	m_pDebugCamera->Update();

	/*m_view = m_pDebugCamera->GetViewMatrix();
	m_projection = m_pDebugCamera->GetProjectionMatrix();*/

	// �L�[���͏����擾����
	DirectX::Keyboard::State keyState = DirectX::Keyboard::Get().GetState();

	m_keyboardTracker->Update(keyState);


	if (m_bodyNumber > m_bodys.size() - 1)
	{
		m_bodyNumber = m_bodys.size() - 1;
	}
	else if (m_keyboardTracker->IsKeyPressed(Keyboard::Up) && m_bodyNumber < m_bodys.size() - 1)
	{
		m_bodyNumber++;
	}
	else if (m_keyboardTracker->IsKeyPressed(Keyboard::Down) && m_bodyNumber >= 1)
	{
		m_bodyNumber--;
	}

	if (m_keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::Enter))
	{
		return GAME_SCENE::PLAY;
	}

	int i = 1;
	for (std::unique_ptr<Body>& body : m_bodys)
	{
		body->Update(i, timer);
		i++;
	}

	if (keyState.IsKeyDown(DirectX::Keyboard::Keys::Enter))
	{
		if (m_changeHammer)
		{
			m_rot -= 0.03f;

			if (m_rot <= 0.009f)
			{
				m_changeHammer = false;
			}
		}
		else
		{
			m_rot += 0.03f;

			if (m_rot >= 1.0f)
			{
				m_changeHammer = true;
			}
		}
	}

	m_hammer->SetYPos(m_bodyNumber + 0.5f);
	m_hammer->SetRot(m_rot * 40);


	//	�C�e���[�^
	std::list<std::unique_ptr<Body>>::iterator it = m_bodys.begin();

	// �C�e���[�^�𓮂���
	std::list<std::unique_ptr<Body>>::iterator ite = std::next(it, m_bodyNumber);

	SetState((*ite)->GetBodyType());


	//	�n���}�[�̏�Ԃ�ύX����
	if (m_keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::R))
	{
		if (m_hammerFlag)
		{
			m_tetxture = m_kaeruTetxture;
			m_hammerTetxture = m_changeTetxture;
			m_hammerFlag = false;
		}
		else
		{
			m_tetxture = m_tobasuTetxture;
			m_hammerTetxture = m_fryTetxture;
			m_hammerFlag = true;
		}

		m_hammer->ChangeState();
	}

	m_hammer->Update();

	//	�ς���̂�ς���
	if (m_keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::Q))
	{
		m_changeType = Body::eBodyType::WOOD;
	}
	else if (m_keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::W))
	{
		m_changeType = Body::eBodyType::BRICK;
	}
	else if (m_keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::E))
	{
		m_changeType = Body::eBodyType::METAL;
	}

	TextureManager* textureManager = TextureManager::GetInstance();


	if (m_keyboardTracker->IsKeyReleased(Keyboard::Enter)&&
		m_hitPowerMax >= m_rot &&
		m_hitPowerMin <= m_rot &&
		(*it)->GetEndMoveFlag())
	{
		if (m_hammerFlag)
		{
			if ((*ite)->GetMoveFlag() == false)
			{
				(*ite)->SetVel(m_vel);

				//	�̂𓮂���
				(*ite)->SetMoveFlag(true);
			}
		}
		else
		{
			//	 �ύX	//
				//	�e�N�X�`��
			int num = static_cast<int>(m_changeType);

			(*ite)->SetTexture(textureManager->GetTexture(static_cast<TextureManager::eTextureName>(num)));
			//	�^�C�v
			(*ite)->SetBodyType(m_changeType);
		}

		m_rot = 0.0f;
	}

	for (int j = 0; j < 3; j++)
	{
		if (m_bodyNum[static_cast<Body::eBodyType>(j)] == 0)
		{
			m_bodys.push_back(std::make_unique<Body>(textureManager->GetTexture(static_cast<TextureManager::eTextureName>(j)),
				m_weit,
				static_cast<Body::eBodyType>(j)));

			m_bodyNum[static_cast<Body::eBodyType>(j)] = 1;
		}
	}



	//	�C�e���[�^
	std::list<std::unique_ptr<Body>>::iterator itr = m_bodys.begin();

	//	����
	while (itr != m_bodys.end())
	{
		if ((*itr)->GetDeleteFlag())
		{
			m_bodyNum[(*itr)->GetBodyType()] = 0;

			itr = m_bodys.erase(itr);
		}
		else
		{
			++itr;
		}
	}


	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void TutorialScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	m_pGridFloor->Draw(context, m_commonState.get(), m_view, m_projection);

	Matrix world = Matrix::Identity;

	//	iro
	DirectX::SimpleMath::Color lightColor = DirectX::SimpleMath::Color(0.98f, 0.98f, 0.98f);
	DirectX::SimpleMath::Vector3 lightDirection = DirectX::SimpleMath::Vector3(-1.0f, -1.0f, 0.2f);

	m_hammer->Render(m_view, m_projection);

	TextureManager* textureManager = TextureManager::GetInstance();

	///	2D�n�̕`�悱������ ///
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//	�Q�[�W
	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::GAUGE_BASE).Get(), DirectX::SimpleMath::Vector2(40.0f, 20.0f), nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	//	�L�k����Q�[�W
	float pos = m_rot;
	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::GAUGE).Get(), DirectX::SimpleMath::Vector2(70.0f, 20.0f + 464.0f), nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 464.0f), DirectX::SimpleMath::Vector2(0.5f, pos));

	//	�Q�[�W�͈̔�
	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::GAUAGE_SCOPE).Get(), m_scopePos, nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f), DirectX::SimpleMath::Vector2(0.5f, 1.0f));

	m_spriteBatch->Draw(m_tetxture.Get(), Vector2(0.0f,0.0f), nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f), DirectX::SimpleMath::Vector2(1.0f, 1.0f));

	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::CHANGE_BODY).Get(), Vector2(600.0f,100.0f), nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f),0.5f);

	m_spriteBatch->Draw(m_hammerTetxture.Get(), Vector2(1000.0f, 100.0f), nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f), 0.25f);


	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::SPACE).Get(), Vector2(0.0f,-50.0f), nullptr, Colors::White, 0.0f, DirectX::SimpleMath::Vector2(0.0f, 0.0f), 1.5f);


	//	Debug�̕`��͂����� //
	//m_spriteFont->DrawString(m_spriteBatch.get(), std::to_string(/* �ϐ��� */).c_str(), DirectX::XMFLOAT2(10, 30));

	///	�����܂� ///
	m_spriteBatch->End();




	// �G�t�F�N�g�Ƀ��C�g�̌��ʂ�ݒ肷��
	m_basicEffect->SetLightEnabled(0, true); // ���C�g 0 �Ԃ��g�p

	m_basicEffect->SetLightDiffuseColor(0, lightColor); // ���C�g�̐F���w��

	m_basicEffect->SetLightDirection(0, lightDirection); // ���C�g�̌������w��

	// �G�t�F�N�g�Ɋe��ϊ��s��̐ݒ� -> �ݒ肪�}�Y���ƌ����ڂ��������Ȃ�Ȃ�
	m_basicEffect->SetWorld(world); // ���[���h�s���ݒ�
	m_basicEffect->SetView(m_view); // �r���[�s��̐ݒ�
	m_basicEffect->SetProjection(m_projection); // �ˉe�s��̐ݒ�
	m_basicEffect->Apply(context); // �G�t�F�N�g�̔��f



	for (std::unique_ptr<Body>& body : m_bodys)
	{
		body->Render(m_view, m_projection, m_basicEffect.get(), m_inputLayout);
	}

}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void TutorialScene::Finalize()
{
}

void TutorialScene::SetState(Body::eBodyType type)
{
	switch (type)
	{
	case Body::eBodyType::WOOD:
		m_scopePos = DirectX::SimpleMath::Vector2(40.0f, 300.0f);
		m_hitPowerMax = 0.4f;
		m_hitPowerMin = 0.1f;
		m_vel = 6.0f;
		m_weit = 2;
		break;
	case Body::eBodyType::BRICK:
		m_scopePos = DirectX::SimpleMath::Vector2(40.0f, 160.0f);
		m_hitPowerMax = 0.7f;
		m_hitPowerMin = 0.4f;
		m_vel = 4.0;
		m_weit = 6;
		break;
	case Body::eBodyType::METAL:
		m_scopePos = DirectX::SimpleMath::Vector2(40.0f, 20.0f);
		m_hitPowerMax = 1.0f;
		m_hitPowerMin = 0.7f;
		m_vel = 2.0f;
		m_weit = 9.0f;
		break;
	case Body::eBodyType::HEAD:
		m_scopePos = DirectX::SimpleMath::Vector2(40.0f, 0.0f);
		m_vel = 0.0f;
		m_weit = 0.0f;
		break;
	}
}
