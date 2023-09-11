#pragma once

#include <map>

class TextureManager final
{
public:
	// �C���X�^���X��Ԃ�
	static TextureManager* GetInstance()
	{
		static TextureManager instance;
		return &instance;
	}

public:
	enum class eTextureName
	{
		METAL,
		BRICK,
		WOOD,
		NUMBER,
		SCORE_NUMBER,
		END,
		GAUGE_BASE,
		GAUGE,
		GAUAGE_SCOPE,
		TITLE,
		CRI,
		OTO_LOGIC,
		CHANGE_BODY,
		NEXT_BODY,
		NOW,
		CHANGE,
		FLY,
		UI_INTERFACE,
		BIKKURI,
		MIHONN,
		WOOD_BODY,
		BRICK_BODY,
		METAL_BODY,
		TOBASU,
		KAERU,
		SPACE,
		NORMAL_END,
		GOOD_END,
		BAD_END,
		SHOOTING_END,
		MODE_SHOOTING,
		MODE_CHANGE,
		MODE_FLY,
		TATAMI,
		WALL,
		YAZIRUSI1,
		YAZIRUSI2,
		OPERATION1,
		OPERATION2,

		NONE
	};

private:
	std::map<eTextureName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;

public:
	void CreateTexture();

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture(eTextureName textureName) { return m_textures[textureName]; }

	//	�֐�
private:
	//	�R���X�g���N�^�A�f�X�g���N�^��private
	TextureManager();
	~TextureManager() = default;

	// �R�s�[�R���X�g���N�^�A���[�u�R���X�g���N�^�A�֌W���������Z�q�̍폜
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager(const TextureManager&&) = delete;
	TextureManager&& operator=(const TextureManager&&) = delete;

};