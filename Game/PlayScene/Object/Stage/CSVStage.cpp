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
	// �}�b�v�t�@�C�����I�[�v���ł��Ȃ��ꍇ
	if (!file.is_open())
		return false;

	std::string str;
	std::string str_conma;

	//	��s���̃f�[�^���i�[����ϐ�
	std::vector<std::string> datas;

	// �X�g�����O�X�g���[���N���X��mapDataStream�ϐ���錾����(�}�b�v�f�[�^�S��)
	std::stringstream mapDataStream;

	// wifs��mapDataStream�ɏo�͂���
	mapDataStream << file.rdbuf();


	int index = 0;

	// getline�֐���1�s���ǂݍ���
	while (std::getline(mapDataStream, str, '\n'))
	{
		//	1�s�ڂ��΂�
		if (index <= 0)
		{
			index++;
			continue;
		}

		// �u,�v��؂育�ƂɃf�[�^��ǂݍ��ނ��߂�istringstream�^�ɂ���
		std::stringstream i_stream;
		i_stream << str;

		// �u,�v��؂育�ƂɃf�[�^��ǂݍ���
		while (std::getline(i_stream, str_conma, ',')) 
		{
			//	�i�[
			datas.push_back(str_conma);
		}

		//	�f�[�^��K�p����
		DATA data;

		//	�ړ�����
		data.moveDir = static_cast<MOVE_DIR>(std::atoi(datas[0].c_str()));;
		//	�����ʒu(0,0,0�Ő^�񒆂ɗ���悤�ɕ␳����)
		data.stratPosition.x = std::atof(datas[1].c_str()) + 3.0f;
		data.stratPosition.y = std::atof(datas[2].c_str());
		data.stratPosition.z = std::atof(datas[3].c_str()) - 7.0f;
		//	�ړ�����
		data.moveVel = std::atof(datas[4].c_str());
		//	�X�R�A
		data.score = std::atoi(datas[5].c_str());

		//	�ݒ�
		m_stageDatas.push_back(data);

		datas.clear();
	}

	file.close();
	return true;
}
