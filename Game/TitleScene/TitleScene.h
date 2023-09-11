/*
�^�C�g���V�[��
*/
#pragma once

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <CommonStates.h>

#include <Model.h>
#include <Effects.h>
#include <VertexTypes.h>
#include <map>

#include "../IScene.h"
#include "Game/GameMain.h"	

//	��
#include "Libraries/MyLibraries/ADX2/Adx2.h"
#include "Resources/Sounds/Sounds.h"
#include "Resources/Sounds/daruma_acf.h"

class TitleScene : public IScene
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

	//	��
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	//	���f��
	std::vector<std::unique_ptr<DirectX::Model>> m_pModels;

	float m_rot;
	int m_renderNum;

	int m_modeNum; 
	std::map<GAME_MODE, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_modeTextures;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_leftYazirusiTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_rightYazirusiTexture;


public:

	// �R���X�g���N�^
	TitleScene();

	// �f�X�g���N�^
	~TitleScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	void ModelDraw();
};