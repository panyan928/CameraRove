#include "cstringutil.h"
#include <stdlib.h>

int find_first_of(const std::string& src, const std::string& separator)
{
	const char* str = src.c_str();
	const char* Separator = separator.c_str();
	int len = src.size();
	for (int i = 0; i < len; i++) {
		if (str[i] == Separator[0]) return i;
	}
	return -1;
}

/**
 * @brief 分割字符�?
 * @details 使用字符串separator分割输入字符串src, 结果存储到res�?
 * @param src 待分割字符串
 * @param separator 分割�?
 * @param res 输出的结果数�?
 */
void CStringUtil::split(const std::string& src, const std::string& separator, std::vector<std::string>& res)
{
	string str = src;
	string substring;
	string::size_type start = 0;
	int index = -1;
	res.clear();
	index = find_first_of(src, separator);
	do
	{
		if (index != -1)
		{
			substring = str.substr(start, index - start);
			res.push_back(substring);
			start = index + separator.size();
			index = str.find(separator, start);
			if (start == string::npos) break;
		}
	} while (index != string::npos);

	//the last part
	substring = str.substr(start);
	res.push_back(substring);
}

/**
 * @brief 从文件路径获取文件格�?
 * @param src 文件路径
 * @return std::string 文件格式
 */
std::string CStringUtil::getFormatFromSrc(const std::string& src)
{
	return src.substr(src.find_last_of('.') + 1);
}

/**
 * @brief 从文件路径获取文件名�?
 * @param src 文件路径
 * @return std::string 文件名称
 */
std::string CStringUtil::getNameFromSrc(const std::string& src)
{
	return subString(src, src.find_last_of('/') + 1, src.find_last_of('.') - 1);
}

/**
 * @brief 分割字符�?
 * @details 根据输入的开始和结束为止分割字符�?
 * @param str 待分割字符串
 * @param begin 开始位�?
 * @param end 结束位置
 * @return std::string 返回分割的字符串
 */
std::string CStringUtil::subString(const std::string str, const int begin, const int end)
{
	int length = end - begin + 1;
	/*
	if(end > begin){
		cout << "error range" << endl;
		return "";
	}
	if(end >= str.size() - 1){
		return str.substr(begin);
	}*/
	return str.substr(begin, length);
}

/**
 * @brief 获取瓦片参数
 * @details 从输入的瓦片路径中获取瓦片参数，瓦片名按 zoom.row.col 组织
 * @param src 输入的瓦片路�?
 * @param zoom_level 
 * @param tile_col 
 * @param tile_row  需要输出的瓦片参数
 * @param separator 分隔�?默认�?'.'
 */
void CStringUtil::getTileParameterFromSrc(const std::string src, int& zoom_level, int& tile_col, int& tile_row, const std::string separator)
{
	std::string name = getNameFromSrc(src);
	std::vector<std::string> res;
	split(name, separator, res);
	zoom_level = atoi(res[0].c_str());
	tile_row = atoi(res[1].c_str());
	tile_col = atoi(res[2].c_str());
}

/**
 * @brief 去除空字符串
 * @param strs 输入字符串数�? 去除后输�?
 */
void CStringUtil::removeEmptyString(vector<string>& strs)
{
	vector<string>::iterator str_iter;
	str_iter = strs.begin();
	while (str_iter != strs.end()) {
		if ((*str_iter).empty()) {
			str_iter = strs.erase(str_iter);
		}
		else
			str_iter++;
	}
}

