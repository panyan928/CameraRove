#include "CGeoLayer.h"

CGeoLayer::CGeoLayer()
{
    _style = 0x00;
    _isVisible = true;
}

CGeoLayer::~CGeoLayer()
{
    if (_style != 0x00)
        delete _style;
    _style = 0x00;
}

string CGeoLayer::layerName() const
{
    return _layerName;
}

void CGeoLayer::setLayerName(string name)
{
    _layerName = name;
}

string CGeoLayer::geometryType() const
{
    return _geometryType;
}

void CGeoLayer::setGeometryType(string type)
{
    this->_geometryType = type;
}

Color CGeoLayer::color() const
{
    return _color;
}

void CGeoLayer::setColor(Color color)
{
    _color = color;
}

Vec2i CGeoLayer::zoom() const
{
    return _zoom;
}

void CGeoLayer::setZoom(Vec2i zoom)
{
    memcpy(&_zoom, &zoom, sizeof(Vec2i));
}

CStyle* CGeoLayer::getOrCreateStyle()
{
    if (_style == 0x00)
        _style = new CStyle();
    return _style;
}
