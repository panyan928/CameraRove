#include "tmbuffer.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb_image_write.h"
TMBuffer::~TMBuffer()
{
    if (_Texture)
        delete _Texture;
    if (_cData)
        delete _cData;
    if (_vData)
        delete _vData;
    if (_hData)
        delete _hData;
    if (_iData)
        delete _iData;
    if (_tData)
        delete _tData;
    if (_sData)
        delete _sData;
    if (_style)
        delete _style;
    if (_imData) {
        //unsigned char* image = static_cast<unsigned char*>(_imData->data());
        //stbi_write_bmp("D://test.bmp", 256, 256, 3, image);
        delete _imData; //yzo
    }

    _Texture = 0x00;
    _cData = 0x00;
    _vData = 0x00;
    _hData = 0x00;
    _iData = 0x00;
    _tData = 0x00;
    _sData = 0x00;
    _style = 0x00;
    _imData = 0x00;
}

GLuint* TMBuffer::texture() const
{
    return _Texture;
}

Vertices* TMBuffer::cData() const
{
    return _cData;
}


Vertices* TMBuffer::vData() const
{
    return _vData;
}

Heights* TMBuffer::hData() const
{
    return _hData;
}

Index* TMBuffer::iData() const
{
    return _iData;
}

Text* TMBuffer::tData() const
{
    return _tData;
}

Stop* TMBuffer::sData() const
{
    return _sData;
}

Image* TMBuffer::imData() const
{
    return _imData;
}

TMStyle::CStyle* TMBuffer::style() const
{
    return _style;
}

void TMBuffer::setData(void* data, DataType type)
{
    if (data != 0x00)
    {
        switch (type)
        {
        case TEXTURE:
            _Texture = static_cast<GLuint*>(data);
            break;
        case COLOR:
            _cData = static_cast<Vertices*>(data);
            break;
        case VERTICE:
            //_vData = new Vertices(*static_cast<Vertices*>(data));
            _vData = static_cast<Vertices*>(data);
            break;
        case INDEX:
            _iData = static_cast<Index*>(data);
            break;
        case STOP:
            _sData = static_cast<Stop*>(data);
            //_sData = new Stop(*static_cast<Stop*>(data));
            break;
        case TEXT:
            _tData = static_cast<Text*>(data);
            break;
        case HEIGHT:
            _hData = static_cast<Heights*>(data);
            break;
        case IMAGE:
            _imData = static_cast<Image*>(data);
            break;
        default:
            break;
        }
    }
    else
    {
        int q = 0;
    }

}

void TMBuffer::setStyle(TMStyle::CStyle* style)
{
    _style = style;
}
