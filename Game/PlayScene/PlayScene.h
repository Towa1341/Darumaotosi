/*
�v���C�V�[��
*/
#pragma once

#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <Effects.h>

#include "../IScene.h"
#include "Game/GameMain.h"

//	��
#include "Libraries/MyLibraries/ADX2/Adx2.h"
#include "Resources/Sounds/Sounds.h"
#include "Resources/Sounds/daruma_acf.h"

class Camera;
class IMode;

class PlayScene : public IScene
{
private:
	//	��
	ADX2* m_pAdx2;
	int m_bgmID;

	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>		m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch>		m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont>		m_spriteFont;

	//	���̃Q�[�����[�h
	GAME_MODE m_nextGameMode;

	//	���݂̃Q�[�����[�h
	IMode* m_pMode;

	//	�r���[�ƃv���W�F�N�V����
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

public:

	// �R���X�g���N�^
	PlayScene();

	// �f�X�g���N�^
	~PlayScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	//	�ǉ�

	//	���[�h�̐���
	void CreateMode();

	//	���[�h�̍폜
	void DeleteMode();
};