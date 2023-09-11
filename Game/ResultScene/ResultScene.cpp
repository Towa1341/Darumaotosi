/*
���U���g�V�[��
*/

#include "pch.h"

#include <WICTextureLoader.h>
#include <SimpleMath.h>

#include "../GameMain.h"
#include "DeviceResources.h"

#include "../InputManager.h"
#include "../TextureManager.h"

#include "ResultScene.h"


using namespace DirectX;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
ResultScene::ResultScene():
	m_texture(nullptr),
	m_headPos(-1.1f)
{
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
ResultScene::~ResultScene()
{
	m_pAdx2->Finalize();
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void ResultScene::Initialize()
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

	TextureManager* textureManaegr = TextureManager::GetInstance();

	//	�N���A�t���O�ɂ���ĕ\������w�i�摜��ς���
	switch (g_clearFlag)
	{
	case CLEAR_FLAG::GOOD:
		m_texture = textureManaegr->GetTexture(TextureManager::eTextureName::GOOD_END);
		break;

	case CLEAR_FLAG::NORMAL:
		m_texture = textureManaegr->GetTexture(TextureManager::eTextureName::NORMAL_END);
		break;

	case CLEAR_FLAG::BAD:
		m_texture = textureManaegr->GetTexture(TextureManager::eTextureName::BAD_END);
		break;
		
	case CLEAR_FLAG::SHOOTING:
		m_texture = textureManaegr->GetTexture(TextureManager::eTextureName::SHOOTING_END);
		break;
	}

	//	��
	m_pAdx2 = &ADX2::GetInstance();
	m_pAdx2->Initialize("Resources/Sounds/daruma.acf", "Resources/Sounds/Sounds.acb");

	//	BGM
	m_bgmID = m_pAdx2->Play(CRI_SOUNDS_���R�[�_�[����);


	//	�e���f���̐ݒ�E�iTODO�F�摜�Ɠ����悤�ɃN���A�t���O�œǂݍ��ނ̕ς���j
	//	����S���ǂݍ��ނ̗ǂ��Ȃ�����N���A�t���O�ɍ��킹�ĂP�����ǂݍ���

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
		L"Resources/Models/nunnHead.cmo",
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
		L"Resources/Models/palaHead.cmo",
		*factory
	));
	delete factory;
}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
GAME_SCENE ResultScene::Update(const DX::StepTimer& timer)
{
	InputManager* inputManager = InputManager::GetInstance();
	inputManager->Update();

	//	�G���^�[�L�[�Ń^�C�g���ɖ߂�
	if (inputManager->GetKeyboardTracker()->IsKeyPressed(Keyboard::Keys::Enter))
	{
		m_pAdx2->Play(CRI_SOUNDS_MOUSE);

		return GAME_SCENE::TITLE;
	}

	//	�����ړ�������
	m_headPos += 0.01f;
	//	��ʉE�[�ɍs�����獶�[�Ɉړ�
	if (m_headPos >= 1.5f)
	{
		m_headPos = -1.5f;
	}

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void ResultScene::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	DirectX::SimpleMath::Matrix world, view, projection, lightWorld;
	world = view = projection = lightWorld = DirectX::SimpleMath::Matrix::Identity;


	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//	�w�i�摜�̕`��
	m_spriteBatch->Draw(m_texture.Get(), SimpleMath::Vector2::Zero);

	//	�����V���[�e�B���O��������X�R�A��\������
	if (g_clearFlag == CLEAR_FLAG::SHOOTING)
	{
		//	�e�N�X�`���̎擾
		TextureManager* textureManager = TextureManager::GetInstance();
		const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> numberTexture = textureManager->GetTexture(TextureManager::eTextureName::SCORE_NUMBER);

		//	�\������ꏊ
		const SimpleMath::Vector2 position(570.0f, 370.0f);
		const SimpleMath::Vector2 spacePosition(60.0f, 0.0f);

		//	�摜�̐؂���ʒu
		//	10�̈�
		float catPosition1 = 32.0f * (g_score / 10);
		RECT rect1 = { 0.0f,catPosition1,32.0f,catPosition1 + 32.0f };
		//	1�̈�
		float catPosition2 = 32.0f * (g_score % 10);
		RECT rect2 = { 0.0f,catPosition2,32.0f,catPosition2 + 32.0f };

		//	�`��
		m_spriteBatch->Draw(numberTexture.Get(), position, &rect1, Colors::White, 0.0f, SimpleMath::Vector2::Zero, 3.0f);
		m_spriteBatch->Draw(numberTexture.Get(), position + spacePosition, &rect2, Colors::White, 0.0f, SimpleMath::Vector2::Zero, 3.0f);
	}

	m_spriteBatch->End();


	//	�����Ă铪�̕`��
	world = SimpleMath::Matrix::CreateScale(0.25f, 0.5f, 0.25f);
	world *= SimpleMath::Matrix::CreateRotationY(95.5f) / 180.0f * XM_PI;
	world *= SimpleMath::Matrix::CreateTranslation(m_headPos, -1.0f, 0.0f);

	int modelNumber = static_cast<int>(g_clearFlag) - 1;
	m_pModels[modelNumber]->Draw(context, *m_commonState.get(), world, view, projection);
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void ResultScene::Finalize()
{
}
