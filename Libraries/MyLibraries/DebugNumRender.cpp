#include "pch.h"
#include "DeviceResources.h"

#include "DebugNumRender.h"

#include <string>

DebugNumRender::DebugNumRender()
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
}

void DebugNumRender::Render(float num, int xpos, int ypos)
{
	///	2D�n�̕`�悱������ ///
	m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_commonState->NonPremultiplied());

	//	Debug�̕`��͂�����
	m_spriteFont->DrawString(m_spriteBatch.get(), std::to_string(num).c_str(), DirectX::XMFLOAT2(20 + xpos, 30 + ypos));

	///	�����܂� ///
	m_spriteBatch->End();
}
