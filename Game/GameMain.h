//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"


// �V�[���̗�
enum class GAME_SCENE : int
{
    NONE,

    TITLE,
    PLAY,
    RESULT,
    //  �V�[���𑝂₷�Ƃ��ɑ��₷
};

//  �N���A����
enum class CLEAR_FLAG
{
    NONE,

    GOOD,   //  �ǂ�
    NORMAL, //  ����
    BAD,    //�@����

    SHOOTING,   //�@�V���[�e�B���O���[�h�p
};

//	���[�h�̗�
enum class GAME_MODE
{
    FLY_SPECIFIC,	//	����̃}�e���A��������΂����[�h
    //FLY_EXEPTING,	//	����̃}�e���A���ȊO���΂����[�h

    SHOOTING,		//	�w��̏ꏊ�ɔ�΂����[�h

    //CHANGE_SAME,    //  �}�e���A����ς��đS�������ɂ��郂�[�h

    NONE,
};


//	�O���[�o���ϐ�
extern CLEAR_FLAG g_clearFlag;
extern GAME_MODE g_gameMode;
extern int g_score;

// �O���錾
class DebugCamera;
class IScene;

/// <summary>
/// Game�N���X����A���[�U���������𔲂��o�����N���X
/// </summary>
class GameMain
{
private:
	// ���̃V�[���ԍ�
	GAME_SCENE m_nextScene;

	// �V�[��
	IScene* m_pScene;
public:
    GameMain();
    ~GameMain();

    void Initialize();
    void Update(const DX::StepTimer& timer);
    void Render();
    void Finalize();

	// �V�[���֌W����

	// �V�[���̍쐬
	void CreateScene();

	// �V�[���̍폜
	void DeleteScene();
};
