/**
 * @file	�ַ�����ͷ�ļ�
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
	* @brief			תΪunicode��
	* @param src		Դ�ַ���
	* @param dst		unicode���ַ���
	* @param dstLen		ת���ĳ���
	*/
void GB2312toUnicode2wchart(const char* src, wchar_t* wchar,int* dstLen);


#endif
