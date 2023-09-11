/*
�`���[�g���A���V�[��

*/
#pragma once

#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <Effects.h>

#include "../IScene.h"
#include "Game/GameMain.h"

//	�I�u�W�F�N�g
#include "../PlayScene/Object/BodyManager.h"		//	�̃}�l�[�W��
#include "../PlayScene/Object/Hammer.h"			//	�n���}�[

#include "../PlayScene/Object/Head.h"			//	��

#include "../PlayScene/Object/Stage/Stage.h"		//	�X�e�[�W

//	UI�n
#include "../PlayScene/UI/MoveNumberUI.h"
#include "../PlayScene/UI/UIManager.h"
#include "../PlayScene/UI/HintUI.h"

//	��
#include "Libraries/MyLibraries/ADX2/Adx2.h"
#include "Resources/Sounds/Sounds.h"
#include "Resources/Sounds/daruma_acf.h"

//	�e�N�X�`���}�l�[�W��
#include "../TextureManager.h"

class Camera;
class GridFloor;

class TutorialScene : public IScene
{
private:
	ADX2* m_pAdx2;

	int m_bgmID;

	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>		m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch>		m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont>		m_spriteFont;

	float m_weit;

	//	�f�o�b�O�J����
	DebugCamera* m_pDebugCamera;
	GridFloor* m_pGridFloor;

	//	�r���[�ƃv���W�F�N�V����
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_projection;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tobasuTetxture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_kaeruTetxture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_tetxture;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_fryTetxture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_changeTetxture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_hammerTetxture;


	//	�Q�[�W�͈̔͂̈ʒu
	DirectX::SimpleMath::Vector2 m_scopePos;
	float m_hitPowerMin;
	float m_hitPowerMax;

	float m_vel;

	//	�n���}�[
	std::unique_ptr<Hammer> m_hammer;

	//	�L�[�{�[�h�g���b�J�[
	std::unique_ptr< DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;

	//	UI�n
	std::unique_ptr<UIManager> m_uiManager;

	//	��
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;


	std::list<std::unique_ptr<Body>> m_bodys;

	float m_rot;

	bool m_changeHammer;
	bool m_hammerFlag;

	int m_bodyNumber;

	Body::eBodyType m_changeType;

	std::map<Body::eBodyType, int> m_bodyNum;
public:

	// �R���X�g���N�^
	TutorialScene();

	// �f�X�g���N�^
	~TutorialScene();

	// ������
	void Initialize() override;

	// �X�V
	GAME_SCENE Update(const DX::StepTimer& timer) override;

	// �`��
	void Draw() override;

	// �I������
	void Finalize() override;

	void SetState(Body::eBodyType type);
};