#pragma once
#include "Game/GameMain.h"

#include <string>
#include <vector>

class CSVStage
{
public:
	//	������������
	enum class MOVE_DIR
	{
		UP = 0, // ��    0
		SIDE,	// ��    1
		AROUND	// �O��  2
	};

	struct DATA
	{
		MOVE_DIR moveDir;							//	��������
		DirectX::SimpleMath::Vector3 stratPosition; //	�����ʒu
		float moveVel;								//	��������
		int score;									//	�X�R�A
	};

private:
	std::vector<DATA> m_stageDatas;

public:
	CSVStage();
	~CSVStage() = default;

	//	�M�~�b�N�̑������擾����
	int GetGimmickNum() { return m_stageDatas.size(); }

	const std::vector<DATA>& GetDatas() const { return m_stageDatas; }

	bool LoadData(const std::string& fileName);
};