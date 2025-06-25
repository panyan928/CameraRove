#include "cchar.h"

/**
 * @brief
 * @param metrics
 */
namespace textRender {
    CChar::CChar(int code, ImageType type)
    {
        m_data = 0x00;
        m_code = code;
        m_type = type;
    }

    CChar::~CChar()
    {
        if (m_data != 0x00)
            delete[] m_data;
        m_data = 0x00;
    }

    void CChar::setInfo(glyphMetrics *metrics)
    {
        memcpy(&m_metrics, metrics, sizeof(glyphMetrics));
        m_size = m_metrics.height * m_metrics.width;
    }

    void CChar::getInfo(glyphMetrics *metrics)
    {
        memcpy(metrics, &m_metrics, sizeof(glyphMetrics));
    }

    unsigned char* CChar::getOrCreateBuffer(size_t size)
    {
        if (m_data == 0x00) {
            m_data = new unsigned char[size];
            memset(m_data, 0, size);
        }
        return m_data;
    }


    int CChar::code() const
    {
        return m_code;
    }

    ImageType CChar::type() const
    {
        return m_type;
    }

}
