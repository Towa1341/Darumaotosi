/*
�^�C�g���V�[��
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "../InputManager.h"
#include "../TextureManager.h"

#include "TitleScene.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
TitleScene::TitleScene() :
	m_rot(0.0f),
	m_renderNum(0),
	m_modeNum(0)
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();

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

	//	�X�R�A�̏�����
	g_score = 0;
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
TitleScene::~TitleScene()
{
	m_pAdx2->Finalize();
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void TitleScene::Initialize()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();

	// D3D�f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̎擾
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	// �R�����X�e�[�g::D3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	//	��
	m_pAdx2 = &ADX2::GetInstance();
	m_pAdx2->Initialize("Resources/Sounds/daruma.acf", "Resources/Sounds/Sounds.acb");

	//	BGM
	m_bgmID = m_pAdx2->Play(CRI_SOUNDS_���R�[�_�[�r�[�g);



	// �G�t�F�N�g�t�@�N�g���̍쐬
	EffectFactory* factory = new EffectFactory(pDR->GetD3DDevice());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��
	factory->SetDirectory(L"Resources/Models");
	// �t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModels.push_back(DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/palaHead.cmo",
		*factory
	));
	delete factory;

	factory = new EffectFactory(pDR->GetD3DDevice());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��
	factory->SetDirectory(L"Resources/Models");
	// �t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModels.push_back(DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/syobonnHead.cmo",
		*factory
	));
	delete factory;

	factory = new EffectFactory(pDR->GetD3DDevice());
	// �e�N�X�`���̓ǂݍ��݃p�X�w��
	factory->SetDirectory(L"Resources/Models");
	// �t�@�C�����w�肵�ă��f���f�[�^�ǂݍ���
	m_pModels.push_back(DirectX::Model::CreateFromCMO(
		pDR->GetD3DDevice(),
		L"Resources/Models/nunnHead.cmo",
		*factory
	));
	delete factory;

	TextureManager* textureManager = TextureManager::GetInstance();
	//	���[�h���Ƃ̉摜�ݒ�
	m_modeTextures[GAME_MODE::SHOOTING] = textureManager->GetTexture(TextureManager::eTextureName::MODE_SHOOTING);
	m_modeTextures[GAME_MODE::FLY_SPECIFIC] = textureManager->GetTexture(TextureManager::eTextureName::MODE_FLY);
	//m_modeTextures[GAME_MODE::FLY_EXEPTING] = textureManager->GetTexture(TextureManager::eTextureName::MODE_CHANGE);

	m_leftYazirusiTexture = textureManager->GetTexture(TextureManager::eTextureName::YAZIRUSI2);
	m_rightYazirusiTexture = textureManager->GetTexture(TextureManager::eTextureName::YAZIRUSI2);
}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
GAME_SCENE TitleScene::Update(const DX::StepTimer& timer)
{
	InputManager* inputManager = InputManager::GetInstance();
	inputManager->Update();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputManager->GetKeyboardTracker();
	TextureManager* texturemanager = TextureManager::GetInstance();

	//	��]���Ă铪�̉�]�Ɛؑ�
	m_rot++;
	if (m_rot >= 360.0f)
	{
		if (m_renderNum == 2)
		{
			m_renderNum = 0;
		}
		else
		{
			m_renderNum++;
		}

		m_rot = 0.0f;
	}

	//	���[�h�ؑցi���j
	if (keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		int num = m_modeNum - 1;

		if (num < 0)
		{
			m_modeNum = static_cast<int>(GAME_MODE::NONE) - 2;
		}
		else
		{
			m_modeNum = num;
		}
		
		//	���̐F��ς���
		m_leftYazirusiTexture = texturemanager->GetTexture(TextureManager::eTextureName::YAZIRUSI1);
	}

	if (keyboardTracker->IsKeyReleased(DirectX::Keyboard::Keys::Left))
	{
		//	���̐F��߂�
		m_leftYazirusiTexture = texturemanager->GetTexture(TextureManager::eTextureName::YAZIRUSI2);
	}

	//	���[�h�ؑցi�E�j
	if (keyboardTracker->IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		int num = m_modeNum + 1;

		if (num >= static_cast<int>(GAME_MODE::NONE))
		{
			m_modeNum = 0;
		}
		else
		{
			m_modeNum = num;
		}
		//	���̐F��ς���
		m_rightYazirusiTexture = texturemanager->GetTexture(TextureManager::eTextureName::YAZIRUSI1);
	}
	
	if (keyboardTracker->IsKeyReleased(DirectX::Keyboard::Keys::Right))
	{
		//	���̐F��߂�
		m_rightYazirusiTexture = texturemanager->GetTexture(TextureManager::eTextureName::YAZIRUSI2);
	}

	//	�V�[���؂�ւ�
	if (inputManager->GetKeyboardTracker()->IsKeyPressed(Keyboard::Keys::Enter))
	{
		m_pAdx2->Play(CRI_SOUNDS_MOUSE);

		g_gameMode = static_cast<GAME_MODE>(m_modeNum);

		return GAME_SCENE::PLAY;
	}

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void TitleScene::Draw()
{
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	TextureManager* textureManager = TextureManager::GetInstance();

	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::TITLE).Get(), DirectX::SimpleMath::Vector2(0.0f, -20.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, 1.5f);

	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::CRI).Get(), DirectX::SimpleMath::Vector2(0.0f, 0.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, 0.25f);

	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::OTO_LOGIC).Get(), DirectX::SimpleMath::Vector2(0.0f, 650.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	m_spriteBatch->Draw(m_modeTextures[static_cast<GAME_MODE>(m_modeNum)].Get(), DirectX::SimpleMath::Vector2(540.0f, 270.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero, 2.0f);

	m_spriteBatch->Draw(m_leftYazirusiTexture.Get(), DirectX::SimpleMath::Vector2(540.0f, 430.0f), nullptr, DirectX::Colors::White, 180.0f / 180.0f * DirectX::XM_PI, DirectX::SimpleMath::Vector2::Zero);
	m_spriteBatch->Draw(m_rightYazirusiTexture.Get(), DirectX::SimpleMath::Vector2(740.0f, 330.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);

	m_spriteBatch->End();

	ModelDraw();
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void TitleScene::Finalize()
{
}

void TitleScene::ModelDraw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	Matrix world, view, projection, lightWorld;
	world = view = projection = lightWorld = Matrix::Identity;

	world = Matrix::CreateScale(0.25f, 0.5f, 0.25f);
	world *= Matrix::CreateRotationY((m_rot - 90.0f) / 180.0f * XM_PI);
	world *= Matrix::CreateTranslation(0.8f, -1.0f, 0.0f);

	//	iro
	DirectX::SimpleMath::Color lightColor = DirectX::SimpleMath::Color(0.25f, 0.25f, 0.25f);
	DirectX::SimpleMath::Vector3 lightDirection = DirectX::SimpleMath::Vector3(0.f, 0.0f, -4.0f);

	m_pModels[m_renderNum]->UpdateEffects([&](IEffect* effect)
		{
			IEffectLights* lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				// �G�t�F�N�g�Ƀ��C�g�̌��ʂ�ݒ肷��
				lights->SetLightEnabled(0, true); // ���C�g 0 �Ԃ��g�p

				lights->SetLightDiffuseColor(0, lightColor); // ���C�g�̐F���w��

				lights->SetLightDirection(0, lightDirection); // ���C�g�̌������w��


			}
			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// �G�t�F�N�g�Ɋe��ϊ��s��̐ݒ� -> �ݒ肪�}�Y���ƌ����ڂ��������Ȃ�Ȃ�
				basicEffect->SetWorld(lightWorld); // ���[���h�s���ݒ�
				basicEffect->SetView(view); // �r���[�s��̐ݒ�
				basicEffect->SetProjection(projection); // �ˉe�s��̐ݒ�
				basicEffect->Apply(context); // �G�t�F�N�g�̔��f
			}
		});

	m_pModels[m_renderNum]->Draw(context, *m_commonState.get(), world, view, projection);

}
