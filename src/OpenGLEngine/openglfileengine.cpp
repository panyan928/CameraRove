#ifndef OpenGLFileEngine_CPP
#define OpenGLFileEngine_CPP

#include "openglfileengine.h"
#include "InputVString1.h"
#include <stdio.h>
#include <stdlib.h>

namespace openglEngine {
    /**
    * @brief            getVerticesFromBinary
    * @details          根据输入从二进制制图数据中读取顶点坐标并返回点数组
    * @param path       文件路径
    * @param srs        顶点坐标系统
    * @param demension  坐标维度
    * @param pt_size    顶点个数
    */
	template<typename T>
	T* OpenGLFileEngine::getVerticesFromBinary(const char* path, int srs, int demension, int& pt_size)
	{
        FILE* ifs = fopen(path, "rb");

        if (ifs) {
            //cout << "OK opening " << path << endl;
            
        }
        else {
            //cout << "Error opening " << path << endl;
            //fclose(ifs);
            return 0x00;
        }

        fseek(ifs,0,SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);
        
        int size = len / sizeof(T);
        T* pts = new T[size];
        pt_size = size / demension;
        int k = 0;

        /* 读取二进制数据 */
        fread(reinterpret_cast<char*>(pts),1,len, ifs);
        fclose(ifs);

        if (srs == CGeoUtil::WGS84) {
            if (demension == 2)
                pts = OMGeoUtil::projectLonlat2XYZ<float>(pts, pt_size);
            if (demension == 3)
                pts = OMGeoUtil::projectLonlatHeight2XYZ<float>(pts, pt_size);
        }

        return pts;
	}

    /**
    * @brief                getStopsFromBinary
    * @details              根据输入从二进制制图数据中读取流分割数据\n
    * 包括分割线要素、分割注记串
    * @param path           文件路径
    * @param pointsCount    顶点个数
    * @param stopsCount     分割点个数
    */
    template<typename T>
    T* OpenGLFileEngine::getStopsFromBinary(const char* path, int& pointsCount, int& stopsCount)
    {
        FILE* ifs = fopen(path, "rb");

        if (!ifs) {
            //cout << "Error opening " << path << endl;
            //fclose(ifs);
            return 0x00;
        }
        else {
            //cout << "OK opening " << path << endl;
        }

        fseek(ifs,0,SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);
        int size = len / sizeof(T);

        fread(reinterpret_cast<char*>(&pointsCount),1,sizeof(int), ifs);
        //infile.read(reinterpret_cast<char*>(&pointsCount), sizeof(int));
        int index = 0;

        stopsCount = size - 1;
        T* stops = new T[stopsCount];
        //infile.read(reinterpret_cast<char*>(stops), stopsCount * sizeof(int));
        fread(reinterpret_cast<char*>(stops),1,stopsCount * sizeof(int), ifs);
        fclose(ifs);

        /********************测试数据的正确性************************/
        /*int sum = 0;
        for (int i = 0; i < size - 1; i++) {
            sum += stops[i];
            cout << stops[i] << endl;
        }
        cout << sum << " " << pointNumber << endl;*/
        /********************************************************/

        return stops;
    }

    /**
    * @brief            getIndexFromBinary
    * @details          从二进制文件读取索引数据   
    * @param path       文件路径
    * @param pt_size    索引个数
    */
    template<typename T>
    T* OpenGLFileEngine::getIndexFromBinary(const char* path, int& size)
    {
        FILE* ifs = fopen(path, "rb");

        if (!ifs) {
            //cout << "Error opening " << path << endl;
            //fclose(ifs);
            return 0x00;
        }
        else {
            //cout << "OK opening " << path << endl;
        }

        fseek(ifs,0,SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);
        size = len / sizeof(T);
        T* pts = new T[size];

        /*读取二进制数据*/
        fread(reinterpret_cast<char*>(pts),1,len, ifs);
        fclose(ifs);
        return pts;
    }

    template<typename T>
    T* OpenGLFileEngine::getHeightFromBinary(const char* path, int& pt_size)
    {
        FILE* ifs = fopen(path, "rb");

        if (!ifs) {
            //cout << "Error opening " << path << endl;
            //fclose(ifs);
            return 0x00;
        }
        else {
            //cout << "OK opening " << path << endl;
        }

        fseek(ifs,0,SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);
        pt_size = len / sizeof(T);
        T* pts = new T[pt_size];

        /*读取二进制数据*/
        fread(reinterpret_cast<char*>(pts),1,len, ifs);
        fclose(ifs);
        return pts;
    }

    template<typename T>
    T* openglEngine::OpenGLFileEngine::getTextureCoor(int interval)
    {
        int size = 256 / interval + 2;
        T* texture_coor = new T[size * size * 2];
        int index = 0;
        for (int i = -1 * interval; i < 256 + interval; i = i + interval) {
            for (int j = -1 * interval; j < 256 + interval; j = j + interval) {
                texture_coor[index++] = 1.0 * (j + interval) / (256 + interval);
                texture_coor[index++] = 1.0 * (i + interval) / (256 + interval);
            }
        }
        return texture_coor;
    }

    template<typename T>
    int OpenGLFileEngine::computeCatorDataFromTile(int zoom, int col, int row, int interval, T*& vertices, int*& index, T*& textures, short int* height)
    {
        if (zoom < 0 || row < 0 || row >= pow(2, zoom) || col < 0 || col >= pow(2, zoom))
            return -1;
        
        int size = 256 / interval + 2;

        /******************** 计算坐标 *******************/
        //vertices = new T[size * size * 3];
        //textures = new T[size * size * 2];
        //index = new int[(size * size - 2 * size + 1) * 6];


        Vec2d leftTop, pt;
        CGeoUtil::getTileLeftTop(zoom, col, row, leftTop[0], leftTop[1]);
        double span = 2 * CGeoUtil::PI * CGeoUtil::WGS_84_RADIUS_EQUATOR / pow(2, zoom) / 256 * interval;
        double x=0.0, y=0.0, z=0.0;
        double lon=0.0, lat=0.0;
        int index_t = 0; int index_v = 0; int index_i = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {

                pt[0] = leftTop[0] + (j - 1) * span;
                pt[1] = leftTop[1] - (i - 1) * span;

                CGeoUtil::WebMercator2lonLat(pt[0], pt[1], lat, lon);

                if (height == 0x00)
                    CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, 0, x, y, z);
                else {
                    int h = height[int(1.0 * i / (size) * 64 * 64 + 1.0 * j / (size) * 64)];
                    CGeoUtil::lonLatHeight2XYZ(lon * CGeoUtil::PI / 180, lat * CGeoUtil::PI / 180, 100 * h, x, y, z);
                }
                    
               // CGeoUtil::WebMercator2XYZ(pt[0], pt[1], x, y, z);
                vertices[index_v++] = x;
                vertices[index_v++] = y;
                vertices[index_v++] = z;

                textures[index_t++] = 1.0 * (j) / (size - 1);
                textures[index_t++] = 1.0 * (i) / (size - 1);

                if (i > 0 && j > 0) {
                    index[index_i++] = (i)*size + j;
                    index[index_i++] = (i)*size + j - 1;
                    index[index_i++] = (i - 1) * size + j;

                    index[index_i++] = i * size + j - 1;
                    index[index_i++] = (i - 1) * size + j - 1;
                    index[index_i++] = (i - 1) * size + j;
                }
            }
        }

        return (size * size - 2 * size + 1) * 6;
    }
    
    template <typename T>
    std::string OpenGLFileEngine::getStringFromBinary(const char* path)
    {
        FILE* ifs = fopen(path, "rb");

        if (!ifs) {
            //cout << "Error opening " << path << endl;
            //fclose(ifs);
            return "";
        }
        else {
            //cout << "OK opening " << path << endl;
        }

        fseek(ifs,0,SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);
        int size = len;
        string buffer;
        buffer.resize(size);

        /* 读取二进制数据 */
        char* content = (char*)malloc(len+1);
        memset(content, 0, len+1);
        fread(content,1,len, ifs);
        buffer=content;
        fclose(ifs);
        free(content);

        return buffer;
    }

    template <typename T>
    wchar_t* OpenGLFileEngine::getWchartFromBinary(const char* path)
    {
        FILE* ifs = fopen(path, "rb");
        if (!ifs) {
            //cout << "Error opening " << path << endl;
            //fclose(ifs);
            return NULL;
        }
        else {
            //cout << "OK opening " << path << endl;
        }

        fseek(ifs,0,SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);
        wchar_t buffer[50];
        char temp[50];
        memset(temp,0,50);
        memset(buffer, 0, 50);
        /* 读取二进制数据 */
        fread(const_cast<char*>(temp),1,len, ifs);
        fclose(ifs);

        int des_len=0;
        GB2312toUnicode2wchart(temp, buffer, &des_len);

        return buffer;
    }

    template<typename T>
    T* OpenGLFileEngine::getVerticesFromDB(sqlite3* db,int zoom ,int row ,int col, CGeoUtil::Proj srs, int demension, int& pt_size)
    {
        char* zErrMsg = 0;
        char sql[1000];
        int rc;
        sqlite3_stmt* stmt;
        //读取数据
	#ifdef WIN32
        sprintf_s(sql, "select vertice from Tiles where zoom = %d and row = %d and col = %d;",zoom,row,col);
	#else
        sprintf(sql, "select vertice from Tiles where zoom = %d and row = %d and col = %d;",zoom,row,col);
	#endif

        sqlite3_prepare(db, sql, strlen(sql), &stmt, 0);
        int result = sqlite3_step(stmt);
        int len = 0;
        len = sqlite3_column_bytes(stmt, 0);
        T* pts = new T[len/sizeof(T)];
        const T* pts_tmp = (T*)sqlite3_column_blob(stmt, 0);
        memcpy(pts, pts_tmp, len);
        pt_size = len / sizeof(T) / demension;
        if (srs == CGeoUtil::WGS84) {
            if (demension == 2)
                pts = OMGeoUtil::projectLonlat2XYZ<float>(pts, pt_size);
            if (demension == 3)
                pts = OMGeoUtil::projectLonlatHeight2XYZ<float>(pts, pt_size);
        }
        sqlite3_finalize(stmt);
        return pts;
    }


    template<typename T>
    T* OpenGLFileEngine::getStopsFromDB(sqlite3* db, int zoom, int row, int col, int& pointsCount, int& stopsCount)
    {
        char* zErrMsg = 0;
        char sql[1000];
        int rc;
        sqlite3_stmt* stmt;
        //读取数据
	#ifdef WIN32
        sprintf_s(sql, "select stop from Tiles where zoom = %d and row = %d and col = %d;", zoom, row, col);
	#else
        sprintf(sql, "select stop from Tiles where zoom = %d and row = %d and col = %d;", zoom, row, col);
	#endif

        sqlite3_prepare(db, sql, strlen(sql), &stmt, 0);
        int result = sqlite3_step(stmt);
        int len = 0;
        len = sqlite3_column_bytes(stmt, 0);
        T* stops = new T[len / sizeof(T) - 1];
        stopsCount = len / sizeof(T) - 1;
        T* count = new T();
        const T* pts_tmp = (T*)sqlite3_column_blob(stmt, 0);
        memcpy(count, pts_tmp, sizeof(int));
        pointsCount = *count;
        memcpy(stops, pts_tmp + 1, len - sizeof(T));
        sqlite3_finalize(stmt);
        delete count;
        return stops;
    }

    template<typename T>
    T* OpenGLFileEngine::getHeightFromDB(sqlite3* db, int zoom, int row, int col, int& pt_size)
    {
        char* zErrMsg = 0;
        char sql[1000];
        int rc;
        sqlite3_stmt* stmt;
        //读取数据
        #ifdef WIN32
            sprintf_s(sql, "select height from Tiles where zoom = %d and row = %d and col = %d;", zoom, row, col);
    	#else
            sprintf(sql, "select height from Tiles where zoom = %d and row = %d and col = %d;", zoom, row, col);
    	#endif
        sqlite3_prepare(db, sql, strlen(sql), &stmt, 0);
        int result = sqlite3_step(stmt);
        int len = 0;
        len = sqlite3_column_bytes(stmt, 0);
        pt_size = len / sizeof(T);
        T* pts = new T[len / sizeof(T)];
        const T* pts_tmp = (T*)sqlite3_column_blob(stmt, 0);
        memcpy(pts, pts_tmp, len);
        sqlite3_finalize(stmt);
        return pts;
    }

    template<typename T>
    std::string OpenGLFileEngine::getStringFromDB(sqlite3* db, int zoom, int row, int col)
    {
        char* zErrMsg = 0;
        char sql[1000];
        int rc;
        sqlite3_stmt* stmt;
        //读取数据
        #ifdef WIN32
            sprintf_s(sql, "select anno from Tiles where zoom = %d and row = %d and col = %d;", zoom, row, col);
    	#else
            sprintf(sql, "select anno from Tiles where zoom = %d and row = %d and col = %d;", zoom, row, col);
    	#endif
        sqlite3_prepare(db, sql, strlen(sql), &stmt, 0);
        int result = sqlite3_step(stmt);
        int len = 0;
        len = sqlite3_column_bytes(stmt, 0);
        int size = len;
        string buffer;
        buffer.resize(size);
        const T* pts_tmp = (T*)sqlite3_column_blob(stmt, 0);
        memcpy(const_cast<char*>(buffer.c_str()), pts_tmp, len);
        sqlite3_finalize(stmt);
        return buffer;
    }
    template<typename T>
    T* OpenGLFileEngine::getImgFromDB(sqlite3* db, int zoom, int row, int col, int& size)
    {
        char* zErrMsg = 0;
        char sql[1000];
        int rc;
        sqlite3_stmt* stmt;
        //读取数据
		#ifdef WIN32
			sprintf_s(sql, "select tile_data from tiles where zoom_level = %d and tile_row = %d and tile_column = %d;", zoom, row, col);
		#else
			sprintf(sql, "select tile_data from tiles where zoom_level = %d and tile_row = %d and tile_column = %d;", zoom, row, col);
		#endif

        sqlite3_prepare(db, sql, strlen(sql), &stmt, 0);
        int result = sqlite3_step(stmt);
        int len = 0;
        len = sqlite3_column_bytes(stmt, 0);
        size = len;
        T* img = new T[len / sizeof(T)];
        const T* pts_tmp = (T*)sqlite3_column_blob(stmt, 0);
        memcpy(img, pts_tmp, len);
        sqlite3_finalize(stmt);
        return img;

    }
}

#endif
