//
// GameMain.h
//
#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"


// シーンの列挙
enum class GAME_SCENE : int
{
    NONE,

    TITLE,
    PLAY,
    RESULT,
    //  シーンを増やすときに増やす
};

//  クリア条件
enum class CLEAR_FLAG
{
    NONE,

    GOOD,   //  良い
    NORMAL, //  普通
    BAD,    //　悪い

    SHOOTING,   //　シューティングモード用
};

//	モードの列挙
enum class GAME_MODE
{
    FLY_SPECIFIC,	//	特定のマテリアルだけ飛ばすモード
    //FLY_EXEPTING,	//	特定のマテリアル以外を飛ばすモード

    SHOOTING,		//	指定の場所に飛ばすモード

    //CHANGE_SAME,    //  マテリアルを変えて全部同じにするモード

    NONE,
};


//	グローバル変数
extern CLEAR_FLAG g_clearFlag;
extern GAME_MODE g_gameMode;
extern int g_score;

// 前方宣言
class DebugCamera;
class IScene;

/// <summary>
/// Gameクラスから、ユーザ処理部分を抜き出したクラス
/// </summary>
class GameMain
{
private:
	// 次のシーン番号
	GAME_SCENE m_nextScene;

	// シーン
	IScene* m_pScene;
public:
    GameMain();
    ~GameMain();

    void Initialize();
    void Update(const DX::StepTimer& timer);
    void Render();
    void Finalize();

	// シーン関係処理

	// シーンの作成
	void CreateScene();

	// シーンの削除
	void DeleteScene();
};
