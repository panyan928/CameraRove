#ifndef CSTRINGUTIL_H
#define CSTRINGUTIL_H

#include <iostream>
#include <vector>

using namespace std;
/**
 * @author myl 1564496396@qq.com
 * @class CStringUtil
 * @brief 字符串静态工具类
 * @details 提供面向需要的各种字符串操作工具
 */
class CStringUtil
{
public:
	static void split(const std::string& src, const std::string& separator, std::vector<std::string>& res);
	static std::string getFormatFromSrc(const std::string& src);
	static std::string getNameFromSrc(const std::string& src);
	static std::string subString(const std::string str, const int begin, const int end);
	static void getTileParameterFromSrc(const std::string src, int& zoom_level, int& tile_col, int& tile_row, const std::string separator = ".");
	static void removeEmptyString(vector<string>& strs);
};

#endif
