#pragma once

#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

class DebugNumRender final
{
public:
	// �C���X�^���X��Ԃ�
	static DebugNumRender* GetInstance()
	{
		static DebugNumRender instance;
		return &instance;
	}

private:
	//	�R�����X�e�[�g
	std::unique_ptr<DirectX::CommonStates>		m_commonState;
	// �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch>		m_spriteBatch;
	// �X�v���C�g�t�H���g
	std::unique_ptr<DirectX::SpriteFont>		m_spriteFont;

public:
	DebugNumRender();
	~DebugNumRender() = default;

	void Render(float num, int xpos, int ypos);
};