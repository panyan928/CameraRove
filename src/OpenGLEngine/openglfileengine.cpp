#ifndef OpenGLFileEngine_CPP
#define OpenGLFileEngine_CPP

#include "openglfileengine.h"
//#include "InputVString1.h"
#include "memory_monitor.h"
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
        // 初始化输出参数，防止未初始化值导致的问题
        pt_size = 0;

        // 额外的安全检查：如果传入的pt_size已经是无效值，立即返回
        // 注意：由于pt_size是引用，我们在函数开始已经初始化为0，这个检查主要作为防御性编程

        FILE* ifs = fopen(path, "rb");

        if (ifs) {
            if (_Debug) cout << "OK opening " << path << endl;

        }
        else {
            if (_Debug) cout << "Error opening " << path << endl;
            //fclose(ifs);
            return 0x00;
        }

        fseek(ifs, 0, SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);

        // 检查文件大小是否有效
        if (len <= 0 || len > 100000000) { // 限制最大文件大小100MB
            if (_Debug) cout << "Invalid file size: " << len << " for file: " << path << endl;
            fclose(ifs);
            return 0x00;
        }

        int size = len / sizeof(T);

        // 添加内存分配异常处理
        if (size <= 0 || size > 10000000) { // 限制最大分配大小
            if (_Debug) cout << "Invalid size: " << size << " for file: " << path << endl;
            fclose(ifs);
            return 0x00;
        }

        // 检查内存分配是否安全
        size_t allocationSize = size * sizeof(T);
        if (!isMemoryAllocationSafe(allocationSize)) {
            if (_Debug) cout << "Memory allocation unsafe for size " << allocationSize << " bytes" << endl;
            fclose(ifs);
            return 0x00;
        }

        T* pts = nullptr;
        try {
            pts = new T[size];
        }
        catch (const std::bad_alloc& e) {
            if (_Debug) cout << "Memory allocation failed for size " << size << ": " << e.what() << endl;
            printMemoryUsage();
            fclose(ifs);
            return 0x00;
        }

        if (!pts) {
            if (_Debug) cout << "Failed to allocate memory for size " << size << endl;
            printMemoryUsage();
            fclose(ifs);
            return 0x00;
        }

        if (_MemoryDebug) {
            cout << "Successfully allocated " << allocationSize / (1024 * 1024) << " MB for " << path << endl;
        }

        // 检查维度参数是否有效
        if (demension <= 0) {
            if (_Debug) cout << "Invalid demension: " << demension << " for file: " << path << endl;
            delete[] pts;
            fclose(ifs);
            return 0x00;
        }

        pt_size = size / demension;
        int k = 0;

        /* 读取二进制数据 */
        fread(reinterpret_cast<char*>(pts), 1, len, ifs);
        fclose(ifs);

        if (srs == CGeoUtil::WGS84) {
            if (demension == 2)
                //                pts = OMGeoUtil::projectLonlat2XYZ<float>(pts, pt_size);
                pts = OMGeoUtil::projectLonlat2Mecator<float>(pts, pt_size);

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
        // 初始化输出参数，防止未初始化值导致的问题
        pointsCount = 0;
        stopsCount = 0;

        FILE* ifs = fopen(path, "rb");

        if (!ifs) {
            if (_Debug) cout << "Error opening " << path << endl;
            //fclose(ifs);
            return 0x00;
        }
        else {
            if (_Debug) cout << "OK opening " << path << endl;
        }

        fseek(ifs, 0, SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);

        // 检查文件大小是否有效
        if (len <= 0 || len > 100000000) { // 限制最大文件大小100MB
            if (_Debug) cout << "Invalid file size: " << len << " for file: " << path << endl;
            fclose(ifs);
            return 0x00;
        }

        int size = len / sizeof(T);

        fread(reinterpret_cast<char*>(&pointsCount), 1, sizeof(int), ifs);
        //infile.read(reinterpret_cast<char*>(&pointsCount), sizeof(int));
        int index = 0;

        stopsCount = size - 1;

        // 添加内存分配异常处理和安全检查
        if (stopsCount <= 0 || stopsCount > 10000000) { // 限制最大分配大小
            if (_Debug) cout << "Invalid stopsCount: " << stopsCount << " for file: " << path << endl;
            fclose(ifs);
            return 0x00;
        }

        // 检查内存分配是否安全
        size_t allocationSize = stopsCount * sizeof(T);
        if (!isMemoryAllocationSafe(allocationSize)) {
            if (_Debug) cout << "Memory allocation unsafe for stops size " << allocationSize << " bytes" << endl;
            fclose(ifs);
            return 0x00;
        }

        T* stops = nullptr;
        try {
            stops = new T[stopsCount];
        }
        catch (const std::bad_alloc& e) {
            if (_Debug) cout << "Memory allocation failed for stops size " << stopsCount << ": " << e.what() << endl;
            printMemoryUsage();
            fclose(ifs);
            return 0x00;
        }

        if (!stops) {
            if (_Debug) cout << "Failed to allocate memory for stops size " << stopsCount << endl;
            printMemoryUsage();
            fclose(ifs);
            return 0x00;
        }

        //infile.read(reinterpret_cast<char*>(stops), stopsCount * sizeof(int));
        fread(reinterpret_cast<char*>(stops), 1, stopsCount * sizeof(int), ifs);
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
        // 初始化输出参数，防止未初始化值导致的问题
        size = 0;

        FILE* ifs = fopen(path, "rb");

        if (!ifs) {
            if (_Debug) cout << "Error opening " << path << endl;
            //fclose(ifs);
            return 0x00;
        }
        else {
            if (_Debug) cout << "OK opening " << path << endl;
        }

        fseek(ifs, 0, SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);

        // 检查文件大小是否有效
        if (len <= 0 || len > 100000000) { // 限制最大文件大小100MB
            if (_Debug) cout << "Invalid file size: " << len << " for file: " << path << endl;
            fclose(ifs);
            return 0x00;
        }

        size = len / sizeof(T);

        // 添加内存分配异常处理和安全检查
        if (size <= 0 || size > 10000000) { // 限制最大分配大小
            if (_Debug) cout << "Invalid size: " << size << " for file: " << path << endl;
            fclose(ifs);
            return 0x00;
        }

        // 检查内存分配是否安全
        size_t allocationSize = size * sizeof(T);
        if (!isMemoryAllocationSafe(allocationSize)) {
            if (_Debug) cout << "Memory allocation unsafe for index size " << allocationSize << " bytes" << endl;
            fclose(ifs);
            return 0x00;
        }

        T* pts = nullptr;
        try {
            pts = new T[size];
        }
        catch (const std::bad_alloc& e) {
            if (_Debug) cout << "Memory allocation failed for index size " << size << ": " << e.what() << endl;
            printMemoryUsage();
            fclose(ifs);
            return 0x00;
        }

        if (!pts) {
            if (_Debug) cout << "Failed to allocate memory for index size " << size << endl;
            printMemoryUsage();
            fclose(ifs);
            return 0x00;
        }

        /*读取二进制数据*/
        fread(reinterpret_cast<char*>(pts), 1, len, ifs);
        fclose(ifs);
        return pts;
    }

    template<typename T>
    T* OpenGLFileEngine::getHeightFromBinary(const char* path, int& pt_size)
    {
        // 初始化输出参数，防止未初始化值导致的问题
        pt_size = 0;

        FILE* ifs = fopen(path, "rb");

        if (!ifs) {
            if (_Debug) cout << "Error opening " << path << endl;
            //fclose(ifs);
            return 0x00;
        }
        else {
            if (_Debug) cout << "OK opening " << path << endl;
        }

        fseek(ifs, 0, SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);

        // 检查文件大小是否有效
        if (len <= 0 || len > 100000000) { // 限制最大文件大小100MB
            if (_Debug) cout << "Invalid file size: " << len << " for file: " << path << endl;
            fclose(ifs);
            return 0x00;
        }

        pt_size = len / sizeof(T);

        // 添加内存分配异常处理和安全检查
        if (pt_size <= 0 || pt_size > 10000000) { // 限制最大分配大小
            if (_Debug) cout << "Invalid pt_size: " << pt_size << " for file: " << path << endl;
            fclose(ifs);
            return 0x00;
        }

        // 检查内存分配是否安全
        size_t allocationSize = pt_size * sizeof(T);
        if (!isMemoryAllocationSafe(allocationSize)) {
            if (_Debug) cout << "Memory allocation unsafe for height size " << allocationSize << " bytes" << endl;
            fclose(ifs);
            return 0x00;
        }

        T* pts = nullptr;
        try {
            pts = new T[pt_size];
        }
        catch (const std::bad_alloc& e) {
            if (_Debug) cout << "Memory allocation failed for height size " << pt_size << ": " << e.what() << endl;
            printMemoryUsage();
            fclose(ifs);
            return 0x00;
        }

        if (!pts) {
            if (_Debug) cout << "Failed to allocate memory for height size " << pt_size << endl;
            printMemoryUsage();
            fclose(ifs);
            return 0x00;
        }

        /*读取二进制数据*/
        fread(reinterpret_cast<char*>(pts), 1, len, ifs);
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
        double x = 0.0, y = 0.0, z = 0.0;
        double lon = 0.0, lat = 0.0;
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
            if (_Debug) cout << "Error opening " << path << endl;
            //fclose(ifs);
            return "";
        }
        else {
            if (_Debug) cout << "OK opening " << path << endl;
        }

        fseek(ifs, 0, SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);
        int size = len;
        string buffer;
        buffer.resize(size);

        /* 读取二进制数据 */
        char* content = (char*)malloc(len + 1);
        memset(content, 0, len + 1);
        fread(content, 1, len, ifs);
        buffer = content;
        fclose(ifs);
        free(content);

        return buffer;
    }

    template<typename T>
    T* OpenGLFileEngine::getVerticesFromMemory(const unsigned char* data, size_t len,
        int srs, int demension, int& pt_size)
    {
        pt_size = 0;
        if (!data || len == 0 || demension <= 0)
            return 0x00;

        if (len > 100000000)
            return 0x00;

        int size = static_cast<int>(len / sizeof(T));
        if (size <= 0 || size > 10000000)
            return 0x00;

        size_t allocationSize = size * sizeof(T);
        if (!isMemoryAllocationSafe(allocationSize))
            return 0x00;

        // 使用malloc代替new，避免嵌入式系统内核内存分配失败
        // 与卫星dat绘制方式保持一致
        T* pts = static_cast<T*>(malloc(allocationSize));
        if (!pts) {
            if (_Debug) cout << "Memory allocation failed (malloc) for size " << allocationSize << " bytes" << endl;
            return 0x00;
        }

        memcpy(pts, data, allocationSize);
        pt_size = size / demension;

        if (srs == CGeoUtil::WGS84) {
            // 投影函数使用delete[]释放输入，需要转换为new分配的内存
            // 如果new失败，不进行投影，返回malloc分配的内存（调用者需要使用free释放）
            T* pts_new = nullptr;
            try {
                pts_new = new T[size];
                if (pts_new) {
                    memcpy(pts_new, pts, allocationSize);
                    free(pts);  // 释放malloc分配的内存
                    pts = pts_new;

                    if (demension == 2)
                        pts = OMGeoUtil::projectLonlat2Mecator<float>(pts, pt_size);
                    if (demension == 3)
                        pts = OMGeoUtil::projectLonlatHeight2XYZ<float>(pts, pt_size);
                    // 投影成功，内存由new分配，调用者应传递useMalloc=false（默认值）
                }
                else {
                    // new失败，保持使用malloc分配的内存，不进行投影
                    if (_Debug) cout << "Memory allocation failed (new), skipping projection, using malloc allocated memory" << endl;
                }
            }
            catch (...) {
                // new失败，保持使用malloc分配的内存，不进行投影
                if (_Debug) cout << "Memory allocation failed (new), skipping projection, using malloc allocated memory" << endl;
            }
        }

        return pts;
    }

    template<typename T>
    T* OpenGLFileEngine::getStopsFromMemory(const unsigned char* data, size_t len,
        int& pointsCount, int& stopsCount)
    {
        pointsCount = 0;
        stopsCount = 0;
        if (!data || len < sizeof(int))
            return 0x00;

        pointsCount = *reinterpret_cast<const int*>(data);
        size_t remain = len - sizeof(int);
        if (remain % sizeof(T) != 0)
            return 0x00;

        stopsCount = static_cast<int>(remain / sizeof(T));
        if (stopsCount <= 0 || stopsCount > 10000000)
            return 0x00;

        size_t allocationSize = stopsCount * sizeof(T);
        if (!isMemoryAllocationSafe(allocationSize))
            return 0x00;

        // 使用malloc代替new，避免嵌入式系统内核内存分配失败
        // 与卫星dat绘制方式保持一致
        T* stops = static_cast<T*>(malloc(allocationSize));
        if (!stops) {
            if (_Debug) cout << "Memory allocation failed (malloc) for stops size " << allocationSize << " bytes" << endl;
            return 0x00;
        }

        memcpy(stops, data + sizeof(int), allocationSize);
        return stops;
    }

    template<typename T>
    std::string OpenGLFileEngine::getStringFromMemory(const unsigned char* data, size_t len)
    {
        if (!data || len == 0)
            return "";
        return std::string(reinterpret_cast<const char*>(data), len);
    }

    template <typename T>
    wchar_t* OpenGLFileEngine::getWchartFromBinary(const char* path)
    {
        FILE* ifs = fopen(path, "rb");
        if (!ifs) {
            if (_Debug) cout << "Error opening " << path << endl;
            //fclose(ifs);
            return NULL;
        }
        else {
            if (_Debug) cout << "OK opening " << path << endl;
        }

        fseek(ifs, 0, SEEK_END);
        int len = ftell(ifs);
        fseek(ifs, 0, SEEK_SET);
        wchar_t buffer[50];
        char temp[50];
        memset(temp, 0, 50);
        memset(buffer, 0, 50);
        /* 读取二进制数据 */
        fread(const_cast<char*>(temp), 1, len, ifs);
        fclose(ifs);

        int des_len = 0;
        GB2312toUnicode2wchart(temp, buffer, &des_len);

        return buffer;
    }


    // 显式模板实例化
    template float* OpenGLFileEngine::getVerticesFromMemory<float>(const unsigned char* data, size_t len, int srs, int demension, int& pt_size);
    template int* OpenGLFileEngine::getStopsFromMemory<int>(const unsigned char* data, size_t len, int& pointsCount, int& stopsCount);
    template std::string OpenGLFileEngine::getStringFromMemory<char>(const unsigned char* data, size_t len);
}

#endif
