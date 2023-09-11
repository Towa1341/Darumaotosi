#include "pch.h"
#include "CSVStage.h"

#include <fstream>
#include <sstream>

CSVStage::CSVStage()
{
}

bool CSVStage::LoadData(const std::string& fileName)
{
	std::ifstream file;

	file.open(fileName);
	// マップファイルをオープンできない場合
	if (!file.is_open())
		return false;

	std::string str;
	std::string str_conma;

	//	一行分のデータを格納する変数
	std::vector<std::string> datas;

	// ストリングストリームクラスのmapDataStream変数を宣言する(マップデータ全体)
	std::stringstream mapDataStream;

	// wifsをmapDataStreamに出力する
	mapDataStream << file.rdbuf();


	int index = 0;

	// getline関数で1行ずつ読み込む
	while (std::getline(mapDataStream, str, '\n'))
	{
		//	1行目を飛ばす
		if (index <= 0)
		{
			index++;
			continue;
		}

		// 「,」区切りごとにデータを読み込むためにistringstream型にする
		std::stringstream i_stream;
		i_stream << str;

		// 「,」区切りごとにデータを読み込む
		while (std::getline(i_stream, str_conma, ',')) 
		{
			//	格納
			datas.push_back(str_conma);
		}

		//	データを適用する
		DATA data;

		//	移動方向
		data.moveDir = static_cast<MOVE_DIR>(std::atoi(datas[0].c_str()));;
		//	初期位置(0,0,0で真ん中に来るように補正する)
		data.stratPosition.x = std::atof(datas[1].c_str()) + 3.0f;
		data.stratPosition.y = std::atof(datas[2].c_str());
		data.stratPosition.z = std::atof(datas[3].c_str()) - 7.0f;
		//	移動距離
		data.moveVel = std::atof(datas[4].c_str());
		//	スコア
		data.score = std::atoi(datas[5].c_str());

		//	設定
		m_stageDatas.push_back(data);

		datas.clear();
	}

	file.close();
	return true;
}
