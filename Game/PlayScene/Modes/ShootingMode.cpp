#include "pch.h"

#include "ShootingMode.h"
#include "../PlayScene.h"

#include "../../InputManager.h"
#include "../../../Libraries/MyLibraries/DebugCamera.h"

#include "Libraries/MyLibraries/DebugNumRender.h"

using namespace DirectX;

const int ShootingMode::BODYMANAGER_NUM = 3;
const float ShootingMode::ROT_MAX = 30.0f;
const float ShootingMode::MOVE_ROT_VALUE = 5.0f;

ShootingMode::ShootingMode() :
	Mode(),
	m_camera(nullptr),
	m_nowBodyNumber(0),
	m_endFlag(false), 
	m_fryRot(-90.0f),
	m_fryRotLength(0.0f),
	m_isFry(false),
	m_fryBody(nullptr),
	m_followFlag(false),
	m_score(0)
{
}

ShootingMode::~ShootingMode()
{
	//	最後にグローバル変数に反映する
	g_score = m_score;
}

void ShootingMode::Initialize()
{
	//	カメラ
	m_camera = new Camera(SimpleMath::Vector3(0.0f, 20.0f, 20.0f), SimpleMath::Vector3(0.0f, 4.0f, -8.0f));

	m_view = m_camera->GetViewMatrix();
	m_projection = m_camera->GetProjectionMatrix();


	//	必要な変数の準備
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	コモンステート：D3Dレンダリング状態オブジェクト
	m_commonState = std::make_unique<DirectX::CommonStates>(device);

	//	プリミティブバッチ
	m_primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);
	//	エフェクト
	m_effect = std::make_unique<BasicEffect>(device);
	//	スプライトバッチ
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

	//	basicEffectの生成
	m_basicEffect = std::make_unique<BasicEffect>(device);

	// ライトの設定に必要な情報
	m_basicEffect->SetLightingEnabled(true); // ライト(on)
	m_basicEffect->SetTextureEnabled(true); // テクスチャ(on)
	m_basicEffect->SetVertexColorEnabled(false); // 頂点カラー(off)

	// 入力レイアウトを作成するための準備
	const void* shaderByteCode;
	size_t byteCodeLength;
	// 使用する頂点シェーダー情報の取得
	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	// 入力レイアウトの作成
	device->CreateInputLayout(
		VertexPositionNormalTexture::InputElements,
		VertexPositionNormalTexture::InputElementCount,
		shaderByteCode,
		byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);

	//	ギミックマネージャ
	m_gimmickManager = std::make_unique<GimmickManager>();

	//	積まれる体の最大数
	const int STACK_BODY_NUM = 3;

	//	BodyManagerの生成と初期化
	for (int i = 0; i < BODYMANAGER_NUM; i++)
	{
		m_bodyManagers.push_back(std::make_unique<BodyManager>());
		m_bodyManagers[i]->Initialize(DirectX::SimpleMath::Vector2((i * 6.0f) - 6.0f, -2.0f));

		//	Bodyを増やす
		for (int j = 0; j < STACK_BODY_NUM; j++)
		{
			m_bodyManagers[i]->AddBody();
		}
	}

	//	最初に選択されている物を０に指定
	m_pBodyManager = m_bodyManagers[0].get();

	//	クリア条件をシューティングモード用に変更しておく
	g_clearFlag = CLEAR_FLAG::SHOOTING;

	//	ハンマーの位置を設定しておく
	GetHammer()->SetXPos(0);

	//	ゆか
	m_ground = DirectX::GeometricPrimitive::CreateBox(context, SimpleMath::Vector3(60.0f, 0.0f, 50.0f));
	m_wall = DirectX::GeometricPrimitive::CreateBox(context, SimpleMath::Vector3(55.0f, 0.0f, 30.0f));

	//	スコアを表示するクラスの初期化
	m_scoreUI = std::make_unique<ScoreUI>();
}

GAME_MODE ShootingMode::Update(const DX::StepTimer& timer)
{
	//	キーボードトラッカー
	InputManager* inputMabager = InputManager::GetInstance();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputMabager->GetKeyboardTracker();

	ModeCommonProcess* modeCommonProcess = GetModeCommonProcess();

	modeCommonProcess->SetBodyManager(m_pBodyManager);

	//	体を生成する
	//modeCommonProcess->FallBodys(timer);

	if (!m_isFry)
	{
		//	エンターを押している間パワーを増減する
		if (inputMabager->GetKeyboardState().Enter)
		{
			modeCommonProcess->MovePower();
		}

		//	飛ばす物を選択する＆打ち切っていたら選択不可
		ChangeBpdyManagers();
		//	上下キーで体を選択する関数
		modeCommonProcess->ChangeBodySelect();
		//	左右キーで向きを決めて設定する
		MoveFryRot();
	}
	else
	{
		m_isFry = m_pBodyManager->GetSelectBody()->GetDeleteFlag();
	}

	//	飛ばした処理
	if (keyboardTracker->IsKeyReleased(DirectX::Keyboard::Keys::Enter) &&
		m_pBodyManager->GetBodysSize() != 0 &&
		m_pBodyManager->GetSelectBody()->GetEndMoveFlag())
	{
		//	パワーによって飛ばす距離を変える
		m_pBodyManager->SetBodyVel((modeCommonProcess->GetPower() + 0.5f) * 0.9f);

		//	飛ばす
		m_pBodyManager->FryBody();

		//	飛ばしたフラグをオンにする
		m_isFry = true;

		//	現在の体のタイプを設定する
		m_fryBody = m_pBodyManager->GetSelectBody();

		//	ギミックと当たる可能性があるかを判定してあればカメラを追従
		if (IfHitGimmick())
		{
			int a = 0;
			//	追従フラグをオンにする
			m_followFlag = true;

			//	予測範囲を消す
		}

		//	パワーをリセットする
		modeCommonProcess->ResetPower();
	}

	if (m_pBodyManager->GetBodysPop() && m_pBodyManager->GetSelectBody()->GetEndMoveFlag())
	{
		//	当たり判定を行うための設定
		m_gimmickManager->SetHitStates(m_pBodyManager->GetSelectBody()->GetPosition(), m_pBodyManager->GetSelectBody()->GetRadius(), 1.0f);

		//	あなとの当たり判定
		if (m_gimmickManager->HitHole() &&
			!m_pBodyManager->GetSelectBody()->GetIsHit())
		{
			//	スコアを加算する 変える
			m_score += m_gimmickManager->GetScore();
			//	加算されたことを伝える
			m_scoreUI->SetScoreCheck(true);
			//	入ったスコアを設定する
			m_scoreUI->SetInScore(m_gimmickManager->GetScore());

			//	当たったことをBodyに知らせる
			m_pBodyManager->GetSelectBody()->SetIsHit(true);
		}

		//	スコアボードとの当たり判定
		if (m_gimmickManager->HitScoreBaord())
		{
			//	落とす
			m_pBodyManager->GetSelectBody()->SetVel(0.0f);
		}
	}

	
	/*if (m_pBodyManager->GetSelectBody()->GetEndMoveFlag())
	{
		m_camera->ResetCamera();
	}*/
	

	//	TODO：追従フラグにする　現：飛んでるかフラグ　確認：フラグなし
	if (m_followFlag)
	{
		//	カメラを追従
		FollowCamera(m_fryBody->GetPosition());

		if (m_fryBody->GetDeleteFlag())
		{
			m_camera->ResetCamera();

			m_fryBody = nullptr;

			m_followFlag = false;
		}
	}

	//	BodyManager更新
	for (int i = 0; i < BODYMANAGER_NUM; i++)
	{
		m_bodyManagers[i]->Update(timer);
	}

	////	カメラ
	//m_camera->Update();

	m_camera->CalculateMatrix();
	m_view = m_camera->GetViewMatrix();
	m_projection = m_camera->GetProjectionMatrix();

	//	あなとスコアボードの更新
	m_gimmickManager->Update(timer);

	//	全部打ち切ったら別のに移動する
	if (!m_pBodyManager->GetBodysPop())
	{
		//	このモードを終了する？？
		bool isEnd = true;

		for (int i = 0; i < 3; i++)
		{
			//	打ち切っていないものを探す
			if (m_bodyManagers[i]->GetBodysPop())
			{
				//	設定する
				m_pBodyManager = m_bodyManagers[i].get();
				//	ハンマーの位置を更新
				GetHammer()->SetXPos(static_cast<float>(i));
				//	まだ終わらない
				isEnd = false;
				break;
			}
		}

		//	打ち切っていないものがあったらFalseになるのでシーンの遷移が行われない
		m_endFlag = isEnd;
	}


	//	スコアの表示の更新
	m_scoreUI->Update(timer, m_score);

	return GAME_MODE::NONE;
}

void ShootingMode::Draw()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	床の描画
	TextureManager* textureManager = TextureManager::GetInstance();
	SimpleMath::Matrix groundWorld = SimpleMath::Matrix::Identity;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture = textureManager->GetTexture(TextureManager::eTextureName::TATAMI);
	groundWorld = SimpleMath::Matrix::CreateTranslation(0.0f, -1.0f, -20.0f);
	m_ground->Draw(groundWorld, m_view, m_projection, DirectX::Colors::White, texture.Get());
	//	壁の描画
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> wallTexture = textureManager->GetTexture(TextureManager::eTextureName::WALL);
	SimpleMath::Matrix wallWorld, wallWorld2, wallWorld3 = SimpleMath::Matrix::Identity;
	//	正面
	wallWorld = wallWorld2 = wallWorld3 = SimpleMath::Matrix::CreateRotationX(-90.0f / 180.0f * DirectX::XM_PI);
	wallWorld *= SimpleMath::Matrix::CreateTranslation(0.0f, 15.0f, -40.0f);
	m_wall->Draw(wallWorld, m_view, m_projection, DirectX::Colors::White, wallTexture.Get());
	//	右
	wallWorld2 *= SimpleMath::Matrix::CreateRotationY(90.0f / 180.0f * DirectX::XM_PI);
	wallWorld2 *= SimpleMath::Matrix::CreateTranslation(25.0f, 15.0f, -20.0f);
	m_wall->Draw(wallWorld2, m_view, m_projection, DirectX::Colors::White, wallTexture.Get());
	//	左
	wallWorld3 *= SimpleMath::Matrix::CreateRotationY(-90.0f / 180.0f * DirectX::XM_PI);
	wallWorld3 *= SimpleMath::Matrix::CreateTranslation(-25.0f, 15.0f, -20.0f);
	m_wall->Draw(wallWorld3, m_view, m_projection, DirectX::Colors::White, wallTexture.Get());

	//	操作説明の描画
	m_spriteBatch->Begin(SpriteSortMode_Deferred, m_commonState->NonPremultiplied());
	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::OPERATION1).Get(), DirectX::SimpleMath::Vector2(0.0f, 0.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);
	m_spriteBatch->Draw(textureManager->GetTexture(TextureManager::eTextureName::OPERATION2).Get(), DirectX::SimpleMath::Vector2(0.0f, 50.0f), nullptr, DirectX::Colors::White, 0.0f, DirectX::SimpleMath::Vector2::Zero);
	m_spriteBatch->End();


	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	//	色と向き
	SimpleMath::Color lightColor = SimpleMath::Color(0.98f, 0.98f, 0.98f);
	SimpleMath::Vector3 lightDirection = SimpleMath::Vector3(-1.0f, -1.0f, 0.2f);

	// エフェクトにライトの効果を設定する
	m_basicEffect->SetLightEnabled(0, true); // ライト 0 番を使用

	m_basicEffect->SetLightDiffuseColor(0, lightColor); // ライトの色を指定

	m_basicEffect->SetLightDirection(0, lightDirection); // ライトの向きを指定

	// エフェクトに各種変換行列の設定 -> 設定がマズいと見た目が正しくならない
	m_basicEffect->SetWorld(world); // ワールド行列を設定
	m_basicEffect->SetView(m_view); // ビュー行列の設定
	m_basicEffect->SetProjection(m_projection); // 射影行列の設定
	m_basicEffect->Apply(context); // エフェクトの反映

	//	マネージャの描画
	for (int i = 0; i < BODYMANAGER_NUM; i++)
	{
		m_bodyManagers[i]->Render(m_view, m_projection, m_basicEffect.get(), m_inputLayout);
	}

	//	ハンマーの描画
	GetHammer()->Render(m_view, m_projection);

	//	あなとスコアボードを描画
	m_gimmickManager->Render(m_view, m_projection);

	//	飛ぶ方向を示す線の描画
	DrawLine();

	//	スコアの描画
	m_scoreUI->Render(m_view, m_projection);
}

void ShootingMode::Finalize()
{
	if (m_camera != nullptr)
	{
		delete(m_camera);
		m_camera = nullptr;
	}
}

bool ShootingMode::GetEndFlag()
{
	return m_endFlag;
}

//	飛ばすやつを変更する
void ShootingMode::ChangeBpdyManagers()
{
	//	キーボードトラッカー
	InputManager* inputMabager = InputManager::GetInstance();
	DirectX::Keyboard::KeyboardStateTracker* keyboardTracker = inputMabager->GetKeyboardTracker();

	//	１、２、３キーで飛ばすものを変更する＆ハンマーの位置を変更する
	//	飛ばすものが無ければ変更不可
	if (keyboardTracker->IsKeyPressed(Keyboard::Keys::D1) &&
		m_bodyManagers[0]->GetBodysPop())
	{
		m_pBodyManager = m_bodyManagers[0].get();
		GetHammer()->SetXPos(0);
	}
	else if (keyboardTracker->IsKeyPressed(Keyboard::Keys::D2) &&
		m_bodyManagers[1]->GetBodysPop())
	{
		m_pBodyManager = m_bodyManagers[1].get();
		GetHammer()->SetXPos(1);
	}
	else if (keyboardTracker->IsKeyPressed(Keyboard::Keys::D3) &&
		m_bodyManagers[2]->GetBodysPop())
	{
		m_pBodyManager = m_bodyManagers[2].get();
		GetHammer()->SetXPos(2);
	}
}

//	飛んでいく角度の線を描画する
void ShootingMode::DrawLine()
{
	DX::DeviceResources* pDR = DX::DeviceResources::GetInstance();
	ID3D11Device1* device = pDR->GetD3DDevice();
	ID3D11DeviceContext1* context = pDR->GetD3DDeviceContext();

	//	エフェクトの設定
	m_effect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
	m_effect->SetProjection(m_projection);
	m_effect->SetView(m_view);
	m_effect->Apply(context);

	//	色
	DirectX::FXMVECTOR color = Colors::Red;
	
	//	位置
	SimpleMath::Vector2 basicPosition = m_pBodyManager->GetPosition();
	SimpleMath::Vector3 startPos = SimpleMath::Vector3(basicPosition.x, 0.0f, basicPosition.y);
	float radian = DirectX::XMConvertToRadians(m_fryRot);
	const float length = 7.0f;
	SimpleMath::Vector3 endPos = SimpleMath::Vector3(basicPosition.x + cos(radian) * length, 0.0f, basicPosition.y + sin(radian) * length);

	//	頂点
	VertexPositionColor startVerTex = VertexPositionColor{ startPos,color };
	VertexPositionColor endVerTex = VertexPositionColor{ endPos,color };
	VertexPositionColor startVerTex2 = VertexPositionColor{ startPos + SimpleMath::Vector3(1.0f,0.0f,0.0f),color };
	VertexPositionColor endVerTex2 = VertexPositionColor{ endPos + SimpleMath::Vector3(1.0f,0.0f,0.0f),color };

	//	描画
	m_primitiveBatch->Begin();

	m_primitiveBatch->DrawLine(startVerTex, endVerTex);

	m_primitiveBatch->DrawQuad(startVerTex, endVerTex, startVerTex2, endVerTex2);

	m_primitiveBatch->End();
}


//	飛んでいく角度を操作する
void ShootingMode::MoveFryRot()
{
	InputManager* inputManager = InputManager::GetInstance();
	DirectX::Keyboard::State keyboardState = inputManager->GetKeyboardState();

	//	左右キーで一定値の角度変更
	if (keyboardState.Right && m_fryRotLength < ROT_MAX)
	{
		m_fryRot += MOVE_ROT_VALUE;
		m_fryRotLength += MOVE_ROT_VALUE;
	}
	else if (keyboardState.Left && m_fryRotLength > -ROT_MAX)
	{
		m_fryRot -= MOVE_ROT_VALUE;
		m_fryRotLength -= MOVE_ROT_VALUE;
	}

	//	角度を設定する
	m_pBodyManager->SetBodyRot(m_fryRot);
}

bool ShootingMode::IfHitGimmick()
{
	ModeCommonProcess* modeCommonProcess = GetModeCommonProcess();

	//	予測位置
	const float GRAVITY = 0.08f;
	const float JUMP = static_cast<float>(static_cast<int>(m_fryBody->GetBodyType()) + 1) * 0.3f;

	//	どれくらいで飛んでいくか
	float time = (JUMP / GRAVITY) * 2;
	float vel = (modeCommonProcess->GetPower() + 0.5f) * 0.9f;

	//	飛んでいく角度
	float radian = DirectX::XMConvertToRadians(m_fryRot);

	SimpleMath::Vector3 basePosition{ m_pBodyManager->GetPosition().x, 0.0f, m_pBodyManager->GetPosition().y };

	//	予測位置
	SimpleMath::Vector3 position = SimpleMath::Vector3(cos(radian) * (vel * time), 0.0f, sin(radian) * (vel * time))
		+ basePosition;
	
	//	予測位置とギミックたちとの当たり判定を返す
	return m_gimmickManager->ifHit(position);
}

void ShootingMode::FollowCamera(DirectX::SimpleMath::Vector3 position)
{
	//	カメラを追従させる　eyeとtargatを変える？？？
	SimpleMath::Vector3 spacePos{ 0.0f,5.0f,10.0f };
	m_camera->SetEyePosition(position + spacePos);
	m_camera->SetTargetPosition(position);
} 