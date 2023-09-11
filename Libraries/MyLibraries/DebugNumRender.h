#pragma once

#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>

class DebugNumRender final
{
public:
	// インスタンスを返す
	static DebugNumRender* GetInstance()
	{
		static DebugNumRender instance;
		return &instance;
	}

private:
	//	コモンステート
	std::unique_ptr<DirectX::CommonStates>		m_commonState;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch>		m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont>		m_spriteFont;

public:
	DebugNumRender();
	~DebugNumRender() = default;

	void Render(float num, int xpos, int ypos);
};