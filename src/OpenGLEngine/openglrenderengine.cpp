#ifndef OpenGLRenderEngine_CPP
#define OpenGLRenderEngine_CPP

#include "openglrenderengine.h"
#include "../libs/stb_image.h"
#include "InputVString1.h"
#include <list>

namespace openglEngine {

//    int OpenGLRenderEngine::hud = 0;
//    int OpenGLRenderEngine::v = 0;
//    int OpenGLRenderEngine::t = 0;
    template<typename T>
	int OpenGLRenderEngine::drawHeight(T* pts, T* color, int* index, int size)
	{
		if (pts == 0x00)
			return -1;
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		glShadeModel(GL_FLAT);
		glEnableClientState(GL_VERTEX_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, pts);
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, color);
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, index);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		//glDisable(GL_CULL_FACE);

		tCount += size / 3;
		vCount += size;

		return 0;
	}

	template<typename T>
	int OpenGLRenderEngine::drawTriangles(T* pts, Color color, int size)
	{
		if (pts == 0x00)
			return -1;
		// glDisable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnableClientState(GL_VERTEX_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, 0, &pts[0]);
		glColor3f(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);
		glDrawArrays(GL_TRIANGLES, 0, size);
		
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GL_CULL_FACE);
		// glEnable(GL_TEXTURE_2D);

        tCount += size / 3;
        vCount += size;

		return 0;
	}

	template<typename T>
	int OpenGLRenderEngine::drawTriangles(T* pts, int* index, Color color, int size)
	{
		if (pts == 0x00)
			return -1;
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnableClientState(GL_VERTEX_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, &pts[0]);
		glColor3f(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, index);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GL_CULL_FACE);

        tCount += size / 3;
        vCount += size;

		return 0;
	}

	template<typename T>
	int OpenGLRenderEngine::drawLines(T* pts, int* stops, int stopsCount, Color color, int lineWidth, int dashArray)
	{
		if (pts == 0x00)
			return -1;

		glDisable(GL_BLEND);
		//glDisable(GL_TEXTURE_2D);
		glEnable(GL_LINE_SMOOTH); 
		glHint(GL_LINE_SMOOTH, GL_NICEST);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &pts[0]);
		glColor3f(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);
		int index = 0;
		for (int i = 0; i < stopsCount; i++) {
			if (dashArray != 0) {
				glEnable(GL_LINE_STIPPLE);
				glLineStipple(1, 0xF800);
				glLineWidth(lineWidth);
				glDrawArrays(GL_LINE_STRIP, index, stops[i]);
				index += stops[i];
				glDisable(GL_LINE_STIPPLE);
			}
			else {
				glLineWidth(lineWidth);
				glDrawArrays(GL_LINE_STRIP, index, stops[i]);
				index += stops[i];
			}
		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		// glEnable(GL_TEXTURE_2D);
        vCount += stops[stopsCount - 1];

		return 0;
	}

	template<typename T>
	int OpenGLRenderEngine::drawRasters(T* pts, T* texture_coor, int* index, const char* texture_path, int pt_size)
	{
		return 0;
	}

	template<typename T>
	int OpenGLRenderEngine::drawRasters(T* pts, T* texture_coor, int* index, unsigned char* image, int width, int height, int nrChannels, int pt_size)
	{
		if (pts == 0x00 || texture_coor == 0x00 || index == 0x00 || pt_size == 0 || image == 0x00)
			return -1;
		static std::list<GLuint> listTextures;
		if (listTextures.size() >= 16) {			
			glDeleteTextures(1, &(listTextures.back()));
			listTextures.pop_back();
		}

		GLuint texture;

		glEnable(GL_TEXTURE_2D);//启用纹理贴图
		glDepthMask(GL_FALSE);//关掉深度测试
		glGenTextures(1, &texture);
		listTextures.push_front(texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glEnable(GL_BLEND); //开混合模式贴图
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 指定混合模式算法
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
#if 1
		glDisable(GL_DEPTH_TEST);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY); //启用顶点数组

		glTexCoordPointer(2, GL_FLOAT, 0, texture_coor);		
		glVertexPointer(3, GL_FLOAT, 0, pts); //设置顶点数组属性
		glDrawElements(GL_TRIANGLES, pt_size, GL_UNSIGNED_INT, index);//GL_TRIANGLES
		//glDrawArrays(GL_TRIANGLES, 0, pt_size);
		/*glBegin(GL_TRIANGLES);
		for (int i = 0; i < pt_size; i++) {
			glArrayElement(index[i]);
		}
		glEnd();*/
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
#else 
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, 1024, 0, 768, -10.0, 10.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();		
		
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glTexCoord2d(0.0, 0.0); glVertex2f(0.0, 0.0);
		glTexCoord2d(1.0, 0.0); glVertex2f(200.0, 0.0);
		glTexCoord2d(1.0, 1.0); glVertex2f(200.0, 200.0);
		glTexCoord2d(0.0, 1.0); glVertex2f(0.0, 200.0);		
		glEnd();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
#endif
		//glDeleteTextures(1, &texture);
		glDisable(GL_TEXTURE_2D);
		//glDisable(GL_CULL_FACE);
		//glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);

        //tCount += pt_size / 3;
        //vCount += pt_size;		
		return 0;
	}

	template<typename T>
	int OpenGLRenderEngine::drawRasters(T* pts, T* texture_coor, int* index, unsigned char* image, int width, int height, int nrChannels, int pt_size, string level2Index)
	{
		if (pts == 0x00 || texture_coor == 0x00 || index == 0x00 || pt_size == 0 || image == 0x00)
			return -1;
		static GLuint Textures[48] = {0};
		static string Level2Indexs[48] = {""};
		static int textureIndex = 0;
		int i=0;

		glEnable(GL_TEXTURE_2D);//启用纹理贴图
		glDepthMask(GL_FALSE);//关掉深度测试

		if (!Textures[0])/*��ʼ��*/
		{
			glGenTextures(48, Textures);
		}

		for (i = 0; i < 48; i++)
		{
			if (Level2Indexs[i].compare(level2Index) == 0) break;
		}
		if (i < 48)
		{
			glBindTexture(GL_TEXTURE_2D, Textures[i]);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, Textures[textureIndex]);
			Level2Indexs[textureIndex] = level2Index;
			textureIndex++;
			if (textureIndex == 48) textureIndex = 0;
			if (nrChannels == 3)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		}
		glDisable(GL_BLEND);
		//glEnable(GL_BLEND); //开混合模式贴图
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// 指定混合模式算法
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glDisable(GL_DEPTH_TEST);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY); //启用顶点数组

		glTexCoordPointer(2, GL_FLOAT, 0, texture_coor);
		glVertexPointer(3, GL_FLOAT, 0, pts); //设置顶点数组属性
		glDrawElements(GL_TRIANGLES, pt_size, GL_UNSIGNED_INT, index);//GL_TRIANGLES
		//glDrawArrays(GL_TRIANGLES, 0, pt_size);
		/*glBegin(GL_TRIANGLES);
		for (int i = 0; i < pt_size; i++) {
			glArrayElement(index[i]);
		}
		glEnd();*/
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		//glDeleteTextures(1, &texture);
		glDisable(GL_TEXTURE_2D);
		//glDisable(GL_CULL_FACE);
		//glDisable(GL_BLEND);
		//glDepthMask(GL_TRUE);

		//tCount += pt_size / 3;
		//vCount += pt_size;

		return 0;
	}

    template<typename T>
	int OpenGLRenderEngine::drawTIN(T* pts, T* texture_coor, int* index, int pt_size, string level2Index)
	{
		if (pts == 0x00 || texture_coor == 0x00 || index == 0x00 || pt_size == 0 )
			return -1;
		static GLuint Textures[48] = {0};
		static string Level2Indexs[48] = {""};
		static int textureIndex = 0;
		int i=0;

		glEnable(GL_TEXTURE_2D);//����������ͼ
		glDepthMask(GL_FALSE);//�ص���Ȳ���

		if (!Textures[0])/*��ʼ��*/
		{
			glGenTextures(48, Textures);
		}

		for (i = 0; i < 48; i++)
		{
			if (Level2Indexs[i].compare(level2Index) == 0) break;
		}
		if (i < 48)
		{
			glBindTexture(GL_TEXTURE_2D, Textures[i]);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, Textures[textureIndex]);
			Level2Indexs[textureIndex] = level2Index;
			textureIndex++;
			if (textureIndex == 48) textureIndex = 0;
		}
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_BLEND); //�����ģʽ��ͼ
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);// ָ�����ģʽ�㷨
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glDisable(GL_DEPTH_TEST);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY); //���ö�������

		glTexCoordPointer(2, GL_FLOAT, 0, texture_coor);
		glVertexPointer(3, GL_FLOAT, 0, pts); //���ö�����������
#if 0
		glDrawElements(GL_LINES, pt_size, GL_UNSIGNED_INT, index);//GL_TRIANGLES
#else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, pt_size, GL_UNSIGNED_INT, index);//GL_TRIANGLES
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif
		//glDrawArrays(GL_TRIANGLES, 0, pt_size);
		/*glBegin(GL_TRIANGLES);
		for (int i = 0; i < pt_size; i++) {
			glArrayElement(index[i]);
		}
		glEnd();*/
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);


		//glDeleteTextures(1, &texture);
		//glDisable(GL_TEXTURE_2D);
		//glDisable(GL_CULL_FACE);
		//glDisable(GL_BLEND);
		//glDepthMask(GL_TRUE);

		//tCount += pt_size / 3;
		//vCount += pt_size;

		return 0;
	}

    template<typename T>
	int OpenGLRenderEngine::drawRasters(T* pts, T* texture_coor, int* index, unsigned char* image, GLuint* texture, int width, int height, int nrChannels, int pt_size)
	{
		return 0;
	}

#if 0
	template<typename T>
	int OpenGLRenderEngine::drawAnnotations(T* pts, int pt_size, string& buffer, int* stops, Color color, textRender::CFontRender* render)
	{
		if (pts == 0x00 || stops == 0x00 || pt_size == 0 || render == 0x00)
			return -1;

		Vec3d world;
		Vec3d screen;
		int index = 0;
		for (int i = 0; i < pt_size; i++) {
			world[0] = pts[3 * i];
			world[1] = pts[3 * i + 1];
			world[2] = pts[3 * i + 2];

			string name = buffer.substr(index, stops[i]);
			index = index + stops[i];

			openGLCoordinatesEngine::world2Screen(screen[0], screen[1], screen[2], world[0], world[1], world[2]);
			Recti item(screen[0], screen[1], screen[0] + name.size() * 9, screen[1] + 18);
			
			if (checkOverlap(item, render->getData())) continue;
			
			wchar_t wstr[100];
			UTF2Unicode(wstr, 100, const_cast<char*>(name.c_str()));
			std::wstring anno = wstr;
			//std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> decode(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
			//anno = decode.from_bytes(name);
			
			//wcout << anno << endl;		
			//if (screen[0] >= 0 && (name.size() * 36 + screen[0]) <= 1024 && screen[1] >= 0 && (screen[1] + 18) <= 768)
			
			render->render(anno, Vec2i(screen[0], screen[1]), 14, "SIMHEI", color);
		}
		//render->render()

        vCount += pt_size;
        hudCount += pt_size;

		return 0;
	}
#endif
	

	template<typename T>
	int OpenGLRenderEngine::drawAnnotations(T* pts, int pt_size, string& buffer, int* stops, TMStyle::CStyle* style, textRender::CFontRender* render, int anno)
	{
		if (pts == 0x00 || pt_size == 0 || render == 0x00)
			return -1;

		Vec3d world;
		Vec3d screen;

		string fontName;
		try {
			fontName = style->text(0)->font()->font();
		}
		catch (const std::out_of_range& e) {
			char errorBuffer[1024];
			int result = snprintf(errorBuffer, sizeof(errorBuffer), "POINT5 %s::::%s:::%d\n", e.what(), __FILE__, __LINE__);
			OutputDebugStringA(errorBuffer);
			return -1;
		}
		catch (const std::exception& e) {
			char errorBuffer[1024];
			int result = snprintf(errorBuffer, sizeof(errorBuffer), "POINT6 %s::::%s:::%d\n", e.what(), __FILE__, __LINE__);
			OutputDebugStringA(errorBuffer);
			return -1;
		}
		catch (...) {
			char errorBuffer[1024];
			int result = snprintf(errorBuffer, sizeof(errorBuffer), "POINT7 ::::%s:::%d\n", __FILE__, __LINE__);
			OutputDebugStringA(errorBuffer);
			return -1;
		}

		int fontSize = style->text(0)->font()->fontSize();
		TMStyle::CColor* color = style->text(0)->getColor();
		float r = 0.0f, g = 0.0f, b = 0.0f;
		color->colorRGB(r, g, b);

		// === 新增：将 buffer 按 \n 拆分为 labels ===
		std::vector<std::string> labels;
		size_t start = 0;
		for (size_t i = 0; i <= buffer.size(); ++i) {
			if (i == buffer.size() || buffer[i] == '\n') {
				if (i > start) {
					labels.push_back(buffer.substr(start, i - start));
				}
				else {
					labels.push_back("");  // 空行也保留位置
				}
				start = i + 1;
			}
		}

		// === 遍历每个点，绑定 labels[i] ===
		int drawCount = 0;
		int labelCount = static_cast<int>(labels.size());

		for (int i = 0; i < pt_size && i < labelCount; ++i) {
			world[0] = pts[3 * i];
			world[1] = pts[3 * i + 1];
			world[2] = pts[3 * i + 2];

			// 获取第 i 个标注
			string name = labels[i];
			//cout << "name" << name << endl;

			// 可选：跳过空文本
			if (name.empty()) {
				continue;
			}

			// 坐标转换
			openGLCoordinatesEngine::world2Screen(screen[0], screen[1], screen[2], world[0], world[1], world[2]);

			// 计算宽高
			int height = 0;
			int width = 0;
			getHeightAndWidth(width, height, name);
			height *= fontSize;
			width = fontSize * width / 2;

			// 防重叠检测 - 最大化检测范围，确保标注之间完全不重叠
			float expandFactor = 1.0f; // 扩大1倍
			int bufferPixels = 1; // 添加固定20像素的缓冲区
			int expandedWidth = static_cast<int>(width * expandFactor / 2) + bufferPixels;
			int expandedHeight = static_cast<int>(height * expandFactor / 2) + bufferPixels;
			Recti item(screen[0] - expandedWidth, screen[1] - expandedHeight, screen[0] + expandedWidth, screen[1] + expandedHeight);

			// 检查是否为airport文件夹下的文字
			// 引入全局变量g_isAirportMode，用于标识当前是否为airport模式
			extern bool g_isAirportMode;

			// airport文件夹下的文字具有最高优先级，不进行防重叠检测
			// 当处于airport模式且文字为红色时（RGB值为红色系），视为airport文件夹下的文字
			bool isAirportText = g_isAirportMode && r > 0.8f && g < 0.2f && b < 0.2f;

			// 只有非airport文字才进行防重叠检测
			if (!isAirportText && checkOverlap(item, render->getData(), anno)) {
				continue;
			}

			// 渲染
			char* name_char = const_cast<char*>(name.c_str());
			render->render(name_char, Vec2i(screen[0] - width / 2, screen[1] - height / 2), fontSize, fontName.c_str(), Color(255 * r, 255 * g, 255 * b, 255));
			++drawCount;

		}

		vCount += drawCount;
		hudCount += drawCount;

		return 0;
	}
#if 0
	template<typename T>
	int OpenGLRenderEngine::drawAnnotations(T* pts, int pt_size, string& buffer, int* stops, int symbol, TMStyle::CStyle* style, textRender::CFontRender* render)
	{
		if (pts == 0x00 || stops == 0x00 || pt_size == 0 || render == 0x00)
			return -1;

		Vec3d world;
		Vec3d screen;
		int index = 0;
		for (int i = 0; i < pt_size; i++) {
			world[0] = pts[3 * i];
			world[1] = pts[3 * i + 1];
			world[2] = pts[3 * i + 2];

			string name = buffer.substr(index, stops[i]);
			index = index + stops[i];

			string fontName = style->text(0)->font()->font();
			int fontSize = style->text(0)->font()->fontSize();
			TMStyle::CColor* color = style->text(0)->getColor();
			float r, g, b;
			color->colorRGB(r, g, b);

			openGLCoordinatesEngine::world2Screen(screen[0], screen[1], screen[2], world[0], world[1], world[2]);
			/*int height = fontSize;
			int widht = fontSize * name.size() / 2;
			Recti item(screen[0], screen[1], screen[0] + name.size() * fontSize / 2, screen[1] + fontSize);*/
			//Recti item(screen[0] - widht / 2, screen[1] - height / 2, screen[0] + widht / 2, screen[1]  height / 2);
			
			int height = 0;
			int width = 0;
			getHeightAndWidth(width, height, name);
			height *= fontSize;
			width = fontSize * width / 2;
			Recti item(screen[0] - width / 2, screen[1] - height / 2, screen[0] + width / 2, screen[1] + height / 2);
			if (checkOverlap(item, render->getData())) continue;

			wchar_t wstr[100];
			UTF2Unicode(wstr, 100, const_cast<char*>(name.c_str()));
            std::wstring anno = wstr;
            //std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> decode(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
            //anno = decode.from_bytes(name);

			//wcout << anno << endl;		
			//if (screen[0] >= 0 && (name.size() * 36 + screen[0]) <= 1024 && screen[1] >= 0 && (screen[1] + 18) <= 768)

			render->render(anno, Vec2i(screen[0] - width / 2, screen[1] - height / 2), fontSize, fontName.c_str(), Color(255, 255 * r, 255 * g, 255 * b));
		}
		//render->render()
        vCount += pt_size;
        hudCount += pt_size;
		return 0;
	}
#endif
	template<typename T>
	int OpenGLRenderEngine::drawSymbolsAndAnnotations(T* pts, int pt_size, const char* path, string& buffer, int* stops, TMStyle::CStyle* style, textRender::CFontRender* render)
	{
		int width= 0, height= 0, nrChannels= 0;
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (!data) {
			cout << "load symbol file error!" << endl;
			return -1;
		}

		Vec3d world;
		Vec3d screen;
		int index = 0;

		string fontName = style->text(0)->font()->font();
		int fontSize = style->text(0)->font()->fontSize();
		TMStyle::CColor* color = style->text(0)->getColor();
		float r=0.0, g=0.0, b=0.0;
		color->colorRGB(r, g, b);

		unsigned int* pixels = render->getData();
		for (int i = 0; i < pt_size; i++) {
			world[0] = pts[3 * i];
			world[1] = pts[3 * i + 1];
			world[2] = pts[3 * i + 2];

			openGLCoordinatesEngine::world2Screen(screen[0], screen[1], screen[2], world[0], world[1], world[2]);
			Recti item(screen[0] - width / 2, screen[1] - height / 2, screen[0] + width / 2, screen[1] + 1 * height / 2);

			if (!checkOverlap(item, pixels, 6)) {
				for (int i = item[1]; i < item[3]; i++) {
					memcpy(pixels + (i * 1024 + item[0]) * 4, data + ((i - item[1]) * width + item[0] - item[0]) * nrChannels, nrChannels * width);
				}
			}

			string name = buffer.substr(index, stops[i]);
            char *name_char = (char*)name.data();
            
			wchar_t wchar[50];
			memset(wchar, 0, 50);
            int des_len=0;            
            //GB2312toUnicode2wchart(name_char, wchar,&des_len);
			index = index + stops[i];
			Recti item_anno(item[2], item[1], item[2] + name.size() * fontSize / 2, item[3] + fontSize);

			if (!checkOverlap(item_anno, pixels)) {	
				wchar_t wstr[100];
				//UTF2Unicode(wstr, 100, const_cast<char*>(name.c_str()));
				//std::wstring anno = wstr;
				//std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> decode(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));
				//anno = decode.from_bytes(name);

				//wcout << anno << endl;		
				//if (screen[0] >= 0 && (name.size() * 36 + screen[0]) <= 1024 && screen[1] >= 0 && (screen[1] + 18) <= 768)

            render->render(name_char, Vec2i(item[2] + 3, item[1]), fontSize, fontName.c_str(), Color(255, 255 * r, 255 * g, 255 * b));
			}
		}
        vCount += pt_size;
        hudCount += 2 * pt_size;
		return 0;
	}


	template<typename T>
	int OpenGLRenderEngine::drawSymbols(T* pts, int pt_size, const char* path, textRender::CFontRender* render)
	{
		int width= 0, height= 0, nrChannels= 0;
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (!data) {
			cout << "load symbol file error!" << endl;
			return -1;
		}
		Vec3d world;
		Vec3d screen;
		unsigned int* pixels = render->getData();
		for (int i = 0; i < pt_size; i++) {

			world[0] = pts[3 * i];
			world[1] = pts[3 * i + 1];
			world[2] = pts[3 * i + 2];

			openGLCoordinatesEngine::world2Screen(screen[0], screen[1], screen[2], world[0], world[1], world[2]);
			//Recti item(screen[0], screen[1], screen[0] + width, screen[1] + height);
			Recti item(screen[0] - width / 2, screen[1] - height / 2, screen[0] + width / 2, screen[1] + 1 * height / 2);
			
			if (checkOverlap(item, pixels, 6)) continue;

			for (int i = item[1]; i < item[3]; i++) {
				memcpy(pixels + (i * 1024 + item[0]) * 4, data + ((i - item[1]) * width + item[0] - item[0]) * nrChannels, nrChannels * width);
				//for (int j = item[0]; j < item[2]; j++) {
				//	if (nrChannels == 3) {
				//		pixels[(i * 1024 + j) * 4 + 0] = data[((i - item[1]) * width + j - item[0]) * nrChannels + 0];
				//		pixels[(i * 1024 + j) * 4 + 1] = data[((i - item[1]) * width + j - item[0]) * nrChannels + 1];
				//		pixels[(i * 1024 + j) * 4 + 2] = data[((i - item[1]) * width + j - item[0]) * nrChannels + 2];
				//		pixels[(i * 1024 + j) * 4 + 3] = 255;
				//	}
				//	else {
				//		memcpy(pixels + (i * 1024 + j) * 4, data + ((i - item[1]) * width + j - item[0]) * nrChannels, nrChannels * width);
				//		/*pixels[(i * 1024 + j) * 4 + 0] = data[((i - item[1]) * width + j - item[0]) * nrChannels + 0];
				//		pixels[(i * 1024 + j) * 4 + 1] = data[((i - item[1]) * width + j - item[0]) * nrChannels + 1];
				//		pixels[(i * 1024 + j) * 4 + 2] = data[((i - item[1]) * width + j - item[0]) * nrChannels + 2];
				//		pixels[(i * 1024 + j) * 4 + 3] = data[((i - item[1]) * width + j - item[0]) * nrChannels + 3];*/
				//	}
				//}
			}

		}

        hudCount += 1 * pt_size;
		return 0;
	}

}

#endif
