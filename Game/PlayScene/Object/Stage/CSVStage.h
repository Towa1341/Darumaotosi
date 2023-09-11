#pragma once
#include "Game/GameMain.h"

#include <string>
#include <vector>

class CSVStage
{
public:
	//	穴が動く方向
	enum class MOVE_DIR
	{
		UP = 0, // 上    0
		SIDE,	// 横    1
		AROUND	// 前後  2
	};

	struct DATA
	{
		MOVE_DIR moveDir;							//	動く方向
		DirectX::SimpleMath::Vector3 stratPosition; //	初期位置
		float moveVel;								//	動く距離
		int score;									//	スコア
	};

private:
	std::vector<DATA> m_stageDatas;

public:
	CSVStage();
	~CSVStage() = default;

	//	ギミックの総数を取得する
	int GetGimmickNum() { return m_stageDatas.size(); }

	const std::vector<DATA>& GetDatas() const { return m_stageDatas; }

	bool LoadData(const std::string& fileName);
};