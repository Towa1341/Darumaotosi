/*
�v���C�V�[��
*/
#include "pch.h"

#include "DeviceResources.h"
#include "../GameMain.h"

#include "../InputManager.h"

#include "PlayScene.h"

#include "Modes/IMode.h"
#include "Modes/FlySpecificMode.h"
#include "Modes/FlyExceptingMode.h"
#include "Modes/ShootingMode.h"

#include <string>

// ���O��Ԃ̗��p
using namespace DirectX::SimpleMath;
using namespace DirectX;

/*--------------------------------------------------
�R���X�g���N�^
--------------------------------------------------*/
PlayScene::PlayScene() :
	m_nextGameMode(g_gameMode),
	m_pMode(nullptr)
{
}

/*--------------------------------------------------
�f�X�g���N�^
--------------------------------------------------*/
PlayScene::~PlayScene()
{
	m_pAdx2->Finalize();
	Finalize();
}

/*--------------------------------------------------
������
--------------------------------------------------*/
void PlayScene::Initialize()
{
	//	�K�v�ȕϐ��̏���
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	�R�����X�e�[�g�FD3D�����_�����O��ԃI�u�W�F�N�g
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	// �X�v���C�g�o�b�`::�f�o�b�O���̕\���ɕK�v
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Fonts/SegoeUI_18.spritefont");

	//	��
	m_pAdx2 = &ADX2::GetInstance();
	m_pAdx2->Initialize("Resources/Sounds/daruma.acf", "Resources/Sounds/Sounds.acb");

	//	BGM
	m_bgmID = m_pAdx2->Play(CRI_SOUNDS_���Ȃ��݂̕��i);
}

/*--------------------------------------------------
�X�V
�߂�l	:���̃V�[���ԍ�
--------------------------------------------------*/
GAME_SCENE PlayScene::Update(const DX::StepTimer& timer)
{
	InputManager* inputManager = InputManager::GetInstance();
	inputManager->Update();

	// ���̃V�[�����ݒ肳��Ă�����V�[���؂�ւ�
	if (m_nextGameMode != GAME_MODE::NONE)
	{
		// �V�[���폜
		DeleteMode();

		// �V�[���쐬
		CreateMode();
	}

	// ���Ԃ�����΍X�V
	if (m_pMode != nullptr)
	{
		m_nextGameMode = m_pMode->Update(timer);

		//	�I���t���O���������烊�U���g��
		if (m_pMode->GetEndFlag())
		{
			return GAME_SCENE::RESULT;
		}
	}

	return GAME_SCENE::NONE;
}

/*--------------------------------------------------
�`��
--------------------------------------------------*/
void PlayScene::Draw()
{
	///	2D�n�̕`�悱������ ///
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//	Debug�̕`��͂�����
	//	m_spriteFont->DrawString(m_spriteBatch.get(), std::to_string(/* �ϐ��� */).c_str(), DirectX::XMFLOAT2(10, 30));

	///	�����܂� ///
	m_spriteBatch->End();
	
	// ���Ԃ�����Ε`��
	if (m_pMode != nullptr)
	{
		m_pMode->Draw();
	}
}

/*--------------------------------------------------
�I������
--------------------------------------------------*/
void PlayScene::Finalize()
{
	DeleteMode();
}

void PlayScene::CreateMode()
{
	// �V�[�����쐬����Ă���Ƃ��͏������Ȃ�
	if (m_pMode != nullptr)
	{
		return;
	}

	// ���V�[���̍쐬	�V�[���𑝂₵�����ꍇ�����ɂ����₷
	switch (m_nextGameMode)
	{
	case GAME_MODE::FLY_SPECIFIC:
	{
		m_pMode = new FlySpecificMode();
		break;
	}
	/*case GAME_MODE::FLY_EXEPTING:
	{
		m_pMode = new FlyExceptingMode();
		break;
	}*/
	case GAME_MODE::SHOOTING:
	{
		m_pMode = new ShootingMode();
		break;
	}
	default:
	{
		// ��O�Ȃ̂ŏ����𒆒f
		return;
	}

	}

	// �쐬�����V�[����������
	m_pMode->Initialize();
}

void PlayScene::DeleteMode()
{
	// �V�[�����쐬����Ă��Ȃ���Ώ������Ȃ�
	if (m_pMode == nullptr)
	{
		return;
	}

	// ���V�[���̏I������
	m_pMode->Finalize();

	// ���V�[���̍폜
	delete m_pMode;
	m_pMode = nullptr;
}