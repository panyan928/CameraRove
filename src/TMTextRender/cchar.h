#ifndef CCHAR_H
#define CCHAR_H

/**
  * @file     	cchar.h
  * @author   	MYL  yunlongma0@163.com
  * @version	V1.0
  * @date    	18-06-2021
  * @brief   	char class
  * @attention    主要存储字符的字形数据、字符编码、类型和位图数据
  */

#include <cstring>
#include "tmtools.h"

namespace textRender {
    struct glyphMetrics
    {
        int    width;
        int    height;
        int    horiBearingX;
        int    horiBearingY;
        int    horiAdvance;
        int    vertBearingX;
        int    vertBearingY;
        int    vertAdvance;
    };

    enum ImageType {
        Gray,   /**< GRAY模式1个像素用1个字节保�?*/ 
        Mono    /**< MONO模式�?个像素仅�?bit保存，只有黑和白 */ 
    };

    class CChar
    {
    public:
        CChar(int code, ImageType type);
        virtual ~CChar();

        void setInfo(glyphMetrics* metrics);
        void getInfo(glyphMetrics* metrics);
        unsigned char* getOrCreateBuffer(size_t size);
        //unsigned char& operator [] (size_t i);
        int code() const;
        ImageType type() const;


    private:
        int              m_code;            /**< char code. */
        unsigned char*   m_data;            /**< char bitmap data. */
        glyphMetrics     m_metrics;         /**< char metrics data. */
        size_t           m_size;            /**< char bitmap data size. */
        ImageType        m_type;            /**< char bitmap type. */
    };
}
#endif // CCHAR_H
