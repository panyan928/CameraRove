#ifndef OpenGLRenderEngine_H
#define OpenGLRenderEngine_H

//#include <GL/freeglut.h>
#include "gl\gl.h"	
#include "gl\glu.h"
#include "../TMUtil/tmtools.h"

#include "../TMTextRender/cfontrender.h"
#include "copenglcoordinatesengine.h"
#include "../TMStyle/cstyle.h"
#include "../TMStyle/ccolor.h"
//#include <codecvt>
#include "../TMUtil/OMapGlobal.h"

using namespace textRender;

namespace openglEngine {
	class OpenGLRenderEngine
	{
	public:
        template <typename T>
		static int drawHeight(T* pts, T* color, int* index, int size);

		template <typename T>
		static int drawTriangles(T* pts, Color color, int size);

		template <typename T>
		static int drawTriangles(T* pts, int* index, Color color, int size);

		template <typename T>
		static int drawLines(T* pts, int* stops, int stopsCount, Color color, int lineWidth, int dashArray);

		template <typename T>
		static int drawRasters(T* pts, T* texture_coor, int* index, const char* texture_path, int pt_size);

		template <typename T>
		static int drawRasters(T* pts, T* texture_coor, int* index, unsigned char* image, int width, int height, int nrChannels, int pt_size);

		template<typename T>
		static int drawRasters(T* pts, T* texture_coor, int* index, unsigned char* image, int width, int height, int nrChannels, int pt_size, string level2Index);

        template <typename T>
		static int drawRasters(T* pts, T* texture_coor, int* index, unsigned char* image,GLuint *texture, int width, int height, int nrChannels, int pt_size);

		template<typename T>
		static int drawTIN(T* pts, T* texture_coor, int* index, int pt_size, string level2Index);

        template <typename T>
		static int drawAnnotations(T* pts, int pt_size, string& buffer, int* stops, Color color, CFontRender* render);

		template <typename T>
		static int drawAnnotations(T* pts, int pt_size, string& buffer, int* stops, TMStyle::CStyle* style, CFontRender* render, int anno);

		template <typename T>
		static int drawAnnotations(T* pts, int pt_size, string& buffer, int* stops, int symbol, TMStyle::CStyle* style, CFontRender* render);
		
		template <typename T>
		static int drawSymbolsAndAnnotations(T* pts, int pt_size, const char* path, string& buffer, int* stops, TMStyle::CStyle* style, CFontRender* render);

		template <typename T>
		static int drawSymbols(T* pts, int pt_size, const char* path, CFontRender* render);

	private:
		static bool checkOverlap(Recti& item, unsigned int* pixels, int anno) {
			if (item[0] < 0 || item[2] >= screenWidth || item[1] < 0 || item[3] >= screenHeight)
				return true;
			//return false;
			// 检查重复
			for (int i = item[1]; i < item[3]; i++) {
				for (int j = item[0]; j < item[2]; j++) {
					//if (pixels[4 * (i * 1024 + j)] != 0 || pixels[4 * (i * 1024 + j) + 1] != 0
						//|| pixels[4 * (i * 1024 + j) + 2] != 0)
					/*if (!(pixels[4 * (i * screenWidth + j) + 0] == 0 && pixels[4 * (i * screenWidth + j) + 1] == 0
						&& pixels[4 * (i * screenWidth + j) + 2] == 0 && pixels[4 * (i * screenWidth + j) + 3] == 0))
						return true;*/
					/*if (!(pixels[4 * (i * screenWidth + j) + 0] == 0 && pixels[4 * (i * screenWidth + j) + 1] == 0
						&& pixels[4 * (i * screenWidth + j) + 2] == 0 && pixels[4 * (i * screenWidth + j) + 3] == 0))
			 		return true;*/
					
					//if (anno <= pixels[5 * (i * screenWidth + j)]) return true; 
					if (pixels[5 * i * screenWidth + j] != 0){
						/*当前级别小于已有地标点级别，判断重复，直接跳过绘制， 不需要修改pixels*/
						if (anno <= pixels[5 * i * screenWidth + j]) return true;
						// 级别更高，需要覆盖原有区域
						else {
							int xmin = pixels[5 * i * screenWidth + screenWidth + j];
							int ymin = pixels[5 * i * screenWidth + screenWidth * 2 + j];
							int xmax = pixels[5 * i * screenWidth + screenWidth * 3 + j];
							int ymax = pixels[5 * i * screenWidth + screenWidth * 4 + j];
							for (int i = ymin; i < ymax; i++) {
								memset(&(pixels[5 * i * screenWidth + xmin]), 0, (xmax - xmin + 1) * sizeof(unsigned int)); //重置低级别区域的pixels
							}
							break;
						}
					}
				}
			}
			
			// 没有重复，直接填充标志位
			for (int i = item[1]; i < item[3]; i++) {
				std::fill(&(pixels[5 * i * screenWidth + item[0]]), &(pixels[5 * i * screenWidth + item[0]]) + item[2] - item[0], anno);
				std::fill(&(pixels[5 * i * screenWidth + screenWidth + item[0]]), &(pixels[5 * i * screenWidth + screenWidth + item[0]]) + item[2] - item[0], item[0]);
				std::fill(&(pixels[5 * i * screenWidth + screenWidth * 2 + item[0]]), &(pixels[5 * i * screenWidth + screenWidth * 2 + item[0]]) + item[2] - item[0], item[1]);
				std::fill(&(pixels[5 * i * screenWidth + screenWidth * 3 + item[0]]), &(pixels[5 * i * screenWidth + screenWidth * 3 + item[0]]) + item[2] - item[0], item[2]);
				std::fill(&(pixels[5 * i * screenWidth + screenWidth * 4 + item[0]]), &(pixels[5 * i * screenWidth + screenWidth * 4 + item[0]]) + item[2] - item[0], item[3]);

				//memset(&(pixels[5 * i * screenWidth + item[0]]), anno * 255/ 6, (item[2] - item[0] + 1) * sizeof(unsigned int)); //第一个W*H存储 anno
				//memset(&(pixels[5 * i * screenWidth + screenWidth + item[0]]), item[0], (item[2] - item[0] + 1) * sizeof(unsigned int)); //第二个W*H存储 item[0] xmin
				//memset(&(pixels[5 * i * screenWidth + screenWidth * 2 + item[0]]), item[1], (item[2] - item[0] + 1) * sizeof(unsigned int)); //第三个W*H存储 item[1] ymin
				//memset(&(pixels[5 * i * screenWidth + screenWidth * 3 + item[0]]), item[2], (item[2] - item[0] + 1) * sizeof(unsigned int)); //第四个W*H存储 item[2] xmax
				//memset(&(pixels[5 * i * screenWidth + screenWidth * 4 + item[0]]), item[3], (item[2] - item[0] + 1) * sizeof(unsigned int)); //第五个W*H存储 item[3] ymax
			}
			return false;
		}

		static void getHeightAndWidth(int& w, int& h, string& name) {
			int width = 0;
			int lineCount = 1;
			for (int i = 0; i < name.size(); i++) {
				if (name[i] != '\n') {
					width++;
					w = max(w, width);
				}
				else {
					width = 0;
					lineCount++;
				}
			}
			h = lineCount;
		}

		static int UTF2Unicode(wchar_t* wstr, int size, char* utf8)
		{
			int size_s = strlen(utf8);
			int size_d = size;

			wchar_t* des = wstr;
			char* src = utf8;
			memset(des, 0, size * sizeof(wchar_t));

			int s = 0, d = 0;
			//    bool toomuchbyte = true; //set true to skip error prefix.

			while (s < size_s && d < size_d)
			{
				unsigned char c = src[s];
				if ((c & 0x80) == 0)
				{
					des[d++] += src[s++];
				}
				else if ((c & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
				{
					wchar_t& wideChar = des[d++];
					wideChar = (src[s + 0] & 0x3F) << 6;
					wideChar |= (src[s + 1] & 0x3F);
					s += 2;
				}
				else if ((c & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
				{
					wchar_t& wideChar = des[d++];

					wideChar = (src[s + 0] & 0x1F) << 12;
					wideChar |= (src[s + 1] & 0x3F) << 6;
					wideChar |= (src[s + 2] & 0x3F);

					s += 3;
				}
				else if ((c & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx
				{
					wchar_t& wideChar = des[d++];

					wideChar = (src[s + 0] & 0x0F) << 18;
					wideChar = (src[s + 1] & 0x3F) << 12;
					wideChar |= (src[s + 2] & 0x3F) << 6;
					wideChar |= (src[s + 3] & 0x3F);
					s += 4;
				}
				else
				{
					wchar_t& wideChar = des[d++]; ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx

					wideChar = (src[s + 0] & 0x07) << 24;
					wideChar = (src[s + 1] & 0x3F) << 18;
					wideChar = (src[s + 2] & 0x3F) << 12;
					wideChar |= (src[s + 3] & 0x3F) << 6;
					wideChar |= (src[s + 4] & 0x3F);
					s += 5;
				}
			}

			return d;
		}
#if 0
		static std::wstring UTF8ToUnicode(std::string const& utf8)
		{
			if (utf8.empty())
			{
				return {};
			}

			std::wstring res;

			for (size_t i = 0; i < utf8.size(); )
			{
				auto c = (unsigned char)utf8[i];
				char32_t wideChar = 0;
				if ((c & 0x80) == 0)
				{
					wideChar = c;
					++i;
				}
				else if ((c & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
				{
					if (i + 2 > utf8.size()) break;
					wideChar = (char32_t(c) & 0x3F) << 6;
					wideChar |= (char32_t(utf8[i + 1]) & 0x3F);
					i += 2;
				}
				else if ((c & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
				{
					if (i + 3 > utf8.size()) break;
					wideChar = (char32_t(c) & 0x1F) << 12;
					wideChar |= (char32_t(utf8[i + 1]) & 0x3F) << 6;
					wideChar |= (char32_t(utf8[i + 2]) & 0x3F);
					i += 3;
				}
				else if ((c & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx
				{
					if (i + 4 > utf8.size()) break;
					wideChar = (char32_t(c) & 0x0F) << 18;
					wideChar |= (char32_t(utf8[i + 1]) & 0x3F) << 12;
					wideChar |= (char32_t(utf8[i + 2]) & 0x3F) << 6;
					wideChar |= (char32_t(utf8[i + 3]) & 0x3F);
					i += 4;
				}
				else///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx
				{
					if (i + 4 > utf8.size()) break;
					wideChar = (char32_t(c) & 0x07) << 24;
					wideChar |= (char32_t(utf8[i + 1]) & 0x3F) << 18;
					wideChar |= (char32_t(utf8[i + 2]) & 0x3F) << 12;
					wideChar |= (char32_t(utf8[i + 3]) & 0x3F) << 6;
					wideChar |= (char32_t(utf8[i + 4]) & 0x3F);
					i += 4;
				}
				res.push_back(wideChar);
			}

			return res;
		}
#endif

	};
}

#include "openglrenderengine.cpp"

#endif 


