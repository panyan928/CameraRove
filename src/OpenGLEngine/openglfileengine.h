#ifndef OpenGLFileEngine_H
#define OpenGLFileEngine_H

#include "../TMUtil/cgeoutil.h"
#include "../TMUtil/OMGeoUtil.h"
#include "sqlite/sqlite3.h"
#include <iostream>
#include <fstream>
using namespace std;

namespace openglEngine {
	class OpenGLFileEngine
	{
	public:
		template <typename T>
		static T* getVerticesFromBinary(const char* path, int srs, int demension, int& pt_size);

		template <typename T>
		static T* getStopsFromBinary(const char* path, int& pointsCount, int& stopsCount);

		template <typename T>
		static T* getIndexFromBinary(const char* path, int& pt_size);

		template <typename T>
		static T* getHeightFromBinary(const char* path, int& pt_size);

		template <typename T>
		static T* getTextureCoor(int interval);

		template <typename T>
		static int computeCatorDataFromTile(int zoom, int col, int row, int interval, T* &vertices, int* &index, T* &textures, short int* height);
		
		template <typename T>
		static std::string getStringFromBinary(const char* path);

		template <typename T>
		static T* getVerticesFromDB(sqlite3* db,int zoom,int row ,int col, CGeoUtil::Proj srs, int demension, int& pt_size);

		template <typename T>
		static T* getStopsFromDB(sqlite3* db, int zoom, int row, int col, int& pointsCount, int& stopsCount);

		template <typename T>
		static T* getHeightFromDB(sqlite3* db, int zoom, int row, int col, int& pt_size);

		template <typename T>
		static std::string getStringFromDB(sqlite3* db, int zoom, int row, int col );

		template <typename T>
		static T* getImgFromDB(sqlite3* db, int zoom, int row, int col, int& len);

        template <typename T>
		static wchar_t* getWchartFromBinary(const char* path);

	};
}

#include"openglfileengine.cpp"

#endif

