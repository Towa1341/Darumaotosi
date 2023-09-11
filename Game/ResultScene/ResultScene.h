/*
���U���g�V�[��
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>
#include <Effects.h>

#include <Model.h>

#include "../IScene.h"
#include "Game/GameMain.h"	

//	��
#include "Libraries/MyLibraries/ADX2/Adx2.h"
#include "Resources/Sounds/Sounds.h"
#include "Resources/Sounds/daruma_acf.h"



class ResultScene : public IScene
{
private:
	ADX2* m_pAdx2;
	int m_bgmID;


	// �R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates> m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;


	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;	

	//	���f��
	std::vector<std::unique_ptr<DirectX::Model>> m_pModels;

	float m_headPos;

public:

	// �R���X�g���N�^
	ResultScene();

	// �f�X�g���N�^
	~ResultScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;


	////	���[�h���Ƃɕς��悤
	//GAME_SCENE NormalUpdate(const DX::StepTimer& timer);
	//void NormalDraw();

	//GAME_SCENE ShootingUpdate(const DX::StepTimer& timer);
	//void ShootingNormalDraw();
};