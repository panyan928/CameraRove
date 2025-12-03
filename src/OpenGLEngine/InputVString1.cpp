/**
 * @file	×Ö·û´®ÀàÊµÏÖ
 * @author	flq
 */
#include "InputVString1.h"
#include "InputCharsets.h"


void GB2312toUnicode2wchart(const char* src, wchar_t* wchar,int* dstLen)
{	
	int len = strlen(src);
	unsigned short* dst = new unsigned short[len + 1];
	int cnt = 0;
	unsigned char code[2];

	for(int i = 0; i < len;i++)
	{
		if( (unsigned char)src[i] < 129 )
		{
			dst[cnt] = src[i];
		}
		else
		{
			if(len==1)
			{
				dst[cnt] = (unsigned char)src[i];
			}
			else
			{
				code[0] = (unsigned char)src[i];
				++i;
				code[1] = (unsigned char)src[i];
	#ifdef CPL_MSB
				tmp = code[0];
				code[0] = code[1];
				code[1] = tmp;
	#endif
				dst[cnt] = charsets_gbk_to_ucs(code);
			}
		}
		++cnt;
	}
	dst[cnt] = 0x00;
	*dstLen = cnt;

	for (int i = 0; i <= cnt; i++)
	{
		wchar[i] = dst[i];
	}

	delete[] dst;
}
