//
// Scene.cpp
//
#include "pch.h"

#include "DeviceResources.h"
#include "GameMain.h"

#include "InputManager.h"
#include "TextureManager.h"

#include "Libraries/MyLibraries/DebugCamera.h"
#include "Libraries/MyLibraries/GridFloor.h"

#include "Game/TitleScene/TitleScene.h"
#include "Game/PlayScene/PlayScene.h"
#include "Game/ResultScene/ResultScene.h"

//	�O���[�o���ϐ��̏�����
CLEAR_FLAG g_clearFlag = CLEAR_FLAG::SHOOTING;
GAME_MODE g_gameMode = GAME_MODE::FLY_SPECIFIC;
int g_score = 0;

// ���O��Ԃ̗��p
using namespace DirectX;

//-------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------
GameMain::GameMain()
	: m_nextScene(GAME_SCENE::TITLE)		// �����V�[���̐ݒ�
	, m_pScene(nullptr)
{
}

//-------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------
GameMain::~GameMain()
{
	Finalize();
}

//-------------------------------------------------------------------
// ����������
//-------------------------------------------------------------------
void GameMain::Initialize()
{
	InputManager* inputManager = InputManager::GetInstance();
	inputManager->Initialize();

	TextureManager* textureManager = TextureManager::GetInstance();
	textureManager->CreateTexture();

	// �V�[���쐬
	CreateScene();
}

//-------------------------------------------------------------------
// �X�V����
//-------------------------------------------------------------------
void GameMain::Update(const DX::StepTimer& timer)
{
	InputManager* inputManager = InputManager::GetInstance();
	//inputManager->Update();

	// ESC�L�[�ŏI��
	if (inputManager->GetKeyboardState().Escape)
	{
		PostQuitMessage(0);
	}

	// ���̃V�[�����ݒ肳��Ă�����V�[���؂�ւ�
	if (m_nextScene != GAME_SCENE::NONE)
	{
		// �V�[���폜
		DeleteScene();
		
		// �V�[���쐬
		CreateScene();
	}

	// ���Ԃ�����΍X�V
	if (m_pScene != nullptr)
	{
		m_nextScene = m_pScene->Update(timer);
	}
}

//-------------------------------------------------------------------
// �`�揈��
//-------------------------------------------------------------------
void GameMain::Render()
{
	// ���Ԃ�����Ε`��
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}
}

//-------------------------------------------------------------------
// ��n��
//-------------------------------------------------------------------
void GameMain::Finalize()
{
	DeleteScene();
}

/*--------------------------------------------------
�V�[���̍쐬
--------------------------------------------------*/
void GameMain::CreateScene()
{
	// �V�[�����쐬����Ă���Ƃ��͏������Ȃ�
	if (m_pScene != nullptr)
	{
		return;
	}

	// ���V�[���̍쐬	�V�[���𑝂₵�����ꍇ�����ɂ����₷
	switch (m_nextScene)
	{
	case GAME_SCENE::TITLE:
	{
		m_pScene = new TitleScene();
		break;
	}
	case GAME_SCENE::PLAY:
	{
		m_pScene = new PlayScene();
		break;
	}
	case GAME_SCENE::RESULT:
	{
		m_pScene = new ResultScene();
		break;
	}
	default:
	{
		// ��O�Ȃ̂ŏ����𒆒f
		return;
	}
	}

	// �쐬�����V�[����������
	m_pScene->Initialize();
}

/*--------------------------------------------------
�V�[���̍폜
--------------------------------------------------*/
void GameMain::DeleteScene()
{
	// �V�[�����쐬����Ă��Ȃ���Ώ������Ȃ�
	if (m_pScene == nullptr)
	{
		return;
	}

	// ���V�[���̏I������
	m_pScene->Finalize();

	// ���V�[���̍폜
	delete m_pScene;
	m_pScene = nullptr;
}
