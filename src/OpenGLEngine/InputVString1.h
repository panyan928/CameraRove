/**
 * @file	字符串类头文件
 * @author	flq
 */
#ifndef INPT_VSTRING1_H
#define INPT_VSTRING1_H

#ifndef NULL
#define NULL 0
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/**
	* @brief			转为unicode码
	* @param src		源字符串
	* @param dst		unicode的字符串
	* @param dstLen		转换的长度
	*/
void GB2312toUnicode2wchart(const char* src, wchar_t* wchar,int* dstLen);


#endif
