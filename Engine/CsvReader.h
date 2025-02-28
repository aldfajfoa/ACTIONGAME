#pragma once
#include <vector>
#include <string>

//-----------------------------------------------------------
//CSVファイルを扱うクラス
//-----------------------------------------------------------
class CsvReader {
public:
	CsvReader();
	CsvReader(std::string filename);
	~CsvReader();
	bool Load(std::string filename);
	int GetHeight();
	int GetWidth(int line);
	std::string GetString(int column, int line);
	int GetInt(int column, int line);
	float GetFloat(int column, int line);
private:
	struct LINEREC {
		std::vector<std::string> record;
	};
	std::vector<LINEREC> all;
};